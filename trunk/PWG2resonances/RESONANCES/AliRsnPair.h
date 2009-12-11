//
// *** Class AliRsnPair ***
//
// TODO
//
// authors: A. Pulvirenti (email: alberto.pulvirenti@ct.infn.it)
//          M. Vala (email: martin.vala@cern.ch)
//

#ifndef ALIRSNPAIR_H
#define ALIRSNPAIR_H

#include "TClonesArray.h"

#include "AliRsnDaughter.h"
#include "AliRsnPairDef.h"
#include "AliRsnPairParticle.h"

class TH1;
class TH2;
class TList;
class TArrayI;

class AliRsnEvent;
class AliRsnCutMgr;
class AliRsnPIDIndex;
class AliRsnFunction;

class AliRsnPair : public TObject
{
  public:

    enum EPairType {
      kNoPID = 0,    kNoPIDMix,
      kRealisticPID, kRealisticPIDMix,
      kPerfectPID,   kPerfectPIDMix,
      kPairTypes
    };

    AliRsnPair(EPairType type = kRealisticPID, AliRsnPairDef *def = 0);
    ~AliRsnPair();

    void    SetOnlyTrue(Bool_t onlyTrue = kTRUE) {fOnlyTrue = onlyTrue;}

    void    Print(Option_t *option = "") const;
    void    LoopPair(AliRsnPIDIndex*const pidIndex1, AliRsnEvent*const ev1, AliRsnPIDIndex*const pidIndex2 = 0, AliRsnEvent* ev2 = 0);
    void    LoopPair(TArrayI *a1, TArrayI *a2, AliRsnEvent *ev1, AliRsnEvent *ev2 = 0);
    void    SetCutMgr(AliRsnCutMgr* const mgr) { fCutMgr = mgr; }
    void    AddFunction(AliRsnFunction*const fcn);
    TList*  GenerateHistograms(TString prefix = "", TList *list=0);

    Bool_t  IsMixed() const {return fIsMixed;}
    Bool_t  IsPairEqual() const {
      if (fPIDMethod == AliRsnDaughter::kNoPID) return (fPairDef->IsLikeSign());
      else return (fPairDef->IsLikeSign() && fPairDef->HasEqualTypes());
    }

    TString GetPairTypeName(EPairType type) const;
    TString GetPairName() const;
    TString GetPairHistName(AliRsnFunction *const fcn, TString text = "") const;
    TString GetPairHistTitle(AliRsnFunction *const fcn, TString text = "") const;

  private:

    AliRsnPair(const AliRsnPair &copy) : TObject(copy),
        fOnlyTrue(kFALSE),fIsMixed(kFALSE),fPairType(kPairTypes),fPIDMethod(AliRsnDaughter::kRealistic),
        fPairDef(0x0),fCutMgr(0x0),fFunctions("AliRsnFunction",0),fTrack1(),fTrack2(),fPairParticle() {}
    AliRsnPair& operator=(const AliRsnPair&) {return *this;}

    void     SetUp(EPairType type);
    void     SetAllFlags(AliRsnDaughter::EPIDMethod pid, Bool_t mix) {fPIDMethod = pid; fIsMixed = mix;}

    Bool_t   CutPass(AliRsnDaughter *d);
    Bool_t   CutPass(AliRsnPairParticle *p);
    Bool_t   CutPass(AliRsnEvent *e);

    Bool_t                      fOnlyTrue;       // select true pairs only?
    Bool_t                      fIsMixed;        // doing event-mixing ?
    EPairType                   fPairType;       // pair type (PID + mixing or not)
    AliRsnDaughter::EPIDMethod  fPIDMethod;      // pid type variable for single track

    AliRsnPairDef              *fPairDef;        // pair definition (particles, charges)
    AliRsnCutMgr               *fCutMgr;         // cut manager
    TClonesArray                fFunctions;      // functions
    AliRsnDaughter              fTrack1;         // track #1 (external loop)
    AliRsnDaughter              fTrack2;         // track #2 (internal loop)
    AliRsnPairParticle          fPairParticle;   // track pair

    ClassDef(AliRsnPair, 2)
};

#endif
