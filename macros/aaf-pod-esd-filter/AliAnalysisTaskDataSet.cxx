#include <TList.h>
#include <TROOT.h>
#include <TSystem.h>
#include <Riostream.h>
#include <TFileCollection.h>
#include <TProof.h>
#include "AliLog.h"
#include "AliAnalysisManager.h"

#include "AliAnalysisTaskDataSet.h"

ClassImp(AliAnalysisTaskDataSet)

//________________________________________________________________________
AliAnalysisTaskDataSet::AliAnalysisTaskDataSet(const char *name)
  : AliAnalysisTaskSE(name),
  fOutputList(0),
  fDataSetColl(0),
  fOutTree(0),
  fPath(0),
  fCounter(0)
{
  // Constructor

  DefineOutput(1, TList::Class());
}

//________________________________________________________________________
AliAnalysisTaskDataSet::~AliAnalysisTaskDataSet()
{
  // Destructor

  if (fOutputList && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) delete fOutputList;
}

//________________________________________________________________________
void AliAnalysisTaskDataSet::UserCreateOutputObjects()
{
  // Create histograms
  // Called once
	
// 	SetPostEventLoop(kTRUE);
  
	fOutputList = new TList();
//   fDataSetColl = new TFileCollection("myDataSet");
// 	fOutputList->Add(fDataSetColl);

  fOutTree = new TTree("pathTree","Output Paths");
  fPath = new TObjString;
  fOutTree->Branch("pathEntry", &fPath);
  fOutputList->Add(fOutTree);
//   fOutTree->SetBranchStyle();
  
  // Post output data.
  PostData(1, fOutputList);
}

//________________________________________________________________________
void AliAnalysisTaskDataSet::UserExec(Option_t *)
{

  
  if (!fCounter++) {
    Int_t gid = gROOT->ProcessLine("gProofServ->GetGroupId();");
    TString filename = Form("root://%s:2094//dev/shm/alice/xrd/data/%s_%d_AliAOD.root",gSystem->HostName(),gSystem->HostName(),gid);
//     fDataSetColl->Add(filename.Data());
    fPath->SetString(filename.Data());
    fOutTree->Fill();
    AliInfo(Form("Adding %s",filename.Data()));
  }
  
  // Post output data.
  PostData(1, fOutputList);

}      

//________________________________________________________________________
void AliAnalysisTaskDataSet::Terminate(Option_t*)
{
  // Draw result to the screen
  // Called once at the end of the query

  fOutputList = dynamic_cast<TList*> (GetOutputData(1));
  if (!fOutputList) {
    AliError("fOutputList not available");
    return;
  }

//   fDataSetColl = (TFileCollection*) fOutputList->FindObject("myDataSet");
//   if (!fDataSetColl) {
//     AliError("fDataSetColl not available");
//     return;
//   }
// 	fDataSetColl->Print();

  fOutTree = (TTree*) fOutputList->FindObject("pathTree");
  if (!fOutTree) {
    AliError("fOutTree not available");
    return;
  }
  fPath = new TObjString;
  TBranch *branch  = fOutTree->GetBranch("pathEntry");
  branch->SetAddress(&fPath);
  Int_t nevent = (Int_t)fOutTree->GetEntries();

  // create tmp file
  TString mytxtfile = "myds.txt";
  ofstream outFile ( mytxtfile.Data() );
  if ( !outFile ) {AliError( Form ( "Cannot open file %s", mytxtfile.Data() ) ); return;}
  
  for (Int_t i=0;i<nevent;i++) {
    fOutTree->GetEntry(i);
    AliInfo(Form("Path is %s",fPath->GetString().Data()));
    outFile << fPath->GetString().Data() << endl;
  }
  outFile.close();
  
  fDataSetColl = new TFileCollection;
  fDataSetColl->AddFromFile(mytxtfile);
  fDataSetColl->Print();
  gProof->RegisterDataSet("myds",fDataSetColl,"O");
  gProof->SetDataSetTreeName("myds","aodTree");
  gProof->VerifyDataSet("myds");
  gProof->ShowDataSets();

}
