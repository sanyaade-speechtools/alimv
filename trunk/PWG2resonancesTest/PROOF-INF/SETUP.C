void SETUP() {
  CheckLoadLibrary("libMonaLisa");
  CheckLoadLibrary("libANALYSIS");
  CheckLoadLibrary("libANALYSISalice");
  CheckLoadLibrary("libCORRFW");
  CheckLoadLibrary("libPWG2resonances");
  CheckLoadLibrary("libPWG2resonancesTest");
  // Set the include paths
  gROOT->ProcessLine(".include PWG2resonancesTest");
  gROOT->ProcessLine(".include PWG2resonancesTest/RESONANCES");
  
  // Set our location, so that other packages can find us
  gSystem->Setenv("PWG2resonancesTest_INCLUDE", "PWG2resonancesTest/RESONANCES");
}

Int_t CheckLoadLibrary(const char* library) {
  // checks if a library is already loaded, if not loads the library

  if (strlen(gSystem->GetLibraries(Form("%s.so", library), "", kFALSE)) > 0)
    return 1;
  
  return gSystem->Load(library);
}
