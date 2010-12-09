#ifndef ALIANALYSISTASKMULTI_H
#define ALIANALYSISTASKMULTI_H
 
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

#include "AliAnalysisTaskSE.h"

class AliAnalysisTaskMulti : public AliAnalysisTaskSE
{
 public:
    AliAnalysisTaskMulti(const char* name="");
    AliAnalysisTaskMulti(const AliAnalysisTaskMulti& obj);
    AliAnalysisTaskMulti& operator=(const AliAnalysisTaskMulti& other);
    virtual ~AliAnalysisTaskMulti() {;}
    
    // Implementation of interface methods
    virtual void   ConnectInputData(Option_t *option = "");
    virtual void   Exec(Option_t* option);

    // new mix interface
    virtual void   UserExecMix(Option_t *option = "");
    
    void      SetCurrentBinIndex(Int_t const index) {fCurrentBinIndex = index ;}
    void      SetCurrentEntry(Long64_t const entry) {fCurrentEntry = entry ;}
    void      SetCurrentEntryMain(Long64_t const entry) {fCurrentEntryMain = entry ;}
    void      SetCurrentEntryMix(Long64_t const entry) {fCurrentEntryMix = entry ;}
    void      SetNumberMixed(Int_t const index) {fNumberMixed = index ;}
    
    Int_t     CurrentBinIndex() { return fCurrentBinIndex;}
    Long64_t  CurrentEntry() { return fCurrentEntry;}
    Long64_t  CurrentEntryMain() { return fCurrentEntryMain;}
    Long64_t  CurrentEntryMix() { return fCurrentEntryMix;}
    Int_t     NumberMixed() { return fNumberMixed;}
    
protected: 
  Long64_t fCurrentEntry;
  Long64_t fCurrentEntryMain;
  Long64_t fCurrentEntryMix;
  Int_t    fCurrentBinIndex;
  Int_t    fNumberMixed;
    
    ClassDef(AliAnalysisTaskMulti, 1); // Analysis task for standard jet analysis
};
 
#endif
