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
#include <AliPhysicsSelection.h>
#endif
void AddAMExampleTasks(TString analysisSource = "proof", TString analysisMode = "test", TString opts = "")
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
   TString mylibs = "libTENDER.so libTENDERSupplies.so";
   
   Bool_t useEventMixingPar = kTRUE;
//    useEventMixingPar = kFALSE;
   
//    Bool_t useEventMixingPar = kTRUE;
//    useEventMixingPar = kFALSE;

   if (useEventMixingPar) {
     AliAnalysisAlien::SetupPar("EventMixing");
     mylibs += " EventMixing.par";
  } else {
    gSystem->Load("libEventMixing.so");
    mylibs += " libEventMixing.so";
  }

   AliAnalysisAlien::SetupPar("ANALYSISaliceMV");
   mylibs += " ANALYSISaliceMV.par ";
   
   mylibs += " AliAnalysisTaskEx02.h AliAnalysisTaskEx02.cxx";
   analysisPlugin->SetAnalysisSource("AliAnalysisTaskEx02.cxx");
   analysisPlugin->SetAdditionalLibs(mylibs.Data());
   analysisPlugin->SetAliRootMode("ALIROOT");

   mgr->SetGridHandler(analysisPlugin);
   AliMultiInputEventHandler *multiInputHandler = 0;
   AliESDInputHandler *esdInputHandler = 0;
   if (useMultiHandler)  {
      multiInputHandler = new AliMultiInputEventHandler();
      // add main input handler
      mgr->SetInputEventHandler(multiInputHandler);

      if (!format.CompareTo("esd")) {
         esdInputHandler = new AliESDInputHandler();
         multiInputHandler->AddInputEventHandler(esdInputHandler);
         if (useMC) multiInputHandler->AddInputEventHandler(new AliMCEventHandler());
      } else if (!format.CompareTo("aod"))multiInputHandler->AddInputEventHandler(new AliAODInputHandler());
   } else {
      if (!InputHandlerSetup(format, useMC)) return;
   }
   if (useMultiHandler)  {
      // add Tender Handler (not working with mixing)
//       gROOT->LoadMacro("AddTenderHandler.C");
//       AddTenderHandler(multiInputHandler);

      // add mixing handler (uncomment to turn on Mixnig)
      gROOT->LoadMacro("AddMixingHandler.C");
      AddMixingHandler(multiInputHandler);

      // adds mixing info task
      gROOT->LoadMacro("AddAnalysisTaskMixInfo.C");
      AddAnalysisTaskMixInfo(format, useMC, opts);

      // add Log Handler
      gROOT->LoadMacro("AddLogHanlder.C");
      AddLogHanlder(multiInputHandler);
   }
   gROOT->LoadMacro("AliAnalysisTaskEx02.cxx++g");

//    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C");
//    AddTaskPhysicsSelection(useMC);
//
//    // maybe we can put it in $ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C
//    if (multiInputHandler)
//       esdInputHandler->SetEventSelection(multiInputHandler->GetEventSelection());

   // load and run AddTask macro
   gROOT->LoadMacro("AddExampleAnalysisTask.C");
   AddExampleAnalysisTask(format, useMC);

}
