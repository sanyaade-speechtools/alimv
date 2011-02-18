//
// Class AliTenderInputEventHandler
//
// Multi input event handler
// TODO example
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#include "AliLog.h"

#include "AliTenderInputEventHandler.h"
#include "AliAnalysisManager.h"
#include "AliESDpid.h"
#include <TChain.h>
#include <TFile.h>

ClassImp(AliTenderInputEventHandler)

//_____________________________________________________________________________
AliTenderInputEventHandler::AliTenderInputEventHandler(const char *name) :
   AliInputEventHandler(name, name),
   fTender("fakeTender")
{
//
// Default constructor.
//
   AliDebug(AliLog::kDebug + 10, "<-");
   AliDebug(AliLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
AliTenderInputEventHandler::~AliTenderInputEventHandler()
{
   //
   // Destructor
   //
   AliDebug(AliLog::kDebug + 10, "<-");
   AliDebug(AliLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
Bool_t AliTenderInputEventHandler::Init(Option_t *opt)
{
   //
   // Init() .
   //
   AliDebug(AliLog::kDebug + 5, Form("<- \"%s\"", opt));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return AliInputEventHandler::Init(opt);
}
//_____________________________________________________________________________
Bool_t AliTenderInputEventHandler::Init(TTree *tree, Option_t *opt)
{
   //
   // Init(const char*path) .
   // Create event pool if needed
   //
   AliDebug(AliLog::kDebug + 5, Form("<- %p %s", (void *) tree, tree->GetName()));



   DoInit();

   AliDebug(AliLog::kDebug + 5, Form("->"));
   return AliInputEventHandler::Init(tree, opt);
}
//_____________________________________________________________________________
Bool_t AliTenderInputEventHandler::Notify()
{
   //
   // Notify()
   //
   AliDebug(AliLog::kDebug + 5, Form("<-"));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return AliInputEventHandler::Notify();
}

//_____________________________________________________________________________
Bool_t AliTenderInputEventHandler::Notify(const char *path)
{
   //
   // Notify(const char*path)
   //
   AliDebug(AliLog::kDebug + 5, Form("<- %s", path));
//    DoInit();
   return AliInputEventHandler::Notify(path);
}
//_____________________________________________________________________________
Bool_t AliTenderInputEventHandler::BeginEvent(Long64_t entry)
{
   //
   // BeginEvent(Long64_t entry)
   //
   AliDebug(AliLog::kDebug + 5, Form("<- %lld", entry));

   DoProcess();


   return AliInputEventHandler::BeginEvent(entry);
}

//_____________________________________________________________________________
Bool_t AliTenderInputEventHandler::FinishEvent()
{
   //
   // FinishEvent()
   //
   AliDebug(AliLog::kDebug + 5, Form("<-"));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return AliInputEventHandler::FinishEvent();
}

void AliTenderInputEventHandler::DoInit()
{

   // TODO connect CORRECT esd handler (event mxing case)
   AliMultiInputEventHandler *multiH = dynamic_cast<AliMultiInputEventHandler*>(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());
//    AliMultiInputEventHandler *multiH = dynamic_cast<AliMultiInputEventHandler*>(ParentHandler());

   AliESDInputHandler*esdH =  dynamic_cast<AliESDInputHandler *>(multiH->GetFirstInputEventHandler());
   if (esdH) {
      fTender.SetESDhandler(esdH);
      if (!esdH->GetESDpid()) esdH->SetESDpid(new AliESDpid());
      TIter next(fTender.GetSupplies());
      AliTenderSupply *supply;
      while ((supply = (AliTenderSupply*)next())) {
         supply->SetTender(&fTender);
      }
      fTender.ConnectInputData();
   } else {
      AliFatal("AliTenderInputEventHandler:No ESD input event handler connected");
   }
}
void AliTenderInputEventHandler::DoProcess()
{


   AliESDInputHandler*esdH =  fTender.GetESDhandler();
   if (esdH) {
      AliDebug(AliLog::kDebug, Form("We have ESD event with %d tracks", fTender.GetEvent()->GetNumberOfTracks()));
   } else {
      AliFatal("No ESD input event handler connected");
   }

   fTender.UserExec("NoPost");;

}
