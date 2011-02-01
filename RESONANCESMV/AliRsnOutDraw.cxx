#include <TObjString.h>
#include <TError.h>

#include "AliRsnOutDraw.h"
#include <TVirtualPad.h>
#include <TF1.h>
#include <TMath.h>


ClassImp(AliRsnOutDraw)

AliRsnOutDraw::AliRsnOutDraw():TObject(),
        fHistograms(),
        fHistogramsStack(),
        fHistogramsStackError(),
        fMaximum(0.0)
{

}


void AliRsnOutDraw::ClearHistograms(Option_t* )
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


void AliRsnOutDraw::AddToHistogramStack(TString formula, TString error, TString norm)
{
    if (!formula.IsNull()) {
        fHistogramsStack.Add(new TObjString(formula));
        if (!error.IsNull())
            fHistogramsStackError.Add(new TObjString(error));
        fHistogramsNormalize.Add(new TObjString(norm));
    }
}


void AliRsnOutDraw::DrawFinalPicture(TVirtualPad* pad, TString opts, Double_t max)
{

    opts.ReplaceAll("SAME","");
    TObjString *formulaStr,*errorStr,*normStr;
    TH1*h=0;
    for (Int_t i=0;i<fHistogramsStack.GetEntries();i++) {
      
      ::Info("",Form("Stack id=%d",i));
      
        formulaStr = (TObjString*) fHistogramsStack.At(i);
        errorStr = (TObjString*) fHistogramsStackError.At(i);
        normStr = (TObjString*) fHistogramsNormalize.At(i);
        h = CreateHistogramFromFormFormula(formulaStr->GetString(),errorStr->GetString(),normStr->GetString());
        if (!i) h->GetYaxis()->SetRangeUser(0.0,max);
        pad->cd();
        h->SetLineColor(i+1);
        h->DrawCopy(opts.Data());
        opts += " SAME";
    }
}

TH1* AliRsnOutDraw::CreateHistogramFromFormFormula(TString formula, TString error,TString norm)
{
    TH1 *hOut = (TH1*)fHistograms.At(0)->Clone();
    hOut->Reset();
    hOut->Print();
    TH1*h;
    TString tmpFormula,tmpError;
    for (Int_t iBin=0;iBin<hOut->GetNbinsX()+2;iBin++) {
        tmpFormula = formula;
        tmpError = error;
        for (Int_t i=0;i<fHistograms.GetEntries();i++) {
            h = (TH1*)fHistograms.At(i);
//       ::Info("",Form("i=%d iBin=%d content=%f",i,iBin,h->GetBinContent(iBin)));
            tmpFormula.ReplaceAll(Form("$%d",i+1),Form("%f",h->GetBinContent(iBin)));
            tmpError.ReplaceAll(Form("$%d",i+1),Form("%f",h->GetBinContent(iBin)));
        }



        TF1 f("tmpFun",tmpFormula.Data());
        TF1 fe("tmpFunError",tmpError.Data());
        Double_t val = f.Eval(0.0);
        Double_t valError = fe.Eval(0.0);
//     ::Info("",Form("string is %s : %s ",formula.Data(),tmpFormula.Data()));
//     ::Info("",Form("Val is %f ",val));

//     ::Info("",Form("string Error is %s : %s ",error.Data(),tmpError.Data()));
//     ::Info("",Form("ValError is %f ",valError));
        hOut->SetBinContent(iBin,val);
        hOut->SetBinError(iBin,valError);

    }

    // normalize
    if (!norm.IsNull()) {
//         norm(0,2);
        norm.ReplaceAll("$","");
        Int_t idNorm = norm.Atoi();
        ::Info("",Form("%d",idNorm));
        TH1 *htmp = (TH1*)fHistograms.At(idNorm-1);
        Double_t minmax[] = {1.05,1.09};
        ScaleHistogram(htmp,hOut,minmax,1.0);
//         ScaleHistogram(hOut,htmp,minmax,1.0);
        TH1 *hClone = (TH1*)hOut->Clone();
        hClone->SetName(Form("%s_norm",hClone->GetName()));
        AddHistoram(hClone);
        fHistograms.Print();
    }

//   hOut->Sumw2();

    return hOut;
}

void AliRsnOutDraw::ScaleHistogram(TH1*h1,TH1*h2,Double_t *minmax,Double_t normCorrection) {
    if (!h1) return;
    if (!h2) return;


    Info("", Form("Norm correction %f",normCorrection));

/*    if (!minmax) {
        //         Info("",Form("Scale BIN <%d,%d>",fNormalizeBinMinI,fNormalizeBinMaxI));
        Double_t normtmp = h2->Integral();
        Info("", Form("h1(%f) h2(%f) normtmp %f", (Double_t)h1->Integral(),(Double_t)h2->Integral(),normtmp));
        Double_t normFactor = (Double_t) h1->Integral() / normtmp;
        Info("", Form("normFactor %f", normFactor));
        h2->Scale(normFactor);
    } else  */
    if (minmax[0] != minmax[1]) {
        Info("",Form("Scale <%f,%f>",minmax[0],minmax[1]));
        Int_t fNormalizeBinMinI = h1->FindBin(minmax[0]);
        Int_t fNormalizeBinMaxI = h1->FindBin(minmax[1]);
        Info("",Form("Scale BIN <%d,%d>",fNormalizeBinMinI,fNormalizeBinMaxI));
        Double_t normtmp = h2->Integral(fNormalizeBinMinI, fNormalizeBinMaxI);
        Info("", Form("h1(%f) h2(%f) normtmp %f", (Double_t)h1->Integral(fNormalizeBinMinI, fNormalizeBinMaxI),(Double_t)h2->Integral(fNormalizeBinMinI, fNormalizeBinMaxI),normtmp));
        Double_t normFactor = (Double_t) h1->Integral(fNormalizeBinMinI, fNormalizeBinMaxI) / normtmp*normCorrection;
        Info("", Form("normFactor %f", normFactor));

        h2->Scale(normFactor);
    } else {
        //         Info("",Form("Scale BIN <%d,%d>",fNormalizeBinMinI,fNormalizeBinMaxI));
        Double_t normtmp = h2->Integral();
        Info("", Form("h1(%f) h2(%f) normtmp %f", (Double_t)h1->Integral(),(Double_t)h2->Integral(),normtmp));
        Double_t normFactor = (Double_t) h1->Integral() / normtmp*normCorrection;
        Info("", Form("normFactor %f", normFactor));
        h2->Scale(normFactor);
    }


}
