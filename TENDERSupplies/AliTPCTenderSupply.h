#ifndef ALITPCTENDERSUPPLY_H
#define ALITPCTENDERSUPPLY_H

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////////////////////////////
//                                                                    //
//  TPC tender, reapply pid on the fly                                //
//                                                                    //
////////////////////////////////////////////////////////////////////////



#include <AliTenderSupply.h>

class TObjArray;
class AliESDpid;
class AliSplineFit;
class AliGRPObject;

class AliTPCTenderSupply: public AliTenderSupply {

public:
   AliTPCTenderSupply();
   AliTPCTenderSupply(const char *name, const AliTender *tender = NULL);

   virtual ~AliTPCTenderSupply() {;}

   void SetGainCorrection(Bool_t gainCorr) {fGainCorrection = gainCorr;}
   void SetDebugLevel(Int_t level)         {fDebugLevel = level;}
   void SetMip(Double_t mip)               {fMip = mip;}
   void SetResponseFunctions(TObjArray *arr) {fArrPidResponseMaster = arr;}

   virtual void              Init();
   virtual void              ProcessEvent();

private:
   AliESDpid          *fESDpid;       //! ESD pid object
   AliSplineFit       *fGainNew;      //! New gain correction
   AliSplineFit       *fGainOld;      //! Old gain correction

   Bool_t fGainCorrection;            //Perform gain correction
   Bool_t fPcorrection;               //!Perform pressure correction
   TObjArray *fArrPidResponseMaster;  //array with gain curves

   Int_t fDebugLevel;                 //debug level
   Double_t fMip;                     //mip position

   AliGRPObject *fGRP;                //!GRP for pressure temperature correction

   void SetSplines();
   Double_t GetGainCorrection();

   void SetParametrisation();

   AliTPCTenderSupply(const AliTPCTenderSupply&c);
   AliTPCTenderSupply& operator= (const AliTPCTenderSupply&c);

   ClassDef(AliTPCTenderSupply, 1);  // TPC tender task
};


#endif

