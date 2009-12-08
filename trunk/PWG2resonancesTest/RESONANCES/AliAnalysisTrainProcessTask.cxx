//
// Class AliAnalysisTrainProcessTask
//
// TODO
//
// authors: Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//          Martin Vala (martin.vala@cern.ch)
//
#include "AliLog.h"
#include "AliAnalysisTrainProcessTask.h"
#include <TH1.h>
#include <TROOT.h>
#include <TList.h>
#include <TSystem.h>
#include <TMonaLisaWriter.h>

ClassImp(AliAnalysisTrainProcessTask)

//_____________________________________________________________________________
AliAnalysisTrainProcessTask::AliAnalysisTrainProcessTask(const char *name) :
    AliAnalysisTaskSE(name),
    fMonaLisaWriter(0),
    fOutputList(0),
    fProcessInfoHist(0),
    fNumberOfEvents(0),
    fNumbersOfRefresh(1000)
{
//
// Default constructor.
//

  AliDebug(AliLog::kDebug + 2, "<-");
  DefineOutput(1, TList::Class());
  AliDebug(AliLog::kDebug + 2, "->");
}

//_____________________________________________________________________________
AliAnalysisTrainProcessTask::AliAnalysisTrainProcessTask(const AliAnalysisTrainProcessTask& copy) :
    AliAnalysisTaskSE(copy)
{
//
// Copy constructor.
//

  AliDebug(AliLog::kDebug + 2, "<-");
  AliDebug(AliLog::kDebug + 2, "->");
}

//_____________________________________________________________________________
void AliAnalysisTrainProcessTask::UserCreateOutputObjects()
{
//
// Creation of output objects.
// These are created through the utility methods in the analysis manager,
// which produces a list of histograms for each specified set of pairs.
// Each of these lists is added to the main list of this task.
//


  AliDebug(AliLog::kDebug + 2, "<-");
  Int_t gid = gROOT->ProcessLine("gProofServ->GetGroupId();");
  fMonaLisaWriter = new TMonaLisaWriter("http://alicepc100.jinr.ru/gentoo-files/monalisa_saske.conf", "Traintest", Form("%s_%d", gSystem->HostName(),gid));
//   TMonaLisaWriter *fMonaLisaWriter = new TMonaLisaWriter("Train",Form("%s",gSystem->HostName()),"147.213.204.6:8884");
  SendMonalisaMessageDouble("numberOfEvents",0);
//   SendMonalisaMessageDouble("Status",0);
//   SendMonalisaMessageDouble("Message","Started");

  fOutputList = new TList();
  fProcessInfoHist = new TH1I("hProcessInfo", "Process Info", 2, 0, 2);
  fOutputList->Add(fProcessInfoHist);
  
  AliDebug(AliLog::kDebug + 2, "->");
}


//_____________________________________________________________________________
void AliAnalysisTrainProcessTask::UserExec(Option_t*)
{
//
// Execution of the analysis task.
// Recovers the input event and processes it with all included pair objects.
//

  AliDebug(AliLog::kDebug + 2, "<-");

  if (fNumberOfEvents%fNumbersOfRefresh==0) SendMonalisaMessageDouble("numberOfEvents",fNumberOfEvents);
  
  
  fProcessInfoHist->Fill(1);

  PostData(1, fOutputList);

  fNumberOfEvents++;
  
  AliDebug(AliLog::kDebug + 2, "->");
}


//_____________________________________________________________________________
void AliAnalysisTrainProcessTask::Terminate(Option_t*)
{
//
// Termination.
// Could be added some monitor histograms here.
//

  AliDebug(AliLog::kDebug + 2, "<-");
  AliAnalysisTask::Terminate();

  TList* list  = dynamic_cast<TList*>(GetOutputData(1));
  if (!list) {
    AliError(Form("At end of analysis, fOutList is %p", list));
    return;
  }


  TH1I *hProcessInfo = (TH1I*) list->FindObject("hProcessInfo");
  if (!hProcessInfo) {
    AliError(Form("hProcessInfo is %p", hProcessInfo));
    return;
  }
  hProcessInfo->Print();
  AliInfo(Form("=== %s ==================", GetName()));
  AliInfo(Form("Number Of Events Processed : %10d", (Long64_t)hProcessInfo->Integral()));
  AliInfo(Form("Number Of Events Accepted  : %10d", (Long64_t)hProcessInfo->GetBinContent(2)));
  AliInfo(Form("Number Of Events Skipped   : %10d", (Long64_t)hProcessInfo->GetBinContent(1)));
  AliInfo(Form("=== end %s ==============", GetName()));

  AliDebug(AliLog::kDebug + 2, "->");
}


void AliAnalysisTrainProcessTask::FinishTaskOutput()
{
//   AliAnalysisTask::FinishTaskOutput();
  SendMonalisaMessageDouble("numberOfEvents",fNumberOfEvents);
}


void AliAnalysisTrainProcessTask::SendMonalisaMessage(const char* name, const char* message)
{
  if (!fMonaLisaWriter) return;
  
  TList *fMonaLisaValuelist = new TList();
  fMonaLisaValuelist->SetOwner(kTRUE);
  TMonaLisaText *text = new TMonaLisaText(name, message);
  fMonaLisaValuelist->Add(text);
  fMonaLisaWriter->SendParameters(fMonaLisaValuelist);
  delete fMonaLisaValuelist;
}


void AliAnalysisTrainProcessTask::SendMonalisaMessageDouble(const char* name, Double_t val)
{
  if (!fMonaLisaWriter) return;
  
  TList *fMonaLisaValuelist = new TList();
  fMonaLisaValuelist->SetOwner(kTRUE);
  TMonaLisaValue* value = new TMonaLisaValue(name, val);
  fMonaLisaValuelist->Add(value);
  
  fMonaLisaWriter->SendParameters(fMonaLisaValuelist);
  delete fMonaLisaValuelist;
}
