#ifndef __CINT__//|
#include <AliAnalysisManager.h>//|
#include <EventMixing/EventMixing/AliMixEventPool.h>//|
#include <EventMixing/EventMixing/AliMixEventCutObj.h>//|
#endif//|

AliMixInputEventHandler *MixingSetting()
{

   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   Int_t bufferSize = 1;
   Int_t mixNum = 5;
   AliMixInputEventHandler *mixHandler = new AliMixInputEventHandler(bufferSize, mixNum);
   mixHandler->SetInputHandlerForMixing(dynamic_cast<AliMultiInputEventHandler*>(mgr->GetInputEventHandler()));
   AliMixEventPool *evPool = new AliMixEventPool();

   AliMixEventCutObj *multi = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity, 1, 201, 20);
   AliMixEventCutObj *zvertex = new AliMixEventCutObj(AliMixEventCutObj::kZVertex, -5, 5, 1);

   evPool->AddCut(multi);
   evPool->AddCut(zvertex);

   // adds event pool (comment it and u will have default mixing)
   mixHandler->SetEventPool(evPool);

   return mixHandler;
}
