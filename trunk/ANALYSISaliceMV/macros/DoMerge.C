#ifndef __CINT__
#include <Rtypes.h>
#include <TString.h>
#include <TChain.h>
#include <TProof.h>
#include <TSelector.h>
#include "TFileMergeSelector.h"
#endif
void DoMerge(TString inFile = "alien:///alice/cern.ch/user/m/mvala/file_collection.root", Bool_t useProof = kFALSE, TString proof = "")
{
   if (inFile.Contains("alien://")) TGrid::Connect("alien://");

   TChain c("filesTree");
   c.AddFile(inFile.Data());
   Long64_t numEntries = c.GetEntries();

   Printf("Num entries : %lld", numEntries);
   if (!numEntries) return;

   if (useProof) {
      c.SetProof();
      TProof::Open(proof.Data());

      if (inFile.Contains("alien://")) gProof->Exec("TGrid::Connect(\"alien:\/\/\");");

//       gProof->AddInput(new TNamed("PROOF_OUTPUTFILE", "root://vala.jinr.ru//tmp/"));
//          gProof->AddInput(new TNamed("PROOF_OUTPUTFILE", "root://alicepc104.jinr.ru//tmp/"));
      gProof->AddInput(new TNamed("PROOF_OUTPUTFILE", "root://prf000-iep-grid.saske.sk:11094//tmp/"));
//       gProof->AddInput(new TNamed("PROOF_USE_ALIEN", "YES"));
      gProof->AddInput(new TNamed("PROOF_USE_ARCHIVE", "MixInfo.root,AnalysisResults.root"));


      gProof->SetParameter("PROOF_PacketizerStrategy", (Int_t)0);
      gProof->SetParameter("PROOF_PacketAsAFraction", 50);
   }

//   gROOT->ProcessLine(".L TFileMergeSelector.cxx++g");
//   TFileMergeSelector* selector = (TFileMergeSelector*) TSelector::GetSelector("TFileMergeSelector");
//    selector->DoMerge();
//    selector->SetFilesToMerge("");
   c.Process("TFileMergeSelector.cxx++g");

}
