void RsnCut2010(AliRsnCutManager *cutManager)
{

   if (!cutManager) return;

   const char *options = "data";
   const char *config = "pid";
   const char *path = ".";

   // load useful macros
   gROOT->LoadMacro(Form("%s/QualityCutsITS.C", path));
   gROOT->LoadMacro(Form("%s/QualityCutsTPC.C", path));
   // interpret the useful information from second argument
   TString opt(options);
   Bool_t isSim  = opt.Contains("sim");
   Bool_t isData = opt.Contains("data");
   if (!isSim && !isData) {
      Error("RsnConfig", "Required to know if working on data or MC");
      return kFALSE;
   }

   // interpret the specific info from third argument
   // which should be fixed in the various calls to this function
   TString conf(config);
   Bool_t addPID    = conf.Contains("pid");
   Bool_t addITSSA  = conf.Contains("its");
   Bool_t addDipCut = conf.Contains("dip");

   // generate a common suffix depending on chosen options
   TString suffix;
   if (addPID)    suffix += "_pid";
   if (addITSSA)  suffix += "_its";
   if (addDipCut) suffix += "_dip";
   Info("RsnConfig", "=== Specific configuration: %s ====================================================", config);
   Info("RsnConfig", "=== suffix used           : %s ====================================================", suffix.Data());

// track cut -----------------------


   AliRsnCutESD2010 *cuts2010 = new AliRsnCutESD2010(Form("cuts2010%s", suffix.Data()), isSim);
   // --> set the reference particle for PID
   cuts2010->SetPID(AliPID::kKaon);
   // --> include or not the ITS standalone (TPC is always in)
   cuts2010->SetUseITSTPC(kTRUE);
   cuts2010->SetUseITSSA(addITSSA);
   // --> set the quality cuts using the general macro and using the 'Copy()' method in AliESDtrackCuts
   cuts2010->CopyCutsTPC(QualityCutsTPC());
   cuts2010->CopyCutsITS(QualityCutsITS());
   // --> set values for PID flags, depending on the choice expressed in the options
   cuts2010->SetCheckITS(addPID);
   cuts2010->SetCheckTPC(addPID);
   cuts2010->SetCheckTOF(addPID);
   // --> set the ITS PID-related variables
   cuts2010->SetITSband(3.0);
   // --> set the TPC PID-related variables
   Double_t bbPar[5];
   if (isSim) {
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
   cuts2010->SetTPCrange(3.0, 5.0);
   cuts2010->SetTPCpLimit(0.35);
   cuts2010->GetESDpid()->GetTPCResponse().SetBetheBlochParameters(bbPar[0], bbPar[1], bbPar[2], bbPar[3], bbPar[4]);
   // --> set the TOF PID-related variables
   cuts2010->SetTOFrange(-3.0, 3.0);

   cutManager->GetCommonDaughterCuts()->AddCut(cuts2010);
   cutManager->GetCommonDaughterCuts()->SetCutScheme(cuts2010->GetName());

}