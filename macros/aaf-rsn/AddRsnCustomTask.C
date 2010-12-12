#include <RESONANCESMV/AliRsnCustomTrackInfo.h>
#include <RESONANCESMV/AliRsnCustomTask.h>
AliAnalysisTask* AddRsnCustomTask(TString format="esd",Bool_t useMC = kFALSE,TString opts="")
{
  // create manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) mgr = new AliAnalysisManager("My test train");
  
  // create our task
  AliRsnCustomTask *task = new AliRsnCustomTask("AliRsnCustomTask");
  
  AliRsnCustomTrackInfo *trackInfo = new AliRsnCustomTrackInfo("myTrackInfo","My Track Info");
  trackInfo->SetParamaterHistogram(AliRsnCustomTrackInfo::kPt,100,0.0,10.0);
  task->AddCustomObject((AliRsnVCustomObj*)trackInfo);
  
//   const char* outFileName = AliAnalysisManager::GetCommonFileName();
  const char* outFileName = "MyRsnCustom.root";
  // create output container
  AliAnalysisDataContainer *outputInfo = mgr->CreateContainer("cRsnInfo", TList::Class(), AliAnalysisManager::kOutputContainer, outFileName);
  AliAnalysisDataContainer *outputHist = mgr->CreateContainer("cRsnCustomList", TList::Class(), AliAnalysisManager::kOutputContainer, outFileName);
  // add our task to the manager
  mgr->AddTask(task);

  // finaly connect input and output
  mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
  // create containers for output

  mgr->ConnectOutput(task, 1, outputInfo);
  mgr->ConnectOutput(task, 2, outputHist);

	return task;
}

