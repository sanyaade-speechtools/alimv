//
// Class AliRsnInputHandler
//
// AliRsnInputHandler
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIRSNINPUTHANDLER_H
#define ALIRSNINPUTHANDLER_H

#include "AliInputEventHandler.h"
#include "AliRsnPIDManager.h"

class AliRsnEvent;
class AliRsnInputHandler : public AliInputEventHandler {

public:
   AliRsnInputHandler(const char *name = "name");
   virtual ~AliRsnInputHandler();

   // From the interface
   virtual Bool_t  Init(Option_t *opt);
   virtual Bool_t  Init(TTree *tree, Option_t *opt);
   virtual Bool_t  BeginEvent(Long64_t entry);
   virtual Bool_t  FinishEvent();
   virtual Bool_t  Notify();
   virtual Bool_t  Notify(const char *path);
   virtual Bool_t  GetEntry();

   AliRsnEvent *GetRsnEvent() { return fRsnEvent; }
   AliRsnPIDManager *GetPIDManager() { return &fRsnPIDManager; }

private:

   AliRsnEvent *fRsnEvent; //!
   AliRsnPIDManager fRsnPIDManager;

   AliRsnInputHandler(const AliRsnInputHandler& handler);
   AliRsnInputHandler &operator=(const AliRsnInputHandler &handler);

   ClassDef(AliRsnInputHandler, 1)
};

#endif
