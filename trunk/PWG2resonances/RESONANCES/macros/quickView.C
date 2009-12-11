void quickView(Bool_t mc=kFALSE)
{
  TString filename = "RSNAnalysis0.root";
  TString listname = "RsnPhi0";
  TString prefix = "PHI0";
  TString mainname = "IM_PT_ETA_MULT";
  TString pidname = "REALISTIC";
  
  TString particlesname = "K+K-";
  TString cutname = "default";
//   cutname = "true";
//   cutname = "notTrue";
  // 0 for IM, 1 for PT, ...
  Int_t projectionToShow = 0;
  
  
  if (mc){
//     filename.ReplaceAll(".root","_MC.root");
    listname += "_MC";
    prefix += "-MC";
    pidname = "PERFECT";
  }
  
  TString fullname = Form("%s_%s_%s_%s_%s",prefix.Data(),mainname.Data(),pidname.Data(),particlesname.Data(),cutname.Data());
  
  TFile *f  = TFile::Open(filename.Data());
  if (!f) {
    ::Error("quickView.C",Form("Error opening file '%s'",filename.Data()));
    return;
  }
  
  
  TList *l1 = (TList*)f->Get(listname.Data());

  THnSparseD  *s1 = (THnSparseD*)l1->FindObject(fullname.Data());
  
  if (!l1) {
    ::Error("quickView.C",Form("Error geting list '%s'",listname.Data()));
    return;
  }
  l1->Print();
  
  if (!s1) {
    ::Error("quickView.C",Form("Error geting THnSparseD '%s'",fullname.Data()));
    return;
  }
  // 0 for IM, 1 for PT, ...
  s1->Projection(projectionToShow)->Draw();
  

}

















