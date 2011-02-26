#include <TString.h>

#include "AliRsnOutDrawObject.h"
#include "AliRsnOutFunction.h"
#include <TVirtualPad.h>
#include <TROOT.h>
#include <TF1.h>

ClassImp(AliRsnOutDrawObject)

AliRsnOutDrawObject::AliRsnOutDrawObject(const char* name, AliRsnOutFunction* rsnFun) : TNamed(name, name),
   fRsnFunction(rsnFun),
   fHist(),
   fFunc(0),
   fFitFunc(0),
   fOptions(),
   fDrawOptions(),
   fStyle(0),
   fStyleFit(0)
{
}

void AliRsnOutDrawObject::Reset()
{
   delete fHist;
//    delete fFunc;
//    delete fFitFunc;
}


void AliRsnOutDrawObject::Print(Option_t* option) const
{
//    TObject::Print();

   if (fRsnFunction) Printf("RsnFunction : %s", fRsnFunction->GetName());

   if (!option)
      Printf("Option : %s", option);
}

TH1 *AliRsnOutDrawObject::CreateHistogram(AliRsnOutManager* rsnOutMgr, Int_t valDim, Int_t cutDim, Double_t min, Double_t max)
{
   if (fRsnFunction && rsnOutMgr) {
      fHist = fRsnFunction->CreateHistogram(rsnOutMgr, valDim, cutDim, min, max);
   }

   return fHist;
}


void AliRsnOutDrawObject::DrawObject(AliRsnOutManager* rsnOutMgr, Double_t min, Double_t max, TVirtualPad* pad, TString opt)
{
   if (fHist && fRsnFunction && rsnOutMgr) {
      if (pad) pad->cd();
      // add style
      TStyle *lastStyle = 0;
      if (fStyle) {
         lastStyle = gROOT->GetStyle(gStyle->GetName());
         fStyle->cd();
      }
      fHist->UseCurrentStyle();
      fHist->GetYaxis()->SetRangeUser(min, max);
      fHist->Draw(Form("%s %s", fDrawOptions.Data(), opt.Data()));

      if (fFitFunc) {
         fFitFunc->UseCurrentStyle();
         fHist->Fit(fFitFunc, "", "SAME", fFitFunc->GetXmin(), fFitFunc->GetXmax());
      }
      pad->Update();
//         delete h;
      if (lastStyle) {
         gROOT->SetStyle(lastStyle->GetName());
         gROOT->ForceStyle();
      }

   }


}


TStyle *AliRsnOutDrawObject::CreateStyle(const char* name, const char* title)
{
   fStyle = new TStyle(name, title);
   return fStyle;
}
