
/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

//-----------------------------------------------------------------------
// Author : R. Vernet, Consorzio Cometa - Catania (it)
//-----------------------------------------------------------------------

#include "Riostream.h"
#include "AliCFRsnTask.h"
#include "TCanvas.h"
#include "AliStack.h"
#include "TParticle.h"
#include "TH1.h"
#include "TH2.h"
#include "TChain.h"
#include "AliMCEvent.h"
#include "AliESDEvent.h"
#include "AliCFManager.h"
#include "AliCFCutBase.h"
#include "AliCFContainer.h"
#include "AliESDtrack.h"
#include "AliLog.h"
#include "AliRsnDaughter.h"
#include "AliCFPair.h"
#include "AliRsnPairParticle.h"
#include "AliRsnCut.h"
#include "AliAODEvent.h"
#include "AliESDVertex.h"
#include "AliMultiplicity.h"
#include "AliCFEffGrid.h"
#include "AliRsnPairDef.h"
#include "AliRsnCutStd.h"
#include "AliRsnCutBetheBloch.h"
#include "AliRsnPIDDefESD.h"
#include "AliRsnFunctionAxis.h"
#include "AliESDtrackCuts.h"

//__________________________________________________________________________
AliCFRsnTask::AliCFRsnTask() :
  AliAnalysisTaskSE(),
  fReadTPCTracks(0),
  fRsnPDG(0),
  fRsnMassMin(0.),
  fRsnMassMax(0.),
  fRsnEvent(),
  fCFManager(0x0),
  fHistEventsProcessed(0x0)
{
  //
  //Default ctor
  //
}
//___________________________________________________________________________
AliCFRsnTask::AliCFRsnTask(const Char_t* name) :
  AliAnalysisTaskSE(name),
  fReadTPCTracks(0),
  fRsnPDG(0),
  fRsnMassMin(0.),
  fRsnMassMax(0.),
  fRsnEvent(),
  fCFManager(0x0),
  fHistEventsProcessed(0x0)
{
  //
  // Constructor. Initialization of Inputs and Outputs
  //
  Info("AliCFRsnTask","Calling Constructor");
  /*
    DefineInput(0) and DefineOutput(0)
    are taken care of by AliAnalysisTaskSE constructor
  */
  DefineOutput(1, TH1I::Class());
  DefineOutput(2, AliCFContainer::Class());
  DefineOutput(3, AliCFContainer::Class());
  DefineOutput(4, TH1D::Class());
}

//___________________________________________________________________________
AliCFRsnTask& AliCFRsnTask::operator=(const AliCFRsnTask& c)
{
  //
  // Assignment operator
  //

  if (this != &c)
  {
    AliAnalysisTaskSE::operator=(c) ;
    fReadTPCTracks = c.fReadTPCTracks;
    fRsnPDG = c.fRsnPDG;
    fRsnEvent = c.fRsnEvent;
    fRsnMassMin = c.fRsnMassMin;
    fRsnMassMax = c.fRsnMassMax;
    fCFManager = c.fCFManager;
    fHistEventsProcessed = c.fHistEventsProcessed;
  }
  return *this;
}

//___________________________________________________________________________
AliCFRsnTask::AliCFRsnTask(const AliCFRsnTask& c) :
  AliAnalysisTaskSE(c),
  fReadTPCTracks(c.fReadTPCTracks),
  fRsnPDG(c.fRsnPDG),
  fRsnMassMin(c.fRsnMassMin),
  fRsnMassMax(c.fRsnMassMax),
  fCFManager(c.fCFManager),
  fHistEventsProcessed(c.fHistEventsProcessed)
{
  //
  // Copy Constructor
  //
}

//___________________________________________________________________________
AliCFRsnTask::~AliCFRsnTask()
{
  //
  //destructor
  //

  Info("~AliCFRsnTask","Calling Destructor");
  if (fCFManager)           delete fCFManager ;
  if (fHistEventsProcessed) delete fHistEventsProcessed;
}

//_________________________________________________
void AliCFRsnTask::UserExec(Option_t *)
{
  //
  // Main loop function
  //

  if (!fInputEvent) {
    Error("UserExec", "NO EVENT FOUND!");
    return;
  }

  if (!fMCEvent) Error("UserExec", "NO MC INFO FOUND!");
  fCFManager->SetEventInfo(fMCEvent);

  AliStack    *stack = fMCEvent->Stack();
  AliESDEvent *esd   = dynamic_cast<AliESDEvent*>(fInputEvent);
  if (!esd)
  {
    Error("UserExec", "Required ESD");
    return;
  }

  // fill container of events: step 0
  //Double_t evMult = ((AliESDEvent*)fInputEvent)->GetMultiplicity()->GetNumberOfTracklets();
  Double_t evMult = fInputEvent->GetNumberOfTracks();
  fCFManager->GetEventContainer()->Fill(&evMult, 0);

  // check primary vertex and eventually fill step 1
  // if a vertex with tracks was successfully reconstructed,
  // it is used for computing DCA;
  // otherwise, the one computed with SPD is used.
  // This is known from the "Status" parameter of the vertex itself.
  const AliESDVertex *v = esd->GetPrimaryVertex();
  if (!v->GetStatus() || (v->GetNContributors() < 3)) v = esd->GetPrimaryVertexSPD();
  if (!v->GetStatus() || (v->GetNContributors() < 3))
  {
    fHistEventsProcessed->Fill(0);
    PostData(1, fHistEventsProcessed);
    PostData(2, fCFManager->GetEventContainer());
    return;
  }
  fCFManager->GetEventContainer()->Fill(&evMult, 1);

  // ESD track cuts
  Double_t  cov11 = 2;
  Double_t  cov22 = 2;
  Double_t  cov33 = 0.5;
  Double_t  cov44 = 0.5;
  Double_t  cov55 = 2;
  Double_t  nSigmaToVertex = 4;
  Double_t  maxChi2PerClusterTPC = 3.5;
  Bool_t    requireTPCRefit = kTRUE;
  Bool_t    requireSigmaToVertex = kTRUE;
  Bool_t    acceptKinkDaughters = kFALSE;
  Int_t     minNClustersTPC = 50;
  AliESDtrackCuts *esdCuts = new AliESDtrackCuts;
  esdCuts->SetMaxCovDiagonalElements(cov11, cov22, cov33, cov44, cov55);
  esdCuts->SetRequireSigmaToVertex(requireSigmaToVertex);
  esdCuts->SetMaxNsigmaToVertex(nSigmaToVertex);
  esdCuts->SetRequireTPCRefit(requireTPCRefit);
  esdCuts->SetAcceptKinkDaughters(acceptKinkDaughters);
  esdCuts->SetMinNClustersTPC(minNClustersTPC);
  esdCuts->SetMaxChi2PerClusterTPC(maxChi2PerClusterTPC);

  // MC-event selection
  Double_t containerInput[3];
  containerInput[0] = evMult;

  // loop on the MC event
  for (Int_t ipart = 0; ipart < stack->GetNprimary(); ipart++)
  {
    AliMCParticle *mcPart  = fMCEvent->GetTrack(ipart);
    containerInput[1] = mcPart->Pt();
    containerInput[2] = mcPart->Eta();
    if (TMath::Abs(mcPart->Particle()->GetPdgCode()) != 333) continue;

    {
      Int_t idaughter1 = mcPart->Particle()->GetFirstDaughter();
      Int_t idaughter2 = mcPart->Particle()->GetLastDaughter();
      TParticle *daughter1 = stack->Particle(idaughter1);
      TParticle *daughter2 = stack->Particle(idaughter2);
      Double_t e1  = daughter1->Energy();
      Double_t e2  = daughter2->Energy();
      Double_t px1 = daughter1->Px();
      Double_t px2 = daughter2->Px();
      Double_t py1 = daughter1->Py();
      Double_t py2 = daughter2->Py();
      Double_t pz1 = daughter1->Pz();
      Double_t pz2 = daughter2->Pz();
      Double_t im = (e1+e2)*(e1+e2) - (px1+px2)*(px1+px2) - (py1+py2)*(py1+py2) - (pz1+pz2)*(pz1+pz2);
      if (im > 0) fHistoIM->Fill(TMath::Sqrt(im));
      static Int_t count = 1;
      cout << "COUNT: " << count++ << endl;
    }

    if (TMath::Abs(mcPart->Particle()->GetPdgCode()) == 333)
      fCFManager->GetParticleContainer()->Fill(containerInput, 9);

    // fill step 0
    // here, fix also the values of container input,
    // which are reused eventually in other MC steps
    if (!fCFManager->CheckParticleCuts(0, mcPart)) continue;
    fCFManager->GetParticleContainer()->Fill(containerInput, 0);

    // fill step 1:
    // here there is need to check the daughters,
    // so they must be converted in AliMCParticles
    Int_t idaughter1 = mcPart->Particle()->GetFirstDaughter();
    Int_t idaughter2 = mcPart->Particle()->GetLastDaughter();
    if (idaughter1 < 0 || idaughter2 < 0)
    {
      Error("UserExec", "IDaughters = %d, %d", idaughter1, idaughter2);
      continue;
    }
    TParticle *daughter1 = stack->Particle(idaughter1);
    TParticle *daughter2 = stack->Particle(idaughter2);
    AliMCParticle part1(daughter1, 0, idaughter1);
    AliMCParticle part2(daughter2, 0, idaughter2);
    if (!fCFManager->CheckParticleCuts(1, &part1)) continue;
    if (!fCFManager->CheckParticleCuts(1, &part2)) continue;
    fCFManager->GetParticleContainer()->Fill(containerInput, 1);

    // fill step 2:
    // reuse the daughters created before for cuts
    if (!fCFManager->CheckParticleCuts(2, &part1)) continue;
    if (!fCFManager->CheckParticleCuts(2, &part2)) continue;
    fCFManager->GetParticleContainer()->Fill(containerInput, 2);
  }

  // Now go to rec level
  // Create RSN interface for event
  fRsnEvent.SetRef(esd, fMCEvent);
  AliRsnDaughter rsnDaughter1;
  AliRsnDaughter rsnDaughter2;

  // set SPD multiplicity
  const AliMultiplicity* SPDMult = esd->GetMultiplicity();
  Int_t nTracklets = SPDMult->GetNumberOfTracklets();

  // priors
  Double_t priors[5] = {0.02, 0.02, 0.83, 0.07, 0.06};

  // bethe block cuts
  AliRsnCutBetheBloch cutBB10("cutKaon10", 0.1, AliPID::kKaon);
  AliRsnCutBetheBloch cutBB20("cutKaon20", 0.2, AliPID::kKaon);

  cutBB10.SetCalibConstant(0, 0.76176e-1);
  cutBB10.SetCalibConstant(1, 10.632);
  cutBB10.SetCalibConstant(2, 0.13279e-4);
  cutBB10.SetCalibConstant(3, 1.8631);
  cutBB10.SetCalibConstant(4, 1.9479);

  cutBB20.SetCalibConstant(0, 0.76176e-1);
  cutBB20.SetCalibConstant(1, 10.632);
  cutBB20.SetCalibConstant(2, 0.13279e-4);
  cutBB20.SetCalibConstant(3, 1.8631);
  cutBB20.SetCalibConstant(4, 1.9479);

  // true pair cuts
  AliRsnCutStd *cutTruePair = new AliRsnCutStd("cutTrue", AliRsnCutStd::kTruePair, fRsnPDG);

  // PID weights
  AliRsnPIDDefESD pidDefESD, pidDefTPC;
  pidDefESD.SetScheme(AliRsnPIDDefESD::kSchemeESD);
  pidDefTPC.SetScheme(AliRsnPIDDefESD::kSchemeTPC);

  // find kaon mass
  AliPID pid;
  Double_t im, kmass = pid.ParticleMass(AliPID::kKaon);

  // define evaluation axes
  AliRsnPairDef pairDef('+', AliPID::kKaon, '-', AliPID::kKaon);
  AliRsnFunctionAxis axisIM(AliRsnFunctionAxis::kPairInvMass,    1000,  0.0,   2.0);
  AliRsnFunctionAxis axisPt(AliRsnFunctionAxis::kPairPt,           10,  0.0,  10.0);
  AliRsnFunctionAxis axisEta(AliRsnFunctionAxis::kPairEta,          10, -1.0,   1.0);
  AliRsnFunctionAxis axisMult(AliRsnFunctionAxis::kEventMult,         8,  0.0, 200.0);

  // external loop on tracks
  for (Int_t iTrack1 = 0; iTrack1 < fInputEvent->GetNumberOfTracks(); iTrack1++)
  {
    AliVParticle* track1 = fInputEvent->GetTrack(iTrack1);
    Int_t label1 = TMath::Abs(track1->GetLabel());
    // connect interface to RSN package
    fRsnEvent.SetDaughter(rsnDaughter1, iTrack1);
    // reject non-kaons
    // if (rsnDaughter1.PerfectPID() != AliPID::kKaon) continue;
    // assigned required PID
    rsnDaughter1.SetRequiredPID(AliPID::kKaon);

    // loop on positive tracks
    for (Int_t iTrack2 = iTrack1 + 1; iTrack2 < fInputEvent->GetNumberOfTracks(); iTrack2++)
    {
      AliVParticle* track2 = fInputEvent->GetTrack(iTrack2);
      Int_t label2 = TMath::Abs(track2->GetLabel());
      // connect interface to RSN package
      fRsnEvent.SetDaughter(rsnDaughter2, iTrack2);
      // reject non-kaons
      // if (rsnDaughter2.PerfectPID() != AliPID::kKaon) continue;
      // assigned required PID
      rsnDaughter2.SetRequiredPID(AliPID::kKaon);

      // check previous cuts for coherence
      AliMCParticle part1(rsnDaughter1.GetParticle(), 0, label1);
      AliMCParticle part2(rsnDaughter1.GetParticle(), 0, label2);
      if (!fCFManager->CheckParticleCuts(1, &part1)) continue;
      if (!fCFManager->CheckParticleCuts(1, &part2)) continue;
      if (!fCFManager->CheckParticleCuts(2, &part1)) continue;
      if (!fCFManager->CheckParticleCuts(2, &part2)) continue;

      // make a mother resonance from the 2 candidate daughters
      AliRsnPairParticle rsnPair;
      rsnPair.SetPair(&rsnDaughter1, &rsnDaughter2);
      
      AliCFPair pair(track1,track2); // This object is used for cuts
                                     // (to be replaced when AliRsnPairParticle
                                     // inherits from AliVParticle)

      // check if true resonance
      if (!cutTruePair->IsSelected(AliRsnCut::kPair, &rsnPair)) continue;
      Info("UserExec", "Found a true reconstructed pair");

      // check if associated MC resonance passes the cuts
      //Int_t motherLabel = rsnPair.GetDaughter(0)->GetParticle()->GetFirstMother();
      //if (motherLabel < 0)
      //{
      //  Info("UserExec", "Rejecting pair with negative mother label");
      //  continue;
      //}
      //TParticle *mother = stack->Particle(motherLabel);
      //AliMCParticle aliMother(mother, 0, motherLabel);
      //if (!fCFManager->CheckParticleCuts(0, &aliMother)) continue;
      
      containerInput[0] = axisMult.Eval(&fRsnEvent); //(Double_t)nTracklets;
      containerInput[1] = axisPt.Eval(&rsnPair, &pairDef);
      containerInput[2] = axisEta.Eval(&rsnPair, &pairDef);

      // step 3: reconstruction
      im = axisIM.Eval(&rsnPair, &pairDef);
      if (im > fRsnMassMax || im < fRsnMassMin)
      {
        Info("UserExec", "Mass = %f -- range = %f - %f --> rejected", im, fRsnMassMin, fRsnMassMax);
        continue;
      }
      fCFManager->GetParticleContainer()->Fill(containerInput, 3);

      // step 4: primary
      /*
      if (!fCFManager->CheckParticleCuts(4, &pair))
      {
        Info("UserExec", "Failed cuts in step #4");
        continue;
      }
      */
      AliESDtrack *esdTrack1 = rsnDaughter1.GetRefESD();
      AliESDtrack *esdTrack2 = rsnDaughter2.GetRefESD();
      if (!esdCuts->IsSelected(esdTrack1)) continue;
      if (!esdCuts->IsSelected(esdTrack2)) continue;
      fCFManager->GetParticleContainer()->Fill(containerInput, 4);

      // step 5-6-7-8: PID containers
      // these 4 containers are parallel, instead of being mutually exclusive:
      // 5 --> cut BB 10%
      // 6 --> cut BB 20%
      // 7 --> realistic PID full
      // 8 --> realistic PID TPC
      Info("UserExec", "Cut 0.1 rel diffs = %f, %f", cutBB10.RelDiff(&rsnDaughter1), cutBB10.RelDiff(&rsnDaughter2));
      if (cutBB10.IsSelected(AliRsnCut::kParticle, &rsnDaughter1) && cutBB10.IsSelected(AliRsnCut::kParticle, &rsnDaughter2)) {
        Info("UserExec", "Cut Bethe-Bloch 0.1 passed");
        fCFManager->GetParticleContainer()->Fill(containerInput, 5);
      }
      else
      {
        Info("UserExec", "Cut Bethe-Bloch 0.1 **NOT** passed");
      }

      Info("UserExec", "Cut 0.2 rel diffs = %f, %f", cutBB20.RelDiff(&rsnDaughter1), cutBB20.RelDiff(&rsnDaughter2));
      if (cutBB20.IsSelected(AliRsnCut::kParticle, &rsnDaughter1) && cutBB20.IsSelected(AliRsnCut::kParticle, &rsnDaughter2)) {
        Info("UserExec", "Cut Bethe-Bloch 0.2 passed");
        fCFManager->GetParticleContainer()->Fill(containerInput, 6);
      }
      else
      {
        Info("UserExec", "Cut Bethe-Bloch 0.2 **NOT** passed");
      }

      // PID ESD
      rsnDaughter1.CombineWithPriors(priors, &pidDefESD);
      rsnDaughter2.CombineWithPriors(priors, &pidDefESD);
      if (rsnDaughter1.RealisticPID() == AliPID::kKaon && rsnDaughter2.RealisticPID() == AliPID::kKaon) {
        Info("UserExec", "Cut ESD realistic PID passed");
        fCFManager->GetParticleContainer()->Fill(containerInput, 7);
      }
      else
      {
        Info("UserExec", "PID of daughter #1 = %s", AliPID::ParticleName(rsnDaughter1.RealisticPID()));
        Info("UserExec", "PID of daughter #2 = %s", AliPID::ParticleName(rsnDaughter2.RealisticPID()));
      }

      // PID TPC
      rsnDaughter1.CombineWithPriors(priors, &pidDefTPC);
      rsnDaughter2.CombineWithPriors(priors, &pidDefTPC);
      if (rsnDaughter1.RealisticPID() == AliPID::kKaon && rsnDaughter2.RealisticPID() == AliPID::kKaon) {
        Info("UserExec", "Cut TPC realistic PID passed");
        fCFManager->GetParticleContainer()->Fill(containerInput, 8);
      }
      {
        Info("UserExec", "PID of daughter #1 = %s", AliPID::ParticleName(rsnDaughter1.RealisticPID()));
        Info("UserExec", "PID of daughter #2 = %s", AliPID::ParticleName(rsnDaughter2.RealisticPID()));
      }
    }
  }


  fHistEventsProcessed->Fill(0);
  /* PostData(0) is taken care of by AliAnalysisTaskSE */
  PostData(1, fHistEventsProcessed) ;
  PostData(2, fCFManager->GetEventContainer());
  PostData(3, fCFManager->GetParticleContainer());
  PostData(4, fHistoIM) ;
}


//___________________________________________________________________________
void AliCFRsnTask::Terminate(Option_t*)
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

  Info("Terminate","");
  AliAnalysisTaskSE::Terminate();
}

//___________________________________________________________________________
void AliCFRsnTask::UserCreateOutputObjects()
{
  //HERE ONE CAN CREATE OUTPUT OBJECTS, IN PARTICULAR IF THE OBJECT PARAMETERS DON'T NEED
  //TO BE SET BEFORE THE EXECUTION OF THE TASK
  //
  Info("UserCreateOutputObjects","CreateOutputObjects of task %s\n", GetName());

  //slot #1
  OpenFile(1);
  fHistEventsProcessed = new TH1I("fHistEventsProcessed","",1,0,1) ;
  fHistoIM = new TH1D("IM","Inv Mass", 2500, 0.0, 2.5);
}

//___________________________________________________________________________
Double_t AliCFRsnTask::GetRapidity(Double_t energy, Double_t pz) {
  if (energy == pz || energy == -pz) {
    printf("GetRapidity : ERROR : rapidity for 4-vector with E = Pz -- infinite result");
    return 999;
  }
  if (energy < pz) {
    printf("GetRapidity : ERROR : rapidity for 4-vector with E = Pz -- infinite result");
    return 999;
  }
  Double_t y = 0.5 * log((energy + pz) / (energy - pz));
  return y;
}
