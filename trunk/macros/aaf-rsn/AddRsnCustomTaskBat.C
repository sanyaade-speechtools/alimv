// #include <RESONANCESMV/AliRsnCustomTrackInfo.h> //|
// #include <RESONANCESMV/AliRsnCustomTask.h> //|
// #include <PWG2resonances/RESONANCES/AliRsnCutSet.h> //|
AliAnalysisTask* AddRsnCustomTaskBat(TString format = "esd", Bool_t useMC = kFALSE, TString opts = "")
{
   // create manager
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) mgr = new AliAnalysisManager("My test train");

   // create our task
   AliRsnCustomTask *task = new AliRsnCustomTask("AliRsnCustomTask");

   AliRsnCutSet *cutSet = 0;
   cutSet = new AliRsnCutSet("myCS", AliRsnTarget::kDaughter);

   // loads macro with RsnCutPIDOnly.C
   gROOT->LoadMacro("RsnCut2010PhiKK.C");
   // -- tracks --> PID
   AliRsnCutManager *cutMgr = new AliRsnCutManager("tmpCutManager");
   RsnCut2010PhiKK(cutMgr);

   Bool_t recalculateTOFPid = kTRUE;
//   recalculateTOFPid=kFALSE;

   AliRsnCustomTrackInfoBat *trackInfoPos = new AliRsnCustomTrackInfoBat("pos", "Positive Tracks");
   trackInfoPos->RecalculateTOFPid(recalculateTOFPid);
   trackInfoPos->SetParamaterHistogram(AliRsnCustomTrackInfo::kPt, 1000, 0.0, 10.0);
   trackInfoPos->SetCharge(1);
   if (cutMgr) trackInfoPos->SetCutSet(cutMgr->GetCommonDaughterCuts());
   task->AddCustomObject(trackInfoPos);

   AliRsnCustomTrackInfoBat *trackInfoNeg = new AliRsnCustomTrackInfoBat("neg", "Negative Tracks");
   trackInfoPos->RecalculateTOFPid(recalculateTOFPid);
   trackInfoNeg->SetParamaterHistogram(AliRsnCustomTrackInfo::kPt, 1000, 0.0, 10.0);
   trackInfoNeg->SetCharge(-1);
   if (cutMgr) trackInfoNeg->SetCutSet(cutMgr->GetCommonDaughterCuts());
   task->AddCustomObject(trackInfoNeg);


//   const char* outFileName = AliAnalysisManager::GetCommonFileName();
   const char* outFileName = "MyRsnCustomBat.root";
   // create output container
   AliAnalysisDataContainer *outputInfo = mgr->CreateContainer("cRsnInfoBat", TList::Class(), AliAnalysisManager::kOutputContainer, outFileName);
   AliAnalysisDataContainer *outputHist = mgr->CreateContainer("cRsnCustomListBat", TList::Class(), AliAnalysisManager::kOutputContainer, outFileName);
   // add our task to the manager
   mgr->AddTask(task);

   // finaly connect input and output
   mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
   // create containers for output

   mgr->ConnectOutput(task, 1, outputInfo);
   mgr->ConnectOutput(task, 2, outputHist);

   return task;
}

