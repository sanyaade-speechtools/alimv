#include "TChain.h"
#include "TTree.h"
#include "TList.h"
#include "TSystem.h"
#include "TFile.h"
#include "Riostream.h"

#include "AliLog.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAnalysisDataContainer.h"

#include "AliRsnOutputTask.h"
#include <THnSparse.h>


ClassImp(AliRsnOutputTask)

//________________________________________________________________________
AliRsnOutputTask::AliRsnOutputTask(const char *name, Bool_t useListInput)
    : AliAnalysisTask(name, ""),
    fUseListInput(useListInput),
    fFileInfo(0),
    fInputList(0),
    fOutputList(0),
    fProcessInfoHist(0),
    fTrainInfoFileName("AliProofAnalysisTrainInfo.root"),
    fRsnInfoFileName("RSNAnalysisInfo.root"),
    fRsnAnalysisFileName("RSNAnalysis0.root")
{
  // Constructor
  if (fUseListInput)
    DefineInput(0, TList::Class());
  else
    DefineInput(0, TChain::Class());

  DefineOutput(0, TList::Class());
}


void AliRsnOutputTask::ImportTxt(TString anaFile,TString txtIn,TString rootOut)
{
  TFile *file = TFile::Open(rootOut.Data(), "RECREATE");
  TTree *tree = new TTree("filesTree", "RSN File Tree");
  AliRsnFileInfo *fileInfo = new AliRsnFileInfo();
  tree->Branch("trainFiles", "AliRsnFileInfo", &fileInfo);

  if (txtIn.IsNull()) {
    fileInfo->SetFileName(Form(Form("%s/%s", gSystem->pwd(),anaFile.Data())));
    tree->Fill();
  }
  else {
    
    TObjArray* array = txtIn.Tokenize(",");
    TObjString *str;
    TString strr;
    for (Int_t i=0;i< array->GetEntriesFast();i++) {
      str = (TObjString *) array->At(i);
      strr = str->GetString().Data();
//       AliInfo(Form("adding file %s",strr.Data()));
      ifstream fileIn;
      fileIn.open(strr.Data());
      TString lineTxt;
      while (fileIn.good()) {
        fileIn >> lineTxt;
        if (lineTxt.IsNull()) continue;
        AliInfo(lineTxt.Data());
        fileInfo->SetFileName(lineTxt.Data());
        tree->Fill();
      }
      fileIn.close();
    }

  }

  file->Write();
  delete file;

}

//________________________________________________________________________
void AliRsnOutputTask::ConnectInputData(Option_t *)
{
  // Connect ESD or AOD here
  // Called once

  if (!fUseListInput) {

    TTree* tree = dynamic_cast<TTree*>(GetInputData(0));
    if (!tree) {
      AliWarning("Maybe Tree");
      AliError("Could not read chain from input slot 0");
    }
    else {
      // One should first check if the branch address was taken by some other task
      char ** address = (char **)GetBranchAddress(0, "trainFiles");
      if (address) {
        fFileInfo = (AliRsnFileInfo*)(*address);
      }
      else {
        fFileInfo = new AliRsnFileInfo();
        SetBranchAddress(0, "trainFiles", &fFileInfo);
      }
//       AliInfo(Form("Processing file %d ...", tree->GetReadEntry()));
    }

    tree->Print();
  }
  else {
    fInputList = dynamic_cast<TList*>(GetInputData(0));
    if (!fInputList) {
      AliWarning("Maybe List");
      AliError("Could not read List from input slot 0");
    }
  }
}

//________________________________________________________________________
void AliRsnOutputTask::CreateOutputObjects()
{
  // Create histograms
  // Called once

  fOutputList = new TList();
  fProcessInfoHist = new TH1I("hProcessInfo", "Process Info", 2, 0, 2);
  fOutputList->Add(fProcessInfoHist);

  fOutputManager.InitAll(fOutputList);

  //   fOutputManager.Print();

}

//________________________________________________________________________
void AliRsnOutputTask::Exec(Option_t *)
{
// Main loop
// Called for each event

  AliInfo(Form("Processing file %s ...", fFileInfo->GetFileName().Data()));
//   TTree *tree = dynamic_cast<TTree*>(GetInputData(0));
//   AliInfo(Form("Processing file %d ...", tree->GetReadEntry()));

  AliInfo(Form("Processed %d events ...", (Long64_t)fProcessInfoHist->Integral()));

  DoProcessInfo();
//       AliInfo(Form("Opening %s ... ", fFileInfo->GetFileName().Data()));
  if (!fUseListInput) {
    TString processInfoString(fFileInfo->GetFileName());
    processInfoString.ReplaceAll(fTrainInfoFileName.Data(), fRsnAnalysisFileName.Data());

    AliInfo(Form("Opening %s ... ", processInfoString.Data()));
    TFile *file = TFile::Open(processInfoString.Data(), "READ");
    if (file) {
//       AliInfo(Form("Processing  %s ... ", fFileInfo->GetFileName().Data()));
      fOutputManager.ProcessAll(file);
//       AliInfo(Form("Processing %s done ... ", fFileInfo->GetFileName().Data()));
    }
    else
      AliError(Form("Error opening %s !!!", fFileInfo->GetFileName().Data()));
    delete file;
//     AliInfo(Form("Opening %s done ... ", fFileInfo->GetFileName().Data()));
  }
  else {
    if (fInputList)
      fOutputManager.ProcessAll(0, fInputList);
  }
//   AliInfo(Form("Processed %d events ...", (Long64_t)fProcessInfoHist->Integral()));

  // Post output data.
  PostData(0, fOutputList);
}

//________________________________________________________________________
void AliRsnOutputTask::Terminate(Option_t *)
{
  
// 

  fOutputList = dynamic_cast<TList*>(GetOutputData(0));
  if (!fOutputList) return;
  TH1I *fProcessInfoHist = (TH1I*) fOutputList->FindObject("hProcessInfo");
  if (!fProcessInfoHist) {
    AliError(Form("fProcessInfoHist is %p", fProcessInfoHist));
//     return;
  } else {

  AliInfo(Form("=== %s ==================", GetName()));
  AliInfo(Form("Number Of Events Processed : %10d", (Long64_t)fProcessInfoHist->Integral()));
  AliInfo(Form("Number Of Events Accepted  : %10d", (Long64_t)fProcessInfoHist->GetBinContent(2)));
  AliInfo(Form("Number Of Events Skipped   : %10d", (Long64_t)fProcessInfoHist->GetBinContent(1)));
  AliInfo(Form("=== end %s ==============", GetName()));
  }

  AliInfo("Doing Post Analysis ...");
  DoSumSubDiv();
  AliInfo("Doing Post Analysis done...");

}


void AliRsnOutputTask::DoProcessInfo()
{
  TList *listProcessInfo = 0;
  if (fUseListInput) {
    listProcessInfo = (TList*) fInputList->FindObject("Info");
    if (!listProcessInfo) return;
  }
  else {
    TString processInfoString(fFileInfo->GetFileName());
    processInfoString.ReplaceAll(fTrainInfoFileName.Data(), fRsnInfoFileName.Data());
    AliInfo(Form("Opening %s ... ", processInfoString.Data()));
    TFile *file = TFile::Open(processInfoString.Data(), "READ");
    if (!file) return;
    listProcessInfo = (TList*) file->Get("RsnInfo");
    if (!listProcessInfo) return;
    AliInfo(Form("Opening %s done ... ", processInfoString.Data()))
  }

  TH1I* histTmp = (TH1I*)listProcessInfo->At(0);
  fProcessInfoHist->AddBinContent(1, histTmp->GetBinContent(1));
  fProcessInfoHist->AddBinContent(2, histTmp->GetBinContent(2));

}


void AliRsnOutputTask::DoSumSubDiv()
{
//     fOutputList->Print();
//   TFile *f = TFile::Open("RSNOutTest2.root","RECREATE");

  fOutputList = dynamic_cast<TList*>(GetOutputData(0));
  if (!fOutputList) return;

  fOutputManager.InitAllExtra(fOutputList);

  fOutputManager.ProcessAllExtra(fOutputList);
//   fOutputList->Write();
//   f->Close();
  
//   fOutputList->Print();
}

