#ifndef ALIRSNCUSTOMTRACKINFOBAT_H
#define ALIRSNCUSTOMTRACKINFOBAT_H

#include "AliRsnCustomTrackInfo.h"

class AliRsnCustomTrackInfoBat : public AliRsnCustomTrackInfo
{

public:
    AliRsnCustomTrackInfoBat(const char* name="noname", const char* title="");
    virtual ~AliRsnCustomTrackInfoBat();
    
    virtual void UserCreateOutputCustom(TList* list);
    virtual void UserExecCustom(AliRsnEvent* ev1, AliRsnEvent* ev2);
    virtual void UserExecMixCustom(AliRsnEvent* ev1, AliRsnEvent* ev2);
    
private:
//   TH1D *fMyHist;

		ClassDef(AliRsnCustomTrackInfoBat,1)
};

#endif // ALIRSNCUSTOMTRACKINFO_H
