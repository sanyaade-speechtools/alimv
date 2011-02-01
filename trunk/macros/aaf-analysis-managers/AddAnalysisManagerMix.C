#include <TString.h>//|
#include <TROOT.h>//|
#include <TProof.h>//|
#include "AliAnalysisManager.h"//|
#include "AliAnalysisAlien.h"//|
#include "AliESDInputHandler.h"//|
#include "AliMCEventHandler.h"//|
#include <EventMixing/EventMixing/AliMultiInputEventHandler.h>//|
#include <EventMixing/EventMixing/AliMixInputEventHandler.h>//|
#include <EventMixing/EventMixing/AliMixEventPool.h>//|
#include <EventMixing/EventMixing/AliMixEventCutObj.h>//|

void AddAnalysisManagerMix(TString analysisSource = "proof", TString analysisMode = "test", TString opts = "")
{

   Bool_t useMC = kFALSE;
   TString format = "esd";

   // ALICE stuff
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) mgr = new AliAnalysisManager("Martin Vala's AM");

   gROOT->LoadMacro("SetupAnalysisPlugin.C");
   AliAnalysisGrid *analysisPlugin = SetupAnalysisPlugin(analysisMode.Data());
   if (!analysisPlugin) return;

   // load par files localy
   gSystem->Load("libANALYSIS.so");
   gSystem->Load("libANALYSISalice.so");

   gSystem->Load("libEventMixing.so");
   // load our task localy

   gSystem->AddIncludePath(Form("-I\"%s/ANALYSIS/EventMixing\"", gSystem->Getenv("ALICE_ROOT")));
   gROOT->ProcessLine(Form(".include %s/ANALYSIS/EventMixing", gSystem->ExpandPathName("$ALICE_ROOT")));

   gROOT->LoadMacro("AliAnalysisTaskCustomMix.cxx+g");

   analysisPlugin->AddIncludePath(gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/EventMixing"));
   analysisPlugin->SetAliRootMode("ALIROOT"); // Loads AF libs by default
   // sets additional settings to plubin
   analysisPlugin->SetAnalysisSource("AliAnalysisTaskCustomMix.cxx");
   analysisPlugin->SetAdditionalLibs("libEventMixing.so AliAnalysisTaskCustomMix.h AliAnalysisTaskCustomMix.cxx");

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

   // add main input handler (with mixing handler)
   mgr->SetInputEventHandler(mainInputHandler);

   // add mixing handler (uncomment to turn on Mixnig)
   gROOT->LoadMacro("MixingSetting.C");
   mainInputHandler->AddInputEventHandler(MixingSetting());

   // adds all tasks
   gROOT->LoadMacro("AddAnalysisTaskAll.C");
   AddAnalysisTaskAll(format, useMC, opts);
}
