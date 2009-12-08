//
// Class AliRsnParticleInfoManager
//
// author: M. Vala (email: martin.vala@cern.ch)
//

#include "AliLog.h"
#include "AliRsnPair.h"

#include "AliRsnParticleInfoManager.h"

ClassImp(AliRsnParticleInfoManager)

//_____________________________________________________________________________
AliRsnParticleInfoManager::AliRsnParticleInfoManager(const char*name) :
    AliRsnVManager(name)
{
//
// Default constructor
//

  AliDebug(AliLog::kDebug +2, "<-");
  AliDebug(AliLog::kDebug +2, "->");
}

//_____________________________________________________________________________
void AliRsnParticleInfoManager::Add(TObject* objPair)
{
//
// Adds a new AliRsnPair to the list owned by this object.
//

  AliDebug(AliLog::kDebug+2, "<-");
  AliRsnParticleInfo *pair = dynamic_cast<AliRsnParticleInfo*>(objPair);

  if (!pair) {
    AliWarning(Form("Pair is %p. Skipping ...", pair));
    return;
  }

//   AliDebug(AliLog::kDebug+1, Form("Adding %s [%d entries] ...", pair->GetPairName().Data(), fArray.GetEntries()));
  fArray.Add((AliRsnParticleInfo*)pair);

  AliDebug(AliLog::kDebug+2, "->");
}

//_____________________________________________________________________________
void AliRsnParticleInfoManager::AddPair(AliRsnParticleInfo* pair)
{
//
// Adds a new AliRsnPair to the list owned by this object.
//

  Add(pair);
}


//_____________________________________________________________________________
void AliRsnParticleInfoManager::Print(Option_t* /*dummy*/) const
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
void AliRsnParticleInfoManager::PrintArray() const
{
//
// Prints all pairs
//

  AliDebug(AliLog::kDebug+2,"<-");

  AliRsnParticleInfo *pair = 0;
  TObjArrayIter next(&fArray);
  while ((pair = (AliRsnParticleInfo*)next())) pair->Print();

  AliDebug(AliLog::kDebug+2,"->");
}

//_____________________________________________________________________________
void AliRsnParticleInfoManager::InitAllParticleInfos(TList* list)
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

AliRsnParticleInfo *pair = 0;
  TObjArrayIter next(&fArray);

  Int_t i = 0;
  while ((pair = (AliRsnParticleInfo*)next())) {
    if (!pair) continue;
    AliInfo(Form("ParticleInfo index %d",i++));
//     AliDebug(AliLog::kDebug+1, Form("InitAllPairs of the PairManager(%s) [%d] ...", pair->GetPairName().Data(), i++));
//     pair->GenerateHistograms(GetName(),list);
    pair->InitHistograms(list);
  }

  AliDebug(AliLog::kDebug+2, "->");
//   return list;
}

//_____________________________________________________________________________
void AliRsnParticleInfoManager::ProcessAllParticleInfos(AliMCEvent *ev)
{
//
// Processes one (single-event analysis) or two (event-mixing) events
// to fill histograms in all stored pairs.
//

  AliDebug(AliLog::kDebug+2, "<-");

  AliRsnParticleInfo *pair = 0;
  TObjArrayIter next(&fArray);

  Int_t i=0;
  while ((pair = (AliRsnParticleInfo*)next())) {
    if (!pair) continue;
    AliInfo(Form("ParticleInfo index %d",i++));
//     AliDebug(AliLog::kDebug+1, Form("ProcessAllPairs of the PairManager(%s) [%d] ...", pair->GetPairName().Data(), i++));
    pair->ProcessHistograms(ev);
  }

  AliDebug(AliLog::kDebug+2, "->");
}
