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
   void DrawFinalPicture(TVirtualPad* pad, TString opts, Double_t max = 0.0);

   TH1* CreateHistogramFromFormFormula(TString formula, TString error, TString norm);

   void AddToHistogramStack(TString formula = "$1", TString error = "sqrt($1)", TString norm = "");

   void ScaleHistogram(TH1* h1, TH1* h2, Double_t* minmax, Double_t normCorrection = 1);

private:
   TObjArray fHistograms;
   TObjArray fHistogramsStack;
   TObjArray fHistogramsStackError;
   TObjArray fHistogramsNormalize;

   Double_t  fMaximum;

   ClassDef(AliRsnOutDraw, 1) // AliRsnOutDraw

};

#endif // ALIRSNOUTDRAW_H
