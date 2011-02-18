//
// Class AliPhysicsSellectionInputHandler
//
// AliPhysicsSellectionInputHandler
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIPHYSICSSELLECTIONINPUTHANDLER_H
#define ALIPHYSICSSELLECTIONINPUTHANDLER_H

#include "AliInputEventHandler.h"

class AliPhysicsSellectionInputHandler : public AliInputEventHandler {

public:
   AliPhysicsSellectionInputHandler(const char *name = "name");
   virtual ~AliPhysicsSellectionInputHandler();

   // From the interface
   virtual Bool_t  Init(Option_t *opt);
   virtual Bool_t  Init(TTree *tree, Option_t *opt);
   virtual Bool_t  BeginEvent(Long64_t entry);
   virtual Bool_t  FinishEvent();
   virtual Bool_t  Notify();
   virtual Bool_t  Notify(const char *path);

private:
   AliPhysicsSellectionInputHandler(const AliPhysicsSellectionInputHandler& handler);
   AliPhysicsSellectionInputHandler &operator=(const AliPhysicsSellectionInputHandler &handler);

   ClassDef(AliPhysicsSellectionInputHandler, 1)
};

#endif
