#ifndef ALIRSNOUTMANAGER_H
#define ALIRSNOUTMANAGER_H

#include <TNamed.h>
#include <TObjArray.h>
#include "AliRsnOutDraw.h"

class TCanvas;
class TVirtualPad;
class AliRsnOutReader;
class AliRsnOutManager : public TNamed
{

public:
    AliRsnOutManager(const char* name, const char* title);
    virtual void Print(Option_t* option = "") const;
    
    void AddOutput(AliRsnOutReader*reader);
    Bool_t SetScheme(TString scheme);

    void PrepareOutput(Int_t dim = -1, Int_t cutDim = -1, Double_t cutMin = 0.0, Double_t cutMax = 0.0, TVirtualPad* pad = 0, TString opts="");
    
    void FinalOutput(Double_t min = 0.0, Double_t max = 0.0,Double_t step = 0.0,Int_t dim = -1, Int_t cutDim = -1, TCanvas *canvas=0);
    
    AliRsnOutDraw *GetDrawer() { return &fDrawer;}
    
private:
  TObjArray       fOutputs;
  TString         fSheme;
  AliRsnOutDraw   fDrawer;
    
    ClassDef(AliRsnOutManager, 1) // AliRsnOutManager
};

#endif // ALIRSNOUTMANAGER_H
