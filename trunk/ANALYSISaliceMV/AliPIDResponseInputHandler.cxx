#include <TFile.h>
#include <TPRegexp.h>

#include "AliLog.h"
#include "AliAnalysisManager.h"
#include "AliMultiInputEventHandler.h"
#include "AliMixInputEventHandler.h"
#include "AliMCEventHandler.h"
#include "AliPIDResponse.h"

#include "AliPIDResponseInputHandler.h"


ClassImp(AliPIDResponseInputHandler)

//_____________________________________________________________________________
AliPIDResponseInputHandler::AliPIDResponseInputHandler(const char *name) :
   AliInputEventHandler(name, name),
   fIsMC(kFALSE),
   fTOFTimeZeroTypeUser(-1),
   fTOFTimeZeroType(AliPIDResponse::kBest_T0),
   fTOFres(100.),
   fPIDResponse(0x0),
   fBeamType("PP"),
   fLHCperiod(),
   fMCperiodTPC(),
   fRecoPass(0),
   fRun(0),
   fOldRun(0),
   fArrPidResponseMaster(0x0),
   fMCurrentMutliIH(0)
{
//
// Default constructor.
//
   AliDebug(AliLog::kDebug + 10, "<-");
   AliDebug(AliLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
AliPIDResponseInputHandler::~AliPIDResponseInputHandler()
{
//
// Destructor
//
   AliDebug(AliLog::kDebug + 10, "<-");
//    delete fArrPidResponseMaster;
   AliDebug(AliLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
Bool_t AliPIDResponseInputHandler::Init(Option_t *opt)
{
//
// Init() is called for all mix input handlers.
//
   AliDebug(AliLog::kDebug + 5, Form("<- opt=%s", opt));

   AliDebug(AliLog::kDebug + 5, Form("->"));
   return kTRUE;
}
//_____________________________________________________________________________
Bool_t AliPIDResponseInputHandler::Init(TTree *tree, Option_t *opt)
{
//
// Init(const char*path) is called for all mix input handlers.
// Create event pool if needed
//
   AliDebug(AliLog::kDebug + 5, Form("<- %p %s opt=%s", (void *) tree, tree->GetName(), opt));

   if (fParentHandler) {
      TString tmp = "";
      AliInputEventHandler *ih = 0;
      AliMultiInputEventHandler *fMCurrentMutliIH = dynamic_cast<AliMultiInputEventHandler*>(fParentHandler);
      if (fMCurrentMutliIH) {
         ih = fMCurrentMutliIH->GetFirstInputEventHandler();
         if (ih) {
            //pid response object
            ih->CreatePIDResponse(fIsMC);
            fPIDResponse = ih->GetPIDResponse();
            if (!fPIDResponse) AliFatal("PIDResponse object was not created");
         }
      }
   }

   AliDebug(AliLog::kDebug + 5, Form("->"));
   return kTRUE;
}
//_____________________________________________________________________________
Bool_t AliPIDResponseInputHandler::Notify()
{
//
// Notify() is called for all mix input handlers
//
   AliDebug(AliLog::kDebug + 5, Form("<-"));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return kTRUE;
}

//_____________________________________________________________________________
Bool_t AliPIDResponseInputHandler::Notify(const char *path)
{
//
// Notify(const char*path) is called for all mix input handlers
//
   AliDebug(AliLog::kDebug + 5, Form("<- %s", path));
   AliDebug(AliLog::kDebug + 5, "->");
   return kTRUE;
}
//_____________________________________________________________________________
Bool_t AliPIDResponseInputHandler::BeginEvent(Long64_t entry)
{
//
// BeginEvent(Long64_t entry) is called for all mix input handlers
//
   AliDebug(AliLog::kDebug + 5, Form("<- %lld", entry));

   if (fParentHandler) {
      TString tmp = "";
      AliInputEventHandler *ih = 0;
      fMCurrentMutliIH = dynamic_cast<AliMultiInputEventHandler*>(fParentHandler);
      if (fMCurrentMutliIH) {
         ih = fMCurrentMutliIH->GetFirstInputEventHandler();
         if (ih) {
            //pid response object
            ih->CreatePIDResponse(fIsMC);
            fPIDResponse = ih->GetPIDResponse();
            if (!fPIDResponse) AliFatal("PIDResponse object was not created");

            AliVEvent *event = ih->GetEvent();
            if (!event) return kFALSE;
            fRun = event->GetRunNumber();

            if (fRun != fOldRun) {
               ExecNewRun();
               fOldRun = fRun;
            }
            Double_t timeZeroType = fTOFTimeZeroTypeUser;
            if (timeZeroType < 0) timeZeroType = fTOFTimeZeroType;
            fPIDResponse->SetTOFResponse(event, (AliPIDResponse::EStartTimeType_t)timeZeroType);
         }
      }
   }
   AliDebug(AliLog::kDebug + 5, "->");
   return kTRUE;
}

Bool_t AliPIDResponseInputHandler::GetEntry()
{
   AliDebug(AliLog::kDebug + 5, "<-");
   AliDebug(AliLog::kDebug + 5, "->");
   return kTRUE;
}

//_____________________________________________________________________________
Bool_t AliPIDResponseInputHandler::FinishEvent()
{
   //
   // FinishEvent() is called for all mix input handlers
   //
   AliDebug(AliLog::kDebug + 5, Form("<-"));
   AliDebug(AliLog::kDebug + 5, Form("->"));
   return kTRUE;
}

void AliPIDResponseInputHandler::ExecNewRun()
{
   //
   // Things to Execute upon a new run
   //

   SetRecoInfo();

   SetITSParametrisation();

   SetTPCPidResponseMaster();
   SetTPCParametrisation();

   fPIDResponse->GetTOFResponse().SetTimeResolution(fTOFres);
}

void AliPIDResponseInputHandler::SetRecoInfo()
{
   //
   // Set reconstruction information
   //

   //reset information
   fRecoPass = 0;
   fLHCperiod = "";
   fMCperiodTPC = "";

   fBeamType = "";

   //Get the current file to check the reconstruction pass (UGLY, but not stored in ESD... )
//   AliAnalysisManager *mgr=AliAnalysisManager::GetAnalysisManager();
   AliVEventHandler *inputHandler = fMCurrentMutliIH->GetFirstInputEventHandler();
   if (!inputHandler) return;

   TTree *tree = (TTree*)inputHandler->GetTree();
   TFile *file = (TFile*)tree->GetCurrentFile();

   if (!file) {
      AliError("Current file not found, cannot set reconstruction information");
      return;
   }

   fBeamType = "PP";

   //find the period by run number (UGLY, but not stored in ESD and AOD... )
   if (fRun >= 114737 && fRun <= 117223)      { fLHCperiod = "LHC10B"; fMCperiodTPC = "LHC10D1";  }
   else if (fRun >= 118503 && fRun <= 121040) { fLHCperiod = "LHC10C"; fMCperiodTPC = "LHC10D1";  }
   else if (fRun >= 122195 && fRun <= 126437) { fLHCperiod = "LHC10D"; fMCperiodTPC = "LHC10F6A"; }
   else if (fRun >= 127719 && fRun <= 130850) { fLHCperiod = "LHC10E"; fMCperiodTPC = "LHC10F6A"; }
   else if (fRun >= 133004 && fRun <= 135029) { fLHCperiod = "LHC10F"; fMCperiodTPC = "LHC10F6A"; }
   else if (fRun >= 135654 && fRun <= 136377) { fLHCperiod = "LHC10G"; fMCperiodTPC = "LHC10F6A"; }
   else if (fRun >= 136851 && fRun <= 139517) { fLHCperiod = "LHC10H"; fMCperiodTPC = "LHC10H8"; fBeamType = "PBPB"; }
   else if (fRun >= 139699) { fLHCperiod = "LHC11A"; fMCperiodTPC = "LHC10F6A"; }

   //find pass from file name (UGLY, but not stored in ESD... )
   TString fileName(file->GetName());
   if (fileName.Contains("/pass1")) {
      fRecoPass = 1;
   } else if (fileName.Contains("/pass2")) {
      fRecoPass = 2;
   } else if (fileName.Contains("/pass3")) {
      fRecoPass = 3;
   }
}

void AliPIDResponseInputHandler::SetITSParametrisation()
{
   //
   // Set the ITS parametrisation
   //
}
void AliPIDResponseInputHandler::SetTPCPidResponseMaster()
{
   //
   // Load the TPC pid response functions from the OADB
   //

   //reset the PID response functions
   delete fArrPidResponseMaster;
   fArrPidResponseMaster = 0;

   TString fileName(Form("%s/COMMON/PID/data/TPCPIDResponse.root", AliAnalysisManager::GetOADBPath()));

   TFile f(fileName.Data());
   if (f.IsOpen() && !f.IsZombie()) {
      fArrPidResponseMaster = dynamic_cast<TObjArray*>(f.Get("TPCPIDResponse"));
      f.Close();
   }

   if (!fArrPidResponseMaster) {
      AliFatal("Could not retrieve the TPC pid response");
      return;
   }
   fArrPidResponseMaster->SetOwner();
}

void AliPIDResponseInputHandler::SetTPCParametrisation()
{
   //
   // Change BB parametrisation for current run
   //

   if (fLHCperiod.IsNull()) {
      AliFatal("No period set, not changing parametrisation");
      return;
   }

   //
   // Set default parametrisations for data and MC
   //

   //data type
   TString datatype = "DATA";
   //in case of mc fRecoPass is per default 1
   if (fIsMC) {
      datatype = "MC";
      fRecoPass = 1;
   }

   //
   //set the PID splines
   //
   TString period = fLHCperiod;
   if (fArrPidResponseMaster) {
      TObject *grAll = 0x0;
      //for MC don't use period information
//     if (fIsMC) period="[A-Z0-9]*";
      //for MC use MC period information
      if (fIsMC) period = fMCperiodTPC;
//pattern for the default entry (valid for all particles)
      TPRegexp reg(Form("TSPLINE3_%s_([A-Z]*)_%s_PASS%d_%s_MEAN", datatype.Data(), period.Data(), fRecoPass, fBeamType.Data()));

      //loop over entries and filter them
      for (Int_t iresp = 0; iresp < fArrPidResponseMaster->GetEntriesFast(); ++iresp) {
         TObject *responseFunction = fArrPidResponseMaster->At(iresp);
         if (responseFunction == 0x0) continue;
         TString responseName = responseFunction->GetName();

         if (!reg.MatchB(responseName)) continue;

         TObjArray *arr = reg.MatchS(responseName);
         TString particleName = arr->At(1)->GetName();
         delete arr;
         if (particleName.IsNull()) continue;
         if (particleName == "ALL") grAll = responseFunction;
         else {
            //find particle id
            for (Int_t ispec = 0; ispec < AliPID::kSPECIES; ++ispec) {
               TString particle = AliPID::ParticleName(ispec);
               particle.ToUpper();
               if (particle == particleName) {
                  //test if there is already a function set. If yes, cleanup
                  TObject *old = const_cast<TObject*>(fPIDResponse->GetTPCResponse().GetResponseFunction((AliPID::EParticleType)ispec));
                  if (old) delete old;
                  fPIDResponse->GetTPCResponse().SetResponseFunction((AliPID::EParticleType)ispec, responseFunction);
                  fPIDResponse->GetTPCResponse().SetUseDatabase(kTRUE);
                  AliInfo(Form("Adding graph: %d - %s", ispec, responseFunction->GetName()));
                  break;
               }
            }
         }
      }

      //set default response function to all particles which don't have a specific one
      if (grAll) {
         for (Int_t ispec = 0; ispec < AliPID::kSPECIES; ++ispec) {
            if (!fPIDResponse->GetTPCResponse().GetResponseFunction((AliPID::EParticleType)ispec)) {
               fPIDResponse->GetTPCResponse().SetResponseFunction((AliPID::EParticleType)ispec, grAll);
               AliInfo(Form("Adding graph: %d - %s", ispec, grAll->GetName()));
            }
         }
      }
   }

   //
   // Setup resolution parametrisation
   //

   //default
   fPIDResponse->GetTPCResponse().SetSigma(3.79301e-03, 2.21280e+04);

   if (fRun >= 122195) {
      fPIDResponse->GetTPCResponse().SetSigma(2.30176e-02, 5.60422e+02);
   }
//   if ( fBeamType == "PBPB" ){
//     Double_t corrSigma=GetMultiplicityCorrectionSigma(GetTPCMultiplicityBin());
//     fPIDResponse->GetTPCResponse().SetSigma(3.79301e-03*corrSigma, 2.21280e+04);
//   }
}




