void RsnCurrentCuts(AliRsnCutManager *cutManager) {
	
	if (!cutManager) return;
	
	// choose PID ONLY
// 	gROOT->LoadMacro("RsnCutPIDOnly.C");
// 	RsnCutPIDOnly(cutManager);
	
	// cut 2010
	gROOT->LoadMacro("RsnCut2010.C");
	RsnCut2010(cutManager);
	
}