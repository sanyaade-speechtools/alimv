//
// Class AliRsnFileInfo
//
// Rsn output FileInfo 
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//

#include "AliRsnFileInfo.h"

ClassImp(AliRsnFileInfo)

//_________________________________________________________________________________________________
AliRsnFileInfo::AliRsnFileInfo() : TObject(), 
  fFileName("")
{
//
// Default constructor.
//
}

void AliRsnFileInfo::Clear(const Option_t*)
{
  fFileName = "";
}


void AliRsnFileInfo::Print(const Option_t*opt) const
{
  Printf(Form("%s%s",opt,fFileName.Data()));
}


