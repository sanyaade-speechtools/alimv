#include <TString.h>

#include "AliRsnOutFunction.h"
#include "AliRsnOutInput.h"
#include "AliRsnOutManager.h"
#include <TSystem.h>
#include <TF1.h>

ClassImp(AliRsnOutFunction)

AliRsnOutFunction::AliRsnOutFunction() : TNamed()
{
}

AliRsnOutFunction::AliRsnOutFunction(const char* name, const char* title): TNamed(name, title)
{
}

void AliRsnOutFunction::Print(Option_t* option) const
{
   TNamed::Print();
   if (!option)
      Printf("Option : %s", option);
}

TH1* AliRsnOutFunction::CreateHistogram(AliRsnOutManager* mgr, Int_t valDim, Int_t cutDim, Double_t min, Double_t max)
{
   AliRsnOutInput *input = (AliRsnOutInput *) mgr->GetInputs()->First();
   TH1 *hOut = (TH1*) input->Get1DHistogram(mgr->GetValDim(), mgr->GetCutDim(), mgr->GetMin(), mgr->GetMax());
   hOut->Reset();
   hOut->SetName(Form("%s_%d_[%.2f,%.2f]", hOut->GetName(), cutDim, min, max));
   hOut->SetTitle(Form("%s %d [%.2f,%.2f]", hOut->GetTitle(), cutDim, min, max));
//    return hOut;

   TString name = GetName();
   TString error = GetTitle();

   Double_t fHistMinMax[] = {1e99, -1e99};
//    fHistMinMax[0] = 1e99;
//    fHistMinMax[1] = -1e99;

   TObjArray tmpHistArray;
   TH1 *h;
   TObjArrayIter next(mgr->GetInputs());
   Int_t iHist = 0;
   while ((input = (AliRsnOutInput*)next())) {
      h = input->Get1DHistogram(valDim, cutDim, min, max);
      h->SetName(Form("%s_%d", h->GetName(), iHist++));
      if (fHistMinMax[0] > h->GetMinimum()) fHistMinMax[0] = h->GetMinimum();
      if (fHistMinMax[1] < h->GetMaximum()) fHistMinMax[1] = h->GetMaximum();
      tmpHistArray.Add(h);
   }

   for (Int_t iBin = 0; iBin < hOut->GetNbinsX() + 2; iBin++) {
      name = GetName();
      error = GetTitle();
      next.Reset();
      iHist = 0;
      while ((input = (AliRsnOutInput*)next())) {
         h = (TH1*) tmpHistArray.At(iHist++);
         name.ReplaceAll(input->GetName(), Form("%f", h->GetBinContent(iBin)));
         error.ReplaceAll(input->GetName(), Form("%f", h->GetBinContent(iBin)));
      }

      TF1 f("tmpFun", name.Data());
      TF1 fe("tmpFunError", error.Data());
      Double_t val = f.Eval(0.0);
      Double_t valError = fe.Eval(0.0);
      hOut->SetBinContent(iBin, val);
      hOut->SetBinError(iBin, valError);
   }

   tmpHistArray.Delete();

   return hOut;
}

