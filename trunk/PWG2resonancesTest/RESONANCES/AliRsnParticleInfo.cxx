//
// Class AliRsnParticleInfo
//
// Rsn output Particle Info
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//
#include "AliLog.h"
#include "AliMCEvent.h"
#include "AliRsnParticleInfo.h"

ClassImp(AliRsnParticleInfo)

//_________________________________________________________________________________________________
AliRsnParticleInfo::AliRsnParticleInfo() : TObject(),
fPDG(333), fNumberActiveOfParameters(0), fParameterArray(10),fCurrentList(0)
{
//
// Default constructor.
//
}

void AliRsnParticleInfo::Clear()
{
  fPDG = 333;
  fParameterArray.Reset();
}


void AliRsnParticleInfo::Print()
{
  AliInfo(Form("PDG \t%d ...", fPDG));
  Int_t i;
  for (i=0;i<fNumberActiveOfParameters;i++) {
    AliInfo(Form("Parameter \t%d ...", fParameterArray.At(i)));
  }
}


void AliRsnParticleInfo::AddHistogram(AliRsnParticleInfo::EParticleParameter parameter, Int_t nbins, Double_t min, Double_t max)
{

  TH1D *hist = new TH1D(Form("%d_%s",fPDG,GetParameterName(parameter).Data()),Form("%s(%d)",GetParameterTitle(parameter).Data(),fPDG),nbins,min,max);
  fHistogramList.AddAt(hist,fNumberActiveOfParameters);
  fParameterArray.AddAt(parameter, fNumberActiveOfParameters);
  fNumberActiveOfParameters++;
}


void AliRsnParticleInfo::InitHistograms(TList* list)
{
  
//   AliInfo("<-");
  TH1D *hist,*histOut;
  Int_t i;
  for (i=0;i<fNumberActiveOfParameters;i++) {
    hist  = (TH1D*) fHistogramList.At(i);
    histOut = new TH1D(hist->GetName(),hist->GetTitle(),hist->GetNbinsX(),hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
    list->Add(histOut);
  }
  
  fCurrentList = list;
//   AliInfo("->");
}


void AliRsnParticleInfo::ProcessHistograms(AliMCEvent* ev)
{
//   AliInfo("<-");
  TH1D *hist;
  Int_t i,pdg;
//   AliMCEvent* mcEvent = dynamic_cast<AliMCEvent*>(ev);
  AliMCEvent* mcEvent = ev;
  if (!mcEvent) {
    AliError("Could not retrieve MC event");
    return;
  }
  
  Int_t ntracks = mcEvent->GetNumberOfTracks();
  AliInfo(Form("MC particles: %d", ntracks));

  // Track loop to fill a pT spectrum
  for (Int_t iTrack = 0; iTrack < ntracks; iTrack++) {
    AliMCParticle *track = dynamic_cast<AliMCParticle*>(mcEvent->GetTrack(iTrack));
    if (!track) {
      AliError(Form("Could not receive track %d", iTrack));
      continue;
    }
    // TODO Fill
    pdg = track->PdgCode();
    if (pdg != fPDG) continue;
    AliInfo(Form("pdg is %d in track %d",pdg,iTrack));
    for (i=0;i<fNumberActiveOfParameters;i++) {
      hist  = (TH1D*) fCurrentList->At(i);
      //     histOut = new TH1D(hist->GetName(),hist->GetTitle(),hist->GetNbinsX(),hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
      //     list->Add(histOut);
      hist->Print();
      hist->Fill(GetParameterValue((EParticleParameter)i,track));
    }
    
    
  } //track loop

  
//   AliInfo("->");
}


TString AliRsnParticleInfo::GetParameterName(EParticleParameter parameter)
{
  switch (parameter) {
    case kP: return "p";
    case kPt: return "pt";
    case kEta: return "eta";
    default: return "error";
  }
  
  
}


TString AliRsnParticleInfo::GetParameterTitle(EParticleParameter parameter)
{
  switch (parameter) {
    case kP: return "p";
    case kPt: return "p_{t}";
    case kEta: return "#{eta}";
    default: return "error";
  }
}



Double_t AliRsnParticleInfo::GetParameterValue(EParticleParameter parameter,AliMCParticle *part)
{
  switch (parameter) {
    case kP: return part->P();
    case kPt: return part->Pt();
    case kEta: return part->Eta();
    default: return 10000.0;
  }
}
