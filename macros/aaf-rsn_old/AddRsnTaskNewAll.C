#ifndef __CINT__//|
#endif//|
Bool_t AddRsnTaskNewAll(TString format = "esd", Bool_t useMC = kFALSE, TString opts = "")
{

   // add our taks
   gROOT->LoadMacro("AddRsnAnalysisTaskNew.C");
   AddRsnAnalysisTaskNew(format, useMC, opts);

   return kTRUE;
}
