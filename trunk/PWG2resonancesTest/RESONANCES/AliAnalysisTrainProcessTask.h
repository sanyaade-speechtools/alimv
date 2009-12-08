//
// Class AliAnalysisTrainProcessTask
//
// Virtual Class derivated from AliRsnVAnalysisTaskSE which will be base class
// for all RSN SE tasks
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//
#ifndef ALIANALYSISTRAINPROCESSTASK_H
#define ALIANALYSISTRAINPROCESSTASK_H
#include <TMonaLisaWriter.h>
#include "AliAnalysisTaskSE.h"
class TList;
class TH1I;
class AliAnalysisTrainProcessTask : public AliAnalysisTaskSE
{

  public:
    AliAnalysisTrainProcessTask(const char *name = "AliAnalysisTrainProcessTask");
    AliAnalysisTrainProcessTask(const AliAnalysisTrainProcessTask& copy);
    virtual ~AliAnalysisTrainProcessTask() {;};

    // Implement this
    virtual void    UserCreateOutputObjects();
    virtual void    UserExec(Option_t*);
    virtual void    Terminate(Option_t*);
    virtual void    FinishTaskOutput();
    
    void SendMonalisaMessage(const char* name, const char* message);
    void SendMonalisaMessageDouble(const char* name, Double_t val);
    
    void SetRefreshNumber(Int_t num) {fNumbersOfRefresh = num;}
    
  private:
    
    TMonaLisaWriter   *fMonaLisaWriter;
//     TList             *fMonaLisaValuelist;
    TList             *fOutputList;
    
    TH1I              *fProcessInfoHist;

    Long64_t          fNumberOfEvents;
    Int_t             fNumbersOfRefresh;
    AliAnalysisTrainProcessTask& operator=(const AliAnalysisTrainProcessTask& /*copy*/) {return *this;}
    

    ClassDef(AliAnalysisTrainProcessTask, 1)
};

#endif
