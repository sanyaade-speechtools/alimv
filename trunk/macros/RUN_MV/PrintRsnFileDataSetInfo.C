#ifndef __CINT__
#include <TEnv.h>
#include <TProof.h>
#include <TString.h>
#include <TError.h>
#include <TCollection.h>
#include <TFile.h>
#include <TFileInfo.h>
#include <TKey.h>
#include <PWG2resonancesTest/RESONANCES/AliRsnFileInfo.h>
#endif

TString fOrigFile;
TString fMyFile;
Bool_t PrintRsnFileDataSetInfo(TString proofName = "skaf.saske.sk") {
  TString input[] = {"/COMMON/COMMON/LHC09a4_ESD_14_test2", "RSNAnalysisInfo.root", "RSNAnalysis0.root"};
//   TString input[] = {"/PWG2/mvala/LHC09a4_AOD_3_out", "AliTrainInfo.root","RSNAnalysis0.root"};
  DoPrintRsnFileDataSetInfo(input[0],input[1],input[2],proofName);
  
}
Bool_t DoPrintRsnFileDataSetInfo(TString dataSetName,const char*origFile="RSNAnalysisInfo.root",const char*myfile="RSNAnalysis0.root", TString proofName = "skaf.saske.sk") {
  fOrigFile = origFile;
  fMyFile = myfile;
  gEnv->SetValue("XSec.GSI.DelegProxy", "2");

  TProof::Open(Form("%s", proofName.Data()));
  if (!gProof) {
    Error("PrintRsnFileDataSetInfo.C", Form("No connection to %s !!!", proofName.Data()));
    return kFALSE;
  }
  gProof->ShowDataSets();
  TFileCollection *coll = gProof->GetDataSet(dataSetName.Data());
  if (!coll) {
    Error("PrintRsnFileDataSetInfo.C", "coll is null");
    return kFALSE;
  }

//   coll->Print("F");
  TIter iter(coll->GetList());
  TFileInfo *fileInfo = 0;
  TUrl *u = 0;
  TString lastFileName;
  Bool_t b=kTRUE;
  while ((fileInfo = dynamic_cast<TFileInfo*>(iter.Next()))) {
    fileInfo->ResetUrl();
    u = (TUrl*) fileInfo->NextUrl();
    u = (TUrl*) fileInfo->NextUrl();
    if (u) {
      Printf("%s", u->GetUrl());
      lastFileName = u->GetUrl();
      b = ListFilesFromTreeFile(u->GetUrl());
      if (!b) {
        Warning("PrintRsnFileDataSetInfo.C","b=kFALSE");
      }
    }
  }
  return kTRUE;
}

Bool_t ListFilesFromTreeFile(const char* filename, const char* treeName = "filesTree") {

  Bool_t b=kTRUE;
  TFile *f = TFile::Open(filename);
  if (!f) {
    Error("PrintRsnFileDataSetInfo.C::ListFilesFromTreeFile",Form("File %s was not opened ",filename));
    return kFALSE;
  }
  TTree *tree = (TTree*) f->Get(treeName);
  if (!tree) {
    Error("PrintRsnFileDataSetInfo.C::ListFilesFromTreeFile",Form("Tree %s was not found ",treeName));
    return kFALSE;
  }
//   tree->Print();

  AliRsnFileInfo *fileInfo = 0;
  tree->SetBranchAddress("trainFiles", &fileInfo);
  Int_t i;
  TString lastFileName;
  for (i = 0;i < tree->GetEntries();i++) {
    tree->GetEntry(i);
    fileInfo->Print("  ");
    lastFileName =fileInfo->GetFileName();
  }
  b = PrintContentOfFile(lastFileName);
  return b;
}

Bool_t PrintContentOfFile(TString filenameStr) {

  filenameStr.ReplaceAll(fOrigFile,fMyFile);
  const char *filename = filenameStr.Data();
  TFile *f = TFile::Open(filename);
  if (!f) {
    Error("PrintRsnFileDataSetInfo.C::PrintContentOfFile",Form("File %s was not opened ",filename));
    return kFALSE;
  }

  Printf("\n");
  Printf("Content of %s",filename);
  TList *listOfKeys = f->GetListOfKeys();
  TIter next(listOfKeys);
  TKey *key;
  while ((key = (TKey*) next())) {
    PrintRecursivlyContentOfKey(key,f);
  }
  
}

Bool_t PrintRecursivlyContentOfKey(TKey *key,TFile *f) {
  Bool_t b=kTRUE;
//   Printf("%s %s",key->GetName(),key->GetClassName());
  TString classname(key->GetClassName());
  if (!classname.CompareTo("TList")) {
    TList *l = (TList*) f->Get(key->GetName());
    l->Print();
  }
  return b;
}
