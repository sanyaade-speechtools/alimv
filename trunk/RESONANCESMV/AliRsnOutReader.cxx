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

}

AliRsnOutReader::AliRsnOutReader(const char* name, const char* filename): TNamed(name, filename),
        fCurrentFile(0),
        fCurrentList(0),
        fCurrentSparse(0),
        fIsOk(kFALSE)
{

}

AliRsnOutReader::~AliRsnOutReader()
{

}

void AliRsnOutReader::Print(Option_t* option) const
{
    if (!option)
        ::Info("AliRsnOutReader::Print",Form("Option : %s",option));
    if (!fCurrentFile || !fCurrentList || !fCurrentSparse) return ;
    ::Info("AliRsnOutReader::Print",Form("Filename : %s | List : %15s | Sparse: %10s",fCurrentFile->GetName(),fCurrentList->GetName(),fCurrentSparse->GetName()));
}
Bool_t AliRsnOutReader::SetOutput(const char* listname, const char* sparsename)
{

    fCurrentFile = TFile::Open(GetTitle(),"READ");
    if (!fCurrentFile) {
        ::Error("AliRsnOutReader::Init",Form("Problem opening file %s !!!",GetTitle()));
        fIsOk = kFALSE;
        return fIsOk;
    }
    fCurrentList = dynamic_cast<TList*>(fCurrentFile->Get(listname));

    if (!fCurrentList) {
        ::Error("AliRsnOutReader::Init",Form("Problem opening list %s !!!",listname));
        fIsOk = kFALSE;
        return fIsOk;
    }

    fCurrentSparse = dynamic_cast<THnSparse*>(fCurrentList->FindObject(sparsename));
    if (!fCurrentSparse) {
        ::Error("AliRsnOutReader::Init",Form("Problem opening THnSparse %s !!!",sparsename));
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
    TH1D *histOut=0;

    if (dim >= fCurrentSparse->GetNdimensions()) return 0;

    // apply cut
    if (cutDim>=0) fCurrentSparse->GetAxis(cutDim)->SetRangeUser(cutMin,cutMax);

    histOut = fCurrentSparse->Projection(dim);

    // TODO set previous TDirectory

    return histOut;
}


Double_t AliRsnOutReader::GetMaximumValue(Int_t dim)
{
  if (!fIsOk) return 0;
  
  return 100;
//   return fCurrentSparse->GetAxis(dim)->Get;
}

