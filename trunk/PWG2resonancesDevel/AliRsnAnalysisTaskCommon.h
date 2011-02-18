#ifndef ALIRSNANALYSISTASKCOMMON_H
#define ALIRSNANALYSISTASKCOMMON_H

class TList;

#ifndef ALIANALYSISTASKSE_H
#include "AliAnalysisTaskSE.h"
#endif

class AliRsnAnalysisTaskCommon : public AliAnalysisTaskSE {
public:
   AliRsnAnalysisTaskCommon();
   AliRsnAnalysisTaskCommon(const char *name);
   virtual ~AliRsnAnalysisTaskCommon();

   virtual void     UserCreateOutputObjects();
   virtual void     UserExec(Option_t *option);
   virtual void     UserExecMix(Option_t*);
   virtual void     Terminate(Option_t *);

private:
   TList           *fOutput;        // Output list

   AliRsnAnalysisTaskCommon(const AliRsnAnalysisTaskCommon&); // not implemented
   AliRsnAnalysisTaskCommon& operator=(const AliRsnAnalysisTaskCommon&); // not implemented

   ClassDef(AliRsnAnalysisTaskCommon, 1); // AliRsnAnalysisTaskCommon
};

#endif

