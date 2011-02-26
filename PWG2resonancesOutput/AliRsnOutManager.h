#ifndef ALIRSNOUTMANAGER_H
#define ALIRSNOUTMANAGER_H

#include <TNamed.h>
#include <TObjArray.h>
#include "AliRsnOutInput.h"
#include "AliRsnOutParamHist.h"

class TCanvas;
class AliRsnOutPad;
class AliRsnOutManager : public TNamed {

public:
   AliRsnOutManager(const char* name, const char* title);
   virtual void Print(Option_t* option = "") const;

   void SetOutputSettings(Double_t min = 0.0, Double_t max = 0.0, Double_t step = 0.0, Int_t dim = -1, Int_t cutDim = -1);

   virtual void Draw(Option_t* option = "");


   void AddInput(AliRsnOutInput *input) { fInputs.Add(input);;}
   TObjArray *GetInputs() { return &fInputs;}

   void AddParamOutput(AliRsnOutParamHist *input) { fParamOutputs.Add(input);}
   TObjArray *GetParamOutputs() { return &fParamOutputs;}

   Double_t GetMin() { return fMin;}
   Double_t GetMax() { return fMax;}
   Double_t GetStep() { return fStep;}
   Int_t GetValDim() { return fValDimension; }
   Int_t GetCutDim() { return fCutDimension; }

   void SetPad(AliRsnOutPad*pad) { fPad = pad; }

private:

   AliRsnOutPad      *fPad;

   TObjArray         fInputs;
   Double_t          fMin;
   Double_t          fMax;
   Double_t          fStep;
   Int_t             fValDimension;
   Int_t             fCutDimension;
   Int_t             fCount;

   TObjArray         fParamOutputs;

   ClassDef(AliRsnOutManager, 1) // AliRsnOutManager
};

#endif // ALIRSNOUTMANAGER_H
