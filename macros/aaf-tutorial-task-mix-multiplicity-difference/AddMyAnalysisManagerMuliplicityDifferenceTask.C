#include <TString.h>
#include <TROOT.h>
#include <AliAnalysisManager.h>
#include <AliAnalysisAlien.h>
#include <TProof.h>
#include <AliESDInputHandler.h>

void AddMyAnalysisManagerMuliplicityDifferenceTask(TString analysisSource = "proof", TString analysisMode = "test", TString opts = "") {

  Bool_t useMC = kFALSE;
  TString format = "esd";

  // ALICE stuff
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) mgr = new AliAnalysisManager("Martin Vala's AM");

  TString anaOption = "proof";
  if (!anaOption.CompareTo("plugin")) {
    gROOT->LoadMacro("SetupAnalysisPlugin.C");
    AliAnalysisGrid *analysisPlugin = SetupAnalysisPlugin(analysisMode.Data());
    if (!analysisPlugin) return;
//   analysisPlugin->SetAnalysisSource("AliAnalysisTaskCustom.cxx");
//   analysisPlugin->SetAdditionalLibs("AliAnalysisTaskCustom.h AliAnalysisTaskCustom.cxx");
    analysisPlugin->SetAdditionalLibs("MIXMV.par");
    mgr->SetGridHandler(analysisPlugin);
    AliAnalysisAlien::SetupPar("MIXMV.par");
    
  } else if (!anaOption.CompareTo("proof")) {
//     TProof::Open("skaf.saske.sk");
    TProof::Open("alice-caf");
    gProof->ClearPackages();
    //   gProof->SetParallel(1);
    
//     gProof->UploadPackage("VO_ALICE@AliRoot::v4-20-13-AN");
    gProof->EnablePackage("VO_ALICE@AliRoot::v4-20-13-AN");
    
    gProof->UploadPackage("MIXMV");
    gProof->EnablePackage("MIXMV");
  } else {
    TProof::Open("");
    gProof->ClearPackages();
//   gProof->SetParallel(1);

    Int_t numWorkers = gProof->GetParallel();
    gProof->SetParameter("PROOF_PacketizerStrategy", (Int_t)0);
    gProof->SetParameter("PROOF_PacketAsAFraction", (Int_t) 1500 / numWorkers);

    gProof->UploadPackage("AliRootProofLite");
    gProof->EnablePackage("AliRootProofLite");

    gProof->UploadPackage("MIXMV");
    gProof->EnablePackage("MIXMV");
  }
  if (!InputHandlerSetup(format, useMC)) return;

  AliESDInputHandler *esdH = dynamic_cast<AliESDInputHandler*>(mgr->GetInputEventHandler());
  esdH->SetMixingHandler(new AliESDInputHandler);
//   AliAnalysisAlien::SetupPar("MIXMV");
//   gROOT->LoadMacro("AliAnalysisTaskCustom.cxx+g");

  // load and run AddTask macro
  gROOT->LoadMacro("AddAnalysisTaskMuliplicityDifference.C");
  AddAnalysisTaskMuliplicityDifference(format, useMC, opts);

}
