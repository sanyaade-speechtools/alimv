Bool_t ConfigRsnKStarKPi(AliRsnAnalysisManager *rsnAnalysisManager)
{
   // decay channels
   AliRsnPairDef         *pairDefpm = new AliRsnPairDef(AliPID::kKaon, '+', AliPID::kPion, '-', 313, 0.896);
   AliRsnPairDef         *pairDefmp = new AliRsnPairDef(AliPID::kKaon, '-', AliPID::kPion, '+', 313, 0.896);
   AliRsnPairDef         *pairDefpp = new AliRsnPairDef(AliPID::kKaon, '+', AliPID::kPion, '+', 313, 0.896);
   AliRsnPairDef         *pairDefmm = new AliRsnPairDef(AliPID::kKaon, '-', AliPID::kPion, '-', 313, 0.896);

   // computation objects
   AliRsnPairFunctions   *pairPMhist = new AliRsnPairFunctions("pairPMHist", pairDefpm);
   AliRsnPairFunctions   *pairMPhist = new AliRsnPairFunctions("pairMPHist", pairDefmp);
   AliRsnPairFunctions   *pairPPhist = new AliRsnPairFunctions("pairPPHist", pairDefpp);
   AliRsnPairFunctions   *pairMMhist = new AliRsnPairFunctions("pairMMHist", pairDefmm);
   //
   // -- Setup cuts ----------------------------------------------------------------------------------
   //

   // -- tracks --> PID ONLY
   // loads macro with RsnCutPIDOnly.C
   gROOT->LoadMacro("RsnCut2010KStarKPi.C");
   RsnCut2010KStarKPi(pairPMhist->GetCutManager());
   RsnCut2010KStarKPi(pairMPhist->GetCutManager());
   RsnCut2010KStarKPi(pairPPhist->GetCutManager());
   RsnCut2010KStarKPi(pairMMhist->GetCutManager());

   // function axes
   AliRsnValue *axisIM   = new AliRsnValue("IM", AliRsnValue::kPairInvMass,  40,  0.7, 1.1);
   AliRsnValue *axisPt = new AliRsnValue("PT", AliRsnValue::kPairPt     ,  0.0, 10.0, 0.100);
   AliRsnValue *axisY  = new AliRsnValue("Y" , AliRsnValue::kPairY      , -1.1,  1.1, 0.100);

   // functions for TH1-like output
   AliRsnFunction *fcnPt    = new AliRsnFunction;
   fcnPt->UseSparse();
   // --> add axes
   fcnPt   ->AddAxis(axisIM);
   fcnPt   ->AddAxis(axisPt);
   fcnPt   ->AddAxis(axisY);

   // add functions to TH1-like output
   pairPMhist->AddFunction(fcnPt);
   pairMPhist->AddFunction(fcnPt);
   pairPPhist->AddFunction(fcnPt);
   pairMMhist->AddFunction(fcnPt);
   //pairPMhist->SetOnlyTrue();

   // add everything to analysis manager
   rsnAnalysisManager->Add(pairPMhist);
   rsnAnalysisManager->Add(pairMPhist);
   rsnAnalysisManager->Add(pairPPhist);
   rsnAnalysisManager->Add(pairMMhist);
   return kTRUE;
}
