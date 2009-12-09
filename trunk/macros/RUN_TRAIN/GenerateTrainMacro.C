#ifndef __CINT__
#include <TString.h>
#include <TSystem.h>
#endif
Bool_t GenerateTrainMacro(TString outputMacroName = "myTrain.C")
{

  gSystem->Exec(Form("rm -f %s", outputMacroName.Data()));
  ofstream outFile(outputMacroName.Data());

  if (!outFile) {
    PrintErrorString(Form("Cannot open file %s", outputMacroName.Data()));
    return kFALSE;
  }

  outputMacroName.ReplaceAll(".C", "");

  TString tmp;
  tmp = Form("Bool_t %s() {", outputMacroName.Data());
  outFile << tmp.Data() << endl;

  tmp = "\tTStopwatch timer;";
  outFile << tmp.Data() << endl;

  tmp = "\ttimer.Start();\n";
  outFile << tmp.Data() << endl;

  tmp = "\tBool_t returnValue = kTRUE;\n";
  outFile << tmp.Data() << endl;

  if (!ImportConfigFile("myConfig.txt", outFile)) {
    outFile.close();
    gSystem->Exec(Form("rm -f %s.C", outputMacroName.Data()));
    return kFALSE;
  }

  tmp = "\ttimer.Stop();";

  outFile << tmp.Data() << endl;

  tmp = "\ttimer.Print();\n";
  outFile << tmp.Data() << endl;

  tmp = Form("\treturn returnValue;");
  outFile << tmp.Data() << endl;

  tmp = Form("}");
  outFile << tmp.Data() << endl;

  outFile.close();

  gSystem->Exec(Form("cat %s.C", outputMacroName.Data()));

  return kTRUE;
}

Bool_t ImportConfigFile(TString config, ofstream & outFile)
{

  outFile << "\t// Import from " << config.Data() << endl;

  ifstream fileIn;

  fileIn.open(config.Data());

  const Int_t numStrings =  10;
  TObjArray* array,*arrayDep;
  TObjString *str, *strDep;
  TString strr[numStrings], strrDep;
  TString deps;

  TString fileline;
  Char_t fileItem[256];
  Int_t i;

  while (fileIn.good()) {
    fileIn.getline(fileItem, 256);
    fileline = fileItem;

    // reset previous configuration
    for (i = 0;i < numStrings;i++) {
      strr[i] = "";
    }

    if (fileline.IsNull()) continue;

    if (fileline.BeginsWith("#")) continue;

    if (fileline.BeginsWith("0")) continue;

    cout << fileline.Data() << endl;

    array = fileline.Tokenize("|");

    for (i = 0;i < array->GetEntriesFast();i++) {
      str = (TObjString *) array->At(i);
      strr[i] = str->GetString();

    }

    if (strr[0].Atoi() == 1) {
      outFile << "\n\t// Dep: " << strr[2].Data() << endl;


      arrayDep = strr[2].Tokenize(";");

      for (i = 0;i < arrayDep->GetEntriesFast();i++) {
        strDep = (TObjString *) arrayDep->At(i);
        strrDep = strDep->GetString();

        if (!deps.Contains(strrDep.Data()))
          deps += Form("%s:", strrDep.Data());
      }

      if (!strr[3].IsNull()) {
        TString tmp = strr[3].Data();

        if (tmp.Contains('('))
          tmp.Resize(tmp.Index("("));

        outFile << Form("\tgROOT->LoadMacro(gSystem->ExpandPathName(\"%s\"));", tmp.Data()) << endl;

        tmp = gSystem->BaseName(strr[3].Data());

        tmp.ReplaceAll(".C", "");

//         if (!tmp.Contains('('))
//           tmp += "()";

//         TString tmp2 = strr[3].Data();
        TString tmp2 = strr[4].Data();
//       arrayDep = strr[2].Tokenize(";");
// 
//       for (i = 0;i < arrayDep->GetEntriesFast();i++) {
//         strDep = (TObjString *) arrayDep->At(i);
//         strrDep = strDep->GetString();
// 
//         if (!deps.Contains(strrDep.Data()))
//           deps += Form("%s:", strrDep.Data());
//       }
        outFile << Form("\tif (!%s(%s)) { Error(\"\",\"Error in task %s\"); return kFALSE; }", tmp.Data(),tmp2.Data(), strr[1].Data()) << endl;
      }
    }

  }

  deps.Remove(deps.Last(':'));

  outFile << "\n\t// All Train Dep: " << deps.Data() << endl;
  fileIn.close();

  outFile << "\n\t// End import from " << config.Data() << "\n" << endl;
  return kTRUE;
}
