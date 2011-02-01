//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Jan 30 14:23:48 2011 by ROOT version 5.27/06b
// from TTree filesTree/File Info
// found on file: test.root
//////////////////////////////////////////////////////////

#ifndef TFileMergeSelector_h
#define TFileMergeSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TString.h>

class TFileMergeSelector : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
//TObjString      *files;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   TString         fString;

   // List of branches
   TBranch        *b_files_fUniqueID;   //!
   TBranch        *b_files_fBits;   //!
   TBranch        *b_files_fString;   //!

   TFileMergeSelector(TTree * /*tree*/ = 0);
   virtual ~TFileMergeSelector();
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   void DoMerge(Bool_t doMerge = kTRUE) {fDoMerge = doMerge;}

   void SetMaxFilesMerged(Int_t nMaxFilesMerged) { fNMaxFilesMerged = nMaxFilesMerged; }

   void SetFilesToMerge(TString files) { fListFilesInArchive = files;}

private:

   Bool_t      fDoMerge;                // flag to do merge
   Bool_t       fUseArchive;            // flag to merge files from archive
   TString     fListFilesInArchive;// List of files to be merged (comma-separated) in zip archive

   TObjArray   fFileMergers;            //! List of file mergers

   Int_t        fNMaxFilesMerged;    // Maximum number in TFile Merger
   Long64_t     fCounter;

   TString     fOutputDir;           // Output directory
   TString         fFilePrefix;


   void        DoMergeAll();

   ClassDef(TFileMergeSelector, 1);
};

#endif
