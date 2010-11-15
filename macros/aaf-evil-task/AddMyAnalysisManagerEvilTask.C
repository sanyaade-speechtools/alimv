#include <TString.h>
#include <TROOT.h>
#include <AliAnalysisManager.h>

void AddMyAnalysisManagerEvilTask(TString analysisSource="proof",TString analysisMode="test",TString opts="") {

  Bool_t useMC = kFALSE;
  TString format="esd";

  // ALICE stuff
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) mgr = new AliAnalysisManager("Evil AM");

  gROOT->LoadMacro("SetupAnalysisPlugin.C");
  AliAnalysisGrid *analysisPlugin = SetupAnalysisPlugin(analysisMode.Data());
  if (!analysisPlugin) return;
  analysisPlugin->SetAnalysisSource("AliAnalysisTaskEvil.cxx");
  analysisPlugin->SetAdditionalLibs("AliAnalysisTaskEvil.h AliAnalysisTaskEvil.cxx");
  mgr->SetGridHandler(analysisPlugin);
  
  if (!InputHandlerSetup(format,useMC) ) return;
  
	gROOT->LoadMacro("AliAnalysisTaskEvil.cxx++g");
  
  // load and run AddTask macro
  gROOT->LoadMacro("AddEvilAnalysisTask.C");
  AddEvilAnalysisTask(format,useMC);
	
}
