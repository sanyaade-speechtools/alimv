void AddRsnAnalysisSE(AliLog::EType_t type=AliLog::kInfo)
{

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  const Int_t numRsnConfigs=1;
  AliRsnAnalysisSE *task = new AliRsnAnalysisSE("AliRsnAnalysisSE",numRsnConfigs);

//   task->SetZeroEventPercentWarning(10); // default is 50%

//   type=AliLog::Debug+3;
//   task->SetLogType(type,"AliRsnAnalysisManager:AliRsnPairManager:AliRsnPairManager:AliRsnPair");
//   task->SetLogType(type,"AliRsnCut");

  task->SetPrintInfoNumber(100);

  task->SetPriorProbability(AliPID::kElectron, 0.02);
  task->SetPriorProbability(AliPID::kMuon,     0.02);
  task->SetPriorProbability(AliPID::kPion,     0.83);
  task->SetPriorProbability(AliPID::kKaon,     0.07);
  task->SetPriorProbability(AliPID::kProton,   0.06);
  task->DumpPriors();

  Int_t i;
  AliRsnAnalysisManager *analMgr=0;

  for (i=0;i<numRsnConfigs;i++)
  {
    analMgr = task->GetAnalysisManager(i);

    if (analMgr)
      analMgr->AddConfig("RsnConfigDubna.C",Form("PHI%d",i),"RsnConfig_PHI");
  }

  AliAnalysisDataContainer *outputInfo = mgr->CreateContainer("RsnInfo", TList::Class(), AliAnalysisManager::kOutputContainer, "RSNAnalysisInfo.root");

  AliAnalysisDataContainer *outputRsn[numRsnConfigs];

  for (i=0;i<numRsnConfigs;i++)
  {
    outputRsn[i] = mgr->CreateContainer(Form("RsnPhi%d",i), TList::Class(), AliAnalysisManager::kOutputContainer, Form("RSNAnalysis%d.root",i));
//     outputRsn[i] = mgr->CreateContainer(Form("RsnPhi%d",i), TList::Class(), AliAnalysisManager::kExchangeContainer/*, Form("RSNAnalysis%d.root",i)*/);

  }

  mgr->AddTask(task);

  mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 1, outputInfo);

  for (i=0;i<numRsnConfigs;i++)
  {
    mgr->ConnectOutput(task, i+2, outputRsn[i]);
//     outputRsn[i]->SetSpecialOutput();
  }

}
