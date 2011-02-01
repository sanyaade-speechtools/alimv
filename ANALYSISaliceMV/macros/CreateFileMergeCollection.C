#ifndef __CINT__
#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TObjString.h>
#endif

void CreateFileMergeCollection(TString outFile = "/tmp/inFiles.root")
{
//   outFile = "alien:///alice/cern.ch/user/m/mvala/tmp/file_collection.root";

   TString myTxt = "test.txt";

   // create list of files from alien catalogue query
   myTxt = "/tmp/my_list_files.txt";
   CreateTxtFileFromAliEn(myTxt, "/alice/cern.ch/user/m/mvala/work/rsn/100/output/000/", "root_archive.zip", "-l 10");
//    CreateTxtFileFromAliEn(myTxt,"/alice/cern.ch/user/m/mvala/work/rsn/100/output/000/","AnalysisResults.root","-l 100");


   CreateFileMergeCollectionFromTxt(myTxt, outFile);
}

void CreateFileMergeCollectionFromTxt(TString fileInName, TString outFile)
{

   TFile f(outFile.Data(), "RECREATE");
   TTree t("filesTree", "File Info");
   TObjString *fileStr = new TObjString();
   t.Branch("files", "TObjString", &fileStr);
   // Open the input stream
   ifstream fileIn;
   fileIn.open(fileInName.Data());

   Int_t count = 0;

   // Read the input list of files and add them to the chain
   TString fileLine;
   while (fileIn.good()) {
      fileIn >> fileLine;
      if (fileLine.IsNull()) continue;

      fileStr->SetString(fileLine);
      t.Fill();
   }
   fileIn.close();
   t.Write();
   Printf("File %s was created.", outFile.Data());
}

void CreateTxtFileFromAliEn(TString outTxtFile, TString path, TString pattern, TString options = "")
{
   // FIXME more elegant from alien classes

   TGrid::Connect("alien://");

   TString command;
   command = "find ";
   command += options;
   if (!options.IsNull()) command += " ";
   command += path;
   command += " ";
   command += pattern;
//     command += conditions;

   printf("command: %s\n", command.Data());
   TGridResult *res = gGrid->Command(command);
   if (res) delete res;
   // Write standard output to file
   gROOT->ProcessLine(Form("gGrid->Stdout(); > %s", Form("%s_tmp", outTxtFile.Data())));

   gSystem->Exec(Form("cat %s | grep -v found > %s", Form("%s_tmp", outTxtFile.Data()), outTxtFile.Data()));

}
