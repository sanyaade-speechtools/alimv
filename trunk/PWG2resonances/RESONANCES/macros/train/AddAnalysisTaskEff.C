//
// Macro to create the full analysis manager for Resonances
//

static Double_t  cov11 = 2;
static Double_t  cov22 = 2;
static Double_t  cov33 = 0.5;
static Double_t  cov44 = 0.5;
static Double_t  cov55 = 2;
static Double_t  nSigmaToVertex = 4;
static Double_t  dcaToVertex = 3.0;
static Double_t  maxChi2PerClusterTPC = 3.5;
static Bool_t    requireTPCRefit = kTRUE;
static Bool_t    requireSigmaToVertex = kTRUE;
static Bool_t    acceptKinkDaughters = kFALSE;
static Int_t     minNClustersTPC = 50;

enum EAxis_t
{
  kMult = 0,
  kPt   = 1,
  kEta  = 2,
  kNDim = 3
} AXIS;

Bool_t AddAnalysisTaskEff
(
  const Int_t      PDG     = 333,               // phi
  const char      *outFile = "phicorr.root",    // output file name
)
{
  // define grid variables and their binnings:
  //  - event multiplicity (0 to 250 in steps of 25)
  //  - pt                 (0 to 10 in steps of 0.5)
  //  - eta                (-1 to 1 in steps of 0.1)
  Double_t multBins[] = {0., 25., 50., 75., 100., 125.0, 150., 175.0, 200.0};
  Double_t ptBins[]   = {0.,  1.,  2.,  3.,   4., 5., 6., 7., 8., 9., 10.};
  Double_t etaBins[]  = {-1.5, -1.4, -1.3, -1.2, -1.1, -1., -.9, -.8, -.7, -.6, -.5, -.4, -.3, -.2, -.1, .0, .1, .2, .3, .4, .5, .6, .7, .8, .9, 1., 1.1, 1.2, 1.3, 1.4, 1.5};

  // define binnings
  Int_t     nBins[AXIS::kNDim];
  Double_t *binLimits[AXIS::kNDim];
  nBins[AXIS::kMult] = sizeof(multBins) / sizeof(multBins[0]) - 1;
  binLimits[AXIS::kMult] = multBins;
  nBins[AXIS::kPt] = sizeof(ptBins) / sizeof(ptBins[0]) - 1;
  binLimits[AXIS::kPt] = ptBins;
  nBins[AXIS::kEta] = sizeof(etaBins) / sizeof(etaBins[0]) - 1;
  binLimits[AXIS::kEta] = etaBins;

  // create the manager
  Info("AliCFRsnTask", "Create manager");
  AliCFManager* man = new AliCFManager();

  // *** Define Event container *******************************************************************
  Info("AliCFRsnTask", "Setting up EVENT container");

  // create container
  AliCFContainer* evCont = new AliCFContainer("evContainer", "Container for events", 2, 1, &nBins[AXIS::kMult]);
  man->SetEventContainer(evCont);

  // set the bin limits
  evCont->SetBinLimits(0, binLimits[AXIS::kMult]);

  // set cuts
  TObjArray *dummy = new TObjArray(0);
  man->SetEventCutsList(0, dummy);
  man->SetEventCutsList(1, dummy);

  // *** Define Particle container *****************************************************************
  Info("AliCFRsnTask", "Setting up PARTICLE container");

  // selection steps:
  //  1) all decaying into charged KK
  //  2) acceptance
  //  3) daughters with pt > 0.2 GeV
  //  4) reconstruction
  //  5) primary selection
  //  6-7-8-9) PID (realistic full, realistic TPC, BetheBloch 10%, BetheBloch 20%)
  Int_t nSelectionSteps = 10;

  // create container
  AliCFContainer* phiCont = new AliCFContainer("phiContainer", "Container for #phi", nSelectionSteps, AXIS::kNDim, nBins);
  man->SetParticleContainer(phiCont);

  // set the bin limits
  for (Int_t i = 0; i < AXIS::kNDim; i++) phiCont->SetBinLimits(i, binLimits[i]);

  // *** Define cuts ******************************************************************************

  // define cuts for all steps
  AliCFParticleGenCuts* decayChannel = new AliCFParticleGenCuts("decayChannel", "MC particle generation cuts");
  //mcGenCuts->SetRequireIsPrimary();
  decayChannel->SetRequirePdgCode(PDG);
  decayChannel->SetDecayChannel(TDatabasePDG::Instance()->GetParticle(PDG)->DecayChannel(0)); // here choose the decay mode you want

  AliCFTrackKineCuts *trackCutPt  = new AliCFTrackKineCuts("trackCutPt", "track cuts: PT");
  AliCFTrackKineCuts *trackCutEta = new AliCFTrackKineCuts("trackCutEta", "track cuts: ETA");
  //mcKineCuts->SetChargeMC(charge);
  trackCutPt->SetPtRange(0.2, 1000000.0);
  trackCutEta->SetEtaRange(-1.0, 1.0);

  AliCFPairIsPrimaryCuts *recIsPrimaryCuts = new AliCFPairIsPrimaryCuts("recIsPrimaryCuts", "rec-level isPrimary cuts");
  recIsPrimaryCuts->GetNegCut()->SetAcceptKinkDaughters(acceptKinkDaughters);
  recIsPrimaryCuts->GetPosCut()->SetAcceptKinkDaughters(acceptKinkDaughters);
  recIsPrimaryCuts->GetNegCut()->SetRequireSigmaToVertex(1);
  recIsPrimaryCuts->GetPosCut()->SetRequireSigmaToVertex(1);
  recIsPrimaryCuts->GetNegCut()->SetMaxNSigmaToVertex(nSigmaToVertex);
  recIsPrimaryCuts->GetPosCut()->SetMaxNSigmaToVertex(nSigmaToVertex);

  AliCFPairQualityCuts *recQualityCuts = new AliCFPairQualityCuts("recQualityCuts", "rec-level quality cuts");
  recQualityCuts->GetNegCut()->SetMaxCovDiagonalElements(cov11, cov22, cov33, cov44, cov55);
  recQualityCuts->GetNegCut()->SetMaxChi2PerClusterTPC(maxChi2PerClusterTPC);
  recQualityCuts->GetNegCut()->SetMinNClusterTPC(minNClustersTPC);
  recQualityCuts->GetPosCut()->SetMaxCovDiagonalElements(cov11, cov22, cov33, cov44, cov55);
  recQualityCuts->GetPosCut()->SetMaxChi2PerClusterTPC(maxChi2PerClusterTPC);
  recQualityCuts->GetPosCut()->SetMinNClusterTPC(minNClustersTPC);

  Info("AliCFRsnTask", "Cuts for step #0: all phis decaying into K+K-");
  TObjArray *step0 = new TObjArray(0);
  step0->AddLast(decayChannel);

  Info("AliCFRsnTask", "Cuts for step #1: daughters in eta acceptance");
  TObjArray *step1 = new TObjArray(0);
  step1->AddLast(trackCutEta);

  Info("AliCFRsnTask", "Cuts for step #2: daughters with transverse momentum > 0.2 GeV");
  TObjArray *step2 = new TObjArray(0);
  step2->AddLast(trackCutPt);

  Info("AliCFRsnTask", "Cuts for step #3: reconstructed daughters");
  TObjArray *step3 = new TObjArray(0);

  Info("AliCFRsnTask", "Cuts for step #4: primary daughter tracks");
  TObjArray *step4 = new TObjArray(0);
  step4->AddLast(recIsPrimaryCuts);
  step4->AddLast(recQualityCuts);

  Info("AliCFRsnTask", "Cuts for step #5: (parallel) cut TPC bethe-bloch 0.1");
  //TObjArray *step5 = new TObjArray(0);

  Info("AliCFRsnTask", "Cuts for step #6: (parallel) cut TPC bethe-bloch 0.2");
  //TObjArray *step6 = new TObjArray(0);

  Info("AliCFRsnTask", "Cuts for step #7: (parallel) cut ESD PID kaons");
  //TObjArray *step7 = new TObjArray(0);

  Info("AliCFRsnTask", "Cuts for step #8: (parallel) cut TPC PID kaons");
  //TObjArray *step8 = new TObjArray(0);


  Info("AliCFRsnTask", "Set cut for all steps...");
  man->SetParticleCutsList(0, step0);
  man->SetParticleCutsList(1, step1);
  man->SetParticleCutsList(2, step2);
  man->SetParticleCutsList(3, dummy);
  man->SetParticleCutsList(4, step4);
  man->SetParticleCutsList(5, dummy);
  man->SetParticleCutsList(6, dummy);
  man->SetParticleCutsList(7, dummy);
  man->SetParticleCutsList(8, dummy);

  // *** Create and initialize task ***************************************************************

  // retrieve analysis manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  Info("AliCFRsnTask", "Creating task...");
  AliCFRsnTask *task = new AliCFRsnTask("AliRsnTask");
  task->SetCFManager(man); //here is set the CF manager
  task->SetRsnPDG(PDG);

  Double_t phiMass  = 1019.46 * 1E-3;
  Double_t phiGamma =    4.26 * 1E-3;
  Double_t phiSigma = phiGamma / 2.35;
  Double_t massMin  = phiMass - 3.0*phiSigma;
  Double_t massMax  = phiMass + 3.0*phiSigma;
  Info("AliCFRsnTask", "Mass range = %f - %f", massMin, massMax);
  task->SetInvMassRange(massMin, massMax);

  // output histo (number of events processed)
  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer("chist0", TH1I::Class(), AliAnalysisManager::kOutputContainer, outFile);
  // output Correction Framework Container (for acceptance & efficiency calculations)
  AliAnalysisDataContainer *coutput2 = mgr->CreateContainer("ccontainer0", AliCFContainer::Class(), AliAnalysisManager::kOutputContainer, outFile);
  AliAnalysisDataContainer *coutput3 = mgr->CreateContainer("ccontainer1", AliCFContainer::Class(), AliAnalysisManager::kOutputContainer, outFile);
  AliAnalysisDataContainer *coutput4 = mgr->CreateContainer("ccontainer2", TH1D::Class(), AliAnalysisManager::kOutputContainer, outFile);

  mgr->AddTask(task);
  mgr->ConnectInput (task, 0, mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 1, coutput1);
  mgr->ConnectOutput(task, 2, coutput2);
  mgr->ConnectOutput(task, 3, coutput3);
  mgr->ConnectOutput(task, 4, coutput4);

  return kTRUE;
}
