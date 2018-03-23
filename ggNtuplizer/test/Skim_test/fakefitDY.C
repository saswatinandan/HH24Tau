#include <TFile.h>
#include <TROOT.h>
#include <TPaveText.h>
#include <iostream>
#include <iomanip>
#include <TH1F.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1D.h>
#include <THStack.h>
#include <TLegend.h>
#include <TStyle.h>
using namespace std;

void fakefit_pt() {  

  std::string file[10]= {"DYFakeinData19.root","DYFakein_DYtest.root","DYFakeinTT.root","DYFakein_STtest.root","DYFakeinWJetsToLNu.root","DYFakeinWW.root", "DYFakeinWZ.root","DYFakeinZZ.root","DYFakeinzzTo4L.root","DYFakeinZH.root"};

  TFile* f = new TFile("fakecombine.root","update");
  TFile* file1 = TFile::Open(file[0].c_str());
  TH1F* deno = (TH1F*) file1->Get("deno");
  TH1F* neo = (TH1F*) file1->Get("neo");

  for(int i=2; i<10; i++) {

    TFile* file_ = TFile::Open(file[i].c_str());
    TH1F* deno_ = (TH1F*) file_->Get("deno");
    TH1F* neo_ = (TH1F*) file_->Get("neo");
    deno->Add(deno_,-1);
    neo->Add(neo_,-1);
  }
  THStack *hs = new THStack("hs","Stacked 1D histograms");
  TFile* file_ = TFile::Open(file[1].c_str());
  TH1F* deno_ = (TH1F*) file_->Get("deno");
  TH1F* neo_ = (TH1F*) file_->Get("neo");
  
  deno_->SetFillColor(6);
  deno_->SetMarkerColor(6);
  deno->SetMarkerStyle(21);
  deno->SetMarkerSize(0.9);
  deno->SetMarkerColor(kBlack);
  hs->Add(deno_);
  hs->Draw("hist H");
  deno->SetMarkerStyle(8);
  deno->SetMarkerSize(0.9);
  deno->SetMarkerColor(kBlack);
  deno->Draw("same PE");
  /*  TFile* file1 = TFile::Open("DYFakeinFor_DY.root");
  TH1F* deno = (TH1F*) file1->Get("deno");
  TH1F* neo = (TH1F*) file1->Get("neo"); */
  neo->Divide(deno);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1111);
  //  TH1F* ffd0  = dynamic_cast<TH1F*>(file1->Get("effi"));
  //  gStyle->SetOptFit(00011);

  double xlow = neo-> GetXaxis()->GetXmin();
  double xhigh = neo-> GetXaxis()->GetXmax();
  int nx = neo->GetNbinsX();

   
  //   TH1D* ffd0 = new TH1D("ffd0", "JetToTau Fake function in DYJets Central Eta Region", nx, xlow, xhigh);

  float lowX=0.21;
  float lowY=0.70;

  TPaveText lumi(lowX, lowY+0.06, lowX+0.15, lowY+0.16, "NDC");
  lumi.SetTextFont(61);
  lumi.SetTextSize(0.08);
  lumi.SetBorderSize(   0 );
  lumi.SetFillStyle(    0 );
  lumi.SetTextAlign(   12 );
  lumi.SetTextColor(    1 );
  lumi.AddText("CMS Preliminary");



   TCanvas *can1 = new TCanvas("can1", "JetToTau Fake function", 900, 700);
   neo->SetMarkerStyle(20);
   neo->SetMarkerSize(0.7);
   neo->GetXaxis()->SetTitle("#tau pt");
   neo->SetTitle("");
   neo->SetName("Fake Rate");
   neo->GetYaxis()->SetTitle("Fake Rate (%)");
   neo-> Draw();
   //   neo->SetLineColor(kRed);
   f->cd();
   TH1F* h =(TH1F*)neo->Clone("mm");
   h->Write();
   f->Write();
    TF1 *func0 = new TF1("func0","[2]+[0]*TMath::Exp([1]*x)",20,150); /// DYFakein_9thjune2Data19.root
    //   TF1 *func0 = new TF1("func0","TMath::Exp([0]*x)",0,150); //"DYFakeintwojet_Data19.root"
   //          func0->SetParameters(0,10);
   //func0->SetParameters(1,0.009);
     func0->SetParameters(0,.011); // DYFakein_9thjune2Data19.root                                
   func0->SetParameters(1,-0.02);// DYFakein_9thjune2Data19.root
   neo-> Fit("func0","","", 20, 150); 
   lumi.Draw();
   cout << func0->GetChisquare() << "\t" << func0->GetNDF() << "\t" << func0->GetProb() << endl;
   can1->Print("fakeDY_ME.png");
   can1->Print("fakeDY_ME.pdf");
}
