#ifndef ALIFILEINFO_H
#define ALIFILEINFO_H

#include <TObject.h>
#include <TString.h>


class AliFileInfo : public TObject {

public:
   AliFileInfo();

   TString FileName() { return fFileName;}
   void    SetFileName(TString fn) { fFileName = fn; }
private:
   TString fFileName;

   ClassDef(AliFileInfo, 1); //File Info
};

#endif // ALIFILEINFO_H
