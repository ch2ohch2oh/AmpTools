#include <iostream>
#include <string>
#include <vector>
#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "IUAmpTools/Kinematics.h"
#include "DalitzDataIO/DalitzDataReader.h"

using namespace std;

int main(int argc, char **argv)
{

  // ************************
  // usage
  // ************************

  cout << endl
       << " *** Plotting Data *** " << endl
       << endl;

  if (argc <= 2)
  {
    cout << "Usage:" << endl
         << endl;
    cout << "\tplotData <input file name> <output file name>" << endl
         << endl;
    return 0;
  }

  // ************************
  // parse the command line parameters
  // ************************

  string infilename(argv[1]);
  string outfilename(argv[2]);

  cout << "Input file name  = " << infilename << endl;
  cout << "Output file name = " << outfilename << endl
       << endl;

  // ************************
  // set up a DalitzDataReader object
  // ************************

  cout << "Creating a Data Reader..." << endl;

  vector<string> args;
  args.push_back(infilename);
  DalitzDataReader dataReader(args);

  cout << "... Finished creating a Data Reader" << endl
       << endl;

  // ************************
  // set up an output Root file
  // ************************

  TFile *plotfile = new TFile(outfilename.c_str(), "recreate");
  TH1::AddDirectory(kFALSE);

  const int bins = 100;
  const double m12_min = 2.42;
  const double m12_max = 2.50;
  const double m23_min = 0.27;
  const double m23_max = 0.35;

  TH1F *hm12 = new TH1F("hm12", "hm12", bins, m12_min, m12_max);
  hm12->SetStats(0);
  hm12->SetTitle("Mass of #Lambda_{c}^{+} #pi^{+}");
  hm12->SetXTitle("M(#Lambda_{c}^{+} #pi^{+}) / GeV");
  hm12->SetYTitle("Events");

  TH1F *hm23 = new TH1F("hm23", "hm23", bins, m23_min, m23_max);
  hm23->SetStats(0);
  hm23->SetTitle("Mass of #pi^{+} #pi^{-}");
  hm23->SetTitle("M(#pi^{+} #pi^{-})");
  hm23->SetYTitle("Events");

  TH1F *hm13 = new TH1F("hm13", "hm13", bins, m12_min, m12_max);
  hm13->SetStats(0);
  hm13->SetTitle("Mass of #Lambda_{c}^{+} #pi^{-}");
  hm13->SetTitle("M(#Lambda_{c}^{+} #pi^{-}) / GeV");
  hm13->SetYTitle("Events");

  TH2F *hs12s23 = new TH2F("hs12s23", "hs12s23",
                           bins, m23_min * m23_min, m23_max * m23_max,
                           bins, m12_min * m12_min, m12_max * m12_max);
  hs12s23->SetStats(0);
  hs12s23->SetTitle("Dalitz plot of #Lambda_{c}^{+}(2625) #to #Lambda_{c}^{+} #pi^{+} #pi^{-}");
  hs12s23->SetYTitle("M^{2}(#Lambda_{c}^{+} #pi^{+}) / GeV^{2}");
  hs12s23->SetXTitle("M^{2}(#pi^{+} #pi^{-}) / GeV^{2}");

  // ************************
  // use the DalitzDataReader object to read events
  //  from a file and then fill histograms
  // ************************

  Kinematics *kin;

  while ((kin = dataReader.getEvent()))
  {

    vector<TLorentzVector> pList = kin->particleList();

    hm12->Fill((pList[0] + pList[1]).M());
    hm23->Fill((pList[1] + pList[2]).M());
    hm13->Fill((pList[0] + pList[2]).M());
    hs12s23->Fill((pList[1] + pList[2]).M2(), (pList[0] + pList[1]).M2());

    if (dataReader.eventCounter() % 1000 == 0)
      cout << "Event counter = " << dataReader.eventCounter() << endl;

    delete kin;
  }

  // ************************
  // write to the output Root file
  // ************************

  plotfile->cd();
  hm12->Write();
  hm23->Write();
  hm13->Write();
  hs12s23->Write();
  plotfile->Close();

  delete plotfile;
  delete hm12;
  delete hm23;
  delete hm13;
  delete hs12s23;
}
