void RsnCutPIDOnly(AliRsnCutManager *cutManager)
{

   if (!cutManager) return;

   // -- tracks --> PID
   AliRsnCutPID *cutPID = new AliRsnCutPID("cutPID", AliPID::kKaon, 0.0, kFALSE);

   cutManager->GetCommonDaughterCuts()->AddCut(cutPID);
   cutManager->GetCommonDaughterCuts()->SetCutScheme(cutPID->GetName());

}