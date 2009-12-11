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

#ifndef ALICFRSNTASK_H
#define ALICFRSNTASK_H

#include "AliRsnEvent.h"
#include "AliAnalysisTaskSE.h"

class TH1I;
class TH1D;
class TParticle ;
class AliCFManager;
class AliRsnPIDDefESD;

class AliCFRsnTask : public AliAnalysisTaskSE {
  public:

  AliCFRsnTask();
  AliCFRsnTask(const Char_t* name);
  AliCFRsnTask& operator= (const AliCFRsnTask& c);
  AliCFRsnTask(const AliCFRsnTask& c);
  virtual ~AliCFRsnTask();

  // ANALYSIS FRAMEWORK STUFF to loop on data and fill output objects
  void     UserCreateOutputObjects();
  void     UserExec(Option_t *option);
  void     Terminate(Option_t *);

  // CORRECTION FRAMEWORK RELATED FUNCTIONS
  void           SetCFManager(AliCFManager* io) {fCFManager = io;}   // global correction manager
  AliCFManager * GetCFManager()                 {return fCFManager;} // get corr manager

  void     SetInvMassRange(Double_t min, Double_t max) {fRsnMassMin = min; fRsnMassMax = max;}
  void     SetRsnPDG(Int_t code) {fRsnPDG = code;}
  static   Double_t GetRapidity(Double_t, Double_t) ;  // returns the rapidity of the Resonance (assuming PDG code)

  void     SetReadTPCTracks (Bool_t flag=kTRUE) {fReadTPCTracks=flag;}

 protected:

  Bool_t           fReadTPCTracks; // flag to loop on TPC tracks only (ESD mode only)

  Int_t            fRsnPDG;       //  PDG code of searched resonance
  Double_t         fRsnMassMin;
  Double_t         fRsnMassMax;

  AliRsnEvent      fRsnEvent;
  AliCFManager    *fCFManager  ;  // pointer to the CF manager

  // Histograms
  //Number of events
  TH1I *fHistEventsProcessed; //! simple histo for monitoring the number of events processed
  TH1D *fHistoIM; //! simple histo for monitoring the number of events processed

  ClassDef(AliCFRsnTask,1);
};

#endif
