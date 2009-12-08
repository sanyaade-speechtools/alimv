#ifndef __CINT__
#endif

AliAnalysisManager *AliRsnTrainOfficial(Int_t debugAnalysisMgr = 0) {

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  if (!mgr)
    mgr = new AliAnalysisManager("RSN Train");

  mgr->SetDebugLevel(debugAnalysisMgr);

  // it has to be here
  gROOT->LoadMacro("AnalysisTrainNew.C");
  
  
  Bool_t useAod = 0;
  Bool_t useKine = 0;
  Bool_t useAODOut = 0;
//   useAod = 1;
  useKine = 1;
//   useAODOut = 1;
  
    
  SetCorrectHandlersRsn(useKine ,useAod,useAODOut);

  // train Task
//   gROOT->LoadMacro("AddAnalysisTrainProcess.C");
//   AddAnalysisTrainProcess(100);

//   mySettings();
  LoadConfig("myConfig.C");
  
  AddAnalysisTasks();

  return mgr;
}

void SetCorrectHandlersRsn(Bool_t useKine = kTRUE, Bool_t useAod = kFALSE, Bool_t useAODOut = kFALSE) {

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  AliAnalysisDataContainer *cin = mgr->GetCommonInputContainer();

  if (cin) return;

  if (useAod) {
      AliAODInputHandler *aodInputHandler = dynamic_cast<AliAODInputHandler*>(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());

      if (!aodInputHandler) {
          Info("AliRsnTrain.C", "Creating aodInputHandler ...");
          aodInputHandler = new AliAODInputHandler();
          mgr->SetInputEventHandler(aodInputHandler);
        }
    } else {
      AliESDInputHandler *esdInputHandler = dynamic_cast<AliESDInputHandler*>(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());

      if (!esdInputHandler) {
          Info("AliRsnTrain.C", "Creating esdInputHandler ...");
          esdInputHandler = new AliESDInputHandler();
          mgr->SetInputEventHandler(esdInputHandler);
        }

      if (useKine) {
          AliMCEventHandler* mcInputHandler = dynamic_cast<AliMCEventHandler*>(AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler());

          if (!mcInputHandler) {
              Info("AliRsnTrain.C", "Creating mcInputHandler ...");
              AliMCEventHandler* mcInputHandler = new AliMCEventHandler();
              mgr->SetMCtruthEventHandler(mcInputHandler);
            }
        }

    }
    
    
    if (useAODOut) {
      Info("AliRsnTrain.C", "Creating aodHandler ...");

      // AOD output handler
      AliAODHandler* aodHandler   = new AliAODHandler();
      aodHandler->SetOutputFileName("AliAOD.root");
      mgr->SetOutputEventHandler(aodHandler);
      if (iAODanalysis) {
         aodHandler->SetFillAOD(kFALSE);
         aodHandler->SetCreateNonStandardAOD();
         if (iJETAN)         aodHandler->SetOutputFileName("AliAOD.Jets.root");
//         if (iPWG3vertexing) aodHandler->SetOutputFileName("AliAOD.VertexingHF.root");
      }
   }
    
    
}

void mySettings() {
Bool_t      useDATE            = kFALSE; // use date in train name
Bool_t      useDBG             = kTRUE; // activate debugging
Bool_t      useMC              = kTRUE;  // use MC info
Bool_t      useTAGS            = kFALSE; // use ESD tags for selection
Bool_t      useKFILTER         = kTRUE;  // use Kinematics filter
Bool_t      useTR              = kTRUE;  // use track references
Bool_t      useCORRFW          = kFALSE; // do not change
Bool_t      useAODTAGS         = kTRUE;  // use AOD tags
Bool_t      saveTrain          = kTRUE;  // save train configuration as:
Bool_t      saveProofToAlien   = kFALSE; // save proof outputs in AliEn

Int_t       iAODanalysis       = 0;      // Analysis on input AOD's
Int_t       iAODhandler        = 1;      // Analysis produces an AOD or dAOD's
Int_t       iESDfilter         = 0; //OK      // ESD to AOD filter (barrel + muon tracks)
Int_t       iMUONcopyAOD       = 0;      // Task that copies only muon events in a separate AOD (PWG3)
Int_t       iJETAN             = 0;      // Jet analysis (PWG4) - needs ESD filter
Int_t       iPWG4partcorr      = 0;      // Gamma-hadron correlations task (PWG4)
Int_t       iPWG4gammaconv     = 0;      // Gamma conversion analysis (PWG4)  # NOT YET WORKING - WAITING FOR FEEDBACK #
Int_t       iPWG4omega3pi      = 1; //OK      // Omega to 3 pi analysis (PWG4)
Int_t       iPWG3vertexing     = 1; //OK     // Vertexing HF task (PWG2)
Int_t       iPWG2femto         = 0;      // Femtoscopy task (PWG2)
Int_t       iPWG2spectra       = 1; //OK      // Spectra tasks (PWG2
Int_t        iPWG2protons      = 1; //OK         // Proton-antiproton analysis
Int_t        iPWG2checkcascade = 1; //OK        // Check cascades task
Int_t        iPWG2perfcascade  = 1; //OK         // Check performance cascade
Int_t        iPWG2checkv0      = 1; //OK         // Check V0 task
Int_t        iPWG2strange      = 1; //OK         // Strangeness task
Int_t       iPWG2flow          = 0;      // Flow analysis tasks (PWG2)   = crashes in AliCFManager
Int_t       iPWG2res           = 0;      // Resonances task (PWG2)
Int_t       iPWG2kink          = 1; //OK      // Kink analysis tasks (PWG2)
Int_t        iPWG2kinkESDMC    = 0;         // Kink ESD-MC comparison (PWG2)
Int_t        iPWG2kinkres      = 0;         // Kink resonances (PWG2)
Int_t        iPWG2kinklikesign = 0;         // Kink like-sign (PWG2)
Int_t       iPWG2evchar        = 1; //OK      // Event characteristics (PWG2)
Int_t       iPWG2unicor        = 0;      // Unicor analysis (PWG2)
}
