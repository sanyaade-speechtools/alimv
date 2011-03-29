#include "AliLog.h"

#include "AliMultiInputEventHandler.h"
#include "AliMixInputEventHandler.h"
#include "AliMCEventHandler.h"

#include "AliEmptyInputHandler.h"
ClassImp(AliEmptyInputHandler)

//_____________________________________________________________________________
AliEmptyInputHandler::AliEmptyInputHandler(const char *name) :
   AliInputEventHandler(name, name)
{
//
// Default constructor.
//
   AliDebug(AliLog::kDebug + 10, "<-");
   AliDebug(AliLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
AliEmptyInputHandler::~AliEmptyInputHandler()
{
//
// Destructor
//
   AliDebug(AliLog::kDebug + 10, "<-");
   AliDebug(AliLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
Bool_t AliEmptyInputHandler::Init(Option_t *opt)
{
//
// Init() is called for all mix input handlers.
//
   AliDebug(AliLog::kDebug + 5, Form("<- opt=%s", opt));

   AliDebug(AliLog::kDebug + 5, Form("->"));
   return kTRUE;
}
//_____________________________________________________________________________
Bool_t AliEmptyInputHandler::Init(TTree *tree, Option_t *opt)
{
//
// Init(const char*path) is called for all mix input handlers.
// Create event pool if needed
//
   AliDebug(AliLog::kDebug + 5, Form("<- %p %s opt=%s", (void *) tree, tree->GetName(), opt));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return kTRUE;
}
//_____________________________________________________________________________
Bool_t AliEmptyInputHandler::Notify()
{
//
// Notify() is called for all mix input handlers
//
   AliDebug(AliLog::kDebug + 5, Form("<-"));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return kTRUE;
}

//_____________________________________________________________________________
Bool_t AliEmptyInputHandler::Notify(const char *path)
{
//
// Notify(const char*path) is called for all mix input handlers
//
   AliDebug(AliLog::kDebug + 5, Form("<- %s", path));
   AliDebug(AliLog::kDebug + 5, "->");
   return kTRUE;
}
//_____________________________________________________________________________
Bool_t AliEmptyInputHandler::BeginEvent(Long64_t entry)
{
//
// BeginEvent(Long64_t entry) is called for all mix input handlers
//
   AliDebug(AliLog::kDebug + 5, Form("<- %lld", entry));

   if (fParentHandler) {
      TString tmp = "";
      AliInputEventHandler *ih = 0;
      AliMultiInputEventHandler *multiIH = dynamic_cast<AliMultiInputEventHandler*>(fParentHandler);
      if (multiIH) {
         ih = multiIH->GetFirstInputEventHandler();
         if (ih) {

         }
      }
   }
   AliDebug(AliLog::kDebug + 5, "->");
   return kTRUE;
}

Bool_t AliEmptyInputHandler::GetEntry()
{
   AliDebug(AliLog::kDebug + 5, "<-");
   AliDebug(AliLog::kDebug + 5, "->");
   return kTRUE;
}

//_____________________________________________________________________________
Bool_t AliEmptyInputHandler::FinishEvent()
{
   //
   // FinishEvent() is called for all mix input handlers
   //
   AliDebug(AliLog::kDebug + 5, Form("<-"));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return kTRUE;
}
