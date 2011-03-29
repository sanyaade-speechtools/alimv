#include <TEntryList.h>

#include "AliLog.h"
#include "AliAnalysisManager.h"

#include "AliRsnAnalysisTaskCommon.h"
#include "AliAODEvent.h"
#include "AliESDEvent.h"
#include "AliMultiInputEventHandler.h"
#include "AliMixInputEventHandler.h"
#include "AliRsnEvent.h"
#include "AliRsnInputHandler.h"

#include "AliMCEventHandler.h"

ClassImp(AliRsnAnalysisTaskCommon)

//________________________________________________________________________
AliRsnAnalysisTaskCommon::AliRsnAnalysisTaskCommon() // All data members should be initialised here
   : AliAnalysisTaskSE(),
     fOutput(0)
{
   // Dummy constructor ALWAYS needed for I/O.
}

//________________________________________________________________________
AliRsnAnalysisTaskCommon::AliRsnAnalysisTaskCommon(const char *name) // All data members should be initialised here
   : AliAnalysisTaskSE(name),
     fOutput(0)
{
   // Constructor
   // Define input and output slots here (never in the dummy constructor)
   // Input slot #0 works with a TChain - it is connected to the default input container
   // Output slot #1 writes into a TH1 container
   DefineOutput(1, TList::Class());                                            // for output list
}

//________________________________________________________________________
AliRsnAnalysisTaskCommon::~AliRsnAnalysisTaskCommon()
{
   // Destructor. Clean-up the output list, but not the histograms that are put inside
   // (the list is owner and will clean-up these histograms). Protect in PROOF case.
   if (fOutput && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
      delete fOutput;
   }
}

//________________________________________________________________________
void AliRsnAnalysisTaskCommon::UserCreateOutputObjects()
{
   // Create histograms
   // Called once (on the worker node)

   // sets all Inuput Handler pointers
   InitInputHandlers();

   fOutput = new TList();
   fOutput->SetOwner();  // IMPORTANT!

   // NEW HISTO added to fOutput here
   PostData(1, fOutput); // Post data for ALL output slots >0 here, to get at least an empty histogram
}

//________________________________________________________________________
void AliRsnAnalysisTaskCommon::UserExec(Option_t *)
{
   // Main loop
   // Called for each event

   // gets first input handler form mixing buffer
//    AliMultiInputEventHandler *ihMultiMix = dynamic_cast<AliMultiInputEventHandler*>(fInputEHMix->InputEventHandler(0));

   Int_t idRsnHandler = 1;
   if (dynamic_cast<AliMCEventHandler*>(fInputEHMain->GetFirstMCEventHandler())) idRsnHandler++;

   AliRsnInputHandler *rsnIH = dynamic_cast<AliRsnInputHandler*>(fInputEHMain->InputEventHandler(idRsnHandler));
//    AliRsnInputHandler *rsnMixIH = dynamic_cast<AliRsnInputHandler*>(ihMultiMix->InputEventHandler(idRsnHandler));

//    AliDebug(AliLog::kDebug,Form("RSN Pointers main=%p mix=%p", rsnIH, rsnMixIH));

   AliRsnEvent *evMain = rsnIH->GetRsnEvent();
//    AliRsnEvent *evMix = rsnMixIH->GetRsnEvent();

   if (evMain->GetRefESD()) {
      AliDebug(AliLog::kDebug, Form("RSN [ESD] main=%d(%lld,%lld)",
                                    evMain->GetRefESD()->GetNumberOfTracks(),
                                    rsnIH->GetPIDManager()->GetParticles(0, AliPID::kKaon)->GetN(),
                                    rsnIH->GetPIDManager()->GetParticles(1, AliPID::kKaon)->GetN()
                                   ));
   }
   PostData(1, fOutput);
}

void AliRsnAnalysisTaskCommon::UserExecMix(Option_t*)
{

   // gets first input handler form mixing buffer
   AliMultiInputEventHandler *ihMultiMix = dynamic_cast<AliMultiInputEventHandler*>(fInputEHMix->InputEventHandler(0));

   Int_t idRsnHandler = 1;
   if (dynamic_cast<AliMCEventHandler*>(fInputEHMain->GetFirstMCEventHandler())) idRsnHandler++;

   AliRsnInputHandler *rsnIH = dynamic_cast<AliRsnInputHandler*>(fInputEHMain->InputEventHandler(idRsnHandler));
   AliRsnInputHandler *rsnMixIH = dynamic_cast<AliRsnInputHandler*>(ihMultiMix->InputEventHandler(idRsnHandler));

//    AliDebug(AliLog::kDebug,Form("RSN Pointers main=%p mix=%p", rsnIH, rsnMixIH));

   AliRsnEvent *evMain = rsnIH->GetRsnEvent();
   AliRsnEvent *evMix = rsnMixIH->GetRsnEvent();

   if (evMain->GetRefESD()) {
      AliDebug(AliLog::kDebug, Form("RSN [ESD] main=%d(%lld,%lld) mix=%d(%lld,%lld)",
                                    evMain->GetRefESD()->GetNumberOfTracks(),
                                    rsnIH->GetPIDManager()->GetParticles(0, AliPID::kKaon)->GetN(),
                                    rsnIH->GetPIDManager()->GetParticles(1, AliPID::kKaon)->GetN(),
                                    evMix->GetRefESD()->GetNumberOfTracks(),
                                    rsnMixIH->GetPIDManager()->GetParticles(0, AliPID::kKaon)->GetN(),
                                    rsnMixIH->GetPIDManager()->GetParticles(1, AliPID::kKaon)->GetN()
                                   ));
      if (evMain->GetRefMCESD())
         AliDebug(AliLog::kDebug, Form("RSN [ESD MC] main=%d mix=%d", evMain->GetRefMCESD()->GetNumberOfTracks(), evMix->GetRefMCESD()->GetNumberOfTracks()));
   } else if (evMain->GetRefAOD()) {
      AliDebug(AliLog::kDebug, Form("RSN [AOD] main=%d(%lld,%lld) mix=%d(%lld,%lld)",
                                    evMain->GetRefAOD()->GetNumberOfTracks(),
                                    rsnIH->GetPIDManager()->GetParticles(0, AliPID::kKaon)->GetN(),
                                    rsnIH->GetPIDManager()->GetParticles(1, AliPID::kKaon)->GetN(),
                                    evMix->GetRefAOD()->GetNumberOfTracks(),
                                    rsnMixIH->GetPIDManager()->GetParticles(0, AliPID::kKaon)->GetN(),
                                    rsnMixIH->GetPIDManager()->GetParticles(1, AliPID::kKaon)->GetN()
                                   ));
      if (evMain->GetRefMCAOD())
         AliDebug(AliLog::kDebug, Form("RSN [ESD MC] main=%d mix=%d", evMain->GetRefMCAOD()->GetNumberOfTracks(), evMix->GetRefMCAOD()->GetNumberOfTracks()));
   }

   PostData(1, fOutput);
}

//________________________________________________________________________
void AliRsnAnalysisTaskCommon::Terminate(Option_t *)
{
   // Draw result to screen, or perform fitting, normalizations
   // Called once at the end of the query

   fOutput = dynamic_cast<TList*>(GetOutputData(1));
   if (!fOutput) { AliError("Could not retrieve TList fOutput"); return; }

}

//_____________________________________________________________________________
void AliRsnAnalysisTaskCommon::InitInputHandlers()
{
   //
   // Sets needed input handlers
   //
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   fInputEHMain = dynamic_cast<AliMultiInputEventHandler *>(mgr->GetInputEventHandler());
   if (fInputEHMain) {
      fInputEHMix = dynamic_cast<AliMixInputEventHandler *>(fInputEHMain->GetFirstMultiInputHandler());
   }
}
