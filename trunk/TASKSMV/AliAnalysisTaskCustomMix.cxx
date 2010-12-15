#include <TList.h>

#include "AliESDEvent.h"
#include "AliMCEvent.h"
#include "AliAnalysisManager.h"

#include "AliMCEventHandler.h"
#include "AliMixInputEventHandler.h"
#include "AliMixInfo.h"
#include "AliMixEventPool.h"

#include "AliAnalysisTaskCustomMix.h"

ClassImp(AliAnalysisTaskCustomMix)

//________________________________________________________________________
AliAnalysisTaskCustomMix::AliAnalysisTaskCustomMix(const char *name)
   : AliAnalysisTaskMulti(name),
     fOutputList(0)
{
   // Constructor
   DefineOutput(1, TList::Class());
}

//________________________________________________________________________
AliAnalysisTaskCustomMix::~AliAnalysisTaskCustomMix()
{
   // Destructor
   if (fOutputList && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) delete fOutputList;
}

void AliAnalysisTaskCustomMix::LocalInit()
{
   AliAnalysisTask::LocalInit();
}


//________________________________________________________________________
void AliAnalysisTaskCustomMix::UserCreateOutputObjects()
{
   // Create histograms
   // Called once
   SetDebugForAllClasses();
   fOutputList = new TList();
   // Post output data.
   PostData(1, fOutputList);
}

//________________________________________________________________________
void AliAnalysisTaskCustomMix::UserExec(Option_t *)
{
   // Main loop
   // Called for each event
   // Post output data.
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   AliMultiInputEventHandler *inEvHMain = dynamic_cast<AliMultiInputEventHandler *>(mgr->GetInputEventHandler());
   AliInputEventHandler *inEvHMainTmp = dynamic_cast<AliInputEventHandler *>(inEvHMain->GetFirstInputEventHandler());
   AliMCEventHandler *inEvHMCTmp = dynamic_cast<AliMCEventHandler *>(inEvHMain->GetFirstMCEventHandler());
   //             AliInputEventHandler *inEvHMainTmp = dynamic_cast<AliInputEventHandler *>(inEvHMain);
   if (!inEvHMainTmp) return;
   AliESDEvent *event = dynamic_cast<AliESDEvent *>(inEvHMainTmp->GetEvent());
   AliMCEvent *mcMain = 0;
   if (inEvHMCTmp) mcMain = inEvHMCTmp->MCEvent();
   if (!event) {
      AliError("Could not retrieve event");
      return;
   } else {
      AliDebug(AliLog::kDebug + 1, Form("---------------- Main Event %d", fEntry));
      AliDebug(AliLog::kDebug + 1, Form("  ESD Multi %d", event->GetNumberOfTracks()));
      AliDebug(AliLog::kDebug + 1, Form("  ESD Vz %f", event->GetVertex()->GetZ()));
      if (mcMain) {
         AliDebug(AliLog::kDebug + 1, Form("  MC  Multi %d", mcMain->GetNumberOfTracks()));
         AliDebug(AliLog::kDebug + 1, Form("  MC  Vz %f", mcMain->GetPrimaryVertex()->GetZ()));
      }
   }
   PostData(1, fOutputList);
}

void AliAnalysisTaskCustomMix::UserExecMix(Option_t *)
{
   // Main loop
   // Called for each event
   if (CurrentEntryMix() < 0) {
      AliError("Mix entru is -1 and it should not happen !!!!!");
      return ;
   }

   AliDebug(AliLog::kDebug, Form("Mixing %lld %d [%lld,%lld] %d", CurrentEntry(), NumberMixed(), CurrentEntryMain(), CurrentEntryMix(), CurrentBinIndex()));
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   AliMultiInputEventHandler *inEvHMain = dynamic_cast<AliMultiInputEventHandler *>(mgr->GetInputEventHandler());
   if (inEvHMain) {
      AliInputEventHandler *inEvHMainTmp = dynamic_cast<AliInputEventHandler *>(inEvHMain->GetFirstInputEventHandler());
      AliMCEventHandler *inEvHMCTmp = dynamic_cast<AliMCEventHandler *>(inEvHMain->GetFirstMCEventHandler());
      //             AliInputEventHandler *inEvHMainTmp = dynamic_cast<AliInputEventHandler *>(inEvHMain);
      if (!inEvHMainTmp) return;
      AliESDEvent *event = dynamic_cast<AliESDEvent *>(inEvHMainTmp->GetEvent());
      AliMCEvent *mcMain = 0;
      if (inEvHMCTmp) mcMain = inEvHMCTmp->MCEvent();
      if (!event) {
         AliError("Could not retrieve event");
         return;
      } else {
         AliDebug(AliLog::kDebug, "**Main Event");
         AliDebug(AliLog::kDebug, Form("  ESD Multi %d", event->GetNumberOfTracks()));
         AliDebug(AliLog::kDebug, Form("  ESD Vz %f", event->GetVertex()->GetZ()));
         if (mcMain) {
            AliDebug(AliLog::kDebug, Form("  MC  Multi %d", mcMain->GetNumberOfTracks()));
            AliDebug(AliLog::kDebug, Form("  MC  Vz %f", mcMain->GetPrimaryVertex()->GetZ()));
         }
      }
      //         AliMixEventInputHandler *mixEH = dynamic_cast<AliMixEventInputHandler *>(inEvHMain->MixingHandler());
      AliMixInputEventHandler *mixEH = dynamic_cast<AliMixInputEventHandler *>(inEvHMain->GetFirstMultiInputHandler());
      if (mixEH) {
         AliDebug(AliLog::kDebug, Form("Mixed Event available %d", mixEH->NumberMixedTimes()));
         Int_t num = mixEH->NumberMixedTimes();
         if (mixEH->BufferSize() < 2)  num = 0;
         AliMultiInputEventHandler *inEvHMainTmpMix = dynamic_cast<AliMultiInputEventHandler *>(mixEH->InputEventHandler(num));
         if (!inEvHMainTmpMix) return;
         AliInputEventHandler *inEvHMixTmp = dynamic_cast<AliInputEventHandler *>(inEvHMainTmpMix->GetFirstInputEventHandler());
         AliMCEventHandler *inEvHMCMixTmp = dynamic_cast<AliMCEventHandler *>(inEvHMainTmpMix->GetFirstMCEventHandler());
         AliESDEvent *eventMix = 0;
         if (inEvHMixTmp) eventMix = dynamic_cast<AliESDEvent *>(inEvHMixTmp->GetEvent());
         AliMCEvent *mcMix = 0;
         if (inEvHMCMixTmp) mcMix = inEvHMCMixTmp->MCEvent();
         if (!eventMix) {
            AliError("Could not retrieve mixed event");
            return;
         } else {
            AliDebug(AliLog::kDebug, Form("**Mixed Event %d", num));
            AliDebug(AliLog::kDebug, Form("  ESDMIX(%d) Multi %d", num, eventMix->GetNumberOfTracks()));
            AliDebug(AliLog::kDebug, Form("  ESDMIX(%d) Vz %f", num, eventMix->GetVertex()->GetZ()));
            if (mcMix) {
               AliDebug(AliLog::kDebug, Form("  MC  Multi %d", mcMix->GetNumberOfTracks()));
               AliDebug(AliLog::kDebug, Form("  MC  Vz %f", mcMix->GetPrimaryVertex()->GetZ()));
            }
         }
      } else {
         AliError("No mixEH");
      }
   } else {
      AliError("No inEvHMain");
   }
   // Post output data.
   PostData(1, fOutputList);
}


//________________________________________________________________________
void AliAnalysisTaskCustomMix::Terminate(Option_t *)
{
   // Draw result to the screen
   // Called once at the end of the query
   fOutputList = dynamic_cast<TList *>(GetOutputData(1));
   if (!fOutputList) {
      AliError("fOutputList not available");
      return;
   }
//     fOutputList->Print();
}

//_____________________________________________________________________________
void AliAnalysisTaskCustomMix::SetLogType(AliLog::EType_t type, TString allClasses)
{
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
void AliAnalysisTaskCustomMix::SetDebugForAllClasses()
{
   //
   // Set debug level for all classes for which it is required
   //
   AliDebug(AliLog::kDebug + 10, "<-");
   TObjArray *array = fLogClassesString.Tokenize(":");
   TObjString *str;
   TString strr;
   for (Int_t i = 0; i < array->GetEntriesFast(); i++) {
      str = (TObjString *) array->At(i);
      strr = str->GetString();
      AliLog::SetClassDebugLevel(strr.Data(), fLogType);
      AliDebug(AliLog::kDebug + 5, Form("Setting Debug level %d to %s ...", (Int_t)fLogType - AliLog::kDebug, strr.Data()));
   }
   AliDebug(AliLog::kDebug + 10, "->");
}

