//
// Class AliRsnOutputObj
//
// Rsn output FileInfo
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//

#ifndef ALIRSNOUTPUTOBJ_H
#define ALIRSNOUTPUTOBJ_H

#include <TNamed.h>
#include <TString.h>
#include <TObjArray.h>
#include <TH1.h>
#include <TFile.h>

class AliRsnOutputObj : public TNamed {

    public:

      enum {
        kOne = 0, kTwo = 1,
        kNumberOfArray = 2,
        kMinus = 0, kDivide = 1,
        kNumberOfBinningHistograms = 100
      };

      AliRsnOutputObj(const char*name = "defualt", const char *title = "default title");

      void Clear();
      void Print();
      void PrintArray(Int_t index);

      void InitHistograms(TList *listAll);
      void InitExtraHistograms(TList *listAll);

      void ProcessHistograms(TFile* file, TList* listIn = 0);
      void ProcessHistograms(Int_t index, TFile* list, TList* listIn = 0);
      void AddHistogram(Int_t index, TString listName, TString name, Int_t projection = 0);

      void ProcessExtraHistograms(TList *list = 0);
      void DoExtraHistogramMinus(TList* list);
      void DoExtraHistogramDivide(TList* list);

      void SetDoMinus(Bool_t b = kTRUE) { fDoMinus = b; }
      void SetDoDivide(Bool_t b = kTRUE) { fDoDivide = b; }

      void SetBinningCut(Int_t index, Double_t min, Double_t max, Double_t step);
      void DoNormalize(Bool_t doNormalize = kTRUE) { fDoNormalize = doNormalize;}

      void SetNormalizeRange(Double_t min = 0, Double_t max = 0);
//     void SetNormalizeRange(Double_t min=0.0,Double_t max=0.0);

      void SetSignalBinRange(Int_t left=1,Int_t right=1) {fSignalBinsLeft=left;fSignalBinsRight=right; }

      TH1D         *GetExtraHistogram(Int_t index = 0, Int_t index2 = 0) { return fExtraHistograms[index][index2]; }

    private:
      TList        *fOutList;
      TH1D         *fHistogram[kNumberOfArray][kNumberOfBinningHistograms];
      TObjArray     fHistogramNameArray[kNumberOfArray];

      Int_t         fCurrentProjectionIndex;
      Int_t         fBinningCutIndex;
      Double_t      fBinningStep;
      Double_t      fBinningMin;
      Double_t      fBinningMax;

      Bool_t        fDoMinus;
      Bool_t        fDoDivide;

      Double_t      fForceRebin;
      Double_t      fForceUserRange[2];

      TH1D         *fExtraHistograms[2][kNumberOfBinningHistograms];
      TH1D         *fNormHistogram[kNumberOfArray][kNumberOfBinningHistograms];
      TH1D         *fParameterHistogram;

      Bool_t        fDoNormalize;
      Double_t      fNormalizeMin;
      Double_t      fNormalizeMax;
      Int_t         fSignalBinsLeft;
      Int_t         fSignalBinsRight;

//     void          LoopOverAllHistograms(Int_t index);

      ClassDef(AliRsnOutputObj, 1)


  };

#endif
