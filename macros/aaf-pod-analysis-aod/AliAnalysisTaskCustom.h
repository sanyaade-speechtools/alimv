#ifndef ALIANALYSISTASKCUSTOM_H
#define ALIANALYSISTASKCUSTOM_H

// example of an analysis task creating a p_t spectrum (it can use ESD and AOD input)
// Authors: Matin.Vala@cern.ch

class TList;
class TH1F;
class AliESDEvent;
class AliAODEvent;

#include "AliAnalysisTaskSE.h"

class AliAnalysisTaskCustom : public AliAnalysisTaskSE {
 public:
  AliAnalysisTaskCustom(const char *name="<default name>");
  virtual ~AliAnalysisTaskCustom();
  
  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t* option);
  virtual void   Terminate(Option_t *);

  void LoopESD();
  void LoopESDMC();
  void LoopAOD();
  void LoopAODMC();
  
  void UseMC(Bool_t useMC=kTRUE) { fUseMC = useMC;}
  
 private:
  Bool_t       fUseMC;
  AliESDEvent *fMyESDEvent;     //! esd event
  AliAODEvent *fMyAODEvent;     //! aod event
  
  TList	      *fOutputList;     //! output list
  TH1F        *fHistPt;         //!Pt spectrum ESD
  TH1F        *fHistPtMC;       //!Pt spectrum MC
   
  AliAnalysisTaskCustom(const AliAnalysisTaskCustom&); // not implemented
  AliAnalysisTaskCustom& operator=(const AliAnalysisTaskCustom&); // not implemented
  
  ClassDef(AliAnalysisTaskCustom, 1); // example of analysis
};

#endif
