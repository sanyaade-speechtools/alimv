//
// This macro adds the specific RSN input handler, with all single particle cuts
//
void AddRsnInputHandler(Bool_t isMC, AliMultiInputEventHandler *multi)
{
   void myError  (const char *msg) {::Error  ("AddRsnInputHandler", msg);}
   void myWarning(const char *msg) {::Warning("AddRsnInputHandler", msg);}
   void myInfo   (const char *msg) {::Info   ("AddRsnInputHandler", msg);}
   
   if (!multi) {
      ::Error("AddRsnInputHandler", "Required a WELL INITIALIZED AliMultiInputEventHandler object");
      return;
   }
   
   //---------------------------------------------
   //  Define single cuts
   //---------------------------------------------
   
   AliRsnCutPhi2010 *cut = new AliRsnCutPhi2010("cut");
      
   //---------------------------------------------
   //  Combine cuts
   //---------------------------------------------
   
   AliRsnCutSet *cuts = new AliRsnCutSet("kaonTPC", AliRsnTarget::kDaughter);
   cuts->AddCut(cut);
   cuts->SetCutScheme(cut->GetName());
   
   // setup selector in the handler and add RSN input handler
   AliRsnInputHandler *rsnIH = new AliRsnInputHandler();
   AliRsnDaughterSelector *sel = rsnIH->GetSelector();
   sel->Add(cuts, kTRUE);
   sel->Init();
   multi->AddInputEventHandler(rsnIH);
}
