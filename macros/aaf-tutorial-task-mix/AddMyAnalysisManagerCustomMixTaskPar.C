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

    // load par files localy
    AliAnalysisAlien::SetupPar("ANALYSISaliceMV");
    AliAnalysisAlien::SetupPar("EventMixing");

    // load our task localy
    gROOT->LoadMacro("AliAnalysisTaskCustomMix.cxx++g");

    // sets additional settings to plubin
    analysisPlugin->SetAnalysisSource("AliAnalysisTaskCustomMix.cxx+");
    analysisPlugin->SetAdditionalLibs("ANALYSISaliceMV.par EventMixing.par AliAnalysisTaskCustomMix.h AliAnalysisTaskCustomMix.cxx");

    // sets plugin to manager
    mgr->SetGridHandler(analysisPlugin);

    AliMultiInputEventHandler *inputHandler = 0;
    inputHandler = new AliMultiInputEventHandler();
    Info("", "Creating esdInputHandler ...");
    AliESDInputHandler *esdInputHandler = new AliESDInputHandler();
    Info("", "Creating mcInputHandler ...");
    AliMCEventHandler* mcInputHandler = new AliMCEventHandler();

    inputHandler->AddInputEventHandler(esdInputHandler);
    inputHandler->AddInputEventHandler(mcInputHandler);
    mgr->SetInputEventHandler(inputHandler);

    Int_t bufferSize = 1;
    Int_t mixNum = 2;
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

    // adds mixing info task
    gROOT->LoadMacro("AddAnalysisTaskMixInfo.C");
    AddAnalysisTaskMixInfo(format, useMC, opts);


    // add our taks
    gROOT->LoadMacro("AddAnalysisTaskCustomMix.C");
    AddAnalysisTaskCustomMix(format, useMC, opts);

}
