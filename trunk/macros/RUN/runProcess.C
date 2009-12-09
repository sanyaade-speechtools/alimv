#ifndef __CINT__
#include <TString.h>
#include <TStopwatch.h>
#include <TROOT.h>
#include <TError.h>
#endif
void runProcess(Long64_t numOfEvents = 1e4, Long64_t numOfEventsSkip = 0 * 1e5)
{
  gEnv->SetValue("XSec.GSI.DelegProxy", "2");
  gEnv->SetValue("Proof.SocketActivityTimeout", 0);

  TTreeCacheUnzip::SetParallelUnzip(TTreeCacheUnzip::kEnable);
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");
  gSystem->Load("libCORRFW");
  gSystem->Load("libPWG2resonances");
  gSystem->Load("libPWG2spectra");
  gSystem->Load("libPWG2AOD");
  gSystem->Load("libPWG2femtoscopy");
  gSystem->Load("libPWG2femtoscopyUser");

//   gEnv->SetValue("Proof.StatsHist",1);
//   gEnv->SetValue("Proof.StatsTrace",1);
//   gEnv->SetValue("Proof.SlaveStatsTrace",1);


  TStopwatch timer;
  timer.Start();

  Bool_t boolProcess = kTRUE;

  gROOT->LoadMacro("PWG2resonancesUtils.C");
  gROOT->LoadMacro("ProcessConfig.C");

  gROOT->LoadMacro("GenerateTrainMacro.C");
  GenerateTrainMacro("AliTrainMV.C");

  TString macro;
  macro = "AliRsnTrain.C";
//   macro = "AliRsnTrainOfficial.C";

//   boolProcess = RunLocaly(macro, numOfEvents, numOfEventsSkip);
  boolProcess = RunOnProof(macro, numOfEvents, numOfEventsSkip);
//   boolProcess = RunOnAliEn(macro,numOfEvents,numOfEventsSkip);


  timer.Stop();
  timer.Print();

  Info("runProcess.C", "Process ended %s.", ((boolProcess) ? "OK" : "with an ERROR"));

}

