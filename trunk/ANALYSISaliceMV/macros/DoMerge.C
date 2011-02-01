#ifndef __CINT__
#include <Rtypes.h>
#include <TString.h>
#include <TChain.h>
#include <TProof.h>
#include <TSelector.h>
#include "TFileMergeSelector.h"
#endif
void DoMerge(TString inFile = "/tmp/inFiles.root", Bool_t useProof = kFALSE, TString proof = "")
{
   TChain c("filesTree");
   c.AddFile(inFile.Data());

   if (useProof) {
      c.SetProof();
      TProof::Open(proof.Data());
      gProof->AddInput(new TNamed("PROOF_OUTPUTFILE", "root://localhost//tmp/"));
      gProof->AddInput(new TNamed("PROOF_USE_ALIEN", "YES"));
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
