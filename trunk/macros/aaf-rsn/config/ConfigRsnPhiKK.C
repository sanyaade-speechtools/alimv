Bool_t ConfigRsnPhiKK(AliRsnAnalysisManager *rsnAnalysisManager)
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

//     RsnCurrentCutsPhiKK
   gROOT->LoadMacro("RsnCurrentCutsPhiKK.C");
   RsnCurrentCutsPhiKK(pairPMhist->GetCutManager());
   RsnCurrentCutsPhiKK(pairPPhist->GetCutManager());
   RsnCurrentCutsPhiKK(pairMMhist->GetCutManager());

   // function axes
   AliRsnValue *axisIM   = new AliRsnValue("IM"  , AliRsnValue::kPairInvMass,  100,  0.9,  1.1);
   AliRsnValue *axisPt = new AliRsnValue("PT", AliRsnValue::kPairPt     ,  0.0, 10.0, 0.100);
   AliRsnValue *axisY  = new AliRsnValue("Y" , AliRsnValue::kPairY      , -1.1,  1.1, 0.100);

   // functions for TH1-like output
   AliRsnFunction *fnc    = new AliRsnFunction;
   fnc->UseSparse();

   // --> add axes
   fnc->AddAxis(axisIM);
   fnc->AddAxis(axisPt);
   fnc->AddAxis(axisY);

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
