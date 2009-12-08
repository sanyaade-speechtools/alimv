//
// Class AliRsnOutputObj
//
// Rsn output Object
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Alberto Pulvirenti (alberto.pulvirenti@ct.infn.it)
//

#include <TObjString.h>

#include "AliLog.h"

#include "AliRsnOutputObj.h"
#include <THnSparse.h>
#include <TList.h>
#include <TMath.h>


ClassImp(AliRsnOutputObj)

//_________________________________________________________________________________________________
AliRsnOutputObj::AliRsnOutputObj(const char*name, const char *title) : TNamed(name, title),
    fOutList(0),
    fCurrentProjectionIndex(0),
    fBinningCutIndex(0),
    fBinningStep(-1),
    fBinningMin(-1),
    fBinningMax(-1),
    fDoMinus(kFALSE),
    fDoDivide(kFALSE),
    fForceRebin(1),
    fParameterHistogram(0),
    fDoNormalize(kTRUE),
    fNormalizeMin(0.0),
    fNormalizeMax(0.0),
    fSignalBinsLeft(0),
    fSignalBinsRight(0)
{
//
// Default constructor.
//
  Int_t i, j;

  for (i = 0;i < kNumberOfArray;i++) {
      for (j = 0;j < kNumberOfBinningHistograms;j++) {
          fHistogram[i][j] = 0;
          fNormHistogram[i][j] = 0;
          fExtraHistograms[i][j] = 0;
        }
    }

  for (i = 0;i < 2;i++) {
      fForceUserRange[i] = 0.0;
    }
}

void AliRsnOutputObj::Clear() {
}


void AliRsnOutputObj::Print() {
  PrintArray(kOne);
  PrintArray(kTwo);
}


void AliRsnOutputObj::PrintArray(Int_t index) {
  AliInfo(Form("Printing Array %i ...", index));
  TObjArrayIter iter(&fHistogramNameArray[index]);
  TObjString *strObj;

  while ((strObj = (TObjString*) iter.Next()))
    AliInfo(Form("== %s", strObj->GetString().Data()));

  AliInfo(Form("End Array %i ...", index));
}



void AliRsnOutputObj::AddHistogram(Int_t index, TString listName, TString name, Int_t projection) {
  fCurrentProjectionIndex = projection;
  fHistogramNameArray[index].Add(new TObjString(Form("%s/%s/%d", listName.Data(), name.Data(), projection)));
}

void AliRsnOutputObj::InitHistograms(TList* listAll) {
  fOutList = new TList();
  fOutList->SetName(GetName());
//   fOutList->SetSetTitle(GetTitle());

//   fOutList= listAll;

  if (fBinningStep > 0) {
      Double_t iStep;
      Int_t i = 1;

      for (iStep = fBinningMin;iStep < fBinningMax;iStep += fBinningStep) {
          if (fHistogramNameArray[0].GetEntries() > 0) {
              fHistogram[0][i] = new TH1D();
              fOutList->Add(fHistogram[0][i]);
            }

          if (fHistogramNameArray[1].GetEntries() > 0) {
              fHistogram[1][i] = new TH1D();
              fOutList->Add(fHistogram[1][i]);
            }

          i++;
        }
    }

//   else {

  if (fHistogramNameArray[0].GetEntries() > 0) {
      fHistogram[0][0] = new TH1D();
      fOutList->Add(fHistogram[0][0]);

    }

  if (fHistogramNameArray[1].GetEntries() > 0) {
      fHistogram[1][0] = new TH1D();
      fOutList->Add(fHistogram[1][0]);
    }

  listAll->Add(fOutList);

}


void AliRsnOutputObj::InitExtraHistograms(TList* listAll) {
//   fOutList=listAll;

  fOutList = (TList*) listAll->FindObject(GetName());
  fOutList->Print();

  if (fBinningStep > 0) {
      Double_t iStep;
      Int_t i = 1;

      for (iStep = fBinningMin;iStep < fBinningMax;iStep += fBinningStep) {
          if (fDoMinus) {
              fExtraHistograms[0][i] = new TH1D();
              fOutList->Add(fExtraHistograms[0][i]);
            }

          if (fDoDivide) {
              fExtraHistograms[1][i] = new TH1D();
              fOutList->Add(fExtraHistograms[1][i]);
            }

          i++;
        }
    }

  if (fDoMinus) {
      fExtraHistograms[0][0] = new TH1D();
      fOutList->Add(fExtraHistograms[0][0]);
      fParameterHistogram = new TH1D();
      fOutList->Add(fParameterHistogram);
    }

  if (fDoDivide) {
      fExtraHistograms[1][0] = new TH1D();
      fOutList->Add(fExtraHistograms[1][0]);
    }



//   listAll->Add(fOutList);
}



void AliRsnOutputObj::ProcessHistograms(TFile* file, TList* listIn) {
//   file->ls();
//   AliInfo(Form("%d", kOne));

//   file->Get
  if (fHistogramNameArray[kOne].GetEntries() > 0)
    ProcessHistograms(kOne, file, listIn);

  if (fHistogramNameArray[kTwo].GetEntries() > 0)
    ProcessHistograms(kTwo, file, listIn);
}

void AliRsnOutputObj::ProcessHistograms(Int_t index, TFile* file, TList *listIn) {
  if (listIn) {
      file = 0;
//     AliInfo(Form("Doing LKIIIIIIIIIIIIIIIIIIISt"));
    } else {
//     AliInfo(Form("Doing FIIIIIILE"));
    }

  TObjArrayIter iter1(&fHistogramNameArray[index]);

  TObjString *strObj = 0;
  THnSparseD* sparseTmp = 0;
  TH1D *histTmp = 0;
  TList *list = 0;
  TObjArray *array = 0;
  Int_t i, iIndexInOneTwo = -1;
  Double_t iStep = 0.0, binStep = 1.0, binMin = 0.0, binMax = 1.0;
  Bool_t doBinning = kFALSE;

  while ((strObj = (TObjString*) iter1.Next())) {
      i = 0;
      iIndexInOneTwo++;

      if (fBinningStep > 0) {
          binStep = fBinningStep;
          binMin = fBinningMin - fBinningStep;
          binMax = fBinningMax;
          doBinning = kTRUE;
        }

      for (iStep = binMin;iStep < binMax;iStep += binStep) {
//       AliInfo(Form("fBinningMin=%f fBinningMax=%f iStep=%f", fBinningMin, fBinningMax, iStep));

//       AliInfo(Form("[%.2f-%.2f]", iStep, iStep+binStep));
          array = strObj->GetString().Tokenize("/");

          if (file)
            list = (TList*) file->Get(((TObjString*)array->At(0))->GetString().Data());
          else {
              list = listIn;
            }

          if (list) {
//         list->Print();
//         AliInfo(Form("Doing Binning ... (%s)",((TObjString*)array->At(1))->GetString().Data()));
              sparseTmp = (THnSparseD*) list->FindObject(((TObjString*)array->At(1))->GetString().Data());

              if (sparseTmp) {

                  if (doBinning) {
//             AliInfo(Form("Doing Binning ... (%d)",fBinningCutIndex));
                      sparseTmp->GetAxis(fBinningCutIndex)->SetRangeUser(iStep + binStep / 2, iStep + binStep / 2);
                    }

                  histTmp = (TH1D*)(sparseTmp->Projection(((TObjString*)array->At(2))->GetString().Atoi()));

                  if (!histTmp) {
                      AliError("histTmp is null !!!");
                    }

//           histTmp->Print();
                  TString histname(fHistogram[index][i]->GetName());

                  if (histname.IsNull()) {
                      if ((!doBinning) || i == 0) {
                          fHistogram[index][i]->SetName(Form("%s_cut%d", histTmp->GetName(), fBinningCutIndex));
                          fHistogram[index][i]->SetTitle(Form("%s_cut%d", histTmp->GetTitle(), fBinningCutIndex));
                        } else {
                          fHistogram[index][i]->SetName(Form("%s_cut%d_[%.2f-%.2f]", histTmp->GetName(), fBinningCutIndex, iStep, iStep + fBinningStep));
                          fHistogram[index][i]->SetTitle(Form("%s cut%d [%.2f-%.2f]", histTmp->GetTitle(), fBinningCutIndex, iStep, iStep + fBinningStep));
//               fHistogram[index][i]->Print();
                        }

//             fHistogram[index][i]->Print();
                      fHistogram[index][i]->SetBins(histTmp->GetNbinsX(), histTmp->GetXaxis()->GetXmin(), histTmp->GetXaxis()->GetXmax());

                      fHistogram[index][i]->Rebin(fForceRebin);
                    }

                  histTmp->Rebin(fForceRebin);

                  fHistogram[index][i]->Add(histTmp);

                  if (file)
                    delete histTmp;
                } else {
                  AliError(Form("%s was not found", ((TObjString*)array->At(1))->GetString().Data()));
                }

              if (file)
                delete list;
            } else {
              AliError(Form("%s was not found", (((TObjString*)array->At(0))->GetString().Data())));
            }

          i++;

        }
    }
}


void AliRsnOutputObj::ProcessExtraHistograms(TList* list) {
  if (!list) {
      return;
    }

//   list->Print();

  fOutList = (TList*) list->FindObject(GetName());

  if (fDoMinus) {
//     DoExtraHistogramMinus(list);
      DoExtraHistogramMinus(fOutList);
    }

  if (fDoDivide) {
//     DoExtraHistogramDivide(list);
      DoExtraHistogramDivide(fOutList);
    }
}


void AliRsnOutputObj::DoExtraHistogramMinus(TList* list) {
//   fNormHistogram

  TObjArrayIter iter1(&fHistogramNameArray[0]);
  TObjArrayIter iter2(&fHistogramNameArray[1]);
  TObjString *strObj = 0;
  TObjArray *array = 0;
  TString tmpStr;
  Int_t iIndexInOneTwo = 0;
  Double_t iStep = 0.0, binStep = 1.0, binMin = 0.0, binMax = 1.0;
  Bool_t doBinning = kFALSE;
  TH1D *histTmp = 0;

  Int_t index = 0, numberOfHistogramsOne = 0, numberOfHistogramsTwo = 0;

  while ((strObj = (TObjString*) iter1.Next())) {
      if (iIndexInOneTwo++ > 0)
        continue;

      if (fBinningStep > 0) {
          binStep = fBinningStep;
          binMin = fBinningMin - fBinningStep;
          binMax = fBinningMax;
          doBinning = kTRUE;
        }

      array = strObj->GetString().Tokenize("/");

      for (iStep = binMin;iStep < binMax;iStep += binStep) {
          if (list) {
              if (numberOfHistogramsOne == 0)
                tmpStr = Form("%s_proj_%d_cut%d", ((TObjString*)array->At(1))->GetString().Data(), fCurrentProjectionIndex, fBinningCutIndex);
              else
                tmpStr = Form("%s_proj_%d_cut%d_[%.2f-%.2f]", ((TObjString*)array->At(1))->GetString().Data(), fCurrentProjectionIndex, fBinningCutIndex, iStep, iStep + fBinningStep);

              histTmp = (TH1D*) list->FindObject(Form("%s", tmpStr.Data()));
              if (!histTmp) {
                  AliError(Form("%s was not found!!! Skipping ...", tmpStr.Data()));
                  continue;
                }

//         AliInfo(Form("%.2f %s %d", iStep, histTmp->GetName(), numberOfHistogramsOne));
              fNormHistogram[index][numberOfHistogramsOne] = (TH1D*) histTmp->Clone();
              fNormHistogram[index][numberOfHistogramsOne]->SetName(Form("%s_norm", fNormHistogram[index][numberOfHistogramsOne]->GetName()));
              fNormHistogram[index][numberOfHistogramsOne]->SetTitle(Form("%s norm", fNormHistogram[index][numberOfHistogramsOne]->GetTitle()));

              list->Add(fNormHistogram[index][numberOfHistogramsOne]);

//         fNormHistogram[index][numberOfHistogramsOne]->Print();
              numberOfHistogramsOne++;
            }
        }

    }

  iIndexInOneTwo = 0;

  index = 1;

  while ((strObj = (TObjString*) iter2.Next())) {
      if (iIndexInOneTwo++ > 0)
        continue;

      if (fBinningStep > 0) {
          binStep = fBinningStep;
          binMin = fBinningMin - fBinningStep;
          binMax = fBinningMax;
          doBinning = kTRUE;
        }

      array = strObj->GetString().Tokenize("/");

      for (iStep = binMin;iStep < binMax;iStep += binStep) {
          if (list) {
              if (numberOfHistogramsTwo == 0)
                tmpStr = Form("%s_proj_%d_cut%d", ((TObjString*)array->At(1))->GetString().Data(), fCurrentProjectionIndex, fBinningCutIndex);
              else
                tmpStr = Form("%s_proj_%d_cut%d_[%.2f-%.2f]", ((TObjString*)array->At(1))->GetString().Data(), fCurrentProjectionIndex, fBinningCutIndex, iStep, iStep + fBinningStep);

              histTmp = (TH1D*) list->FindObject(Form("%s", tmpStr.Data()));

              if (!histTmp) {
                  AliError(Form("%s was not found!!! Skipping ...", tmpStr.Data()));
                  continue;
                }

//         AliInfo(Form("%.2f %s", iStep, histTmp->GetName()));
              fNormHistogram[index][numberOfHistogramsTwo] = (TH1D*) histTmp->Clone();
              fNormHistogram[index][numberOfHistogramsTwo]->SetName(Form("%s_norm", fNormHistogram[index][numberOfHistogramsTwo]->GetName()));
              fNormHistogram[index][numberOfHistogramsTwo]->SetTitle(Form("%s norm", fNormHistogram[index][numberOfHistogramsTwo]->GetTitle()));

              list->Add(fNormHistogram[index][numberOfHistogramsTwo]);

//         fNormHistogram[index][numberOfHistogramsTwo]->Print();
              numberOfHistogramsTwo++;
            }
        }

//     i++;
    }

//   AliInfo(Form("nHist1 = %d \t nHist2 = %d", numberOfHistogramsOne, numberOfHistogramsTwo));

//   if (numberOfHistogramsOne != numberOfHistogramsTwo)
//   {
//     AliWarning(Form("Number of histograms are not equal (%d!=%d)!!! Using lower number ...", numberOfHistogramsOne, numberOfHistogramsTwo));
//     if (numberOfHistogramsOne > numberOfHistogramsTwo)
//       numberOfHistogramsOne = numberOfHistogramsTwo;
//   }

// AliInfo(Form("nHist1 = %d ", numberOfHistogramsOne));

  Int_t fNormalizeBinMinI = 0;

  Int_t fNormalizeBinMaxI = 0;

  Double_t normtmp = 1.0, normFactor = 1.0;

  Int_t i = 0;

  for (i = 0;i < numberOfHistogramsOne;i++) {
//     fNormHistogram[0][i]->Print();
//     fNormHistogram[1][i]->Print();
      if (fHistogramNameArray[1].GetEntries() > 0) {

          if (fNormalizeMin == fNormalizeMax) {
              normtmp = fNormHistogram[1][i]->Integral() > 0 ? fNormHistogram[1][i]->Integral() : 1;
//       Info("", Form("%f", normtmp));
              normFactor = (Double_t) fNormHistogram[0][i]->Integral() / normtmp ;
            } else {
              fNormalizeBinMinI = fNormHistogram[0][i]->FindBin(fNormalizeMin);
              fNormalizeBinMaxI = fNormHistogram[0][i]->FindBin(fNormalizeMax);
//           AliInfo(Form("Norm Bin Range is %d %d",fNormalizeBinMinI,fNormalizeBinMaxI));
              normtmp = fNormHistogram[1][i]->Integral(fNormalizeBinMinI, fNormalizeBinMaxI) > 0 ? fNormHistogram[1][i]->Integral(fNormalizeBinMinI, fNormalizeBinMaxI) : 1;
//       Info("", Form("%f", normtmp));
              normFactor = (Double_t) fNormHistogram[0][i]->Integral(fNormalizeBinMinI, fNormalizeBinMaxI) / normtmp;
            }
        }

//     AliInfo(Form("normFactor = %f ", normFactor));
//     TH1D *hSub

      fExtraHistograms[0][i]->SetName(fNormHistogram[0][i]->GetName());
      fExtraHistograms[0][i]->SetTitle(fNormHistogram[0][i]->GetTitle());
      fExtraHistograms[0][i]->SetBins(fNormHistogram[0][i]->GetNbinsX(), fNormHistogram[0][i]->GetXaxis()->GetXmin(), fNormHistogram[0][i]->GetXaxis()->GetXmax());


      fExtraHistograms[0][i]->Add(fNormHistogram[0][i]);
      fExtraHistograms[0][i]->SetName(Form("sub_%s", fExtraHistograms[0][i]->GetName()));

      if (fHistogramNameArray[1].GetEntries() > 0) {
          if (fNormHistogram[1][i]) {
              if (fDoNormalize)
                fNormHistogram[1][i]->Scale(normFactor);

              fExtraHistograms[0][i]->Add(fNormHistogram[1][i], -1);
            }
        }

//     fExtraHistograms[0][i]->Print();
    }


// fill fParameterHistogram histogram
//   fParameterHistogram = new TH1D("param","param",(Int_t)((binMax-binMin))/binStep,binMin,binMax);
  fParameterHistogram->SetName(Form("%s_param",fOutList->GetName()));
  fParameterHistogram->SetTitle("param");
  fParameterHistogram->SetBins((Int_t)((binMax-binMin))/binStep,binMin+binStep,binMax);
  
//   fParameterHistogram->Print();
  Int_t mybin;
  Double_t myIntegral = 0.0;
  Double_t myIntegralError = 0.0;
  Int_t binsL = fSignalBinsLeft;
  Int_t binsR = fSignalBinsRight;
  i=1;
  for (iStep = binMin+binStep;iStep < binMax;iStep += binStep) {
//     AliInfo(Form("%f %f",iStep,iStep+binStep));
    fExtraHistograms[0][i]->Print();
    mybin = fExtraHistograms[0][i]->FindBin(1.020);
//     AliInfo(Form("mybin = %d",mybin));
    myIntegral = fExtraHistograms[0][i]->Integral(mybin-binsL,mybin+binsR);
//     AliInfo(Form("integral(%d %d) is %f",mybin-binsL,mybin+binsR,myIntegral));

    myIntegral = 0.0;
    myIntegralError = 0.0;
    for (Int_t j=mybin-binsL;j< mybin+binsR+1;j++) {
      AliInfo(Form("%d %f %f (%f)",j,fExtraHistograms[0][i]->GetBinContent(j),fExtraHistograms[0][i]->GetBinError(j),TMath::Power(fExtraHistograms[0][i]->GetBinError(j),2)));
      if (fExtraHistograms[0][i]->GetBinContent(j)<0) continue;
      myIntegral += fExtraHistograms[0][i]->GetBinContent(j);
      myIntegralError += TMath::Power(fExtraHistograms[0][i]->GetBinError(j),2);
    }
    
//     AliInfo(Form("integral(%d %d) is %f with error %f",mybin-binsL,mybin+binsR,myIntegral,TMath::Sqrt(myIntegralError)));
    
    if (myIntegral>0) {
      fParameterHistogram->SetBinContent(i,myIntegral);
      fParameterHistogram->SetBinError(i,TMath::Sqrt(myIntegralError));
    }
    else {
      fParameterHistogram->SetBinContent(i,0.01);
      fParameterHistogram->SetBinError(i,TMath::Sqrt(0));
    }
    i++;
  }
}


void AliRsnOutputObj::DoExtraHistogramDivide(TList* list) {
  if (!list)
    return;
}


void AliRsnOutputObj::SetBinningCut(Int_t index, Double_t min, Double_t max, Double_t step) {
  fBinningCutIndex = index;
  fBinningMin = min;
  fBinningMax = max;
  fBinningStep = step;
}


void AliRsnOutputObj::SetNormalizeRange(Double_t min, Double_t max) {
  fNormalizeMin = min;
  fNormalizeMax = max;
}


// void AliRsnOutputObj::SetNormalizeRange(Double_t min, Double_t max)
// {
//
// }




// void AliRsnOutputObj::LoopOverAllHistograms(Int_t index)
// {
//   TObjArrayIter iter1(&fHistogramNameArray[index]);
//   TObjString *strObj = 0;
//   THnSparseD* sparseTmp = 0;
//   TH1D *histTmp = 0;
//   TList *list = 0;
//   TObjArray *array = 0;
//   Int_t i, iIndexInOneTwo = -1;
//   Double_t iStep = 0.0, binStep = 1.0, binMin = 0.0, binMax = 1.0;
//   Bool_t doBinning = kFALSE;
//   while ((strObj = (TObjString*) iter1.Next()))
//   {
//     i = 0;
//     iIndexInOneTwo++;
//     if (fBinningStep > 0)
//     {
//       binStep = fBinningStep;
//       binMin = fBinningMin;
//       binMax = fBinningMax;
//       doBinning = kTRUE;
//     }
//
//     for (iStep = binMin;iStep < binMax;iStep += binStep)
//     {
//     }
//   }
// }
