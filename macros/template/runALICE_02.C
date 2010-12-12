#include <TError.h>//
#include <TROOT.h>//
#include <TString.h>//
void RunAnalysisManager(TString,TString,TString.TString);//
//
Int_t runALICE(TString analysisSource="proof",TString analysisMode="test") { //
//
//
  fNEvents = 1*1e9;
  fNEventsSkip = 0;
//
//   RunAnalysisManager("AddMyAnalysisManagerESDFilter",analysisSource.Data(),analysisMode.Data());
//
//   RunAnalysisManager("AddMyAnalysisManagerCustomTask",analysisSource.Data(),analysisMode.Data(),"");
  //
//   fNEvents = 1e6;
//   fNEventsSkip = 350;
//   RunAnalysisManager("AddMyAnalysisManagerCustomMixTask",analysisSource.Data(),analysisMode.Data(),"");
  RunAnalysisManager("AddMyAnalysisManagerCustomMixTaskPar",analysisSource.Data(),analysisMode.Data(),"");
//   
//   RunAnalysisManager("AddMyAnalysisManagerEvilTask",analysisSource.Data(),analysisMode.Data(),"");
  
// 	for (Int_t i=0;i<2;i++)
//     RunAnalysisManager("AddMyAnalysisManagerEvilTask",analysisSource.Data(),analysisMode.Data(),Form("%d",i));
//
//   for (Int_t i=0;i<1;i++) {
//     if (gROOT->IsInterrupted()) break;
//     RunAnalysisManager("AddMyAnalysisManagerAodAnalysisTask",analysisSource.Data(),analysisMode.Data(),Form("%d",i));
//   }
//
} //