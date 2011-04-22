//
// *** Configuration script for phi->KK analysis with 2010 runs ***
// 
// A configuration script for RSN package needs to define the followings:
//
// (1) decay tree of each resonance to be studied, which is needed to select
//     true pairs and to assign the right mass to all candidate daughters
// (2) cuts at all levels: single daughters, tracks, events
// (3) output objects: histograms or trees
//
Bool_t RsnConfigPhiTPC
(
   AliRsnAnalysisTask *task,
   Bool_t              isMC,
   Bool_t              isMix,
   Bool_t              useCentrality,
   AliRsnCutSet       *eventCuts
)
{
   if (!task) ::Error("RsnConfigPhiTPC", "NULL task");
   
   // we define here a suffix to differentiate names of different setups for the same resonance
   // and we define also the name of the list of tracks we want to select for the analysis
   // (if will fail if no lists with this name were added to the RsnInputHandler)
   const char *suffix     = "tpc";
   const char *listName   = "kaonTPC";
   Bool_t      useCharged =  kTRUE;
   Int_t       listID     = -1;
   
   // find the index of the corresponding list in the RsnInputHandler
   AliAnalysisManager        *mgr   = AliAnalysisManager::GetAnalysisManager();
   AliMultiInputEventHandler *multi = dynamic_cast<AliMultiInputEventHandler*>(mgr->GetInputEventHandler());
   if (multi) {
      TObjArray          *array = multi->InputEventHandlers();
      AliRsnInputHandler *rsn   = (AliRsnInputHandler*)array->FindObject("rsnInputHandler");
      if (rsn) {
         AliRsnDaughterSelector *sel = rsn->GetSelector();
         listID = sel->GetID(listName, useCharged);
      }
   }
   if (listID >= 0)
      ::Info ("RsnConfigPhiTPC.C", "Required list '%s' stays in position %d", listName, listID);
   else {
      ::Error("RsnConfigPhiTPC.C", "Required list '%s' absent in handler!", listName);
      return kFALSE;
   }
   
   // ----------------------------------------------------------------------------------------------
   // -- DEFINITIONS -------------------------------------------------------------------------------
   // ----------------------------------------------------------------------------------------------
   
   // PAIR DEFINITIONS:
   // this contains the definition of particle species and charge for both daughters of a resonance,
   // which are used for the following purposes:
   // --> species is used to assign the mass to the daughter (e.g. for building invariant mass)
   // --> charge is used to select what tracks to use when doing the computation loops
   // When a user wants to compute a like-sign background, he must define also a pair definition
   // for each like-sign: in case of charged track decays, we need one for ++ and one for --
   // Last two arguments are necessary only in some cases (but it is not bad to well initialize them):
   // --> PDG code of resonance, which is used for selecting true pairs, when needed
   // --> nominal resonance mass, which is used for computing quantities like Y or Mt
   AliRsnPairDef *phi_kaonP_kaonM = new AliRsnPairDef(AliRsnDaughter::kKaon, '+', AliRsnDaughter::kKaon, '-', 333, 1.019455);
   AliRsnPairDef *phi_kaonP_kaonP = new AliRsnPairDef(AliRsnDaughter::kKaon, '+', AliRsnDaughter::kKaon, '+', 333, 1.019455);
   AliRsnPairDef *phi_kaonM_kaonM = new AliRsnPairDef(AliRsnDaughter::kKaon, '-', AliRsnDaughter::kKaon, '-', 333, 1.019455);

   // PAIR LOOPS:
   // these are the objects which drive the computations and fill the output histograms
   // each one requires to be initialized with an AliRsnPairDef object, which provided masses,
   // last argument tells if the pair is for mixing or not (this can be also set afterwards, anyway)
   const Int_t     nPairs = 5;
   Bool_t          addPair[nPairs] = {1, 1, 1, 1, 1};
   AliRsnLoopPair *phiLoop[nPairs];
   phiLoop[0] = new AliRsnLoopPair(Form("%s_phi_kaonP_kaonM"     , suffix), phi_kaonP_kaonM, kFALSE);
   phiLoop[1] = new AliRsnLoopPair(Form("%s_phi_kaonP_kaonM_true", suffix), phi_kaonP_kaonM, kFALSE);
   phiLoop[2] = new AliRsnLoopPair(Form("%s_phi_kaonP_kaonM_mix" , suffix), phi_kaonP_kaonM, kTRUE );
   phiLoop[3] = new AliRsnLoopPair(Form("%s_phi_kaonP_kaonP"     , suffix), phi_kaonP_kaonP, kFALSE);
   phiLoop[4] = new AliRsnLoopPair(Form("%s_phi_kaonM_kaonM"     , suffix), phi_kaonM_kaonM, kFALSE);

   // set additional option for true pairs
   // 1) we select only pairs coming from the same mother, which must have the right PDG code (from pairDef)
   // 2) we select only pairs decaying according to the right channel (from pairDef species+charge definitions)
   phiLoop[1]->SetOnlyTrue(kTRUE);
   phiLoop[1]->SetCheckDecay(kTRUE);
   
   // don't add true pairs if not MC
   if (!isMC) addPair[1] = 0;
   addPair[0] = !isMix;
   addPair[1] = !isMix;
   addPair[2] =  isMix;
   addPair[3] = !isMix;
   addPair[4] = !isMix;
   
   // ----------------------------------------------------------------------------------------------
   // -- COMPUTED VALUES & OUTPUTS -----------------------------------------------------------------
   // ----------------------------------------------------------------------------------------------
   
   // All values which should be computed are defined here and passed to the computation objects,
   // since they define all that is computed bye each one, and, in case one output is a histogram
   // they define the binning and range for that value
   //
   // NOTE:
   // --> multiplicity bins have variable size
   
   Double_t mult[] = {  0.,  1.,  2.,  3.,  4.,  5.,   6.,   7.,   8.,   9.,  10.,  11.,  12., 13., 
                       14., 15., 16., 17., 18., 19.,  20.,  21.,  22.,  23.,  24.,  25.,  30., 35., 
                       40., 50., 60., 70., 80., 90., 100., 120., 140., 160., 180., 200., 500.};
   Int_t    nmult  = sizeof(mult) / sizeof(mult[0]);
   
   AliRsnValuePair *axisIM      = new AliRsnValuePair("IM" , AliRsnValuePair::kInvMass   );
   AliRsnValuePair *axisRes     = new AliRsnValuePair("RES", AliRsnValuePair::kInvMassRes);
   AliRsnValuePair *axisPt      = new AliRsnValuePair("PT" , AliRsnValuePair::kPt        );
   AliRsnValuePair *axisY       = new AliRsnValuePair("Y"  , AliRsnValuePair::kY);
   
   AliRsnValueEvent*axisCentV0  = new AliRsnValueEvent("CNT" , AliRsnValueEvent::kCentralityV0);
   AliRsnValueEvent*axisMultESD = new AliRsnValueEvent("MESD", AliRsnValueEvent::kMultESDCuts );
   AliRsnValueEvent*axisMultSPD = new AliRsnValueEvent("MSPD", AliRsnValueEvent::kMultSPD     );
   AliRsnValueEvent*axisMultTRK = new AliRsnValueEvent("MTRK", AliRsnValueEvent::kMult        );
   AliRsnValueEvent*axisMultMC  = new AliRsnValueEvent("MMC" , AliRsnValueEvent::kMultMC      );
   
   axisIM     ->SetBins(500, 0.9, 1.4);
   axisRes    ->SetBins(-0.5,   0.5, 0.001);
   axisPt     ->SetBins(50, 0.0, 5.0);
   axisY      ->SetBins(1, -0.5, 0.5);

   axisCentV0 ->SetBins(20, 0.0, 100.0);
   axisMultESD->SetBins(nmult, mult);
   axisMultSPD->SetBins(nmult, mult);
   axisMultTRK->SetBins(nmult, mult);
   axisMultMC ->SetBins(nmult, mult);

   // create outputs:
   // we define one for true pairs, where we add resolution, and another without it, for all others
   // it seems that it is much advantageous to use sparse histograms when adding more than 2 axes
   AliRsnListOutput *out[2];
   out[0] = new AliRsnListOutput("res"  , AliRsnListOutput::kHistoSparse);
   out[1] = new AliRsnListOutput("nores", AliRsnListOutput::kHistoSparse);
   
   // add values to outputs:
   // if centrality is required, we add it only, otherwise we add all multiplicities
   // other axes (invmass, pt) are always added
   for (Int_t i = 0; i < 2; i++) {
      out[i]->AddValue(axisIM);
      out[i]->AddValue(axisPt);
      out[i]->AddValue(axisY);
      if (useCentrality) {
         ::Info("RsnConfigPhiTPC.C", "Adding centrality axis");
         out[i]->AddValue(axisCentV0);
      } else {
         ::Info("RsnConfigPhiTPC.C", "Adding multiplicity axes");
         //out[i]->AddValue(axisMultESD);
         //out[i]->AddValue(axisMultSPD);
         out[i]->AddValue(axisMultTRK);
         if (isMC) out[i]->AddValue(axisMultMC);
      }
   }
   // resolution only in the first
   out[0]->AddValue(axisRes);
   
   // ----------------------------------------------------------------------------------------------
   // -- ADD SETTINGS TO LOOPS AND LOOPS TO TASK ---------------------------------------------------
   // ----------------------------------------------------------------------------------------------
   
   for (Int_t ip = 0; ip < nPairs; ip++) {
      // skip pairs not to be added
      if (!addPair[ip]) continue;
      // assign list IDs
      phiLoop[ip]->SetListID(0, listID);
      phiLoop[ip]->SetListID(1, listID);
      // assign event cuts
      phiLoop[ip]->SetEventCuts(eventCuts);
      // assign outputs
      if (ip != 1)
         phiLoop[ip]->AddOutput(out[1]);
      else
         phiLoop[ip]->AddOutput(out[0]);
      // add to task
      task->AddLoop(phiLoop[ip]);
   }
   
   return kTRUE;
}
