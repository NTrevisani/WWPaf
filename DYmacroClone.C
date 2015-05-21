//should copy a bad root file in a working root file
//run typing:  root -l DYmacroClone.C                                                                                                                  

#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "TString.h"
#include "TLegend.h"
#include "TObjArray.h"
#include <algorithm>
#include "TCut.h"
#include <iostream>


TString latinoVar = "";
TString darkVar = "";
Float_t range = 1000.;

using namespace std;

void DYmacroClone(){

  TFile* DY = new TFile("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/MoriondeffW/TauTau/latino_DYtt_19.5fb.root","read");

  TTree *tDark = (TTree*) DY -> Get("latino;1");
  cout<<"ciao"<<endl;

  
  TObjArray *tl = tDark->GetListOfBranches();
  TString nBranch = tl->First()->GetName();

  Int_t cont = 0;
  tDark->SetBranchStatus("*",0);
  while (tl -> After(tl->FindObject(nBranch)) != 0){                                                                                           


    nBranch = tl -> At(cont) -> GetName();
  if (cont == 202 || cont == 203) //skip the repetute branches
  tDark->SetBranchStatus(nBranch,0);
  else 
  tDark->SetBranchStatus(nBranch,1);
     ++cont;    
     cout<<cont<<endl;
  }
  
  //  tDark->SetBranchStatus("*",1);

  TFile *newfile = new TFile("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/MoriondeffW/TauTau/latino_DYtt_19.5fb_Small.root","recreate");

  cout<<"trabajando..."<<endl;
  TTree *newtree = tDark->CloneTree();

  newtree->Print();
  newfile->Write();
}
