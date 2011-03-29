void RsnCurrentCutsKStar(AliRsnCutManager *cutManager)
{

   if (!cutManager) return;

   // cut 2010
   gROOT->LoadMacro("RsnCut2010KStarKPi.C");
   RsnCut2010KStarKPi(cutManager);

   // choose PID ONLY
//    gROOT->LoadMacro("RsnCutPIDOnly.C");
//    RsnCutPIDOnly(cutManager);

}
