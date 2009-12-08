#ifndef __CINT__
#endif
AliAnalysisManager *AliRsnTrain(Int_t debugAnalysisMgr = 0) {

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  if (!mgr)
    mgr = new AliAnalysisManager("RSN Train");

  mgr->SetDebugLevel(debugAnalysisMgr);

  Bool_t useAod = 0;
  Bool_t useKine = 0;
//   useAod = 1;
  useKine = 1;

  SetCorrectHandlersRsn(useKine ,useAod);

  // train Task
//   gROOT->LoadMacro("AddAnalysisTrainProcess.C");
//   AddAnalysisTrainProcess(100);

  // ESDMC filter
  gROOT->LoadMacro("AddESDMCFilter.C");
//     AddESDMCFilter();

  // RSN Test
  gROOT->LoadMacro("AddRsnAnalysisSETest.C");
//     AddRsnAnalysisSETest();

  // RSN SE
  gROOT->LoadMacro("AddRsnAnalysisSE.C");
  AddRsnAnalysisSE();

  // RSN SE MC
  gROOT->LoadMacro("AddRsnAnalysisSE_MC.C");
//   AddRsnAnalysisSE_MC();

  // RSN ParticleInfo
  gROOT->LoadMacro("AddRsnParticleInfo.C");
//     AddRsnParticleInfo();

  // RSN OUT
  gROOT->LoadMacro("AddRsnOutPut.C");
//   AddRsnOutPut();

  // Custom Task
//   gProof->Load("AliAnalysisTaskCustom.cxx++");
//   gROOT->LoadMacro("AddCustomAnalysisTask.C");
//   AddCustomAnalysisTask();


  return mgr;
}

void SetCorrectHandlersRsn(Bool_t useKine = kTRUE, Bool_t useAod = kFALSE) {

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  AliAnalysisDataContainer *cin = mgr->GetCommonInputContainer();

  if (cin) return;

  if (useAod) {
      AliAODInputHandler *aodInputHandler = dynamic_cast<AliAODInputHandler*>(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());

      if (!aodInputHandler) {
          Info("AliRsnTrain.C", "Creating aodInputHandler ...");
          aodInputHandler = new AliAODInputHandler();
          mgr->SetInputEventHandler(aodInputHandler);
        }
    } else {
      AliESDInputHandler *esdInputHandler = dynamic_cast<AliESDInputHandler*>(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());

      if (!esdInputHandler) {
          Info("AliRsnTrain.C", "Creating esdInputHandler ...");
          esdInputHandler = new AliESDInputHandler();
          mgr->SetInputEventHandler(esdInputHandler);
        }

      if (useKine) {
          AliMCEventHandler* mcInputHandler = dynamic_cast<AliMCEventHandler*>(AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler());

          if (!mcInputHandler) {
              Info("AliRsnTrain.C", "Creating mcInputHandler ...");
              AliMCEventHandler* mcInputHandler = new AliMCEventHandler();
              mgr->SetMCtruthEventHandler(mcInputHandler);
            }
        }

    }
}
