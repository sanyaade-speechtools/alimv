void RsnCurrentCuts(AliRsnCutManager *cutManager) {
	
	if (!cutManager) return;
	
  // cut 2010
   gROOT->LoadMacro("RsnCut2010.C");
   RsnCut2010(cutManager);
  
	// choose PID ONLY
// 	gROOT->LoadMacro("RsnCutPIDOnly.C");
// 	RsnCutPIDOnly(cutManager);
	
//   gROOT->LoadMacro("RsnCutPIDOnlyWithQualityCuts.C");
// 	RsnCutPIDOnlyWithQualityCuts(cutManager);
}
  