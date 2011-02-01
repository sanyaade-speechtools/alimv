#include <TError.h>
#include <Rtypes.h>
#include <TROOT.h>
#include <TSystem.h>

Int_t LoadLibs()
{
   ::Info("", "Loading ...");

   if (gROOT->Macro(gSystem->ExpandPathName("$ALICE_ROOT/macros/loadlibs.C"))) return 1;

   if (gSystem->Load("libANALYSIS.so") < 0) return 2;

   if (gSystem->Load("libANALYSISalice.so") < 0) return 3;

   if (gSystem->Load("MIXMV/libMIXMV.so") < 0) return 4;

   ::Info("", "Loading done...");

   return 0;
}
