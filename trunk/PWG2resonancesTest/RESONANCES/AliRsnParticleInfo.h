//
// Class AliRsnParticleInfo
//
// Rsn output FileInfo
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//

#ifndef ALIRSNPARTICLEINFO_H
#define ALIRSNPARTICLEINFO_H

#include <TObject.h>
#include <TList.h>
#include <TString.h>
#include <TArrayI.h>
#include <TH1.h>
#include <AliMCParticle.h>
#include <AliMCEvent.h>

class AliRsnParticleInfo : public TObject
{
  public:

    enum EParticleParameter {
      kP = 0, kPt, kEta
    };

    AliRsnParticleInfo();

    void Clear();
    void Print();

    void SetPDG(Int_t pdg) {
      fPDG = pdg;
    }
    Int_t GetPDG()  {
      return fPDG;
    }

    void AddHistogram(EParticleParameter parameter, Int_t nbins, Double_t min, Double_t max);

    void InitHistograms(TList *list);
    void ProcessHistograms(AliMCEvent *ev);
    
    TString GetParameterName(EParticleParameter parameter);
    TString GetParameterTitle(EParticleParameter parameter);
    Double_t GetParameterValue(EParticleParameter parameter,AliMCParticle *part);
  private:

    Int_t       fPDG;
    Int_t       fNumberActiveOfParameters;
    TArrayI     fParameterArray;
    TList       fHistogramList;
    TList       *fCurrentList;

    ClassDef(AliRsnParticleInfo, 1)

};

#endif
