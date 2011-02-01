//
// Class AliRsnCustomTask
//
// Virtual Class derivated from AliRsnVAnalysisTask which will be base class
// for all RSN SE tasks
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//

#include <TList.h>

#include "AliRsnCustomTask.h"
#include "AliRsnVCustomObj.h"

ClassImp(AliRsnCustomTask)

//_____________________________________________________________________________
AliRsnCustomTask::AliRsnCustomTask(const char *name, Bool_t useKine) :
  AliRsnVAnalysisTask(name, useKine),
  fOutList(0)
{
//
// Default constructor.
// Defines another output slot for histograms/ntuples
//

  DefineOutput(2, TList::Class());
}

//_____________________________________________________________________________
AliRsnCustomTask::AliRsnCustomTask(const AliRsnCustomTask& copy) :
  AliRsnVAnalysisTask(copy)
{
//
// Copy constructor.
//
}

//_____________________________________________________________________________
AliRsnCustomTask& AliRsnCustomTask::operator=(const AliRsnCustomTask& copy)
{
//
// Assigment operator.
//

  AliRsnVAnalysisTask::operator=(copy);
  
  if (fOutList) fOutList->Clear();
  
  return (*this);
}

//_____________________________________________________________________________
void AliRsnCustomTask::RsnUserCreateOutputObjects()
{
//
// Creation of output objects.
// These are created through the utility methods in the analysis manager,
// which asks all the AliRsnPair objects to initialize their output which
// is then linked to the TList data member of this, which will contain all the output.
//

  if (!fOutList) fOutList = new TList;
  fOutList->Clear();
  
  AliRsnVCustomObj *inObj=0;
  TIter next(&fInputObjects);
  while ((inObj = (AliRsnVCustomObj*)next())) 
    inObj->UserCreateOutputCustom(fOutList);
  
  PostData(2, fOutList);
}

//_____________________________________________________________________________
void AliRsnCustomTask::RsnUserExec(Option_t*)
{
//
// Execution of the analysis task.
// Recovers the input event and processes it with all included pair objects,
// using 'reconstructed' or 'MonteCarlo' functions depending on MC-only flag.
//

	if (IsMixing()) return;
  
  AliRsnVCustomObj *inObj=0;
  TIter next(&fInputObjects);
  while ((inObj = (AliRsnVCustomObj*)next())) 
    inObj->UserExecCustom(AliRsnEvent::GetCurrentEvent1(),AliRsnEvent::GetCurrentEvent2());

	PostData(2, fOutList);
}

void AliRsnCustomTask::RsnUserExecMix(Option_t* /*opt*/)
{
  if (!IsMixing()) return;
  
  if (!fMixedEH) return;
  
  AliDebug(AliLog::kDebug,Form("RSN Mixing %lld %d [%lld,%lld] %d", fMixedEH->CurrentEntry(),fMixedEH->NumberMixed(),fMixedEH->CurrentEntryMain(),fMixedEH->CurrentEntryMix(),fMixedEH->CurrentBinIndex()));
  
  // the virtual class has already sorted tracks in the PID index
  // so we need here just to call the execution of analysis
  AliRsnVCustomObj *inObj=0;
  TIter next(&fInputObjects);
  while ((inObj = (AliRsnVCustomObj*)next())) 
    inObj->UserExecMixCustom(AliRsnEvent::GetCurrentEvent1(),AliRsnEvent::GetCurrentEvent2());
  
}
void AliRsnCustomTask::RsnTerminate(Option_t* )
{
}

