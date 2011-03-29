#ifndef __CINT__//|
#include "ANALYSISaliceMV/AliCentralityInputHandler.h" //|
#endif//|
void AddCentralityInputHandler(AliMultiInputEventHandler *multiInputHandler)
{
   if (multiInputHandler) {
      AliCentralityInputHandler *centralityIH = new AliCentralityInputHandler();
      multiInputHandler->AddInputEventHandler(centralityIH);
   }
}

