
#include <TList.h>

#include "AliESDpid.h"
#include "../TOF/AliTOFT0maker.h"
#include "../TOF/AliTOFcalib.h"
#include "AliCDBManager.h"

#include "AliRsnCustomTrackInfo.h"

ClassImp(AliRsnCustomTrackInfo)

AliRsnCustomTrackInfo::AliRsnCustomTrackInfo(const char* name, const char* title): AliRsnVCustomObj(name, title),
fCharge(0),
fCutSet(0),
fRecalculateTOFPid(kFALSE),
fESDpid(0x0),
fTOFmaker(0x0),
fTOFcalib(0x0),
fTOFcalibrateESD(kFALSE),
fTOFcorrectTExp(kFALSE),
fTOFuseT0(kFALSE),
fTOFtuneMC(kFALSE),
fTOFresolution(0.0)
{

  for(Int_t i=0;i<kNumTypes;i++) {
    fUseParameter[i] = kFALSE;
    fParameter[i] = 0;
    fParameterBins[i] = 1;
    fParameterMin[i] = 0.0;
    fParameterMax[i] = 1.0;
  }
}

AliRsnCustomTrackInfo::~AliRsnCustomTrackInfo()
{
//   for(Int_t i=0;i<kNumTypes;i++) {
//     if (fParameter[i]) delete fParameter[i];
//   }
}


void AliRsnCustomTrackInfo::SetParamaterHistogram(AliRsnCustomTrackInfo::EType type, Int_t nBins, Double_t min, Double_t max)
{
  Int_t t = (Int_t) type;
  fUseParameter[t] = kTRUE;
  fParameterBins[t] = nBins;
  fParameterMin[t] = min;
  fParameterMax[t] = max;
}


void AliRsnCustomTrackInfo::UserCreateOutputCustom(TList* list)
{

  if (fRecalculateTOFPid) {
    // setup TPC response
    if (!fESDpid) fESDpid = new AliESDpid;
    fESDpid->GetTPCResponse().SetBetheBlochParameters(1.41543 / 50.0, 2.63394E1, 5.0411E-11, 2.12543, 4.88663);
    //   fESDpid->GetTPCResponse().SetBetheBlochParameters(fTPCpar[0], fTPCpar[1], fTPCpar[2], fTPCpar[3], fTPCpar[4]);
    
    // setup TOF maker & calibration
    if (!fTOFcalib) fTOFcalib = new AliTOFcalib;
    if (!fTOFmaker) fTOFmaker = new AliTOFT0maker(fESDpid, fTOFcalib);
    fTOFmaker->SetTimeResolution(fTOFresolution);
  }
  
  for(Int_t i=0;i<kNumTypes;i++) {
    if (fUseParameter[i]) {
      fParameter[i] = new TH1D(Form("h%s_%s",GetName(),GetParameterName(i)),Form("%s (%s)",GetTitle(),GetParameterName(i)),fParameterBins[i],fParameterMin[i],fParameterMax[i]);
      list->Add(fParameter[i]);
    }
  }
  
}
void AliRsnCustomTrackInfo::UserExecCustom(AliRsnEvent* ev1, AliRsnEvent* ev2)
{
  
  AliESDEvent *esd   = dynamic_cast<AliESDEvent*>(ev1->GetRefESD());
  
  if (fRecalculateTOFPid) {
    // TOF stuff #1: init OCDB
    Int_t run = esd->GetRunNumber();
    AliCDBManager *cdb = AliCDBManager::Instance();
    cdb->SetDefaultStorage("raw://");
    cdb->SetRun(run);
    // TOF stuff #2: init calibration
    fTOFcalib->SetCorrectTExp(fTOFcorrectTExp);
    fTOFcalib->Init();
    // TOF stuff #3: calibrate
    if (fTOFcalibrateESD) fTOFcalib->CalibrateESD(esd);
    if (fTOFtuneMC) fTOFmaker->TuneForMC(esd);
    if (fTOFuseT0) 
    {
      fTOFmaker->ComputeT0TOF(esd);
      fTOFmaker->ApplyT0TOF(esd);
      fESDpid->MakePID(esd, kFALSE, 0.);
    }
  }
  
  AliRsnDaughter daughter0;
  AliRsnDaughter::ERefType type0;
  Int_t index0;
  for(Int_t iTrack=0;iTrack<ev1->GetAbsoluteSum();iTrack++) {
    
    if (!ev1->ConvertAbsoluteIndex(iTrack, index0, type0)) continue;
    ev1->SetDaughter(daughter0, index0, type0);

    if (fCharge && daughter0.Charge() != fCharge) continue;
    
    if (fCutSet && !fCutSet->IsSelected(&daughter0)) continue;
    // TODO correct it
//     d = (AliRsnDaughter) ev1->GetDaughter(iTrack);
    for(Int_t iParam=0;iParam<kNumTypes;iParam++) {
      if (fUseParameter[iParam]) fParameter[iParam]->Fill(GetValue(iParam,&daughter0));
    }
  }
  
  if (ev2) return;
  
}
void AliRsnCustomTrackInfo::UserExecMixCustom(AliRsnEvent* ev1, AliRsnEvent* ev2)
{
    AliRsnVCustomObj::UserExecMixCustom(ev1, ev2);
}

const char* AliRsnCustomTrackInfo::GetParameterName(Int_t num)
{
  if (!IsInTypeRange(num)) return "error";
switch (num) {
  case kP : return "p";
  case kPt : return "pt";
  case kEta : return "eta";
  default : return "error";
  }
}
const char* AliRsnCustomTrackInfo::GetParameterTitle(Int_t num)
{
  if (!IsInTypeRange(num)) return "error";
  switch (num) {
    case kP : return "P";
    case kPt : return "P_{t}";
    case kEta : return "{eta}";
    default : return "error";
  }
}

Double_t AliRsnCustomTrackInfo::GetValue(Int_t num,AliRsnDaughter* d)
{
  if (!IsInTypeRange(num)) return 0.0;
  switch (num) {
    case kP : return d->GetRef()->P();
    case kPt : return d->GetRef()->Pt();
    case kEta : return d->GetRef()->Eta();
    default : AliError("Not Supported parameter");return 0.0;
  }
}

Bool_t AliRsnCustomTrackInfo::IsInTypeRange(Int_t num)
{
  if (num<0 || num>=kNumTypes) return kFALSE;
  
  return kTRUE;
}


