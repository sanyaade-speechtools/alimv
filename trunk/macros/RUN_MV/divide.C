void divide(TString filename = "RSNOutTest.root") {
  TFile *f = TFile::Open(filename.Data());
  
  TString strOne("ESD_REAL2");
  TString strTwo("ESD_REAL");
    
  TList *l1 = f->Get("RsnPostAnalysis");
  TList *l11 = (TList*) l1->FindObject(strOne.Data());
  TList *l12 = (TList*) l1->FindObject(strTwo.Data());
  
  TH1D *h1 = (TH1D*) l11->FindObject(Form("%s_param",strOne.Data()));
  TH1D *h2 = (TH1D*) l12->FindObject(Form("%s_param",strTwo.Data()));
  
//   h1->Draw();
//   h2->Draw("same");
  
  h1->Divide(h2,h1,1,1,"b");
//   h1->GetXaxis()->SetRangeUser(0.0,6.0);
  h1->Draw();
}
