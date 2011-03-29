#include <TError.h>
#include <TVirtualPad.h>
#include <TCanvas.h>
#include <TMath.h>

#include "AliRsnOutManager.h"
#include <TString.h>
#include "AliRsnOutPad.h"
#include <TSystem.h>


ClassImp(AliRsnOutManager)

AliRsnOutManager::AliRsnOutManager(const char* name, const char* title): TNamed(name, title),
   fInputs(),
   fMin(0.0),
   fMax(10.0),
   fStep(10.0),
   fValDimension(0),
   fCutDimension(1),
   fCount(0)
{

}

void AliRsnOutManager::Print(Option_t* option) const
{
   TNamed::Print(option);

   Printf("min=%.2f max=%.2f step=%.2f valDim=%d cutDim=%d", fMin, fMax, fStep, fValDimension, fCutDimension);

   TObjArrayIter next(&fInputs);
   AliRsnOutInput *input;
   while ((input = (AliRsnOutInput*)next())) {
      input->Print();
   }

}

void AliRsnOutManager::SetOutputSettings(Double_t min, Double_t max, Double_t step, Int_t dim, Int_t cutDim)
{
   fMin = min;
   fMax = max;
   fStep = step;
   fValDimension = dim;
   fCutDimension = cutDim;
}

void AliRsnOutManager::Draw(Option_t* /*option*/)
{
   //TODO reuse mainCanvas
   fCount++;
   TCanvas *mainCanvas = new TCanvas(Form("%s_%d", GetName(), fCount), Form("%s (%d)", GetTitle(), fCount));
   Double_t numHists = (fMax - fMin) / fStep;
   Int_t numHistInt = TMath::Sqrt(numHists);

   Int_t divY = numHists / TMath::Sqrt(numHists);
   Int_t divX = numHists / divY;

//    if (TMath::Sqrt(numHists) - numHistInt > 0) divY++;

   Printf("Canvas will have %.2f histogram and is divided to [%d,%d] ", numHists, divX, divY);

   mainCanvas->Divide(divX, divY);

   AliRsnOutParamHist *obj;
   TObjArrayIter next(&fParamOutputs);
   while ((obj = (AliRsnOutParamHist*)next())) {
      obj->CreateHistogram(numHists, fMin, fMax);
   }


   Int_t iCountPad = 0;
   for (Double_t iVal = fMin; iVal < fMax ; iVal += fStep) {
      Printf("Dim=%d CutDim=%d [%.2f,%.2f]", fValDimension, fCutDimension, iVal, iVal + fStep);
      if (fPad) {
         fPad->DrawPad(this, fValDimension, fCutDimension, iVal, iVal + fStep, mainCanvas->cd(++iCountPad));
      }
      next.Reset();
      while ((obj = (AliRsnOutParamHist*)next())) {
         obj->Fill(iCountPad);
      }
   }

   next.Reset();
   Int_t count = 0;
   while ((obj = (AliRsnOutParamHist*)next())) {
      TCanvas *c = new TCanvas(Form("c%d", count++), "c title");
      c->cd();
      obj->DrawOutHist("E1");
   }

   mainCanvas->cd();
}
