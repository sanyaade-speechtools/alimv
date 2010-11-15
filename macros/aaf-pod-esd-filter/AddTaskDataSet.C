void AddTaskDataSet() {
  // create manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) return;

	// create our task
  AliAnalysisTaskDataSet *task = new AliAnalysisTaskDataSet("AliAnalysisTaskDataSet");

  // create output container
  AliAnalysisDataContainer *output1 = mgr->CreateContainer("cDSList", TList::Class(), AliAnalysisManager::kOutputContainer, "MYDS.root");
//   output1->SetSpecialOutput();
//   mgr->SetSpecialOutputLocation("root://localhost//dev/shm/alice/xrd/data/");
  
  // add our task to the manager
  mgr->AddTask(task);

  // finaly connect input and output
  mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 1, output1);
}
