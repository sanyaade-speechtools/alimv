#ifndef ALIANALYSISTASKCUSTOMMIXOLD_H
#define ALIANALYSISTASKCUSTOMMIXOLD_H

// example of an analysis task creating a p_t spectrum (it can use ESD and AOD input)
// Authors: Matin.Vala@cern.ch

#include "AliLog.h"

#include "AliAnalysisTaskSE.h"

class TList;
class AliMixInfo;
class AliAnalysisTaskCustomMixOld : public AliAnalysisTaskSE {
public:
   AliAnalysisTaskCustomMixOld(const char *name = "<default name>");
   virtual ~AliAnalysisTaskCustomMixOld();

   virtual void    UserCreateOutputObjects();
   virtual void    UserExec(Option_t *option);
   virtual void    Terminate(Option_t *);
//    virtual void    UserExecMix(Option_t *option = "");

   virtual void    LocalInit();

   // sets log type to list of classes
   void            SetLogType(AliLog::EType_t type, TString allClasses = "");
   // sets correctly debug level to AliLog for all classes listed in fLogClassesString
   void            SetDebugForAllClasses();

private:

   TList       *fOutputList;     //! output list
   AliLog::EType_t     fLogType;           // log type
   TString             fLogClassesString;  // all classes string divided with ":"

   AliAnalysisTaskCustomMixOld(const AliAnalysisTaskCustomMixOld &); // not implemented
   AliAnalysisTaskCustomMixOld &operator=(const AliAnalysisTaskCustomMixOld &); // not implemented

   ClassDef(AliAnalysisTaskCustomMixOld, 1); // example of analysis
};

#endif
