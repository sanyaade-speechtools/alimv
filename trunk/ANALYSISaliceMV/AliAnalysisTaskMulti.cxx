
#include "AliAnalysisTaskMulti.h"
#include "AliAnalysisManager.h"
#include "AliMultiInputEventHandler.h"
#include "AliMCEventHandler.h"
#include "AliESDInputHandler.h"
#include "AliAODHandler.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisDataSlot.h"
#include "AliAODMCParticle.h"

ClassImp(AliAnalysisTaskMulti)

AliAnalysisTaskMulti::AliAnalysisTaskMulti(const char *name):
    AliAnalysisTaskSE(name),
    fCurrentEntry(0),
    fCurrentEntryMain(0),
    fCurrentEntryMix(0),
    fCurrentBinIndex(-1),
    fNumberMixed(0),
    fMultiInputHandler(0),
    fMCEventHandler(0)
{
    // Default constructor
}

AliAnalysisTaskMulti::AliAnalysisTaskMulti(const AliAnalysisTaskMulti &obj):
    AliAnalysisTaskSE(obj),
    fCurrentEntry(obj.fCurrentEntry),
    fCurrentEntryMain(obj.fCurrentEntryMain),
    fCurrentEntryMix(obj.fCurrentEntryMix),
    fCurrentBinIndex(obj.fCurrentBinIndex),
    fNumberMixed(obj.fNumberMixed),
    fMultiInputHandler(obj.fMultiInputHandler),
    fMCEventHandler(obj.fMCEventHandler)
{
}


AliAnalysisTaskMulti &AliAnalysisTaskMulti::operator=(const AliAnalysisTaskMulti &other)
{
// Assignment
    AliAnalysisTaskSE::operator=(other);
    return *this;
}


void AliAnalysisTaskMulti::ConnectInputData(Option_t* /*option*/)
{
// Connect the input data
    if(fDebug > 1) printf("AnalysisTaskSE::ConnectInputData() \n");
//
//  ESD
//
    
    
    // connect input handlers (multi input handler is handled)
    ConnectMultiHandler();

    if(fInputHandler) {
        if((fInputHandler->GetTree())->GetBranch("ESDfriend."))
            fESDfriend = ((AliESDInputHandler *)fInputHandler)->GetESDfriend();
        fInputEvent = fInputHandler->GetEvent();
    } else if(fMCEvent) {
        AliWarning("No Input Event Handler connected, only MC Truth Event Handler") ;
    } else {
        AliError("No Input Event Handler connected") ;
        return ;
    }
    
    // disconnect multi handler
    DisconnectMultiHandler();
}


void AliAnalysisTaskMulti::Exec(Option_t *option)
{
//
// Exec analysis of one event

    // connect input handler
    ConnectMultiHandler();
    
    if(fDebug >= 10)
        printf("Task is active %5d\n", IsActive());
    if(fDebug > 1) AliInfo("AliAnalysisTaskMulti::Exec() \n");
//
    AliAODHandler *handler = dynamic_cast<AliAODHandler *>
                             ((AliAnalysisManager::GetAnalysisManager())->GetOutputEventHandler());
    AliAODInputHandler *aodH = dynamic_cast<AliAODInputHandler *>(fInputHandler);
//
// Was event selected ? If no event selection mechanism, the event SHOULD be selected (AG)
    UInt_t isSelected = AliVEvent::kAny;
    if(fInputHandler && fInputHandler->GetEventSelection()) {
        // Get the actual offline trigger mask for the event and AND it with the
        // requested mask. If no mask requested select by default the event.
        if(fOfflineTriggerMask)
            isSelected = fOfflineTriggerMask & fInputHandler->IsEventSelected();
    }
//  Functionality below moved in the filter tasks (AG)
//    if (handler) handler->SetFillAOD(isSelected);
    if(fInputHandler) {
        fEntry = fInputHandler->GetReadEntry();
        fESDfriend = ((AliESDInputHandler *)fInputHandler)->GetESDfriend();
    }
// Notify the change of run number
    if(InputEvent() && (InputEvent()->GetRunNumber() != fCurrentRunNumber)) {
        fCurrentRunNumber = InputEvent()->GetRunNumber();
        NotifyRun();
    } else if(fMCEvent)
        fEntry = fMCEvent->Header()->GetEvent();
    if(!((Entry()-1)%100) && fDebug > 0)
        AliInfo(Form("%s ----> Processing event # %lld", CurrentFileName(), Entry()));
    if(handler && aodH) {
        fMCEvent = aodH->MCEvent();
        Bool_t merging = aodH->GetMergeEvents();
        if(!(handler->IsStandard()) && !(handler->AODIsReplicated())) {
            if((handler->NeedsHeaderReplication()) && (fgAODHeader)) {
                // copy the contents by assigment
                *fgAODHeader =  *(dynamic_cast<AliAODHeader *>(InputEvent()->GetHeader()));
            }
            if((handler->NeedsTracksBranchReplication() || merging) && (fgAODTracks)) {
                TClonesArray *tracks = (dynamic_cast<AliAODEvent *>(InputEvent()))->GetTracks();
                new(fgAODTracks) TClonesArray(*tracks);
            }
            if((handler->NeedsVerticesBranchReplication() || merging) && (fgAODVertices)) {
                TClonesArray *vertices = (dynamic_cast<AliAODEvent *>(InputEvent()))->GetVertices();
                new(fgAODVertices) TClonesArray(*vertices);
            }
            if((handler->NeedsV0sBranchReplication()) && (fgAODV0s)) {
                TClonesArray *v0s = (dynamic_cast<AliAODEvent *>(InputEvent()))->GetV0s();
                new(fgAODV0s) TClonesArray(*v0s);
            }
            if((handler->NeedsTrackletsBranchReplication()) && (fgAODTracklets)) {
                *fgAODTracklets = *(dynamic_cast<AliAODEvent *>(InputEvent()))->GetTracklets();
            }
            if((handler->NeedsPMDClustersBranchReplication()) && (fgAODPMDClusters)) {
                TClonesArray *pmdClusters = (dynamic_cast<AliAODEvent *>(InputEvent()))->GetPmdClusters();
                new(fgAODPMDClusters) TClonesArray(*pmdClusters);
            }
            if((handler->NeedsJetsBranchReplication() || merging) && (fgAODJets)) {
                TClonesArray *jets = (dynamic_cast<AliAODEvent *>(InputEvent()))->GetJets();
                new(fgAODJets) TClonesArray(*jets);
            }
            if((handler->NeedsFMDClustersBranchReplication()) && (fgAODFMDClusters)) {
                TClonesArray *fmdClusters = (dynamic_cast<AliAODEvent *>(InputEvent()))->GetFmdClusters();
                new(fgAODFMDClusters) TClonesArray(*fmdClusters);
            }
            if((handler->NeedsCaloClustersBranchReplication() || merging) && (fgAODCaloClusters)) {
                TClonesArray *caloClusters = (dynamic_cast<AliAODEvent *>(InputEvent()))->GetCaloClusters();
                new(fgAODCaloClusters) TClonesArray(*caloClusters);
            }
            if((handler->NeedsMCParticlesBranchReplication() || merging) && (fgAODMCParticles)) {
                TClonesArray *mcParticles = (TClonesArray *)((dynamic_cast<AliAODEvent *>(InputEvent()))->FindListObject("mcparticles"));
                new(fgAODMCParticles) TClonesArray(*mcParticles);
            }
            if((handler->NeedsDimuonsBranchReplication() || merging) && (fgAODDimuons)) {
                fgAODDimuons->Clear();
                TClonesArray &dimuons = *fgAODDimuons;
                TClonesArray &tracksnew = *fgAODTracks;
                Int_t nMuonTrack[100];
                for(Int_t imuon = 0; imuon < 100; imuon++) nMuonTrack[imuon] = 0;
                Int_t nMuons=0;
                for(Int_t ii=0; ii < fgAODTracks->GetEntries(); ii++) {
                    AliAODTrack *track = (AliAODTrack *) fgAODTracks->At(ii);
                    if(track->IsMuonTrack()) {
                        nMuonTrack[nMuons]= ii;
                        nMuons++;
                    }
                }
                Int_t jDimuons=0;
                if(nMuons >= 2) {
                    for(Int_t i = 0; i < nMuons; i++) {
                        Int_t index0 = nMuonTrack[i];
                        for(Int_t j = i+1; j < nMuons; j++) {
                            Int_t index1 = nMuonTrack[j];
                            tracksnew.At(index0)->ResetBit(kIsReferenced);
                            tracksnew.At(index0)->SetUniqueID(0);
                            tracksnew.At(index1)->ResetBit(kIsReferenced);
                            tracksnew.At(index1)->SetUniqueID(0);
                            new(dimuons[jDimuons++]) AliAODDimuon(tracksnew.At(index0),tracksnew.At(index1));
                        }
                    }
                }
            }
            // Additional merging if needed
            if(merging) {
                // mcParticles
                TClonesArray *mcparticles = (TClonesArray *)((aodH->GetEventToMerge())->FindListObject("mcparticles"));
                Int_t npart = mcparticles->GetEntries();
                Int_t nc = fgAODMCParticles->GetEntries();
                Int_t nc0 = nc;
                for(Int_t i = 0; i < npart; i++) {
                    AliAODMCParticle *particle = (AliAODMCParticle *) mcparticles->At(i);
                    new((*fgAODMCParticles)[nc++]) AliAODMCParticle(*particle);
                }
                // tracks
                TClonesArray *tracks = aodH->GetEventToMerge()->GetTracks();
                Int_t ntr = tracks->GetEntries();
                nc  = fgAODTracks->GetEntries();
                for(Int_t i = 0; i < ntr; i++) {
                    AliAODTrack    *track = (AliAODTrack *) tracks->At(i);
                    AliAODTrack *newtrack = new((*fgAODTracks)[nc++]) AliAODTrack(*track);
                    newtrack->SetLabel(newtrack->GetLabel() + nc0);
                }
                for(Int_t i = 0; i < nc; i++) {
                    AliAODTrack *track = (AliAODTrack *) fgAODTracks->At(i);
                    track->ResetBit(kIsReferenced);
                    track->SetUniqueID(0);
                }
                // clusters
                TClonesArray *clusters = aodH->GetEventToMerge()->GetCaloClusters();
                Int_t ncl  = clusters->GetEntries();
                nc         =  fgAODCaloClusters->GetEntries();
                for(Int_t i = 0; i < ncl; i++) {
                    AliAODCaloCluster    *cluster = (AliAODCaloCluster *) clusters->At(i);
                    new((*fgAODCaloClusters)[nc++]) AliAODCaloCluster(*cluster);
                }
                // cells
                AliAODCaloCells *cellsA = aodH->GetEventToMerge()->GetEMCALCells();
                Int_t ncells  = cellsA->GetNumberOfCells();
                nc = fgAODEmcalCells->GetNumberOfCells();
                for(Int_t i  = 0; i < ncells; i++) {
                    Int_t cn  = cellsA->GetCellNumber(i);
                    Int_t pos = fgAODEmcalCells->GetCellPosition(cn);
                    if(pos >= 0) {
                        Double_t amp = cellsA->GetAmplitude(i) + fgAODEmcalCells->GetAmplitude(pos);
                        fgAODEmcalCells->SetCell(pos, cn, amp);
                    } else {
                        Double_t amp = cellsA->GetAmplitude(i);
                        fgAODEmcalCells->SetCell(nc++, cn, amp);
                        fgAODEmcalCells->Sort();
                    }
                }
            } // merging
            handler->SetAODIsReplicated();
        }
    }
// Call the user analysis
//     AliMCEventHandler*    mcH = 0;
//     mcH = (AliMCEventHandler*) ((AliAnalysisManager::GetAnalysisManager())->GetMCtruthEventHandler());


    if(!fMCEventHandler) {
        if(isSelected)
            UserExec(option);
    } else {
      if(isSelected && (fMCEventHandler->InitOk()))
            UserExec(option);
    }
// Added protection in case the derived task is not an AOD producer.
    AliAnalysisDataSlot *out0 = GetOutputSlot(0);
    if(out0 && out0->IsConnected()) PostData(0, fTreeA);
    
    DisconnectMultiHandler();
}

void AliAnalysisTaskMulti::UserExecMix(Option_t *option)
{
    AliDebug(AliLog::kDebug+5,Form("<- %s",option));
    AliDebug(AliLog::kDebug+5,"->");
}

void AliAnalysisTaskMulti::ConnectMultiHandler()
{
  fInputHandler = (AliInputEventHandler *)((AliAnalysisManager::GetAnalysisManager())->GetInputEventHandler());
  fMultiInputHandler = dynamic_cast<AliMultiInputEventHandler *>(fInputHandler);
  if(fMultiInputHandler) {
//     fMultiInputHandler = (AliMultiInputEventHandler*)fInputHandler;
    fInputHandler = dynamic_cast<AliInputEventHandler *>(fMultiInputHandler->GetFirstInputEventHandler());
    fMCEventHandler = dynamic_cast<AliMCEventHandler *>(fMultiInputHandler->GetFirstMCEventHandler());
  }
  if(fMCEventHandler) fMCEvent = fMCEventHandler->MCEvent();
}
void AliAnalysisTaskMulti::DisconnectMultiHandler()
{
  if(fMultiInputHandler) fInputHandler = fMultiInputHandler;
}

