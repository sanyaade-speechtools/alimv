#ifndef ALIANALYSISTASKCUSTOMMIX_H
#define ALIANALYSISTASKCUSTOMMIX_H

// example of an analysis task creating a p_t spectrum (it can use ESD and AOD input)
// Authors: Matin.Vala@cern.ch

#include "AliLog.h"

#include "AliAnalysisTaskMulti.h"

class TList;
class AliMixInfo;
class AliAnalysisTaskMixInfo : public AliAnalysisTaskMulti
{
public:
    AliAnalysisTaskMixInfo(const char *name="<default name>");
    virtual ~AliAnalysisTaskMixInfo();

    virtual void    LocalInit();
    virtual void    UserCreateOutputObjects();
    virtual void    UserExec(Option_t *option);
    virtual void    Terminate(Option_t *);
    virtual void    UserExecMix(Option_t *option = "");
    virtual void    FinishTaskOutput();

    void            InitMixInfo();
    // sets log type to list of classes
    void            SetLogType(AliLog::EType_t type, TString allClasses = "");
    // sets correctly debug level to AliLog for all classes listed in fLogClassesString
    void            SetDebugForAllClasses();

private:

    TList       *fOutputList;     //! output list
    AliMixInfo  *fMixInfo;        //!

    Long64_t    fCurrentEntryTmp;

    AliLog::EType_t     fLogType;           // log type
    TString             fLogClassesString;  // all classes string divided with ":"

    AliAnalysisTaskMixInfo(const AliAnalysisTaskMixInfo &); // not implemented
    AliAnalysisTaskMixInfo &operator=(const AliAnalysisTaskMixInfo &); // not implemented

    ClassDef(AliAnalysisTaskMixInfo, 1); // example of analysis
};

#endif
