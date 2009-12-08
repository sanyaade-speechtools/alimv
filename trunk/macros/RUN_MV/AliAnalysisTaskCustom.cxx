#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"

#include "AliVEvent.h"
#include "AliVParticle.h"

#include "AliESDEvent.h"
#include "AliESDtrack.h"

#include "AliMCEvent.h"
#include "AliMCParticle.h"
#include "AliAnalysisManager.h"

#include "AliAnalysisTaskCustom.h"

// example of an analysis task creating a p_t spectrum
// Authors: Panos Cristakoglou, Jan Fiete Grosse-Oetringhaus, Christian Klein-Boesing

ClassImp(AliAnalysisTaskCustom)

//________________________________________________________________________
AliAnalysisTaskCustom::AliAnalysisTaskCustom(const char *name)
  : AliAnalysisTaskSE(name), 
    fOutputList(0),
    fHistPtESD(0),
    fHistPtMC(0)
{
  // Constructor

  // Define input and output slots here
  // Input slot #0 defined by parent class
  // Output slot #0 reserved by parent class for AOD (not used)
  DefineOutput(1, TList::Class());
}

//________________________________________________________________________
void AliAnalysisTaskCustom::UserCreateOutputObjects()
{
  // Create histograms
  // Called once

  fOutputList = new TList();
  
  fHistPtESD = new TH1F("fHistPtESD", "P_{T} distribution ESD", 15, 0.1, 3.1);
  fHistPtESD->GetXaxis()->SetTitle("P_{T} (GeV/c)");
  fHistPtESD->GetYaxis()->SetTitle("dN/dP_{T} (c/GeV)");
  fHistPtESD->SetMarkerStyle(kFullCircle);
  fOutputList->Add(fHistPtESD);
  
  fHistPtMC = new TH1F("fHistPtMC", "P_{T} distribution MC", 15, 0.1, 3.1);
  fHistPtMC->GetXaxis()->SetTitle("P_{T} (GeV/c)");
  fHistPtMC->GetYaxis()->SetTitle("dN/dP_{T} (c/GeV)");
  fHistPtMC->SetMarkerStyle(kFullCircle);
  fOutputList->Add(fHistPtMC);
  
}

//________________________________________________________________________
void AliAnalysisTaskCustom::UserExec(Option_t *)
{
  // Main loop
  // Called for each event

  AliVEvent *event = InputEvent();
  if (!event) {
     Error("UserExec", "Could not retrieve event");
     return;
  }

  LoopESD();
//   LoopESDMC();

  // Post output data.
  PostData(1, fOutputList);

}      

//________________________________________________________________________
void AliAnalysisTaskCustom::Terminate(Option_t*)
{
  // Draw result to the screen
  // Called once at the end of the query

  fOutputList = dynamic_cast<TList*> (GetOutputData(1));
  if (!fOutputList) {
    Error("Terminate","fOutputList not available");
    return;
  }

  fHistPtESD = (TH1F*) fOutputList->FindObject("fHistPtESD");
  if (!fHistPtESD) {
    Error("Terminate","fHistPtESD not available");
    return;
  }
  
  fHistPtMC = (TH1F*) fOutputList->FindObject("fHistPtMC");
  if (!fHistPtMC) {
    Error("Terminate","fHistPtMC not available");
    return;
  }
   
  TCanvas *c1 = new TCanvas(GetName(),"Pt",10,10,700,350);
  c1->Divide(2,1);
  c1->cd(1)->SetLogy();
  fHistPtMC->DrawCopy("E");

  c1->cd(2)->SetLogy();
  fHistPtESD->DrawCopy("E");
}

//________________________________________________________________________
void AliAnalysisTaskCustom::LoopESD()
{
  
  AliESDEvent *esdEvent = dynamic_cast<AliESDEvent*> (InputEvent());
  if (!esdEvent) {
     Error("UserExec", "Could not retrieve esdEvent");
     return;
  }

  // Retreive the number of tracks for this event.
  Int_t ntracks = esdEvent->GetNumberOfTracks();
  Printf("ESD tracks: %d", ntracks);

  // Track loop to fill a pT spectrum
  for (Int_t iTracks = 0; iTracks < ntracks; iTracks++) {
    AliESDtrack *track = (AliESDtrack *)esdEvent->GetTrack(iTracks);
    if (!track) {
      Error("UserExec", "Could not receive track %d", iTracks);
      continue;
    }

    fHistPtESD->Fill(track->Pt());
  } //track loop 

}   

//________________________________________________________________________
void AliAnalysisTaskCustom::LoopESDMC()
{
  // Main loop
  // Called for each event

  AliMCEvent *mcEvent = (AliMCEvent*) MCEvent();
  if (!mcEvent) {
    Error("UserExec", "Could not retrieve MC event");
    return;
  }
  
  Int_t ntracks = mcEvent->GetNumberOfTracks();
  Printf("MC particles: %d", ntracks);

// Track loop to fill a pT spectrum
  for (Int_t iTrack = 0; iTrack < ntracks; iTrack++) {
    AliMCParticle *track = dynamic_cast<AliMCParticle*>(mcEvent->GetTrack(iTrack));
    if (!track) {
      Error("UserExec", "Could not receive track %d", iTrack);
      continue;
    }

    fHistPtMC->Fill(track->Pt());
  } //track loop
}   
