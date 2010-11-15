#include <AliAnalysisManager.h> //|
#include <AliLog.h> //|
#include "AliAnalysisTaskCustomMix.h" //|

void AddAnalysisTaskCustomMix(TString format="esd",Bool_t useMC = kFALSE,TString opts="")
{
  // create manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) return;
  
//   mgr->SetDebugLevel(10);

	// create our task
  AliAnalysisTaskCustomMix *task = new AliAnalysisTaskCustomMix("AliAnalysisTaskCustomMix");
//   task->SetMixingDebug(AliLog::kDebug);
  
  Int_t debugLevel = 5;
  TString myclasses="";
  myclasses+=":AliAnalysisTaskCustomMix";
  myclasses+=":AliMixEventInputHandler";
//   myclasses+=":AliMixEventPool";
//   myclasses+=":AliMixInputHandlerInfo";
  
  task->SetLogType(AliLog::kDebug+debugLevel,myclasses.Data());

//   AliMixManager *mixMgr = task->GetMixManager();
//   mixMgr->SetMixNumber(10);
//   AliMixMixerSimple *mixer = new AliMixMixerSimple();
//   mixer->SetUsePostProcess(kFALSE);
//   mixMgr->SetMixer(mixer);

// uncomment this to use MC
//   task->UseMC(useMC);
  
  // create mix output container
  AliAnalysisDataContainer *outputMix = mgr->CreateContainer("cMixInfoList", TList::Class(), AliAnalysisManager::kOutputContainer, Form("MixInfo%s.root",opts.Data()));
//   output1->SetSpecialOutput();
//   mgr->SetSpecialOutputLocation("root://localhost//dev/shm/alice/xrd/data/");
  
  // add our task to the manager
  mgr->AddTask(task);

  // finaly connect input and output
  mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 1, outputMix);

}
