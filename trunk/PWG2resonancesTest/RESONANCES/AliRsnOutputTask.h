#ifndef ALIRSNOUTPUTTASK_H
#define ALIRSNOUTPUTTASK_H

#include <TH1.h>
#include "AliRsnFileInfo.h"
#include "AliAnalysisTask.h"
#include "AliRsnOutputManager.h"

class AliRsnOutputTask : public AliAnalysisTask
{
  public:
    AliRsnOutputTask(const char *name = "AliRsnOutputTask",Bool_t useListInput=kTRUE);
    virtual ~AliRsnOutputTask() {}

    virtual void  ConnectInputData(Option_t *);
    virtual void  CreateOutputObjects();
    virtual void  Exec(Option_t *option);
    virtual void  Terminate(Option_t *);

    AliRsnOutputManager *GetOutputManager() { return &fOutputManager;};
    void SetOutputManagerName(const char *name) {fOutputManager.SetName(name);}
    void SetTrainInfoFileName(TString str="AliProofAnalysisTrainInfo.root") {fTrainInfoFileName = str;}
    void SetRsnInfoFileName(TString str="RSNAnalysisInfo.root") {fRsnInfoFileName = str;}
    void SetAnalysisFileName(TString str="RSNAnalysis0.root") {fRsnAnalysisFileName = str;}
    void   ImportTxt(TString anaFile,TString txt="",TString rootOut="RSNFileInfoTreeTmp.root");
  private:
    
    Bool_t          fUseListInput;
    
    AliRsnOutputManager fOutputManager;
    
    AliRsnFileInfo  *fFileInfo;
    TList           *fInputList;
    TList           *fOutputList;
    
    TH1I            *fProcessInfoHist;

    TString         fTrainInfoFileName;
    TString         fRsnInfoFileName;
    TString         fRsnAnalysisFileName;

    
    void DoProcessInfo();
    void DoSumSubDiv();

    AliRsnOutputTask(const AliRsnOutputTask&); // not implemented
    AliRsnOutputTask& operator=(const AliRsnOutputTask&); // not implemented

    ClassDef(AliRsnOutputTask, 1);

};

#endif
