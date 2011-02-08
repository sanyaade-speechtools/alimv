
#ifndef __CINT__
#include "AliRsnOutReader.h"
#include "AliRsnOutManager.h"
#include <TCanvas.h>
#include <TMath.h>
#include <AliRsnOutDraw.h>
#include <TDirectory.h>
#include <TLine.h>
#endif
void testRsnOutReader()
{
   gROOT->ProcessLine(".L AliRsnOutDraw.cxx+g");
   gROOT->ProcessLine(".L AliRsnOutReader.cxx+g");
   gROOT->ProcessLine(".L AliRsnOutManager.cxx+g");

   const char* fileName = "/home/mvala/ALICE/tmp/RES_TO_TEST/new/AnalysisResults.root";
//     const char* fileName = "/home/mvala/ALICE/RESULTS/AliEn-2011-02-07/0001/AnalysisResults.root";

//    const char* fileName="/home/mvala/ALICE/tmp/RES_TO_TEST/AnalysisResults_8641.root";
//    const char* fileName="/home/mvala/WORK/ALICE/RESULTS/AliEn-2011-01-25/0001/AnalysisResults.root";

   const char*partName = "";
   const char* listName = Form("RsnHist%s", partName);
//     const char* listNameMix = Form("RsnHist%s_mix",partName);
   const char* listNameMix = Form("RsnHist%s", partName);
   const char* sparseName1 = "_pairPMHist_IM_PT_Y";
   const char* sparseName2 = "_pairPPHist_IM_PT_Y";
   const char* sparseName3 = "_pairMMHist_IM_PT_Y";

   Bool_t doAdditionalCut = kFALSE;
//     doAdditionalCut = kTRUE;
//     Double_t additionalCut[] = {2,-0.7,-0.4};
   Double_t additionalCut[] = {2, 0.4, 0.7};
//     Double_t additionalCut[] = {2,-0.2,0.2};
//     Double_t additionalCut[] = {2,-0.4,-0.2};
   AliRsnOutReader *myRsnOutKPKM1 = new AliRsnOutReader("KPKM", fileName);
   if (!myRsnOutKPKM1->SetOutput(listName, sparseName1)) return;
   if (doAdditionalCut) myRsnOutKPKM1->SetAdditionalCut(additionalCut[0], additionalCut[1], additionalCut[2]);

   AliRsnOutReader *myRsnOutKPKM2 = new AliRsnOutReader("KPKMmix", fileName);
   if (!myRsnOutKPKM2->SetOutput(listNameMix, sparseName2)) return;
   if (doAdditionalCut) myRsnOutKPKM2->SetAdditionalCut(additionalCut[0], additionalCut[1], additionalCut[2]);

   AliRsnOutReader *myRsnOutKPKM3 = new AliRsnOutReader("KPKMmix", fileName);
   if (!myRsnOutKPKM3->SetOutput(listNameMix, sparseName3)) return;
   if (doAdditionalCut) myRsnOutKPKM3->SetAdditionalCut(additionalCut[0], additionalCut[1], additionalCut[2]);

   // Drawing of 1 histogram Group
   AliRsnOutManager myOutputManager("phiKK", "#phi#rightarrowK^{+}K^{-} - p_{T}");
//     AliRsnOutManager myOutputManager("phiKK", "#phi#rightarrowK^{+}K^{-} - #eta");
   myOutputManager.AddOutput(myRsnOutKPKM1);
   myOutputManager.AddOutput(myRsnOutKPKM2);
   myOutputManager.AddOutput(myRsnOutKPKM3);
   myOutputManager.Print();

//    myOutputManager.SetScheme("$1-$2");




   AliRsnOutDraw *drawer = myOutputManager.GetDrawer();
   drawer->AddToHistogramStack("$1", "sqrt($1)");
   drawer->AddToHistogramStack("$2+$3", "sqrt($2+$3)"/*, "$1"*/);
//     drawer->AddToHistogramStack("$3", "sqrt($3)"/*, "$1"*/);
//   drawer->AddToHistogramStack("$3","sqrt($3)");
//   drawer->AddToHistogramStack("$1-$3","sqrt(pow($1,2)+pow($3,2))");
//    drawer->AddToHistogramStack("$1-$3", "sqrt($1+$3)", "", "gaus");
//   drawer->AddToHistogramStack("$3-$2","sqrt(pow($1,1)+pow($3,1))");

   drawer->SetNormMinMax(1.05, 1.09);
   drawer->SetFitMinMax(1.01, 1.03);



   TCanvas *c = new TCanvas();

   Double_t min = 0.5;
   Double_t max = 3.0;
   Double_t step = 0.1;

   Int_t dimVal = 0;
   Int_t dimCut = 1;

// //    pt full
//    min = 0.0;
//    max = 10.0;
//    step = 10.0;
//    dimVal=0;
//    dimCut=1;



//    // eta cut full
//    min = -0.9;
//    max = 0.8;
//    step = 0.1;
//    dimVal=0;
//    dimCut=2;

   if (!doAdditionalCut)
      drawer->AddFitParamHistogram("mass", "#phi mass vs. p_{T}", 1, min, max, step);
   else
      drawer->AddFitParamHistogram("mass", Form("#phi mass vs. p_{T} with #eta [%.2f,%.2f]", additionalCut[1], additionalCut[2]), 1, min, max, step);
   //     drawer.AddFitParamHistogram("width","width title",2,min,max,step);
   myOutputManager.FinalOutput(min, max, step, dimVal, dimCut, c);


//   myOutputManager.FinalOutput(0,-1,0.0,1.0,c->cd(1));

   Double_t correctMass = 1.019445;
   TCanvas *c2 = new TCanvas();
   TH1D *hMass = (TH1D*)gDirectory->FindObject("mass");
   hMass->DrawCopy("E1");
   TLine *l = new TLine(hMass->GetXaxis()->GetXmin(), correctMass, hMass->GetXaxis()->GetXmax(), correctMass);
   l->SetLineColor(kBlue);
   l->SetLineStyle(kDashed);
   l->Draw();

//    hPM->Draw();
//    c->cd(2);
//    hPMMix->Draw("SAME");

}
