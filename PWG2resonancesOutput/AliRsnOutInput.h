#ifndef ALIRSNOUTINPUT_H
#define ALIRSNOUTINPUT_H

#include <TH1.h>
#include <TNamed.h>
#include <Rtypes.h>

class TFile;
class TList;
class THnSparse;
class AliRsnOutInput : public TNamed {

public:
   AliRsnOutInput();
   AliRsnOutInput(const char* name, const char* filename);
   virtual ~AliRsnOutInput();

   virtual void Print(Option_t* option = "") const;

   Bool_t SetInputSource(const char*listname, const char*histname);

   TH1 *Get1DHistogram(Int_t dim = -1, Int_t cutDim = -1, Double_t cutMin = 0.0, Double_t cutMax = 0.0);

   void SetNormalizeInput(AliRsnOutInput *norm, Double_t min = 1.04, Double_t max = 1.1);

   void ScaleHistogram(TH1*h1, TH1*h2, Double_t *minmax, Double_t normCorrection);

private:
   TFile *fCurrentFile;
   TList *fCurrentList;
   THnSparse *fCurrentSparse;
   AliRsnOutInput *fNormInput;
   Bool_t fIsOk;
   Double_t fHistNormMinMax[2];

   ClassDef(AliRsnOutInput, 1) // ALIRSNOUTINPUT_H
};

#endif // ALIRSNOUTREADER_H
