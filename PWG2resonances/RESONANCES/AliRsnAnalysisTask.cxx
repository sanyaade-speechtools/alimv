#include <TEntryList.h>

#include "AliLog.h"
#include "AliAnalysisManager.h"
#include "AliMultiInputEventHandler.h"
#include "AliMixInputEventHandler.h"
#include "AliMCEventHandler.h"

#include "AliRsnEvent.h"
#include "AliRsnLoop.h"
#include "AliRsnInputHandler.h"

#include "AliRsnAnalysisTask.h"

ClassImp(AliRsnAnalysisTask)

//__________________________________________________________________________________________________
AliRsnAnalysisTask::AliRsnAnalysisTask() :
   AliAnalysisTaskSE(),
   fOutput(0),
   fRsnObjects(0)
{
//
// Dummy constructor ALWAYS needed for I/O.
//
}

//__________________________________________________________________________________________________
AliRsnAnalysisTask::AliRsnAnalysisTask(const char *name) :
   AliAnalysisTaskSE(name),
   fOutput(0),
   fRsnObjects(0)
{
//
// Default constructor.
// Define input and output slots here (never in the dummy constructor)
// Input slot #0 works with a TChain - it is connected to the default input container
// Output slot #1 writes into a TH1 container
//

   DefineOutput(1, TList::Class());
}

//__________________________________________________________________________________________________
AliRsnAnalysisTask::AliRsnAnalysisTask(const AliRsnAnalysisTask& copy) :
   AliAnalysisTaskSE(copy),
   fOutput(0),
   fRsnObjects(copy.fRsnObjects)
{
//
// Copy constructor.
// Implemented as requested by C++ standards.
// Can be used in PROOF and by plugins.
//
}

//__________________________________________________________________________________________________
AliRsnAnalysisTask& AliRsnAnalysisTask::operator=(const AliRsnAnalysisTask& copy)
{
//
// Assignment operator.
// Implemented as requested by C++ standards.
// Can be used in PROOF and by plugins.
//
   AliAnalysisTaskSE::operator=(copy);
   fRsnObjects = copy.fRsnObjects;

   return (*this);
}

//__________________________________________________________________________________________________
AliRsnAnalysisTask::~AliRsnAnalysisTask()
{
//
// Destructor.
// Clean-up the output list, but not the histograms that are put inside
// (the list is owner and will clean-up these histograms). Protect in PROOF case.
//

   if (fOutput && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
      delete fOutput;
   }
}

//__________________________________________________________________________________________________
void AliRsnAnalysisTask::Add(AliRsnLoop *obj)
{
//
// Add new computation object
//

   fRsnObjects.Add(obj);
}

//__________________________________________________________________________________________________
void AliRsnAnalysisTask::UserCreateOutputObjects()
{
//
// Initialization of outputs.
// This is called once per worker node.
//

   // sets all Inuput Handler pointers
   InitInputHandlers();

   // create list and set it as owner of its content (MANDATORY)
   fOutput = new TList();
   fOutput->SetOwner();

   // loop on computators and initialize all their outputs
   TObjArrayIter next(&fRsnObjects);
   AliRsnLoop *obj = 0x0;
   while ((obj = (AliRsnLoop*)next())) {
      obj->Init(GetName(), fOutput);
   }

   // post data for ALL output slots >0 here, to get at least an empty histogram
   PostData(1, fOutput);
}

//__________________________________________________________________________________________________
void AliRsnAnalysisTask::UserExec(Option_t *)
{
//
// Main loop for single-event computations.
// It is called for each event and executes the 'DoLoop'
// function of all AliRsnLoop instances stored here.
//

   Int_t idRsnHandler = 1;
   if (dynamic_cast<AliMCEventHandler*>(fInputEHMain->GetFirstMCEventHandler())) idRsnHandler++;

   AliRsnInputHandler *rsnIH = dynamic_cast<AliRsnInputHandler*>(fInputEHMain->InputEventHandler(idRsnHandler));
   AliRsnEvent *evMain = rsnIH->GetRsnEvent();

   TObjArrayIter next(&fRsnObjects);
   AliRsnLoop *obj = 0x0;
   while ((obj = (AliRsnLoop*)next())) {
      if (obj->IsMixed()) continue;
      obj->DoLoop(evMain, rsnIH->GetSelector());
   }

   PostData(1, fOutput);
}

//__________________________________________________________________________________________________
void AliRsnAnalysisTask::UserExecMix(Option_t*)
{
//
// Main loop for event-mixing computations
// It is called for each pair of matched events
// and executes the 'DoLoop' function of all AliRsnLoop instances stored here.
//

   // gets first input handler form mixing buffer
   AliMultiInputEventHandler *ihMultiMix = dynamic_cast<AliMultiInputEventHandler*>(fInputEHMix->InputEventHandler(0));

   Int_t idRsnHandler = 1;
   if (dynamic_cast<AliMCEventHandler*>(fInputEHMain->GetFirstMCEventHandler())) idRsnHandler++;

   AliRsnInputHandler *rsnIH = dynamic_cast<AliRsnInputHandler*>(fInputEHMain->InputEventHandler(idRsnHandler));
   AliRsnInputHandler *rsnMixIH = dynamic_cast<AliRsnInputHandler*>(ihMultiMix->InputEventHandler(idRsnHandler));

   AliRsnEvent *evMain = rsnIH->GetRsnEvent();
   AliRsnEvent *evMix = rsnMixIH->GetRsnEvent();

   TObjArrayIter next(&fRsnObjects);
   AliRsnLoop *obj = 0x0;
   while ((obj = (AliRsnLoop*)next())) {
      if (!obj->IsMixed()) continue;
      obj->DoLoop(evMain, rsnIH->GetSelector(), evMix, rsnMixIH->GetSelector());
   }

   PostData(1, fOutput);
}

//________________________________________________________________________
void AliRsnAnalysisTask::Terminate(Option_t *)
{
//
// Draw result to screen, or perform fitting, normalizations
// Called once at the end of the query
//

   fOutput = dynamic_cast<TList*>(GetOutputData(1));
   if (!fOutput) { AliError("Could not retrieve TList fOutput"); return; }
}

//_____________________________________________________________________________
void AliRsnAnalysisTask::InitInputHandlers()
{
//
// Sets needed input handlers
//
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   fInputEHMain = dynamic_cast<AliMultiInputEventHandler *>(mgr->GetInputEventHandler());
   if (fInputEHMain) {
      fInputEHMix = dynamic_cast<AliMixInputEventHandler *>(fInputEHMain->GetFirstMultiInputHandler());
   }
}