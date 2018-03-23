//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Compiling the code:   ./Make.sh ZTT_XSection.cc
//   Running the code:     ./ZTT_XSection.exe OutPut.root   Input.root
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "TreeReader.h"
#include "WeightCalculator.h"
#include "TLorentzVector.h"
#include <string> //remove space for successful compilation
#include <ostream> //remove space for successful compilation
#include <iostream>
#include <iomanip>
#include <sstream>

void TT_fake(TH1F* h_deno, TH1F* h_neu, TTree* Run_Tree, bool Muon, std::string input, TH1F* HistoTot);
void DY_fake(TH1F* h_deno, TH1F* h_neu, TTree* Run_Tree, bool Muon, std::string input, TH1F* HistoTot);
void Wjets_fake(TH1F* h_deno, TH1F* h_neu, TTree* Run_Tree, bool Muon, std::string input, TH1F* HistoTot);
void QCD_fake(TH1F* h_deno, TH1F* h_neu, TTree* Run_Tree, bool Muon, std::string input, TH1F* HistoTot ) ;
void QCD_muonfake(TH1F* h_deno, TH1F* h_neu, TTree* Run_Tree, bool Muon, std::string input, TH1F* HistoTot ) ;

TFile * HZZ= TFile::Open("ScaleFactors_mu_Moriond2017_v2.root");
TH2F* SF = (TH2F*) HZZ->Get("FINAL");

vector <float> W_events = W_EvenetMultiplicity();
vector <float> DY_events = DY_EvenetMultiplicity();


int  main(int argc, char** argv) {
  using namespace std;

  std::vector<string> input;
  for (int f = 1; f < argc; f++) {
    input.push_back(*(argv + f));

    cout << "\n INPUT NAME IS:   " << input[f - 1] << "\n";
  }

  bool Muon = (input[0]=="Muon") ? true : false;
  

  for(int k=2; k<input.size(); k++ ) {

    TFile *f_Double = TFile::Open(input[k].c_str());
    cout << "\n  Now is running on ------->   " << std::string(f_Double->GetName()) << "\n";

    TFile * myFile = TFile::Open(f_Double->GetName());
    TH1F * HistoTot = (TH1F*) myFile->Get("hcount");

    TTree *Run_Tree = (TTree*) f_Double->Get("EventTree");    
    
    /////////////////////////   General Info
    Run_Tree->SetBranchAddress("isData", &isData);
    Run_Tree->SetBranchAddress("run", &run);
    Run_Tree->SetBranchAddress("lumis", &lumis);
    Run_Tree->SetBranchAddress("event", &event);
    Run_Tree->SetBranchAddress("genWeight",&genWeight);
    Run_Tree->SetBranchAddress("HLTEleMuX", &HLTEleMuX);
    Run_Tree->SetBranchAddress("HLTEleMuXIsPrescaled", &HLTEleMuXIsPrescaled);
    Run_Tree->SetBranchAddress("puTrue", &puTrue);        
    
    
    /////////////////////////   Tau Info
    Run_Tree->SetBranchAddress("nTau", &nTau);
    Run_Tree->SetBranchAddress("tauPt"  ,&tauPt);
    Run_Tree->SetBranchAddress("tauEta"  ,&tauEta);
    Run_Tree->SetBranchAddress("tauPhi"  ,&tauPhi);
    Run_Tree->SetBranchAddress("tauMass"  ,&tauMass);
    Run_Tree->SetBranchAddress("tauCharge"  ,&tauCharge);
    Run_Tree->SetBranchAddress("taupfTausDiscriminationByDecayModeFinding", &taupfTausDiscriminationByDecayModeFinding);
    Run_Tree->SetBranchAddress("tauByTightMuonRejection3", &tauByTightMuonRejection3);
    Run_Tree->SetBranchAddress("tauByLooseMuonRejection3", &tauByLooseMuonRejection3);
    Run_Tree->SetBranchAddress("tauByMVA6TightElectronRejection"  ,&tauByMVA6TightElectronRejection);
    Run_Tree->SetBranchAddress("tauByLooseCombinedIsolationDeltaBetaCorr3Hits",&tauByLooseCombinedIsolationDeltaBetaCorr3Hits);
    Run_Tree->SetBranchAddress("tauByMediumCombinedIsolationDeltaBetaCorr3Hits",&tauByMediumCombinedIsolationDeltaBetaCorr3Hits);
    Run_Tree->SetBranchAddress("tauByMVA6LooseElectronRejection", &tauByMVA6LooseElectronRejection);
    Run_Tree->SetBranchAddress("tauDxy",&tauDxy);
    Run_Tree->SetBranchAddress("tauByMediumIsolationMVArun2v1DBoldDMwLT", &tauByMediumIsolationMVArun2v1DBoldDMwLT);
    Run_Tree->SetBranchAddress("tauByLooseIsolationMVArun2v1DBoldDMwLT", &tauByLooseIsolationMVArun2v1DBoldDMwLT);
    Run_Tree->SetBranchAddress("tauByVLooseIsolationMVArun2v1DBoldDMwLT", &tauByVLooseIsolationMVArun2v1DBoldDMwLT);

    /////////////////////////   Mu Info
    Run_Tree->SetBranchAddress("nMu", &nMu);
    Run_Tree->SetBranchAddress("eleEn", &eleEn);
    Run_Tree->SetBranchAddress("muEn", &muEn);
    Run_Tree->SetBranchAddress("tauEnergy", &tauEnergy);
    Run_Tree->SetBranchAddress("muPt"  ,&muPt);
    Run_Tree->SetBranchAddress("muEta"  ,&muEta);
    Run_Tree->SetBranchAddress("muPhi"  ,&muPhi);
    Run_Tree->SetBranchAddress("muIsoTrk", &muIsoTrk);
    Run_Tree->SetBranchAddress("muCharge",&muCharge);
    //  Run_Tree->SetBranchAddress("muIsMediumID",&muIsMediumID);
    //Run_Tree->SetBranchAddress("muIsLooseID",&muIsLooseID);
    Run_Tree->SetBranchAddress("muPFChIso", &muPFChIso);
    Run_Tree->SetBranchAddress("muPFPhoIso", &muPFPhoIso);
    Run_Tree->SetBranchAddress("muPFNeuIso", &muPFNeuIso);
    Run_Tree->SetBranchAddress("muPFPUIso", &muPFPUIso);
    Run_Tree->SetBranchAddress("muD0",&muD0);
    Run_Tree->SetBranchAddress("muDz",&muDz);
    Run_Tree->SetBranchAddress("muIDbit", &muIDbit);
    Run_Tree->SetBranchAddress("muFiredTrgs", &muFiredTrgs);
    
    /////////////////////////   Ele Info
    Run_Tree->SetBranchAddress("nEle", &nEle);
    Run_Tree->SetBranchAddress("elePt"  ,&elePt);
    Run_Tree->SetBranchAddress("eleEta"  ,&eleEta);
    Run_Tree->SetBranchAddress("elePhi"  ,&elePhi);
    Run_Tree->SetBranchAddress("elePFChIso", &elePFChIso);
    Run_Tree->SetBranchAddress("eleIDMVA", &eleIDMVA);
    Run_Tree->SetBranchAddress("eleCharge",&eleCharge);
    Run_Tree->SetBranchAddress("eleSCEta",&eleSCEta);
    Run_Tree->SetBranchAddress("elePFChIso", &elePFChIso);
    Run_Tree->SetBranchAddress("elePFPhoIso", &elePFPhoIso);
    Run_Tree->SetBranchAddress("elePFNeuIso", &elePFNeuIso);
    Run_Tree->SetBranchAddress("elePFPUIso", &elePFPUIso);
    Run_Tree->SetBranchAddress("eleIDMVANonTrg", &eleIDMVANonTrg);
    Run_Tree->SetBranchAddress("eleD0",&eleD0);
    Run_Tree->SetBranchAddress("eleDz",&eleDz);
    Run_Tree->SetBranchAddress("eleMissHits", &eleMissHits);
    Run_Tree->SetBranchAddress("eleConvVeto", &eleConvVeto);
    Run_Tree->SetBranchAddress("eleChargeConsistent", &eleChargeConsistent);
    Run_Tree->SetBranchAddress("ele2ndChargeConsistent", &ele2ndChargeConsistent);
    /////////////////////////   Jet Info
    Run_Tree->SetBranchAddress("nJet",&nJet);
    Run_Tree->SetBranchAddress("jetPt",&jetPt);
    Run_Tree->SetBranchAddress("jetEta",&jetEta);
    Run_Tree->SetBranchAddress("jetPhi",&jetPhi);
    Run_Tree->SetBranchAddress("jetEn",&jetEn);
    //  Run_Tree->SetBranchAddress("jetpfCombinedInclusiveSecondaryVertexV2BJetTags",&jetpfCombinedInclusiveSecondaryVertexV2BJetTags);
    Run_Tree->SetBranchAddress("jetCSV2BJetTags",&jetCSV2BJetTags);
    Run_Tree->SetBranchAddress("mcMomPID" ,&mcMomPID);
    Run_Tree->SetBranchAddress("mcGMomPID" ,&mcGMomPID);
    Run_Tree->SetBranchAddress("nMC" ,&nMC);
    Run_Tree->SetBranchAddress("mcPID", &mcPID);
    Run_Tree->SetBranchAddress("mcPt", &mcPt);
    Run_Tree->SetBranchAddress("mcEta", &mcEta);
    Run_Tree->SetBranchAddress("mcPhi", &mcPhi);
    Run_Tree->SetBranchAddress("mcE", &mcE);
    Run_Tree->SetBranchAddress("mcMomPt", &mcMomPt);
    Run_Tree->SetBranchAddress("mcMomEta", &mcMomEta);
    Run_Tree->SetBranchAddress("mcMomPhi", &mcMomPhi);
    Run_Tree->SetBranchAddress("mcMomMass", &mcMomMass);
    Run_Tree->SetBranchAddress("mcHadronPt", &mcHadronPt);
    Run_Tree->SetBranchAddress("mcHadronEta", &mcHadronEta);
    Run_Tree->SetBranchAddress("mcHadronPhi", &mcHadronPhi);
    Run_Tree->SetBranchAddress("mcHadronMass", &mcHadronMass);
    Run_Tree->SetBranchAddress("mcHadronE", &mcHadronE);
    Run_Tree->SetBranchAddress("mcHadronGMomPID", &mcHadronGMomPID);
    Run_Tree->SetBranchAddress("mcHadronMomPID", &mcHadronMomPID);
    Run_Tree->SetBranchAddress("mcHadronMomPt", &mcHadronMomPt);
    Run_Tree->SetBranchAddress("mcHadronMomMass", &mcHadronMomMass);
    Run_Tree->SetBranchAddress("mcHadronMomEta", &mcHadronMomEta);
    Run_Tree->SetBranchAddress("mcHadronMomPhi", &mcHadronMomPhi);
    Run_Tree->SetBranchAddress("mcStatusFlag", &mcStatusFlag);
    Run_Tree->SetBranchAddress("mcStatus", &mcStatus);
    Run_Tree->SetBranchAddress("mcMass", &mcMass);
    Run_Tree->SetBranchAddress("num_gen_jets", &num_gen_jets);
  
    
    /////////////////////////   MET Info
    Run_Tree->SetBranchAddress("pfMET",&pfMET);
    Run_Tree->SetBranchAddress("pfMETPhi",&pfMETPhi);


    std::string output = input[k];
    size_t str = output.rfind("/");
    output.erase(0,str+1);
    output = input[1] + "Fakein" + output;
    //std::string output = input[1] + "Fakein_Data.root";
    
    cout << "\n\n\n OUTPUT NAME IS:    " << output << endl;     //PRINTING THE OUTPUT FILE NAME                                                
    TFile *fout = TFile::Open(output.c_str(), "RECREATE");
    
    TH1F* h_deno = new TH1F("deno","tauPt at the denominator", 13,20,150);
    TH1F* h_neu = new TH1F("neo","tauPt at the neumerator", 13,20,150);
    TH1F* h_effi = new TH1F("effi","effi",13,20,150);
    /*TH1F* h_deno = new TH1F("deno","tauPt at the denominator", 14,10,150);
    TH1F* h_neu = new TH1F("neo","tauPt at the neumerator", 14,10,150);
    TH1F* h_effi = new TH1F("effi","effi",14,10,150);*/
    h_deno->Sumw2();
    h_effi->Sumw2();
    
    if(input[1]=="TT") TT_fake(h_deno, h_effi, Run_Tree, Muon,input[k],HistoTot);
    if(input[1]=="DY") DY_fake(h_deno, h_effi, Run_Tree, Muon,input[k],HistoTot);
    if(input[1]=="Wjets") Wjets_fake(h_deno, h_effi, Run_Tree, Muon,input[k], HistoTot);
    if(input[1]=="QCD") QCD_fake(h_deno, h_effi, Run_Tree, Muon,input[k], HistoTot);
    if(input[1]=="QCD_muonfake") QCD_muonfake(h_deno, h_effi, Run_Tree, Muon,input[k], HistoTot);
    fout->cd();
    // h_effi = (TH1F*) h_neu->Divide(h_deno);
    h_neu = (TH1F*)h_effi->Clone();
    h_neu->SetName("neo");
    h_effi->Divide(h_deno);
    h_deno->Write();
    h_neu->Write();
    h_effi->Write();
    fout->Close();
  }

  /* else {

      std::string output = input[k];
      size_t root = output.find(".root");
      output.erase(root);
      output += "_MCFake.root";
      cout << "\n\n\n OUTPUT NAME IS:    " << output << endl;     //PRINTING THE OUTPUT FILE NAME                                                
      TFile *fout = TFile::Open(output.c_str(), "RECREATE");

      TH1F* h_deno = new TH1F("deno","tauPt at the denominator", 13,20,150);
      TH1F* h_neu = new TH1F("neo","tauPt at the neumerator", 13,20,150);
      TH1F* h_effi = new TH1F("effi","effi",13,20,150);
      h_deno->Sumw2();
      h_effi->Sumw2();

      if(input[k].find("TT") != string::npos) TT_fake(h_deno, h_effi, Run_Tree, Muon,input[k], HistoTot);
      if(input[k].find("DY") != string::npos) DY_fake(h_deno, h_effi, Run_Tree, Muon,input[k],HistoTot);
      if(input[k].find("W") != string::npos) Wjets_fake(h_deno, h_effi, Run_Tree, Muon,input[k],HistoTot);
      fout->cd();
      // h_effi = (TH1F*) h_neu->Divide(h_deno);                                                                                             
      h_neu = (TH1F*)h_effi->Clone();
      h_neu->SetName("neo");
      h_effi->Divide(h_deno);
      h_deno->Write();
      h_neu->Write();
      h_effi->Write();
      fout->Close();
      }*/
  //}
}


  
void TT_fake(TH1F* h_deno, TH1F* h_neu, TTree* Run_Tree, bool Muon, std::string input, TH1F* HistoTot )       {
  
  Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();
  
  cout<<"nentries_wtn====" << nentries_wtn << "\n";


  TFile * PUData= TFile::Open("dataMoriondPU.root");
  TH1F * HistoPUData= (TH1F *) PUData->Get("pileup");
  HistoPUData->Scale(1.0/HistoPUData->Integral());

  TFile * PUMC= TFile::Open("mcMoriondPU.root");
  TH1F * HistoPUMC= (TH1F *) PUMC->Get("pileup");
  HistoPUMC->Scale(1.0/HistoPUMC->Integral());

  
  for ( Int_t i = 0; i < nentries_wtn; i++) {
  
    //  for ( Int_t i = 0; i < 10000; i++) { 
    Run_Tree->GetEntry(i);
    
    if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
    
    bool PassTrigger =   (Muon) ? ((HLTEleMuX >> 14 & 1) || (HLTEleMuX >> 15 & 1)) : (HLTEleMuX >> 5 & 1);// || (HLTEleMuX >> 19 & 1) || (HLTEleMuX >> 20 & 1)) : (HLTEleMuX >> 34 & 1);
    if(!PassTrigger) continue;                                       
    
    int count_bjet(0);
    for (int ijet= 0 ; ijet < nJet ; ijet++){
      if (jetPt->at(ijet) > 20 && fabs(jetEta->at(ijet)) < 2.5 && jetCSV2BJetTags->at(ijet) > 0.935){//0.935. //0.679
	count_bjet +=1;
      }
    }
    if(count_bjet <1) continue; 
    if(pfMET <80) continue;

    float LumiWeight = 1;
    float GetGenWeight=1;
    float PUWeight = 1;

    if (!isData) {
      if (HistoTot) LumiWeight = weightCalc(HistoTot, input, num_gen_jets, W_events, DY_events);
      //      cout << "lumi=========" << LumiWeight << endl;                                                                                  
      GetGenWeight=genWeight;
      int puNUmmc=int(puTrue->at(0)*10);
      int puNUmdata=int(puTrue->at(0)*10);
      float PUMC_=HistoPUMC->GetBinContent(puNUmmc+1);
      float PUData_=HistoPUData->GetBinContent(puNUmdata+1);
      if (PUMC_ ==0)
	cout<<"PUMC_ is zero!!! & num pileup= "<< puTrue->at(0)<<"\n";
      else
	PUWeight= PUData_/PUMC_;
    }

    double weight = GetGenWeight*PUWeight*LumiWeight;


    ///////////////////////////////////////////////
    //Important Analysis Loop Will Happen Here!!!//
    ///////////////////////////////////////////////
    
    bool firstPart(true);
    std::vector<int> vec_muele, vec_tau;
    
    if(Muon) {
      
      for  (int imu=0 ; imu < nMu; imu++){
	bool mupt = (firstPart) ? (muPt->at(imu) >18) : (muPt->at(imu) >9);
	UShort_t id = (muIDbit->at(imu) >> 1 & 1);
	float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
	if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
	  IsoMu= ( muPFChIso->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
	
	if(mupt && id && fabs(muEta->at(imu)) <2.4 && IsoMu <0.3) {
	  firstPart = false;
	  vec_muele.push_back(imu);
	}
      }
      
    }
    else {
      
      for (int iele = 0; iele < nEle; ++iele) {
	
	bool elept = (firstPart) ? (elePt->at(iele) >24) : (elePt->at(iele) >13);
	
	float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
	if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
	  IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);
	
	bool eleMVAId= false;
	if (fabs (eleSCEta->at(iele)) < 0.8 && eleIDMVA->at(iele) > 0.967083) eleMVAId= true;
	else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <  1.5 && eleIDMVA->at(iele) > 0.929117) eleMVAId= true;
	else if ( fabs (eleSCEta->at(iele)) >  1.5 && eleIDMVA->at(iele) > 0.726311 ) eleMVAId= true;
	else eleMVAId= false;
	
	if(elept && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle <0.25) {
	  firstPart = false;
	  vec_muele.push_back(iele);
	}
      }
    }
    
    if(muCharge->at(vec_muele[0]) * muCharge->at(vec_muele[1]) >0) continue;

    
    bool muveto(false);
    
    for(int imu=0; imu < nMu; imu++) {
      if(imu == vec_muele[0] || imu == vec_muele[1]) continue;
      UShort_t id = (muIDbit->at(imu) >> 1 & 1);
      
      float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
      if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
	IsoMu= ( muPFChIso->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
      
      if(muPt->at(imu) >9 && fabs(muEta->at(imu)) <2.4 && id && IsoMu <0.3) {
	muveto = true;
	break;
      }
    }
    
    if(muveto) continue;
    
    TLorentzVector m1,m2,M;
    if(Muon) {
      m1.SetPtEtaPhiE(muPt->at(vec_muele[0]),muEta->at(vec_muele[0]),muPhi->at(vec_muele[0]),muEn->at(vec_muele[0]));
      m2.SetPtEtaPhiE(muPt->at(vec_muele[1]),muEta->at(vec_muele[1]),muPhi->at(vec_muele[1]),muEn->at(vec_muele[1]));
    }
    else {
      m1.SetPtEtaPhiE(elePt->at(vec_muele[0]),eleEta->at(vec_muele[0]),elePhi->at(vec_muele[0]),eleEn->at(vec_muele[0]));
      m2.SetPtEtaPhiE(elePt->at(vec_muele[1]),eleEta->at(vec_muele[1]),elePhi->at(vec_muele[1]),eleEn->at(vec_muele[1]));
    }
    
    M= m1+m2;
    if(M.M() >70 && M.M() <110) continue;
    
    bool eleveto(false);
    
    for (int iele = 0; iele < nEle; ++iele) {
      
      float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
      if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
	IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);
      
      bool eleMVAId= false;
      if (fabs (eleSCEta->at(iele)) < 0.8 && eleIDMVA->at(iele) > 0.967083) eleMVAId= true;
      else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <  1.5 && eleIDMVA->at(iele) > 0.929117) eleMVAId= true;
      else if ( fabs (eleSCEta->at(iele)) >  1.5 && eleIDMVA->at(iele) > 0.726311 ) eleMVAId= true;
      else eleMVAId= false;
      
      if(elePt->at(iele) >15 && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle <0.1) {
	eleveto = true;
	break;
      }
    }
    
    if(eleveto) continue;
    
    for  (int itau=0 ; itau < nTau; itau++) {
      
      if(tauPt->at(itau) > 20 && fabs(tauEta->at(itau)) < 2.3 && tauByMVA6LooseElectronRejection->at(itau) !=0 && tauByTightMuonRejection3->at(itau) !=0 && taupfTausDiscriminationByDecayModeFinding->at(itau) !=0) 
	vec_tau.push_back(itau);
    }
    
    if(vec_tau.size() <2) continue;

    for(int i=0; i<vec_tau.size(); i++) {
      h_deno->Fill(tauPt->at(vec_tau[i]), weight);
      if(tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[i]) ==0) continue;
      h_neu->Fill(tauPt->at(vec_tau[i]),weight);
    }
  }
}

void DY_fake(TH1F* h_deno, TH1F* h_neu, TTree* Run_Tree, bool Muon, std::string input, TH1F* HistoTot )       {

  Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();

  cout <<"nentries_wtn====" << nentries_wtn << "\n";

  TFile * PUData= TFile::Open("dataMoriondPU.root");
  TH1F * HistoPUData= (TH1F *) PUData->Get("pileup");
  HistoPUData->Scale(1.0/HistoPUData->Integral());

  TFile * PUMC= TFile::Open("mcMoriondPU.root");
  TH1F * HistoPUMC= (TH1F *) PUMC->Get("pileup");
  HistoPUMC->Scale(1.0/HistoPUMC->Integral());

  for ( Int_t i = 0; i < nentries_wtn; i++) {
    //for ( Int_t i = 0; i < 100000; i++) {
    Run_Tree->GetEntry(i);
    
    if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
    
    bool PassTrigger =   (Muon) ? ((HLTEleMuX >> 14 & 1) || (HLTEleMuX >> 15 & 1)) : (HLTEleMuX >> 5 & 1);
    // || (HLTEleMuX >> 19 & 1) || (HLTEleMuX >> 20 & 1)) : (HLTEleMuX >> 34 & 1);                          
    if(!PassTrigger) continue;
    int count_bjet(0);
    for (int ijet= 0 ; ijet < nJet ; ijet++){
      if (jetPt->at(ijet) > 20 && fabs(jetEta->at(ijet)) < 2.5 && jetCSV2BJetTags->at(ijet) > 0.800){
        count_bjet +=1;
	break;
      }
    }
    if(count_bjet) continue;
    if(pfMET >20) continue;

    float LumiWeight = 1;
    float GetGenWeight=1;
    float PUWeight = 1;

    if (!isData) {
      if (HistoTot) LumiWeight = weightCalc(HistoTot, input, num_gen_jets, W_events, DY_events);
      //      cout << "lumi=========" << LumiWeight << endl;                                                                                  
      GetGenWeight=genWeight;
      int puNUmmc=int(puTrue->at(0)*10);
      int puNUmdata=int(puTrue->at(0)*10);
      float PUMC_=HistoPUMC->GetBinContent(puNUmmc+1);
      float PUData_=HistoPUData->GetBinContent(puNUmdata+1);
      if (PUMC_ ==0)
	cout<<"PUMC_ is zero!!! & num pileup= "<< puTrue->at(0)<<"\n";
      else
	PUWeight= PUData_/PUMC_;
    }

    double weight = GetGenWeight*PUWeight*LumiWeight;



    ///////////////////////////////////////////////                                                                                            
    //Important Analysis Loop Will Happen Here!!!//                                                                                            
    ///////////////////////////////////////////////                                                                                            

    bool firstPart(true);
    std::vector<int> vec_muele, vec_tau;

    if(Muon) {

      for  (int imu=0 ; imu < nMu; imu++){
        bool mupt = (firstPart) ? (muPt->at(imu) >18) : (muPt->at(imu) >9);
        UShort_t id = (muIDbit->at(imu) >> 1 & 1);
        float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
        if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
          IsoMu= ( muPFChIso->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);

        if(mupt && id && fabs(muEta->at(imu)) <2.4 && IsoMu <0.3) {
          firstPart = false;
          vec_muele.push_back(imu);
        }
      }

    }
    else {
      for (int iele = 0; iele < nEle; ++iele) {

        bool elept = (firstPart) ? (elePt->at(iele) >24) : (elePt->at(iele) >13);

	float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
        if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
          IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);

	bool eleMVAId= false;
        if (fabs (eleSCEta->at(iele)) <= 0.8 && eleIDMVA->at(iele) > 0.837) eleMVAId= true;
        else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <=  1.5 && eleIDMVA->at(iele) > 0.715) eleMVAId= true;
        else if ( fabs (eleSCEta->at(iele)) >=  1.5 && eleIDMVA->at(iele) > 0.357 ) eleMVAId= true;
	else eleMVAId= false;

	//bool elecharge = eleChargeConsistent->at(iele) || ele2ndChargeConsistent->at(iele);
        if(elept && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle <0.25) {
          firstPart = false;
          vec_muele.push_back(iele);
	}
      }
    }

    if(vec_muele.size() !=2) continue;
    if (Muon) {
      if(muCharge->at(vec_muele[0]) * muCharge->at(vec_muele[1]) >0) continue;
    }
    else {
      if(eleCharge->at(vec_muele[0]) * eleCharge->at(vec_muele[1]) >0) continue;
    }

    bool muveto(false);

    for(int imu=0; imu < nMu; imu++) {
      if(Muon && (imu == vec_muele[0] || imu == vec_muele[1])) continue;
      UShort_t id = (muIDbit->at(imu) >> 1 & 1);

      float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
      if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
        IsoMu= ( muPFChIso->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);

      if(muPt->at(imu) >9 && fabs(muEta->at(imu)) <2.4 && id && IsoMu <0.3) {
	muveto = true;
        break;
      }
    }

    if(muveto) continue;

    TLorentzVector m1,m2,M;
    if(Muon) {
      m1.SetPtEtaPhiE(muPt->at(vec_muele[0]),muEta->at(vec_muele[0]),muPhi->at(vec_muele[0]),muEn->at(vec_muele[0]));
      m2.SetPtEtaPhiE(muPt->at(vec_muele[1]),muEta->at(vec_muele[1]),muPhi->at(vec_muele[1]),muEn->at(vec_muele[1]));
    }
    else {
      m1.SetPtEtaPhiE(elePt->at(vec_muele[0]),eleEta->at(vec_muele[0]),elePhi->at(vec_muele[0]),eleEn->at(vec_muele[0]));
      m2.SetPtEtaPhiE(elePt->at(vec_muele[1]),eleEta->at(vec_muele[1]),elePhi->at(vec_muele[1]),eleEn->at(vec_muele[1]));
    }

    M= m1+m2;
    if(M.M() <70 || M.M() >110) continue;

    bool eleveto(false);

    for (int iele = 0; iele < nEle; ++iele) {

      if(!Muon && (iele == vec_muele[0] || iele == vec_muele[1])) continue;
      float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
      if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
        IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);

      bool eleMVAId= false;
      if (fabs (eleSCEta->at(iele)) <= 0.8 && eleIDMVA->at(iele) > 0.837) eleMVAId= true;
      else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <=  1.5 && eleIDMVA->at(iele) > 0.715) eleMVAId= true;
      else if ( fabs (eleSCEta->at(iele)) >=  1.5 && eleIDMVA->at(iele) > 0.357 ) eleMVAId= true;
      else eleMVAId= false;

      if(elePt->at(iele) >13 && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle <0.25) {
	eleveto = true;
        break;
      }
    }

    if(eleveto) continue;
    for  (int itau=0 ; itau < nTau; itau++) {

      //      if(tauPt->at(itau) > 20 && fabs(tauEta->at(itau)) < 2.3 && tauByMVA6LooseElectronRejection->at(itau) !=0 && tauByTightMuonRejection3->at(itau) !=0 && taupfTausDiscriminationByDecayModeFinding->at(itau) !=0 && tauByVLooseIsolationMVArun2v1DBoldDMwLT->at(itau) !=0)
      bool taupass = (Muon) ? (tauByMVA6LooseElectronRejection->at(itau) !=0 && tauByTightMuonRejection3->at(itau) !=0) : (tauByMVA6TightElectronRejection->at(itau) !=0 && tauByLooseMuonRejection3->at(itau) !=0);
      if(tauPt->at(itau) > 10 && fabs(tauEta->at(itau)) < 2.3 && taupass && taupfTausDiscriminationByDecayModeFinding->at(itau) !=0) 
        vec_tau.push_back(itau);
    }

    if(vec_tau.size() <2) continue;
    for(int i=0; i<vec_tau.size(); i++) {
      h_deno->Fill(tauPt->at(vec_tau[i]),weight);
      if(tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[i]) ==0) continue;
      h_neu->Fill(tauPt->at(vec_tau[i]),weight);
    }
  }
}

void Wjets_fake(TH1F* h_deno, TH1F* h_neu, TTree* Run_Tree, bool Muon, std::string input, TH1F* HistoTot )       {

  Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();

  cout<<"nentries_wtn====" << nentries_wtn << "\n";

  TFile * PUData= TFile::Open("dataMoriondPU.root");
  TH1F * HistoPUData= (TH1F *) PUData->Get("pileup");
  HistoPUData->Scale(1.0/HistoPUData->Integral());

  TFile * PUMC= TFile::Open("mcMoriondPU.root");
  TH1F * HistoPUMC= (TH1F *) PUMC->Get("pileup");
  HistoPUMC->Scale(1.0/HistoPUMC->Integral());


    for ( Int_t i = 0; i < nentries_wtn; i++) {                                                                                            
      //    for ( Int_t i = 0; i < 10000; i++) {
    Run_Tree->GetEntry(i);
    
    if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
    bool PassTrigger =   (Muon) ? ((HLTEleMuX >> 19 & 1) || (HLTEleMuX >> 20 & 1))  : (HLTEleMuX >> 0 & 1);// || (HLTEleMuX >> 19 & 1) || (HLTEleMuX >> 20 & 1)) : (HLTEleMuX >> 34 & 1);                                                                                                    
    if(!PassTrigger) continue;
    int count_bjet(0);
    for (int ijet= 0 ; ijet < nJet ; ijet++){
      if (jetPt->at(ijet) > 20 && fabs(jetEta->at(ijet)) < 2.5 && jetCSV2BJetTags->at(ijet) > 0.800){
	count_bjet +=1;
	break;
      }
    }
    if(count_bjet) continue;
    if(pfMET <40) continue;

    float LumiWeight = 1;
    float GetGenWeight=1;
    float PUWeight = 1;

    if(input.find("W1Jets") != string::npos) num_gen_jets=1;
    if(input.find("W2Jets") != string::npos) num_gen_jets=2;
    if(input.find("W3Jets") != string::npos) num_gen_jets=3;
    if(input.find("W4Jets") != string::npos) num_gen_jets=4;

    if (!isData) {
      if (HistoTot) LumiWeight = weightCalc(HistoTot, input, num_gen_jets, W_events, DY_events);
      //      cout << "lumi=========" << LumiWeight << endl;                                                                                  
      GetGenWeight=genWeight;
      int puNUmmc=int(puTrue->at(0)*10);
      int puNUmdata=int(puTrue->at(0)*10);
      float PUMC_=HistoPUMC->GetBinContent(puNUmmc+1);
      float PUData_=HistoPUData->GetBinContent(puNUmdata+1);
      if (PUMC_ ==0)
	cout<<"PUMC_ is zero!!! & num pileup= "<< puTrue->at(0)<<"\n";
      else
	PUWeight= PUData_/PUMC_;
    }

    double weight = GetGenWeight*PUWeight*LumiWeight;

    
    ///////////////////////////////////////////////
    //Important Analysis Loop Will Happen Here!!!//    
    ///////////////////////////////////////////////    
    
    bool firstPart(true);
    std::vector<int> vec_muele, vec_tau;
    
    if(Muon) {

      for  (int imu=0 ; imu < nMu; imu++){
	//	bool mupt = (firstPart) ? (muPt->at(imu) >18) : (muPt->at(imu) >9);
	bool mupt =  (muPt->at(imu) >26);
	UShort_t id = (muIDbit->at(imu) >> 1 & 1);
	float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
	if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
	  IsoMu= ( muPFChIso->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);

	//	bool isocut = (firstPart) ? IsoMu <0.3 : IsoMu >0.3; 

	if(mupt && id && fabs(muEta->at(imu)) <2.4 && IsoMu <0.3) {
	  //	  firstPart = false;
	  vec_muele.push_back(imu);
	  break;
	}
      }

    }
    else {
      for (int iele = 0; iele < nEle; ++iele) {
	
	bool elept = (firstPart) ? (elePt->at(iele) >26) : (elePt->at(iele) >13);
	
	float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
	if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
	  IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);
	
	bool eleMVAId= false;
	if (fabs (eleSCEta->at(iele)) <= 0.8 && eleIDMVA->at(iele) > 0.837) eleMVAId= true;
	else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <=  1.5 && eleIDMVA->at(iele) > 0.715) eleMVAId= true;
	else if ( fabs (eleSCEta->at(iele)) >=  1.5 && eleIDMVA->at(iele) > 0.357 ) eleMVAId= true;
	else eleMVAId= false;

	
	if(elept && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle <0.25) {
	  firstPart = false;
	  vec_muele.push_back(iele);
	}
      }
    }

    //    if(vec_muele.size() <2) continue;
    if(vec_muele.size() <1) continue; 
    //    if(muCharge->at(vec_muele[0]) * muCharge->at(vec_muele[1]) >0) continue;
    float MuMetTranverseMass= (Muon) ? TMass_FNew(muPt->at(vec_muele[0]), muPhi->at(vec_muele[0]), pfMET, pfMETPhi) : TMass_FNew(elePt->at(vec_muele[0]), elePhi->at(vec_muele[0]), pfMET, pfMETPhi);
    
    if(MuMetTranverseMass <50) continue;
    
    bool muveto(false);
    
    for(int imu=0; imu < nMu; imu++) {
      //  if(imu == vec_muele[0] || imu == vec_muele[1]) continue;
      if(Muon && imu == vec_muele[0]) continue; 
      UShort_t id = (muIDbit->at(imu) >> 1 & 1);
      
      float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
      if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
	IsoMu= ( muPFChIso->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
      
      if(muPt->at(imu) >9 && fabs(muEta->at(imu)) <2.4 && id && IsoMu <0.3) {
	muveto = true;
	break;
      }
    }
    
    if(muveto) continue;
    
    TLorentzVector m1,m2,M;
    /*    if(Muon) {
      m1.SetPtEtaPhiE(muPt->at(vec_muele[0]),muEta->at(vec_muele[0]),muPhi->at(vec_muele[0]),muEn->at(vec_muele[0]));
      m2.SetPtEtaPhiE(muPt->at(vec_muele[1]),muEta->at(vec_muele[1]),muPhi->at(vec_muele[1]),muEn->at(vec_muele[1]));
    }
    else {
      m1.SetPtEtaPhiE(elePt->at(vec_muele[0]),eleEta->at(vec_muele[0]),elePhi->at(vec_muele[0]),eleEn->at(vec_muele[0]));
      m2.SetPtEtaPhiE(elePt->at(vec_muele[1]),eleEta->at(vec_muele[1]),elePhi->at(vec_muele[1]),eleEn->at(vec_muele[1]));
    }
    
    M= m1+m2;
    if(M.M() >70 && M.M() <110) continue;*/
 
    bool eleveto(false);
    for (int iele = 0; iele < nEle; ++iele) {
      if(!Muon && iele == vec_muele[0]) continue;
      float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
      if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
	IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);
      
      bool eleMVAId= false;
      if (fabs (eleSCEta->at(iele)) <= 0.8 && eleIDMVA->at(iele) > 0.837) eleMVAId= true;
      else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <=  1.5 && eleIDMVA->at(iele) > 0.715) eleMVAId= true;
      else if ( fabs (eleSCEta->at(iele)) >=  1.5 && eleIDMVA->at(iele) > 0.357 ) eleMVAId= true;
      else eleMVAId= false;
      
      if(elePt->at(iele) >13 && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle <0.25) {
	eleveto = true;
	break;
      }
 }
    
    if(eleveto) continue;
    
    for  (int itau=0 ; itau < nTau; itau++) {

      //      if(muCharge->at(vec_muele[0])*tauCharge->at(itau) <0) continue;
      if(vec_tau.size() >0 && tauCharge->at(itau)*tauCharge->at(vec_tau[0]) <0) continue;
      bool antitau = (Muon) ? (tauByMVA6LooseElectronRejection->at(itau) !=0 && tauByTightMuonRejection3->at(itau) !=0) : (tauByMVA6TightElectronRejection->at(itau) !=0 && tauByLooseMuonRejection3->at(itau) !=0);
      if(tauPt->at(itau) > 10 && fabs(tauEta->at(itau)) < 2.3 && antitau && taupfTausDiscriminationByDecayModeFinding->at(itau) !=0)
	vec_tau.push_back(itau);
    }
    
    if(vec_tau.size() <2) continue;

    for(int i=0; i<vec_tau.size(); i++) {
      h_deno->Fill(tauPt->at(vec_tau[i]), weight);
      if(tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[i]) ==0) continue;
      h_neu->Fill(tauPt->at(vec_tau[i]),weight);
    }
  }
}


void QCD_fake(TH1F* h_deno, TH1F* h_neu, TTree* Run_Tree, bool Muon, std::string input, TH1F* HistoTot )       {

  Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();

  cout<<"nentries_wtn====" << nentries_wtn << "\n";

  TFile * PUData= TFile::Open("dataMoriondPU.root");
  TH1F * HistoPUData= (TH1F *) PUData->Get("pileup");
  HistoPUData->Scale(1.0/HistoPUData->Integral());

  TFile * PUMC= TFile::Open("mcMoriondPU.root");
  TH1F * HistoPUMC= (TH1F *) PUMC->Get("pileup");
  HistoPUMC->Scale(1.0/HistoPUMC->Integral());


  for ( Int_t i = 0; i < nentries_wtn; i++) {
  //   for ( Int_t i = 0; i < 100; i++) {                                                                                               
    Run_Tree->GetEntry(i);

    if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
    bool PassTrigger =   (Muon) ? ((HLTEleMuX >> 14 & 1) || (HLTEleMuX >> 15 & 1)) : (HLTEleMuX >> 34 & 1);// || (HLTEleMuX >> 19 & 1) || (HLTEleMuX >> 20 & 1)) : (HLTEleMuX >> 34 & 1);
    if(!PassTrigger) continue;

    int count_bjet(0);
    for (int ijet= 0 ; ijet < nJet ; ijet++){
      if (jetPt->at(ijet) > 20 && fabs(jetEta->at(ijet)) < 2.5 && jetCSV2BJetTags->at(ijet) > 0.800){
	count_bjet +=1;
	break;
      }
    }
    if(count_bjet) continue;
    //    if(pfMET <20) continue;
    
    float LumiWeight = 1;
    float GetGenWeight=1;
    float PUWeight = 1;
    
    if(input.find("W1Jets") != string::npos) num_gen_jets=1;
    if(input.find("W2Jets") != string::npos) num_gen_jets=2;
    if(input.find("W3Jets") != string::npos) num_gen_jets=3;
    if(input.find("W4Jets") != string::npos) num_gen_jets=4;
    
    if (!isData) {
      if (HistoTot) LumiWeight = weightCalc(HistoTot, input, num_gen_jets, W_events, DY_events);
      GetGenWeight=genWeight;
      int puNUmmc=int(puTrue->at(0)*10);
      int puNUmdata=int(puTrue->at(0)*10);
      float PUMC_=HistoPUMC->GetBinContent(puNUmmc+1);
      float PUData_=HistoPUData->GetBinContent(puNUmdata+1);
      if (PUMC_ ==0)
	cout<<"PUMC_ is zero!!! & num pileup= "<< puTrue->at(0)<<"\n";
      else
	PUWeight= PUData_/PUMC_;
    }
    
    double weight = GetGenWeight*PUWeight*LumiWeight;

    ///////////////////////////////////////////////
    //Important Analysis Loop Will Happen Here!!!
    ///////////////////////////////////////////////                                                                                            

    bool firstPart(true);
    std::vector<int> vec_muele, vec_tau;
    
    if(Muon) {
      
      for  (int imu=0 ; imu < nMu; imu++){
	bool mupt = (firstPart) ? (muPt->at(imu) >18) : (muPt->at(imu) >9);
	UShort_t id = (muIDbit->at(imu) >> 1 & 1);
	float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
	if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
	  IsoMu= ( muPFChIso->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
	
	//      bool isocut = (firstPart) ? IsoMu <0.3 : IsoMu >0.3;
	if(mupt && id && fabs(muEta->at(imu)) <2.4 && IsoMu >0.3) {
	  firstPart = false;
	  vec_muele.push_back(imu);
	}
      }
      
    }
    else {
      for (int iele = 0; iele < nEle; ++iele) {
	
	bool elept = (firstPart) ? (elePt->at(iele) >18) : (elePt->at(iele) >15);
	
	float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
	if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
	  IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);
	
	bool eleMVAId= false;
	if (fabs (eleSCEta->at(iele)) < 0.8 && eleIDMVA->at(iele) > 0.967083) eleMVAId= true;
	else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <  1.5 && eleIDMVA->at(iele) > 0.929117) eleMVAId= true;
	else if ( fabs (eleSCEta->at(iele)) >  1.5 && eleIDMVA->at(iele) > 0.726311 ) eleMVAId= true;
	else eleMVAId= false;
	
	if(elept && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle >0.1) {
	  firstPart = false;
	  vec_muele.push_back(iele);
	}
      }
    }
    
    if(vec_muele.size() !=2) continue;
    if(muCharge->at(vec_muele[0]) * muCharge->at(vec_muele[1]) <0) continue;
    float MuMetTranverseMass= TMass_FNew(muPt->at(vec_muele[0]), muPhi->at(vec_muele[0]), pfMET, pfMETPhi);
    
    //    if(MuMetTranverseMass <50) continue;
    
    bool muveto(false);
    
    for(int imu=0; imu < nMu; imu++) {
      if(imu == vec_muele[0] || imu == vec_muele[1]) continue;
      UShort_t id = (muIDbit->at(imu) >> 1 & 1);
      
      float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
      if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
	IsoMu= ( muPFChIso->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);

      if(muPt->at(imu) >9 && fabs(muEta->at(imu)) <2.4 && id && IsoMu <0.3) {
	muveto = true;
	break;
      }
    }
    
    //    if(muveto) continue;
    
    TLorentzVector m1,m2,M;
    if(Muon) {
      m1.SetPtEtaPhiE(muPt->at(vec_muele[0]),muEta->at(vec_muele[0]),muPhi->at(vec_muele[0]),muEn->at(vec_muele[0]));
      m2.SetPtEtaPhiE(muPt->at(vec_muele[1]),muEta->at(vec_muele[1]),muPhi->at(vec_muele[1]),muEn->at(vec_muele[1]));
    }
    else {
      m1.SetPtEtaPhiE(elePt->at(vec_muele[0]),eleEta->at(vec_muele[0]),elePhi->at(vec_muele[0]),eleEn->at(vec_muele[0]));
      m2.SetPtEtaPhiE(elePt->at(vec_muele[1]),eleEta->at(vec_muele[1]),elePhi->at(vec_muele[1]),eleEn->at(vec_muele[1]));
    }
    
    M= m1+m2;
    if(M.M() >70 && M.M() <110) continue;
    
    bool eleveto(false);
    for (int iele = 0; iele < nEle; ++iele) {
  
      float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
      if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
	IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);
      
      bool eleMVAId= false;
      if (fabs (eleSCEta->at(iele)) < 0.8 && eleIDMVA->at(iele) > 0.967083) eleMVAId= true;
      else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <  1.5 && eleIDMVA->at(iele) > 0.929117) eleMVAId= true;
      else if ( fabs (eleSCEta->at(iele)) >  1.5 && eleIDMVA->at(iele) > 0.726311 ) eleMVAId= true;
      else eleMVAId= false;
      
      if(elePt->at(iele) >15 && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle <0.1) {
	eleveto = true;
	break;
      }
    }
    
    if(eleveto) continue;
    
    for  (int itau=0 ; itau < nTau; itau++) {

      if(tauPt->at(itau) > 10 && fabs(tauEta->at(itau)) < 2.3 && tauByMVA6LooseElectronRejection->at(itau) !=0 && tauByTightMuonRejection3->at(itau) !=0 && taupfTausDiscriminationByDecayModeFinding->at(itau) !=0)
	vec_tau.push_back(itau);
    }
    
    if(vec_tau.size() <2) continue;
    
    for(int i=0; i<vec_tau.size(); i++) {
      h_deno->Fill(tauPt->at(vec_tau[i]), weight);
      if(tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[i]) ==0) continue;
      h_neu->Fill(tauPt->at(vec_tau[i]),weight);
    }
  }
}


double scaleFactor(double pt, double eta) {
  
  int ptbin(-1), etabin(-1);
  for(int i=0; i<SF->GetYaxis()->GetNbins(); i++) {
    double binlow = SF->GetYaxis()->GetBinLowEdge(i+1);
    double binwidth = SF->GetYaxis()->GetBinWidth(i+1);
    //    cout << "binlow== " << binlow << "binhigh= " << binlow+binwidth << "pt== " << pt << endl;                                            
    if(pt >= binlow && pt < binlow+binwidth) {
      ptbin = i+1;
      break;
    }
  }
  if(ptbin==-1) {
    //    cout << "**************** pt = " << pt << "\t doesn't fall within the pt range of the histogram" << endl;                            
    ptbin = SF->GetYaxis()->GetNbins();
  }

  for(int i=0; i<SF->GetXaxis()->GetNbins(); i++) {
    double binlow = SF->GetXaxis()->GetBinLowEdge(i+1);
    double binwidth = SF->GetXaxis()->GetBinWidth(i+1);
    if(eta >=binlow && eta < binlow+binwidth) {
      etabin = i+1;
      break;
    }
  }
  if(etabin==-1)cout <<"**************** eta = " << eta << "\t doesn't fall within the eta range of the histogram"<< endl;
  //  cout << "ptbin== " << ptbin << "\t" << "etabin== " << etabin << "pt== " << pt << "eta== " << eta << endl;                                
  //cout << "SF== " << SF->GetBinContent(etabin,ptbin)  << endl;                                                                               
  return SF->GetBinContent(etabin,ptbin);
}

void QCD_muonfake(TH1F* h_deno, TH1F* h_neu, TTree* Run_Tree, bool Muon, std::string input, TH1F* HistoTot )       {
  
  Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();
  
  cout<<"nentries_wtn====" << nentries_wtn << "\n";
  
  TFile * PUData= TFile::Open("dataMoriondPU.root");
  TH1F * HistoPUData= (TH1F *) PUData->Get("pileup");
  HistoPUData->Scale(1.0/HistoPUData->Integral());
  
  TFile * PUMC= TFile::Open("mcMoriondPU.root");
  TH1F * HistoPUMC= (TH1F *) PUMC->Get("pileup");
  HistoPUMC->Scale(1.0/HistoPUMC->Integral());
  
  
  for ( Int_t i = 0; i < nentries_wtn; i++) {
    //    cout << "ii=========" << i << endl;
    //   for ( Int_t i = 0; i < 100; i++
    Run_Tree->GetEntry(i);
    
    if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
    bool PassTrigger =   (Muon) ? ((HLTEleMuX >> 14 & 1) || (HLTEleMuX >> 15 & 1)) : (HLTEleMuX >> 34 & 1);// || (HLTEleMuX >> 19 & 1) || (HLT \EleMuX >> 20 & 1)) : (HLTEleMuX >> 34 & 1);                                                                                                    
    if(!PassTrigger) continue;
    
    int count_bjet(0);
    for (int ijet= 0 ; ijet < nJet ; ijet++){
      if (jetPt->at(ijet) > 20 && fabs(jetEta->at(ijet)) < 2.5 && jetCSV2BJetTags->at(ijet) > 0.800){
	count_bjet +=1;
	break;
      }
    }
    if(count_bjet) continue;
    //    if(pfMET <20) continue;                                                                                                              
    
    float LumiWeight = 1;
    float GetGenWeight=1;
    float PUWeight = 1;
    
    if(input.find("W1Jets") != string::npos) num_gen_jets=1;
    if(input.find("W2Jets") != string::npos) num_gen_jets=2;
    if(input.find("W3Jets") != string::npos) num_gen_jets=3;
    if(input.find("W4Jets") != string::npos) num_gen_jets=4;
    
    if (!isData) {
      if (HistoTot) LumiWeight = weightCalc(HistoTot, input, num_gen_jets, W_events, DY_events);
      GetGenWeight=genWeight;
      int puNUmmc=int(puTrue->at(0)*10);
      int puNUmdata=int(puTrue->at(0)*10);
      float PUMC_=HistoPUMC->GetBinContent(puNUmmc+1);
      float PUData_=HistoPUData->GetBinContent(puNUmdata+1);
      if (PUMC_ ==0)
	cout<<"PUMC_ is zero!!! & num pileup= "<< puTrue->at(0)<<"\n";
      else
	PUWeight= PUData_/PUMC_;
    }
    
    double weight = GetGenWeight*PUWeight*LumiWeight;
    
    ///////////////////////////////////////////////
    //Important Analysis Loop Will Happen Here!!!
    ///////////////////////////////////////////////	\
    
    
    int firstPart(1);
    std::vector<int> vec_muele, vec_tau;
    float IsoMu(99);
    
    std::vector<int> rand;

    if(Muon) {
      
      //      for  (int imu=0 ; imu < nMu; imu++) rand.push_back(imu);
      //std::random_shuffle (rand.begin(), rand.end() );
      
      for  (int imu=0 ; imu < nMu; imu++){
	bool mupt = (firstPart==1) ? (muPt->at(imu) >18) : (muPt->at(imu) >9);
	if(firstPart <3 && !(muFiredTrgs->at(imu) >> 10 & 1) && !(muFiredTrgs->at(imu) >> 12 & 1)) continue;
	IsoMu=muPFChIso->at(imu)/muPt->at(imu);
	if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
	  IsoMu= ( muPFChIso->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
	
	bool iso = (firstPart <3) ? IsoMu >0.3 : true;
	if(mupt && fabs(muEta->at(imu)) <2.4 && iso) {
	  firstPart +=1;
	  vec_muele.push_back(imu);
	}
      }
      
    }
    else {
      for (int iele = 0; iele < nEle; ++iele) {
	
	bool elept = (firstPart) ? (elePt->at(iele) >18) : (elePt->at(iele) >15);
	float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
	if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
	  IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);
	
	bool eleMVAId= false;
	if (fabs (eleSCEta->at(iele)) < 0.8 && eleIDMVA->at(iele) > 0.967083) eleMVAId= true;
	else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <  1.5 && eleIDMVA->at(iele) > 0.929117) eleMVAId= true;
	else if ( fabs (eleSCEta->at(iele)) >  1.5 && eleIDMVA->at(iele) > 0.726311 ) eleMVAId= true;
	else eleMVAId= false;
	
	if(elept && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle >0.1) {
	  firstPart = false;
	  vec_muele.push_back(iele);
	}
      }
    }

    if(vec_muele.size() <3) continue;
    //    if(muCharge->at(vec_muele[0]) * muCharge->at(vec_muele[1]) <0) continue;
    float MuMetTranverseMass= TMass_FNew(muPt->at(vec_muele[0]), muPhi->at(vec_muele[0]), pfMET, pfMETPhi);
    
    //    if(MuMetTranverseMass <50) continue;                                                                                                 
    
    TLorentzVector m1,m2,M;
    if(Muon) {
      m1.SetPtEtaPhiE(muPt->at(vec_muele[0]),muEta->at(vec_muele[0]),muPhi->at(vec_muele[0]),muEn->at(vec_muele[0]));
      m2.SetPtEtaPhiE(muPt->at(vec_muele[1]),muEta->at(vec_muele[1]),muPhi->at(vec_muele[1]),muEn->at(vec_muele[1]));
    }
    else {
      m1.SetPtEtaPhiE(elePt->at(vec_muele[0]),eleEta->at(vec_muele[0]),elePhi->at(vec_muele[0]),eleEn->at(vec_muele[0]));
      m2.SetPtEtaPhiE(elePt->at(vec_muele[1]),eleEta->at(vec_muele[1]),elePhi->at(vec_muele[1]),eleEn->at(vec_muele[1]));
    }
    
    M= m1+m2;
    if(M.M() >70 && M.M() <110) continue;
    bool eleveto(false);
    for (int iele = 0; iele < nEle; ++iele) {
      
      float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
      if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
	IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);
      
      bool eleMVAId= false;
      if (fabs (eleSCEta->at(iele)) < 0.8 && eleIDMVA->at(iele) > 0.967083) eleMVAId= true;
      else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <  1.5 && eleIDMVA->at(iele) > 0.929117) eleMVAId= true;
      else if ( fabs (eleSCEta->at(iele)) >  1.5 && eleIDMVA->at(iele) > 0.726311 ) eleMVAId= true;
      else eleMVAId= false;
      
      if(elePt->at(iele) >15 && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle <0.1) {
	eleveto = true;
	break;
      }
    }
      if(eleveto) continue;
      
      bool jetoverlap(false);
      for  (int ijet=0 ; ijet < nJet; ijet++) {
	jetoverlap = false;
	if(jetPt->at(ijet) < 20 ||  fabs(jetEta->at(ijet)) > 2.4) continue;
	for(int i=0; i<vec_muele.size(); i++) {
	  double deltaR = dR_(muEta->at(vec_muele[i]), muPhi->at(vec_muele[i]), jetEta->at(ijet), jetPhi->at(ijet));
	  if (deltaR < 0.5) {
	    jetoverlap = true;
	    break;
	  }
	}
	if(!jetoverlap) break;
      }

      if(jetoverlap) continue;
      for(int i=2; i<vec_muele.size(); i++) {
	//	if((muFiraedTrgs->at(vec_muele[i]) >> 10 & 1) || (muFiredTrgs->at(vec_muele[i]) >> 12 & 1)) continue;
	h_deno->Fill(muPt->at(vec_muele[i]), weight);
	UShort_t id = (muIDbit->at(vec_muele[i]) >> 1 & 1);
	if(IsoMu >0.3 || !id) continue;
	h_neu->Fill(muPt->at(vec_muele[i]),weight);
      }
  }
}

