Bool_t AddAnalysisTaskAllOld(TString format="esd",Bool_t useMC = kFALSE,TString opts="") {
	
	// //     // adds mixing info task
//     gROOT->LoadMacro("AddAnalysisTaskMixInfo.C");
//     AddAnalysisTaskMixInfo(format, useMC, opts);

		gROOT->LoadMacro("AddAnalysisTaskCustomMixOld.C");
    AddAnalysisTaskCustomMixOld(format, useMC, opts);
    
    return kTRUE;
}
