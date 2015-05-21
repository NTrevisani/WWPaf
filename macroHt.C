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
#include "TKey.h"
#include "TObject.h"
#include "TStyle.h"

TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

int macroHt(TString option){

  TFile *resummed[3];
  TFile *plain[3];
  TH1F* hres[3];
  TH1F* hpla[3]; 

  TString name[3];
  name[0] = "WWTo2L2Nu_pow_nnll";
  name[1] = "WWTo2L2Nu_mad_nnll";
  name[2] = "WWTo2L2Nu_mcnlo_nnll";

  TCanvas *c1[3];

  tdrStyle->SetTitleFontSize(0.12);
  tdrStyle->SetTitleH(1); // Set the height of the title box
  tdrStyle->SetTitleW(1); // Set the width of the title box

  TPad* pad1[3]; 

  TCanvas *c2 = new TCanvas("c2","c2",600.,600.);
  TCanvas *c3 = new TCanvas("c3","c3",600.,600.);

  for (int i = 0; i < 3; ++i){
  resummed[i] = new TFile("rootFiles/AllJet/OF/" + name[i] + ".root");
  plain[i] = new TFile(name[i] + ".root");

  hres[i] = (TH1F*)resummed[i]->Get("hHt3");
  hpla[i] = (TH1F*)plain[i]->Get("hHt3");

  hres[i]->Rebin(10);  
  hpla[i]->Rebin(10);  

  hres[i]->GetXaxis()->SetRangeUser(0.,600.);
  hpla[i]->GetXaxis()->SetRangeUser(0.,600.);

  hres[i]->GetYaxis()->SetRangeUser(0.,500.);
  hpla[i]->GetYaxis()->SetRangeUser(0.,500.);

  c1[i] = new TCanvas("c1","c1",600,600);
  c1[i]->cd();

  pad1[i] = new TPad("pad1", "pad1", 0., 0., 1.0, 1.0);
  pad1[i] ->SetLeftMargin(0.14);
  pad1[i] ->SetBottomMargin(0.18);
  pad1[i] ->Draw();
  pad1[i] ->cd();

  hres[i]->SetTitle("Ht without B-Tagging");
  
  hres[i]->GetXaxis()->SetTitle("Energy [GeV]");
  
  hres[i]->GetXaxis()->SetTitleSize(0.05);
  hres[i]->GetXaxis()->SetTitleOffset(0.9);
  hres[i]->GetXaxis()->SetLabelSize(0.05);
  hres[i]->GetYaxis()->SetTitleSize(0.05);
  hres[i]->GetYaxis()->SetLabelSize(0.05);
  hres[i]->SetLineWidth(2);
  hpla[i]->SetLineWidth(2);
  hpla[i]->SetStats(0);
  hres[i]->SetStats(0);
  hres[i]->SetLineColor(kBlue);
  hpla[i]->SetLineColor(kRed);

  hres[i]->Draw("");
  hpla[i]->Draw("same");

  TLegend* leg = new TLegend(0.40,0.79,0.60,0.89);
  leg->AddEntry(hpla[i],"Without Resummation","l");
  leg->AddEntry(hres[i],"With Resummation","l");
  leg->SetTextSize(0.05);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->Draw();

  c1[i]->Print(name[i] + ".pdf","pdf");

  delete leg;

  }

  //delete hpla[i];
  //delete hres[i];
  //delete c1;
}
