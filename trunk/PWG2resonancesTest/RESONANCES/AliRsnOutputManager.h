//
// Class AliRsnOutputManager
// author: M. Vala (email: martin.vala@cern.ch)
//

#ifndef ALIRSNOUTPUTMANAGER_H
#define ALIRSNOUTPUTMANAGER_H

#include <TList.h>
#include <TFile.h>

#include "AliRsnVManager.h"
#include "AliRsnOutputObj.h"

class AliRsnOutputManager : public AliRsnVManager
{
  public:

    AliRsnOutputManager(const char *name = "defaultPairMgr");
    virtual ~AliRsnOutputManager() {;};

    //virtual void   Add(AliRsnPair *pair);
    virtual void   Add(TObject *pair);
    virtual void   AddPair(AliRsnOutputObj *pair);
    virtual void   PrintArray() const;
    virtual void   Print(Option_t *option = "") const;

    void    InitAll(TList* list);
    void    InitAllExtra(TList* list);
    void    ProcessAll(TFile* file,TList *list=0,TH1I *processInfoHist=0);
    void    ProcessAllExtra(TList *list);

  private:

    ClassDef(AliRsnOutputManager, 1)


};

#endif
