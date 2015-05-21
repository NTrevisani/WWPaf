#include "TROOT.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "TList.h"
//#include "TIter.h"
#include "TKey.h"
#include "TObject.h"

TFile *ttbar = new TFile("rootFiles/AllJet/OF/TTbar.root","read");
TFile *inclusive = new TFile("rootFiles/AllJet/OF/TTbarInclusiveMC.root","read");
TFile *finale = new TFile("rootFiles/AllJet/OF/TTbarInclusiveMCnew.root","recreate");
 

void sub(TString name){
  TH1F *htt = (TH1F*) ttbar->Get(name);
  TH1F *hincl =(TH1F*) inclusive->Get(name);
  
  hincl->Add(htt,-1);
  
  hincl->Write();
}

int macroSubtract(){
  
  TList* list = ttbar->GetListOfKeys() ;
  cout<<list->GetSize()<<endl;
  
  for (int i = 11; i < list->GetSize(); ++i){
    TString coso = list->At(i)->GetName();
    cout<<coso<<endl;
    sub(coso);
  }
  


}


