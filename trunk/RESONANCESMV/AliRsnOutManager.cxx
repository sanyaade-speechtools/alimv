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
      extraHistTitle = Form("%d [%.1f-%.1f]", cutDim, cutMin, cutMax);
   }

   TObjArrayIter next(&fOutputs);
   AliRsnOutReader *r;


   // check for maximum
//   Double_t histMax = 0.0,histMaxTmp=0.0;

//   next.Reset();
//   while (( r = (AliRsnOutReader*)next())) {
//     histMaxTmp = r->GetMaximumValue(dim);
//     if (histMax<histMaxTmp) histMax = histMaxTmp;
//   }


   TH1*h = 0;
   TString extraOpts = "";
//   Int_t num=1;
   next.Reset();
   fDrawer.ClearHistograms();
   Double_t max = 0.0;
   while ((r = (AliRsnOutReader*)next())) {
      h = r->Get1DHistogram(dim, cutDim, cutMin, cutMax);
      if (h) {
         h->SetName(Form("%s_%s_%s", r->GetName(), h->GetName(), extraHistName.Data()));
         if (extraOpts.IsNull()) h->SetTitle(Form("%s %s", GetTitle(), extraHistTitle.Data()));
         fDrawer.AddHistoram(h);
         ::Info("", Form("Max Hists current %f  in  hist %f", max, h->GetMaximum()));
         if (max < h->GetMaximum()) max = 1.3 * h->GetMaximum();
         /*


               h->SetLineColor(num++);
               h->SetStats(kFALSE);

         //       h->GetYaxis()->SetRangeUser(0.0,histMax);
               h->DrawCopy(extraOpts.Data());
               extraOpts="SAME";*/
      }
   }
   ::Info("", Form("Max Hists current %f", max));
   fDrawer.DrawFinalPicture(pad, opts, max);

}

void AliRsnOutManager::FinalOutput(Double_t min, Double_t max, Double_t step, Int_t dim, Int_t cutDim, TCanvas* canvas)
{
   if (!canvas) canvas = new TCanvas();

   Int_t i = 1;
   Double_t numHists = (max - min) / step;
   Int_t divX = numHists / TMath::Sqrt(numHists);
   Int_t divY = numHists / divX;
   if ((Int_t)numHists % divX != 0) divY++;
   canvas->Divide(divX, divY);

   for (Double_t iVal = min; iVal <= max - step; iVal += step) {
      PrepareOutput(dim, cutDim, iVal, iVal + step, canvas->cd(i++));
   }

   canvas->cd();
}

