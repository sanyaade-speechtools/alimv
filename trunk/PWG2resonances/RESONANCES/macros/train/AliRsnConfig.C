//
// AliRsnConfig class
// ---
// This class summarizes all required settings for an analysis.
// It is used when running an RSN analysis stand-alone, not inserted
// in the train (personal analysis, tests, etc.), with the simple
// steering macro 'rsnAlien.C' provided here.
//
struct AliRsnConfig
{
public:

  AliRsnConfig() :
    dataSource("wn.xml"), treeName("esdTree"), nRead(0), nSkip(0),
    taskList("AddAnalysisTaskRsn.C"),
    outFile("rsn.root")
      { /* does nothing, just safe initialization */ }

  TString     dataSource;   // source containing the events to be read
  TString     treeName;     // name of input TTree
  Int_t       nReadFiles;   // number of files to be read
  Int_t       nSkipFiles;   // number of files to be skipped from beginning

  TString     taskList;     // list of 'AddTask' macros to run --> beware of the order

  TString     outFile;      // name of output file
};
