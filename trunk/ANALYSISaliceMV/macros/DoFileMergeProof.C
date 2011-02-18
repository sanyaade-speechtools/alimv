#ifndef __CINT__
#include <Rtypes.h>
#include <TString.h>
#include <TChain.h>
#include <TProof.h>
#include <TSelector.h>
#include "TFileMergeSelector.h"
#include <TSystem.h>
#include <TFileCollection.h>
#include <TTimeStamp.h>
#endif

void DoFileMergeProof(TString alienFindCmd = "-l 100 /alice/cern.ch/user/m/mvala/work/rsn/2011-02-16/03_step_1/output/ root_archive.zip",
                      TString filesInZip = "AnalysisResults.root",
                      TString proof = "skaf.saske.sk",
                      TString proofOpt = "", TString outputSE = "prf000-iep-grid.saske.sk:11094")
{

   TProof *p = TProof::Open(proof.Data(), proofOpt.Data());
   if (!p) {
      Printf("Error opending %s !!!", proof.Data());
      return;
   }

   TString usernameLetter = p->GetUser();
   usernameLetter.Remove(1, usernameLetter.Length());

   TString xrdDir = Form("/tmp/alice/cern.ch/user/%s/%s/%lld-%d", usernameLetter.Data(), p->GetUser(), (Long_t)TTimeStamp().GetSec(), gSystem->GetPid());
   TString collNameOut = Form("root://%s/%s/my_collection.root", outputSE.Data(), xrdDir.Data());

   gSystem->Exec(Form("xrd %s mkdir %s", outputSE.Data(), xrdDir.Data()));
   Printf("Directory root://%s/%s created ...", outputSE.Data(), xrdDir.Data());


   CreateFileMergeCollection(alienFindCmd, collNameOut);

   TChain c("filesTree");
   c.AddFile(collNameOut.Data());
   Long64_t numEntries = c.GetEntries();
	Printf("Num entries : %lld", numEntries);
	if (!numEntries) return;
	
	TFileCollection mergeColl;
	Printf("adding file %s in to collection ",collNameOut.Data());
	gSystem->Exec(Form("echo %s > /tmp/tmpmergefile.txt",collNameOut.Data()));
	mergeColl.AddFromFile("/tmp/tmpmergefile.txt");
	p->RegisterDataSet("merge_coll",&mergeColl,"OV");


   // creating alien on workers
   Printf("Creating AliEn connection on workers (it may take while)");

   Bool_t useAliRoot = kTRUE;

   if (useAliRoot) AddAlirootSupport(p);
   else p->Exec("TGrid::Connect(\"alien:\/\/\");");


   p->AddInput(new TNamed("PROOF_OUTPUTFILE", Form("root://%s/%s", outputSE.Data(), xrdDir.Data())));

   if (!filesInZip.IsNull()) p->AddInput(new TNamed("PROOF_USE_ARCHIVE", filesInZip.Data()));

   // tmp TODO not tested if filesInZip.IsNull()
   if (filesInZip.IsNull()) return;

   if (proof.IsNull()) p->AddInput(new TNamed("PROOF_IS_LITE", "YES"));

   // use TFileCp instead of TProofOutputFile
   p->AddInput(new TNamed("PROOF_USE_TFILECP", "YES"));

   p->AddInput(new TNamed("PROOF_MERGE_NUM", "10"));

   p->SetParameter("PROOF_PacketizerStrategy", (Int_t)0);
   p->SetParameter("PROOF_PacketAsAFraction", numEntries / p->GetParallel());

	p->ShowDataSets();
	p->Process("merge_coll","TFileMergeSelector.cxx++g");
	
	// remove tm dataset
	p->RemoveDataSet("merge_coll");

}

void AddAlirootSupport(TProof *p, TString alirootVer = "VO_ALICE@AliRoot::v4-21-14-AN")
{

   TList *list = new TList();
   list->Add(new TNamed("ALIROOT_EXTRA_LIBS", "EventMixing"));
   list->Add(new TNamed("ALIROOT_ENABLE_ALIEN", "1"));
   p->EnablePackage(alirootVer.Data(), list);

}


void CreateFileMergeCollection(TString alienFindCmd,
                               TString collectionFileOut,
                               TString outFile = "/tmp/tmpInFiles.root",
                               TString myTxt = "/tmp/tmpListFiles.txt")
{
   // create list of files from alien catalogue query
   CreateTxtFileFromAliEn(myTxt, alienFindCmd);

   CreateFileMergeCollectionFromTxt(myTxt, outFile, collectionFileOut);
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
   f.Write();

   Printf("File %s with %lld entries was created.", outFile.Data(), t.GetEntries());

   if (!outFileToAliEn.IsNull()) {
      Printf("Copying %s to %s ...", outFile.Data(), outFileToAliEn.Data());
      TFile::Cp(outFile.Data(), outFileToAliEn.Data());
   }
   fileIn.close();

}

void CreateTxtFileFromAliEn(TString outTxtFile, TString alienFindCmd)
{
   ofstream outFile(outTxtFile.Data());
   if (!outFile) {
      Printf(Form("Error: Cannot create file %s", outTxtFile.Data()));
      return;
   }

   TGrid::Connect("alien://");

   TString command;
   command = "find ";
   command += alienFindCmd;

   printf("AliEn find command: %s\n", command.Data());
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

