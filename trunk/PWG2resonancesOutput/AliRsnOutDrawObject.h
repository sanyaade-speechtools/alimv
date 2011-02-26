#ifndef ALIRSNOUTDRAWOBJECT_H
#define ALIRSNOUTDRAWOBJECT_H

#include <TNamed.h>
#include <TString.h>
#include <TStyle.h>

class AliRsnOutManager;
class AliRsnOutFunction;
class TF1;
class TH1;
class TVirtualPad;
class AliRsnOutDrawObject : public TNamed {

public:
   AliRsnOutDrawObject(const char*name, AliRsnOutFunction *rsnFun = 0);
   virtual void Print(Option_t* option = "") const;
   void Reset();
   TH1* CreateHistogram(AliRsnOutManager* rsnOutMgr, Int_t valDim, Int_t cutDim, Double_t min, Double_t max);
   void DrawObject(AliRsnOutManager* rsnOutMgr, Double_t min, Double_t max, TVirtualPad* pad = 0, TString opt = "");

   TH1 *GetHist() { return fHist; }
   void SetHist(TH1 *hist) { fHist = hist; }

   TF1 *GetFun() { return fFunc; }
   void SetFun(TF1 *fun) { fFunc = fun; }

   TF1 *GetFitFun() { return fFitFunc; }
   void SetFitFun(TF1 *fun) { fFitFunc = fun; }

   TString GetOptions() { return fOptions; }
   void SetOptions(TString opts) { fOptions = opts;}

   TString GetDrawOptions() { return fDrawOptions; }
   void SetDrawOptions(TString opts) { fDrawOptions = opts;}

   TStyle *CreateStyle(const char*name, const char *title = "");
   TStyle *GetStyle() { return fStyle; }
   TStyle *GetFitStyle() { return fStyleFit; }

   AliRsnOutFunction *GetRsnFunction() { return fRsnFunction; }

private:

   AliRsnOutFunction *fRsnFunction;

   TH1 *fHist;
   TF1 *fFunc;
   TF1 *fFitFunc;

   TString fOptions;
   TString fDrawOptions;

   TStyle *fStyle;
   TStyle *fStyleFit;


   ClassDef(AliRsnOutDrawObject, 1) // ALIRSNOUTFUNCTION_H
};

#endif // ALIRSNOUTREADER_H
