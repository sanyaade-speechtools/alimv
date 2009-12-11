void AddCustomAnalysisTask(AliLog::EType_t type=AliLog::kInfo)
{
  
  gROOT->ProcessLine(Form(".include %s/include",gSystem->ExpandPathName("/home/mvala/ALICE/alitrain")));

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  AliAnalysisTaskCustom *task = new AliAnalysisTaskCustom("AliAnalysisTaskCustom");

  AliAnalysisDataContainer *output1 = mgr->CreateContainer("cCustomList", TList::Class(), AliAnalysisManager::kOutputContainer, "BBOutput.root");

  mgr->AddTask(task);

  mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 1, output1);

}
