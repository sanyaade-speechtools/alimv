#include <TList.h>

#include "AliESDpid.h"
#include "../TOF/AliTOFT0maker.h"
#include "../TOF/AliTOFcalib.h"
#include "AliCDBManager.h"

#include "AliRsnCustomTrackInfoBat.h"

ClassImp(AliRsnCustomTrackInfoBat)

AliRsnCustomTrackInfoBat::AliRsnCustomTrackInfoBat(const char* name, const char* title): AliRsnCustomTrackInfo(name, title)
{
}

AliRsnCustomTrackInfoBat::~AliRsnCustomTrackInfoBat()
{
}


void AliRsnCustomTrackInfoBat::UserCreateOutputCustom(TList* list)
{
	
	AliRsnCustomTrackInfo::UserCreateOutputCustom(list);

  if (fRecalculateTOFPid) {
    // setup TPC response
    if (fESDpid) delete fESDpid;
    fESDpid = new AliESDpid;
    fESDpid->GetTPCResponse().SetBetheBlochParameters(1.41543 / 50.0, 2.63394E1, 5.0411E-11, 2.12543, 4.88663);
    //   fESDpid->GetTPCResponse().SetBetheBlochParameters(fTPCpar[0], fTPCpar[1], fTPCpar[2], fTPCpar[3], fTPCpar[4]);
    
    // setup TOF maker & calibration
    if (fTOFcalib) delete fTOFcalib;
    fTOFcalib = new AliTOFcalib;
    
    if (fTOFmaker) delete fTOFmaker;
    fTOFmaker = new AliTOFT0maker(fESDpid, fTOFcalib);
    fTOFmaker->SetTimeResolution(fTOFresolution);
  }
  
	// more histograms can be added here
	// fMyHist = TH1D(...)
	// list->Add(fMyHist);
	
}
void AliRsnCustomTrackInfoBat::UserExecCustom(AliRsnEvent* ev1, AliRsnEvent* ev2)
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
void AliRsnCustomTrackInfoBat::UserExecMixCustom(AliRsnEvent* ev1, AliRsnEvent* ev2)
{
    AliRsnVCustomObj::UserExecMixCustom(ev1, ev2);
}
