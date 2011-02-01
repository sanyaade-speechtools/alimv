//Standard Root includes
// #include <TROOT.h>
#include <TRint.h>
#include <TSystem.h>
#include <TError.h>
#include <TString.h>
//_____________________________________________________________________________
int main(int argc, char **argv)
{

   TString alimvPath = gSystem->Getenv("ALIMV");
   Printf("path is %s", alimvPath.Data());
   TRint *theApp = new TRint("alimv", &argc, argv);

   Int_t num = 0;
   // root libs
   if (gSystem->Load("libTree.so") < 0) {num++; return num;}

   if (gSystem->Load("libGeom.so") < 0) {num++; return num;}
   if (gSystem->Load("libVMC.so") < 0) {num++; return num;}
   if (gSystem->Load("libPhysics.so") < 0) {num++; return num;}

   // aliroot base analysis lib
   if (gSystem->Load("libSTEERBase.so") < 0) {num++; return num;}
   if (gSystem->Load("libESD.so") < 0) {num++; return num;}
   if (gSystem->Load("libAOD.so") < 0) {num++; return num;}
   if (gSystem->Load("libANALYSIS.so") < 0) {num++; return num;}
   if (gSystem->Load("libANALYSISalice.so") < 0) {num++; return num;}

   // extra libs
   if (!alimvPath.IsNull()) {
      if (alimvPath.Last('/') + 1 == alimvPath.Length()) alimvPath.Remove(alimvPath.Last('/'));
      alimvPath.Append(Form("/lib/tgt_%s/", gSystem->GetBuildArch()));
      gSystem->SetDynamicPath(Form("%s:%s", alimvPath.Data(), gSystem->GetDynamicPath()));
      if (gSystem->Load("libANALYSISaliceMV.so") < 0) {num++; return num;}
   }

   if (gSystem->Load("libEventMixing.so") < 0) {num++; return num;}
   // --- Start the event loop ---
   theApp->Run();

   return(0);
}

