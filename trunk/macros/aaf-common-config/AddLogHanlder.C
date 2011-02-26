#ifndef __CINT__//|
#include <AliLog.h>//|
#include <ANALYSISaliceMV/AliLogInputHandler.h>//|
#endif//|
void AddLogHanlder(AliMultiInputEventHandler *multiInputHandler)
{

   // adds log handler
   if (multiInputHandler) {
      AliLogInputHandler *logHandler = new AliLogInputHandler();
//       logHandler->AddClassDebugLevel("AliAnalysisTaskEx02", AliLog::kDebug);
//       logHandler->AddClassDebugLevel("AliMixEventPool", AliLog::kDebug + 1);
//       logHandler->AddClassDebugLevel("AliMixEventCutObj", AliLog::kDebug + 1);
      logHandler->AddClassDebugLevel("AliAnalysisTaskMixInfo", AliLog::kDebug + 1);

//       logHandler->AddClassDebugLevel("AliTenderInputEventHandler", AliLog::kDebug + 5);
//       logHandler->AddClassDebugLevel("AliMixInputEventHandler", AliLog::kDebug);

//       logHandler->AddClassDebugLevel("AliTriggerAnalysis", AliLog::kFatal);
      multiInputHandler->AddInputEventHandler(logHandler);
   }
}
