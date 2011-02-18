//
// Class AliPhysicsSelectionInputHandler
//
// AliPhysicsSelectionInputHandler
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIPHYSICSSELECTIONINPUTHANDLER_H
#define ALIPHYSICSSELECTIONINPUTHANDLER_H

#include "AliInputEventHandler.h"
class fPhysicsSelection;
class AliPhysicsSelectionInputHandler : public AliInputEventHandler {

public:
   AliPhysicsSelectionInputHandler();
   AliPhysicsSelectionInputHandler(const char *name,const char *opt="");
   virtual ~AliPhysicsSelectionInputHandler();

   // From the interface
   virtual Bool_t  Init(Option_t *opt);
   virtual Bool_t  Init(TTree *tree, Option_t *opt);
   virtual Bool_t  BeginEvent(Long64_t entry);
   virtual Bool_t  FinishEvent();
   virtual Bool_t  Notify();
   virtual Bool_t  Notify(const char *path);

private:
   TString 		fOption;           // option string  
   AliPhysicsSelection *fPhysicsSelection; // event selection class
  
   AliPhysicsSelectionInputHandler(const AliPhysicsSelectionInputHandler& handler);
   AliPhysicsSelectionInputHandler &operator=(const AliPhysicsSelectionInputHandler &handler);

   ClassDef(AliPhysicsSelectionInputHandler, 1)
};

#endif
