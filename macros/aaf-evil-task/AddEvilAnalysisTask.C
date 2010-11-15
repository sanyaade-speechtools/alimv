AliAnalysisTask* AddEvilAnalysisTask(TString format="esd",Bool_t useMC = kFALSE)
{
  // create manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) mgr = new AliAnalysisManager("My test train");
  
  // create our task
  AliAnalysisTaskEvil *task = new AliAnalysisTaskEvil("AliAnalysisTaskEvil");

  // uncomment this to use MC
  task->UseMC(useMC);
  
  task->SetHistRange(100,0,10);
  
  // add some more 1D histograms
//   task->SetNum1D(100);
  
  // add some more 2D histograms
//    task->SetNum2D(1000);
  
  // add some more 3D hostograms
//     task->SetNum3D(100);
  
  // Let's create memory leak (0 - no memory leak)
//   task->SetNumMemoryLeak(1); 

  // Set bad host (it will crash on that machine)
//   task->SetBadHost("prf005-iep-grid.saske.sk",1);
  
//   task->SetNumLoopsInOneEvent(10);
	
	// no putput is produced (only TList is there)
// 	task->NoOutput(kTRUE/*,kTRUE*/);
  
		
/*	gROOT->LoadMacro("VerifyAnalysisTask.C");
	if (VerifyAnalysisTask(task)) {
		delete task;
		return 0;
	}*/
		
  // create output container
  AliAnalysisDataContainer *output1 = mgr->CreateContainer("cCustomList", TList::Class(), AliAnalysisManager::kOutputContainer, "MyOutput.root");

  // add our task to the manager
  mgr->AddTask(task);

  // finaly connect input and output
  mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 1, output1);

	return task;
}

