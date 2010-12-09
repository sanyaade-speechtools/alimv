Int_t SETUP() {

  gSystem->SetDynamicPath(Form(".:%s", gSystem->GetDynamicPath()));

  // Set the include paths
  gROOT->ProcessLine(Form(".include %s/EventMixing",gSystem->pwd()));
  
  // Set our location, so that other packages can find us
  gSystem->Setenv("EventMixing_INCLUDE", Form("%s/EventMixing",gSystem->pwd()));

  return gSystem->Load(TString::Format("%s/libEventMixing.so",gSystem->pwd()).Data());
}
