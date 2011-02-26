#include "AliRsnOutPad.h"
#include "AliRsnOutDrawObject.h"
#include <TH1.h>
#include <TVirtualPad.h>
#include <TLine.h>
#include <TLegend.h>

ClassImp(AliRsnOutPad)

AliRsnOutPad::AliRsnOutPad(const char* name, const char* title): TNamed(name, title),
   fDrawZeroLine(kFALSE),
   fLegend(0)
{

}

void AliRsnOutPad::Print(Option_t* option) const
{
   TNamed::Print(option);
   AliRsnOutDrawObject *obj;
   TObjArrayIter next(&fList);
   while ((obj = (AliRsnOutDrawObject*)next())) {
      obj->Print();
   }
}

void AliRsnOutPad::DrawPad(AliRsnOutManager *rsnOutMgr, Int_t valDim, Int_t cutDim, Double_t min, Double_t max, TVirtualPad* pad, TString opt)
{
   Double_t fHistMinMax[] = {1e99, -1e99};
   TH1*h;
//    TF1*f;
   AliRsnOutDrawObject *obj;
   TObjArrayIter next(&fList);
   while ((obj = (AliRsnOutDrawObject*)next())) {
      h = obj->CreateHistogram(rsnOutMgr, valDim, cutDim, min, max);
      if (fHistMinMax[0] > h->GetMinimum()) fHistMinMax[0] = h->GetMinimum();
      if (fHistMinMax[1] < h->GetMaximum()) fHistMinMax[1] = h->GetMaximum();
      h->SetTitle(Form("%s [%.2f,%.2f]", GetTitle(), min, max));;
   }

   Double_t diff = fHistMinMax[1] - fHistMinMax[0];
   Double_t corr[] = {0.2, 1.2};
   next.Reset();
   TLegend *legend = 0;
   if (fLegend) legend = (TLegend *)fLegend->Clone();
   while ((obj = (AliRsnOutDrawObject*)next())) {
      obj->DrawObject(rsnOutMgr, -diff * corr[0], diff * corr[1], pad, opt.Data());
      h = obj->GetHist();
      if (legend && h) legend->AddEntry(h, obj->GetName(), "l");
//       f = obj->GetFitFun();
//       if (f) legend->AddEntry(f,Form("fit %s",obj->GetName()),"l");
      opt += " SAME";
   }

   if (legend) legend->Draw();

   if (fDrawZeroLine) {
      TLine *l = new TLine(h->GetXaxis()->GetXmin(), 0.0, h->GetXaxis()->GetXmax(), 0.0);
      l->SetLineColor(kBlack);
      l->SetLineStyle(kDashed);
      l->Draw();

   }

   if (pad)pad->Update();

}

void AliRsnOutPad::Add(AliRsnOutDrawObject* obj)
{
   fList.Add(obj);
}
