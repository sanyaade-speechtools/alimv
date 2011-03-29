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
#include <PWG2resonancesDevel/AliRsnInputHandler.h>
#endif
void AddAMRsnNew(TString analysisSource = "proof", TString analysisMode = "test", TString opts = "")
{

   Bool_t useMC = kFALSE;
//    useMC = kTRUE;
   TString format = "esd";
   format = "aod";
   format.ToLower();

   Bool_t useMultiHandler = kTRUE;
//    useMultiHandler = kFALSE;

   Bool_t useEventMixingPar = kTRUE;
   useEventMixingPar = kFALSE;

   Bool_t useRSNPar = kTRUE;
//    useRSNPar = kFALSE;

   if (useEventMixingPar) useRSNPar = kTRUE;

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

   if (useEventMixingPar) {
      AliAnalysisAlien::SetupPar("EventMixing");
      mylibs += " EventMixing.par";
   } else {
      gSystem->Load("libEventMixing.so");
      mylibs += " libEventMixing.so";
   }

   if (useRSNPar) {
      AliAnalysisAlien::SetupPar("PWG2resonances");
      mylibs += " PWG2resonances.par";
   } else {
      gSystem->Load("libPWG2resonances.so");
      mylibs += " libPWG2resonances.so";
   }

   AliAnalysisAlien::SetupPar("ANALYSISaliceMV");
   mylibs += " ANALYSISaliceMV.par ";

//    AliAnalysisAlien::SetupPar("PWG2resonancesDevel");
//    mylibs += " PWG2resonancesDevel.par ";

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

   Bool_t usePhysSel = kTRUE;
   usePhysSel = kFALSE;

   Bool_t useMixing = kTRUE;
   useMixing = kFALSE;

   Bool_t useTender = kTRUE;
   useTender = kFALSE;

   Bool_t useRsnInputHandler = kTRUE;
//    useRsnInputHandler = kFALSE;

   if (useMultiHandler)  {


      if (useTender) {
         // add Tender Handler (not working with mixing)
         gROOT->LoadMacro("AddTenderHandler.C");
         AddTenderHandler(multiInputHandler);
      }

      if (useRsnInputHandler) {
         // add Rsn input handler (it has to be after ESD,MC,Tender input handler, but before Mixing)
         gROOT->LoadMacro("AddRsnInputHandler.C");
         AddRsnInputHandler(multiInputHandler);
      }

      if (useMixing) {
         // add mixing handler (uncomment to turn on Mixnig)
         gROOT->LoadMacro("AddMixingHandler.C");
         AddMixingHandler(multiInputHandler, format, useMC, opts);
      }
      // add Log Handler
      gROOT->LoadMacro("AddLogHanlder.C");
      AddLogHanlder(multiInputHandler);
   }

   if (usePhysSel) {
      gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C");
      AddTaskPhysicsSelection(useMC);

      // maybe we can put it in $ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C
      AliMultiInputEventHandler *multiIH = dynamic_cast<AliMultiInputEventHandler*>(mgr->GetInputEventHandler());
      if (multiIH)
         esdInputHandler->SetEventSelection(multiIH->GetEventSelection());
   }
   // load and run AddTask macro
   gROOT->LoadMacro("AddRsnTaskNewAll.C");
   AddRsnTaskNewAll(format, useMC);

}
