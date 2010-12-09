#include <TString.h>
#include <TROOT.h>
#include <TProof.h>
#include "AliAnalysisManager.h"
#include "AliAnalysisAlien.h"
#include "AliESDInputHandler.h"
#include <AliMCEventHandler.h>
// #include <AliMultiInputEventHandler.h>
// #include <AliMCEventHandler.h>
// #include <MIXMV/MIX/AliMixEventPool.h>

void AddMyAnalysisManagerCustomMixTask(TString analysisSource = "proof", TString analysisMode = "test", TString opts = "")
{

  Bool_t useMC = kFALSE;
  TString format = "esd";

  // ALICE stuff
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) mgr = new AliAnalysisManager("Martin Vala's AM");

  gROOT->LoadMacro("SetupAnalysisPlugin.C");
  AliAnalysisGrid *analysisPlugin = SetupAnalysisPlugin(analysisMode.Data());
  if (!analysisPlugin) return;

  gSystem->AddIncludePath(Form("-I\"%s/ANALYSIS/EventMixing\"", gSystem->Getenv("ALICE_ROOT")));
  gROOT->ProcessLine(Form(".include %s/ANALYSIS/EventMixing", gSystem->ExpandPathName("$ALICE_ROOT")));
  gSystem->Load("libEventMixing.so");
  gROOT->LoadMacro("AliAnalysisTaskCustomMix.cxx++g");
  
  analysisPlugin->SetAnalysisSource("AliAnalysisTaskCustomMix.cxx");
  analysisPlugin->SetAdditionalLibs("libEventMixing.so AliAnalysisTaskCustomMix.h AliAnalysisTaskCustomMix.cxx");
  mgr->SetGridHandler(analysisPlugin);

  Bool_t useDefaultHandlers = kTRUE;
//   useDefaultHandlers = kFALSE;

  AliMultiInputEventHandler *inputHandler = 0;
  if (useDefaultHandlers) {
    if (!InputHandlerSetup(format, useMC)) return;
  } else {
    inputHandler = new AliMultiInputEventHandler();
    Info("", "Creating esdInputHandler ...");
    AliESDInputHandler *esdInputHandler = new AliESDInputHandler();
//   Info("", "Creating mcInputHandler ...");
//   AliMCEventHandler* mcInputHandler = new AliMCEventHandler();

    inputHandler->AddInputEventHandler(esdInputHandler);
//     inputHandler->AddInputEventHandler(mcInputHandler);
    mgr->SetInputEventHandler(inputHandler);
  }


  Int_t bufferSize = 2;
  AliESDInputHandler *esdH = dynamic_cast<AliESDInputHandler*>(mgr->GetInputEventHandler());
//   if (!esdH) return;
//   AliMixEventInputHandler *esdMixH = new AliMixEventInputHandler(bufferSize);
  if (useDefaultHandlers) {
    esdMixH->SetInputHandlerForMixing((AliESDInputHandler*)mgr->GetInputEventHandler());
  } else {
    esdMixH->SetInputHandlerForMixing((AliMultiInputEventHandler*)mgr->GetInputEventHandler());
  }
  AliMixEventPool *evPool = new AliMixEventPool("MyPool");
//   AliMixEventCutObj *multi = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity,0,1000,1000);
  AliMixEventCutObj *multi = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity, 1, 101, 10);
  AliMixEventCutObj *zvertex = new AliMixEventCutObj(AliMixEventCutObj::kZVertex, -5, 5, 1);
//   AliMixEventCutObj *zvertex = new AliMixEventCutObj(AliMixEventCutObj::kZVertex,-500,500,1000);
//   AliMixEventCutObj *tracklets = new AliMixEventCutObj(AliMixEventCutObj::kNumberTracklets, 0, 100, 10);

//   mgr->SetDebugLevel(10);
  evPool->AddCut(multi);
  evPool->AddCut(zvertex);
//   evPool->AddCut(tracklets);


  evPool->Print();
  esdMixH->SetEventPool(evPool);
  if (useDefaultHandlers) {
    AliESDInputHandler *esdH = dynamic_cast<AliESDInputHandler*>(mgr->GetInputEventHandler());
    if (!esdH) return;
    esdH->SetMixingHandler(esdMixH);
  } else {
    inputHandler->SetMixingHandler(esdMixH);
  }
  
  gROOT->LoadMacro("AddAnalysisTaskMixInfo.C");
  AddAnalysisTaskMixInfo(format, useMC, opts);
  
  gROOT->LoadMacro("AddAnalysisTaskCustomMix.C");
  AddAnalysisTaskCustomMix(format, useMC, opts);

// load and run AddTask macro

}
