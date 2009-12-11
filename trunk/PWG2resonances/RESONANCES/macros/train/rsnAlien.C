//=============================================================================
//
// Steering macro for running analysis using XML list of files from AliEn.
// ---
// Usage requirements:
// -- 1: run with aliroot (not with root)
// -- 2: must have in working dir the utility macros
//       "CreateChainFromXML.C" and "LoadParsLocal.C"
// ---
// This macro is designed to be used in AliEn jobs.
//
//=============================================================================

void rsnAlien
(
  Int_t       nReadFiles  = 0,                       // number of files read from collection
  Int_t       nSkipFiles  = 0,                       // offset
  const char *addMacro    = "AddAnalysisTaskRsn.C",  // macro that adds RSN task
  const char *inputSource = "wn.xml",                // XML source
  const char *treeName    = "esdTree",               // type of data
  Bool_t      useKine     = kTRUE,
  Bool_t      createAOD   = kFALSE,
  Bool_t      useESDCuts  = kFALSE
)
{
  // check: creation of AODs can be required only with ESD source
  if (strcmp(treeName, "esdTree") && createAOD)
  {
    Error("rsnAlien.C", "Cannot require to create AODs if the input is not ESD");
    return;
  }

  // load compiled libraries
  gSystem->Load("libANALYSIS.so");
  gSystem->Load("libANALYSISalice.so");

  // load RSN library
  //gSystem->Load("/home/pulvir/code/resonances/new-package/alice-rsn-package/PWG2resonances/libPWG2resonances.so");
  gROOT->LoadMacro("LoadParsLocal.C");
  LoadParsLocal("PWG2resonances", ".");

  // connect to PROOF
  TGrid::Connect("alien://");

  // define debug depth for RSN
  AliLog::EType_t debugRsnType = AliLog::kInfo;

  // create analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("ResonanceAnalysis");

  // create MC handler (optional)
  if (useKine)
  {
    AliMCEventHandler *mcHandler  = new AliMCEventHandler();
    mgr->SetMCtruthEventHandler(mcHandler);
  }

  // create input handler according to data type
  if (!strcmp(treeName, "esdTree"))
  {
    AliESDInputHandler *esdHandler = new AliESDInputHandler();
    mgr->SetInputEventHandler(esdHandler);
    if (createAOD)
    {
      AliAODHandler *aodHandler = new AliAODHandler();
      aodHandler->SetOutputFileName("AliAOD.root");
      mgr->SetOutputEventHandler(aodHandler);
    }
  }
  else if (!strcmp(treeName, "aodTree"))
  {
    AliAODInputHandler *aodHandler = new AliAODInputHandler();
    mgr->SetInputEventHandler(aodHandler);
  }

  // if required to create AODs, add the filtering task
  if (createAOD)
  {
    gROOT->LoadMacro("AddAnalysisTaskFilter.C");
    AddAnalysisTaskFilter(useKine);
  }

  // add the main analysis task...
  gROOT->LoadMacro(addMacro);
  // ...and execute
  TString macro(addMacro);
  macro.ReplaceAll(".C","");
  gROOT->ProcessLine(Form("%s((AliLog::EType_t)%d)", macro.Data(), debugRsnType));

  // create the input TChain
  gROOT->LoadMacro("CreateChainFromXML.C");
  TChain *analysisChain = CreateChainFromXML(inputSource, treeName, nReadFiles, nSkipFiles);
  if (!analysisChain) {
    Error("runAlien", "Analysis TChain not properly initialized");
    return 1;
  }

  // initialize analysis and run it
  TStopwatch timer;
  timer.Start();
  mgr->InitAnalysis();
  mgr->PrintStatus();
  mgr->StartAnalysis("alien", analysisChain);
  timer.Stop();
  timer.Print();
}
