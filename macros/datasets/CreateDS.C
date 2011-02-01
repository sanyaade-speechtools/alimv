void CreateDS(TString txtfile = "ESD_vala_LHC10a12.txt", TString dsName = "test")
{
   TProof *p = TProof::Open("");
   TFileCollection *coll = new TFileCollection;
   coll->AddFromFile(txtfile.Data());
   p->RemoveDataSet(dsName.Data());
   p->RegisterDataSet(dsName.Data(), coll);
   p->VerifyDataSet(dsName.Data());
   p->ShowDataSets();
}
