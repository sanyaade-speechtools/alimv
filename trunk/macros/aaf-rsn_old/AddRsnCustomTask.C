// #include <RESONANCESMV/AliRsnCustomTrackInfo.h> //|
// #include <RESONANCESMV/AliRsnCustomTask.h> //|
// #include <PWG2resonances/RESONANCES/AliRsnCutSet.h> //|
AliAnalysisTask* AddRsnCustomTask(TString format = "esd", Bool_t useMC = kFALSE, TString opts = "")
{
   // create manager
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) mgr = new AliAnalysisManager("My test train");

   // create our task
   AliRsnCustomTask *task = new AliRsnCustomTask("AliRsnCustomTask");

   AliRsnCutSet *cutSet = 0;
   cutSet = new AliRsnCutSet("myCS", AliRsnTarget::kDaughter);

   // loads macro with RsnCutPIDOnly.C
   gROOT->LoadMacro("RsnCurrentCuts.C");
   // -- tracks --> PID
   AliRsnCutManager *cutMgr = new AliRsnCutManager("tmpCutManager");
   RsnCurrentCuts(cutMgr);
//   RsnCurrentCuts(cutMgr);
//   RsnCurrentCuts(cutMgr);

   AliRsnCustomTrackInfo *trackInfoPos = new AliRsnCustomTrackInfo("pos", "Positive Tracks");
   trackInfoPos->SetParamaterHistogram(AliRsnCustomTrackInfo::kPt, 1000, 0.0, 10.0);
   trackInfoPos->SetCharge(1);
   if (cutMgr) trackInfoPos->SetCutSet(cutMgr->GetCommonDaughterCuts());
   task->AddCustomObject(trackInfoPos);

   AliRsnCustomTrackInfo *trackInfoNeg = new AliRsnCustomTrackInfo("neg", "Negative Tracks");
   trackInfoNeg->SetParamaterHistogram(AliRsnCustomTrackInfo::kPt, 1000, 0.0, 10.0);
   trackInfoNeg->SetCharge(-1);
   if (cutMgr) trackInfoNeg->SetCutSet(cutMgr->GetCommonDaughterCuts());
   task->AddCustomObject(trackInfoNeg);


//   const char* outFileName = AliAnalysisManager::GetCommonFileName();
   const char* outFileName = "MyRsnCustom.root";
   // create output container
   AliAnalysisDataContainer *outputInfo = mgr->CreateContainer("cRsnInfo", TList::Class(), AliAnalysisManager::kOutputContainer, outFileName);
   AliAnalysisDataContainer *outputHist = mgr->CreateContainer("cRsnCustomList", TList::Class(), AliAnalysisManager::kOutputContainer, outFileName);
   // add our task to the manager
   mgr->AddTask(task);

   // finaly connect input and output
   mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
   // create containers for output

   mgr->ConnectOutput(task, 1, outputInfo);
   mgr->ConnectOutput(task, 2, outputHist);

   return task;
}

