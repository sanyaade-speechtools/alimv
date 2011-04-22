#ifndef __CINT__//|
#include <PWG2resonancesDevel/AliRsnInputHandler.h> //|
#include <AliPID.h>
#include <PWG2resonances/RESONANCES/AliRsnCutPID.h>//|
#include <PWG2resonances/RESONANCES/AliRsnCutSet.h>//|
#include <PWG2resonances/RESONANCES/AliRsnCutPIDITS.h>//|
#include <PWG2resonances/RESONANCES/AliRsnCutPIDTOF.h>//|
#include <PWG2resonances/RESONANCES/AliRsnCutPIDTPC.h>//|
#include <PWG2resonances/RESONANCES/AliRsnDaughterSelector.h>//|
#endif//|

void AddRsnInputHandler(AliMultiInputEventHandler *multiInputHandler,Bool_t useMC=kFALSE)
{
   if (multiInputHandler) {

//    gROOT->LoadMacro("AddCutsToRsnInputHandler.C");
//    AddCutsToRsnInputHandler(multiInputHandler, useMC);

	AddRsnInputHandlerBPID(kFALSE, multiInputHandler);

   }
}



void AddRsnInputHandlerBPID(Bool_t isMC, AliMultiInputEventHandler *multi) {
	
	   //---------------------------------------------
   //  Define single cuts
   //---------------------------------------------

   // Track quality for ITS standalone:
   // this cut is used to select tracks of good quality, irrespective of the PID.
   // When adding status flags, the second argument tells if each considered flag
   // must be active or not in the track status, since the ITS-SA tracks need that
   // some of them are OFF (e.g.: kTPCin)
   AliRsnCutTrackQuality *cutQualityITS = new AliRsnCutTrackQuality("cutQualityITS");
   cutQualityITS->AddStatusFlag(AliESDtrack::kITSin    , kTRUE);
   cutQualityITS->AddStatusFlag(AliESDtrack::kTPCin    , kFALSE);
   cutQualityITS->AddStatusFlag(AliESDtrack::kITSrefit , kTRUE);
   cutQualityITS->AddStatusFlag(AliESDtrack::kTPCrefit , kFALSE);
   cutQualityITS->AddStatusFlag(AliESDtrack::kITSpureSA, kFALSE);
   cutQualityITS->AddStatusFlag(AliESDtrack::kITSpid   , kTRUE);
   cutQualityITS->SetPtRange(0.15, 1E+20);
   cutQualityITS->SetEtaRange(-0.8, 0.8);
   cutQualityITS->SetDCARPtFormula("0.0595+0.0182/pt^1.55");
   cutQualityITS->SetDCAZmax(2.0);
   cutQualityITS->SetSPDminNClusters(1);
   cutQualityITS->SetITSminNClusters(4);
   cutQualityITS->SetITSmaxChi2(2.0);
   cutQualityITS->SetTPCminNClusters(0);
   cutQualityITS->SetTPCmaxChi2(1E+10);
   cutQualityITS->SetRejectKinkDaughters();
      
   // Track quality for TPC+ITS:
   // works exactly like the one above, but has settings for selecting TPC+ITS tracks
   // in this case, the flags required are all necessary, so here the procedure is simpler
   AliRsnCutTrackQuality *cutQualityTPC = new AliRsnCutTrackQuality("cutQualityTPC");
   cutQualityTPC->AddStatusFlag(AliESDtrack::kTPCin   , kTRUE);
   cutQualityTPC->AddStatusFlag(AliESDtrack::kTPCrefit, kTRUE);
   cutQualityTPC->AddStatusFlag(AliESDtrack::kITSrefit, kTRUE);
   cutQualityTPC->SetPtRange(0.15, 1E+20);
   cutQualityTPC->SetEtaRange(-0.8, 0.8);
   cutQualityTPC->SetDCARPtFormula("0.0182+0.0350/pt^1.01");
   cutQualityTPC->SetDCAZmax(2.0);
   cutQualityTPC->SetSPDminNClusters(1);
   cutQualityTPC->SetITSminNClusters(0);
   cutQualityTPC->SetITSmaxChi2(1E+20);
   cutQualityTPC->SetTPCminNClusters(70);
   cutQualityTPC->SetTPCmaxChi2(4.0);
   cutQualityTPC->SetRejectKinkDaughters();
	
	
	AliRsnCutPID *cutBPIDKaon = new AliRsnCutPID("cutBPIDKaon", AliPID::kKaon, 0.0, kFALSE);
	
	
	AliRsnCutSet *cutsQualityITS = new AliRsnCutSet("qualityITS", AliRsnTarget::kDaughter);
   AliRsnCutSet *cutsQualityTPC = new AliRsnCutSet("qualityTPC", AliRsnTarget::kDaughter);
	AliRsnCutSet *cutsKaonITS    = new AliRsnCutSet("kaonITS"   , AliRsnTarget::kDaughter);
   AliRsnCutSet *cutsKaonTPC    = new AliRsnCutSet("kaonTPC"   , AliRsnTarget::kDaughter);
   AliRsnCutSet *cutsKaonAll    = new AliRsnCutSet("kaonAll"   , AliRsnTarget::kDaughter);
	
	   // ITS standalone: quality only
   cutsQualityITS->AddCut(cutQualityITS);
   cutsQualityITS->SetCutScheme(cutQualityITS->GetName());
   
   // TPC+ITS: quality only
   cutsQualityTPC->AddCut(cutQualityTPC);
   cutsQualityTPC->SetCutScheme(cutQualityTPC->GetName());
	
   // ITS standalone: quality and ITS PID
   cutsKaonITS->AddCut(cutQualityITS);
   cutsKaonITS->AddCut(cutBPIDKaon);
   cutsKaonITS->SetCutScheme(Form("%s&%s", cutQualityITS->GetName(), cutBPIDKaon->GetName()));
   
   // TPC standalone: quality and TPC PID
   cutsKaonTPC->AddCut(cutQualityTPC);
   cutsKaonTPC->AddCut(cutBPIDKaon);
   cutsKaonTPC->SetCutScheme(Form("%s&%s", cutQualityTPC->GetName(), cutBPIDKaon->GetName()));
   
   // all tracks: both qualities and PIDs
   cutsKaonAll->AddCut(cutQualityITS);
   cutsKaonAll->AddCut(cutQualityTPC);
   cutsKaonAll->AddCut(cutBPIDKaon);
   cutsKaonAll->SetCutScheme(Form("(%s)|(%s)", cutsKaonITS->GetCutScheme().Data(), cutsKaonTPC->GetCutScheme().Data()));
	

	
	   // setup selector in the handler and add RSN input handler
   AliRsnInputHandler *rsnIH = new AliRsnInputHandler();
   AliRsnDaughterSelector *sel = rsnIH->GetSelector();
//    sel->Add(cutsQualityITS, kTRUE);
// 	sel->Add(cutsQualityTPC, kTRUE);
// 	sel->Add(cutsKaonITS, kTRUE);
	sel->Add(cutsKaonTPC, kTRUE);
// 	sel->Add(cutsKaonAll, kTRUE);
	
   sel->Init();
   multi->AddInputEventHandler(rsnIH);
}

AliRsnCutSet *GetMyPIDCuts()
{

   Double_t sigma[] = { -3.0, 3.0 };

   AliRsnCutPIDITS *itsCut = new AliRsnCutPIDITS("cutITS", AliPID::kKaon, sigma[0], sigma[1], kTRUE);

   AliRsnCutPIDTPC *tpcCut = new AliRsnCutPIDTPC("cutTPC", AliPID::kKaon, sigma[0], sigma[1], kTRUE);

   AliRsnCutPIDTOF *tofCut = new AliRsnCutPIDTOF("cutTOF", AliPID::kKaon, sigma[0], sigma[1], kTRUE);

   TString cutSheme;
   AliRsnCutSet *cut2010Set = new AliRsnCutSet("MyPIDCuts", AliRsnCut::kDaughter);
   cut2010Set->AddCut(itsCut);
   cutSheme += itsCut->GetName();
   cutSheme += "&";
   cut2010Set->AddCut(tpcCut);
   cutSheme += tpcCut->GetName();
   cutSheme += "&";
   cut2010Set->AddCut(tofCut);
   cutSheme += tpcCut->GetName();

   cut2010Set->SetCutScheme(cutSheme.Data());

   return cut2010Set;
}

AliRsnCutSet *GetCut2010Kaon(TString format = "esd", Bool_t realPID = kTRUE, Bool_t perfPID = kFALSE, Bool_t isSim = kFALSE, Bool_t addITSSA = kFALSE)
{


   AliRsnCutSet *cut2010Set = new AliRsnCutSet("cut2010Set", AliRsnCut::kDaughter);

   gROOT->LoadMacro("$(ALICE_ROOT)/PWG2/RESONANCES/macros/train/LHC2010-7TeV-phi/ConfigESDCutsITS.C");
   gROOT->LoadMacro("$(ALICE_ROOT)/PWG2/RESONANCES/macros/train/LHC2010-7TeV-phi/ConfigESDCutsTPC.C");

   TString suffix = "";
   // track cut -----------------------
   // --> global cuts for 2010 analysis
   // --> most options are set to right values by default
   AliRsnCutESD2010 *cuts2010_esd = new AliRsnCutESD2010(Form("cuts2010_esd%s", suffix.Data()));

   // ----> set all other defaults
   ConfigESDCutsTPC(cuts2010_esd->GetCutsTPC());
   ConfigESDCutsITS(cuts2010_esd->GetCutsITS());

   cut2010Set->AddCut(cuts2010_esd);
   cut2010Set->SetCutScheme(cuts2010_esd->GetName());

   return cut2010Set;

//   AliRsnCutAOD2010 *cuts2010_aod = new AliRsnCutAOD2010(Form("cuts2010_aod_%s", suffix.Data()));
   // ----> set the flag for sim/data management (which sets some other options)
   cuts2010_esd->SetMC(isSim);
//   cuts2010_aod->SetMC(isSim);
   // ----> include or not the ITS standalone (TPC is always in)
   cuts2010_esd->SetUseITSTPC(kTRUE);
   cuts2010_esd->SetUseITSSA(addITSSA);
   //cuts2010_aod->SetUseITSTPC(kTRUE);
   //cuts2010_aod->SetUseITSSA (addITSSA);
   // ----> require to check PID or not, depending on the label
   if (realPID) {
      // if doing realistic PID, it must be activated
      cuts2010_esd->SetCheckITS(kTRUE);
      cuts2010_esd->SetCheckTPC(kTRUE);
      cuts2010_esd->SetCheckTOF(kTRUE);
//     cuts2010_esd->SetCheckTOF (kFALSE);
   } else {
      // otherwise (both for no pid and perfect PID)
      // the PID cuts are deactivated
      cuts2010_esd->SetCheckITS(kFALSE);
      cuts2010_esd->SetCheckTPC(kFALSE);
      cuts2010_esd->SetCheckTOF(kFALSE);
   }

   // track cut -----------------------------
   // --> perfect PID for check of PID issues
   AliRsnCutPID *cutPID = new AliRsnCutPID("cutPID", AliPID::kKaon, 0.0, kTRUE);

   // cut set for tracks------------------------
   // --> only common cuts for tracks are needed
   // --> standard 2010 cuts are applied always
   TString cutSchemeTrack;
   if (!format.CompareTo("esd")) {
      cut2010Set->AddCut(cuts2010_esd);
      cutSchemeTrack += cuts2010_esd->GetName();
   } else if (!format.CompareTo("aod")) {
//     cutSchemeTrack += cuts2010_aod->GetName();
   } else {
      Error("Required ESD or AOD");
      return 0;
   }

   if (perfPID) {
      cut2010Set->AddCut(cutPID);
      cutSchemeTrack += "&cutPID";
   }

   cut2010Set->SetCutScheme(cutSchemeTrack.Data());

   return cut2010Set;

}