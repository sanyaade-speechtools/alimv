#include "AliAnalysisAlien.h" //|
AliAnalysisGrid* SetupAnalysisPlugin(TString analysisMode)
{

   AliAnalysisAlien *plugin = new AliAnalysisAlien();

   // Overwrite all generated files, datasets and output results from a previous session
   plugin->SetOverwriteMode();
   // Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
   plugin->SetRunMode(analysisMode.Data());  // VERY IMPORTANT - DECRIBED BELOW

   // seutp aliroot version
   plugin->SetAliROOTVersion("v4-21-21-AN");

   // adds Proof setting
   MySetupAnalysisPluginProof(plugin);

   // adds AliEn settings
   MySetupAnalysisPluginAliEn(plugin);

   return plugin;
}

void MySetupAnalysisPluginProof(AliAnalysisAlien *plugin)
{
   plugin->SetProofCluster("alice-caf.cern.ch");
//     plugin->SetProofCluster("alicaf@lxbsq1410.cern.ch:21001");
   plugin->SetProofCluster("skaf.saske.sk");
   //   plugin->SetProofCluster("skaf-test.saske.sk");
//    plugin->SetProofCluster("jraf.jinr.ru");
//       plugin->SetProofCluster("kiaf.sdfarm.kr");
//   plugin->SetProofCluster("pod@localhost:21001");

// Dataset to be used

   // aod
   plugin->SetProofDataSet("/PWG2/mvala/LHC10b_000115401_p2_a017#aodTree");
//    plugin->SetProofDataSet("/alice/data/LHC10h_000137366_p2#esdTree");
   plugin->SetProofDataSet("/alice/data/LHC10b_000115401_p2#esdTree");
//     plugin->SetProofDataSet("/alice/sim/LHC10h9_000137366#esdTree");
//       plugin->SetProofDataSet("/alice/sim/LHC10a12_104157#esdTree");
//       plugin->SetProofDataSet("/alice/sim/LHC10g2d_130844#esdTree");
//       plugin->SetProofDataSet("/alice/data/LHC10b_000117222_p2");
//     plugin->SetProofDataSet("/alice/data/LHC10h_000137161_p1_plusplusplus");

//     plugin->SetProofDataSet("ds.txt");
//    plugin->SetProofDataSet("mysim.txt");
//     plugin->SetProofDataSet("LHC10b.txt");
//     plugin->SetProofDataSet("/default/alicaf/LHC10a20_140500");


   // May need to reset proof. Supported modes: 0-no reset, 1-soft, 2-hard
   plugin->SetProofReset(0);
// May limit the number of workers per slave. If used with SetNproofWorkers, SetParallel(nproofworkers) will be called after connection
//   plugin->SetNproofWorkers(30);
//    plugin->SetNproofWorkersPerSlave(1);
// May request connection to alien upon connection to grid
//    plugin->SetProofConnectGrid(kTRUE);

//    plugin->SetNproofWorkers(10);
// May use a specific version of root installed in proof
//     plugin->SetRootVersionForProof("VO_ALICE@ROOT::v5-27-06a-1");
// May set the aliroot mode. Check http://aaf.cern.ch/node/83
   plugin->SetAliRootMode("default"); // Loads AF libs by default
//     plugin->SetAliRootMode("ALIROOT"); // Loads AF libs by default
// May request ClearPackages (individual ClearPackage not supported)
   plugin->SetClearPackages();
// Plugin test mode works only providing a file containing test file locations
   plugin->SetFileForTestMode("ESD_vala_LHC10a12.txt");
   plugin->SetFileForTestMode("ESD_alien_test.txt");
//    plugin->SetFileForTestMode("ESD_alien_test_local.txt");
//    plugin->SetFileForTestMode("ESD_vala_LHC10b_3_files.txt");

//    plugin->SetFileForTestMode("VALA_EXT3_ESD_LHC10b.txt");
//    plugin->SetFileForTestMode("VALA_EXT3_ESD_MC_LHC10d4.txt");
//    plugin->SetFileForTestMode("VALA_EXT3_AOD_MC_LHC10d4.txt");

   plugin->SetFileForTestMode("VALA_ESD_LHC10b.txt");
//    plugin->SetFileForTestMode("VALA_ESD_LHC10h.txt");
//    plugin->SetFileForTestMode("VALA_ESD_MC_LHC10d4.txt");
//    plugin->SetFileForTestMode("VALA_AOD_MC_LHC10d4.txt");
//    plugin->SetFileForTestMode("VALA_AOD_40_LHC10h.txt");

//     plugin->SetFileForTestMode("AOD_vala_LHC10b.txt");
//    plugin->SetFileForTestMode("AOD_vala_LHC10b_zip.txt");
   //++++++++++++++ end PROOF ++++++++++++++++

}

void MySetupAnalysisPluginAliEn(AliAnalysisAlien *plugin)
{

   plugin->SetAPIVersion("V1.1x");
   plugin->SetROOTVersion("v5-28-00c");
//    plugin->SetAliROOTVersion("v4-20-13-AN");
   plugin->SetExecutableCommand("aliroot -b -q");


   // Method 1: Create automatically XML collections using alien 'find' command.
// Define production directory LFN

   Int_t nRunsPerMaster = 0;
   Int_t maxRunsPerMaster = 100;
   TString runs = "";
   // DATA
   plugin->SetGridDataDir("/alice/data/2010/LHC10b");
   plugin->SetDataPattern("*ESDs/pass2/*ESDs.root"); // real data check reco pass and data base directory
   plugin->SetRunPrefix("000");   // real data
   runs = "117222, 117220, 117116, 117112, 117109, 117099, 117092, 117063, 117060, 117059, 117053, 117052, 117050, 117048, 116643, 116574, 116571, 116562, 116403, 116402, 116288, 116102, 115414, 115401, 115393, 115193, 115186, 114931";
   plugin->SetSplitMaxInputFileNumber(1);
//     // AOD
//     plugin->SetGridDataDir("/alice/data/2010/LHC10b");
//     plugin->SetDataPattern("*ESDs/pass2/AOD034/*AliAOD.root");
//     plugin->SetRunPrefix("000");   // real data

//    plugin->SetDataPattern("*tag.root");  // Use ESD tags (same applies for AOD's)
// ...then add run numbers to be considered
//    plugin->AddRunNumber(125020);    // simulated

//    // sim
//    plugin->SetGridDataDir("/alice/sim/LHC10d4/");
//    plugin->SetDataPattern("*ESDs.root"); // real data check reco pass and data base directory
//    plugin->SetRunPrefix("");   // sim data
//    runs = "120829,120825";
// //     runs = "120829,120825,120824,120823,120822,120821,120820,120758,120750";
//    plugin->SetSplitMaxInputFileNumber(10);

   plugin->SetGridDataDir("/alice/data/2010/LHC10h");
   plugin->SetDataPattern("*ESDs/pass2/*ESDs.root"); // real data check reco pass and data base directory
   plugin->SetRunPrefix("000");   // real data
   runs = "137366";
//    runs = "137366, 138200, 139172";
   plugin->SetSplitMaxInputFileNumber(100);


   // AOD
//     runs = "117220";
   TObjArray* array = runs.Tokenize(",");
   TObjString *str;
   TString strr, strr2_1, strr2_2;
   for (Int_t i = 0; i < array->GetEntriesFast(); i++) {
      str = (TObjString *) array->At(i);
      strr = str->GetString();
      if (!strr.IsNull()) {
         plugin->AddRunNumber(strr.Atoi());
         nRunsPerMaster++;
      }
   }


   if (nRunsPerMaster > maxRunsPerMaster) nRunsPerMaster = maxRunsPerMaster;
   plugin->SetNrunsPerMaster(nRunsPerMaster);

// Method 2: Declare existing data files (raw collections, xml collections, root file)
// If no path mentioned data is supposed to be in the work directory (see SetGridWorkingDir())
// XML collections added via this method can be combined with the first method if
// the content is compatible (using or not tags)
//   plugin->AddDataFile("tag.xml");
//   plugin->AddDataFile("/alice/data/2008/LHC08c/000057657/raw/Run57657.Merged.RAW.tag.root");

// Define alien work directory where all files will be copied. Relative to alien $HOME.
   plugin->SetGridWorkingDir("work/rsn/PbPb/0001/");
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
//    plugin->SetSplitMaxInputFileNumber(1);
// Optionally set number of failed jobs that will trigger killing waiting sub-jobs.
//    plugin->SetMaxInitFailed(5);
// Optionally resubmit threshold.
   plugin->SetMasterResubmitThreshold(90);
// Optionally set time to live (default 30000 sec)
//    plugin->SetTTL(20000);
   // 23h 30m
   plugin->SetTTL(84600);

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

   plugin->SetKeepLogs(kTRUE);

}
