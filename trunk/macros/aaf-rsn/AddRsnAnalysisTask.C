#ifndef __CINT__ //|
#include <AliAnalysisManager.h>//|
#include <TROOT.h>//|
#endif//|
AliAnalysisTask* AddRsnAnalysisTask(TString format = "esd", Bool_t useMC = kFALSE, TString opts = "")
{

   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) mgr = new AliAnalysisManager("RSN train");

   // initialize task with all available slots, even if not all of them will be used:
   AliRsnAnalysisTask *task = new AliRsnAnalysisTask("RSN_MV");

   gROOT->LoadMacro("RsnConfigTest.C");
   RsnConfigTest(task,useMC);

   
//    gROOT->LoadMacro("RsnConfigTest.C");
//    RsnConfigTest(task,useMC);
   
   // add the task to manager
   mgr->AddTask(task);

   // connect input container according to source choice
   mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());

   // create paths for the output in the common file
   TString commonPath = AliAnalysisManager::GetCommonFileName();

   // create containers for output
   AliAnalysisDataContainer *output = mgr->CreateContainer(Form("RsnHist%s", opts.Data()), TList::Class(), AliAnalysisManager::kOutputContainer, commonPath.Data());
   mgr->ConnectOutput(task, 1, output);

   return task;
}
