//
// Class AliRsnAnalysisME
//
// TODO
//
// authors: Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//          Martin Vala (martin.vala@cern.ch)
//
#include <Riostream.h>
#include "AliESDEvent.h"
#include "AliMCEvent.h"
#include "AliAODEvent.h"

#include "AliRsnCutSet.h"
#include "AliRsnVATProcessInfo.h"
#include "AliRsnParticleInfoSE.h"

ClassImp(AliRsnParticleInfoSE)

//_____________________________________________________________________________
AliRsnParticleInfoSE::AliRsnParticleInfoSE(const char *name, Int_t numOfOutputs) :
    AliRsnVAnalysisTaskSE(name, numOfOutputs)
{
//
// Default constructor.
//

  AliDebug(AliLog::kDebug + 2, "<-");
  for (Int_t i = 0;i < fNumberOfOutputs;i++) {
    DefineOutput(i + 2, TList::Class());
  }
  AliDebug(AliLog::kDebug + 2, "->");
}

//_____________________________________________________________________________
AliRsnParticleInfoSE::AliRsnParticleInfoSE(const AliRsnParticleInfoSE& copy) :
    AliRsnVAnalysisTaskSE(copy)
{
//
// Copy constructor.
//

  AliDebug(AliLog::kDebug + 2, "<-");
  AliDebug(AliLog::kDebug + 2, "->");
}

//_____________________________________________________________________________
void AliRsnParticleInfoSE::RsnUserCreateOutputObjects()
{
//
// Creation of output objects.
// These are created through the utility methods in the analysis manager,
// which produces a list of histograms for each specified set of pairs.
// Each of these lists is added to the main list of this task.
//

  AliDebug(AliLog::kDebug + 2, "<-");

  Int_t i;
  for (i = 1; i < kMaxNumberOfOutputs + 1; i++) {
    if (i <= fNumberOfOutputs + 1) OpenFile(i);
    fOutList[i] = new TList();
    fOutList[i]->SetOwner();
  }

  for (i = 0; i < fNumberOfOutputs; i++) {
    fParticleManager[i].InitAllParticleInfos(fOutList[i+1]);
  }

  AliDebug(AliLog::kDebug + 2, "->");
}

//_____________________________________________________________________________
void AliRsnParticleInfoSE::RsnUserExec(Option_t*)
{
//
// Execution of the analysis task.
// Recovers the input event and processes it with all included pair objects.
//

  AliDebug(AliLog::kDebug + 2, "<-");

  fTaskInfo.SetEventUsed(kFALSE);

  if (fESDEvent) {
    AliDebug(AliLog::kDebug + 1, Form("fESDEvent is %p", fESDEvent));
    AliDebug(AliLog::kDebug, Form("ESD tracks %d", fESDEvent->GetNumberOfTracks()));
  }
  if (fMCEvent) {
    AliDebug(AliLog::kDebug + 1, Form("fMCEvent is %p", fMCEvent));
    AliDebug(AliLog::kDebug, Form("MC tracks %d", fMCEvent->GetNumberOfTracks()));
  }
  if (fAODEventIn) {
    AliDebug(AliLog::kDebug + 1, Form("fAODEventIn is %p", fAODEventIn));
    AliDebug(AliLog::kDebug, Form("AOD(in) tracks %d", fAODEventIn->GetNumberOfTracks()));
  }
  if (fAODEventOut) {
    AliDebug(AliLog::kDebug + 1, Form("fAODEventOut if %p", fAODEventOut));
    AliDebug(AliLog::kDebug, Form("AOD(out) tracks %d", fAODEventOut->GetNumberOfTracks()));
  }

  // Removing empty events
  if (fRsnEvent.GetMultiplicity() <= 0) {
    AliDebug(AliLog::kDebug, "Zero event!!! Skipping ...");
    fTaskInfo.SetEventUsed(kFALSE);
    return;
  }

  // if cuts are passed or not cuts were defined,
  // update the task info...
  fTaskInfo.SetEventUsed(kTRUE);

  for (Int_t i = 0; i < fNumberOfOutputs; i++) {
//     if (!)
    fParticleManager[i].ProcessAllParticleInfos(fMCEvent);
    MCLoop(i);
    ESDLoop(i);
    PostData(i + 2, fOutList[i+1]);
  }
  
  AliDebug(AliLog::kDebug + 2, "->");
}


//_____________________________________________________________________________
void AliRsnParticleInfoSE::RsnTerminate(Option_t*)
{
//
// Termination.
// Could be added some monitor histograms here.
//

  AliDebug(AliLog::kDebug + 2, "<-");
  AliDebug(AliLog::kDebug + 2, "->");
}

//_____________________________________________________________________________
void AliRsnParticleInfoSE::MCLoop(Int_t outputIndex)
{
//
// MC Loop
//
  AliMCEvent* mcEvent = fMCEvent;
  if (!mcEvent) {
    Error("UserExec", "Could not retrieve MC event");
    return;
  }

  Int_t ntracks = mcEvent->GetNumberOfTracks();
  AliInfo(Form("MC particles: %d", ntracks));

// Track loop to fill a pT spectrum
  for (Int_t iTrack = 0; iTrack < ntracks; iTrack++) {
    AliMCParticle *track = dynamic_cast<AliMCParticle*>(mcEvent->GetTrack(iTrack));
    if (!track) {
      Error("UserExec", "Could not receive track %d", iTrack);
      continue;
    }
    // TODO Fill
  
  } //track loop
}

//_____________________________________________________________________________
void AliRsnParticleInfoSE::ESDLoop(Int_t outputIndex)
{
//
// ESD Loop
//

  AliESDEvent *esdEvent = fESDEvent;
  if (!esdEvent) {
    Error("UserExec", "Could not retrieve esdEvent");
    return;
  }

// Retreive the number of tracks for this event.
  Int_t ntracks = esdEvent->GetNumberOfTracks();
  AliInfo(Form("ESD particles: %d", ntracks));
  
  for (Int_t iTracks = 0; iTracks < ntracks; iTracks++) {
    AliESDtrack *track = (AliESDtrack *)esdEvent->GetTrack(iTracks);
    if (!track) {
      Error("UserExec", "Could not receive track %d", iTracks);
      continue;
    }

    // TODO Fill

  } //track loop

}

//_____________________________________________________________________________
AliRsnParticleInfoManager* AliRsnParticleInfoSE::GetParticleManager(Int_t index, TString name)
{
  //
  // Recovery the analysis manager
  //
  
  if (!name.IsNull())
  {
    SetParticleManagerName(name.Data(), index);
  }
  
  return &fParticleManager[index];
}

