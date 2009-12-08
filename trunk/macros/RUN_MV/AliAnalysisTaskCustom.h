#ifndef ALIANALYSISTASKCUSTOM_H
#define ALIANALYSISTASKCUSTOM_H

// example of an analysis task creating a p_t spectrum
// Authors: Panos Cristakoglou, Jan Fiete Grosse-Oetringhaus, Christian Klein-Boesing

class TList;
class TH1F;

#include "AliAnalysisTaskSE.h"

class AliAnalysisTaskCustom : public AliAnalysisTaskSE {
 public:
  AliAnalysisTaskCustom(const char *name="<default name>");
  virtual ~AliAnalysisTaskCustom() {}
  
  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t* option);
  virtual void   Terminate(Option_t *);

  
  void LoopESD();
  void LoopESDMC();
  
  void LoopAOD() {;};
  void LoopAODMC() {;};
  
 private:
  TList	      *fOutputList;
  
  TH1F        *fHistPtESD;	//Pt spectrum ESD
  TH1F        *fHistPtMC;	//Pt spectrum MC
   
  AliAnalysisTaskCustom(const AliAnalysisTaskCustom&); // not implemented
  AliAnalysisTaskCustom& operator=(const AliAnalysisTaskCustom&); // not implemented
  
  ClassDef(AliAnalysisTaskCustom, 1); // example of analysis
};

#endif
