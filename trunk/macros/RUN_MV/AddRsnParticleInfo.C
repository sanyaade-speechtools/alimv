#ifndef __CINT__
#endif

void AddRsnParticleInfo(AliLog::EType_t type=AliLog::kInfo)
{
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  AliRsnParticleInfoSE *taskPI = new AliRsnParticleInfoSE("AliRsnParticleInfoSE",1);
//   task->SetLogType(type,"AliRsnAnalysisManager:AliRsnPairManager:AliRsnPairManager:AliRsnPair");
//   task->SetPrintInfoNumber(10);


  AliRsnParticleInfo *phi = new AliRsnParticleInfo();
  phi->SetPDG(333);
  phi->AddHistogram(AliRsnParticleInfo::kP,1000,0,10);
  phi->AddHistogram(AliRsnParticleInfo::kPt,1000,0,10);
  phi->AddHistogram(AliRsnParticleInfo::kEta,1001,-5,5);
//   phi->Print();
  
  AliRsnParticleInfoManager *particleInfoMgr = taskPI->GetParticleManager();
  particleInfoMgr->AddPair(phi);
  particleInfoMgr->Print();
  
//   return;

  AliAnalysisDataContainer *outputInfo = mgr->CreateContainer("RsnInfo2", TList::Class(), AliAnalysisManager::kOutputContainer, "RSNAnalysisInfo2.root");
  AliAnalysisDataContainer *outputRsn = mgr->CreateContainer("RsnMC_Phi", TList::Class(), AliAnalysisManager::kOutputContainer, "RSNAnalysis0.root");
  
  mgr->AddTask(taskPI);

  mgr->ConnectInput(taskPI, 0, mgr->GetCommonInputContainer());
  mgr->ConnectOutput(taskPI, 1, outputInfo);

  mgr->ConnectOutput(taskPI, 2, outputRsn);
//  outputRsn[i]->SetSpecialOutput();

}
