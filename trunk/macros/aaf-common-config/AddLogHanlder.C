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
//       logHandler->AddClassDebugLevel("AliMixInputEventHandler", AliLog::kDebug+5);
//       logHandler->AddClassDebugLevel("AliMixInputHandlerInfo", AliLog::kDebug+5);
//       logHandler->AddClassDebugLevel("AliRsnInputHandler", AliLog::kDebug+5);
//       logHandler->AddClassDebugLevel("AliTriggerAnalysis", AliLog::kFatal);
//       logHandler->AddClassDebugLevel("AliRsnPair", AliLog::kDebug+5);
//       logHandler->AddClassDebugLevel("AliRsnValue", AliLog::kDebug+5);
//       logHandler->AddClassDebugLevel("AliRsnAnalysisTask", AliLog::kDebug);
      multiInputHandler->AddInputEventHandler(logHandler);
   }
}
