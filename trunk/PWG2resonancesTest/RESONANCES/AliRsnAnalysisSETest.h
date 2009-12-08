//
// Class AliRsnAnalysisSETest
//
// Virtual Class derivated from AliRsnVAnalysisTaskSE which will be base class
// for all RSN SE tasks
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//
#ifndef ALIRSNANALYSISSETEST_H
#define ALIRSNANALYSISSETEST_H

#include "AliPID.h"
// #include "AliRsnAnalysisManager.h"

#include "AliRsnPIDIndex.h"
#include "AliRsnEvent.h"

#include "AliRsnAnalysisSE.h"

// class AliRsnPIDDefESD;
// class AliRsnCutSet;

class AliRsnAnalysisSETest : public AliRsnAnalysisSE
{

  public:
    AliRsnAnalysisSETest(const char *name = "AliRsnAnalysisSETest");
    AliRsnAnalysisSETest(const AliRsnAnalysisSETest& copy);
    virtual ~AliRsnAnalysisSETest() {;};

    // Implement this
    virtual void    RsnUserCreateOutputObjects();
    virtual void    RsnUserExec(Option_t*);
    virtual void    RsnTerminate(Option_t*);

  private:

    AliRsnAnalysisSETest& operator=(const AliRsnAnalysisSETest& /*copy*/) {return *this;}

    ClassDef(AliRsnAnalysisSETest, 1)
};

#endif
