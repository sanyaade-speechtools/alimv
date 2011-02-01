
#ifndef __CINT__
#include "AliRsnOutReader.h"
#include "AliRsnOutManager.h"
#include <TCanvas.h>
#include <TMath.h>
#include <AliRsnOutDraw.h>
#endif
void testRsnOutReader()
{
   gROOT->ProcessLine(".L AliRsnOutDraw.cxx+g");
   gROOT->ProcessLine(".L AliRsnOutReader.cxx+g");
   gROOT->ProcessLine(".L AliRsnOutManager.cxx+g");

   const char* fileName = "/home/mvala/ALICE/tmp/RES_TO_TEST/new/AnalysisResults.root";
//    const char* fileName="/home/mvala/ALICE/tmp/RES_TO_TEST/AnalysisResults_8641.root";
//    const char* fileName="/home/mvala/WORK/ALICE/RESULTS/AliEn-2011-01-25/0001/AnalysisResults.root";

   const char* listName = "RsnHist";
   const char* listNameMix = "RsnHist_mix";
   const char* sparseName = "_pairPMHist_IM_PT_Y";



   AliRsnOutReader *myRsnOutKPKM = new AliRsnOutReader("KPKM", fileName);
   if (!myRsnOutKPKM->SetOutput(listName, sparseName)) return;
//    TH1D *hPM = myRsnOut->Get1DHistogram(,0);
//    TH1D *hPM = myRsnOut->Get1DHistogram("_pairPMHist_IM_PT_Y",0,1,0.0,1.0);
//    hPM->SetTitle("K^{+}K^{-}");

   AliRsnOutReader *myRsnOutKPKMMix = new AliRsnOutReader("KPKMmix", fileName);
   if (!myRsnOutKPKMMix->SetOutput(listNameMix, sparseName)) return;
//    TH1D *hPMMix = myRsnOutMix->Get1DHistogram("_pairPMHist_IM_PT_Y",0);
//    TH1D *hPMMix = myRsnOutMix->Get1DHistogram("_pairPMHist_IM_PT_Y",0,1,0.0,1.0);
//    hPMMix->SetTitle("K^{+}K^{-} Mixed");


   // Drawing of 1 histogram Group
   AliRsnOutManager myOutputManager("nameOfHisto", "Title of Histo");

   myOutputManager.AddOutput(myRsnOutKPKM);
   myOutputManager.AddOutput(myRsnOutKPKMMix);
   myOutputManager.Print();

   myOutputManager.SetScheme("$1-$2");


   AliRsnOutDraw *drawer = myOutputManager.GetDrawer();
   drawer->AddToHistogramStack("$1", "sqrt($1)");
   drawer->AddToHistogramStack("$2", "sqrt($2)", "$1");
//   drawer->AddToHistogramStack("$3","sqrt($3)");
//   drawer->AddToHistogramStack("$1-$3","sqrt(pow($1,2)+pow($3,2))");
   drawer->AddToHistogramStack("$1-$3", "sqrt($1+$3)");
//   drawer->AddToHistogramStack("$3-$2","sqrt(pow($1,1)+pow($3,1))");

   TCanvas *c = new TCanvas();

   Double_t min = 0.4;
   Double_t max = 2.0;
   Double_t step = 0.1;

   Int_t dimVal = 0;
   Int_t dimCut = 1;

//    //    // pt full
//    min = 0.0;
//    max = 10.0;
//    step = 10.0;
//    dimVal=0;
//    dimCut=1;



//    // eta cut
//    min = -1.1;
//    max = 1.1;
//    step = 0.1;
//    dimVal=0;
//    dimCut=2;


   myOutputManager.FinalOutput(min, max, step, dimVal, dimCut, c);


//   myOutputManager.FinalOutput(0,-1,0.0,1.0,c->cd(1));


//    hPM->Draw();
//    c->cd(2);
//    hPMMix->Draw("SAME");

}
