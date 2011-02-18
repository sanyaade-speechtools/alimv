Bool_t AddAnalysisTaskAllRsnBat(TString format = "esd", Bool_t useMC = kFALSE, TString opts = "")
{

   // add our taks
//    gROOT->LoadMacro("AddRsnTask.C");
//    opts = "Phi";
//    AddRsnTask(format, useMC, "ConfigRsnPhiKKHistsOnly.C", opts);

   gROOT->LoadMacro("AddRsnCustomTaskBat.C");
   AddRsnCustomTaskBat(format, useMC, opts);
   return kTRUE;
}
