#ifndef __CINT__
#include <TString.h>
#include <TROOT.h>
#include <AliAnalysisManager.h>
#include <AliMultiInputEventHandler.h>
#include <AliAnalysisGrid.h>
#include <ANALYSISaliceMV/AliLogInputHandler.h>
#include <AliLog.h>
#include <ANALYSISaliceMV/AliTenderInputEventHandler.h>
#include <AliESDpid.h>
#include <AliAnalysisAlien.h>
#endif
void AddMyAnalysisManagerExampleTasks(TString analysisSource = "proof", TString analysisMode = "test", TString opts = "")
{

   Bool_t useMC = kFALSE;
//   useMC = kTRUE;
   TString format = "esd";
//    format = "aod";
   format.ToLower();

   Bool_t useMultiHandler = kFALSE;
   useMultiHandler = kTRUE;

   // ALICE stuff
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) mgr = new AliAnalysisManager("Evil AM");

   gROOT->LoadMacro("SetupAnalysisPlugin.C");
   AliAnalysisGrid *analysisPlugin = SetupAnalysisPlugin(analysisMode.Data());
   if (!analysisPlugin) return;

   gSystem->Load("libAOD.so");
   gSystem->Load("libANALYSIS.so");
   gSystem->Load("libANALYSISalice.so");
   gSystem->Load("libTENDER.so");
   gSystem->Load("libTENDERSupplies.so");
   gSystem->Load("libEventMixing.so");
//    AliAnalysisAlien::SetupPar("EventMixing");
   AliAnalysisAlien::SetupPar("ANALYSISaliceMV");

   analysisPlugin->SetAnalysisSource("AliAnalysisTaskEx02.cxx");
//    analysisPlugin->SetAdditionalLibs("libTENDER.so libTENDERSupplies.so EventMixing.par ANALYSISaliceMV.par AliAnalysisTaskEx02.h AliAnalysisTaskEx02.cxx");
   analysisPlugin->SetAdditionalLibs("libTENDER.so libTENDERSupplies.so libEventMixing.so ANALYSISaliceMV.par AliAnalysisTaskEx02.h AliAnalysisTaskEx02.cxx");
   analysisPlugin->SetAliRootMode("ALIROOT");

   mgr->SetGridHandler(analysisPlugin);
   AliMultiInputEventHandler *multiInputHandler = 0;
   if (useMultiHandler)  {
      multiInputHandler = new AliMultiInputEventHandler();
      // add main input handler
      mgr->SetInputEventHandler(multiInputHandler);

      if (!format.CompareTo("esd")) {
         multiInputHandler->AddInputEventHandler(new AliESDInputHandler());
         if (useMC) multiInputHandler->AddInputEventHandler(new AliMCEventHandler());
      } else if (!format.CompareTo("aod"))multiInputHandler->AddInputEventHandler(new AliAODInputHandler());
   } else {
      if (!InputHandlerSetup(format, useMC)) return;
   }
   if (useMultiHandler)  {
      // add Tender Handler
      gROOT->LoadMacro("AddTenderHandler.C");
//       AddTenderHandler(multiInputHandler);

      // add mixing handler (uncomment to turn on Mixnig)
      gROOT->LoadMacro("AddMixingHandler.C");
//       AddMixingHandler(multiInputHandler);

      // adds mixing info task
//       gROOT->LoadMacro("AddAnalysisTaskMixInfo.C");
//       AddAnalysisTaskMixInfo(format, useMC, opts);

      // add Log Handler
      gROOT->LoadMacro("AddLogHanlder.C");
      AddLogHanlder(multiInputHandler);
   }
   gROOT->LoadMacro("AliAnalysisTaskEx02.cxx++g");

   // load and run AddTask macro
   gROOT->LoadMacro("AddExampleAnalysisTask.C");
   AddExampleAnalysisTask(format, useMC);

}

