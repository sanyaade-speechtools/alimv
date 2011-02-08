#ifndef __CINT__
#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TObjString.h>
#include <TGridResult.h>

#endif

void CreateFileMergeCollection(TString outFile = "/tmp/inFiles.root", TString outFileToAliEn = "")
{

   outFileToAliEn = "alien:///alice/cern.ch/user/m/mvala/file_collection.root";
   TString myTxt = "test.txt";

   // create list of files from alien catalogue query
   CreateTxtFileFromAliEn(myTxt, "/alice/cern.ch/user/m/mvala/work/rsn/100/output/000/", "root_archive.zip", "-l 10");

   CreateFileMergeCollectionFromTxt(myTxt, outFile, outFileToAliEn);
}

void CreateFileMergeCollectionFromTxt(TString fileInName, TString outFile, TString outFileToAliEn = "")
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
   f->Write();

   Printf("File %s with %lld entries was created.", outFile.Data(), t.GetEntries());

   if (!outFileToAliEn.IsNull()) {
      Printf("Copying %s to %s ...", outFile.Data(), outFileToAliEn.Data());
      TFile::Cp(outFile.Data(), outFileToAliEn.Data());
   }
   fileIn.close();

}

void CreateTxtFileFromAliEn(TString outTxtFile, TString path, TString pattern, TString options = "")
{
   ofstream outFile(outTxtFile.Data());
   if (!outFile) {
      Printf(Form("Error: Cannot create file %s", outTxtFile.Data()));
      return;
   }

   TGrid::Connect("alien://");

   TString command;
   command = "find ";
   command += options;
   if (!options.IsNull()) command += " ";
   command += path;
   command += " ";
   command += pattern;

   printf("command: %s\n", command.Data());
   TGridResult *res = gGrid->Command(command);
   if (!res) {
      Printf(Form("Error: command %s could not be executed !!!", command.Data()));
      return;
   }

   res->Print();

   TString lfn;
   for (Int_t i = 0; i < res->GetEntries(); i++) {
      lfn = res->GetKey(i, "lfn");
      outFile << "alien://" << lfn.Data() << endl;
   }
   outFile.close();
   if (res) delete res;
}
