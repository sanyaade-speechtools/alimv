//
// Class AliRsnParticleInfoSE
//
// Virtual Class derivated from AliRsnVAnalysisTaskSE which will be base class
// for all RSN SE tasks
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//
#ifndef ALIRSNPARTICLEINFOSE_H
#define ALIRSNPARTICLEINFOSE_H

#include "AliRsnVAnalysisTaskSE.h"
#include "AliRsnParticleInfoManager.h"

class AliRsnParticleInfoSE : public AliRsnVAnalysisTaskSE
{

  public:
    AliRsnParticleInfoSE(const char *name = "AliRsnParticleInfoSE",Int_t numOfOutputs=1);
    AliRsnParticleInfoSE(const AliRsnParticleInfoSE& copy);
    virtual ~AliRsnParticleInfoSE() {;};

    // Implement this
    virtual void    RsnUserCreateOutputObjects();
    virtual void    RsnUserExec(Option_t*);
    virtual void    RsnTerminate(Option_t*);
    
    AliRsnParticleInfoManager *GetParticleManager(Int_t index = 0, TString name = "");
    void SetParticleManagerName(const char *name, Int_t index = 0) {fParticleManager[index].SetName(name);}

  private:

    AliRsnParticleInfoManager fParticleManager[10];
    
    AliRsnParticleInfoSE& operator=(const AliRsnParticleInfoSE& /*copy*/) {return *this;}
    
    void MCLoop(Int_t outputIndex=0);
    void ESDLoop(Int_t outputIndex=0);

    ClassDef(AliRsnParticleInfoSE, 1)
};

#endif
