// The class definition in TFileMergeSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("TFileMergeSelector.C")
// Root > T->Process("TFileMergeSelector.C","some options")
// Root > T->Process("TFileMergeSelector.C+")
//

#include "TFileMergeSelector.h"
#include <TH2.h>
#include <TStyle.h>
#include <TFileMerger.h>
#include <TSystem.h>
#include <TProofOutputFile.h>
#include <TAlien.h>

ClassImp(TFileMergeSelector)


TFileMergeSelector::TFileMergeSelector(TTree*): TSelector(),
   fDoMerge(kTRUE),
   fUseArchive(kFALSE),
   fListFilesInArchive(),
   fFileMergers(),
   fNMaxFilesMerged(10),
   fCounter(0),
   fOutputDir(""),
   fFilePrefix("")
{

}

TFileMergeSelector::~TFileMergeSelector()
{
   fFileMergers.Delete();
}

void TFileMergeSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_files_fUniqueID);
   fChain->SetBranchAddress("fBits", &fBits, &b_files_fBits);
   fChain->SetBranchAddress("fString", &fString, &b_files_fString);

}

Bool_t TFileMergeSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TFileMergeSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void TFileMergeSelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   // sets TObjArray to be owner
   fFileMergers.SetOwner(kTRUE);

   TNamed *out = (TNamed *) fInput->FindObject("PROOF_USE_ALIEN");
   if (out) {
      TGrid::Connect("alien://");
      fFilePrefix = "alien://";
   }
   out = (TNamed *) fInput->FindObject("PROOF_USE_ARCHIVE");
   if (out) {
      fUseArchive = kTRUE;
      fListFilesInArchive = out->GetTitle();
   } else {
      fUseArchive = kFALSE;
   }

   if (fListFilesInArchive.IsNull()) fListFilesInArchive = "dummy.root";

}

Bool_t TFileMergeSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either TFileMergeSelector::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fChain->GetEntry(entry);
   Printf("File is %s", fString.Data());

   TObjArray* array = fListFilesInArchive.Tokenize(",");
   TObjString *str;
   TString strr, origFileName;
   TFileMerger *merger;
   for (Int_t i = 0; i < array->GetEntriesFast(); i++) {
      str = (TObjString *) array->At(i);
      strr = str->GetString();
      origFileName = strr;
      merger = (TFileMerger*)fFileMergers.At(i);


      if (!merger) {
         merger = new TFileMerger(kFALSE);
         merger->SetFastMethod(kTRUE);
         strr.ReplaceAll(".root", "_all.root");
         merger->OutputFile(strr.Data());
         fFileMergers.Add(merger);
         Printf("Created new merger with output %s", merger->GetOutputFileName());
         strr = origFileName;
         strr.ReplaceAll(".root", "_previous.root");

         if (!gSystem->AccessPathName(strr.Data())) {
            Printf("Adding previous fime  %s", strr.Data());
            merger->AddFile(strr.Data());
         }
      } else {
         Printf("Already have merger with output file %s", merger->GetOutputFileName());
      }

      if (fDoMerge) {
         if (fUseArchive) {
            Printf("merger->AddFile %s%s#%s", fFilePrefix.Data(), fString.Data(), origFileName.Data());
            merger->AddFile(Form("%s%s#%s", fFilePrefix.Data(), fString.Data(), origFileName.Data()));
         } else {
            Printf("merger->AddFile %s%s", fFilePrefix.Data(), fString.Data());
            merger->AddFile(Form("%s%s", fFilePrefix.Data(), fString.Data()));
         }
      }

      if (fCounter % fNMaxFilesMerged == 0) {
         if (fCounter) DoMergeAll();

      }
   }

   fCounter++;

   return kTRUE;
}

void TFileMergeSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

   DoMergeAll();

   TObjArray* array = fListFilesInArchive.Tokenize(",");
   TObjString *str;
   TString strr;
//     TFileMerger *merger;
   TString origFileName;
   for (Int_t i = 0; i < array->GetEntriesFast(); i++) {
      str = (TObjString *) array->At(i);
      strr = str->GetString();
      origFileName = strr;
      strr.ReplaceAll(".root", "_previous.root");

      if (!gSystem->AccessPathName(strr.Data())) {
         // add proof output file
         TProofOutputFile *fProofFile = new TProofOutputFile(strr.Data(), "M");
         TNamed *out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE");
         if (out) fProofFile->SetOutputFileName(Form("%s/%s", out->GetTitle(), origFileName.Data()));
         // add proof file to fOutputs
         fProofFile->Print();
         fOutput->Add(fProofFile);

      }

   }



   gSystem->Exec("ls -alh");

}

void TFileMergeSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}

void TFileMergeSelector::DoMergeAll()
{
   TObjArrayIter next(&fFileMergers);
   TFileMerger *m;
   while ((m = (TFileMerger*) next())) {
      Printf("Merging in to %s ...", m->GetOutputFileName());
      if (fDoMerge) {
         m->Merge();
         TString tmpOutputName = m->GetOutputFileName();
         tmpOutputName.ReplaceAll("_all", "_previous");

         Printf("mv %s %s", m->GetOutputFileName(), tmpOutputName.Data());
         gSystem->Exec(Form("mv %s %s", m->GetOutputFileName(), tmpOutputName.Data()));
      }
   }

   Printf("Deleting all mergers");
   fFileMergers.RemoveAll();
}

