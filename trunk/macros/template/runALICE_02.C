#ifndef __CINT__
#include <TError.h>
#include <TROOT.h>
#include <TError.h>
#include <TString.h>
#include <TSystem.h>
#include <TROOT.h>
#endif
Long64_t fNEvents = 2 * 1e5;
Long64_t fNEventsSkip = 0;
void RunAnalysisManager(TString, TString, TString.TString); //|
Int_t runALICE(TString analysisSource = "grid", TString analysisMode = "terminate")
{
   fNEvents = 1 * 1e9;
   fNEventsSkip = 0;

   Int_t ret = 0;
   ::Info(gSystem->HostName(), Form("Running <%s> in <%s> mode ...", analysisSource.Data(), analysisMode.Data()));

   // Loading ANALYSIS Base libs
   ::Info(gSystem->HostName(), "Loading ...");
   if (LoadLibsBase()) { ::Error(gSystem->HostName(), "Error loading base libs !!!"); return kFALSE;}
   ::Info(gSystem->HostName(), "Loading done ...");
   // ++++++++++++++++ Adding Analysis Managers +++++++++++++++++++++++

//    fNEvents = 1e3;
//    fNEventsSkip = 350;


//    RunAnalysisManager("AddMyAnalysisManagerExampleTasks", analysisSource.Data(), analysisMode.Data(), "");

//    RunAnalysisManager("AddMyAnalysisManagerEvilTask", analysisSource.Data(), analysisMode.Data(), "");
//
//    // mix with parfiles
//    RunAnalysisManager("AddAnalysisManagerMixPar", analysisSource.Data(), analysisMode.Data(), "");
//
//    // rsn from aliroot
//    RunAnalysisManager("AddAnalysisManagerMixRsn", analysisSource.Data(), analysisMode.Data(), "");
//
//    // rsn with parfiles
   RunAnalysisManager("AddAnalysisManagerMixRsnPar", analysisSource.Data(), analysisMode.Data(), "");
//
//    // rsn with parfiles aod
//    RunAnalysisManager("AddAnalysisManagerMixRsnParAOD", analysisSource.Data(), analysisMode.Data(), "");
//
//    for (Int_t i = 0; i < 2; i++)
//       RunAnalysisManager("AddMyAnalysisManagerEvilTask", analysisSource.Data(), analysisMode.Data(), Form("%d", i));

} //