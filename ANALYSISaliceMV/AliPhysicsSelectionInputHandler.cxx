//
// Class AliPhysicsSelectionInputHandler
//
// Multi input event handler
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#include "AliLog.h"

#include "AliPhysicsSelectionInputHandler.h"

ClassImp(AliPhysicsSelectionInputHandler)

//_____________________________________________________________________________
AliPhysicsSelectionInputHandler::AliPhysicsSelectionInputHandler() :
   AliInputEventHandler("",""),
   fOption(""),
   fPhysicsSelection(0)
{
//
// Default constructor.
//
   AliDebug(AliLog::kDebug + 10, "<-");
   AliDebug(AliLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
AliPhysicsSelectionInputHandler::AliPhysicsSelectionInputHandler(const char *name,const char *opt) :
   AliInputEventHandler(name, name),
   fOption(opt),
   fPhysicsSelection(new AliPhysicsSelection)
{
//
// Default constructor.
//
   AliDebug(AliLog::kDebug + 10, "<-");
   AliDebug(AliLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
AliPhysicsSelectionInputHandler::~AliPhysicsSelectionInputHandler()
{
   //
   // Destructor
   //
   AliDebug(AliLog::kDebug + 10, "<-");
   AliDebug(AliLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
Bool_t AliPhysicsSelectionInputHandler::Init(Option_t *opt)
{
   //
   // Init() is called for all mix input handlers.
   //
   AliDebug(AliLog::kDebug + 5, Form("<- \"%s\"", opt));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return AliInputEventHandler::Init(opt);
}
//_____________________________________________________________________________
Bool_t AliPhysicsSelectionInputHandler::Init(TTree *tree, Option_t *opt)
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
Bool_t AliPhysicsSelectionInputHandler::Notify()
{
   //
   // Notify() is called for all mix input handlers
   //
   AliDebug(AliLog::kDebug + 5, Form("<-"));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return AliInputEventHandler::Notify();
}

//_____________________________________________________________________________
Bool_t AliPhysicsSelectionInputHandler::Notify(const char *path)
{
   //
   // Notify(const char*path) is called for all mix input handlers
   //
   AliDebug(AliLog::kDebug + 5, Form("<- %s", path));
   return AliInputEventHandler::Notify(path);
}
//_____________________________________________________________________________
Bool_t AliPhysicsSelectionInputHandler::BeginEvent(Long64_t entry)
{
   //
   // BeginEvent(Long64_t entry) is called for all mix input handlers
   //
   AliDebug(AliLog::kDebug + 5, Form("<- %lld", entry));
   return AliInputEventHandler::BeginEvent(entry);
}

//_____________________________________________________________________________
Bool_t AliPhysicsSelectionInputHandler::FinishEvent()
{
   //
   // FinishEvent() is called for all mix input handlers
   //
   AliDebug(AliLog::kDebug + 5, Form("<-"));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return AliInputEventHandler::FinishEvent();
}

