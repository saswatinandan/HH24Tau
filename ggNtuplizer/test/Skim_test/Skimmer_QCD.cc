#define Skimmer_cxx
#include "Skimmer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <TMath.h>
#include <string>
#include <sstream>
using namespace std;


void Skimmer::Loop(TString outputName, bool Muon, int skm)
{
    
    
  TH1F* hEvents = (TH1F*)gDirectory->Get("ggNtuplizer/hEvents");
  //  cout << "hEvents = " << hEvents->GetBinContent(1);
  //    TH1F* hPU     = (TH1F*)gDirectory->Get("ggNtuplizer/hPU");
  //    TH1F* hPUTrue = (TH1F*)gDirectory->Get("ggNtuplizer/hPUTrue");
  //TH1F* hEvents = (TH1F*)gDirectory->Get("hEvents");
  //TH1F* hPU     = (TH1F*)gDirectory->Get("hPU");
  //TH1F* hPUTrue = (TH1F*)gDirectory->Get("hPUTrue");
    
  TFile* file = TFile::Open(outputName, "RECREATE");

  TTree* MyNewTree = fChain->CloneTree(0);
    
  fChain->SetBranchStatus("*",0);
  fChain->SetBranchStatus("hasGoodVtx",1);
  fChain->SetBranchStatus("vt*",1);
  fChain->SetBranchStatus("EventTag",1);
  fChain->SetBranchStatus("run",1);
  fChain->SetBranchStatus("event",1);
  fChain->SetBranchStatus("lumis",1);
  fChain->SetBranchStatus("isData",1);
  fChain->SetBranchStatus("HLT*",1);
  fChain->SetBranchStatus("gen*",1);
  fChain->SetBranchStatus("pdf",1);
  fChain->SetBranchStatus("pthat",1);
  fChain->SetBranchStatus("processID",1);
  fChain->SetBranchStatus("rho*",1);
  fChain->SetBranchStatus("pu*",1);
  fChain->SetBranchStatus("mc*",1);
  fChain->SetBranchStatus("pfMET*",1);
  fChain->SetBranchStatus("n*",1);
  fChain->SetBranchStatus("c*",1);
  fChain->SetBranchStatus("jet*",1);
  fChain->SetBranchStatus("AK8*",0);
  fChain->SetBranchStatus("ele*",1);
  fChain->SetBranchStatus("mu*",1);
  fChain->SetBranchStatus("pho",0);
  fChain->SetBranchStatus("tau*",1);
  fChain->SetBranchStatus("m*",1); 
    
  TH1F* hcount = new TH1F("hcount", "", 10, 1, 10);
  //  TH1F* h_mass = new TH1F("mass","",20,70,110);
  if (fChain == 0) return;
    
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  
  for (int jentry=0; jentry<nentries;jentry++) {
        
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
        
        
    if(jentry % 10000 == 0) cout << "Processed " << jentry << " events out of " <<nentries<<endl;

    hcount->Fill(1);
    hcount->Fill(2,genWeight);

    int z(0), ntau(0), ntaumu(0), mucharge(0), ntauele(0);
    //    cout << "nMC=== " << nMC << endl;
    /*    for(int imc=0; imc<nMC; imc++) {
      //      if(mcPID->at(imc) ==25 && mcMass->at(imc) >80 && mcMass->at(imc) <100) z+=1;
      if(mcPID->at(imc) ==25) z+=1;
      if(fabs(mcPID->at(imc)) ==16 && fabs(mcMomPID->at(imc)) ==15 && mcGMomPID->at(imc) ==25) ntau +=1;
      if(fabs(mcPID->at(imc)) ==12 && fabs(mcMomPID->at(imc)) ==15 && mcGMomPID->at(imc) ==25) ntauele +=1;
      if(fabs(mcPID->at(imc)) ==14 && fabs(mcMomPID->at(imc)) ==15 && mcGMomPID->at(imc) ==25) {
	ntaumu +=1;
	int charge = (mcMomPID->at(imc)==15) ? 1 : -1;
	mucharge +=charge;
      }
    }
    //    cout << "z== " << z << "ntau=== " << ntau << endl;
    if(z==2) hcount->Fill(3);
    if(z==2 && ntau==4) hcount->Fill(4);
    if(z==2 && ntau==4 && ntaumu ==2 && ntauele==0) hcount->Fill(5);
    if(z==2 && ntau==4 && ntaumu ==2 && ntauele==0 && mucharge) hcount->Fill(6);*/

        
    bool firstPart = true;
    int isMuEle(0);
    //if(nMu<2)
    //    cout << "nMu== " << nMu << endl;
    if(Muon) {
      for (int imu = 0; imu < nMu; ++imu) {
	
	//	bool mupt = (firstPart) ? (muPt->at(imu) >18) : (muPt->at(imu) >9);
	bool mupt =  (muPt->at(imu) >15); /// only for QCD
	UShort_t id = (muIDbit->at(imu) >> 1 & 1);
	
	float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
	if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
	  IsoMu= ( muPFChIso->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
	
	//	if(mupt && id && fabs(muEta->at(imu)) <2.4 && IsoMu <0.3) {
	if(mupt && id && fabs(muEta->at(imu)) <2.4) { ////////////////// only for QCD MC
	  firstPart = false;
	  isMuEle +=1;
	}
	//	if(isMuEle ==2) break;
	if(isMuEle ==1) break; //// only for QCD
      }
      
    }
    else {

      for (int iele = 0; iele < nEle; ++iele) {
	
        if(iele == 0 && elePt->at(iele) <25) break;
	
        bool elept = (firstPart) ? (elePt->at(iele) >18) : (elePt->at(iele) >15);
	
	float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
	if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
	  IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);
	
	bool eleMVAId= false;
	if (fabs (eleSCEta->at(iele)) < 0.8 && eleIDMVA->at(iele) > 0.967083) eleMVAId= true;
	else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <  1.5 && eleIDMVA->at(iele) > 0.929117) eleMVAId= true;
	else if ( fabs (eleSCEta->at(iele)) >  1.5 && eleIDMVA->at(iele) > 0.726311 ) eleMVAId= true;
	else eleMVAId= false;

        if(elept && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle <0.1) {
          firstPart = false;
          isMuEle +=1;
        }
        if(isMuEle ==2) break;
      }
      
    }

    //    if(isMuEle!=2) continue;
    if(isMuEle!=1) continue; // only for QCD
    vector<int> vec_tau;

    for  (int itau=0 ; itau < nTau; itau++) {
      
      bool antielemu = tauByMVA6LooseElectronRejection->at(itau) !=0 && tauByTightMuonRejection3->at(itau) !=0;
      
      if(tauPt->at(itau) > 10 && fabs(tauEta->at(itau)) < 2.3 && antielemu && taupfTausDiscriminationByDecayModeFinding->at(itau) !=0)
	vec_tau.push_back(itau);
      if(vec_tau.size() ==2) break;
    }

    if(vec_tau.size() !=2) continue;
    hcount->Fill(7);
    MyNewTree->Fill();
  }
    
  MyNewTree->AutoSave();
  cout << "hEvents->Write();" << endl;
  hEvents->Write();
  hcount->Write();
  file->Close();
}

int main(int argc, char* argv[]){
    
  string FinaName=argv[1];
  stringstream ss(FinaName);
  bool Muon = true;
    
  string token;
  string M;
  int count=0;
  string realName;
  while (getline(ss,token, '/'))
    {
      count++;
      cout<< token <<endl;
      if (count == 5) {
	cout<<"   ----->    5   "<<token<<"  _____   \n";
	realName=token;
      }
      M=token;
    }
    
  TString outputName = "skimed_"+realName+M;
  cout<<" outputName is ---> "<<outputName<<"\n";
    
  Skimmer t("root://cmseos.fnal.gov//store/user/snandan/Moriond17"+FinaName);
  cout << "outname== " << "root://cmsxrootd.fnal.gov//store/user/snandan/Moriond17" << FinaName << endl;
  //FinaName.erase(FinaName.begin(),FinaName.end()-10);
  //  Skimmer t("root://cmsxrootd.fnal.gov//"+FinaName);
  t.Loop(outputName, Muon,0);
  return 0;
}

