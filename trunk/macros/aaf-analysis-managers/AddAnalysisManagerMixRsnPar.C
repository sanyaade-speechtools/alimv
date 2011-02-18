#ifndef __CINT__//|
#include <TString.h>//|
#include <TROOT.h>//|
#include <TProof.h>//|
#include "AliAnalysisManager.h"//|
#include "AliAnalysisAlien.h"//|
#include "AliESDInputHandler.h"//|
#include "AliMCEventHandler.h"//|
#include <ANALYSISaliceMV/AliMultiInputEventHandler.h>//|
#include <EventMixing/EventMixing/AliMixInputEventHandler.h>//|
#include <EventMixing/EventMixing/AliMixEventPool.h>//|
#include <EventMixing/EventMixing/AliMixEventCutObj.h>//|
#endif//|
void AddAnalysisManagerMixRsnPar(TString analysisSource = "proof", TString analysisMode = "test", TString opts = "")
{

   Bool_t useMC = kFALSE;
   TString format = "esd";

   // ALICE stuff
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) mgr = new AliAnalysisManager("Martin Vala's AM");

   gROOT->LoadMacro("SetupAnalysisPlugin.C");
   AliAnalysisGrid *analysisPlugin = SetupAnalysisPlugin(analysisMode.Data());
   if (!analysisPlugin) return;

   gSystem->Load("libXMLParser.so");
   gSystem->Load("libAOD.so");
   gSystem->Load("libANALYSIS.so");
   gSystem->Load("libANALYSISalice.so");
   gSystem->Load("libCORRFW.so");
   gSystem->Load("libTOFbase.so");
   gSystem->Load("libTOFrec.so");
   gSystem->Load("libTOFsim.so");

   // Loads AF libs by default
   analysisPlugin->SetAliRootMode("ALIROOT");

   Bool_t useEventMixingPar = kTRUE;
//    useEventMixingPar = kFALSE;

   Bool_t useTENDERPar = kTRUE;
   useTENDERPar = kFALSE;

//    if (useTENDERPar) {
//       AliAnalysisAlien::SetupPar("TENDER");
//       AliAnalysisAlien::SetupPar("TENDERSupplies");
//    }
//    else {
//       gSystem->Load("libTENDER.so");
//       gSystem->Load("libTENDERSupplies.so");
//    }

   if (useEventMixingPar) AliAnalysisAlien::SetupPar("EventMixing");
   else gSystem->Load("libEventMixing.so");

//    AliAnalysisAlien::SetupPar("ANALYSISaliceMV");
   AliAnalysisAlien::SetupPar("PWG2resonances");

   TString mylibs = "libXMLParser.so libCORRFW.so";
   mylibs += " libTOFbase.so libTOFsim.so libTOFrec.so";

//    if (useTENDERPar) mylibs += " TENDER.par TENDERSupplies.par";
//    else mylibs += " libTENDER.so libTENDERSupplies.so";

   if (useEventMixingPar) mylibs += " EventMixing.par";
   else mylibs += " libEventMixing.so";

//    mylibs += " ANALYSISaliceMV.par";
   mylibs += " PWG2resonances.par";

   analysisPlugin->SetAdditionalLibs(mylibs.Data());

   // sets plugin to manager
   mgr->SetGridHandler(analysisPlugin);

   Info("AddAnalysisManagerMixRsn.C", "Creating AliMultiInputEventHandler ...");
   AliMultiInputEventHandler *mainInputHandler = new AliMultiInputEventHandler();
   Info("AddAnalysisManagerMixRsn.C", "Creating esdInputHandler ...");
   AliESDInputHandler *esdInputHandler = new AliESDInputHandler();
   mainInputHandler->AddInputEventHandler(esdInputHandler);

   if (useMC) {
      Info("AddAnalysisManagerMixRsn.C", "Creating mcInputHandler ...");
      AliMCEventHandler* mcInputHandler = new AliMCEventHandler();
      mainInputHandler->AddInputEventHandler(mcInputHandler);
   }

   // add Tender Handler
//    gROOT->LoadMacro("AddTenderHandler.C");
// //    AddTenderHandler(mainInputHandler);

   // add main input handler (with mixing handler)
   mgr->SetInputEventHandler(mainInputHandler);

   // add mixing handler (uncomment to turn on Mixnig)
   gROOT->LoadMacro("AddMixingHandler.C");
   AddMixingHandler(mainInputHandler);

   // add Log Handler
//    gROOT->LoadMacro("AddLogHanlder.C");
//    AddLogHanlder(mainInputHandler);

   // adds all tasks
   gROOT->LoadMacro("AddAnalysisTaskAllRsn.C");
   AddAnalysisTaskAllRsn(format, useMC, opts);
}
