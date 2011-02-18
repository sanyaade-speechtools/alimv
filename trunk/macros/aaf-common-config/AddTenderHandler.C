#include <Rtypes.h>
void AddTenderHandler(AliMultiInputEventHandler *multiInputHandler)
{

   if (multiInputHandler) {
      AliESDInputHandler*esdIH = dynamic_cast<AliESDInputHandler*>(multiInputHandler->GetFirstInputEventHandler());
      if (esdIH) {
         Bool_t useV0 = kFALSE;
         //get the current analysis manager
         Bool_t checkEvtSelection = useV0;

         AliTenderInputEventHandler *tenderIH = new AliTenderInputEventHandler();
         AliTender *tender = tenderIH->GetTender();
         tender->SetCheckEventSelection(checkEvtSelection);
         tender->SetDefaultCDBStorage("raw://");

         //========= Attach VZERO supply ======
         if (useV0) {
            AliVZEROTenderSupply *vzeroSupply = new AliVZEROTenderSupply("VZEROtender");
            vzeroSupply->SetDebug(kFALSE);
            tender->AddSupply(vzeroSupply);
         }
//          //========= Attach TPC supply ======
//          AliTPCTenderSupply *tpcSupply = new AliTPCTenderSupply("TPCtender");
//          tpcSupply->SetDebugLevel(2);
//          //tpcSupply->SetMip(50.);
//          tender->AddSupply(tpcSupply);
//
// //          ========= Attach TOF supply ======
//          AliTOFTenderSupply *tofTender = new AliTOFTenderSupply("TOFtender");
//          tofTender->SetTimeZeroType(AliESDpid::kTOF_T0);
//          tender->AddSupply(tofTender);


         //
         // === Attach TOF supply ==============================================
         //
         AliTPCTenderSupply *TPCtender = new AliTPCTenderSupply("TPCtender");
         tender->AddSupply(TPCtender);

         //
         // === Attach TOF supply ==============================================
         //
         Float_t tofres       = 80;
         Bool_t  corrExpTimes = kFALSE;
         Bool_t  applyT0      = kFALSE;
         tofres = 100;
         corrExpTimes = kTRUE;
         applyT0 = kTRUE;

         AliTOFTenderSupply *TOFtender = new AliTOFTenderSupply("TOFtender");
         TOFtender->SetTOFres(tofres);
         TOFtender->SetApplyT0(applyT0);
         TOFtender->SetCorrectExpTimes(corrExpTimes);
         tender->AddSupply(TOFtender);

         //========= Attach TRD supply ======
         AliTRDTenderSupply *trdSupply = new AliTRDTenderSupply("TRDtender");
         tender->AddSupply(trdSupply);

         //========= Attach PID supply ======
         tender->AddSupply(new AliPIDTenderSupply("PIDtender"));

         //========= Attach Primary Vertex supply ======
         tender->AddSupply(new AliVtxTenderSupply("PriVtxtender"));

         multiInputHandler->AddInputEventHandler(tenderIH);
      }
   }

}
