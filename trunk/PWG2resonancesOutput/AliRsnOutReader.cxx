#include <TError.h>
#include <TFile.h>

#include "AliRsnOutReader.h"
#include <THnSparse.h>

ClassImp(AliRsnOutReader)

AliRsnOutReader::AliRsnOutReader() : TNamed(),
   fCurrentFile(0),
   fCurrentList(0),
   fCurrentSparse(0),
   fIsOk(kFALSE)
{
   for (Int_t i = 0; i < 3; i++) {
      fAdditionalCutMinMaxBin[i] = -1;
   }
}

AliRsnOutReader::AliRsnOutReader(const char* name, const char* filename): TNamed(name, filename),
   fCurrentFile(0),
   fCurrentList(0),
   fCurrentSparse(0),
   fIsOk(kFALSE)
{
   for (Int_t i = 0; i < 3; i++) {
      fAdditionalCutMinMaxBin[i] = -1;
   }
}

AliRsnOutReader::~AliRsnOutReader()
{

}

void AliRsnOutReader::Print(Option_t* option) const
{
   if (!option)
      Printf("Option : %s", option);
   if (!fCurrentFile || !fCurrentList || !fCurrentSparse) return ;
   Printf("Filename : %s | List : %15s | Sparse: %10s", fCurrentFile->GetName(), fCurrentList->GetName(), fCurrentSparse->GetName());
}
Bool_t AliRsnOutReader::SetOutput(const char* listname, const char* sparsename)
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

TH1* AliRsnOutReader::Get1DHistogram(Int_t dim, Int_t cutDim, Double_t cutMin, Double_t cutMax)
{
   if (!fIsOk) return 0;

   // TODO set current file
//   fCurrentFile->cd();
   TH1D *histOut = 0;

   if (dim >= fCurrentSparse->GetNdimensions()) return 0;

   if (fAdditionalCutMinMaxBin[0] >= 0) fCurrentSparse->GetAxis(fAdditionalCutMinMaxBin[0])->SetRangeUser(fAdditionalCutMinMaxBin[1], fAdditionalCutMinMaxBin[2]);

   // apply cut
   if (cutDim >= 0) fCurrentSparse->GetAxis(cutDim)->SetRangeUser(cutMin, cutMax);

   histOut = fCurrentSparse->Projection(dim);

   // TODO set previous TDirectory

   return histOut;
}

void AliRsnOutReader::SetAdditionalCut(Double_t dimCut, Double_t min, Double_t max)
{
   fAdditionalCutMinMaxBin[0] = dimCut;
   fAdditionalCutMinMaxBin[1] = min;
   fAdditionalCutMinMaxBin[2] = max;
}
