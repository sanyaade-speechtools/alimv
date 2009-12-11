void runRsnAnalysisSE(AliLog::EType_t type=AliLog::kInfo,Bool_t useKine = kTRUE)
{
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  AliAnalysisDataContainer *cin;
  AliAnalysisDataContainer *cout;

  cin = SetCorrectHandlersAndReturnInput(mgr,useKine);

  const Int_t numRsnConfigs=3;
  AliRsnAnalysisSE *task = new AliRsnAnalysisSE("AliRsnAnalysisSE",numRsnConfigs);
//   task->SetZeroEventPercentWarning(10); // default is 50%
  task->SetLogType(type,"AliRsnAnalysisManager:AliRsnPairManager:AliRsnPairManager:AliRsnPair");
  task->SetPrintInfoNumber(10);

  task->SetPriorProbability(AliPID::kElectron, 0.02);
  task->SetPriorProbability(AliPID::kMuon,     0.02);
  task->SetPriorProbability(AliPID::kPion,     0.83);
  task->SetPriorProbability(AliPID::kKaon,     0.07);
  task->SetPriorProbability(AliPID::kProton,   0.06);
  task->DumpPriors();

  Int_t i;
  AliRsnAnalysisManager *analMgr=0;
  for(i=0;i<numRsnConfigs;i++){
    analMgr = task->GetAnalysisManager("MyAnalysisSE",i);
    if (analMgr)
      analMgr->AddConfig("RsnConfig.C",Form("PHI%d",i),"RsnConfig_PHI");
  }
  AliAnalysisDataContainer *outputInfo = mgr->CreateContainer("RsnInfo", TList::Class(), AliAnalysisManager::kOutputContainer, "RSNAnalysisInfo.root");
  AliAnalysisDataContainer *outputRsn[numRsnConfigs];
  for(i=0;i<numRsnConfigs;i++){
    outputRsn[i] = mgr->CreateContainer(Form("RsnPhi%d",i), TList::Class(), AliAnalysisManager::kOutputContainer, Form("RSNAnalysis%d.root",i));
  }
  
  mgr->AddTask(task);

  mgr->ConnectInput(task, 0, cin);
  mgr->ConnectOutput(task, 1, outputInfo);

  for(i=0;i<numRsnConfigs;i++){
    mgr->ConnectOutput(task, i+2, outputRsn[i]);
//     outputRsn[i]->SetSpecialOutput();
  }

}

AliAnalysisDataContainer *SetCorrectHandlersAndReturnInput(AliAnalysisManager *mgr,Bool_t useKine = kTRUE)
{

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  AliAnalysisDataContainer *cin;

  if (useKine) {
    AliMCEventHandler* mcInputHandler = dynamic_cast<AliMCEventHandler*> (AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler());
    if (!mcInputHandler) {
      Info("","Creating mcInputHandler ...");
      AliMCEventHandler* mcInputHandler = new AliMCEventHandler();
      mgr->SetMCtruthEventHandler(mcInputHandler);
    }
  }

  AliESDInputHandler *esdInputHandler = dynamic_cast<AliESDInputHandler*> (AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());
  if (!esdInputHandler) {
    Info("","Creating esdInputHandler ...");
    esdInputHandler = new AliESDInputHandler();
    mgr->SetInputEventHandler  (esdInputHandler);
    cin = mgr->GetCommonInputContainer();
  } else {
    cin = mgr->GetCommonOutputContainer();
  }

  return cin;
}
