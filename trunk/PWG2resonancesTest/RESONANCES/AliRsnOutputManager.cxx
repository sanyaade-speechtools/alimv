//
// Class AliRsnOutputManager
//
// author: M. Vala (email: martin.vala@cern.ch)
//

#include "AliLog.h"
#include "AliRsnPair.h"

#include "AliRsnOutputManager.h"

ClassImp(AliRsnOutputManager)

//_____________________________________________________________________________
AliRsnOutputManager::AliRsnOutputManager(const char*name) :
    AliRsnVManager(name)
{
//
// Default constructor
//

  AliDebug(AliLog::kDebug +2, "<-");
  AliDebug(AliLog::kDebug +2, "->");
}

//_____________________________________________________________________________
void AliRsnOutputManager::Add(TObject* objPair)
{
//
// Adds a new AliRsnPair to the list owned by this object.
//

  AliDebug(AliLog::kDebug+2, "<-");
  AliRsnOutputObj *pair = dynamic_cast<AliRsnOutputObj*>(objPair);

  if (!pair) {
    AliWarning(Form("Pair is %p. Skipping ...", pair));
    return;
  }

//   AliDebug(AliLog::kDebug+1, Form("Adding %s [%d entries] ...", pair->GetPairName().Data(), fArray.GetEntries()));
fArray.Add((AliRsnOutputObj*)pair);

  AliDebug(AliLog::kDebug+2, "->");
}

//_____________________________________________________________________________
void AliRsnOutputManager::AddPair(AliRsnOutputObj* pair)
{
//
// Adds a new AliRsnPair to the list owned by this object.
//

  Add(pair);
}


//_____________________________________________________________________________
void AliRsnOutputManager::Print(Option_t* /*dummy*/) const
{
//
// Overload of TObject::Print() method.
// With respect to the other print method, adds a title string.
//

  AliDebug(AliLog::kDebug+2,"<-");

  AliInfo(Form("\t\t======== Pair Manager %s ========", GetName()));
  PrintArray();

  AliDebug(AliLog::kDebug+2,"->");
}

//_____________________________________________________________________________
void AliRsnOutputManager::PrintArray() const
{
//
// Prints all pairs
//

  AliDebug(AliLog::kDebug+2,"<-");

  AliRsnOutputObj *pair = 0;
  TObjArrayIter next(&fArray);
  while ((pair = (AliRsnOutputObj*)next())) pair->Print();

  AliDebug(AliLog::kDebug+2,"->");
}

//_____________________________________________________________________________
void AliRsnOutputManager::InitAll(TList* list)
{
//
// Initialize all pairs, and builds a TList of histograms
// which are created by each of them, in order to link it
// to the output handler in the AnalysisTasks.
//

  AliDebug(AliLog::kDebug+2, "<-");

//   TList *list = new TList();
//   list->SetName(GetName());
//   list->SetOwner();

AliRsnOutputObj *pair = 0;
  TObjArrayIter next(&fArray);

//   Int_t i = 0;
  while ((pair = (AliRsnOutputObj*)next())) {
    if (!pair) continue;
//     AliInfo(Form("AliRsnOutputObj index %d",i++));
//     AliDebug(AliLog::kDebug+1, Form("InitAllPairs of the PairManager(%s) [%d] ...", pair->GetPairName().Data(), i++));
//     pair->GenerateHistograms(GetName(),list);
    pair->InitHistograms(list);
  }

  AliDebug(AliLog::kDebug+2, "->");
//   return list;
}


void AliRsnOutputManager::InitAllExtra(TList* list)
{
  //
  // Initialize all pairs, and builds a TList of histograms
  // which are created by each of them, in order to link it
  // to the output handler in the AnalysisTasks.
  //
  
  AliDebug(AliLog::kDebug+2, "<-");
  
  //   TList *list = new TList();
  //   list->SetName(GetName());
  //   list->SetOwner();
  
  AliRsnOutputObj *pair = 0;
  TObjArrayIter next(&fArray);
  
  //   Int_t i = 0;
  while ((pair = (AliRsnOutputObj*)next())) {
    if (!pair) continue;
    //     AliInfo(Form("AliRsnOutputObj index %d",i++));
    //     AliDebug(AliLog::kDebug+1, Form("InitAllPairs of the PairManager(%s) [%d] ...", pair->GetPairName().Data(), i++));
    //     pair->GenerateHistograms(GetName(),list);
    pair->InitExtraHistograms(list);
  }
  
  AliDebug(AliLog::kDebug+2, "->");
  //   return list;
}


//_____________________________________________________________________________
void AliRsnOutputManager::ProcessAll(TFile* file, TList* list,TH1I *processInfoHist)
{
//
// Processes one (single-event analysis) or two (event-mixing) events
// to fill histograms in all stored pairs.
//

  AliDebug(AliLog::kDebug+2, "<-");

  AliRsnOutputObj *pair = 0;
  TObjArrayIter next(&fArray);

//   Int_t i=0;
  while ((pair = (AliRsnOutputObj*)next())) {
    if (!pair) continue;
//     AliInfo(Form("AliRsnOutputObj index %d",i++));
//     AliDebug(AliLog::kDebug+1, Form("ProcessAllPairs of the PairManager(%s) [%d] ...", pair->GetPairName().Data(), i++));
    pair->ProcessHistograms(file,list);
    if (processInfoHist)
      AliInfo(Form("Processed %d events ...", (Long64_t)processInfoHist->Integral()));
  }

  AliDebug(AliLog::kDebug+2, "->");
}


void AliRsnOutputManager::ProcessAllExtra(TList* list)
{
  AliDebug(AliLog::kDebug+2, "<-");
  
  AliRsnOutputObj *pair = 0;
  TObjArrayIter next(&fArray);
  
//   Int_t i=0;
  while ((pair = (AliRsnOutputObj*)next())) {
    if (!pair) continue;
    //     AliInfo(Form("AliRsnOutputObj index %d",i++));
    //     AliDebug(AliLog::kDebug+1, Form("ProcessAllPairs of the PairManager(%s) [%d] ...", pair->GetPairName().Data(), i++));
    pair->ProcessExtraHistograms(list);
  }
  
  AliDebug(AliLog::kDebug+2, "->");
}

