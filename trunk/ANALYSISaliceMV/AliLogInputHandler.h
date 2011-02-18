//
// Class AliLogInputHandler
//
// AliLogInputHandler
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ALILOGINPUTHANDLER_H
#define ALILOGINPUTHANDLER_H

#include "AliInputEventHandler.h"

class AliLogInputHandler : public AliInputEventHandler {

public:
   AliLogInputHandler(const char *name = "name");
   virtual ~AliLogInputHandler();

   // From the interface
   virtual Bool_t  Init(Option_t *opt);
   virtual Bool_t  Init(TTree *tree, Option_t *opt);
   virtual Bool_t  BeginEvent(Long64_t entry);
   virtual Bool_t  FinishEvent();
   virtual Bool_t  Notify();
   virtual Bool_t  Notify(const char *path);

   void AddClassDebugLevel(const char *classname, Int_t level);

private:

   TObjArray   fClassLogSettings;
   AliLogInputHandler(const AliLogInputHandler& handler);
   AliLogInputHandler &operator=(const AliLogInputHandler &handler);

   ClassDef(AliLogInputHandler, 1)
};

#endif
