//
// Class AliEmptyInputHandler
//
// AliEmptyInputHandler
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIEMPTYINPUTHANDLER_H
#define ALIEMPTYINPUTHANDLER_H

#include "AliInputEventHandler.h"

class AliEmptyInputHandler : public AliInputEventHandler {

public:
   AliEmptyInputHandler(const char *name = "PIDResoponseIH");
   virtual ~AliEmptyInputHandler();

   // From the interface
   virtual Bool_t  Init(Option_t *opt);
   virtual Bool_t  Init(TTree *tree, Option_t *opt);
   virtual Bool_t  BeginEvent(Long64_t entry);
   virtual Bool_t  FinishEvent();
   virtual Bool_t  Notify();
   virtual Bool_t  Notify(const char *path);
   virtual Bool_t  GetEntry();

private:

   AliEmptyInputHandler(const AliEmptyInputHandler& handler);
   AliEmptyInputHandler &operator=(const AliEmptyInputHandler &handler);

   ClassDef(AliEmptyInputHandler, 1)
};

#endif
