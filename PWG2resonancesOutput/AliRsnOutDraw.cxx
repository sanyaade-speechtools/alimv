#include <TObjString.h>
#include <TError.h>
#include <TVirtualPad.h>
#include <TF1.h>
#include <TMath.h>

#include "AliRsnOutDraw.h"
#include <TH1.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TFile.h>
#include <TLine.h>



ClassImp(AliRsnOutDraw)

AliRsnOutDraw::AliRsnOutDraw(): TObject(),
   fHistograms(),
   fHistogramsStack(),
   fHistogramsStackError(),
   fHistogramsNormalize(),
   fFitParamHistogram(),
   fNumPars(0),
   fParIds()
{
   for (Int_t i = 0; i < 2; i++) {
      fNormMinMax[i] = 0;
      fFitMinMax[i] = 0;
   }
}


void AliRsnOutDraw::ClearHistograms(Option_t*)
{
   fHistograms.Delete();
//   fHistogramsStack.Delete();
}

void AliRsnOutDraw::AddHistoram(TH1* hist)
{
   if (hist) {
      fHistograms.Add(hist);
   }
}


void AliRsnOutDraw::AddToHistogramStack(TString formula, TString error, TString norm, TString fit)
{
   if (!formula.IsNull()) {
      fHistogramsStack.Add(new TObjString(formula));
      fHistogramsStackError.Add(new TObjString(error));
      fHistogramsNormalize.Add(new TObjString(norm));
      fHistogramsStackFit.Add(new TObjString(fit));
   }
}

void AliRsnOutDraw::AddFitParamHistogram(const char* name, const char* title, Int_t parId, Double_t min, Double_t max, Double_t step)
{
   Int_t nBins = (Int_t)((max - min) / step);
   Printf("%.2f %.2f %.2f %d", min, max, step, nBins);
   TH1D *h = new TH1D(name, title, nBins, min, max);
//    h->Sumw2();
   fFitParamHistogram.Add(h);
   fParIds.Set(fNumPars + 1);
   fParIds.AddAt(parId, fNumPars++);
//    Printf("%d %d",fNumPars-1,fParIds.At(fNumPars-1));
}



void AliRsnOutDraw::DrawFinalPicture(TVirtualPad* pad, TString opts, Double_t max, Double_t cutMin, Double_t cutMax)
{

   opts.ReplaceAll("SAME", "");
   TString formulaStr, errorStr, normStr, fitStr;
   TF1 *fitFun = 0;
   TH1*h = 0;
   for (Int_t i = 0; i < fHistogramsStack.GetEntries(); i++) {


//       Printf("Stack id=%d", i);

      formulaStr = ((TObjString*) fHistogramsStack.At(i))->GetString();
      errorStr = ((TObjString*) fHistogramsStackError.At(i))->GetString();
      normStr = ((TObjString*) fHistogramsNormalize.At(i))->GetString();
      fitStr = ((TObjString*) fHistogramsStackFit.At(i))->GetString();
      h = CreateHistogramFromFormFormula(formulaStr, errorStr, normStr);
      if (!i) h->GetYaxis()->SetRangeUser(-max * 0.1, max);
      pad->cd();
      h->SetLineColor(i + 1);
      h->DrawCopy(opts.Data());
      if (!opts.Contains("SAME")) opts += " SAME";

      TLine *l = new TLine(h->GetXaxis()->GetXmin(), 0.0, h->GetXaxis()->GetXmax(), 0.0);
      l->SetLineColor(kBlue);
      l->SetLineStyle(kDashed);
      l->Draw();

      pad->Update();
//          gSystem->Sleep(500);

      if (!fitStr.IsNull()) {
         h->Fit(fitStr.Data(), "", opts.Data(), fFitMinMax[0], fFitMinMax[1]);

         fitFun = (TF1*)gROOT->GetListOfFunctions()->FindObject("gaus");
         for (Int_t iFitPar = 0; iFitPar < fFitParamHistogram.GetEntries(); iFitPar++) {
//                Printf("par[%d] is %f [%d] average=%.2f [%.2f-%.2f]",iFitPar, fitFun->GetParameter(fParIds.At(iFitPar)),fParIds.At(iFitPar),(cutMin+cutMax)/2,cutMin,cutMax);
            TH1D *histPar = (TH1D *)fFitParamHistogram.At(iFitPar);
//                   Printf("%d %d %f",histPar->GetNbinsX(), histPar->FindBin((cutMin + cutMax) / 2)),histPar->GetBinContent((histPar->FindBin((cutMin + cutMax) / 2)));
            histPar->SetBinContent(histPar->FindBin((cutMin + cutMax) / 2), fitFun->GetParameter(fParIds.At(iFitPar)));
            histPar->SetBinError(histPar->FindBin((cutMin + cutMax) / 2), fitFun->GetParError(fParIds.At(iFitPar)));
         }


         pad->Update();
      }
   }
}

TH1* AliRsnOutDraw::CreateHistogramFromFormFormula(TString formula, TString error, TString norm)
{
   TH1 *hOut = (TH1*)fHistograms.At(0)->Clone();
   hOut->Reset();
//    hOut->Print();
   TH1*h;
   TString tmpFormula, tmpError;
   for (Int_t iBin = 0; iBin < hOut->GetNbinsX() + 2; iBin++) {
      tmpFormula = formula;
      tmpError = error;
      for (Int_t i = 0; i < fHistograms.GetEntries(); i++) {
         h = (TH1*)fHistograms.At(i);
//       ::Info("",Form("i=%d iBin=%d content=%f",i,iBin,h->GetBinContent(iBin)));
         tmpFormula.ReplaceAll(Form("$%d", i + 1), Form("%f", h->GetBinContent(iBin)));
         tmpError.ReplaceAll(Form("$%d", i + 1), Form("%f", h->GetBinContent(iBin)));
      }

      TF1 f("tmpFun", tmpFormula.Data());
      TF1 fe("tmpFunError", tmpError.Data());
      Double_t val = f.Eval(0.0);
      Double_t valError = fe.Eval(0.0);
//     ::Info("",Form("string is %s : %s ",formula.Data(),tmpFormula.Data()));
//     ::Info("",Form("Val is %f ",val));

//     ::Info("",Form("string Error is %s : %s ",error.Data(),tmpError.Data()));
//     ::Info("",Form("ValError is %f ",valError));
      hOut->SetBinContent(iBin, val);
      hOut->SetBinError(iBin, valError);

   }

   // normalize
   if (!norm.IsNull()) {
//         norm(0,2);
      norm.ReplaceAll("$", "");
      Int_t idNorm = norm.Atoi();
//       Printf("%d", idNorm);
      TH1 *htmp = (TH1*)fHistograms.At(idNorm - 1);
//       Double_t minmax[] = {1.05, 1.09};
      ScaleHistogram(htmp, hOut, fNormMinMax, 1.0);
//         ScaleHistogram(hOut,htmp,minmax,1.0);
      TH1 *hClone = (TH1*)hOut->Clone();
      hClone->SetName(Form("%s_norm", hClone->GetName()));
      AddHistoram(hClone);
//       fHistograms.Print();
   }

//   hOut->Sumw2();

   return hOut;
}

void AliRsnOutDraw::ScaleHistogram(TH1*h1, TH1*h2, Double_t *minmax, Double_t normCorrection)
{
   if (!h1) return;
   if (!h2) return;


//    Printf("Norm correction %f", normCorrection);

   if (minmax[0] != minmax[1]) {
//       Printf("Scale <%f,%f>", minmax[0], minmax[1]);
      Int_t fNormalizeBinMinI = h1->FindBin(minmax[0]);
      Int_t fNormalizeBinMaxI = h1->FindBin(minmax[1]);
//       Printf("Scale BIN <%d,%d>", fNormalizeBinMinI, fNormalizeBinMaxI);
      Double_t normtmp = h2->Integral(fNormalizeBinMinI, fNormalizeBinMaxI);
//       Printf("h1(%f) h2(%f) normtmp %f", (Double_t)h1->Integral(fNormalizeBinMinI, fNormalizeBinMaxI), (Double_t)h2->Integral(fNormalizeBinMinI, fNormalizeBinMaxI), normtmp);
      Double_t normFactor = (Double_t) h1->Integral(fNormalizeBinMinI, fNormalizeBinMaxI) / normtmp * normCorrection;
//       Printf("normFactor %f", normFactor);

      h2->Scale(normFactor);
   } else {
      //         Info("",Form("Scale BIN <%d,%d>",fNormalizeBinMinI,fNormalizeBinMaxI));
      Double_t normtmp = h2->Integral();
//       Printf("h1(%f) h2(%f) normtmp %f", (Double_t)h1->Integral(), (Double_t)h2->Integral(), normtmp);
      Double_t normFactor = (Double_t) h1->Integral() / normtmp * normCorrection;
//       Printf("normFactor %f", normFactor);
      h2->Scale(normFactor);
   }


}
