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
   gSystem->Load("libTOFbase.so");
   gSystem->Load("libTOFrec.so");
   gSystem->Load("libTOFsim.so");
   gSystem->Load("libCORRFW.so");

   Bool_t useEventMixingPar = kTRUE;
   useEventMixingPar = kFALSE;

   if (useEventMixingPar) AliAnalysisAlien::SetupPar("EventMixing");
   else gSystem->Load("libEventMixing.so");
   AliAnalysisAlien::SetupPar("PWG2resonances");

   // Loads AF libs by default
   analysisPlugin->SetAliRootMode("ALIROOT");
   // sets additional settings to plubin
   if (useEventMixingPar) analysisPlugin->SetAdditionalLibs("libXMLParser.so libTOFbase.so libTOFrec.so libTOFsim.so libCORRFW.so EventMixing.par PWG2resonances.par");
   else analysisPlugin->SetAdditionalLibs("libXMLParser.so libTOFbase.so libTOFrec.so libTOFsim.so libCORRFW.so libEventMixing.so PWG2resonances.par");

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
   gROOT->LoadMacro("AddAnalysisTaskAllRsn.C");
   AddAnalysisTaskAllRsn(format, useMC, opts);
}
