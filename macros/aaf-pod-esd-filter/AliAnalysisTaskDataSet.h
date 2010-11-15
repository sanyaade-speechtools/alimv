#ifndef ALIANALYSISTASKCUSTOM_H
#define ALIANALYSISTASKCUSTOM_H

// create DS
// Authors: Matin.Vala@cern.ch

class TList;
class TFileCollection;

#include "AliAnalysisTaskSE.h"

class AliAnalysisTaskDataSet : public AliAnalysisTaskSE {
 public:
  AliAnalysisTaskDataSet(const char *name="<default name>");
  virtual ~AliAnalysisTaskDataSet();
  
  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t* option);
  virtual void   Terminate(Option_t *);

 private:
  TList	      		*fOutputList;     //! output list
  TFileCollection *fDataSetColl;    //!DS
  TTree           *fOutTree;
  TObjString      *fPath;

  Long64_t        fCounter;
   
  AliAnalysisTaskDataSet(const AliAnalysisTaskDataSet&); // not implemented
  AliAnalysisTaskDataSet& operator=(const AliAnalysisTaskDataSet&); // not implemented
  
  ClassDef(AliAnalysisTaskDataSet, 1); // example of analysis
};

#endif
