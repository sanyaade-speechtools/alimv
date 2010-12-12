#ifndef ALIRSNVCUSTOMOBJ_H
#define ALIRSNVCUSTOMOBJ_H

#include <TNamed.h>

class TList;
class AliRsnEvent;
class AliRsnVCustomObj : public TNamed
{

public:
    AliRsnVCustomObj(const char* name, const char* title);
    
    virtual void UserCreateOutputCustom(TList */*list*/) {;}
    virtual void UserExecCustom(AliRsnEvent*/*ev1*/,AliRsnEvent */*ev2*/) {;}
    virtual void UserExecMixCustom(AliRsnEvent*/*ev1*/,AliRsnEvent */*ev2*/) {;}
    
    ClassDef(AliRsnVCustomObj, 1)
};

#endif // ALIRSNVCUSTOMOBJ_H
