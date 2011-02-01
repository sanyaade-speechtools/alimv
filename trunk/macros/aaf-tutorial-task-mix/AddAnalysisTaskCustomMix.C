#include <AliAnalysisManager.h> //|
#include <AliLog.h> //|
#include <TASKSMV/AliAnalysisTaskCustomMix.h>//|

void AddAnalysisTaskCustomMix(TString format = "esd", Bool_t useMC = kFALSE, TString opts = "")
{
   // create manager
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) return;

//   mgr->SetDebugLevel(10);

   // create our task
   AliAnalysisTaskCustomMix *task = new AliAnalysisTaskCustomMix("AliAnalysisTaskCustomMix");
//   task->SetMixingDebug(AliLog::kDebug);

   Int_t debugLevel = 0;
   TString myclasses = "";
//   myclasses+=":AliAnalysisTaskCustomMix";
//   myclasses+=":AliMixInputEventHandler";
//   myclasses+=":AliMultiInputEventHandler";
//   myclasses+=":AliMixEventPool";
//   myclasses+=":AliMixInputHandlerInfo";
   if (!myclasses.IsNull()) task->SetLogType(AliLog::kDebug + debugLevel, myclasses.Data());

//   AliMixManager *mixMgr = task->GetMixManager();
//   mixMgr->SetMixNumber(10);
//   AliMixMixerSimple *mixer = new AliMixMixerSimple();
//   mixer->SetUsePostProcess(kFALSE);
//   mixMgr->SetMixer(mixer);

// uncomment this to use MC
//   task->UseMC(useMC);

   // create mix output container
   AliAnalysisDataContainer *outputMix = mgr->CreateContainer("cMixOutput", TList::Class(), AliAnalysisManager::kOutputContainer, Form("MixOutput%s.root", opts.Data()));
//   output1->SetSpecialOutput();
//   mgr->SetSpecialOutputLocation("root://localhost//dev/shm/alice/xrd/data/");

   // add our task to the manager
   mgr->AddTask(task);

   // finaly connect input and output
   mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
   mgr->ConnectOutput(task, 1, outputMix);

}
