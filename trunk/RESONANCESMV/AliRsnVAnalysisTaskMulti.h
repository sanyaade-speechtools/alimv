//
// Class AliRsnVAnalysisTaskMulti
//
// Virtual Class derivated from AliAnalysisTaskMulti which will be base class
// for all RSN Multi tasks
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//

#ifndef ALIRSNVANALYSISTASKMULTI_H
#define ALIRSNVANALYSISTASKMULTI_H

#include <TH1.h>

#include "AliAnalysisTaskMulti.h"

#include "AliRsnEvent.h"
#include "AliRsnVATProcessInfo.h"

class AliESDEvent;
class AliAODEvent;
class AliMCEvent;

class AliRsnVAnalysisTaskMulti : public AliAnalysisTaskMulti
{
  public:
  
    AliRsnVAnalysisTaskMulti(const char *name = "AliRsnVAnalysisTaskMulti", Bool_t mcOnly = kFALSE);
    AliRsnVAnalysisTaskMulti(const AliRsnVAnalysisTaskMulti& copy);
    AliRsnVAnalysisTaskMulti& operator= (const AliRsnVAnalysisTaskMulti& /*copy*/) { return *this; }
    virtual ~AliRsnVAnalysisTaskMulti() {/* Does nothing*/;}

    // basic interface methods
    virtual void    LocalInit();
    virtual Bool_t  UserNotify();
    virtual void    ConnectInputData(Option_t *opt);
    virtual void    UserCreateOutputObjects();
    virtual void    UserExec(Option_t* opt);
		virtual void    UserExecMix(Option_t* option = "");
    virtual void    Terminate(Option_t* opt);

    // customized methods (to be implemented in derived classes)
    virtual void    RsnUserCreateOutputObjects();
    virtual void    RsnUserExec(Option_t*);
    virtual void    RsnUserExecMix(Option_t*);
    virtual void    RsnTerminate(Option_t*);

    // event pre-processing functions
    virtual Bool_t  EventProcess();

    // getters
    AliRsnEvent*           GetRsnEvent(Int_t i=0) {return &fRsnEvent[i];}
    AliRsnVATProcessInfo*  GetInfo()     {return &fTaskInfo;}

    // setters
    void SetMCOnly(Bool_t mcOnly = kTRUE)                           {fMCOnly = mcOnly;}
    void SetPrintInfoNumber(const Long64_t &num = 100)              {fTaskInfo.SetPrintInfoNumber(num);}
    
    void SetMixing(Bool_t doMix = kTRUE) {fIsMixing = doMix;}
    Bool_t IsMixing() { return fIsMixing;}

  protected:

    AliESDEvent            *fESDEvent[2];         //  ESD event
    AliMCEvent             *fMCEvent[2];          //  MC event
    AliAODEvent            *fAODEventIn[2];       //  AOD event from input
    AliAODEvent            *fAODEventOut[2];      //  AOD event from output from previous taks

    Bool_t                  fIsMixing;        // flag is using mixing
    Bool_t                  fMCOnly;           //  use only MC information
    AliRsnEvent             fRsnEvent[2];         //  interface to event for RSN package

    TList                  *fInfoList;         //! output list for informations
    AliRsnVATProcessInfo    fTaskInfo;         //  task info

    void                    SetupMixingEvents();

    ClassDef(AliRsnVAnalysisTaskMulti, 1)
};

#endif
