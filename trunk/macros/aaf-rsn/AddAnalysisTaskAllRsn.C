Bool_t AddAnalysisTaskAllRsn(TString format = "esd", Bool_t useMC = kFALSE, TString opts = "")
{

   // //     // adds mixing info task
   gROOT->LoadMacro("AddAnalysisTaskMixInfo.C");
   AddAnalysisTaskMixInfo(format, useMC, opts);

   // add our taks
   gROOT->LoadMacro("AddRsnTask.C");
   opts = "Phi";
   AddRsnTask(format, useMC, "ConfigRsnPhiKK.C", opts);
   AddRsnTask(format, useMC, "ConfigRsnPhiKK.C", Form("%s_mix", opts.Data()));
   opts = "Kstar";
   AddRsnTask(format, useMC, "ConfigRsnKStarKPi.C", opts);
   AddRsnTask(format, useMC, "ConfigRsnKStarKPi.C", Form("%s_mix", opts.Data()));

   return kTRUE;
}
