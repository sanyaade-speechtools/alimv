//
// Class AliRsnAnalysisMETest
//
// Virtual Class derivated from AliRsnVAnalysisTaskME which will be base class
// for all RSN SE tasks
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//
#ifndef ALIRSNANALYSISMETEST_H
#define ALIRSNANALYSISMETEST_H

#include "AliPID.h"
#include "AliRsnVAnalysisTaskME.h"
#include "AliRsnAnalysisManager.h"
#include "AliRsnEvent.h"
#include "AliRsnPIDIndex.h"

class AliPID;
class AliESDtrackCuts;
class AliRsnAnalysisMETest : public AliRsnVAnalysisTaskME
{

  public:
    AliRsnAnalysisMETest(const char *name = "AliRsnAnalysisMETest");
    AliRsnAnalysisMETest(const AliRsnAnalysisMETest& copy);
    virtual ~AliRsnAnalysisMETest() {;};

    // Implement this
    virtual void    RsnUserCreateOutputObjects();
    virtual void    RsnUserExec(Option_t*);
    virtual void    RsnTerminate(Option_t*);

    AliRsnAnalysisManager *GetAnalysisManager(TString name="");
    void SetAnalysisManagerName(const char*name) { fRsnAnalysisManager.SetName(name);};

    // Prior probs
    void            SetPriorProbability(AliPID::EParticleType type, Double_t p);
    void            DumpPriors();
    void            GetPriorProbability(Double_t *out)const;

  private:

    AliRsnAnalysisMETest& operator=(const AliRsnAnalysisMETest& /*copy*/) {return *this;}

    AliRsnAnalysisManager fRsnAnalysisManager;      // analysis main engine
    AliRsnPIDIndex        fPIDIndex;                // utility --> PID sorter
    AliRsnPIDIndex        fPIDIndexMix;             // utility --> PID sorter (mixed event)
    AliRsnEvent           fEvent;                   // utility --> event interface
    AliRsnEvent           fEventMix;                // utility --> event interface (mixed event)

    Double_t              fPrior[AliPID::kSPECIES]; // prior probabilities

    ClassDef(AliRsnAnalysisMETest, 1)
};

#endif
