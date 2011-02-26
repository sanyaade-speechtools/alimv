#ifndef ALIRSNOUTFUNCTION_H
#define ALIRSNOUTFUNCTION_H

#include <TNamed.h>
#include <TH1.h>
#include <TObjArray.h>

class TObjArray;
class AliRsnOutManager;
class AliRsnOutFunction : public TNamed {

public:
   AliRsnOutFunction();
   AliRsnOutFunction(const char* name, const char* title = "");

   virtual void Print(Option_t* option = "") const;

   TH1 *CreateHistogram(AliRsnOutManager* mgr, Int_t valDim, Int_t cutDim, Double_t min, Double_t max);

private:

   ClassDef(AliRsnOutFunction, 1) // ALIRSNOUTFUNCTION_H
};

#endif // ALIRSNOUTREADER_H
