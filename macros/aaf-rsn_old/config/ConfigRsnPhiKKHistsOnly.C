Bool_t ConfigRsnPhiKKHistsOnly(AliRsnAnalysisManager *rsnAnalysisManager)
{

   // decay channels
   AliRsnPairDef         *pairDefpm = new AliRsnPairDef(AliPID::kKaon, '+', AliPID::kKaon, '-', 333, 1.019455);
   AliRsnPairDef         *pairDefpp = new AliRsnPairDef(AliPID::kKaon, '+', AliPID::kKaon, '+', 333, 1.019455);
   AliRsnPairDef         *pairDefmm = new AliRsnPairDef(AliPID::kKaon, '-', AliPID::kKaon, '-', 333, 1.019455);

   // computation objects
   AliRsnPairFunctions   *pairPMhist = new AliRsnPairFunctions("pairPMHist", pairDefpm);
   AliRsnPairFunctions   *pairPPhist = new AliRsnPairFunctions("pairPPHist", pairDefpp);
   AliRsnPairFunctions   *pairMMhist = new AliRsnPairFunctions("pairMMHist", pairDefmm);

   //
   // -- Setup cuts ----------------------------------------------------------------------------------
   //

   // -- tracks --> PID ONLY
   // loads macro with RsnCutPIDOnly.C

// //     RsnCurrentCuts
//    gROOT->LoadMacro("RsnCurrentCuts.C");
//    RsnCurrentCuts(pairPMhist->GetCutManager());
//    RsnCurrentCuts(pairPPhist->GetCutManager());
//    RsnCurrentCuts(pairMMhist->GetCutManager());

   gROOT->LoadMacro("RsnCut2010PhiKK.C");
   RsnCut2010PhiKK(pairPMhist->GetCutManager());
   RsnCut2010PhiKK(pairPPhist->GetCutManager());
   RsnCut2010PhiKK(pairMMhist->GetCutManager());

   // function axes
   AliRsnValue *axisIM   = new AliRsnValue("IM"  , AliRsnValue::kPairInvMass,  10000,  0.9,  10);
   AliRsnValue *axisPt = new AliRsnValue("PT", AliRsnValue::kPairPt     ,  0.0, 10.0, 0.100);
   AliRsnValue *axisY  = new AliRsnValue("Y" , AliRsnValue::kPairY      , -1.1,  1.1, 0.100);

   // functions for TH1-like output
   AliRsnFunction *fnc    = new AliRsnFunction;
//    fnc->UseSparse();

   // --> add axes
   fnc->AddAxis(axisIM);
//    fnc->AddAxis(axisPt);
//    fnc->AddAxis(axisY);

   // add functions to TH1-like output
   pairPMhist->AddFunction(fnc);
   pairPPhist->AddFunction(fnc);
   pairMMhist->AddFunction(fnc);
   //pairPMhist->SetOnlyTrue();

   // add everything to analysis manager
   rsnAnalysisManager->Add(pairPMhist);
   rsnAnalysisManager->Add(pairPPhist);
   rsnAnalysisManager->Add(pairMMhist);
   return kTRUE;
}
