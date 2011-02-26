#ifndef ALIRSNOUTPARAMHIST_H
#define ALIRSNOUTPARAMHIST_H
#include <TNamed.h>
#include <TString.h>
#include <TStyle.h>
#include <Rtypes.h>

class TVirtualPad;
class TH1;
class AliRsnOutDrawObject;
class AliRsnOutParamHist : public TNamed {

public:
   enum ERsnOutType { kHistIntegral = 0, kFitFunParam = 1};

   AliRsnOutParamHist(AliRsnOutDrawObject *drawObj, const char*name, const char* title);

   virtual void Print(Option_t* option = "") const;
   void DrawOutHist(TString opt = "", TVirtualPad* pad = 0);

   void SetParameters(ERsnOutType type, Double_t *parameters);

   void Fill(Int_t bin);

   Double_t GetValue();
   Double_t GetError();

   TH1 *CreateHistogram(Int_t nBins, Double_t min, Double_t max);

   TH1 *GetHist() { return fOutHist;}
   void DrawDefaultValueLine(Double_t val) {fDrawLineDefaultValue = kTRUE; fDefaultValue = val;}

   TStyle *CreateStyle(const char*name, const char *title = "");

   void SetDrawOptions(TString opt) { fDrawOpts = opt;}

private:

   Int_t                fType;
   AliRsnOutDrawObject  *fDrawObj;
   TH1                  *fOutHist;
   Double_t *fParamters;

   Double_t fDefaultValue;
   Bool_t fDrawLineDefaultValue;
   TString fDrawOpts;

   TStyle *fStyle;

   ClassDef(AliRsnOutParamHist, 1) // AliRsnOutParamHist
};

#endif // ALIRSNOUTPARAMHIST_H
