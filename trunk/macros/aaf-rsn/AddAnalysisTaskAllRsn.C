Bool_t AddAnalysisTaskAllRsn(TString format="esd",Bool_t useMC = kFALSE,TString opts="") {
	
	// //     // adds mixing info task
    gROOT->LoadMacro("AddAnalysisTaskMixInfo.C");
    AddAnalysisTaskMixInfo(format, useMC, opts);

		// add our taks
    gROOT->LoadMacro("AddRsnTask.C");
    AddRsnTask(format, useMC, opts);
    AddRsnTask(format, useMC, Form("%s mix",opts));
		
	return kTRUE;
}
