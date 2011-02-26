#include <TError.h>
#include <TFile.h>
#include <THnSparse.h>

#include "AliRsnOutInput.h"
ClassImp(AliRsnOutInput)

AliRsnOutInput::AliRsnOutInput() : TNamed(),
   fCurrentFile(0),
   fCurrentList(0),
   fCurrentSparse(0),
   fIsOk(kFALSE)
{
}

AliRsnOutInput::AliRsnOutInput(const char* name, const char* filename): TNamed(name, filename),
   fCurrentFile(0),
   fCurrentList(0),
   fCurrentSparse(0),
   fNormInput(0),
   fIsOk(kFALSE)
{
   fHistNormMinMax[0] = 0.0;
   fHistNormMinMax[1] = 0.0;
}

AliRsnOutInput::~AliRsnOutInput()
{

}

void AliRsnOutInput::Print(Option_t* option) const
{
   if (!option)
      Printf("Option : %s", option);
   if (!fCurrentFile || !fCurrentList || !fCurrentSparse) return ;
   Printf("Filename : %s | List : %15s | Sparse: %10s", fCurrentFile->GetName(), fCurrentList->GetName(), fCurrentSparse->GetName());
}
Bool_t AliRsnOutInput::SetInputSource(const char* listname, const char* sparsename)
{

   fCurrentFile = TFile::Open(GetTitle(), "READ");
   if (!fCurrentFile) {
      Printf("Problem opening file %s !!!", GetTitle());
      fIsOk = kFALSE;
      return fIsOk;
   }
   fCurrentList = dynamic_cast<TList*>(fCurrentFile->Get(listname));

   if (!fCurrentList) {
      Printf("Problem opening list %s !!!", listname);
      fIsOk = kFALSE;
      return fIsOk;
   }

   fCurrentSparse = dynamic_cast<THnSparse*>(fCurrentList->FindObject(sparsename));
   if (!fCurrentSparse) {
      Printf("Problem opening THnSparse %s !!!", sparsename);
      fIsOk = kFALSE;
      return fIsOk;
   }

   fIsOk = kTRUE;
   return fIsOk;
}

TH1* AliRsnOutInput::Get1DHistogram(Int_t dim, Int_t cutDim, Double_t cutMin, Double_t cutMax)
{
   if (!fIsOk) return 0;

   // TODO set current file
//   fCurrentFile->cd();
   TH1 *histOut = 0;

   if (dim >= fCurrentSparse->GetNdimensions()) return 0;

//    if (fAdditionalCutMinMaxBin[0] >= 0) fCurrentSparse->GetAxis(fAdditionalCutMinMaxBin[0])->SetRangeUser(fAdditionalCutMinMaxBin[1], fAdditionalCutMinMaxBin[2]);

   // apply cut
   if (cutDim >= 0) fCurrentSparse->GetAxis(cutDim)->SetRangeUser(cutMin, cutMax);

   histOut = fCurrentSparse->Projection(dim);

   TH1 *histNorm = 0;
   if (fNormInput && fHistNormMinMax[0] != fHistNormMinMax[1]) {
      histOut->SetName(Form("%s_norm", histOut->GetName()));
      histNorm = fNormInput->Get1DHistogram(dim, cutDim, cutMin, cutMax);
      ScaleHistogram(histNorm, histOut, fHistNormMinMax, 1.0);
   }

//    Double_t fHistNormMinMax[] = {1.04, 1.09};



   delete histNorm;

   // TODO set previous TDirectory

   return histOut;
}

void AliRsnOutInput::ScaleHistogram(TH1*h1, TH1*h2, Double_t *minmax, Double_t normCorrection)
{
   if (!h1) return;
   if (!h2) return;

   if (minmax[0] != minmax[1]) {
      Int_t fNormalizeBinMinI = h1->FindBin(minmax[0]);
      Int_t fNormalizeBinMaxI = h1->FindBin(minmax[1]);
      Double_t normtmp = h2->Integral(fNormalizeBinMinI, fNormalizeBinMaxI);
      Double_t normFactor = (Double_t) h1->Integral(fNormalizeBinMinI, fNormalizeBinMaxI) / normtmp * normCorrection;
      h2->Scale(normFactor);
   } else {
      Double_t normtmp = h2->Integral();
      Double_t normFactor = (Double_t) h1->Integral() / normtmp * normCorrection;
      h2->Scale(normFactor);
   }
}

void AliRsnOutInput::SetNormalizeInput(AliRsnOutInput* norm, Double_t min, Double_t max)
{
   fNormInput = norm;
   fHistNormMinMax[0] = min;
   fHistNormMinMax[1] = max;
}
