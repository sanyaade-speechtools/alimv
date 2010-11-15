#include <TChain.h>
#include <TList.h>
#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>

#include "AliVEvent.h"
#include "AliVParticle.h"

#include "AliESDEvent.h"
#include "AliESDtrack.h"

#include "AliAODEvent.h"
#include "AliAODTrack.h"

#include "AliMCEvent.h"
#include "AliMCParticle.h"
#include "AliAnalysisManager.h"

#include "AliAnalysisTaskCustomMix.h"
#include "AliMixEventInputHandler.h"
#include "AliESDInputHandlerRP.h"
#include "AliMixEventPool.h"

ClassImp(AliAnalysisTaskCustomMix)

//________________________________________________________________________
AliAnalysisTaskCustomMix::AliAnalysisTaskCustomMix(const char *name)
    : AliAnalysisTaskSE(name),
    fUseMC(kFALSE),
    fMyESDEvent(0),
    fMyAODEvent(0),
    fOutputList(0),
    fHistPt(0),
    fHistPtMC(0) {
  // Constructor

  DefineOutput(1, TList::Class());
}

//________________________________________________________________________
AliAnalysisTaskCustomMix::~AliAnalysisTaskCustomMix() {
  // Destructor

  if (fOutputList && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) delete fOutputList;
}

//________________________________________________________________________
void AliAnalysisTaskCustomMix::UserCreateOutputObjects() {
  // Create histograms
  // Called once
  SetDebugForAllClasses();
  fOutputList = new TList();

  fHistPt = new TH1F("fHistPt", "P_{T} distribution REC", 15, 0.1, 3.1);
  fHistPt->GetXaxis()->SetTitle("P_{T} (GeV/c)");
  fHistPt->GetYaxis()->SetTitle("dN/dP_{T} (c/GeV)");
  fHistPt->SetMarkerStyle(kFullCircle);
  fOutputList->Add(fHistPt);

  if (fUseMC) {
    fHistPtMC = new TH1F("fHistPtMC", "P_{T} distribution MC", 15, 0.1, 3.1);
    fHistPtMC->GetXaxis()->SetTitle("P_{T} (GeV/c)");
    fHistPtMC->GetYaxis()->SetTitle("dN/dP_{T} (c/GeV)");
    fHistPtMC->SetMarkerStyle(kFullCircle);
    fOutputList->Add(fHistPtMC);
  }

  // Post output data.
  PostData(1, fOutputList);
}

//________________________________________________________________________
void AliAnalysisTaskCustomMix::UserExec(Option_t *) {
  // Main loop
  // Called for each event


  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  AliESDInputHandler *inEvHMain = dynamic_cast<AliESDInputHandler *>(mgr->GetInputEventHandler());
//   if (inEvHMain) {
// 
//     AliMixEventInputHandler*mixEH = dynamic_cast<AliMixEventInputHandler *>(inEvHMain->MixingHandler());
//     if (mixEH) {
//       mixEH->GetEntry();
// //       mixEH->Notify(mixEH->InputEventHandler(0)->GetTree()->GetCurrentFile()->GetName());
// //       mixEH->Init("proof");
//     } else {
//       AliError("mixED is null");
//     }
// 
//   }



//   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
//   AliInputEventHandler *inEvHMain = dynamic_cast<AliInputEventHandler *>(mgr->GetInputEventHandler());
  if (inEvHMain) {

    AliMixEventInputHandler*mixEH = dynamic_cast<AliMixEventInputHandler *>(inEvHMain->MixingHandler());
    if (mixEH) {
      AliInfo(Form("num mixed %d", mixEH->MixedEventNumber()));
      AliESDEvent *event = dynamic_cast<AliESDEvent*>(inEvHMain->GetEvent());
      if (!event) {
        AliError("Could not retrieve event");
        return;
      } else {
        AliInfo("**Main Event");
        AliInfo(Form("  ESD Multi %d", event->GetNumberOfTracks()));
        AliInfo(Form("  ESD Vz %f", event->GetVertex()->GetZ()))
      }
      if (mixEH->MixedEventNumber() > 0) {
        for (Int_t i = 0;i < mixEH->BufferSize();i++) {
          AliESDEvent *eventMix = dynamic_cast<AliESDEvent*>(mixEH->InputEventHandler(i)->GetEvent());
          if (!eventMix) {
            AliError("Could not retrieve event");
            return;
          } else {
            AliInfo(Form("**Mixed Event %d", i));
            AliInfo(Form("  ESDMIX%d Multi %d", i, eventMix->GetNumberOfTracks()));
            AliInfo(Form("  ESDMIX%d Vz %f", i, eventMix->GetVertex()->GetZ()))
          }
        }
      } else {
        AliWarning("No events to mix found");
      }

    }

  }

//     AliVEvent *event = InputEvent();
//     if (!event) {
//       AliError("Could not retrieve event");
//       return;
//     }

  Bool_t doPt = kFALSE;
  if (doPt) {
    fMyESDEvent = dynamic_cast<AliESDEvent*>(InputEvent());
    if (fMyESDEvent) {
      LoopESD();
      if (fUseMC)
        LoopESDMC();
    }

    fMyAODEvent = dynamic_cast<AliAODEvent*>(InputEvent());
    if (fMyAODEvent) {
      LoopAOD();
      if (fUseMC)
        LoopAODMC();
    }

  }

  // Post output data.
  PostData(1, fOutputList);

}

//________________________________________________________________________
void AliAnalysisTaskCustomMix::Terminate(Option_t*) {
  // Draw result to the screen
  // Called once at the end of the query

  fOutputList = dynamic_cast<TList*>(GetOutputData(1));
  if (!fOutputList) {
    AliError("fOutputList not available");
    return;
  }

  fHistPt = (TH1F*) fOutputList->FindObject("fHistPt");
  if (!fHistPt) {
    AliError("fHistPt not available");
    return;
  }

  Int_t pixelX = 350;
  if (fUseMC) pixelX = 700;

  TCanvas *c1 = new TCanvas(GetName(), "Pt", 10, 10, pixelX, 350);
  if (fUseMC) c1->Divide(2, 1);

  c1->cd(1)->SetLogy();
  if (fHistPt)
    fHistPt->DrawCopy("E");

  if (fUseMC) {
    fHistPtMC = (TH1F*) fOutputList->FindObject("fHistPtMC");
    if (!fHistPtMC) {
      AliError("fHistPtMC not available");
      return;
    }
    c1->cd(2)->SetLogy();
    fHistPtMC->DrawCopy("E");
  }
}

//________________________________________________________________________
void AliAnalysisTaskCustomMix::LoopESD() {

  // Retreive the number of tracks for this event.
  Int_t ntracks = fMyESDEvent->GetNumberOfTracks();
  AliInfo(Form("ESD tracks: %d", ntracks));

  // Track loop to fill a pT spectrum
  for (Int_t iTracks = 0; iTracks < ntracks; iTracks++) {
    AliESDtrack *track = (AliESDtrack *)fMyESDEvent->GetTrack(iTracks);
    if (!track) {
      AliError(Form("Could not receive track %d", iTracks));
      continue;
    }

    fHistPt->Fill(track->Pt());
  } //track loop

}

//________________________________________________________________________
void AliAnalysisTaskCustomMix::LoopESDMC() {
  // Main loop
  // Called for each event

  AliMCEvent *mcEvent = (AliMCEvent*) MCEvent();
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

    fHistPtMC->Fill(track->Pt());
  } //track loop
}

//________________________________________________________________________
void AliAnalysisTaskCustomMix::LoopAOD() {

  // Retreive the number of tracks for this event.
  Int_t ntracks = fMyAODEvent->GetNumberOfTracks();
  AliInfo(Form("AOD tracks: %d", ntracks));

  // Track loop to fill a pT spectrum
  for (Int_t iTracks = 0; iTracks < ntracks; iTracks++) {
    AliAODTrack *track = (AliAODTrack *)fMyAODEvent->GetTrack(iTracks);
    if (!track) {
      AliError(Form("Could not receive track %d", iTracks));
      continue;
    }

    fHistPt->Fill(track->Pt());
  } //track loop

}

//________________________________________________________________________
void AliAnalysisTaskCustomMix::LoopAODMC() {
  // Main loop
  // Called for each event

  AliWarning("Not implemented yet !!!")
  return;
//   AliMCEvent *mcEvent = (AliMCEvent*) MCEvent();
//   if (!mcEvent) {
//     AliError("Could not retrieve MC event");
//     return;
//   }
//
//   Int_t ntracks = mcEvent->GetNumberOfTracks();
//   AliInfo(Form("MC particles: %d", ntracks));
//
// // Track loop to fill a pT spectrum
//   for (Int_t iTrack = 0; iTrack < ntracks; iTrack++) {
//     AliMCParticle *track = dynamic_cast<AliMCParticle*>(mcEvent->GetTrack(iTrack));
//     if (!track) {
//       AliError("Could not receive track %d", iTrack);
//       continue;
//     }
//
//     fHistPtMC->Fill(track->Pt());
//   } //track loop
}

//_____________________________________________________________________________
void AliAnalysisTaskCustomMix::SetLogType(AliLog::EType_t type, TString allClasses) {
  //
  // Set Log level for this and other classes (list of their names)
  //

  AliDebug(AliLog::kDebug + 10, "<-");
  fLogType = type;
  fLogClassesString = allClasses;
  SetDebugForAllClasses();
  AliDebug(AliLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
void AliAnalysisTaskCustomMix::SetDebugForAllClasses() {
  //
  // Set debug level for all classes for which it is required
  //

  AliDebug(AliLog::kDebug + 10, "<-");
  TObjArray* array = fLogClassesString.Tokenize(":");
  TObjString *str;
  TString strr;
  for (Int_t i = 0;i < array->GetEntriesFast();i++) {
    str = (TObjString *) array->At(i);
    strr = str->GetString();
    AliLog::SetClassDebugLevel(strr.Data(), fLogType);
    AliDebug(AliLog::kDebug + 5, Form("Setting Debug level %d to %s ...", (Int_t)fLogType - AliLog::kDebug, strr.Data()));
  }
  AliDebug(AliLog::kDebug + 10, "->");
}

