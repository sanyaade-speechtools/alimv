//
// Class AliLogInputHandler
//
// Multi input event handler
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#include "AliLog.h"

#include "AliLogInputHandler.h"

ClassImp(AliLogInputHandler)

//_____________________________________________________________________________
AliLogInputHandler::AliLogInputHandler(const char *name) :
   AliInputEventHandler(name, name)
{
//
// Default constructor.
//
   AliDebug(AliLog::kDebug + 10, "<-");
   AliDebug(AliLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
AliLogInputHandler::~AliLogInputHandler()
{
   //
   // Destructor
   //
   AliDebug(AliLog::kDebug + 10, "<-");
   AliDebug(AliLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
Bool_t AliLogInputHandler::Init(Option_t *opt)
{
   //
   // Init() is called for all mix input handlers.
   //
   AliDebug(AliLog::kDebug + 5, Form("<- \"%s\"", opt));

   TObjArrayIter next(&fClassLogSettings);
   TNamed*obj = 0;
   while ((obj = (TNamed *) next())) {
      TString level(obj->GetTitle());
      AliInfo(Form("Setting Class Debug Level : %s -> %d", obj->GetName(), level.Atoi()));
      AliLog::SetClassDebugLevel(obj->GetName(), level.Atoi());
   }
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return AliInputEventHandler::Init(opt);
}
//_____________________________________________________________________________
Bool_t AliLogInputHandler::Init(TTree *tree, Option_t *opt)
{
   //
   // Init(const char*path) is called for all mix input handlers.
   // Create event pool if needed
   //
   AliDebug(AliLog::kDebug + 5, Form("<- %p %s", (void *) tree, tree->GetName()));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return AliInputEventHandler::Init(tree, opt);
}
//_____________________________________________________________________________
Bool_t AliLogInputHandler::Notify()
{
   //
   // Notify() is called for all mix input handlers
   //
   AliDebug(AliLog::kDebug + 5, Form("<-"));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return AliInputEventHandler::Notify();
}

//_____________________________________________________________________________
Bool_t AliLogInputHandler::Notify(const char *path)
{
   //
   // Notify(const char*path) is called for all mix input handlers
   //
   AliDebug(AliLog::kDebug + 5, Form("<- %s", path));
   return AliInputEventHandler::Notify(path);
}
//_____________________________________________________________________________
Bool_t AliLogInputHandler::BeginEvent(Long64_t entry)
{
   //
   // BeginEvent(Long64_t entry) is called for all mix input handlers
   //
   AliDebug(AliLog::kDebug + 5, Form("<- %lld", entry));
   return AliInputEventHandler::BeginEvent(entry);
}

//_____________________________________________________________________________
Bool_t AliLogInputHandler::FinishEvent()
{
   //
   // FinishEvent() is called for all mix input handlers
   //
   AliDebug(AliLog::kDebug + 5, Form("<-"));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return AliInputEventHandler::FinishEvent();
}

void AliLogInputHandler::AddClassDebugLevel(const char* classname, Int_t level)
{
   fClassLogSettings.Add(new TNamed(classname, Form("%d", level)));
}
