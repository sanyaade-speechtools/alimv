//
// Class AliRsnParticleInfoManager
// author: M. Vala (email: martin.vala@cern.ch)
//

#ifndef ALIRSNPARTICLEINFOMANAGER_H
#define ALIRSNPARTICLEINFOMANAGER_H

#include <TList.h>

#include "AliRsnVManager.h"
#include "AliMCEvent.h"
#include "AliRsnParticleInfo.h"

class AliRsnParticleInfoManager : public AliRsnVManager
{
  public:

    AliRsnParticleInfoManager(const char *name = "defaultPairMgr");
    virtual ~AliRsnParticleInfoManager() {;};

    //virtual void   Add(AliRsnPair *pair);
    virtual void   Add(TObject *pair);
    virtual void   AddPair(AliRsnParticleInfo *pair);
    virtual void   PrintArray() const;
    virtual void   Print(Option_t *option = "") const;

    void   InitAllParticleInfos(TList* list);
    void   ProcessAllParticleInfos(AliMCEvent* ev);

  private:

    ClassDef(AliRsnParticleInfoManager, 1)
};

#endif
