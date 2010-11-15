#include <TString.h>
#include <TROOT.h>
#include <AliAnalysisManager.h>

void AddMyAnalysisManagerAodAnalysisTask(TString analysisSource="proof",TString analysisMode="test",TString opts="") {

  Bool_t useMC = kFALSE;
  TString format="aod";
  
  // ALICE stuff
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) mgr = new AliAnalysisManager("Martin Vala's AM");
  gROOT->LoadMacro("SetupAnalysisPlugin.C");
  AliAnalysisGrid *analysisPlugin = SetupAnalysisPlugin(analysisMode.Data());
  if (!analysisPlugin) return;
  analysisPlugin->SetAnalysisSource("AliAnalysisTaskCustom.cxx+");
  analysisPlugin->SetAdditionalLibs("AliAnalysisTaskCustom.h AliAnalysisTaskCustom.cxx");
  analysisPlugin->SetProofCluster("pod@localhost:21001");
  analysisPlugin->SetProofDataSet("myds#aodTree");
  mgr->SetGridHandler(analysisPlugin);

  if (!InputHandlerSetup(format,useMC) ) return;
  
  gROOT->LoadMacro("AliAnalysisTaskCustom.cxx+g");
  
  // load and run AddTask macro
  gROOT->LoadMacro("AddAnalysisTaskAodAnalysis.C");  
  AddAnalysisTaskAodAnalysis(format,useMC,opts);
  
}
