#include "AliLog.h"
#include "AliAnalysisManager.h"

#include "AliRsnAnalysisTaskCommon.h"

ClassImp(AliRsnAnalysisTaskCommon)

//________________________________________________________________________
AliRsnAnalysisTaskCommon::AliRsnAnalysisTaskCommon() // All data members should be initialised here
   : AliAnalysisTaskSE(),
     fOutput(0)
{
   // Dummy constructor ALWAYS needed for I/O.
}

//________________________________________________________________________
AliRsnAnalysisTaskCommon::AliRsnAnalysisTaskCommon(const char *name) // All data members should be initialised here
   : AliAnalysisTaskSE(name),
     fOutput(0)
{
   // Constructor
   // Define input and output slots here (never in the dummy constructor)
   // Input slot #0 works with a TChain - it is connected to the default input container
   // Output slot #1 writes into a TH1 container
   DefineOutput(1, TList::Class());                                            // for output list
}

//________________________________________________________________________
AliRsnAnalysisTaskCommon::~AliRsnAnalysisTaskCommon()
{
   // Destructor. Clean-up the output list, but not the histograms that are put inside
   // (the list is owner and will clean-up these histograms). Protect in PROOF case.
   if (fOutput && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
      delete fOutput;
   }
}

//________________________________________________________________________
void AliRsnAnalysisTaskCommon::UserCreateOutputObjects()
{
   // Create histograms
   // Called once (on the worker node)

   fOutput = new TList();
   fOutput->SetOwner();  // IMPORTANT!

   // NEW HISTO added to fOutput here
   PostData(1, fOutput); // Post data for ALL output slots >0 here, to get at least an empty histogram
}

//________________________________________________________________________
void AliRsnAnalysisTaskCommon::UserExec(Option_t *)
{
   // Main loop
   // Called for each event

   PostData(1, fOutput);
}

void AliRsnAnalysisTaskCommon::UserExecMix(Option_t*)
{

   PostData(1, fOutput);
}

//________________________________________________________________________
void AliRsnAnalysisTaskCommon::Terminate(Option_t *)
{
   // Draw result to screen, or perform fitting, normalizations
   // Called once at the end of the query

   fOutput = dynamic_cast<TList*>(GetOutputData(1));
   if (!fOutput) { AliError("Could not retrieve TList fOutput"); return; }

}
