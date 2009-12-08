void AddRsnAnalysisSETest(AliLog::EType_t type=AliLog::kInfo,Bool_t useKine = kTRUE)
{
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  AliRsnAnalysisSETest *task = new AliRsnAnalysisSETest("AliRsnAnalysisSETest");
  task->SetPrintInfoNumber(1000);
  
    type=AliLog::kDebug;
//   task->SetLogType(type,"AliRsnAnalysisManager:AliRsnPairManager:AliRsnPairManager:AliRsnPair");

  AliAnalysisDataContainer *outputInfo = mgr->CreateContainer("RsnInfoTest", TList::Class(), AliAnalysisManager::kOutputContainer, "RSNAnalysisInfoTest.root");

  mgr->AddTask(task);

  mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 1, outputInfo);

}
