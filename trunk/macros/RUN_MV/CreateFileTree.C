#ifndef __CINT__
#include <TFile.h>
#include <TTree.h>
#include "../../PWG2resonancesTest/RESONANCES/AliRsnFileInfo.h"
#endif
void CreateFileTree(TString fileTxt = "RSN.txt")
{

//   gROOT->LoadMacro("PWG2resonancesUtils.C");
  AliRsnUtils utils(AliRsnUtils::kLocal);

  utils.LoadPars("STEERBase:ESD:AOD:ANALYSIS:ANALYSISalice:PWG2resonances:PWG2resonancesTest");

  ifstream fileIn;
  fileIn.open(fileTxt.Data());

  Int_t count = 0;

  fileTxt.ReplaceAll("txt","root");
  TFile f(Form("RSNTree_%s",fileTxt.Data()), "RECREATE");

  TTree *tree = new TTree("filesTree", "Train File Tree");
  AliRsnFileInfo *fileInfo = new AliRsnFileInfo();
  
  tree->Branch("trainFiles", "AliRsnFileInfo", &fileInfo);

  TString file;
  while (fileIn.good()) {
    fileIn >> file;
    if (file.IsNull()) continue;
    fileInfo->SetFileName(file);
    tree->Fill();
    fileInfo->Clear();
  }
  fileIn.close();
  f.Write();
  tree->Print();
}
