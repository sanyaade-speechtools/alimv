Bool_t ConfigRsnSimple(AliRsnAnalysisManager *rsnAnalysisManager) {
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
	gROOT->LoadMacro("RsnCurrentCuts.C");
  RsnCurrentCuts(pairPMhist->GetCutManager());
  RsnCurrentCuts(pairPPhist->GetCutManager());
  RsnCurrentCuts(pairMMhist->GetCutManager());
	
  // function axes
  AliRsnValue     *axisIM   = new AliRsnValue("IM"  , AliRsnValue::kPairInvMass,  100,  0.9,  1.1);

  // functions for TH1-like output
  AliRsnFunction *fcnPt    = new AliRsnFunction;
  // --> add axes
  fcnPt   ->AddAxis(axisIM);

  // add functions to TH1-like output
  pairPMhist->AddFunction(fcnPt);
  pairPPhist->AddFunction(fcnPt);
  pairMMhist->AddFunction(fcnPt);
  //pairPMhist->SetOnlyTrue();
  
  // add everything to analysis manager
  rsnAnalysisManager->Add(pairPMhist);
  rsnAnalysisManager->Add(pairPPhist);
  rsnAnalysisManager->Add(pairMMhist);
  return kTRUE;
}
