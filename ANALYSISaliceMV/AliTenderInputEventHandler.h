//
// Class AliTenderInputEventHandler
//
// AliTenderInputEventHandler
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ALITENDERINPUTEVENTHANDLER_H
#define ALITENDERINPUTEVENTHANDLER_H

#include "AliCDBManager.h"
#include "AliESDInputHandler.h"
#include "AliTenderSupply.h"

#include "AliInputEventHandler.h"

#include "AliMultiInputEventHandler.h"
#include "AliTender.h"
class AliTenderInputEventHandler : public AliInputEventHandler {

public:

   AliTenderInputEventHandler(const char *name = "name");
   virtual ~AliTenderInputEventHandler();

   // From the interface
   virtual Bool_t  Init(Option_t *opt);
   virtual Bool_t  Init(TTree *tree, Option_t *opt);
   virtual Bool_t  BeginEvent(Long64_t entry);
   virtual Bool_t  FinishEvent();

   virtual Bool_t  Notify();
   virtual Bool_t  Notify(const char *path);

   AliTender *GetTender() { return &fTender;}

   void DoInit();
   void DoProcess();

private:

   AliTender fTender;

   AliTenderInputEventHandler(const AliTenderInputEventHandler& handler);
   AliTenderInputEventHandler &operator=(const AliTenderInputEventHandler &handler);

   ClassDef(AliTenderInputEventHandler, 1)
};

#endif
