#include <TError.h>
void ShowMixInfo(TString fileName) {
  //   AliLog::SetClassDebugLevel("AliMixEventCutObj", AliLog::kDebug);
  
	TFile *f = TFile::Open(fileName.Data());
	if (!f) {
		::Error("ShowMixInfo.C",Form("Problem opening file %s !!!",fileName.Data()));
		return;
	}
	TList *list =  (TList*) f->Get("cMixInfoList");
  if (list) list->Print();
	AliMixInfo *mi = (AliMixInfo*) list->FindObject("mixInfo");
  if (mi) {
    
    mi->Draw(/*"HIST"*/);
  }

}