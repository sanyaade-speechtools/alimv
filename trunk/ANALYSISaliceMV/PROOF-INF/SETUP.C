Int_t SETUP() {

  gSystem->SetDynamicPath(Form(".:%s", gSystem->GetDynamicPath()));

  // Set the include paths
  gROOT->ProcessLine(Form(".include %s",gSystem->pwd()));

  // Set our location, so that other packages can find us
  gSystem->Setenv("ANALYSISaliceMV_INCLUDE", Form("%s",gSystem->pwd()));

  return gSystem->Load(TString::Format("%s/libANALYSISaliceMV.so",gSystem->pwd()).Data());
}
