#ifndef __CINT__//|
#include <AliAnalysisManager.h>//|
#include <AliMultiInputEventHandler.h>//|
#include <EventMixing/EventMixing/AliMixEventPool.h>//|
#include <EventMixing/EventMixing/AliMixEventCutObj.h>//|
#include <EventMixing/EventMixing/AliMixInputEventHandler.h>//|
#include <AliVEvent.h>//|
#endif//|

void AddMixingHandler(AliMultiInputEventHandler* multiInputHandler, TString format = "esd", Bool_t useMC = kFALSE, TString opts = "")
{

   if (!multiInputHandler) return;

   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   const Int_t bufferSize = 1;
   const Int_t mixNum = 3;
   AliMixInputEventHandler *mixHandler = new AliMixInputEventHandler(bufferSize, mixNum);
   mixHandler->SetInputHandlerForMixing(dynamic_cast<AliMultiInputEventHandler*>(mgr->GetInputEventHandler()));
   AliMixEventPool *evPool = new AliMixEventPool();

   AliMixEventCutObj *multi = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity, 2, 102, 10);
   AliMixEventCutObj *zvertex = new AliMixEventCutObj(AliMixEventCutObj::kZVertex, -5, 5, 1);

   evPool->AddCut(multi);
   evPool->AddCut(zvertex);

   // adds event pool (comment it and u will have default mixing)
   mixHandler->SetEventPool(evPool);

   mixHandler->SelectCollisionCandidates(AliVEvent::kAny);

   multiInputHandler->AddInputEventHandler(mixHandler);

   // adds mixing info task
   gROOT->LoadMacro("AddAnalysisTaskMixInfo.C");
   AddAnalysisTaskMixInfo(format, useMC, opts);
}
