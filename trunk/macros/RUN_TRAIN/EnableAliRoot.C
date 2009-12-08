Bool_t EnableAliRoot(TString aliroot_version = "v4-18-08-AN",TString platform="linuxx8664gcc") {
//   TString root_version="v5-25-02",

//   TProof::Open("skaf.saske.sk");
  TProof *proof = gProof;

  if (!proof) {
    Error("EnableAliRoot.C", "No connection to proof !!!");
    return kFALSE;
  }



  gProof->Exec(Form("gSystem->Setenv\(\"ALICE_ROOT\",gSystem->ExpandPathName\(\"$ROOTSYS/aliroot/%s\"\)\)", aliroot_version.Data()),kTRUE);
//   gProof->Exec("gSystem->Getenv\(\"ALICE_ROOT\"\)");
  gProof->Exec(Form("gSystem->Setenv\(\"ALICE_TARGET\",\"%s\")",platform.Data()),kTRUE);
  
//   gProof->Exec(Form("gSystem->Setenv\(\"LD_LIBRARY_PATH\",gSystem->ExpandPathName\(\"$ALICE_ROOT/lib/tgt_linuxx8664gcc/:$LD_LIBRARY_PATH\"\)\)", aliroot_version.Data()),kTRUE);
  
//   TString 
  gProof->Exec("gSystem->SetDynamicPath\(Form\(\".:%s:%s\"\,gSystem->ExpandPathName\(\"$ALICE_ROOT/lib/tgt_$ALICE_TARGET\"\),gSystem->GetDynamicPath())\)",kTRUE);
//   gProof->Exec("gSystem->Load\(gSystem->ExpandPathName\(\"$ALICE_ROOT/lib/tgt_linuxx8664gcc/libSTEERBase.so\"\)\)");




  TString libs = "VMC,Net,Tree,Physics,STEERBase,ESD,AOD,ANALYSIS,ANALYSISalice";
  libs += ",CORRFW";
  libs += ",PWG3base,PWG3muon,JETAN,PWG4PartCorrBase,PWG4PartCorrDep,PWG4GammaConv,PWG4omega3pi,PWG2spectra";
  libs += ",PWG2flowCommon,PWG2flowTasks,PWG2resonances,PWG2kink,PWG2unicor,PWG2evchar";
  libs += ",PWG2AOD,PWG2femtoscopy,PWG2femtoscopyUser,PWG3vertexingHF";
  
//    libs+=",libPWG2resonances";
  if (LoadLibraries(libs) <0 ) return kFALSE;

//   gProof->Exec("gSystem->Load\(\"libSTEERBase.so\"\)");
//   gProof->Exec("gSystem->GetDynamicPath()");

  return kTRUE;
}
Int_t LoadLibraries(TString libs = "VMC,Net,Tree,Physics,STEERBase,ESD,AOD,ANALYSIS,ANALYSISalice") {
  // Load ROOT and Analysis Framework libraries
  TObjArray* tokens = libs.Tokenize(",");
  TIter iter(tokens);
  TObjString* lib = 0;

  while (lib = (TObjString*) iter.Next())
    if (CheckLoadLibrary(lib->GetString()) < 0) {
      cout << "ERROR: " << lib->GetString() << ".so not found or mismatch." << endl;
      return -1;
    }
}

Int_t CheckLoadLibrary(TString library) {
  // checks if a library is already loaded, if not loads the library
  //

  if (library.IsNull())
    return 0;
  library.ReplaceAll(".so","");

  library = Form("lib%s.so",library.Data());

  Printf("Loading %s ...",library.Data());

  Int_t retVal = 0;
  retVal = gSystem->Load(library.Data());
  if (retVal<0) {
    Error("CheckLoadLibrary",Form("Error loading %s", library.Data()));
    return -1;
  }
    
  retVal = gProof->Exec(Form("gSystem->Load\(\"%s\"\);", library.Data()),kTRUE);

//   Printf("retVal is %d",retVal);
  return retVal;
//   return 0;
//   return gSystem->Load(library);
}
