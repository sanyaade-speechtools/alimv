void SETUP() {

  CheckLoadLibrary("libMonaLisa");
  
  // Set the include paths
  gROOT->ProcessLine(".include PWG2resonancesTest");
  gROOT->ProcessLine(".include PWG2resonancesTest/RESONANCES");

  // Set our location, so that other packages can find us
  gSystem->Setenv("PWG2resonancesTest_INCLUDE", "PWG2resonancesTest/RESONANCES");

  CheckLoadLibrary("libPWG2resonancesTest");
}

Int_t CheckLoadLibrary(const char* library) {
  if (strlen(gSystem->GetLibraries(Form("%s.so", library), "", kFALSE)) > 0)
    return 1;
  
  return gSystem->Load(Form("%s.so", library));
}
