#ifndef __CINT__
#endif
Bool_t RunOnProof(TString macro, Long64_t numberOfEvents, Long64_t skipEvents)
{

  fgMode = "proof";

//   fgDontRun = "a";
//   fgPROOFForceLocal = kTRUE;

//   fgRootVersionInProof = "v5-25-02";
  
  fgPARSEnableOnly="";
  fgPARSEnableOnly="alitrain_2009-11-29_1122";
  fgPARSEnableOnly="alitrain_latest";
  fgPARSEnableOnly="alitrain_v4-18-12-AN";
  fgPARSEnableOnly="alitrain_v4-18-13-AN";
  
  fgUseLocalLibs = kTRUE;
  fgAlirootLibPath = "$ALICE_ROOT/lib/tgt_$ALICE_TARGET";
  fgAlirootLibPath = "$ALIMV/$ALIMV_TARGET/lib";
  fgAlirootLibPath = "/home/mvala/WORK/SVN/alitrain/5.25.05/linuxx8664gcc/lib";
  fgLIBS = "STEERBase:ESD:AOD:ANALYSIS:ANALYSISalice:CORRFW";
  fgLIBS += ":PWG2spectra:PWG2AOD:PWG2femtoscopy:PWG2femtoscopyUser";
  fgLIBS += ":PWG2resonances";
      
  fgPARS = "";
//   // adds standard ANALYSIS parfiles
//   fgPARS = "STEERBase:ESD:AOD:ANALYSIS:ANALYSISalice:CORRFW";
// //   fgPARS += ":ANALYSISTest";
//   // adds standard RESONANCE parfiles
//   fgPARS += ":PWG2resonances";
//   fgPARS += ":PWG2resonancesTest";

  fgPARS = "PWG2resonancesTest";
  
  fgPARSClean = "";
// fgPARSClean = "PWG2resonancesTest";
//   fgPARSClean = "all";



  fgUser = gSystem->ExpandPathName("$USER");
//   fgUser = "aliceskaf";
  
//   fgRootVersionInProof = "v5-24-00";
//     fReset="RESET";
//   fgProofToConnect = Form("%s@localhost:2093",fgUser.Data());
//     fgProofToConnect = Form("%s@localhost:3093",fgUser.Data());
  fgProofToConnect = Form("%s@alicecaf.cern.ch", fgUser.Data());
   fgProofToConnect = Form("%s@alicepc100.jinr.ru",fgUser.Data());
//   fgProofToConnect = Form("%s@prf001-iep-grid.saske.sk",fgUser.Data());
  fgProofToConnect = Form("%s@skaf.saske.sk", fgUser.Data());
//   fgProofToConnect = Form("%s@localhost",fgUser.Data());
//     fgPARSClean = "PWG2resonances:PWG2resonancesTEST";
//     fgPARSClean="ALL";

  fgDataType = (Int_t) AliRsnUtils::kDataSet;
//   fgInputFileName = "/COMMON/COMMON/LHC08c12_0.9TeV_0.5T#esdTree";
  fgInputFileName = "/COMMON/COMMON/LHC09a4_10TeV#esdTree";

// fgInputFileName = "/COMMON/COMMON/LHC09a4_run8101X#esdTree";
// fgInputFileName = "/COMMON/COMMON/LHC09a4_run8158X#esdTree";
// fgInputFileName = "/COMMON/COMMON/LHC09a4_run8159X#esdTree";
// fgInputFileName = "/COMMON/COMMON/LHC09a5_run9000X#esdTree";
// fgInputFileName = "/COMMON/COMMON/LHC09a6_run9200X#esdTree";

//   fgInputFileName = "/COMMON/COMMON/test#esdTree";
  fgInputFileName = "/COMMON/COMMON/LHC09a4_80xxx_SE#esdTree";
  
//   fgInputFileName = "/COMMON/COMMON/LHC09a4_80xxx#esdTree";
//   fgInputFileName = "/COMMON/COMMON/LHC09a4_81xxx#esdTree";
  fgInputFileName = "/COMMON/COMMON/LHC09d9_137001#esdTree";

//   fgInputFileName = "/COMMON/COMMON/LHC09a4_81xxx#esdTree";
//   fgInputFileName = "/COMMON/COMMON/test#esdTree";
//   fgInputFileName = "/COMMON/COMMON/test_ram#esdTree";
//     fgInputFileName = "/default/mvala/LHC09a4_80xxx_test#esdTree";
  fgTreeName = "esdTree";

//   fgInputFileName = "/COMMON/COMMON/LHC09a4_AOD1#aodTree";
// //   fgInputFileName = "/COMMON/COMMON/LHC09a4_AOD1_SE#aodTree";
// //   fgInputFileName = "/COMMON/COMMON/LHC09a4_AOD1_TEST#aodTree";
// // //   fgInputFileName = "/COMMON/COMMON/LHC09a4_aod_test#aodTree";
//   fgTreeName = "aodTree";

//   fgInputFileName = "/PWG2/mvala/LHC09a4_AOD_3_out#filesTree";
//   fgInputFileName = "/PWG0/aliprod/LHC09a4_ESD_14_out#filesTree";
// //   fgInputFileName = "/COMMON/COMMON/myOutTest#filesTree";
// //   fgInputFileName = "/COMMON/COMMON/LHC09a4_ESD_14_test2#filesTree";
//   fgTreeName = "filesTree";
  
//    fgDataType = (Int_t) AliRsnUtils::kTxt;
//    fgInputFileName = "ESD_alicepc100_PDC08.txt";
//    fgInputFileName = "ESD_alicepc100_PDC08_test_hdd.txt";


  // sets up macro
  fgMacro = macro;

  // number of events
  fgNumOfEvents = numberOfEvents;

  // number of events to skip
  fgNumOfEventsSkip = skipEvents;


  return runProof();
}

Bool_t RunLocaly(TString macro, Long64_t numberOfEvents, Long64_t skipEvents)
{

  fgMode = "local";
  
  fgDontRun = "";

  fgUseLocalLibs = kTRUE;
  fgAlirootLibPath = "$ALICE_ROOT/lib/tgt_$ALICE_TARGET";
  fgAlirootLibPath = "$ALIMV/$ALICE_TARGET/lib";
  fgAlirootLibPath = "/home/mvala/ALICE/AliTrain/linuxx8664gcc/lib/";
  
  gSystem->Load("libVMC");
  gSystem->Load("libNet");
  gSystem->Load("libTree");
  gSystem->Load("libPhysics");
  
  gSystem->Load("libProofPlayer");

  
  // adds standard ANALYSIS parfiles
  fgPARS = "STEERBase:ESD:AOD:ANALYSIS:ANALYSISalice:CORRFW";
  if (fgUseLocalLibs) {
//     Info("","Loading AliRoot Libs ...");
//     gROOT->Macro(Form("%s/macros/loadlibs.C",gSystem->ExpandPathName("$ALICE_ROOT/")));
//     Info("","Loading AliRoot Libs is done...");
      fgPARS = "";
      fgLIBS = "STEERBase:ESD:AOD";
      fgLIBS += ":ANALYSIS:ANALYSISalice:CORRFW";
//       fgLIBS += ":PWG2resonances";
//       fgPARS += "ANALYSIS:ANALYSISalice:CORRFW";
    }
    

    
  // adds standard RESONANCE parfiles
//   fgPARS += ":ANALYSISTest";
//   fgPARS += ":PWG2resonances";
//   fgPARS += ":PWG2resonancesTest";

  // clean packages
//   fgPARSClean = "all";
//   fgPARSClean = "ANALYSISTest:PWG2resonances:PWG2resonancesMV";

  // input
  fgDataType = (Int_t) AliRsnUtils::kTxt;
  fgInputFileName = "ESD_vala_PDC08.txt";
  fgInputFileName = "test.txt";
  fgTreeName = "esdTree";

  fgInputFileName = "LHC09a4_AOD1_TEST.txt";
  fgInputFileName = "LHC09a4_AOD1_LOCAL.txt";
  fgTreeName = "aodTree";
  
  
//   fgDataType = (Int_t) AliRsnUtils::kTxt;
//   fgInputFileName = "RSNOUTTree.txt";
//   fgTreeName = "filesTree";
//  
  
//     fgInputFileName = "AOD_vala_PDC08.txt";
//     fgTreeName = "aodTree";
//     fgDoMixing = kTRUE;

//   Int_t tagCollNum = 160009;
//   fgDataType = AliRsnUtils::kXmlCollectionTag;
//   fgCollName = Form("/alice/cern.ch/user/m/mvala/DATA/ESD/collections/PDC07f/%d_tag.xml",tagCollNum);
//   fgInputFileName = Form("alien://%s",fgCollName.Data());

  // sets up macro
  fgMacro = macro;
  // number of events
  fgNumOfEvents = numberOfEvents;
  // number of events to skip
  fgNumOfEventsSkip = skipEvents;

  return runLocal();

}

Bool_t RunOnAliEn(TString macro, Long64_t numberOfEvents, Long64_t skipEvents, Int_t num = 0)
{

  TGrid::Connect("alien://");

  fgMode = "alien";

  fgDontRun = "";
  
  fgAlienShouldRun = kTRUE;
  fgAlienShoudlCopy = kTRUE;

//   fgUseLocalLibs = kTRUE;
//   fgAlirootLibPath = "$ALICE_ROOT/lib/tgt_$ALICE_TARGET";
  fgAlirootLibPath = "$ALIMV/$ALICE_TARGET/lib";

  // adds standard ANALYSIS parfiles
  fgPARS = "STEERBase:ESD:AOD:ANALYSIS:ANALYSISalice";
  if (fgUseLocalLibs) {
      //     Info("","Loading AliRoot Libs ...");
      //     gROOT->Macro(Form("%s/macros/loadlibs.C",gSystem->ExpandPathName("$ALICE_ROOT/")));
      //     Info("","Loading AliRoot Libs is done...");
      fgPARS = "";
      fgLIBS = "STEERBase:ESD:AOD";
      //     fgLIBS += ":ANALYSIS:ANALYSISalice";
      fgPARS += "ANALYSIS:ANALYSISalice:CORRFW";
    }
  // adds standard RESONANCE parfiles

  fgPARS += ":ANALYSISTest";
  fgPARS += ":PWG2resonances";
  fgPARS += ":PWG2resonancesTest";

  fgPARSClean = "";

//   TString productionName = "PDC07f";
  TString productionName = "LHC09a4";
  Int_t tagCollNum = 1;
  tagCollNum = 4000000;
  tagCollNum = 1;
  if (num > 0)
    tagCollNum = num;
  fgAlienSplit = 10;
  fgProjectDir = Form("/alice/cern.ch/user/m/mvala/RSNTASK/ANALYSIS/%s_1002/ALL", productionName.Data());
  //     fProjectDir = Form("/alice/cern.ch/user/m/mvala/RSNTASK/ANALYSIS/%s/%d",productionName.Data(),tagCollNum);
  fgOutputDir = Form("/alice/cern.ch/user/m/mvala/DATA/RSN/%s_1002/%d", productionName.Data(), tagCollNum);


//   fgDataType = AliRsnUtils::kXmlCollection;
//   fgCollName = Form("/alice/cern.ch/user/m/mvala/DATA/ESD/collections/%s/%d.xml",productionName.Data(),tagCollNum);
  fgDataType = AliRsnUtils::kXmlCollectionTag;
  fgCollName = Form("/alice/cern.ch/user/m/mvala/DATA/ESD/collections/%s/%d_tag.xml", productionName.Data(), tagCollNum);
  fgInputFileName = Form("alien://%s", fgCollName.Data());

  fgTreeName = "esdTree";


  // extra files
  fgExtraInputFiles = "PWG2resonancesUtils.C:AliRsnTrain.C:runRsnAnalysisSE.C:runRsnAnalysisME.C";
  fgExtraInputFiles += ":RsnConfigTest.C:ConfigureCuts.C";

  fgJDL = (TAlienJDL*) gGrid->GetJDLGenerator();

  // jdl part (you can use mini.jdl or set up by yourself
  // utils->GetJDL()->... [take look in TAlienJDL in $ROOTSYS/net/alien/])
//   utils->GetJDL()->Parse ( "mini.jdl" );
//   fgJDL->SetExecutable("root.sh");
  fgJDL->SetExecutable("aliroot.sh");
  fgJDL->AddToPackages("VO_ALICE@APISCONFIG::V2.4");
//   fgJDL->AddToPackages("VO_ALICE@alien::v2_16");
//   fgJDL->AddToPackages("VO_ALICE@ROOT::v5-21-01-alice");
  fgJDL->AddToPackages("VO_ALICE@AliRoot::v4-16-Rev-06");
  fgJDL->AddToOutputArchive("log_archive.zip:stdout,stderr@ALICE::CERN::SE");
  fgJDL->AddToOutputArchive("root_archive.zip:*.root@ALICE::CERN::SE");
//   fgJDL->SetOutputDirectory(Form("%s/",pname.Data()));
  fgJDL->SetOutputDirectory(Form("%s/#alien_counter_03i#", fgOutputDir.Data()));
  fgJDL->SetSplitMode("se", fgAlienSplit);
  fgJDL->SetInputDataList("wn.xml");
  fgJDL->SetInputDataListFormat("xml-single");
//     fgJDL->SetInputDataListFormat(Form("merge:%s",fCollName.Data()));

  fgJDL->AddToInputDataCollection(Form("LF:%s,nodownload", fgCollName.Data()));
//   fgJDL->AddToMerge("histOut.root:/alice/jdl/mergerootfile.jdl:histOut.Merged.root");
//   fgJDL->SetMergedOutputDirectory(Form("%s",pname.Data()));

// sets up macro
  fgMacro = macro;
// number of events
  fgNumOfEvents = numberOfEvents;
// number of events to skip
  fgNumOfEventsSkip = skipEvents;

  return runAlien();
}
