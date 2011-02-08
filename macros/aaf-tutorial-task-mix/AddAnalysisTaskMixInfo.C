#include <AliAnalysisManager.h> //|
#include <AliLog.h> //|
#include "AliAnalysisTaskMixInfo.h" //|
void AddAnalysisTaskMixInfo(TString format = "esd", Bool_t useMC = kFALSE, TString opts = "")
{
   // create manager
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) return;

   // create our task
   AliAnalysisTaskMixInfo *task = new AliAnalysisTaskMixInfo("AliAnalysisTaskMixInfo");
//   task->SetMixingDebug(AliLog::kDebug);

   Int_t debugLevel = 0;
   TString myclasses = "";
   myclasses += ":AliAnalysisTaskMixInfo";
//       myclasses+=":AliAnalysisTaskCustomMix";
//       myclasses+=":AliMixInputEventHandler";
//       myclasses+=":AliMultiInputEventHandler";
//       myclasses+=":AliMixEventPool";
//         myclasses+=":AliMixEventCutObj";
//       myclasses+=":AliMixInputHandlerInfo";
//       myclasses+=":AliRsnAnalysisMulti";
//       myclasses+=":AliRsnPair";

   if (!myclasses.IsNull()) task->SetLogType(AliLog::kDebug + debugLevel, myclasses.Data());

   // create mix output container
   AliAnalysisDataContainer *outputMix = mgr->CreateContainer("cMixInfoList", TList::Class(), AliAnalysisManager::kOutputContainer, Form("MixInfo%s.root", opts.Data()));

   // add our task to the manager
   mgr->AddTask(task);

   // finaly connect input and output
   mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
   mgr->ConnectOutput(task, 1, outputMix);

}
