#ifndef __CINT__//|
#include "ANALYSISaliceMV/AliPIDResponseInputHandler.h" //|
#endif//|
void AddPIDResponseInputHandler(AliMultiInputEventHandler *multiInputHandler)
{
   if (multiInputHandler) {
      AliPIDResponseInputHandler *pidResponseIH = new AliPIDResponseInputHandler();
      multiInputHandler->AddInputEventHandler(pidResponseIH);
   }
}
