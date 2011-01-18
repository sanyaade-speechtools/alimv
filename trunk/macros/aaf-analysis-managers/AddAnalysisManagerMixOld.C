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

void AddAnalysisManagerMixOld(TString analysisSource = "proof", TString analysisMode = "test", TString opts = "")
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
    gROOT->ProcessLine(Form(".include %s/ANALYSIS/EventMixing",gSystem->ExpandPathName("$ALICE_ROOT")));
    
    gROOT->LoadMacro("AliAnalysisTaskCustomMix.cxx++g");
		gROOT->LoadMacro("AliAnalysisTaskCustomMixOld.cxx++g");

    analysisPlugin->AddIncludePath(gSystem->ExpandPathName("$ALICE_ROOT/ANALYSIS/EventMixing"));
    analysisPlugin->SetAliRootMode("ALIROOT"); // Loads AF libs by default
    // sets additional settings to plubin
    analysisPlugin->SetAnalysisSource("AliAnalysisTaskCustomMix.cxx AliAnalysisTaskCustomMixOld.cxx");
    analysisPlugin->SetAdditionalLibs("libEventMixing.so AliAnalysisTaskCustomMix.h AliAnalysisTaskCustomMix.cxx AliAnalysisTaskCustomMixOld.h AliAnalysisTaskCustomMixOld.cxx");

		// sets plugin to manager
    mgr->SetGridHandler(analysisPlugin);

    Info("AddAnalysisManagerMixRsn.C", "Creating esdInputHandler ...");
    AliESDInputHandler *mainInputHandler = new AliESDInputHandler();
    
    Int_t bufferSize = 5;
    Int_t mixNum = 1;
    AliMixInputEventHandler *mixHandler = new AliMixInputEventHandler(bufferSize,mixNum);
    mixHandler->SetInputHandlerForMixing(dynamic_cast<AliESDInputHandler*> (mainInputHandler));
    AliMixEventPool *evPool = new AliMixEventPool();
    
    
    AliMixEventCutObj *multi = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity, 1, 101, 10);
    AliMixEventCutObj *zvertex = new AliMixEventCutObj(AliMixEventCutObj::kZVertex, -5, 5, 1);
    
    evPool->AddCut(multi);
    evPool->AddCut(zvertex);
    
    // adds event pool (comment it and u will have default mixing)
    mixHandler->SetEventPool(evPool);
    
    //     add mixing handler (uncomment to turn on Mixnig)
    mainInputHandler->SetMixingHandler(mixHandler);

    // add main input handler (with mixing handler)
    mgr->SetInputEventHandler(mainInputHandler);

    // adds all tasks
    gROOT->LoadMacro("AddAnalysisTaskAllOld.C");
    AddAnalysisTaskAllOld(format, useMC, opts);
}
