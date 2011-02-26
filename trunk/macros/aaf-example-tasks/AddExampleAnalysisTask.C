AliAnalysisTask* AddExampleAnalysisTask(TString format = "esd", Bool_t useMC = kFALSE)
{
   // create manager
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) mgr = new AliAnalysisManager("My test train");

   // create our task
   AliAnalysisTaskEx02 *task = new AliAnalysisTaskEx02("AliAnalysisTaskEx02");
   task->SelectCollisionCandidates(AliVEvent::kAny);
//    task->SelectCollisionCandidates(AliVEvent::kMB);

   // create output container
   AliAnalysisDataContainer *output1 = mgr->CreateContainer("cEx2", TList::Class(), AliAnalysisManager::kOutputContainer, "MyOutput.root");

   // add our task to the manager
   mgr->AddTask(task);

   // finaly connect input and output
   mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
   mgr->ConnectOutput(task, 1, output1);

   return task;
}

