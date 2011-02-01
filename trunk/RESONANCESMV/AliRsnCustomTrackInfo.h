#ifndef ALIRSNCUSTOMTRACKINFO_H
#define ALIRSNCUSTOMTRACKINFO_H

#include <TH1.h>
#include "AliRsnCutSet.h"

#include "AliRsnVCustomObj.h"
class AliESDpid;
class AliTOFT0maker;
class AliTOFcalib;
class AliRsnCustomTrackInfo : public AliRsnVCustomObj {

public:
   enum EType { kP = 0, kPt = 1, kEta = 2, kNumTypes };

   AliRsnCustomTrackInfo(const char* name = "noname", const char* title = "");
   virtual ~AliRsnCustomTrackInfo();

   virtual void UserCreateOutputCustom(TList* list);
   virtual void UserExecCustom(AliRsnEvent* ev1, AliRsnEvent* ev2);
   virtual void UserExecMixCustom(AliRsnEvent* ev1, AliRsnEvent* ev2);

   const char* GetParameterName(Int_t num);
   const char* GetParameterTitle(Int_t num);

   void SetParamaterHistogram(EType type, Int_t nBins, Double_t min, Double_t max);
   void SetCutSet(AliRsnCutSet *cutSet) { fCutSet = cutSet;}
   void SetCharge(Short_t ch) { fCharge = ch; }

   void             RecalculateTOFPid(Bool_t yn = kTRUE)  {fRecalculateTOFPid = yn;}
   void             SetTOFcalibrateESD(Bool_t yn = kTRUE)  {fTOFcalibrateESD = yn;}
   void             SetTOFcorrectTExp(Bool_t yn = kTRUE)  {fTOFcorrectTExp = yn;}
   void             SetTOFuseT0(Bool_t yn = kTRUE)  {fTOFuseT0 = yn;}
   void             SetTOFtuneMC(Bool_t yn = kTRUE)  {fTOFtuneMC = yn;}
   void             SetTOFresolution(Double_t v = 100.0) {fTOFresolution = v;}


protected:

   Bool_t      fUseParameter[kNumTypes];     //  flag if parameter is used
   TH1D       *fParameter[kNumTypes];        //! histograms
   Int_t       fParameterBins[kNumTypes];    //  nBins
   Double_t    fParameterMin[kNumTypes];     //  min
   Double_t    fParameterMax[kNumTypes];     //  max

   Short_t                fCharge;

   AliRsnCutSet          *fCutSet;           //  cut set

   Bool_t                 fRecalculateTOFPid;
   AliESDpid             *fESDpid;           //! PID manager
   AliTOFT0maker         *fTOFmaker;         //! TOF time0 computator
   AliTOFcalib           *fTOFcalib;         //! TOF calibration
   Bool_t                 fTOFcalibrateESD;  //  TOF settings
   Bool_t                 fTOFcorrectTExp;   //  TOF settings
   Bool_t                 fTOFuseT0;         //  TOF settings
   Bool_t                 fTOFtuneMC;        //  TOF settings
   Double_t               fTOFresolution;    //  TOF settings

   Double_t GetValue(Int_t num, AliRsnDaughter* d);
   Bool_t IsInTypeRange(Int_t num);

   ClassDef(AliRsnCustomTrackInfo, 1)
};

#endif // ALIRSNCUSTOMTRACKINFO_H
