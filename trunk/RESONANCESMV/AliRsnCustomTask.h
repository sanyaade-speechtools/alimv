//
// Class AliRsnCustomTask
//
// AliRsnCustomTask is task u can customize for your needs
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//
#ifndef ALIRSNCUSTOMTASK_H
#define ALIRSNCUSTOMTASK_H

#include <TObjArray.h>
#include "AliRsnVCustomObj.h"

#include "AliRsnVAnalysisTaskMulti.h"

class AliRsnCustomTask : public AliRsnVAnalysisTaskMulti
{
  public:
  
    AliRsnCustomTask(const char *name = "AliRsnCustomTask", Bool_t useKine = kFALSE);
    AliRsnCustomTask(const AliRsnCustomTask& copy);
    AliRsnCustomTask& operator=(const AliRsnCustomTask& copy);
    virtual ~AliRsnCustomTask() {}

    virtual void            RsnUserCreateOutputObjects();
    virtual void            RsnUserExec(Option_t*);
		virtual void  					RsnUserExecMix(Option_t* );
    virtual void            RsnTerminate(Option_t*);
    
    void AddCustomObject(AliRsnVCustomObj*obj) { fInputObjects.Add(obj);}

  private:

		TList                  *fOutList;                 // list of output events
		TObjArray               fInputObjects;            // list of input objects

    ClassDef(AliRsnCustomTask, 1)
};

#endif
