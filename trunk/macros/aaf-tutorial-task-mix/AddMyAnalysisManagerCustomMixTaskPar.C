#include <TString.h>//|
#include <TROOT.h>//|
#include <TProof.h>//|
#include "AliAnalysisManager.h"//|
#include "AliAnalysisAlien.h"//|
#include "AliESDInputHandler.h"//|
#include "AliMCEventHandler.h"//|
#include "AliMixInputEventHandler.h"//|
#include "AliMixEventPool.h"//|
#include "AliMixEventCutObj.h" //|
void AddMyAnalysisManagerCustomMixTaskPar(TString analysisSource = "proof", TString analysisMode = "test", TString opts = "")
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
    // load par files localy
//     gROOT->ProcessLine(".x $(ALICE_ROOT)/macros/loadlibs.C");
    gSystem->Load("libANALYSIS.so");
    gSystem->Load("libANALYSISalice.so");
    gSystem->Load("libCORRFW.so");
//     gSystem->Load("libPWG2resonances.so");
    AliAnalysisAlien::SetupPar("PWG2resonances");
    AliAnalysisAlien::SetupPar("ANALYSISaliceMV");
    AliAnalysisAlien::SetupPar("EventMixing");
		AliAnalysisAlien::SetupPar("RESONANCESMV");

    // load our task localy
    gROOT->LoadMacro("AliAnalysisTaskCustomMix.cxx++g");

    
//     analysisPlugin->SetAliRootMode("ALIROOT"); // Loads AF libs by default
    // sets additional settings to plubin
    analysisPlugin->SetAnalysisSource("AliAnalysisTaskCustomMix.cxx+");
    analysisPlugin->SetAdditionalLibs("libCORRFW.so ANALYSISaliceMV.par EventMixing.par AliAnalysisTaskCustomMix.h AliAnalysisTaskCustomMix.cxx");
    
//     analysisPlugin->SetAdditionalLibs("libCORRFW.so libPWG2resonances.so ANALYSISaliceMV.par EventMixing.par RESONANCESMV.par AliAnalysisTaskCustomMix.h AliAnalysisTaskCustomMix.cxx");
//     analysisPlugin->SetAdditionalLibs("libXMLParser.so libCORRFW.so PWG2resonances.par ANALYSISaliceMV.par EventMixing.par RESONANCESMV.par AliAnalysisTaskCustomMix.h AliAnalysisTaskCustomMix.cxx");
    
    // sets plugin to manager
    mgr->SetGridHandler(analysisPlugin);

    AliMultiInputEventHandler *inputHandler = 0;
    inputHandler = new AliMultiInputEventHandler();
    Info("", "Creating esdInputHandler ...");
    AliESDInputHandler *esdInputHandler = new AliESDInputHandler();
    inputHandler->AddInputEventHandler(esdInputHandler);

    if (useMC) {
      Info("", "Creating mcInputHandler ...");
      AliMCEventHandler* mcInputHandler = new AliMCEventHandler();
      inputHandler->AddInputEventHandler(mcInputHandler);
    }
    mgr->SetInputEventHandler(inputHandler);

    Int_t bufferSize = 1;
    Int_t mixNum = 5;
    AliMixInputEventHandler *mixHandler = new AliMixInputEventHandler(bufferSize,mixNum);
    mixHandler->SetInputHandlerForMixing(dynamic_cast<AliMultiInputEventHandler*> (mgr->GetInputEventHandler()));
    AliMixEventPool *evPool = new AliMixEventPool();
    AliMixEventCutObj *multi = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity, 1, 101, 10);
    AliMixEventCutObj *zvertex = new AliMixEventCutObj(AliMixEventCutObj::kZVertex, -5, 5, 1);

    evPool->AddCut(multi);
    evPool->AddCut(zvertex);

    // adds event pool (comment it and u will have default mixing)
    mixHandler->SetEventPool(evPool);

    inputHandler->AddInputEventHandler(mixHandler);

//     // adds mixing info task
    gROOT->LoadMacro("AddAnalysisTaskMixInfo.C");
    AddAnalysisTaskMixInfo(format, useMC, opts);
// 
// 
//     // add our taks
    gROOT->LoadMacro("AddAnalysisTaskCustomMix.C");
    AddAnalysisTaskCustomMix(format, useMC, opts);

		// add our taks
//     gROOT->LoadMacro("AddRsnTask.C");
//     AddRsnTask(format, useMC, opts);
    
//     gROOT->LoadMacro("AddRsnTaskMix.C");
//     AddRsnTaskMix(format, useMC, opts);
		
}
