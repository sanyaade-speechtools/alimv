#include <TString.h>
#include <TROOT.h>
#include <AliAnalysisManager.h>

void AddMyAnalysisManagerESDFilter(TString analysisSource="proof",TString analysisMode="test",TString opts="") {

  Bool_t useMC = kFALSE;
  TString format="esd";
  
  // ALICE stuff
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) mgr = new AliAnalysisManager("ESD filter");

  gROOT->LoadMacro("SetupAnalysisPlugin.C");
  AliAnalysisGrid *analysisPlugin = SetupAnalysisPlugin(analysisMode.Data());
  if (!analysisPlugin) return;
  mgr->SetGridHandler(analysisPlugin);
  analysisPlugin->SetProofCluster("pod@localhost:21001");
	analysisPlugin->SetProofDataSet("LHC10b.txt");
  analysisPlugin->SetAnalysisSource("AliAnalysisTaskDataSet.cxx");
  analysisPlugin->SetAdditionalLibs("AliAnalysisTaskDataSet.h AliAnalysisTaskDataSet.cxx");
  if (!InputHandlerSetup(format,useMC,kTRUE) ) return;
//   if (!InputHandlerSetup(format,useMC) ) return;
  // load and run AddTask macro
  gROOT->LoadMacro("AddTaskESDFilter.C");
  AddTaskESDFilter(kFALSE,kFALSE,kFALSE,kFALSE);

  gROOT->LoadMacro("AliAnalysisTaskDataSet.cxx+g");
  // load and run AddTask macro
  gROOT->LoadMacro("AddTaskDataSet.C");
  AddTaskDataSet();
  
}
