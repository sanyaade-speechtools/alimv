//
// Class AliMixEventInputHandler
//
// Mixing input handler prepare N events before UserExec
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIMIXEVENTINPUTHANDLER_H
#define ALIMIXEVENTINPUTHANDLER_H

#include <TObjArray.h>
#include <TArrayI.h>

#include "AliMultiInputEventHandler.h"

class TChain;
class AliMixEventPool;
class AliMixInputHandlerInfo;
class AliInputEventHandler;
class AliMixInputEventHandler : public AliMultiInputEventHandler
{

public:
    AliMixInputEventHandler(const Int_t size=1,const Int_t mixNum=1);
    virtual ~AliMixInputEventHandler() {;};

    // From the interface
    virtual Bool_t  Init(Option_t *opt) {return AliMultiInputEventHandler::Init(opt);}
    virtual Bool_t  Init(TTree *tree, Option_t *opt);
    virtual Bool_t  Notify();
    virtual Bool_t  Notify(const char *path);
    virtual Bool_t  BeginEvent(Long64_t entry);
    virtual Bool_t  GetEntry();
    virtual Bool_t  FinishEvent();

    // removing default impementation
    virtual void            AddInputEventHandler(const AliVEventHandler *const /*inHandler*/);

    void                    SetInputHandlerForMixing(const AliInputEventHandler *const inHandler);
    void                    SetEventPool(AliMixEventPool *const evPool) {fEventPool = evPool;}

    AliInputEventHandler   *InputEventHandler(const Int_t index);
    AliMixEventPool        *GetEventPool() const { return fEventPool;}
    Int_t                   BufferSize() const {return fBufferSize;}
    Int_t                   NumberMixedTimes() const {return fNumberMixed;}
    Int_t                   MixNumber() const {return fMixNumber;}
    Long64_t                EntryAll() const { return fEntryCounter;}

    void                    UseDefaultProcess(Bool_t b=kTRUE) { fUseDefautProcess = b; }
    void                    UsePreMixEvents(Bool_t b=kTRUE) { fUsePreMixEvents = b; }

    void                    SetMixNumber(const Int_t mixNum);

protected:

    TObjArray               fMixTrees;              // buffer of input handlers
    TArrayI                 fTreeMap;               // tree map
    AliMixInputHandlerInfo *fMixIntupHandlerInfoTmp;//! mix input handler info full chain
    Long64_t                fEntryCounter;          // entry counter
    AliMixEventPool        *fEventPool;             // event pool
    Int_t                   fNumberMixed;           // number of mixed events with current event
    Int_t                   fMixNumber;             // user's mix number request

private:

    Bool_t                  fUseDefautProcess;
    Bool_t                  fUsePreMixEvents;

    virtual Bool_t          MixStd();
    virtual Bool_t          MixBuffer();
    virtual Bool_t          MixEventsMoreTimesWithOneEvent();
    virtual Bool_t          MixEventsMoreTimesWithBuffer();

    void                    UserExecMixAllTasks(Long64_t entryCounter, Int_t idEntryList, Long64_t entryMainReal, Long64_t entryMixReal, Int_t numMixed);

    AliMixInputEventHandler(const AliMixInputEventHandler &handler);
    AliMixInputEventHandler &operator=(const AliMixInputEventHandler &handler);

    ClassDef(AliMixInputEventHandler, 1)
};

#endif
