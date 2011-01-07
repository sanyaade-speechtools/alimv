Bool_t AddAnalysisTaskAllRsnBat(TString format="esd",Bool_t useMC = kFALSE,TString opts="") {
	
	// //     // adds mixing info task
    gROOT->LoadMacro("AddRsnCustomTaskBat.C");
    AddRsnCustomTaskBat(format, useMC, opts);
	return kTRUE;
}
