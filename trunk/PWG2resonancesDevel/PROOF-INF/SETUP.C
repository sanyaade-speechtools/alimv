Int_t SETUP()
{

   gSystem->SetDynamicPath(Form("%s:%s", gSystem->pwd(), gSystem->GetDynamicPath()));

   // Set the include paths
   gROOT->ProcessLine(Form(".include %s", gSystem->pwd()));

   // Set our location, so that other packages can find us
   gSystem->Setenv("PWG2resonancesDevel_INCLUDE", Form("%s", gSystem->pwd()));

   return gSystem->Load(TString::Format("%s/libPWG2resonancesDevel.so", gSystem->pwd()).Data());
}
