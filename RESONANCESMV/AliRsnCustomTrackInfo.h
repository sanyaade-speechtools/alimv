#ifndef ALIRSNCUSTOMTRACKINFO_H
#define ALIRSNCUSTOMTRACKINFO_H

#include "AliRsnVCustomObj.h"
#include <TH1.h>
#include <AliRsnCutSet.h>

class AliRsnCustomTrackInfo : public AliRsnVCustomObj
{

public:
  enum EType { kP=0,kPt=1,kEta=2,kNumTypes };

    AliRsnCustomTrackInfo(const char* name="noname", const char* title="");
    virtual ~AliRsnCustomTrackInfo();
    
    virtual void UserCreateOutputCustom(TList* list);
    virtual void UserExecCustom(AliRsnEvent* ev1, AliRsnEvent* ev2);
    virtual void UserExecMixCustom(AliRsnEvent* ev1, AliRsnEvent* ev2);
    
    const char* GetParameterName(Int_t num);
    const char* GetParameterTitle(Int_t num);
    
    void SetParamaterHistogram(EType type,Int_t nBins,Double_t min,Double_t max);
    void SetCutSet(AliRsnCutSet *cutSet) { fCutSet = cutSet;}
    void SetCharge(Short_t ch) { fCharge = ch; }
    
private:
    
  Bool_t      fUseParameter[kNumTypes];     //  flag if parameter is used
  TH1D       *fParameter[kNumTypes];        //! histograms
  Int_t       fParameterBins[kNumTypes];    //  nBins
  Double_t    fParameterMin[kNumTypes];     //  min
  Double_t    fParameterMax[kNumTypes];     //  max
  
  Short_t     fCharge;
  
  AliRsnCutSet *fCutSet;                    //  cut set
  
  Double_t GetValue(Int_t num, AliRsnDaughter* d);
  Bool_t IsInTypeRange(Int_t num);
    
    ClassDef(AliRsnCustomTrackInfo,1)
};

#endif // ALIRSNCUSTOMTRACKINFO_H
