#include <AliAnalysisManager.h>
#include <AliLog.h>
#include <MIXMV/MIX/AliMixManager.h>
#include <MIXMV/MIX/AliAnalysisTaskMultiplicityDifference.h>
#include <MIXMV/MIX/AliMixMixerSimple.h>

void AddAnalysisTaskMuliplicityDifference(TString format="esd",Bool_t useMC = kFALSE,TString opts="")
{
  // create manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) return;

	// create our task
  AliAnalysisTaskMultiplicityDifference *task = new AliAnalysisTaskMultiplicityDifference("AliAnalysisTaskMultiplicityDifference");
//   task->SetMixingDebug(AliLog::kDebug);
  
//   task->SetLogType(AliLog::kDebug+1,"AliAnalysisTaskMix:AliMixManager:AliMixMixerSimple");
//   task->SetLogType(AliLog::kDebug+3,"AliAnalysisTaskMix:AliMixManager:AliMixMixerSimple");

  AliMixManager *mixMgr = task->GetMixManager();
  mixMgr->SetMixNumber(10);
  AliMixMixerSimple *mixer = new AliMixMixerSimple();
  mixer->SetUsePostProcess(kFALSE);
  mixMgr->SetMixer(mixer);
  // uncomment this to use MC
//   task->UseMC(useMC);
  
  // create mix output container
  AliAnalysisDataContainer *outputMix = mgr->CreateContainer("cMixInfoList", TList::Class(), AliAnalysisManager::kOutputContainer, Form("MixInfo%s.root",opts.Data()));
//   output1->SetSpecialOutput();
//   mgr->SetSpecialOutputLocation("root://localhost//dev/shm/alice/xrd/data/");
  AliAnalysisDataContainer *output = mgr->CreateContainer("cMultiDiffList", TList::Class(), AliAnalysisManager::kOutputContainer, Form("MultiDiff%s.root",opts.Data()));
  
  // add our task to the manager
  mgr->AddTask(task);

  // finaly connect input and output
  mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 1, outputMix);
  mgr->ConnectOutput(task, 2, output);

}
