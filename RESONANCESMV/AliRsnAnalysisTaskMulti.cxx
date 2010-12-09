#include <TList.h>

#include "AliESDEvent.h"
#include "AliMCEvent.h"
#include "AliAnalysisManager.h"

#include "AliMCEventHandler.h"
#include "AliMixInputEventHandler.h"

#include "AliRsnAnalysisTaskMulti.h"
#include <AliMixInfo.h>
#include <AliMixEventPool.h>

ClassImp(AliRsnAnalysisTaskMulti)

//________________________________________________________________________
AliRsnAnalysisTaskMulti::AliRsnAnalysisTaskMulti(const char *name)
    : AliAnalysisTaskMulti(name),
      fOutputList(0)
{
    // Constructor
    DefineOutput(1, TList::Class());
}

//________________________________________________________________________
AliRsnAnalysisTaskMulti::~AliRsnAnalysisTaskMulti()
{
    // Destructor
    if(fOutputList && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) delete fOutputList;
}

void AliRsnAnalysisTaskMulti::LocalInit()
{
    AliAnalysisTask::LocalInit();
}


//________________________________________________________________________
void AliRsnAnalysisTaskMulti::UserCreateOutputObjects()
{
    // Create histograms
    // Called once
    SetDebugForAllClasses();
    fOutputList = new TList();
    // Post output data.
    PostData(1, fOutputList);
}

//________________________________________________________________________
void AliRsnAnalysisTaskMulti::UserExec(Option_t *)
{
    // Main loop
    // Called for each event
    // Post output data.
    PostData(1, fOutputList);
}

void AliRsnAnalysisTaskMulti::UserExecMix(Option_t *)
{
    // Main loop
    // Called for each event
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    AliMultiInputEventHandler *inEvHMain = dynamic_cast<AliMultiInputEventHandler *>(mgr->GetInputEventHandler());
    if(inEvHMain) {
        //         AliMixEventInputHandler *mixEH = dynamic_cast<AliMixEventInputHandler *>(inEvHMain->MixingHandler());
        AliMixInputEventHandler *mixEH = dynamic_cast<AliMixInputEventHandler *>(inEvHMain->GetFirstMultiInputHandler());
        if(mixEH) {
            AliInfo(Form("Mixed Event available %d", mixEH->NumberMixedTimes()));
            AliInputEventHandler *inEvHMainTmp = dynamic_cast<AliInputEventHandler *>(inEvHMain->GetFirstInputEventHandler());
            AliMCEventHandler *inEvHMCTmp = dynamic_cast<AliMCEventHandler *>(inEvHMain->GetFirstMCEventHandler());
            //             AliInputEventHandler *inEvHMainTmp = dynamic_cast<AliInputEventHandler *>(inEvHMain);
            if(!inEvHMainTmp) return;
            AliESDEvent *event = dynamic_cast<AliESDEvent *>(inEvHMainTmp->GetEvent());
            AliMCEvent *mcMain = 0;
            if(inEvHMCTmp) mcMain = inEvHMCTmp->MCEvent();
            if(!event) {
                AliError("Could not retrieve event");
                return;
            } else {
                AliInfo("**Main Event");
                AliInfo(Form("  ESD Multi %d", event->GetNumberOfTracks()));
                AliInfo(Form("  ESD Vz %f", event->GetVertex()->GetZ()))
                if(mcMain) {
                    AliInfo(Form("  MC  Multi %d", mcMain->GetNumberOfTracks()));
                    AliInfo(Form("  MC  Vz %f", mcMain->GetPrimaryVertex()->GetZ()))
                }
            }
            AliMultiInputEventHandler *inEvHMainTmpMix = dynamic_cast<AliMultiInputEventHandler *>(mixEH->GetFirstMultiInputHandler());
            if(!inEvHMainTmpMix) return;
            AliInputEventHandler *inEvHMixTmp = dynamic_cast<AliInputEventHandler *>(inEvHMainTmpMix->GetFirstInputEventHandler());
            AliMCEventHandler *inEvHMCMixTmp = dynamic_cast<AliMCEventHandler *>(inEvHMainTmpMix->GetFirstMCEventHandler());
            AliESDEvent *eventMix=0;
            if(inEvHMixTmp) eventMix = dynamic_cast<AliESDEvent *>(inEvHMixTmp->GetEvent());
            AliMCEvent *mcMix = 0;
            if(inEvHMCMixTmp) mcMix = inEvHMCMixTmp->MCEvent();
            if(!eventMix) {
                AliError("Could not retrieve mixed event");
                return;
            } else {
                AliInfo(Form("**Mixed Event %d", 0));
                AliInfo(Form("  ESDMIX%d Multi %d", 0, eventMix->GetNumberOfTracks()));
                AliInfo(Form("  ESDMIX%d Vz %f", 0, eventMix->GetVertex()->GetZ()))
                if(mcMix) {
                    AliInfo(Form("  MC  Multi %d", mcMix->GetNumberOfTracks()));
                    AliInfo(Form("  MC  Vz %f", mcMix->GetPrimaryVertex()->GetZ()))
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
void AliRsnAnalysisTaskMulti::Terminate(Option_t *)
{
    // Draw result to the screen
    // Called once at the end of the query
    fOutputList = dynamic_cast<TList *>(GetOutputData(1));
    if(!fOutputList) {
        AliError("fOutputList not available");
        return;
    }
    fOutputList->Print();
}

//_____________________________________________________________________________
void AliRsnAnalysisTaskMulti::SetLogType(AliLog::EType_t type, TString allClasses)
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
void AliRsnAnalysisTaskMulti::SetDebugForAllClasses()
{
    //
    // Set debug level for all classes for which it is required
    //
    AliDebug(AliLog::kDebug + 10, "<-");
    TObjArray *array = fLogClassesString.Tokenize(":");
    TObjString *str;
    TString strr;
    for(Int_t i = 0; i < array->GetEntriesFast(); i++) {
        str = (TObjString *) array->At(i);
        strr = str->GetString();
        AliLog::SetClassDebugLevel(strr.Data(), fLogType);
        AliDebug(AliLog::kDebug + 5, Form("Setting Debug level %d to %s ...", (Int_t)fLogType - AliLog::kDebug, strr.Data()));
    }
    AliDebug(AliLog::kDebug + 10, "->");
}

