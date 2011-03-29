//
// Class AliCentralityInputHandler
//
// AliCentralityInputHandler
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ALICENTRALITYINPUTHANDLER_H
#define ALICENTRALITYINPUTHANDLER_H

#include "AliInputEventHandler.h"

class AliCentralityInputHandler : public AliInputEventHandler {

public:
   AliCentralityInputHandler(const char *name = "PIDResoponseIH");
   virtual ~AliCentralityInputHandler();

   // From the interface
   virtual Bool_t  Init(Option_t *opt);
   virtual Bool_t  Init(TTree *tree, Option_t *opt);
   virtual Bool_t  BeginEvent(Long64_t entry);
   virtual Bool_t  FinishEvent();
   virtual Bool_t  Notify();
   virtual Bool_t  Notify(const char *path);
   virtual Bool_t  GetEntry();

private:

   AliCentralityInputHandler(const AliCentralityInputHandler& handler);
   AliCentralityInputHandler &operator=(const AliCentralityInputHandler &handler);

   ClassDef(AliCentralityInputHandler, 1)
};

#endif
