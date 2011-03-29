#ifndef ALIRSNANALYSISTASKCOMMON_H
#define ALIRSNANALYSISTASKCOMMON_H

#ifndef ALIANALYSISTASKSE_H
#include "AliAnalysisTaskSE.h"
#endif

class TList;
class AliMixInputEventHandler;
class AliMultiInputEventHandler;
class AliRsnAnalysisTaskCommon : public AliAnalysisTaskSE {
public:
   AliRsnAnalysisTaskCommon();
   AliRsnAnalysisTaskCommon(const char *name);
   virtual ~AliRsnAnalysisTaskCommon();

   virtual void     UserCreateOutputObjects();
   virtual void     UserExec(Option_t *option);
   virtual void     UserExecMix(Option_t*);
   virtual void     Terminate(Option_t *);
   void InitInputHandlers();
private:
   TList           *fOutput;        // Output list

   AliMultiInputEventHandler  *fInputEHMain;       //! input multi handler
   AliMixInputEventHandler    *fInputEHMix;        //! mix input handler

   AliRsnAnalysisTaskCommon(const AliRsnAnalysisTaskCommon&); // not implemented
   AliRsnAnalysisTaskCommon& operator=(const AliRsnAnalysisTaskCommon&); // not implemented

   ClassDef(AliRsnAnalysisTaskCommon, 1); // AliRsnAnalysisTaskCommon
};

#endif

