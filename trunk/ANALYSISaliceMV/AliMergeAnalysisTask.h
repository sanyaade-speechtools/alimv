#ifndef ALIMERGEANALYSISTASK_H
#define ALIMERGEANALYSISTASK_H

#include <AliAnalysisTask.h>

#include <TString.h>
#include <TProofOutputFile.h>

#include "AliFileInfo.h"
#include <Rtypes.h>
// class AliFileInfo;

class AliMergeAnalysisTask : public AliAnalysisTask {

public:
   AliMergeAnalysisTask(const char* name = "mergeTask", const char* title = "Merge Task");
   virtual void CreateOutputObjects();
   virtual void Terminate(Option_t* option = "");
   virtual void Exec(Option_t* option);

   virtual void FinishTaskOutput();

   void SetFilesToBeMerged(TString listOfFiles) { fListOfFiles = listOfFiles; }
   void SetMergeNum(Long64_t mergeNum) { fMergeNum = mergeNum; }

   void DoMerge(Bool_t doMerge = kTRUE) { fDoMerge = doMerge; }
   void SetLite(Bool_t isLite = kTRUE) { fIsLite = isLite; }
   void SetOutputDir(TString outputDir) { fOutputDir = outputDir;}

protected:
   virtual void ConnectInputData(Option_t* option = "");

private:

   AliFileInfo *fFileInfo;     //! File Info
   TString     fListOfFiles;   // List of files to be merged (comma-separated)
   TObjArray   fFileMergers;   //!
   Long64_t    fMergeNum;      //! merge num
   Long64_t    fNumMerged;     //! num merged
   Long64_t    fNumChunk;      //! num chunk
   TString     fLastChunkFN;   //! last chunk filename

   TString     fOutputDir;     // output dir

   Bool_t      fDoMerge;       // flag to do merge
   Bool_t      fIsLite;       // flag is Lite

   TProofOutputFile *fProofFile;

   void        DoMergeAll();


   ClassDef(AliMergeAnalysisTask, 1); //Merge task
};

#endif // ALIMERGEANALYSISTASK_H
