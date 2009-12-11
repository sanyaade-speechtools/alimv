//
// Macro to create the full analysis manager for Resonances
//

Bool_t AddAnalysisTaskRsnME(const char *outFile = "rsn.root", Bool_t useKine = kTRUE)
{
  // Retrieve analysis manager.
  // Since it is usually created in the steering macro,
  // then we don't ever need to initialize a new one
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  /*
  // create handlers for input and MonteCarlo
  AliMCEventHandler  *mcHandler  = new AliMCEventHandler();
  AliESDInputHandler *esdHandler = new AliESDInputHandler();
  //AliAODHandler      *aodHandler = new AliAODHandler();
  //aodHandler->SetOutputFileName("AliAOD.root");
  if (useKine) mgr->SetMCtruthEventHandler(mcHandler);
  mgr->SetInputEventHandler(esdHandler);
  //mgr->SetOutputEventHandler(aodHandler);
  */

  // initialize task
  AliRsnAnalysisME *task = new AliRsnAnalysisME("AliRsnAnalysisSE");

  // set prior probabilities for PID
  task->SetPriorProbability(AliPID::kElectron, 0.02);
  task->SetPriorProbability(AliPID::kMuon,     0.02);
  task->SetPriorProbability(AliPID::kPion,     0.83);
  task->SetPriorProbability(AliPID::kKaon,     0.07);
  task->SetPriorProbability(AliPID::kProton,   0.06);
  task->DumpPriors();

  // initialize analysis manager with pairs from config
  gROOT->LoadMacro("CreatePairsPhi.C");
  AliRsnAnalysisManager *anaMgr = task->GetAnalysisManager("MyAnalysisSE");
  anaMgr->Add(CreatePairsPhi("PHI"));

  // add the task to manager
  mgr->AddTask(task);

  // initialize container for the output
  // (input container is common)
  AliAnalysisDataContainer *in = mgr->CreateContainer("cchain", TChain::Class(), AliAnalysisManager::kInputContainer);
  AliAnalysisDataContainer *exc = mgr->CreateContainer("tree", TTree::Class(), AliAnalysisManager::kExchangeContainer, "default");
  AliAnalysisDataContainer *out = mgr->CreateContainer("RSN", TList::Class(), AliAnalysisManager::kOutputContainer, outFile);

  // connect input and output slots for this task
  mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 0, exc);
  mgr->ConnectOutput(task, 1, out);
}
