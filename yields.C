#include "TROOT.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"


float yields(TString variable = "hPtLepton1WWLevel3"){
  //data file
  TFile *fdata = new TFile("rootFiles/AllJet/OF/DataRun2012_Total.root","read");

  //signal files
  TFile *fsig1 = new TFile("rootFiles/AllJet/OF/WWTo2L2Nu_pow_nnll.root","read");
  TFile *fsig2 = new TFile("rootFiles/AllJet/OF/ggWWto2L.root","read");

  //all the background files
  TFile *fbkg1 = new TFile("rootFiles/AllJet/OF/TTbar.root","read");
  TFile *fbkg2 = new TFile("rootFiles/AllJet/OF/TW.root","read");
  TFile *fbkg3 = new TFile("rootFiles/AllJet/OF/WZ.root","read");
  TFile *fbkg4 = new TFile("rootFiles/AllJet/OF/ZZ.root","read");
  TFile *fbkg5 = new TFile("rootFiles/AllJet/OF/WgammaNoStar.root","read");
  TFile *fbkg6 = new TFile("rootFiles/AllJet/OF/WgammaStar.root","read");
  TFile *fbkg7 = new TFile("rootFiles/AllJet/OF/WJetsFakes_Total.root","read");
  TFile *fbkg8 = new TFile("rootFiles/AllJet/OF/DY.root","read");
  TFile *fbkg9 = new TFile("rootFiles/AllJet/OF/DYtautau.root","read");
  TFile *fbkg10 = new TFile("rootFiles/AllJet/OF/Zgamma.root","read");
  TFile *fbkg11 = new TFile("rootFiles/AllJet/OF/VVV.root","read");
  TFile *fbkg12 = new TFile("rootFiles/AllJet/OF/HWW125.root","read");
  TFile *fbkg13 = new TFile("rootFiles/AllJet/OF/VBF.root","read");

  //defining names
  TString name[13];
  name[0]  = "VBF";
  name[1]  = "TTbar";
  name[2]  = "TW";
  name[3]  = "WZ";
  name[4]  = "ZZ";
  name[5]  = "WgammaNoStar";
  name[6]  = "WgammaStar";
  name[7]  = "WJets";
  name[8]  = "DY";
  name[9]  = "DYtautau";
  name[10] = "Zgamma";
  name[11] = "VVV";
  name[12] = "HWW125";

  //defining graphs
  TGraph *g  = new TGraph();
  TGraph *g2 = new TGraph();
  TGraph *g3 = new TGraph();
  TGraph *g4 = new TGraph();

  //defining data histogram
  TH1F* hdata = (TH1F*)fdata -> Get(variable);

  //defining signal histograms
  TH1F* hsig1 = (TH1F*)fsig1 -> Get(variable);
  TH1F* hsig2 = (TH1F*)fsig2 -> Get(variable);

  //defining background histograms
  TH1F *hbkg[13];

  hbkg[0]   = (TH1F*)fbkg13 -> Get(variable);
  hbkg[1]   = (TH1F*)fbkg1  -> Get(variable);
  hbkg[2]   = (TH1F*)fbkg2  -> Get(variable);
  hbkg[3]   = (TH1F*)fbkg3  -> Get(variable);
  hbkg[4]   = (TH1F*)fbkg4  -> Get(variable);
  hbkg[5]   = (TH1F*)fbkg5  -> Get(variable);
  hbkg[6]   = (TH1F*)fbkg6  -> Get(variable);
  hbkg[7]   = (TH1F*)fbkg7  -> Get(variable);
  hbkg[8]   = (TH1F*)fbkg8  -> Get(variable);
  hbkg[9]   = (TH1F*)fbkg9  -> Get(variable);
  hbkg[10]  = (TH1F*)fbkg10 -> Get(variable);
  hbkg[11]  = (TH1F*)fbkg11 -> Get(variable);
  hbkg[12]  = (TH1F*)fbkg12 -> Get(variable);
  
  //histograms without Ht cut
  variable.Remove(variable.Length() - 1);
  variable = variable + "NoHt3";

  TH1F* hsig1b = (TH1F*)fsig1 -> Get(variable);
  TH1F* hsig2b = (TH1F*)fsig2 -> Get(variable);

  TH1F *hbkgb[13];

  hbkgb[0]   = (TH1F*)fbkg13 -> Get(variable);
  hbkgb[1]   = (TH1F*)fbkg1  -> Get(variable);
  hbkgb[2]   = (TH1F*)fbkg2  -> Get(variable);
  hbkgb[3]   = (TH1F*)fbkg3  -> Get(variable);
  hbkgb[4]   = (TH1F*)fbkg4  -> Get(variable);
  hbkgb[5]   = (TH1F*)fbkg5  -> Get(variable);
  hbkgb[6]   = (TH1F*)fbkg6  -> Get(variable);
  hbkgb[7]   = (TH1F*)fbkg7  -> Get(variable);
  hbkgb[8]   = (TH1F*)fbkg8  -> Get(variable);
  hbkgb[9]   = (TH1F*)fbkg9  -> Get(variable);
  hbkgb[10]  = (TH1F*)fbkg10 -> Get(variable);
  hbkgb[11]  = (TH1F*)fbkg11 -> Get(variable);
  hbkgb[12]  = (TH1F*)fbkg12 -> Get(variable);
  
  TH1F* hdatab = (TH1F*)fdata -> Get(variable);

  //total number of events - no cut
  cout<<"No Ht Cut"<<endl;

  std::ofstream inFile("yields.txt",std::ios::out);

  inFile<<"\\begin{tabular}{cSS}"<<endl;
  inFile<<"\\toprule"<<endl;
  inFile<<"Process & {No Ht Cut} & {Ht < 250~GeV}\\"<<endl;
  inFile<<"\\midrule"<<endl;

  Float_t total = hsig1b -> Integral() + hsig2b -> Integral();
  Float_t totalHt = hsig1 -> Integral() + hsig2 -> Integral();
  for(int j = 0; j < 12; ++j){
    total += hbkgb[j] -> Integral();
    totalHt += hbkg[j] -> Integral();
  }

  Float_t check = 0.;
  for(int i = 0; i < 12; ++i){
    Float_t num = hbkgb[i] -> Integral();
    Float_t numHt = hbkg[i] -> Integral();
    inFile<<name[i]<<" & "<<num<<" & "<<numHt<<"\\"<<endl;;//100.*num/total<<"%"<<endl;
    check += 100.*num/total;
  }

  inFile<<"qqWW & "<<hsig1b -> Integral()<<" & "<<hsig1 -> Integral()<<" \\"<<endl;//100.*hsig1 -> Integral()/total<<"%"<<endl;
  inFile<<"ggWW & "<<hsig2b -> Integral()<<" & "<<hsig2 -> Integral()<<" \\"<<endl;//100.*hsig2 -> Integral()/total<<"%"<<endl;
  check += 100.*hsig1 -> Integral()/total + 100.*hsig2 -> Integral()/total;  
  inFile<<"\\bottomrule"<<endl;
  inFile<<"Total & "<<total<<" & "<<totalHt<<"\\"<<endl;
  inFile<<"Data & "<<hdatab -> Integral()<<" & "<<hdata -> Integral()<<"\\"<<endl;
  inFile<<"\\end{tabular}"<<endl;

  inFile.close();

  cout<<"Data = "<<hdata -> Integral()<<" -> "<<100.*hsig1 -> Integral()/total<<"%"<<endl;
  cout<<"total = "<<total<<endl;

  //total number of events - lower cut
  cout<<"Ht < 250GeV"<<endl;
  total = hsig1 -> Integral(0.,250.) + hsig2 -> Integral(0.,250.);
  for(int j = 0; j < 12; ++j)
    total += hbkg[j] -> Integral(0.,250.);

  check = 0.;
  for(int i = 0; i < 12; ++i){
    Float_t num = hbkg[i] -> Integral(0.,250.);
    cout<<name[i]<<" = "<<num<<" -> "<<100.*num/total<<"%"<<endl;
    check += 100.*num/total;
  }

  cout<<"qqWW = "<<hsig1 -> Integral(0.,250.)<<" -> "<<100.*hsig1 -> Integral(0.,250.)/total<<"%"<<endl;
  cout<<"ggWW = "<<hsig2 -> Integral(0.,250.)<<" -> "<<100.*hsig2 -> Integral(0.,250.)/total<<"%"<<endl;
  check += 100.*hsig1 -> Integral(0.,250.)/total + 100.*hsig2 -> Integral(0.,250.)/total;  

  cout<<"Data = "<<hdata -> Integral(0.,250.)<<" -> "<<100.*hsig1 -> Integral(0.,250.)/total<<"%"<<endl;
  cout<<"total = "<<total<<endl;

  //total number of events - upper cut SBAGLIATO!!!
  cout<<"Ht > 250GeV"<<endl;
  total = hsig1 -> Integral() + hsig2 -> Integral();
  for(int j = 0; j < 12; ++j)
    total += hbkg[j] -> Integral();

  check = 0.;
  for(int i = 0; i < 12; ++i){
    Float_t num = hbkg[i] -> Integral(250.,3000.);
    cout<<name[i]<<" = "<<num<<" -> "<<100.*num/total<<"%"<<endl;
    check += 100.*num/total;
  }

  cout<<"qqWW = "<<hsig1 -> Integral(250.,3000.)<<" -> "<<100.*hsig1 -> Integral(250.,3000.)/total<<"%"<<endl;
  cout<<"ggWW = "<<hsig2 -> Integral(250.,3000.)<<" -> "<<100.*hsig2 -> Integral(250.,3000.)/total<<"%"<<endl;
  check += 100.*hsig1 -> Integral(250.,3000.)/total + 100.*hsig2 -> Integral(250.,3000.)/total;  

  cout<<"Data = "<<hdata -> Integral(250.,3000.)<<" -> "<<100.*hsig1 -> Integral(250.,3000.)/total<<"%"<<endl;
  cout<<"total = "<<total<<endl;

  return check;
}
