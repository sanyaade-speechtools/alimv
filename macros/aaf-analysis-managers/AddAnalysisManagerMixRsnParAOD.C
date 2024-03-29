#include <TString.h>//|
#include <TROOT.h>//|
#include <TProof.h>//|
#include "AliAnalysisManager.h"//|
#include "AliAnalysisAlien.h"//|
#include <AliAODInputHandler.h>//|
#include <EventMixing/EventMixing/AliMixInputEventHandler.h>//|
#include <EventMixing/EventMixing/AliMixEventPool.h>//|
#include <EventMixing/EventMixing/AliMixEventCutObj.h>//|


void AddAnalysisManagerMixRsnParAOD(TString analysisSource = "proof", TString analysisMode = "test", TString opts = "")
{

   Bool_t useMC = kFALSE;
   TString format = "aod";

   // ALICE stuff
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) mgr = new AliAnalysisManager("Martin Vala's AM");

   gROOT->LoadMacro("SetupAnalysisPlugin.C");
   AliAnalysisGrid *analysisPlugin = SetupAnalysisPlugin(analysisMode.Data());
   if (!analysisPlugin) return;

   gSystem->Load("libXMLParser.so");
   gSystem->Load("libANALYSIS.so");
   gSystem->Load("libAOD.so");
   gSystem->Load("libANALYSISalice.so");
   gSystem->Load("libCORRFW.so");
   gSystem->Load("libEventMixing.so");
//     gSystem->Load("libPWG2resonances.so");
//    AliAnalysisAlien::SetupPar("EventMixing");
   AliAnalysisAlien::SetupPar("PWG2resonances");
//    AliAnalysisAlien::SetupPar("RESONANCESMV");

   analysisPlugin->SetAliRootMode("ALIROOT"); // Loads AF libs by default
   // sets additional settings to plubin
   analysisPlugin->SetAdditionalLibs("libXMLParser.so libCORRFW.so libEventMixing.so PWG2resonances.par");

   // sets plugin to manager
   mgr->SetGridHandler(analysisPlugin);

   Info("AddAnalysisManagerMixRsn.C", "Creating AliMultiInputEventHandler ...");
   AliMultiInputEventHandler *mainInputHandler = new AliMultiInputEventHandler();
   Info("AddAnalysisManagerMixRsn.C", "Creating esdInputHandler ...");
   AliAODInputHandler *aodInputHandler = new AliAODInputHandler();
   mainInputHandler->AddInputEventHandler(aodInputHandler);
//
//             add main input handler (with mixing handler)
   mgr->SetInputEventHandler(mainInputHandler);
   gROOT->LoadMacro("MixingSetting.C");
   mainInputHandler->AddInputEventHandler(MixingSetting());

//     if (!InputHandlerSetup(format, useMC)) return;


   // adds all tasks
   gROOT->LoadMacro("AddAnalysisTaskAllRsn.C");
   AddAnalysisTaskAllRsn(format, useMC, opts);
}
