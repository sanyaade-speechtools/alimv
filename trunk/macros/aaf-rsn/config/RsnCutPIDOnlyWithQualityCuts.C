void RsnCutPIDOnlyWithQualityCuts(AliRsnCutManager *cutManager) {
	
	if (!cutManager) return;
	const char* path =".";
	
	gROOT->LoadMacro(Form("%s/QualityCutsITS.C", path));
  gROOT->LoadMacro(Form("%s/QualityCutsTPC.C", path));
	
	AliRsnCutESDPrimary *qualityITS = new AliRsnCutESDPrimary("cutITS");
	QualityCutsITS(qualityITS->GetCuts());
	
	AliRsnCutESDPrimary *qualityTPC = new AliRsnCutESDPrimary("cutTPC");
	QualityCutsTPC(qualityTPC->GetCuts());
	
	  // -- tracks --> PID
  AliRsnCutPID *cutPID = new AliRsnCutPID("cutPID", AliPID::kKaon, 0.0, kFALSE);
	
// 	cutManager->GetCommonDaughterCuts()->AddCut(qualityITS);
	cutManager->GetCommonDaughterCuts()->AddCut(qualityTPC);
//   cutManager->GetCommonDaughterCuts()->AddCut(cutPID);
//   cutManager->GetCommonDaughterCuts()->SetCutScheme(Form("%s",qualityITS->GetName()));
//   cutManager->GetCommonDaughterCuts()->SetCutScheme(Form("%s",qualityTPC->GetName()));
//   cutManager->GetCommonDaughterCuts()->SetCutScheme(Form("%s&%s",qualityITS->GetName(),qualityTPC->GetName()));
  
//   cutManager->GetCommonDaughterCuts()->SetCutScheme(Form("%s",cutPID->GetName()));
//   cutManager->GetCommonDaughterCuts()->SetCutScheme(Form("%s&%s&%s",qualityITS->GetName(),qualityTPC->GetName(),cutPID->GetName()));
//   cutManager->GetCommonDaughterCuts()->Print();
}