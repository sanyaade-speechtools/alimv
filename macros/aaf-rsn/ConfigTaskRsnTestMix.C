//
// This function configures the entire task for all resonances the user is interested in.
// This is done by creating all configuration objects which are defined in the package.
//
// Generally speaking, one has to define the following objects for each resonance:
//
//  1 - an AliRsnPairDef to define the resonance decay channel to be studied
//  2 - an AliRsnPair{Ntuple|Functions} where the output is stored
//  3 - one or more AliRsnCut objects to define track selections
//      which will have then to be organized into AliRsnCutSet objects
//  4 - an AliRsnCutManager to include all cuts to be applied (see point 3)
//  5 - definitions to build the TNtuple or histograms which are returned
//
// The return value is used to know if the configuration was successful
//
// Bool_t RsnConfigTask(AliRsnAnalysisMulti*task, const char *dataLabel)
Bool_t RsnConfigTask(AliRsnAnalysisMulti*task, const char *dataLabel)
{
  // for safety, return if no task is passed
  if (!task)
  {
    Error("ConfigTaskRsn", "Task not found");
    return kFALSE;
  }
  
  // interpret the useful information from second argument
  TString strDataLabel(dataLabel);
  Bool_t isESD   = strDataLabel.Contains("ESD");
  Bool_t isAOD   = strDataLabel.Contains("AOD");
  Bool_t isSim   = strDataLabel.Contains("sim");
  Bool_t isData  = strDataLabel.Contains("data");
  Bool_t isPass1 = strDataLabel.Contains("pass1");
  Bool_t isPass2 = strDataLabel.Contains("pass2");

  //
  // -- Set cuts for events (applied to all analyses) -----------------------------------------------
  //
  // -- Setup pairs ---------------------------------------------------------------------------------
  //

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
  

  // -- tracks --> PID
  AliRsnCutPID *cutPID = new AliRsnCutPID("cutPID", AliPID::kKaon, 0.0, kFALSE);
  
  // cut managers
  // define a proper name for each mult bin, to avoid omonyme output histos
  pairPMhist->GetCutManager()->GetCommonDaughterCuts()->AddCut(cutPID);
  pairPMhist->GetCutManager()->GetCommonDaughterCuts()->SetCutScheme("cutPID");
  pairPPhist->GetCutManager()->GetCommonDaughterCuts()->AddCut(cutPID);
  pairPPhist->GetCutManager()->GetCommonDaughterCuts()->SetCutScheme("cutPID");
  pairMMhist->GetCutManager()->GetCommonDaughterCuts()->AddCut(cutPID);
  pairMMhist->GetCutManager()->GetCommonDaughterCuts()->SetCutScheme("cutPID");
  

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
  task->GetAnalysisManager()->Add(pairPMhist);
  task->GetAnalysisManager()->Add(pairPPhist);
  task->GetAnalysisManager()->Add(pairMMhist);
  return kTRUE;
}
