#include <TString.h>
#include <TROOT.h>
#include <AliAnalysisManager.h>

void AddMyAnalysisManagerEvilTask(TString analysisSource = "proof", TString analysisMode = "test", TString opts = "")
{

   Bool_t useMC = kFALSE;
//   useMC = kTRUE;
   TString format = "esd";
   Bool_t useMultiHandler = kFALSE;
   useMultiHandler = kTRUE;

   // ALICE stuff
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) mgr = new AliAnalysisManager("Evil AM");

   gROOT->LoadMacro("SetupAnalysisPlugin.C");
   AliAnalysisGrid *analysisPlugin = SetupAnalysisPlugin(analysisMode.Data());
   if (!analysisPlugin) return;

   if (useMultiHandler) {
      AliAnalysisAlien::SetupPar("ANALYSISaliceMV");
      analysisPlugin->SetAnalysisSource("AliAnalysisTaskEvil.cxx");
      analysisPlugin->SetAdditionalLibs("ANALYSISaliceMV.par AliAnalysisTaskEvil.h AliAnalysisTaskEvil.cxx");
   } else {
      analysisPlugin->SetAnalysisSource("AliAnalysisTaskEvil.cxx");
      analysisPlugin->SetAdditionalLibs("AliAnalysisTaskEvil.h AliAnalysisTaskEvil.cxx");
   }

   mgr->SetGridHandler(analysisPlugin);

   if (useMultiHandler)  {
      AliMultiInputEventHandler *inputHandler = new AliMultiInputEventHandler();
      inputHandler->AddInputEventHandler(new AliESDInputHandler());
      if (useMC) inputHandler->AddInputEventHandler(new AliMCEventHandler());
      mgr->SetInputEventHandler(inputHandler);
   } else {
      if (!InputHandlerSetup(format, useMC)) return;
   }

   gROOT->LoadMacro("AliAnalysisTaskEvil.cxx++g");

   // load and run AddTask macro
   gROOT->LoadMacro("AddEvilAnalysisTask.C");
   AddEvilAnalysisTask(format, useMC);

}
