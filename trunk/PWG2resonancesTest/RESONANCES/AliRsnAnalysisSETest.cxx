//
// Class AliRsnAnalysisME
//
// TODO
//
// authors: Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//          Martin Vala (martin.vala@cern.ch)
//
#include <Riostream.h>
#include <TFile.h>
#include "AliESDEvent.h"
#include "AliMCEvent.h"
#include "AliAODEvent.h"

#include "AliRsnCutSet.h"
#include "AliRsnVATProcessInfo.h"
#include "AliRsnAnalysisSETest.h"

ClassImp(AliRsnAnalysisSETest)

//_____________________________________________________________________________
AliRsnAnalysisSETest::AliRsnAnalysisSETest(const char *name) :
    AliRsnAnalysisSE(name,0)
{
//
// Default constructor.
//

  AliDebug(AliLog::kDebug+2,"<-");
//   for (Int_t i=0;i<fNumberOfOutputs;i++) {
//     DefineOutput(i+2, TList::Class());
//   }
  AliDebug(AliLog::kDebug+2,"->");
}

//_____________________________________________________________________________
AliRsnAnalysisSETest::AliRsnAnalysisSETest(const AliRsnAnalysisSETest& copy) :
  AliRsnAnalysisSE(copy)
{
//
// Copy constructor.
//

  AliDebug(AliLog::kDebug+2,"<-");
  AliDebug(AliLog::kDebug+2,"->");
}

//_____________________________________________________________________________
void AliRsnAnalysisSETest::RsnUserCreateOutputObjects()
{
//
// Creation of output objects.
// These are created through the utility methods in the analysis manager,
// which produces a list of histograms for each specified set of pairs.
// Each of these lists is added to the main list of this task.
//

  AliDebug(AliLog::kDebug+2,"<-");

  AliDebug(AliLog::kDebug+2,"->");
}

//_____________________________________________________________________________
void AliRsnAnalysisSETest::RsnUserExec(Option_t*)
{
//
// Execution of the analysis task.
// Recovers the input event and processes it with all included pair objects.
//

  AliDebug(AliLog::kDebug+2,"<-");
  
  fTaskInfo.SetEventUsed(kFALSE);
  
  TTree* tree = dynamic_cast<TTree*>(GetInputData(0));
  if (!tree) {
    AliWarning("Maybe Tree");
    AliError("Could not read chain from input slot 0");
  }
  AliInfo(Form("Processing file %s ...",tree->GetCurrentFile()->GetName()));

  if (fESDEvent) {
    AliDebug(AliLog::kDebug+1, Form("fESDEvent is %p", fESDEvent));
    AliDebug(AliLog::kDebug, Form("ESD tracks %d", fESDEvent->GetNumberOfTracks()));
  }
  if (fMCEvent) {
    AliDebug(AliLog::kDebug+1, Form("fMCEvent is %p", fMCEvent));
    AliDebug(AliLog::kDebug, Form("MC tracks %d", fMCEvent->GetNumberOfTracks()));
  }
  if (fAODEventIn) {
    AliDebug(AliLog::kDebug+1, Form("fAODEventIn is %p", fAODEventIn));
    AliDebug(AliLog::kDebug, Form("AOD(in) tracks %d", fAODEventIn->GetNumberOfTracks()));
  }
  if (fAODEventOut) {
    AliDebug(AliLog::kDebug+1, Form("fAODEventOut if %p", fAODEventOut));
    AliDebug(AliLog::kDebug, Form("AOD(out) tracks %d", fAODEventOut->GetNumberOfTracks()));
  }

  // if cuts are passed or not cuts were defined,
  // update the task info...
  fTaskInfo.SetEventUsed(kTRUE);
  
  AliDebug(AliLog::kDebug+2,"->");
}


//_____________________________________________________________________________
void AliRsnAnalysisSETest::RsnTerminate(Option_t*)
{
//
// Termination.
// Could be added some monitor histograms here.
//

  AliDebug(AliLog::kDebug+2,"<-");
  AliDebug(AliLog::kDebug+2,"->");
}
