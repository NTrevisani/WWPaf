#include "TH1F.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include <iostream>
#include <fstream>
#include <algorithm> 
#include "TLatex.h" 
#include "TMath.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include <math>

using namespace std;

TString folder = "rootFiles/AllJet/OF/";

std::ofstream inFile ("0-Jet.txt", std::ios::out);     // ==> root -l -b -q 'computeEff.C("0-Jet")'
//std::ofstream inFile ("1+Jet.txt", std::ios::out);     // ==> root -l -b -q 'computeEff.C("1+Jet")'
//std::ofstream inFile ("Inclusive.txt", std::ios::out); // ==> root -l -b -q 'computeEff.C("Inclusive")'

//histograms
TH1F *h;
TH1F *hh;

//graphs
TGraphAsymmErrors *noHt = new TGraphAsymmErrors;
TGraphAsymmErrors *withHt = new TGraphAsymmErrors;
TGraphAsymmErrors *upHt = new TGraphAsymmErrors;

void compute(TString name = "hEff",
	     TString gen  = "mad",
	     Int_t num    = 0,
	     Int_t choose = 1
	     ){
  //files
  if(choose == 1 || choose == 3){
    TFile *mad = new TFile(folder + "WWTo2L2Nu_" + gen + "_nnll.root","read");
    TFile *madRup = new TFile(folder + "WWTo2L2Nu_" + gen + "_nnllRup.root","read");
    TFile *madRdown = new TFile(folder + "WWTo2L2Nu_" + gen + "_nnllRdown.root","read");
    TFile *madQup = new TFile(folder + "WWTo2L2Nu_" + gen + "_nnllQup.root","read");
    TFile *madQdown = new TFile(folder + "WWTo2L2Nu_" + gen + "_nnllQdown.root","read");
  }
    
  else if (choose == 2){
    TFile *mad = new TFile(folder + "WWTo2L2Nu_" + gen + "_nnllHtPlus.root","read");
    TFile *madRup = new TFile(folder + "WWTo2L2Nu_" + gen + "_nnllRupHtPlus.root","read");
    TFile *madRdown = new TFile(folder + "WWTo2L2Nu_" + gen + "_nnllRdownHtPlus.root","read");
    TFile *madQup = new TFile(folder + "WWTo2L2Nu_" + gen + "_nnllQupHtPlus.root","read");
    TFile *madQdown = new TFile(folder + "WWTo2L2Nu_" + gen + "_nnllQdownHtPlus.root","read");  
  }

  float entries;
  if (gen == "mad") entries = 1933235;
  if (gen == "pow") entries = 999864;
  if (gen == "mcnlo") entries = 539594;

  //computing efficiencies
  h = (TH1F*) mad -> Get(name);
  if(choose == 3){
    cout<<name<<": "<<h->GetEntries()<<endl;
    TString name2 = name;
    name2.Replace(name.Length()-1,1,"2"); 
    h -> Add((TH1F*) mad -> Get(name2));
    cout<<name2<<": "<<h->GetEntries()<<endl;
  }
  //h->Rebin(10);
  //hh = (TH1F*) mad -> Get("h_n_PV");
  //h -> Divide(hh);
  Float_t EffMad = h -> Integral() / entries;//hh->GetEntries();// * hh->Integral();
  cout<< gen + " efficiency = "<<EffMad<<endl;
  inFile<< gen + " efficiency = "<<EffMad<<endl;

  h = (TH1F*) madRup -> Get(name);
  if(choose == 3){
    cout<<name<<": "<<h->GetEntries()<<endl;
    TString name2 = name;
    name2.Replace(name.Length()-1,1,"2"); 
    h -> Add((TH1F*) mad -> Get(name2));
    cout<<name2<<": "<<h->GetEntries()<<endl;
  }
  //h->Rebin(10);
  //hh = (TH1F*) madRup -> Get("h_n_PV");
  //h -> Divide(hh);
  Float_t EffMadRup = h -> Integral() / entries;//hh->GetEntries();// * hh->Integral();
  cout<<gen + " efficiency with enhanced R scale = "<<EffMadRup<<endl;
  inFile<<gen + " efficiency with Enhanced R scale = "<<EffMadRup<<endl;

  h = (TH1F*) madRdown -> Get(name);
  if(choose == 3){
    cout<<name<<": "<<h->GetEntries()<<endl;
    TString name2 = name;
    name2.Replace(name.Length()-1,1,"2"); 
    h -> Add((TH1F*) mad -> Get(name2));
    cout<<name2<<": "<<h->GetEntries()<<endl;
  }
  //h->Rebin(10);
  //hh = (TH1F*) madRdown -> Get("h_n_PV");
  //h -> Divide(hh);
  Float_t EffMadRdown = h -> Integral() / entries;//hh->GetEntries() * hh->Integral();
  cout<<gen + " efficiency with Reduced R scale = "<<EffMadRdown<<endl;
  inFile<<gen + " efficiency with Reduced R scale = "<<EffMadRdown<<endl;

  Float_t errMadR = 0.5 * (EffMadRup - EffMadRdown);
  //min(fabs(EffMadRup - EffMad),fabs(EffMadRdown - EffMad));

  h = (TH1F*) madQup -> Get(name);
  if(choose == 3){
    cout<<name<<": "<<h->GetEntries()<<endl;
    TString name2 = name;
    name2.Replace(name.Length()-1,1,"2"); 
    h -> Add((TH1F*) mad -> Get(name2));
    cout<<name2<<": "<<h->GetEntries()<<endl;
  }
  //h->Rebin(10);
  //hh = (TH1F*) madQup -> Get("h_n_PV");
  //h -> Divide(hh);
  Float_t EffMadQup = h -> Integral() / entries;//hh->GetEntries();// * hh->Integral();
  cout<<"Madgraph Efficiency With Enhanced Q scale = "<<EffMadQup<<endl;
  inFile<<gen + " efficiency with Enhanced Q scale = "<<EffMadQup<<endl;

  h = (TH1F*) madQdown -> Get(name);
  if(choose == 3){
    cout<<name<<": "<<h->GetEntries()<<endl;
    TString name2 = name;
    name2.Replace(name.Length()-1,1,"2"); 
    h -> Add((TH1F*) mad -> Get(name2));
    cout<<name2<<": "<<h->GetEntries()<<endl;
  }
  //h->Rebin(10);
  //hh = (TH1F*) madQdown -> Get("h_n_PV");
  //h -> Divide(hh);
  Float_t EffMadQdown = h -> Integral() / entries;//hh->GetEntries() * hh->Integral();
  cout<<gen + " efficiency with Reduced Q scale = "<<EffMadQdown<<endl;
  inFile<<gen + " efficiency with Reduced Q scale = "<<EffMadQdown<<endl;

  Float_t errMadQ = 0.5 * (EffMadQup - EffMadQdown);
  //min(fabs(EffMadQup - EffMad),fabs(EffMadQdown - EffMad));

  Float_t errMadUp = sqrt((EffMadQdown - EffMad)**2 + (EffMadRup - EffMad)**2);
  Float_t errMadDown = sqrt((EffMadQup - EffMad)**2 + (EffMadRdown - EffMad)**2);

  Float_t errEffMad = sqrt(errMadQ * errMadQ + errMadR * errMadR);
  cout<<gen + " efficiency Error = "<<errEffMad<<endl;

  cout<<gen + "Efficiency = "<<EffMad*100<<"% +- "<<errEffMad*100<<"%"<<endl;//+ "<<errMadUp*100<<"% - "<<errMadDown*100<<"%"<<endl;
  inFile<<gen + "Efficiency = "<<EffMad*100<<"% +- "<<errEffMad*100<<"%"<<endl;//+ "<<errMadUp*100<<"% - "<<errMadDown*100<<"%"<<endl;

  Float_t errEffPerc = 100. * errEffMad / EffMad;
  cout<<gen + " efficiency Percentage Error = "<<errEffPerc<<endl;
  inFile<<gen + " efficiency Percentage Error = "<<errEffPerc<<endl;

  /*
  cout<<gen + " efficiency Percentage Error Up = "<<100 * errMadUp / EffMad<<endl;
  cout<<gen + " efficiency Percentage Error Down = "<<100 * errMadDown / EffMad<<endl;

  inFile<<gen + " efficiency Percentage Error Up = "<<100 * errMadUp / EffMad<<endl;
  inFile<<gen + " efficiency Percentage Error Down = "<<100 * errMadDown / EffMad<<endl;
  */
  cout<<"\n"<<endl;

  if (choose == 2){
    if(name.Contains("Ht")){
    upHt->SetPoint(num, 1 + num * 0.02, EffMad);
    upHt->SetPointError(num, 0., 0., 5*fabs(errEffMad), 5*fabs(errEffMad));//5*fabs(errMadQ), 5*fabs(errMadQ), 5*fabs(errMadR), 5*fabs(errMadR));
    }
  }
  else if (choose == 1 || choose == 3){ 
    if(name.Contains("Plus")){
    upHt->SetPoint(num, 1 + num * 0.02, EffMad);
    upHt->SetPointError(num, 0., 0., 5*fabs(errEffMad), 5*fabs(errEffMad));//5*fabs(errMadQ), 5*fabs(errMadQ), 5*fabs(errMadR), 5*fabs(errMadR));
    }
    else if (name.Contains("Ht")){
    noHt->SetPoint(num, 1 + num * 0.02, EffMad);
    noHt->SetPointError(num, 0., 0., 5*fabs(errEffMad), 5*fabs(errEffMad));//5*fabs(errMadQ), 5*fabs(errMadQ), 5*fabs(errMadR), 5*fabs(errMadR));
  }
  else{
    withHt->SetPoint(num, 1 + num * 0.02, EffMad);
    withHt->SetPointError(num, 0., 0., 5*fabs(errEffMad), 5*fabs(errEffMad));//5*fabs(errMadQ), 5*fabs(errMadQ), 5*fabs(errMadR), 5*fabs(errMadR));
  }
  cout<<fabs(EffMadQup - EffMad)<<endl;
  }
}

void computeEff(TString text = "Inclusive"){

  if (text == "Inclusive"){
    inFile<<"NO HT SELECTIONS"<<endl;
    compute("hEffNoHt3","mad",0,1);
    compute("hEffNoHt3","pow",1,1);
    compute("hEffNoHt3","mcnlo",2,1);
    
    inFile<<"USING HT SELECTIONS"<<endl;
    compute("hEff3","mad",0,1);
    compute("hEff3","pow",1,1);
    compute("hEff3","mcnlo",2,1);

    inFile<<"USING REVERSE HT SELECTIONS"<<endl;
    compute("hEffHtPlus3","mad",0,1);
    compute("hEffHtPlus3","pow",1,1);
    compute("hEffHtPlus3","mcnlo",2,1);
  }

  else if (text == "0-Jet"){
    inFile<<"NO HT SELECTIONS"<<endl;
    compute("hEffNoHt0","mad",0,1);
    compute("hEffNoHt0","pow",1,1);
    compute("hEffNoHt0","mcnlo",2,1);
    
    inFile<<"USING HT SELECTIONS"<<endl;
    compute("hEff0","mad",0,1);
    compute("hEff0","pow",1,1);
    compute("hEff0","mcnlo",2,1);

    inFile<<"USING REVERSE HT SELECTIONS"<<endl;
    compute("hEffHtPlus0","mad",0,1);
    compute("hEffHtPlus0","pow",1,1);
    compute("hEffHtPlus0","mcnlo",2,1);
  }

  else if (text == "1+Jet"){
    inFile<<"NO HT SELECTIONS"<<endl;
    compute("hEffNoHt1","mad",0,3);
    compute("hEffNoHt1","pow",1,3);
    compute("hEffNoHt1","mcnlo",2,3); 

    inFile<<"USING HT SELECTIONS"<<endl;
    compute("hEff1","mad",0,3);
    compute("hEff1","pow",1,3);
    compute("hEff1","mcnlo",2,3);

    inFile<<"USING REVERSE HT SELECTIONS"<<endl;
    compute("hEffHtPlus1","mad",0,3);
    compute("hEffHtPlus1","pow",1,3);
    compute("hEffHtPlus1","mcnlo",2,3);
 }

  //Graphics
  TCanvas *c1 = new TCanvas("c1","c1",600.,1400.);
  c1->cd();

  TPad *p1 = new TPad("p1","p1",0.,0.,1.,1.);
  p1->SetLeftMargin(0.18);
  p1->SetBottomMargin(0.14);
  p1->Draw();
  p1->cd();

  withHt -> SetTitle("Signal Efficiency");
  //if (text == "Inclusive")
  withHt -> GetYaxis() -> SetRangeUser(0.0,0.066);
  //else if (text == "0-Jet")
  withHt -> GetYaxis() -> SetRangeUser(0.0,0.066);
  withHt -> GetXaxis() -> SetRangeUser(0.,4.);
  withHt -> SetMarkerStyle(20);
  withHt -> SetMarkerColor(kRed);
  withHt -> SetMarkerSize(1.2);
  withHt -> GetXaxis() -> SetTitleSize(0.045);
  withHt -> GetXaxis() -> SetTitleOffset(1.5);
  //  withHt -> GetXaxis() -> SetTitle("1 - Madgraph; 2 - Powheg; 3 - MC@NLO");
  withHt -> GetYaxis() -> SetTitleSize(0.05);
  withHt -> GetYaxis() -> SetTitleOffset(1.8);
  withHt -> GetYaxis() -> SetTitle("Efficiency");
  withHt -> GetYaxis() -> SetLabelSize(0.06);
  withHt -> GetXaxis() -> SetLabelSize(0.08);
  withHt -> GetXaxis() -> SetLabelOffset(0.02);

  withHt -> GetXaxis() -> SetBinLabel(withHt -> GetXaxis() -> FindBin(1.000), "Madgraph");
  withHt -> GetXaxis() -> SetBinLabel(withHt -> GetXaxis() -> FindBin(1.020), "Powheg");
  withHt -> GetXaxis() -> SetBinLabel(withHt -> GetXaxis() -> FindBin(1.040), "MC@NLO");
  withHt -> GetXaxis() -> CenterLabels();
  withHt -> GetXaxis() -> LabelsOption("h");

  TLatex* tl = new TLatex(0.85, 0.75, text);
  tl->SetNDC();
  tl->SetTextAlign(   32);
  tl->SetTextFont (   42);
  tl->SetTextSize (0.06);

  TLatex* tl2 = new TLatex(0.85, 0.7, "Error x 5");
  tl2->SetNDC();
  tl2->SetTextAlign(   32);
  tl2->SetTextFont (   42);
  tl2->SetTextSize (0.05);
  
  TLatex* tl3 = new TLatex(0.85, 0.67, "X axis Error: Q scale x 5");
  tl3->SetNDC();
  tl3->SetTextAlign(   32);
  tl3->SetTextFont (   42);
  tl3->SetTextSize (0.03);

  noHt -> SetMarkerStyle(20);
  noHt -> SetMarkerColor(kBlue);
  noHt -> SetMarkerSize(1.2);

  upHt -> SetMarkerStyle(20);
  upHt -> SetMarkerColor(kViolet);
  upHt -> SetMarkerSize(1.2);
  
  p1 -> Update();

  TAxis *taxix = withHt -> GetXaxis();
  TAxis *taxiy = withHt -> GetYaxis();

  //  TGaxis *axis = new TGaxis(p1->GetUxmin(),p1->GetUymin(),p1->GetUxmax(),p1->GetUymin(),0,2,510,"+L");
  TGaxis *axis = new TGaxis(0.995/*taxix->GetXmin()*/,0./*taxiy->GetXmin()*/,1.045/*taxix->GetXmax()*/,0./*taxiy->GetXmin()*/,0.995,1.045,408,"+L");
  axis->SetLabelSize(withHt->GetYaxis()->GetLabelSize());
  axis->SetLabelFont(withHt->GetYaxis()->GetLabelFont());
  axis->SetTitleSize(withHt->GetYaxis()->GetTitleSize());
  axis->SetTitleFont(withHt->GetYaxis()->GetTitleFont());
  axis->SetTitleOffset(0.);
  //  axis->SetTitle("Background Rejection");
  
  withHt -> Draw("ap");
  noHt   -> Draw("psame");
  upHt   -> Draw("psame");
  //axis   -> Draw("same");

  TLegend* leg = new TLegend(0.20,0.79,0.60,0.89);
  leg->AddEntry(withHt,"With Ht Selection","p");
  leg->AddEntry(noHt,"Without Ht Selection","p");
  leg->AddEntry(upHt,"With Reverse Ht Selection","p");
  leg->SetTextSize(0.05);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->Draw();
  tl->Draw("same");
  tl2->Draw("same");
  //tl3->Draw("same");
  c1->Print(text +".pdf","pdf");

  inFile.close();
}
