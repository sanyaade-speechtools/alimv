#include <TList.h>

#include "AliESDEvent.h"
#include "AliMCEvent.h"
#include "AliAnalysisManager.h"

#include "AliMCEventHandler.h"
#include "AliMixInputEventHandler.h"

#include "AliAnalysisTaskMixInfo.h"
#include "AliMixInfo.h"
#include "AliMixEventPool.h"

ClassImp(AliAnalysisTaskMixInfo)

//________________________________________________________________________
AliAnalysisTaskMixInfo::AliAnalysisTaskMixInfo(const char *name)
    : AliAnalysisTaskMulti(name),
      fOutputList(0),
      fMixInfo(0),
      fCurrentEntryTmp(-1)
{
    // Constructor
    DefineOutput(1, TList::Class());
}

//________________________________________________________________________
AliAnalysisTaskMixInfo::~AliAnalysisTaskMixInfo()
{
    // Destructor
    if(fOutputList && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) delete fOutputList;
}

void AliAnalysisTaskMixInfo::LocalInit()
{
//     SetDebugForAllClasses();
    AliAnalysisTask::LocalInit();
}


//________________________________________________________________________
void AliAnalysisTaskMixInfo::UserCreateOutputObjects()
{
    // Create histograms
    // Called once
//     AliLog::SetGlobalLogLevel(AliLog::kInfo);
    SetDebugForAllClasses();
    fOutputList = new TList();
    fOutputList->SetOwner(kTRUE);
    if(!fMixInfo) InitMixInfo();

    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    AliMultiInputEventHandler *inEvHMain = dynamic_cast<AliMultiInputEventHandler *>(mgr->GetInputEventHandler());
    if(inEvHMain) {
        AliMixInputEventHandler *mixEH = dynamic_cast<AliMixInputEventHandler *>(inEvHMain->GetFirstMultiInputHandler());
        if(mixEH) {
            AliMixEventPool *evPool = mixEH->GetEventPool();
            if (evPool) {
              evPool->SetBufferSize(mixEH->BufferSize());
              evPool->SetMixNumber(mixEH->MixNumber());
              fMixInfo->SetEventPool(evPool);
            }
        }
    }

    if(fMixInfo) fOutputList->Add(fMixInfo);
    
    // Post output data.
    PostData(1, fOutputList);
}



//________________________________________________________________________
void AliAnalysisTaskMixInfo::UserExec(Option_t *)
{
    // Main loop
    // Called for each event
//     if (fCurrentEntry != fCurrentEntryTmp) {
//       fCurrentEntryTmp = fCurrentEntry;
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    AliMultiInputEventHandler *inEvHMain = dynamic_cast<AliMultiInputEventHandler *>(mgr->GetInputEventHandler());
    AliMixInputEventHandler *mixEH = dynamic_cast<AliMixInputEventHandler *>(inEvHMain->GetFirstMultiInputHandler());
    if(fMixInfo) {
        if(mixEH->BufferSize()>1) {
            if(mixEH->NumberMixedTimes()>=mixEH->BufferSize())
                fMixInfo->FillHistogram(AliMixInfo::kMainEvents,CurrentBinIndex());
        } else {
            fMixInfo->FillHistogram(AliMixInfo::kMainEvents,CurrentBinIndex());
        }
    }
//     }
    if(CurrentEntryMix()>=0)
        AliDebug(AliLog::kDebug+1,Form("Main %lld %d [%lld,%lld] %d",CurrentEntry(), NumberMixed(),CurrentEntryMain(),CurrentEntryMix(),NumberMixed()));
    // Post output data.
    PostData(1, fOutputList);
}

void AliAnalysisTaskMixInfo::UserExecMix(Option_t *)
{
    if(fMixInfo) fMixInfo->FillHistogram(AliMixInfo::kMixedEvents,CurrentBinIndex());
    if(CurrentEntryMix()<0) { AliError("Mix entru is -1 and it should not happen !!!!!"); return ;}
    AliDebug(AliLog::kDebug,Form("Mixing %lld %d [%lld,%lld] %d",CurrentEntry(),NumberMixed(),CurrentEntryMain(),CurrentEntryMix(),CurrentBinIndex()));
    // Post output data.
    PostData(1, fOutputList);
}

void AliAnalysisTaskMixInfo::FinishTaskOutput()
{
    if(fMixInfo) fMixInfo->Print();
}


//________________________________________________________________________
void AliAnalysisTaskMixInfo::Terminate(Option_t *)
{
    // Draw result to the screen
    // Called once at the end of the query
    fOutputList = dynamic_cast<TList *>(GetOutputData(1));
    if(!fOutputList) {
        AliError("fOutputList not available");
        return;
    }
    fOutputList->Print();
    fMixInfo = (AliMixInfo *) fOutputList->FindObject("mixInfo");
    if(fMixInfo) {
      fMixInfo->Draw("HIST");
      AliMixEventPool *evPool = (AliMixEventPool *) fMixInfo->GetEventPool("mixEventPool");
      if(evPool) evPool->Print();
    }
}

//_____________________________________________________________________________
void AliAnalysisTaskMixInfo::SetLogType(AliLog::EType_t type, TString allClasses)
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
void AliAnalysisTaskMixInfo::SetDebugForAllClasses()
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


void AliAnalysisTaskMixInfo::InitMixInfo()
{
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    AliMultiInputEventHandler *inEvHMain = dynamic_cast<AliMultiInputEventHandler *>(mgr->GetInputEventHandler());
    if(inEvHMain) {
        //         AliMixEventInputHandler *mixEH = dynamic_cast<AliMixEventInputHandler *>(inEvHMain->MixingHandler());
        AliMixInputEventHandler *mixEH = dynamic_cast<AliMixInputEventHandler *>(inEvHMain->GetFirstMultiInputHandler());
        if(mixEH) {
            fMixInfo = new AliMixInfo("mixInfo","Mix title");
//             if(fMixInfo) fMixInfo->SetOutputList(fOutputList);
            AliMixEventPool *evPool = mixEH->GetEventPool();
            if(!evPool) {
                //             TList *list = new TList;
                if(fMixInfo) fMixInfo->CreateHistogram(AliMixInfo::kMainEvents,1,1,2);
                if(fMixInfo) fMixInfo->CreateHistogram(AliMixInfo::kMixedEvents,1,1,2);
            } else {
                if(evPool->NeedInit()) evPool->Init();
                Int_t num = evPool->GetListOfEntryLists()->GetEntriesFast();
                if(fMixInfo) fMixInfo->CreateHistogram(AliMixInfo::kMainEvents,num,1,num+1);
                if(fMixInfo) fMixInfo->CreateHistogram(AliMixInfo::kMixedEvents,num,1,num+1);
            }
        } else {
            AliError("No mixEH");
        }
    } else {
        AliError("No inEvHMain");
    }
}
