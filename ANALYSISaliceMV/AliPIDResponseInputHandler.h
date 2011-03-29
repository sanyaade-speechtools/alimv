//
// Class AliPIDResponseInputHandler
//
// AliPIDResponseInputHandler
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIPIDRESPONSEINPUTHANDLER_H
#define ALIPIDRESPONSEINPUTHANDLER_H

#include "AliInputEventHandler.h"
class AliPIDResponse;
class TObjArray;
class AliPIDResponseInputHandler : public AliInputEventHandler {

public:
   AliPIDResponseInputHandler(const char *name = "PIDResoponseIH");
   virtual ~AliPIDResponseInputHandler();

   // From the interface
   virtual Bool_t  Init(Option_t *opt);
   virtual Bool_t  Init(TTree *tree, Option_t *opt);
   virtual Bool_t  BeginEvent(Long64_t entry);
   virtual Bool_t  FinishEvent();
   virtual Bool_t  Notify();
   virtual Bool_t  Notify(const char *path);
   virtual Bool_t  GetEntry();

   void ExecNewRun();
   void SetRecoInfo();
   void SetITSParametrisation();
   void SetTPCPidResponseMaster();
   void SetTPCParametrisation();

private:

   Bool_t  fIsMC;                       // flag for MC
   Int_t   fTOFTimeZeroTypeUser;        //  start time type for tof (ESD)
   Int_t   fTOFTimeZeroType;            //! default start time type for tof (ESD)
   Float_t fTOFres;                     //  TOF resolution

   AliPIDResponse *fPIDResponse;        //! PID response Handler

   TString fBeamType;                   //! beam type (PP) or (PBPB)
   TString fLHCperiod;                  //! LHC period
   TString fMCperiodTPC;                //! corresponding MC period to use for the TPC splines
   Int_t   fRecoPass;                   //! reconstruction pass
   Int_t   fRun;                        //! current run number
   Int_t   fOldRun;                     //! current run number

   TObjArray *fArrPidResponseMaster;    //  TPC pid splines

   AliMultiInputEventHandler *fMCurrentMutliIH;

   AliPIDResponseInputHandler(const AliPIDResponseInputHandler& handler);
   AliPIDResponseInputHandler &operator=(const AliPIDResponseInputHandler &handler);

   ClassDef(AliPIDResponseInputHandler, 1)

};

#endif
