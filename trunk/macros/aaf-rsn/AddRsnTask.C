
//
// This macro serves to add the RSN analysis task to the steering macro.
//
// Inputs:
//   - dataLabel   = a string with informations about the type of data
//                   which could be needed to be ported to the config macro
//                   to set up some cuts
//   - configMacro = macro which configures the analysis; it has *ALWAYS*
//                   defined inside a function named 'RsnConfigTask()',
//                   whatever the name of the macro itself, whose first two
//                   arguments must have to be the task and the 'dataLabel' argument.
// 

AliAnalysisTask* AddRsnTask(TString format="esd",Bool_t useMC = kFALSE,TString opts="")
{
	
	TString configMacro = "ConfigRsnSimple.C";
  TString suffix;
	// retrieve analysis manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) mgr = new AliAnalysisManager("RSN train");
	
  // initialize task with all available slots, even if not all of them will be used:
  AliRsnAnalysisMulti *task = new AliRsnAnalysisMulti("RsnAnalysis");
  task->SetZeroEventPercentWarning(100.0);
//   task->SelectCollisionCandidates();
  if (opts.Contains("mix")) {
    task->SetMixing();
    suffix = "_mix";
  }
  
  // load and execute configuration macro
  gROOT->LoadMacro(configMacro);
	configMacro.ReplaceAll(".C","");
  if (!gROOT->ProcessLine(Form("%s((AliRsnAnalysisManager*)0x%lx)",configMacro.Data(),(ULong_t)task->GetAnalysisManager()))) return kFALSE;

  // add the task to manager
  mgr->AddTask(task);

  // connect input container according to source choice
  mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());

  // create paths for the output in the common file
  Char_t commonPath[500];
  sprintf(commonPath, "%s", AliAnalysisManager::GetCommonFileName());

  // create containers for output
  AliAnalysisDataContainer *outputInfo = mgr->CreateContainer(Form("RsnInfo%s",suffix.Data()), TList::Class(), AliAnalysisManager::kOutputContainer, commonPath);
  AliAnalysisDataContainer *outputHist = mgr->CreateContainer(Form("RsnHist%s",suffix.Data()), TList::Class(), AliAnalysisManager::kOutputContainer, commonPath);
  mgr->ConnectOutput(task, 1, outputInfo);
  mgr->ConnectOutput(task, 2, outputHist);

	return task;
}
