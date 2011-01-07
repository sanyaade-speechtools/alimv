#ifndef ALIANALYSISTASKMULTI_H
#define ALIANALYSISTASKMULTI_H

//
// Class AliAnalysisTaskMulti
//
// AliAnalysisTaskMulti is normal SE task with some additional functionality for
// AliMultiInputEventHandler and extends UserExecMix function for mixing
//
// authors:
//          Martin Vala (martin.vala@cern.ch)
//

#include "AliAnalysisTaskSE.h"

class AliMultiInputEventHandler;
class AliMCEventHandler;
class AliAnalysisTaskMulti : public AliAnalysisTaskSE {
public:
   AliAnalysisTaskMulti(const char* name = "");
   AliAnalysisTaskMulti(const AliAnalysisTaskMulti& obj);
   AliAnalysisTaskMulti& operator=(const AliAnalysisTaskMulti& other);

   // Implementation of interface methods
   virtual void   ConnectInputData(Option_t *option = "");
   virtual void   Exec(Option_t* option);

   // new mix interface
   virtual void   UserExecMix(Option_t *option = "");

   void      SetCurrentBinIndex(Int_t const index) { fCurrentBinIndex = index; }
   void      SetCurrentEntry(Long64_t const entry) { fCurrentEntry = entry ; }
   void      SetCurrentEntryMain(Long64_t const entry) { fCurrentEntryMain = entry ; }
   void      SetCurrentEntryMix(Long64_t const entry) { fCurrentEntryMix = entry ; }
   void      SetNumberMixed(Int_t const index) { fNumberMixed = index; }

   Int_t     CurrentBinIndex() const { return fCurrentBinIndex; }
   Long64_t  CurrentEntry() const { return fCurrentEntry; }
   Long64_t  CurrentEntryMain() const { return fCurrentEntryMain; }
   Long64_t  CurrentEntryMix() const { return fCurrentEntryMix; }
   Int_t     NumberMixed() const { return fNumberMixed; }

protected:
   Long64_t fCurrentEntry;       //! current entry number (adds 1 for every event processed on each worker)
   Long64_t fCurrentEntryMain;   //! current entry in chain of processed files
   Long64_t fCurrentEntryMix;    //! current mixed entry in chain of processed files
   Int_t    fCurrentBinIndex;    //! current bin index
   Int_t    fNumberMixed;        //! number mixed

   AliMultiInputEventHandler *fMultiInputHandler;  //! pointer to multihandler
   AliMCEventHandler *fMCEventHandler;             //! pointer to MCEventHandler

   void ConnectMultiHandler();
   void DisconnectMultiHandler();

   ClassDef(AliAnalysisTaskMulti, 1); // Analysis task for standard jet analysis
};

#endif
