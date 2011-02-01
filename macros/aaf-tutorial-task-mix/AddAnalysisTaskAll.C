Bool_t AddAnalysisTaskAll(TString format = "esd", Bool_t useMC = kFALSE, TString opts = "")
{

   // //     // adds mixing info task
   gROOT->LoadMacro("AddAnalysisTaskMixInfo.C");
   AddAnalysisTaskMixInfo(format, useMC, opts);
// //     // add our taks
   gROOT->LoadMacro("AddAnalysisTaskCustomMix.C");
   AddAnalysisTaskCustomMix(format, useMC, opts);

   return kTRUE;
}
