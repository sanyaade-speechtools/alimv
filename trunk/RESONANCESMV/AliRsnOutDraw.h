#ifndef ALIRSNOUTDRAW_H
#define ALIRSNOUTDRAW_H

#include <TObject.h>
#include <TObjArray.h>
#include <TH1D.h>
#include <TString.h>

class TVirtualPad;
class AliRsnOutDraw : public TObject {

public:
   AliRsnOutDraw();

   virtual void ClearHistograms(Option_t*  = "");

   void AddHistoram(TH1* hist);
   void DrawFinalPicture(TVirtualPad* pad, TString opts, Double_t max, Double_t cutMin, Double_t cutMax);

   TH1* CreateHistogramFromFormFormula(TString formula, TString error, TString norm);

   void AddToHistogramStack(TString formula = "$1", TString error = "sqrt($1)", TString norm = "", TString fit = "");

   void ScaleHistogram(TH1* h1, TH1* h2, Double_t* minmax, Double_t normCorrection = 1);

   void SetNormMinMax(Double_t min, Double_t max) { fNormMinMax[0] = min; fNormMinMax[1] = max;}
   void SetFitMinMax(Double_t min, Double_t max) { fFitMinMax[0] = min; fFitMinMax[1] = max;}

   void AddFitParamHistogram(const char *name, const char *title, Int_t parId, Double_t min, Double_t max, Double_t step);


private:
   TObjArray fHistograms;
   TObjArray fHistogramsStack;
   TObjArray fHistogramsStackError;
   TObjArray fHistogramsStackFit;
   TObjArray fHistogramsNormalize;

   TObjArray fFitParamHistogram;
   Int_t     fNumPars;
   TArrayI  fParIds;

   Double_t  fNormMinMax[2];
   Double_t  fFitMinMax[2];

   ClassDef(AliRsnOutDraw, 1) // AliRsnOutDraw

};

#endif // ALIRSNOUTDRAW_H

