#include <TError.h>//|
#include <TROOT.h>//|
#include <TString.h>//|
void RunAnalysisManager(TString,TString,TString.TString);//|
//
Int_t runALICE(TString analysisSource="proof",TString analysisMode="test") { //|
//
//
  fNEvents = 1*1e9;
  fNEventsSkip = 0;
//
//   RunAnalysisManager("AddMyAnalysisManagerESDFilter",analysisSource.Data(),analysisMode.Data());
//
//   RunAnalysisManager("AddMyAnalysisManagerCustomTask",analysisSource.Data(),analysisMode.Data(),"");
  //
//   fNEvents = 1e5;
//   fNEventsSkip = 350;
  
  // mix from aliroot
//   RunAnalysisManager("AddAnalysisManagerMix",analysisSource.Data(),analysisMode.Data(),"");
  
  // mix with parfiles
//   RunAnalysisManager("AddAnalysisManagerMixPar",analysisSource.Data(),analysisMode.Data(),"");
  
  // rsn from aliroot
//   RunAnalysisManager("AddAnalysisManagerMixRsn",analysisSource.Data(),analysisMode.Data(),"");
  
  // rsn with parfiles
  RunAnalysisManager("AddAnalysisManagerMixRsnPar",analysisSource.Data(),analysisMode.Data(),"");
  
  // rsn with parfiles Bat
//     RunAnalysisManager("AddAnalysisManagerMixRsnParBat",analysisSource.Data(),analysisMode.Data(),"");
  
  // mix from aliroot
//   RunAnalysisManager("AddAnalysisManagerMixOld",analysisSource.Data(),analysisMode.Data(),"");
  
//   RunAnalysisManager("AddMyAnalysisManagerEvilTask",analysisSource.Data(),analysisMode.Data(),"");
  
//     RunAnalysisManager("AddMyAnalysisManagerRsnCustomTask",analysisSource.Data(),analysisMode.Data(),"");
  
// 	for (Int_t i=0;i<2;i++)
//     RunAnalysisManager("AddMyAnalysisManagerEvilTask",analysisSource.Data(),analysisMode.Data(),Form("%d",i));
//
//   for (Int_t i=0;i<1;i++) {
//     if (gROOT->IsInterrupted()) break;
//     RunAnalysisManager("AddMyAnalysisManagerAodAnalysisTask",analysisSource.Data(),analysisMode.Data(),Form("%d",i));
//   }
//
} //