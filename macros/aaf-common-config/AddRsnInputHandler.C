#ifndef __CINT__//|
#include <PWG2resonancesDevel/AliRsnInputHandler.h> //|
#include <AliPID.h>
#include <PWG2resonances/RESONANCES/AliRsnCutPID.h>//|
#include <PWG2resonances/RESONANCES/AliRsnCutSet.h>//|
#include <PWG2resonances/RESONANCES/AliRsnCutPIDITS.h>//|
#include <PWG2resonances/RESONANCES/AliRsnCutPIDTOF.h>//|
#include <PWG2resonances/RESONANCES/AliRsnCutPIDTPC.h>//|
#endif//|

void AddRsnInputHandler(AliMultiInputEventHandler *multiInputHandler)
{
   if (multiInputHandler) {
//       AliRsnInputHandler *rsnIH = new AliRsnInputHandler();
//       AliRsnPIDManager *pidMgr =  rsnIH->GetPIDManager();
//
//       pidMgr->SetCut(GetBPID(), AliPID::kKaon);
//
// //       pidMgr->SetCut(GetCut2010Kaon(), AliPID::kKaon);
//
// //       pidMgr->SetCut(GetMyPIDCuts(), AliPID::kKaon);
//
//       pidMgr->Print();
//
//       multiInputHandler->AddInputEventHandler(rsnIH);

      AddRsnInputHandlerAlberto(kFALSE, multiInputHandler);
   }
}

void AddRsnInputHandlerAlberto(Bool_t isMC, AliMultiInputEventHandler *multi)
{
   void myError(const char * msg) {::Error("AddRsnInputHandler", msg);}
   void myWarning(const char * msg) {::Warning("AddRsnInputHandler", msg);}
   void myInfo(const char * msg) {::Info("AddRsnInputHandler", msg);}

   if (!multi) {
      ::Error("AddRsnInputHandler", "Required a WELL INITIALIZED AliMultiInputEventHandler object");
      return;
   }

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

   // ITS PID for Kaons
   // In this implementation, it is a 3sigma cut around the Bethe-Bloch value.
   //
   // NOTE:
   // --> The initialization of the BB is different between data and MC.
   // --> The cut is the same for all momenta.
   AliRsnCutPIDITS *cutPIDITSkaon = new AliRsnCutPIDITS("cutPIDITSkaon", AliPID::kKaon, -3.0, 3.0);
   cutPIDITSkaon->SetMC(isMC);
   cutPIDITSkaon->SetMomentumRange(0.0, 1E20);

   // TPC PID for Kaons
   // In this implementation, there are two instances:
   // - below 350 MeV --> 5 sigma cut
   // - above 350 MeV --> 3 sigma cut
   //
   // NOTE:
   // --> The initialization of the BB is different between data and MC.
   AliRsnCutPIDTPC *cutPIDTPCkaonLow  = new AliRsnCutPIDTPC("cutPIDTPCkaonLow" , AliPID::kKaon, -5.0, 5.0);
   AliRsnCutPIDTPC *cutPIDTPCkaonHigh = new AliRsnCutPIDTPC("cutPIDTPCkaonHigh", AliPID::kKaon, -3.0, 3.0);

   // assign the momentum range and tell to reject tracks outside it
   cutPIDTPCkaonLow ->SetMomentumRange(0.00, 0.35);
   cutPIDTPCkaonHigh->SetMomentumRange(0.35, 1E20);
   cutPIDTPCkaonLow ->SetRejectOutside(kTRUE);
   cutPIDTPCkaonHigh->SetRejectOutside(kTRUE);

   // BB parameterization depends on data sample (MC, data)
   // the momentum range is passed and tracks outside it are rejected
   Double_t bbPar[5];
   if (isMC) {
      bbPar[0] = 2.15898 / 50.0;
      bbPar[1] = 1.75295E1;
      bbPar[2] = 3.40030E-9;
      bbPar[3] = 1.96178;
      bbPar[4] = 3.91720;
   } else {
      bbPar[0] = 1.41543 / 50.0;
      bbPar[1] = 2.63394E1;
      bbPar[2] = 5.0411E-11;
      bbPar[3] = 2.12543;
      bbPar[4] = 4.88663;
   }
   cutPIDTPCkaonLow ->SetBBParam(bbPar);
   cutPIDTPCkaonHigh->SetBBParam(bbPar);

   // TOF PID for Kaons
   // In this implementation it is a 3sigma cout aroung expected kaon time.
   //
   // NOTE:
   // --> It is important to choose if this cut must reject tracks not matched in TOF.
   //     Usually, if TPC pid is used, we can accept them, otherwise we must reject.
   //     (here we assume TPC is used)
   AliRsnCutPIDTOF *cutPIDTOFkaon = new AliRsnCutPIDTOF("cutPIDTOFkaon", AliPID::kKaon, -3.0, 3.0);
   cutPIDTOFkaon->SetRejectUnmatched(kFALSE);

   //---------------------------------------------
   //  Combine cuts
   //---------------------------------------------

   // make several combinations of cuts:
   // ITS and TPC standard
   AliRsnCutSet *cutsKaonITS = new AliRsnCutSet("kaonITS", AliRsnTarget::kDaughter);
   AliRsnCutSet *cutsKaonTPC = new AliRsnCutSet("kaonTPC", AliRsnTarget::kDaughter);
   AliRsnCutSet *cutsKaonAll = new AliRsnCutSet("kaonAll", AliRsnTarget::kDaughter);

   // ITS standalone: quality and ITS PID
   cutsKaonITS->AddCut(cutQualityITS);
   cutsKaonITS->AddCut(cutPIDITSkaon);
   cutsKaonITS->SetCutScheme(Form("%s&%s", cutQualityITS->GetName(), cutPIDITSkaon->GetName()));

   // TPC standalone: quality and TPC PID
   cutsKaonTPC->AddCut(cutQualityTPC);
   cutsKaonTPC->AddCut(cutPIDTPCkaonLow);
   cutsKaonTPC->AddCut(cutPIDTPCkaonHigh);
   cutsKaonTPC->AddCut(cutPIDTOFkaon);
   cutsKaonTPC->SetCutScheme(Form("%s&(%s|%s)&%s", cutQualityTPC->GetName(), cutPIDTPCkaonLow->GetName(), cutPIDTPCkaonHigh->GetName(), cutPIDTOFkaon->GetName()));

   // all tracks: both qualities and PIDs
   cutsKaonAll->AddCut(cutQualityITS);
   cutsKaonAll->AddCut(cutQualityTPC);
   cutsKaonAll->AddCut(cutPIDITSkaon);
   cutsKaonAll->AddCut(cutPIDTPCkaonLow);
   cutsKaonAll->AddCut(cutPIDTPCkaonHigh);
   cutsKaonAll->AddCut(cutPIDTOFkaon);
   cutsKaonAll->SetCutScheme(Form("(%s)|(%s)", cutsKaonITS->GetCutScheme().Data(), cutsKaonTPC->GetCutScheme().Data()));

   // setup selector in the handler and add RSN input handler
   AliRsnInputHandler *rsnIH = new AliRsnInputHandler();
   AliRsnDaughterSelector *sel = rsnIH->GetSelector();
   sel->Add(cutsKaonITS, kTRUE);
   sel->Add(cutsKaonTPC, kTRUE);
   sel->Add(cutsKaonAll, kTRUE);
   sel->Init();
   multi->AddInputEventHandler(rsnIH);
}



AliRsnCutSet *GetBPID(Bool_t usePerfectPID = kFALSE)
{
   AliRsnCutPID *cutBPIDKaon = new AliRsnCutPID("cutBPIDKaon", AliPID::kKaon, 0.0, usePerfectPID);
   AliRsnCutSet *cutBPIDKaonSet = new AliRsnCutSet("cutBPIDKaonSet", AliRsnCut::kDaughter);
   cutBPIDKaonSet->AddCut(cutBPIDKaon);
   cutBPIDKaonSet->SetCutScheme("cutBPIDKaon");
   return cutBPIDKaonSet;
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