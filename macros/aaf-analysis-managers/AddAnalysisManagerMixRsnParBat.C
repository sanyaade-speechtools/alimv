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

void AddAnalysisManagerMixRsnParBat(TString analysisSource = "proof", TString analysisMode = "test", TString opts = "")
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
   gSystem->Load("libANALYSIS.so");
   gSystem->Load("libANALYSISalice.so");
   gSystem->Load("libCORRFW.so");
   gSystem->Load("libEventMixing.so");
   AliAnalysisAlien::SetupPar("PWG2resonances");
   AliAnalysisAlien::SetupPar("RESONANCESMV");

   gROOT->LoadMacro("AliRsnCustomTrackInfoBat.cxx+g");

//     analysisPlugin->AddIncludePath(gSystem->ExpandPathName("$ALICE_ROOT/include/EventMixing"));

   // sets additional settings to plubin
   analysisPlugin->SetAnalysisSource("AliRsnCustomTrackInfoBat.cxx");
   analysisPlugin->SetAdditionalLibs("libEventMixing.so PWG2resonances.par RESONANCESMV.par AliRsnCustomTrackInfoBat.h AliRsnCustomTrackInfoBat.cxx");

   analysisPlugin->SetAliRootMode("ALIROOT"); // Loads AF libs by default
   // sets plugin to manager
   mgr->SetGridHandler(analysisPlugin);

   Info("AddAnalysisManagerMixRsn.C", "Creating AliMultiInputEventHandler ...");
   AliESDInputHandler *mainInputHandler = new AliESDInputHandler();

   // add main input handler (with mixing handler)
   mgr->SetInputEventHandler(mainInputHandler);

   // adds all tasks
   gROOT->LoadMacro("AddAnalysisTaskAllRsnBat.C");
   AddAnalysisTaskAllRsnBat(format, useMC, opts);
}
