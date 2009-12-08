#ifndef __CINT__
#endif
void AddRsnOutPut(Bool_t useList = kFALSE, AliLog::EType_t type = AliLog::kInfo) {

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  AliRsnOutputTask *task = new AliRsnOutputTask("AliRsnOutputTask", useList);

  task->SetTrainInfoFileName("RSNAnalysisInfo.root");
  task->SetRsnInfoFileName("RSNAnalysisInfo.root");
  task->SetAnalysisFileName("RSNAnalysis0.root");
  
//   task->SetRsnInfoFileName("proof_train_archive.zip#AliTrainInfo.root");
//   task->SetRsnInfoFileName("proof_train_archive.zip#RSNAnalysisInfo.root");
// task->SetRsnInfoFileName("RSNAnalysisInfo.root");
// task->SetRsnInfoFileName("RSNAnalysisInfo_MC.root");

//   task->ImportTxt("RSNAnalysis0.root");
//   task->ImportTxt("RSNAnalysis0_MC.root");
  // define input analysis file, input list file, and final output root file
  TString txtfiles = "LHC09a4_800xx_rsnout.txt,LHC09a4_10TeV_rsnout.txt,LHC09a4_run8101X_rsnout.txt,";
  txtfiles += "LHC09a4_run8158X_rsnout.txt,LHC09a4_run8158X_rsnout.txt,LHC09a4_run8159X_rsnout.txt,";
  txtfiles += "LHC09a5_run9000X_rsnout.txt,LHC09a6_run9200X_rsnout.txt";

//     task->ImportTxt("RSNAnalysis0.root",txtfiles.Data(),"RSNFileInfoTreeTmp.root");
//


  AliRsnOutputManager *outputMgr = task->GetOutputManager();

  const char *listname = "RsnPhi0";
  const char *stringPrefix = "PHI0_IM_PT_ETA_MULT";
//   stringPrefix = "PHI0_IM_PT_ETA";
  const char *stringPID = "REALISTIC";
  const char *stringCut = "default";
//   stringCut= "notTrue";

// const char *listname = "RsnPhi0_MC";
// const char *stringPrefix = "PHI0-MC_IM_PT_ETA_MULT";
// const char *stringPID= "PERFECT";
// const char *stringCut= "true";

//   const *char sPID= "REALISTIC";

  AliRsnOutputObj *phiInvMassPt = new AliRsnOutputObj("ESD_REAL");
  phiInvMassPt->AddHistogram(0, listname, Form("%s_%s_K+K-_%s", stringPrefix, stringPID, "default"), 0);
//   phiInvMassPt->AddHistogram(0, listname, Form("%s_%s_K-K-_%s",stringPrefix,stringPID,"default"), 0);

  phiInvMassPt->AddHistogram(1, listname, Form("%s_%s_K+K+_%s", stringPrefix, stringPID, "default"), 0);
  phiInvMassPt->AddHistogram(1, listname, Form("%s_%s_K-K-_%s", stringPrefix, stringPID, "default"), 0);

// phiInvMassPt->AddHistogram(0, listname, Form("%s_%s_K+K-_%s","PHI0_IM_PT_ETA_MULT","REALISTIC","true"), 0);

// phiInvMassPt->AddHistogram(1, "RsnPhi0_MC", Form("%s_%s_K+K-_%s","PHI0-MC_IM_PT_ETA_MULT","PERFECT","true"), 0);


  phiInvMassPt->SetBinningCut(1, 0.0, 10.0, 0.5);
//   phiInvMassPt->SetBinningCut(2, -1.0, 1.0, 0.1);
  phiInvMassPt->SetDoMinus();
//   phiInvMassPt->ForceRebin(2);
//   phiInvMassPt->ForceRange(0.9,1.1);
  phiInvMassPt->SetNormalizeRange(1.05, 1.2);
  phiInvMassPt->SetSignalBinRange(4, 6);
//   phiInvMassPt->DoNormalize(kFALSE);
  outputMgr->AddPair(phiInvMassPt);

  listname = "RsnPhi0_MC";
  stringPrefix = "PHI0-MC_IM_PT_ETA_MULT";
  const char *stringPID = "PERFECT";
  const char *stringCut = "true";


  AliRsnOutputObj *phiInvMassPt2 = new AliRsnOutputObj("ESD_REAL2");
  phiInvMassPt2->AddHistogram(0, listname, Form("%s_%s_K+K-_%s", stringPrefix, stringPID, "trueA"), 0);
  //   phiInvMassPt2->AddHistogram(0, listname, Form("%s_%s_K-K-_%s",stringPrefix,stringPID,"default"), 0);

//   phiInvMassPt2->AddHistogram(1, listname, Form("%s_%s_K+K+_%s",stringPrefix,stringPID,"true"), 0);
//   phiInvMassPt2->AddHistogram(1, listname, Form("%s_%s_K-K-_%s",stringPrefix,stringPID,"default"), 0);

  // phiInvMassPt2->AddHistogram(0, listname, Form("%s_%s_K+K-_%s","PHI0_IM_PT_ETA_MULT","REALISTIC","true"), 0);

  // phiInvMassPt2->AddHistogram(1, "RsnPhi0_MC", Form("%s_%s_K+K-_%s","PHI0-MC_IM_PT_ETA_MULT","PERFECT","true"), 0);


  phiInvMassPt2->SetBinningCut(1, 0.0, 10.0, 0.5);
  phiInvMassPt2->SetDoMinus();
  phiInvMassPt2->DoNormalize(kFALSE);
//   phiInvMassPt->SetNormalizeRange(1.2,1.4);
  phiInvMassPt2->SetSignalBinRange(10, 10);
  //   phiInvMassPt2->ForceRebin(2);
  //   phiInvMassPt2->ForceRange(0.9,1.1);


  outputMgr->AddPair(phiInvMassPt2);



//   AliRsnOutputObj *phiInvMassPt = new AliRsnOutputObj();
//   phiInvMassPt->AddHistogram(0, listname, Form("%s_%s_K+K-_%s",stringPrefix,stringPID,stringCut), 0);
//
//   phiInvMassPt->AddHistogram(1, listname, Form("%s_%s_K+K+_%s",stringPrefix,stringPID,stringCut), 0);
//   phiInvMassPt->AddHistogram(1, listname, Form("%s_%s_K-K-_%s",stringPrefix,stringPID,stringCut), 0);
//
//   phiInvMassPt->SetBinningCut(1, 0.0, 10.0, 1.0);
//   phiInvMassPt->SetDoMinus();
// //   phiInvMassPt->ForceRebin(2);
// //   phiInvMassPt->ForceRange(0.9,1.1);
//
//
//   outputMgr->AddPair(phiInvMassPt);

//   AliRsnOutputObj *phiInvMassEta = new AliRsnOutputObj();
//   phiInvMassEta->AddHistogram(0, listname, "PHI0_IM_PT_ETA_REALISTIC_K+K-_default", 0);
//
//   phiInvMassEta->AddHistogram(1, listname, "PHI0_IM_PT_ETA_REALISTIC_K+K+_default", 0);
//   phiInvMassEta->AddHistogram(1, listname, "PHI0_IM_PT_ETA_REALISTIC_K-K-_default", 0);
//
//   phiInvMassEta->SetBinningCut(2, -1.0, 1.0.0, 0.1);
//   phiInvMassEta->SetDoMinus();
//
//   outputMgr->AddPair(phiInvMassEta);

//
//
//   AliRsnOutputObj *phiPt = new AliRsnOutputObj();
//   phiPt->AddHistogram(0, listname, "PHI0_IM_PT_ETA_PERFECT_K+K-_default", 1);
//
//   phiPt->AddHistogram(1, listname, "PHI0_IM_PT_ETA_PERFECT_K+K+_default", 1);
//   phiPt->AddHistogram(1, listname, "PHI0_IM_PT_ETA_PERFECT_K-K-_default", 1);
//
// //     phiPt->Print();
//
//   outputMgr->AddPair(phiPt);
//
//   AliRsnOutputObj *phiEta = new AliRsnOutputObj();
//   phiEta->AddHistogram(0, listname, "PHI0_IM_PT_ETA_PERFECT_K+K-_default", 2);
//
//   phiEta->AddHistogram(1, listname, "PHI0_IM_PT_ETA_PERFECT_K+K+_default", 2);
//   phiEta->AddHistogram(1, listname, "PHI0_IM_PT_ETA_PERFECT_K-K-_default", 2);
//
//   //     phiEta->Print();
//
//   outputMgr->AddPair(phiEta);

  if (!useList)
    AliAnalysisDataContainer *cin = mgr->CreateContainer("cRsnFilesTree", TTree::Class(), AliAnalysisManager::kInputContainer);
// AliAnalysisDataContainer *cin = mgr->CreateContainer("RsnPhi0", TList::Class(), AliAnalysisManager::kInputContainer);
  AliAnalysisDataContainer *output = mgr->CreateContainer("RsnPostAnalysis", TList::Class(), AliAnalysisManager::kOutputContainer, "RSNOutTest.root");
//   AliAnalysisDataContainer *output = mgr->CreateContainer("RsnPostAnalysis", TList::Class(), AliAnalysisManager::kExchangeContainer/*, "RSNOutTest.root"*/);
//   task->SetPostEventLoop();
  mgr->AddTask(task);

//   AliAnalysisTask *taskextra = mgr->GetTask("AliRsnAnalysisSE");
//   mgr->ConnectInput(taskextra, 0 , mgr->GetCommonOutputContainer());
//   if (outputX)
//     mgr->ConnectInput(task, 0, outputX);
//   else

  mgr->ConnectInput(task, 0, cin);
  mgr->ConnectOutput(task, 0, output);


}
