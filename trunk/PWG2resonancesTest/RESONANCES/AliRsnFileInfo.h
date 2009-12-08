//
// Class AliRsnFileInfo
//
// Rsn output FileInfo
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//

#ifndef ALIRSNFILEINFO_H
#define ALIRSNFILEINFO_H

#include <TObject.h>
#include <TString.h>

class AliRsnFileInfo : public TObject
{
  public:

    AliRsnFileInfo();
//     virtual ~AliRsnFileInfo();

    void Clear(const Option_t*);
    void Print(const Option_t*opt) const;

    TString   GetFileName() { return fFileName; }
    void      SetFileName(TString filename) { fFileName = filename; }

  private:
    TString fFileName;

    ClassDef(AliRsnFileInfo, 1)
};

#endif
