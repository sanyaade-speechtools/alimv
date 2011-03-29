#ifndef __CINT__
#include <TString.h>
#include <AliRsnOutManager.h>
#include <AliRsnOutFunction.h>
#include <AliRsnOutPad.h>
#include <AliRsnOutDrawObject.h>
#include <TF1.h>
#include <AliRsnOutParamHist.h>
#endif
TString gBaseName = "Hist_IM_PT_Y";
gBaseName = "Hist_IM_PT";
void RunRsnOutput()
{

   Load();
   TString fileName1 = "/home/mvala/ALICE/RESULTS/AliEn-2011-02-16/step_1/AnalysisResults.root";
   TString fileName2 = "/home/mvala/ALICE/RESULTS/AliEn-2011-02-16/step_10/AnalysisResults.root";
   TString fileName3 = "/home/mvala/ALICE/RESULTS/AliEn-2011-02-16/step_100/AnalysisResults.root";
   TString fileName4 = "/home/mvala/ALICE/RESULTS/AliEn-2011-02-16/step_100_no_vz/AnalysisResults.root";

//    fileName1 = "/home/mvala/tmp/alimv/0015/AnalysisResults.root";
//    fileName2 = "/home/mvala/tmp/alimv/0016/AnalysisResults.root";
//    fileName3 = "/home/mvala/tmp/alimv/0017/AnalysisResults.root";

// fileName1 = "/tmp/alimv/last/AnalysisResults.root";

   fileName1 = "/home/mvala/ALICE/RESULTS/PROOF_PBPB/1mil/NSigma/AnalysisResults.root";

//    ShowMyHist(fileName1,0.0,3.0,3.0,0,1,"step1","step 1");

   RsnPMVsPMmix(fileName1, 0.0, 3.0, 3.0, 0, 1, "step1", "step 1");
//       RsnPMVsPMmix(fileName1,0.0,3.0,0.5,0,1,"step1","step 1");s
//       RsnPMVsPMmix(fileName1,0.0,3.0,0.1,0,1,"step1","step 1");
//    RsnPMVsPMmix(fileName2,0.0,3.0,3.0,0,1,"step10","step 10");
//    RsnPMVsPMmix(fileName3,0.0,3.0,3.0,0,1,"step100","step 100");



//    RsnPMVsPMmix(fileName3, 0.5, 3.0, 2.5, 0, 1, "1");
//    RsnPMVsPMmix(fileName1,0.5,3.0,0.2,0,1,"2");
//    RsnPMVsPMmix(fileName1,0.5,3.0,0.5,0,1,"3");
//    RsnPMVsPMmix(fileName1,2.0,2.1,0.1,0,1,"3");
//       RsnPMVsPMmix(fileName1,0.5,3.0,2.5,0,1,"1");
//    RsnPPVsPPmix(fileName1,0.0,3.0,3.0,0,1,"step1","step 1");
//    RsnPPVsPPmix(fileName2,"2");
//    RsnPPVsPPmix(fileName3,"3");

//    RsnPPmixVsPPmix(fileName1, fileName2, fileName3, fileName4, "1");

}


void RsnPMVsPMmix(TString fileName, Double_t min, Double_t max, Double_t step, Int_t valDim, Int_t cutDim, TString canvasNameStr = "0", TString canvasTitleStr = "0")
{
   TString prefix = "";
   prefix = "RSN";
   TString partName = "";
   partName = "Phi";


   AliRsnOutManager *rsnOutMgr = new AliRsnOutManager(Form("mgrRsnPMVsPMmix_%s", canvasNameStr.Data()), Form("Phi->K+K- vs. Pt (%s)", canvasTitleStr.Data()));
   AliRsnOutInput *input1 = new AliRsnOutInput("#1", fileName.Data());
   input1->SetInputSource(Form("RsnHist%s", partName.Data()), Form("%s_pairPM%s", prefix.Data(), gBaseName.Data()));
   rsnOutMgr->AddInput(input1);
   AliRsnOutInput *input2 = new AliRsnOutInput("#2", fileName.Data());
   input2->SetInputSource(Form("RsnHist%s", partName.Data()), Form("%s_pairPP%s", prefix.Data(), gBaseName.Data()));
//    input2->SetInputSource(Form("RsnHist%s", partName.Data()), Form("%s_pairPP%s", prefix.Data(),gBaseName.Data()));
   input2->SetNormalizeInput(input1, 1.04, 1.09);
   rsnOutMgr->AddInput(input2);
//    AliRsnOutInput *input3 = new AliRsnOutInput("#3", fileName.Data());
//    input3->SetInputSource(Form("RsnHist%s", partName.Data()), Form("%s_pairPP%s", prefix.Data(),gBaseName.Data()));
//    input3->SetNormalizeInput(input1);
//    rsnOutMgr->AddInput(input3);

   rsnOutMgr->Print();

   TStyle *s = 0;
   AliRsnOutPad *pad = new AliRsnOutPad("name", "#phi#rightarrowK^{+}K^{-} P_{T}");
   AliRsnOutDrawObject *hist1 = new AliRsnOutDrawObject("K^{+}K^{-}", new AliRsnOutFunction("#1", "sqrt(#1)"));
   s = hist1->CreateStyle("MyStyle1");
   s->SetHistLineColor(kBlue);
   pad->Add(hist1);

   AliRsnOutDrawObject *hist2 = new AliRsnOutDrawObject("K^{+}K^{+} mix", new AliRsnOutFunction("#2", "sqrt(#2)"));
   s = hist2->CreateStyle("MyStyle2");
   s->SetHistLineColor(kRed);
   pad->Add(hist2);

   AliRsnOutDrawObject *hist3 = new AliRsnOutDrawObject("signal", new AliRsnOutFunction("#1-#2", "sqrt(#1+#2)"));
   s = hist3->CreateStyle("MyStyle3");
   s->SetHistLineColor(kBlack);
//    TF1 *fit = new TF1("myfit","gaus",1.00,1.04);
//    TF1 *fit = GetBreitWignerGausFun(1.00, 1.04);
   TF1 *fit = GetBreitWignerGausFun(1.01, 1.03);
   fit->SetLineColor(kGreen);
   fit->SetLineWidth(2);
   hist3->SetFitFun(fit);
   pad->Add(hist3);

   pad->DrawZeroLine(kTRUE);
   pad->SetLegend(new TLegend(0.15, 0.6, 0.35, 0.8));
   rsnOutMgr->SetPad(pad);

   AliRsnOutParamHist *signal = new AliRsnOutParamHist(hist1, "signal", "#phi P_{T} distribution");
//    Double_t signalPar[] = { 1.00, 1.04 };
   Double_t signalPar[] = { 1.01, 1.03 };
   signal->SetParameters(AliRsnOutParamHist::kHistIntegral, signalPar);
   signal->SetDrawOptions("E1");


   Double_t massPhi = 1.019445;
   AliRsnOutParamHist *mass = new AliRsnOutParamHist(hist3, "mass", Form("Mass of #phi (%.5f)", massPhi));
   Double_t massPar[] = { 1 };
   mass->SetParameters(AliRsnOutParamHist::kFitFunParam, massPar);
   s = mass->CreateStyle("MyStyle3_mass");
   s->SetHistLineColor(kRed);
   s->SetFuncColor(kBlack);
   s->SetFuncStyle(kDashed);
   mass->DrawDefaultValueLine(massPhi);
   mass->SetDrawOptions("E1");

   Double_t widthPhi = 0.00426;
   AliRsnOutParamHist *width = new AliRsnOutParamHist(hist3, "width", Form("width of #phi (%.5f)", widthPhi));
   Double_t widthPar[] = { 0 };
   width->SetParameters(AliRsnOutParamHist::kFitFunParam, widthPar);
   s = width->CreateStyle("MyStyle3_width");
   s->SetHistLineColor(kBlue);
   s->SetFuncColor(kBlack);
   s->SetFuncStyle(kDashed);
   width->DrawDefaultValueLine(widthPhi);
   width->SetDrawOptions("E1");



   if ((max - min) / step != 1.0) {
      rsnOutMgr->AddParamOutput(signal);
      rsnOutMgr->AddParamOutput(mass);
      rsnOutMgr->AddParamOutput(width);
   } else {
      rsnOutMgr->AddParamOutput(signal);
   }
   rsnOutMgr->SetOutputSettings(min, max, step, valDim, cutDim);
//    rsnOutMgr->SetOutputSettings(0.5, 3.0, 0.1, 0, 1);
   rsnOutMgr->Draw();

}

void RsnPPVsPPmix(TString fileName, Double_t min, Double_t max, Double_t step, Int_t valDim, Int_t cutDim, TString canvasNameStr = "0", TString canvasTitleStr = "0")
{

   TString prefix = "";
   prefix = "RSN";
   TString partName = "";
   partName = "Phi";


   AliRsnOutManager *rsnOutMgr = new AliRsnOutManager(Form("mgrRsnPPVsPPmix_%s", canvasNameStr.Data()), "Phi->K+K- vs. Pt");
   AliRsnOutInput *input1 = new AliRsnOutInput("#1", fileName.Data());
   input1->SetInputSource(Form("RsnHist%s", partName.Data()), Form("%s_pairPP%s", prefix.Data(), gBaseName.Data()));
   rsnOutMgr->AddInput(input1);
   AliRsnOutInput *input2 = new AliRsnOutInput("#2", fileName.Data());
   input2->SetInputSource(Form("RsnHist%s_mix", partName.Data()), Form("%s_pairPP%s", prefix.Data(), gBaseName.Data()));
//    input2->SetInputSource(Form("RsnHist%s", partName.Data()), Form("%s_pairPP%s", prefix.Data(),gBaseName.Data()));
   input2->SetNormalizeInput(input1, 1.04, 1.09);
   rsnOutMgr->AddInput(input2);
//    AliRsnOutInput *input3 = new AliRsnOutInput("#3", fileName.Data());
//    input3->SetInputSource(Form("RsnHist%s", partName.Data()), Form("%s_pairPP%s", prefix.Data(),gBaseName.Data()));
//    input3->SetNormalizeInput(input1);
//    rsnOutMgr->AddInput(input3);

   rsnOutMgr->Print();

   TStyle *s = 0;
   AliRsnOutPad *pad = new AliRsnOutPad("name", "#phi#rightarrowK^{+}K^{-} P_{T}");
   AliRsnOutDrawObject *hist1 = new AliRsnOutDrawObject("myname1", new AliRsnOutFunction("#1", "sqrt(#1)"));
   s = hist1->CreateStyle("MyStyle1");
   s->SetHistLineColor(kBlue);
   pad->Add(hist1);

   AliRsnOutDrawObject *hist2 = new AliRsnOutDrawObject("myname2", new AliRsnOutFunction("#2", "sqrt(#2)"));
   s = hist2->CreateStyle("MyStyle2");
   s->SetHistLineColor(kRed);
   pad->Add(hist2);

   AliRsnOutDrawObject *hist3 = new AliRsnOutDrawObject("myname3", new AliRsnOutFunction("#1-#2", "sqrt(#1+#2)"));
   s = hist3->CreateStyle("MyStyle3");
   s->SetHistLineColor(kViolet);
// //    TF1 *fit = new TF1("myfit","gaus",1.00,1.04);
//    TF1 *fit = GetBreitWignerGausFun(1.00, 1.04);
//    fit->SetLineColor(kGreen);
//    fit->SetLineWidth(2);
//    hist3->SetFitFun(fit);
   pad->Add(hist3);

   pad->DrawZeroLine(kTRUE);
   rsnOutMgr->SetPad(pad);

   rsnOutMgr->SetOutputSettings(min, max, step, valDim, cutDim);
   rsnOutMgr->Draw();

}
void RsnPPmixVsPPmix(TString fileName1, TString fileName2 = "", TString fileName3 = "", TString fileName4 = "", TString canvasNameStr = "0")
{
   TStyle *s = 0;

   TString prefix = "";
   prefix = "RSN";
   TString partName = "";
   partName = "Phi";

   Bool_t doNorm = kFALSE;
   doNorm = kTRUE;

   AliRsnOutManager *rsnOutMgr = new AliRsnOutManager(Form("mgrRsnPPmixVsPPmix_%s", canvasNameStr.Data()), "Phi->K+K- vs. Pt");

   if (!fileName1.IsNull()) {
      AliRsnOutInput *input1 = new AliRsnOutInput("#1", fileName1.Data());
      input1->SetInputSource(Form("RsnHist%s_mix", partName.Data()), Form("%s_pairPM%s", prefix.Data(), gBaseName.Data()));
      rsnOutMgr->AddInput(input1);
      AliRsnOutPad *pad = new AliRsnOutPad("name", "#phi#rightarrowK^{+}K^{-} P_{T}");
      AliRsnOutDrawObject *hist1 = new AliRsnOutDrawObject("mix multi step #1", new AliRsnOutFunction("#1", "sqrt(#1)"));
      s = hist1->CreateStyle("MyStyle1");
      s->SetHistLineColor(kBlue);
      pad->Add(hist1);
   }

   if (!fileName2.IsNull()) {
      AliRsnOutInput *input2 = new AliRsnOutInput("#2", fileName2.Data());
      input2->SetInputSource(Form("RsnHist%s_mix", partName.Data()), Form("%s_pairPM%s", prefix.Data(), gBaseName.Data()));
//    input2->SetInputSource(Form("RsnHist%s", partName.Data()), Form("%s_pairPP%s", prefix.Data(),gBaseName.Data()));
      if (doNorm) input2->SetNormalizeInput(input1, 1.04, 1.09);
      rsnOutMgr->AddInput(input2);
      AliRsnOutDrawObject *hist2 = new AliRsnOutDrawObject("mix multi step #2", new AliRsnOutFunction("#2", "sqrt(#2)"));
      s = hist2->CreateStyle("MyStyle2");
      s->SetHistLineColor(kRed);
      pad->Add(hist2);
   }

   if (!fileName3.IsNull()) {
      AliRsnOutInput *input3 = new AliRsnOutInput("#3", fileName3.Data());
      input3->SetInputSource(Form("RsnHist%s_mix", partName.Data()), Form("%s_pairPM%s", prefix.Data(), gBaseName.Data()));
      if (doNorm) input3->SetNormalizeInput(input1, 1.04, 1.09);
      rsnOutMgr->AddInput(input3);
      AliRsnOutDrawObject *hist3 = new AliRsnOutDrawObject("mix multi step #3", new AliRsnOutFunction("#3", "sqrt(#3)"));
      s = hist3->CreateStyle("MyStyle3");
      s->SetHistLineColor(kGreen);
      pad->Add(hist3);
   }

   if (!fileName4.IsNull()) {
      AliRsnOutInput *input4 = new AliRsnOutInput("#4", fileName4.Data());
      input4->SetInputSource(Form("RsnHist%s_mix", partName.Data()), Form("%s_pairPM%s", prefix.Data(), gBaseName.Data()));
      if (doNorm) input4->SetNormalizeInput(input1, 1.04, 1.09);
      rsnOutMgr->AddInput(input4);
      AliRsnOutDrawObject *hist4 = new AliRsnOutDrawObject("mix multi step #4", new AliRsnOutFunction("#4", "sqrt(#4)"));
      s = hist4->CreateStyle("MyStyle4");
      s->SetHistLineColor(kViolet);
      pad->Add(hist4);
   }

   rsnOutMgr->Print();


//    AliRsnOutDrawObject *hist3 = new AliRsnOutDrawObject(new AliRsnOutFunction("#1-#2", "sqrt(#1+#2)"));
//    s = hist3->CreateStyle("MyStyle3");
//    s->SetHistLineColor(kViolet);
// // //    TF1 *fit = new TF1("myfit","gaus",1.00,1.04);
// //    TF1 *fit = GetBreitWignerGausFun(1.00, 1.04);
// //    fit->SetLineColor(kGreen);
// //    fit->SetLineWidth(2);
// //    hist3->SetFitFun(fit);
//    pad->Add(hist3);

   pad->DrawZeroLine(kTRUE);
   pad->SetLegend(new TLegend(0.15, 0.7, 0.35, 0.8));


   rsnOutMgr->SetPad(pad);

   rsnOutMgr->SetOutputSettings(0.0, 10.0, 10.0, 0, 1);
//    rsnOutMgr->SetOutputSettings(0.5, 3.0, 0.1, 0, 1);
   rsnOutMgr->Draw();

}

void Load()
{
   gROOT->ProcessLine(".L ../AliRsnOutInput.cxx+g");
   gROOT->ProcessLine(".L ../AliRsnOutFunction.cxx+g");
   gROOT->ProcessLine(".L ../AliRsnOutDrawObject.cxx+g");
   gROOT->ProcessLine(".L ../AliRsnOutPad.cxx+g");
   gROOT->ProcessLine(".L ../AliRsnOutParamHist.cxx+g");
   gROOT->ProcessLine(".L ../AliRsnOutManager.cxx+g");
   gROOT->ProcessLine(".x FitFunctions.C");
}

TF1 *GetBreitWignerGausFun(Double_t min = 1.01, Double_t max = 1.03)
{
   // Fitting SNR histo
   printf("Fitting...\n");

   // Setting fit range and start values
   Double_t fr[2];
   Double_t sv[4], pllo[4], plhi[4], fp[4], fpe[4];

   fr[0] = min; // b.b.
   fr[1] = max; // b.b.
//    fr[0] = 0.988; // b.b.
//    fr[1] = 1.04; // b.b.

   Double_t mySigma = 0.0017;
//    mySigma = 0.0030;
   //pllo[0]=0.00445; pllo[1]=1.019; pllo[2]=0.001; pllo[3]=0.002;
   //plhi[0]=0.00446; plhi[1]=1.021; plhi[2]=10; plhi[3]=0.009;
   pllo[0] = 0.0030; pllo[1] = 1.018; pllo[2] = 0.001; pllo[3] = mySigma; // b.b.
   plhi[0] = 0.0090; plhi[1] = 1.020; plhi[2] = 10.000; plhi[3] = mySigma; // b.b.

   //sv[0]=.007; sv[1]=1.019; sv[2]=5.; sv[3]=0.004;
   sv[0] = .00436; sv[1] = 1.0196; sv[2] = 5.; sv[3] = mySigma; // b.b.

   Double_t chisqr;
   Int_t    ndf;
   TF1 *ffit = new TF1("aaa", brewigGaufun, fr[0], fr[1], 4);
   ffit->SetParameters(sv);
   ffit->SetParNames("Width", "Mean", "Area", "Sigma");

   for (Int_t i = 0; i < 4; i++) {
      ffit->SetParLimits(i, pllo[i], plhi[i]);
   }

   return ffit;

}

void ShowMyHist(TString fileName, Double_t min, Double_t max, Double_t step, Int_t valDim, Int_t cutDim, TString canvasNameStr = "0", TString canvasTitleStr = "0")
{
   TString prefix = "";
   prefix = "RSN";
   TString partName = "";
   partName = "Phi";


   AliRsnOutManager *rsnOutMgr = new AliRsnOutManager(Form("mgrRsnPMVsPM_%s", canvasNameStr.Data()), Form("Phi->K+K- vs. Pt (%s)", canvasTitleStr.Data()));
   AliRsnOutInput *input1 = new AliRsnOutInput("#1", fileName.Data());
   input1->SetInputSource(Form("RsnHist%s", partName.Data()), Form("%s_pairPM%s", prefix.Data(), gBaseName.Data()));
   rsnOutMgr->AddInput(input1);

   rsnOutMgr->Print();

   TStyle *s = 0;
   AliRsnOutPad *pad = new AliRsnOutPad("name", "#phi#rightarrowK^{+}K^{-} P_{T}");
   AliRsnOutDrawObject *hist1 = new AliRsnOutDrawObject("K^{+}K^{-}", new AliRsnOutFunction("#1", "sqrt(#1)"));
   s = hist1->CreateStyle("MyStyle1");
   s->SetHistLineColor(kBlue);
   pad->Add(hist1);

//    pad->DrawZeroLine(kTRUE);
//    pad->SetLegend(new TLegend(0.15, 0.6, 0.35, 0.8));
   rsnOutMgr->SetPad(pad);

   rsnOutMgr->SetOutputSettings(min, max, step, valDim, cutDim);
//    rsnOutMgr->SetOutputSettings(0.5, 3.0, 0.1, 0, 1);
   rsnOutMgr->Draw();

}