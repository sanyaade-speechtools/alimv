void AddAnalysisTrainProcess(Int_t refreshNumber = 100,AliLog::EType_t type=AliLog::kInfo)
{
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  AliAnalysisTrainProcessTask *task = new AliAnalysisTrainProcessTask("AliAnalysisTrainProcessTask");
  task->SetRefreshNumber(refreshNumber);

  AliAnalysisDataContainer *outputInfo = mgr->CreateContainer("Info", TList::Class(), AliAnalysisManager::kOutputContainer, "AliTrainInfo.root");

  mgr->AddTask(task);

  mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 1, outputInfo);

}
