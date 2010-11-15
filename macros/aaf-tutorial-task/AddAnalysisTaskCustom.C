void AddAnalysisTaskCustom(TString format="esd",Bool_t useMC = kFALSE,TString opts="")
{
  // create manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) return;

	// create our task
  AliAnalysisTaskCustom *task = new AliAnalysisTaskCustom("AliAnalysisTaskCustom");

  // uncomment this to use MC
  task->UseMC(useMC);
  
  // create output container
  AliAnalysisDataContainer *output1 = mgr->CreateContainer("cCustomList", TList::Class(), AliAnalysisManager::kOutputContainer, Form("MyOutput%s.root",opts.Data()));
//   output1->SetSpecialOutput();
//   mgr->SetSpecialOutputLocation("root://localhost//dev/shm/alice/xrd/data/");
  
  // add our task to the manager
  mgr->AddTask(task);

  // finaly connect input and output
  mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 1, output1);

}

