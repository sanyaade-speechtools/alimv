#include <TError.h>
#include <TString.h>
#include <TSystem.h>
#include <TROOT.h>

Long64_t fNEvents = 2*1e5;
Long64_t fNEventsSkip = 0;

Int_t runALICE(TString analysisSource="grid",TString analysisMode="terminate") {

  Int_t ret = 0;
  ::Info(gSystem->HostName(),Form("Running <%s> in <%s> mode ...",analysisSource.Data(),analysisMode.Data()));

  // Loading ANALYSIS Base libs
  ::Info(gSystem->HostName(),"Loading ...");
  ret = LoadLibsBase();
  if (ret) { ::Error(gSystem->HostName(),"Error loading base libs !!!");return ret;}
  ::Info(gSystem->HostName(),"Loading done ...");

  // ++++++++++++++++ Adding Analysis Managers +++++++++++++++++++++++
