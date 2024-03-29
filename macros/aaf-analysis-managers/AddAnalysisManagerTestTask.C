#include <TString.h>//|
#include <TROOT.h>//|
#include <TProof.h>//|
#include "AliAnalysisManager.h"//|
#include "AliAnalysisAlien.h"//|
#include "AliESDInputHandler.h"//|
#include "AliMCEventHandler.h"//|
#include "AliAODInputHandler.h"//|

void AddAnalysisManagerTestTask(TString analysisSource = "proof", TString analysisMode = "test", TString opts = "")
{

   Bool_t useMC = kFALSE;
   TString format = "esd";
   format = "aod";

   // ALICE stuff
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) mgr = new AliAnalysisManager("Martin Vala's AM");

   gROOT->LoadMacro("SetupAnalysisPlugin.C");
   AliAnalysisGrid *analysisPlugin = SetupAnalysisPlugin(analysisMode.Data());
   if (!analysisPlugin) return;

   // load par files localy
   gSystem->Load("libANALYSIS.so");
   gSystem->Load("libANALYSISalice.so");

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

   Int_t bufferSize = 1;
   Int_t mixNum = 5;
   AliMixInputEventHandler *mixHandler = new AliMixInputEventHandler(bufferSize, mixNum);
   mixHandler->SetInputHandlerForMixing(dynamic_cast<AliMultiInputEventHandler*>(mainInputHandler));
   AliMixEventPool *evPool = new AliMixEventPool();


   AliMixEventCutObj *multi = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity, 1, 101, 10);
   AliMixEventCutObj *zvertex = new AliMixEventCutObj(AliMixEventCutObj::kZVertex, -5, 5, 1);

   evPool->AddCut(multi);
   evPool->AddCut(zvertex);

   // adds event pool (comment it and u will have default mixing)
   mixHandler->SetEventPool(evPool);

   //     add mixing handler (uncomment to turn on Mixnig)
   mainInputHandler->AddInputEventHandler(mixHandler);

   // add main input handler (with mixing handler)
   mgr->SetInputEventHandler(mainInputHandler);

   // adds all tasks
   gROOT->LoadMacro("AddAnalysisTaskAll.C");
   AddAnalysisTaskAll(format, useMC, opts);
}
