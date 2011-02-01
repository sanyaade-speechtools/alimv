#include <TTree.h>
#include <TROOT.h>
#include <TFile.h>
#include <TFileMerger.h>
#include <Rtypes.h>
#include <TProof.h>
#include "AliLog.h"
#include "AliMergeAnalysisTask.h"
#include <TSystem.h>


ClassImp(AliMergeAnalysisTask)

AliMergeAnalysisTask::AliMergeAnalysisTask(const char* name, const char* title): AliAnalysisTask(name, title),
   fFileInfo(0),
   fListOfFiles("MixInfo.root,AnalysisResults.root"),
   fFileMergers(2),
   fMergeNum(10),
   fNumMerged(0),
   fNumChunk(0),
   fLastChunkFN(""),
   fDoMerge(kFALSE),
   fIsLite(kFALSE),
   fProofFile(0)
{
   DefineInput(0, TTree::Class());
}

void AliMergeAnalysisTask::ConnectInputData(Option_t* /*option*/)
{
   // One should first check if the branch address was taken by some other task
   char ** address = (char **)GetBranchAddress(0, "fi");
   if (address) {
      fFileInfo = (AliFileInfo*)(*address);
   } else {
      fFileInfo = new AliFileInfo();
      SetBranchAddress(0, "fi", &fFileInfo);
   }
}

void AliMergeAnalysisTask::CreateOutputObjects()
{
   fFileMergers.SetOwner(kTRUE);
}

void AliMergeAnalysisTask::Exec(Option_t* /*option*/)
{
//     AliInfo(Form("FileName is %s",fFileInfo->FileName().Data()));
//       AliInfo(Form("PWD is %s",gSystem->pwd()));
//     gSystem->Sleep(10);

   TObjArray* array = fListOfFiles.Tokenize(",");
   TObjString *str;
   TString strr, origFileName;
   TFileMerger *merger;
   for (Int_t i = 0; i < array->GetEntriesFast(); i++) {
      str = (TObjString *) array->At(i);
      strr = str->GetString();
      origFileName = strr;
      merger = (TFileMerger*)fFileMergers.At(i);
      if (merger) AliInfo(Form("Already have merger with output file %s", merger->GetOutputFileName()));

      if (!merger) {

         merger = new TFileMerger(kFALSE);
         merger->SetFastMethod(kTRUE);
         strr.ReplaceAll(".root", "_all.root");
         merger->OutputFile(strr.Data());
         fFileMergers.Add(merger);
         AliInfo(Form("Created new merger with output %s", merger->GetOutputFileName()));
         strr = origFileName;
         strr.ReplaceAll(".root", "_previous.root");

         if (!gSystem->AccessPathName(strr.Data())) {
            AliInfo(Form("Adding previous fime  %s", strr.Data()));
            merger->AddFile(strr.Data());
         }
      }

      if (fDoMerge) {
         AliInfo(Form("merger->AddFile alien://%s#%s", fFileInfo->FileName().Data(), origFileName.Data()));
         merger->AddFile(Form("alien://%s#%s", fFileInfo->FileName().Data(), origFileName.Data()));
      }
   }

   if (fNumMerged % fMergeNum == 0) {
      if (fNumMerged) DoMergeAll();
   }
   fNumMerged++;
}

void AliMergeAnalysisTask::FinishTaskOutput()
{

   gSystem->Sleep(100);

   DoMergeAll();

   TObjArray* array = fListOfFiles.Tokenize(",");
   TObjString *str;
   TString strr;
   TFileMerger *merger;
   TString origFileName;
   for (Int_t i = 0; i < array->GetEntriesFast(); i++) {
      str = (TObjString *) array->At(i);
      strr = str->GetString();
      origFileName = strr;
      strr.ReplaceAll(".root", "_previous.root");

      if (fIsLite) {
         origFileName.ReplaceAll(".root", Form("_%d.root", gSystem->GetPid()));
      } else {
         Int_t gid = gROOT->ProcessLine("gProofServ->GetGroupId();");
         origFileName.ReplaceAll(".root", Form("_%d.root", gid));
      }
      AliInfo(Form("mv %s %s", strr.Data(), origFileName.Data()));
      gSystem->Exec(Form("mv %s %s", strr.Data(), origFileName.Data()));

      if (fOutputDir.IsNull()) {
         AliError("fOutputDir is empty!!!");
         return;
      }

      AliInfo(Form("TFile::Cp %s %s/%s", origFileName.Data(), fOutputDir.Data(), origFileName.Data()));
      TFile::Cp(origFileName.Data(), Form("%s/%s", fOutputDir.Data(), origFileName.Data()));
   }
}

void AliMergeAnalysisTask::DoMergeAll()
{
   TObjArrayIter next(&fFileMergers);
   TFileMerger *m;
   while ((m = (TFileMerger*) next())) {
      AliInfo(Form("Merging in to %s ...", m->GetOutputFileName()))
      if (fDoMerge) {
         m->Merge();
         TString tmpOutputName = m->GetOutputFileName();
         tmpOutputName.ReplaceAll("_all", "_previous");

         AliInfo(Form("mv %s %s", m->GetOutputFileName(), tmpOutputName.Data()));
         gSystem->Exec(Form("mv %s %s", m->GetOutputFileName(), tmpOutputName.Data()));
      }
//     if (m) delete m;
   }
   AliInfo("Deleting all mergers");
   fFileMergers.RemoveAll();
   fNumChunk++;
}


void AliMergeAnalysisTask::Terminate(Option_t* /*option*/)
{

}
