  // ++++++++++++++++ End Analysis Managers +++++++++++++++++++++++
  ::Info(gSystem->HostName(),"All done ...");
	return ret;
}

Int_t LoadLibsBase() {
  Int_t num=0;
  if (gSystem->Load("libTree.so")<0) {num++; return num;}
  if (gSystem->Load("libGeom.so")<0) {num++; return num;}
  if (gSystem->Load("libVMC.so")<0) {num++; return num;}
  if (gSystem->Load("libPhysics.so")<0) {num++; return num;}
  if (gSystem->Load("libSTEERBase.so")<0) {num++; return num;}
  if (gSystem->Load("libESD.so")<0) {num++; return num;}
  if (gSystem->Load("libAOD.so")<0) {num++; return num;}
  if (gSystem->Load("libANALYSIS.so")<0) {num++; return num;}
  if (gSystem->Load("libANALYSISalice.so")<0) {num++; return num;}
  
  gSystem->AddIncludePath(Form("-I\"%s/include\"", gSystem->Getenv("ALICE_ROOT")));
  gROOT->ProcessLine(Form(".include %s/include",gSystem->ExpandPathName("$ALICE_ROOT")));
  return 0;
}

void RunAnalysisManager(TString managerMacro,TString analysisSource="proof",TString analysisMode="test",TString opt="",TString mgrOpt="") {

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (mgr) { delete mgr; mgr=0; }
  
  gROOT->LoadMacro(Form("%s.C",managerMacro.Data()));
  gROOT->ProcessLine(Form("%s\(\"%s\",\"%s\",\"%s\"\)",managerMacro.Data(),analysisSource.Data(),analysisMode.Data(),opt.Data()));

  mgr = AliAnalysisManager::GetAnalysisManager();

  // Run analysis
  mgr->InitAnalysis();
  mgr->PrintStatus();

  if (mgrOpt.IsNull()) {
    if ((!analysisSource.CompareTo("proof")) || (!analysisSource.CompareTo("local"))) {
      mgr->StartAnalysis(analysisSource.Data(),fNEvents,fNEventsSkip);
    } else {
      mgr->StartAnalysis(analysisSource.Data());
    }
  } else {
    mgr->StartAnalysis("proof",mgrOpt.Data(),fNEvents,fNEventsSkip);
  }
  
//   if ((gProof) && (gProof->IsLite()))
//       TProof::Mgr("")->GetSessionLogs()->Display("*",-1000,-1);
}

Bool_t InputHandlerSetup(TString format = "esd", Bool_t useKine = kTRUE, Bool_t useAODOut=kFALSE)
{
  format.ToLower();

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  AliAnalysisDataContainer *cin = mgr->GetCommonInputContainer();

  if (cin) return;

  if (!format.CompareTo("esd"))
  {
    AliESDInputHandler *esdInputHandler = dynamic_cast<AliESDInputHandler*>(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());

    if (!esdInputHandler)
    {
      Info("CustomAnalysisTaskInputSetup", "Creating esdInputHandler ...");
      esdInputHandler = new AliESDInputHandler();
      mgr->SetInputEventHandler(esdInputHandler);
    }

    if (useKine)
    {
      AliMCEventHandler* mcInputHandler = dynamic_cast<AliMCEventHandler*>(AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler());

      if (!mcInputHandler)
      {
        Info("CustomAnalysisTaskInputSetup", "Creating mcInputHandler ...");
        AliMCEventHandler* mcInputHandler = new AliMCEventHandler();
        mgr->SetMCtruthEventHandler(mcInputHandler);
      }
    }

  }
  else if (!format.CompareTo("aod"))
  {
    AliAODInputHandler *aodInputHandler = dynamic_cast<AliAODInputHandler*>(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());

    if (!aodInputHandler)
    {
      Info("CustomAnalysisTaskInputSetup", "Creating aodInputHandler ...");
      aodInputHandler = new AliAODInputHandler();
      mgr->SetInputEventHandler(aodInputHandler);
    }
  }
  else
  {
    AliWarning("Wrong input format!!! Only ESD and AOD are supported. Skipping Task ...");
    return kFALSE;
  }

  if (useAODOut) {
    AliAODHandler* aodHandler   = new AliAODHandler();
    aodHandler->SetOutputFileName("AliAOD.root");
    mgr->SetOutputEventHandler(aodHandler);
  }

  return kTRUE;
}
