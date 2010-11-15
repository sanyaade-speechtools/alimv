#ifndef ALIANALYSISTASKCUSTOMMIX_H
#define ALIANALYSISTASKCUSTOMMIX_H

// example of an analysis task creating a p_t spectrum (it can use ESD and AOD input)
// Authors: Matin.Vala@cern.ch

#include "AliLog.h"

#include "AliAnalysisTaskSE.h"

class TList;
class TH1F;
class AliESDEvent;
class AliAODEvent;
class AliAnalysisTaskCustomMix : public AliAnalysisTaskSE {
 public:
  AliAnalysisTaskCustomMix(const char *name="<default name>");
  virtual ~AliAnalysisTaskCustomMix();
  
  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t* option);
  virtual void   Terminate(Option_t *);

  void LoopESD();
  void LoopESDMC();
  void LoopAOD();
  void LoopAODMC();
  
  void UseMC(Bool_t useMC=kTRUE) { fUseMC = useMC;}

  // sets log type to list of classes
  void            SetLogType(AliLog::EType_t type, TString allClasses = "");
  // sets correctly debug level to AliLog for all classes listed in fLogClassesString
  void            SetDebugForAllClasses();


 private:
  Bool_t       fUseMC;
  AliESDEvent *fMyESDEvent;     //! esd event
  AliAODEvent *fMyAODEvent;     //! aod event
  
  TList	      *fOutputList;     //! output list
  TH1F        *fHistPt;         //!Pt spectrum ESD
  TH1F        *fHistPtMC;       //!Pt spectrum MC
  
  AliLog::EType_t     fLogType;           // log type
  TString             fLogClassesString;  // all classes string divided with ":"
   
  AliAnalysisTaskCustomMix(const AliAnalysisTaskCustomMix&); // not implemented
  AliAnalysisTaskCustomMix& operator=(const AliAnalysisTaskCustomMix&); // not implemented
  
  ClassDef(AliAnalysisTaskCustomMix, 1); // example of analysis
};

#endif
