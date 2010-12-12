#include "AliAnalysisAlien.h"
AliAnalysisGrid* SetupAnalysisPlugin(TString analysisMode) {
  
  AliAnalysisAlien *plugin = new AliAnalysisAlien();

  // Overwrite all generated files, datasets and output results from a previous session
  plugin->SetOverwriteMode();
  // Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
  plugin->SetRunMode(analysisMode.Data());  // VERY IMPORTANT - DECRIBED BELOW

  // seutp aliroot version
  plugin->SetAliROOTVersion("v4-21-09-AN");

  // adds Proof setting 
  MySetupAnalysisPluginProof(plugin);

  // adds AliEn settings
  MySetupAnalysisPluginAliEn(plugin);
  
  return plugin;
}

void MySetupAnalysisPluginProof(AliAnalysisAlien *plugin) {
    plugin->SetProofCluster("alice-caf.cern.ch");
//     plugin->SetProofCluster("alicaf@lxbsq1410.cern.ch:21001");
//     plugin->SetProofCluster("skaf.saske.sk");
//   plugin->SetProofCluster("skaf-test.saske.sk");
//       plugin->SetProofCluster("kiaf.sdfarm.kr");
//   plugin->SetProofCluster("pod@localhost:21001");

// Dataset to be used
    
    plugin->SetProofDataSet("/alice/data/LHC10h_000137366_p1#esdTree");
//     plugin->SetProofDataSet("/alice/data/LHC10b_000115401_p2#esdTree");
//     plugin->SetProofDataSet("/alice/sim/LHC10h9_000137366#esdTree");
//       plugin->SetProofDataSet("/alice/sim/LHC10a12_104157#esdTree");
//       plugin->SetProofDataSet("/alice/sim/LHC10g2d_130844#esdTree");
//       plugin->SetProofDataSet("/alice/data/LHC10b_000117222_p2");
//     plugin->SetProofDataSet("/alice/data/LHC10h_000137161_p1_plusplusplus");
      
//     plugin->SetProofDataSet("ds.txt");
//     plugin->SetProofDataSet("mysim.txt");
//     plugin->SetProofDataSet("LHC10b.txt");
//     plugin->SetProofDataSet("/default/alicaf/LHC10a20_140500");


    // May need to reset proof. Supported modes: 0-no reset, 1-soft, 2-hard
  plugin->SetProofReset(0);
// May limit the number of workers per slave. If used with SetNproofWorkers, SetParallel(nproofworkers) will be called after connection
//   plugin->SetNproofWorkers(1);
//    plugin->SetNproofWorkersPerSlave(1);
// May request connection to alien upon connection to grid
//     plugin->SetProofConnectGrid(kTRUE);

//    plugin->SetNproofWorkers(5);
// May use a specific version of root installed in proof
//     plugin->SetRootVersionForProof("VO_ALICE@ROOT::v5-27-06a-1");
// May set the aliroot mode. Check http://aaf.cern.ch/node/83
    plugin->SetAliRootMode("default"); // Loads AF libs by default
//     plugin->SetAliRootMode("ALIROOT"); // Loads AF libs by default
// May request ClearPackages (individual ClearPackage not supported)
    plugin->SetClearPackages();
// Plugin test mode works only providing a file containing test file locations
    plugin->SetFileForTestMode("ESD_vala_LHC10a12.txt");
   //++++++++++++++ end PROOF ++++++++++++++++

}

void MySetupAnalysisPluginAliEn(AliAnalysisAlien *plugin) {

   plugin->SetAPIVersion("V1.1x");
   plugin->SetROOTVersion("v5-27-06b");
//    plugin->SetAliROOTVersion("v4-20-13-AN");
   // Method 1: Create automatically XML collections using alien 'find' command.
// Define production directory LFN
   plugin->SetGridDataDir("/alice/sim/LHC10e6");
   
//    plugin->SetGridDataDir("/alice/data/2010/LHC10b");
// Set data search pattern
  plugin->SetDataPattern("*ESDs.root");  // simulated, tags not used
//   plugin->SetDataPattern("*ESDs/pass2/*ESDs.root"); // real data check reco pass and data base directory
//   plugin->SetRunPrefix("000");   // real data
  
//    plugin->SetDataPattern("*tag.root");  // Use ESD tags (same applies for AOD's)
// ...then add run numbers to be considered
//    plugin->AddRunNumber(125020);    // simulated

//   TString runs = "117222:117220:117120:117118:117116:117112:117099";
//   runs += ":117092:117063:117060:117059:117053:117052:117050";
  TString runs = "117222";
  runs = "117054";
  TObjArray* array = runs.Tokenize ( ":" );
  TObjString *str;
  TString strr,strr2_1,strr2_2;
  for ( Int_t i = 0;i < array->GetEntriesFast();i++ ) {
    str = ( TObjString * ) array->At ( i );
    strr = str->GetString();
    if ( !strr.IsNull() ) {
      plugin->AddRunNumber(strr.Atoi());
    }
  }

// Method 2: Declare existing data files (raw collections, xml collections, root file)
// If no path mentioned data is supposed to be in the work directory (see SetGridWorkingDir())
// XML collections added via this method can be combined with the first method if
// the content is compatible (using or not tags)
//   plugin->AddDataFile("tag.xml");
//   plugin->AddDataFile("/alice/data/2008/LHC08c/000057657/raw/Run57657.Merged.RAW.tag.root");

// Define alien work directory where all files will be copied. Relative to alien $HOME.
   plugin->SetGridWorkingDir("work/rsnTest/001");
// Declare alien output directory. Relative to working directory.
   plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output
// Declare the analysis source files names separated by blancs. To be compiled runtime
// using ACLiC on the worker nodes.
//    plugin->SetAnalysisSource("AliAnalysisTaskCustomMix.cxx");
//    plugin->SetAdditionalRootLibs("CORRFW PWG2resonances");
//    plugin->SetAdditionalRootLibs("PWG2resonances");
//    plugin->SetAdditionalRootLibs("PWG2resonances");
//
//    plugin->SetAdditionalLibs("AliAnalysisTaskCustomMix.h AliAnalysisTaskCustomMix.cxx");
//    plugin->EnablePackage("PWG2resonances");
//    plugin->EnablePackage("");
//    plugin->EnablePackage("");
// Declare all libraries (other than the default ones for the framework. These will be
// loaded by the generated analysis macro. Add all extra files (task .cxx/.h) here.

// No need for output file names. Procedure is automatic.
//   plugin->SetOutputFiles("Pt.ESD.1.root");
//   plugin->SetDefaultOutputs();
// No need define the files to be archived. Note that this is handled automatically by the plugin.
//   plugin->SetOutputArchive("log_archive.zip:stdout,stderr");
// Set a name for the generated analysis macro (default MyAnalysis.C) Make this unique !
   plugin->SetAnalysisMacro("AnalysisTest.C");
// Optionally set maximum number of input files/subjob (default 100, put 0 to ignore). The optimum for an analysis
// is correlated with the run time - count few hours TTL per job, not minutes !
   plugin->SetSplitMaxInputFileNumber(10);
// Optionally set number of failed jobs that will trigger killing waiting sub-jobs.
   plugin->SetMaxInitFailed(5);
// Optionally resubmit threshold.
   plugin->SetMasterResubmitThreshold(90);
// Optionally set time to live (default 30000 sec)
   plugin->SetTTL(20000);
// Optionally set input format (default xml-single)
   plugin->SetInputFormat("xml-single");
// Optionally modify the name of the generated JDL (default analysis.jdl)
   plugin->SetJDLName("TaskMix.jdl");
// Optionally modify job price (default 1)
   plugin->SetPrice(1);
// Optionally modify split mode (default 'se')
   plugin->SetSplitMode("se");
// set number of files to test
   plugin->SetNtestFiles(2);

}
