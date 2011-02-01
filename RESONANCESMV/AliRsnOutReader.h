#ifndef ALIRSNOUTREADER_H
#define ALIRSNOUTREADER_H

#include <TH1.h>
#include <TNamed.h>
#include <Rtypes.h>

class TFile;
class TList;
class THnSparse;
class AliRsnOutReader : public TNamed {

public:
   AliRsnOutReader();
   AliRsnOutReader(const char* name, const char* filename);
   virtual ~AliRsnOutReader();

   virtual void Print(Option_t* option = "") const;

   Bool_t SetOutput(const char*listname, const char*histname);

   TH1 *Get1DHistogram(Int_t dim = -1, Int_t cutDim = -1, Double_t cutMin = 0.0, Double_t cutMax = 0.0);

   Double_t GetMaximumValue(Int_t dim);

private:
   TFile *fCurrentFile;
   TList *fCurrentList;
   THnSparse *fCurrentSparse;

   Bool_t fIsOk;


   ClassDef(AliRsnOutReader, 1) // AliRsnOutReader
};

#endif // ALIRSNOUTREADER_H
