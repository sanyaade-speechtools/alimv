#include <PWG2resonances/RESONANCES/AliRsnLoopPair.h>
void RsnConfigPhi(AliRsnAnalysisTask *task)
{

   Int_t iStart = 0;
   iStart = 1;

   TString opt;

   AliRsnDaughterDef *defKaonP = new AliRsnDaughterDef(AliRsnDaughter::kKaon, '+');
   AliRsnDaughterDef *defKaonM = new AliRsnDaughterDef(AliRsnDaughter::kKaon, '-');

   AliRsnPairDef *pairDef[3];
   pairDef[0] = new AliRsnPairDef(AliRsnDaughter::kKaon, '+', AliRsnDaughter::kKaon, '-', 333, 1.019455); // unlike
   pairDef[1] = new AliRsnPairDef(AliRsnDaughter::kKaon, '+', AliRsnDaughter::kKaon, '+', 333, 1.019455); // like ++
   pairDef[2] = new AliRsnPairDef(AliRsnDaughter::kKaon, '-', AliRsnDaughter::kKaon, '-', 333, 1.019455); // like --

   AliRsnLoopPair *pair[4];
   pair[0] = new AliRsnLoopPair(Form("%s_kaonP_kaonM_phi", opt.Data()), pairDef[0], kFALSE); // unlike - true
   pair[1] = new AliRsnLoopPair(Form("%s_kaonP_kaonM_all", opt.Data()), pairDef[0], kFALSE); // unlike - all
   pair[2] = new AliRsnLoopPair(Form("%s_kaonP_kaonP_all", opt.Data()), pairDef[1], kFALSE); // like ++
   pair[3] = new AliRsnLoopPair(Form("%s_kaonM_kaonM_all", opt.Data()), pairDef[2], kFALSE); // like --

   AliRsnLoopEvent *event = new AliRsnLoopEvent(Form("%s_evt", opt.Data()));

   // set additional option for true pairs (slot [0])
   pair[0]->SetOnlyTrue(kTRUE);
   pair[0]->SetCheckDecay(kTRUE);


   for (Int_t i = iStart; i < 4; i++) pair[i]->SetListID(0, 0);

   AliRsnValue *axisIM      = new AliRsnValue("IM"  , AliRsnValue::kPairInvMass   ,  0.9, 1.4, 0.001);
   AliRsnValue *axisPt      = new AliRsnValue("PT"  , AliRsnValue::kPairPt        ,  0.0, 5.0, 0.1);

   for (Int_t i = iStart; i < 4; i++) {
//       if (i == 0) pair[i]->AddValue(axisRes);
      pair[i]->AddValue(axisIM);
      pair[i]->AddValue(axisPt);
   }

   AliRsnOutput *fcnImPt  = new AliRsnOutput("out4", "IM:PT", AliRsnOutput::kHistoSparse);
   for (Int_t ipair = 0; ipair < 4; ipair++) {
      if (!ipair) pair[ipair]->AddOutput(fcnImPt);
   }

   Bool_t isMix = kTRUE;
   isMix = kFALSE;
   // add all created objects to the task
   for (Int_t i = iStart; i < 4; i++) {
      pair[i]->SetMixed(isMix);
      task->Add(pair[i]);
   }
}
