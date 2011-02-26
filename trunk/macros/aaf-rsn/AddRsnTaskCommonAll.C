#ifndef __CINT__//|
#endif//|
Bool_t AddRsnTaskCommonAll(TString format = "esd", Bool_t useMC = kFALSE, TString opts = "")
{

   // add our taks
   gROOT->LoadMacro("AddRsnCommonTask.C");
   AddRsnCommonTask(format, useMC, opts);

   return kTRUE;
}
