//
// Add the filtering analysis task which generates the AOD
//
void AddAnalysisTaskFilter(Bool_t useKine)
{
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  // Filtering of MC particles (decays conversions etc)
  // this task is also needed to set the MCEventHandler
  // to the AODHandler, this will not be needed when
  // AODHandler goes to ANALYSISalice
  if (useKine)
  {
    AliAnalysisTaskMCParticleFilter *kinefilter = new AliAnalysisTaskMCParticleFilter("MCFilter");
    mgr->AddTask(kinefilter);
  }

  // define ESD track cuts
  AliESDtrackCuts* esdTrackCuts = new AliESDtrackCuts("AliESDtrackCuts", "Standard");
  esdTrackCuts->SetMinNClustersTPC(50);
  esdTrackCuts->SetMaxChi2PerClusterTPC(3.5);
  esdTrackCuts->SetMaxCovDiagonalElements(2.,2.,.5,.5,2.);
  esdTrackCuts->SetRequireTPCRefit(kFALSE);
  esdTrackCuts->SetMaxNsigmaToVertex(3.);
  esdTrackCuts->SetRequireSigmaToVertex(kTRUE);
  esdTrackCuts->SetAcceptKinkDaughters(kFALSE);

  // define filter
  AliAnalysisFilter* trackFilter = new AliAnalysisFilter("trackFilter");
  trackFilter->AddCuts(esdTrackCuts);

  // define analysis task for filter
  // if no track filter is set, all ESD tracks will become AOD tracks
  AliAnalysisTaskESDfilter *esdfilter = new AliAnalysisTaskESDfilter("ESD Filter");
  esdfilter->SetTrackFilter(trackFilter);

  // add the task to manager
  mgr->AddTask(esdfilter);

  // create containers for input/output
  AliAnalysisDataContainer *in  = mgr->GetCommonInputContainer();
  AliAnalysisDataContainer *out = mgr->GetCommonOutputContainer();
  out->SetSpecialOutput();
  if (useKine) {
    mgr->ConnectInput (kinefilter, 0, in );
    mgr->ConnectOutput(kinefilter, 0, out);
  }
  mgr->ConnectInput (esdfilter, 0, in );
  mgr->ConnectOutput(esdfilter, 0, out);
}

