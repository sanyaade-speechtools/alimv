void RsnCurrentCutsPhiKK(AliRsnCutManager *cutManager)
{

   if (!cutManager) return;

   // cut 2010
   gROOT->LoadMacro("RsnCut2010PhiKK.C");
   RsnCut2010PhiKK(cutManager);

   // choose PID ONLY
//    gROOT->LoadMacro("RsnCutPIDOnly.C");
//    RsnCutPIDOnly(cutManager);
//
}
