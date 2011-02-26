#ifndef ALIRSNOUTPAD_H
#define ALIRSNOUTPAD_H

#include <TNamed.h>
#include <TObjArray.h>

class TLegend;
class AliRsnOutManager;
class AliRsnOutDrawObject;
class TVirtualPad;
class AliRsnOutPad : public TNamed {

public:
   AliRsnOutPad(const char* name = "", const char *title = "");

   virtual void Print(Option_t* option = "") const;
   void DrawPad(AliRsnOutManager* rsnOutMgr, Int_t valDim, Int_t cutDim, Double_t min, Double_t max, TVirtualPad* pad = 0, TString opt = "");

   void Add(AliRsnOutDrawObject *obj);

   TObjArray *GetList() { return &fList;}

   void DrawZeroLine(Bool_t drawZeroLine = kTRUE) { fDrawZeroLine = drawZeroLine;}

   void SetLegend(TLegend *legend) { fLegend = legend;}

private:

   TObjArray fList;

   Bool_t fDrawZeroLine;

   TLegend *fLegend;

   ClassDef(AliRsnOutPad, 1) // AliRsnOutPad
};

#endif // ALIRSNOUTPAD_H

