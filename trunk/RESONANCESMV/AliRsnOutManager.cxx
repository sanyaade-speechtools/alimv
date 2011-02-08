#include <TError.h>
#include <TVirtualPad.h>
#include <TCanvas.h>
#include <TMath.h>

#include "AliRsnOutManager.h"
#include "AliRsnOutReader.h"


ClassImp(AliRsnOutManager)

AliRsnOutManager::AliRsnOutManager(const char* name, const char* title): TNamed(name, title),
   fOutputs(),
   fSheme("")
{

}

void AliRsnOutManager::Print(Option_t* option) const
{
   TNamed::Print(option);

   TObjArrayIter next(&fOutputs);
   AliRsnOutReader *r;
   while ((r = (AliRsnOutReader*)next())) {
      r->Print();
   }
}


void AliRsnOutManager::AddOutput(AliRsnOutReader* reader)
{
   fOutputs.Add(reader);
}

Bool_t AliRsnOutManager::SetScheme(TString scheme)
{
   // scheme should be validated
   fSheme = scheme;
   return kTRUE;
}

void AliRsnOutManager::PrepareOutput(Int_t dim, Int_t cutDim, Double_t cutMin, Double_t cutMax, TVirtualPad* pad, TString opts)
{

   if (pad) pad->cd();

   TString extraHistName;
   TString extraHistTitle;
   if (cutDim >= 0) {
      extraHistName = Form("%d_[%.1f-%.1f]", cutDim, cutMin, cutMax);
      extraHistTitle = Form("[%.1f-%.1f]", cutDim, cutMin, cutMax);
   }

   TObjArrayIter next(&fOutputs);
   AliRsnOutReader *r;

   TH1*h = 0;
   TString extraOpts = "";
   next.Reset();
   fDrawer.ClearHistograms();
   Double_t max = 0.0;
   while ((r = (AliRsnOutReader*)next())) {
      h = r->Get1DHistogram(dim, cutDim, cutMin, cutMax);
      if (h) {
         h->SetName(Form("%s_%s_%s", r->GetName(), h->GetName(), extraHistName.Data()));
         if (extraOpts.IsNull()) h->SetTitle(Form("%s %s", GetTitle(), extraHistTitle.Data()));
         fDrawer.AddHistoram(h);
//          Printf("Max Hists current %f  in  hist %f", max, h->GetMaximum());
         if (max < h->GetMaximum()) max = 1.3 * h->GetMaximum();
      }
   }
//    Printf("Max Hists current %f", max);
   fDrawer.DrawFinalPicture(pad, opts, max, cutMin, cutMax);

}

void AliRsnOutManager::FinalOutput(Double_t min, Double_t max, Double_t step, Int_t dim, Int_t cutDim, TCanvas* canvas)
{
   if (!canvas) canvas = new TCanvas();

   Int_t i = 1;
   Double_t numHists = (max - min) / step;
   Int_t divY = numHists / TMath::Sqrt(numHists);
   Int_t divX = numHists / divY;

   Printf("***************========================== [%d,%d] %.2f", divX, divY, numHists);

   canvas->Divide(divX, divY);

   for (Double_t iVal = min; iVal <= max - step ; iVal += step) {
      Printf("Dim=%d CutDim=%d [%.2f,%.2f]", dim, cutDim, iVal, iVal + step);
      PrepareOutput(dim, cutDim, iVal, iVal + step, canvas->cd(i++));
   }

   canvas->cd();
}

