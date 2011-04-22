#ifndef __CINT__//|
#endif//|
Bool_t AddRsnTaskAll(TString format = "esd", Bool_t useMC = kFALSE, TString opts = "")
{

   // add our taks
   gROOT->LoadMacro("AddRsnAnalysisTask.C");
   AddRsnAnalysisTask(format, useMC, opts);

   return kTRUE;
}
