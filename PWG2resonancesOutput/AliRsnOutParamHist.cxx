#include "AliRsnOutParamHist.h"
#include <TH1.h>
#include <TString.h>
#include "AliRsnOutDrawObject.h"
#include "AliRsnOutManager.h"
#include <TRandom.h>
#include <TF1.h>
#include <TMath.h>
#include <TVirtualPad.h>
#include <TROOT.h>
#include <TLine.h>

ClassImp(AliRsnOutParamHist)

AliRsnOutParamHist::AliRsnOutParamHist(AliRsnOutDrawObject* drawObj, const char*name, const char* title) : TNamed(name, title),
   fType(0),
   fDrawObj(drawObj),
   fParamters(0),
   fDefaultValue(0.0),
   fDrawLineDefaultValue(kFALSE),
   fDrawOpts(),
   fStyle(0)
{
}

void AliRsnOutParamHist::SetParameters(AliRsnOutParamHist::ERsnOutType type, Double_t* parameters)
{
   fType = (Int_t) type;
   fParamters = parameters;
}

TH1* AliRsnOutParamHist::CreateHistogram(Int_t nBins, Double_t min, Double_t max)
{
   fOutHist = new TH1D(GetName(), GetTitle(), nBins, min, max);
   return fOutHist;
}

void AliRsnOutParamHist::Print(Option_t* option) const
{
   TNamed::Print(option);
   Printf("type=%d drawObj=%s", fType, ((AliRsnOutManager*)fDrawObj->GetRsnFunction())->GetName());
}

void AliRsnOutParamHist::DrawOutHist(TString opt, TVirtualPad* pad)
{
   if (fOutHist) {
      if (pad) pad->cd();

      TStyle *lastStyle = 0;
      if (fStyle) {
         lastStyle = gROOT->GetStyle(gStyle->GetName());
         fStyle->cd();
      }
      fOutHist->UseCurrentStyle();
      fOutHist->Draw(Form("%s %s", fDrawOpts.Data(), opt.Data()));

      if (fDrawLineDefaultValue) {
         TLine *l = new TLine(fOutHist->GetXaxis()->GetXmin(), fDefaultValue, fOutHist->GetXaxis()->GetXmax(), fDefaultValue);
         l->SetLineColor(fStyle->GetFuncColor());
         l->SetLineStyle(fStyle->GetFuncStyle());
         l->Draw();

      }


      if (pad)  pad->Update();
//         delete h;
      if (lastStyle) {
         gROOT->SetStyle(lastStyle->GetName());
         gROOT->ForceStyle();
      }
   }
}


void AliRsnOutParamHist::Fill(Int_t bin)
{
   if (fOutHist) {
      fOutHist->SetBinContent(bin, GetValue());
      fOutHist->SetBinError(bin, GetError());
   }
}

Double_t AliRsnOutParamHist::GetValue()
{
   TH1*h = 0;
   TF1 *f = 0;
   switch (fType) {
      case kHistIntegral:
         h = fDrawObj->GetHist();
         if (h) return h->Integral(h->GetXaxis()->FindBin(fParamters[0]) , h->GetXaxis()->FindBin(fParamters[1]));
         break;
      case kFitFunParam:
         f = fDrawObj->GetFitFun();
         if (f) return f->GetParameter((Int_t)fParamters[0]);
         break;
   }

   return 0.0;
}

Double_t AliRsnOutParamHist::GetError()
{
   TH1*h = 0;
   TF1 *f = 0;
   switch (fType) {
      case kHistIntegral:
         h = fDrawObj->GetHist();
         if (h) {
            Double_t error = 0;
            for (Int_t iBin = h->GetXaxis()->FindBin(fParamters[0]); iBin <= h->GetXaxis()->FindBin(fParamters[1]); iBin++) {
//               Printf("%d %f %f %f",iBin,h->GetBinContent(iBin),h->GetBinError(iBin),TMath::Power(h->GetBinError(iBin),2));
               error += TMath::Power(h->GetBinError(iBin), 2);
            }

//          Printf("Final error %f",TMath::Sqrt(error));
            return TMath::Sqrt(error);
         }
         break;
      case kFitFunParam:
         f = fDrawObj->GetFitFun();
         if (f) return f->GetParError((Int_t)fParamters[0]);
         break;
   }

   return 0.0;
}

TStyle* AliRsnOutParamHist::CreateStyle(const char* name, const char* title)
{
   fStyle = new TStyle(name, title);
   return fStyle;
}
