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


TFile * MuCorrId_BCDEF= TFile::Open(("Id_EfficienciesAndSF_BCDEF.root"));
TH2F * HistoMuId_BCDEF= (TH2F *) MuCorrId_BCDEF->Get("MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
TFile * HZZ= TFile::Open("ScaleFactors_mu_Moriond2017_v2.root");
TH2F* SF = (TH2F*) HZZ->Get("FINAL");
TFile * MuCorrId_GH= TFile::Open(("Id_EfficienciesAndSF_GH.root"));
TH2F * HistoMuId_GH= (TH2F *) MuCorrId_GH->Get("MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");

TFile * MuCorrIso_BCDEF= TFile::Open(("Iso_EfficienciesAndSF_BCDEF.root"));
TH2F * HistoMuIso_BCDEF= (TH2F *) MuCorrIso_BCDEF->Get("TightISO_TightID_pt_eta/pt_abseta_ratio");

TFile * MuCorrIso_GH= TFile::Open(("Iso_EfficienciesAndSF_GH.root"));
TH2F * HistoMuIso_GH= (TH2F *) MuCorrIso_GH->Get("TightISO_TightID_pt_eta/pt_abseta_ratio");

TFile * MuCorrTrack= TFile::Open(("Tracking_EfficienciesAndSF_BCDEFGH.root"));
TGraphAsymmErrors * HistoMuTrack= (TGraphAsymmErrors *) MuCorrTrack->Get("ratio_eff_eta3_dr030e030_corr");

TH2F* HistoMuId[2]={HistoMuId_BCDEF, HistoMuId_GH};
TH2F* HistoMuIso[2]={HistoMuIso_BCDEF,HistoMuIso_GH};

double fakeweight(double pt);
//ScaleFactor * myScaleFactor = new ScaleFactor();
//myScaleFactor->init_ScaleFactor("../../../../HTT-utilities/LepEffInterface/data/Muon/Run2016BtoH/Muon_IdIso_IsoLt0p15_2016BtoH_eff.root");

int  main(int argc, char** argv) {
  using namespace std;

  std::vector<string> input;
  for (int f = 1; f < argc; f++) {
    input.push_back(*(argv + f));

    cout << "\n INPUT NAME IS:   " << input[f - 1] << "\n";
  }

  if(input[0] != "Muon" && input[0] != "Electron" && input[0] != "MuElectron") {
    cout << "*************upppsssss**************8 pls type either Muon or Electron or MuElectron"   << endl;
    return 1;
  }
  bool Muon = (input[0]=="Muon");
  bool Electron = (input[0] == "Electron");
  bool Muelectron = (input[0] == "MuElectron");

  vector <float> W_events = W_EvenetMultiplicity();
  vector <float> DY_events = DY_EvenetMultiplicity();

  for(int k=1; k<input.size(); k++ ) {

    myMap1 = new std::map<std::string, TH1F*>();
    myMap2 = new map<string, TH2F*>();

    TFile *f_Double = TFile::Open(input[k].c_str());
    cout << "\n  Now is running on ------->   " << std::string(f_Double->GetName()) << "\n";

    TFile * myFile = TFile::Open(f_Double->GetName());
    TH1F * HistoTot = (TH1F*) myFile->Get("hcount");

    ScaleFactor * myScaleFactor = new ScaleFactor();
    myScaleFactor->init_ScaleFactor("../../../..//HTT-utilities/LepEffInterface/data/Muon/Run2016BCD/Muon_IdIso0p15_eff.root");
    //        TTree *Run_Tree = (TTree*) f_Double->Get("ggNtuplizer/EventTree");
    TTree *Run_Tree = (TTree*) f_Double->Get("EventTree");    

    std::string output = input[k];
    size_t root = output.find(".root");
    output.erase(root);
    output += "_controlPlot.root";
    //output = ".root";
    cout << "\n\n\n OUTPUT NAME IS:    " << output << endl;     //PRINTING THE OUTPUT FILE NAME
    TFile *fout = TFile::Open(output.c_str(), "RECREATE");
    TH1F* pu = new TH1F("pu","pu",100,0.,2.);
    TH1F* h_preweight =  new TH1F("preweight","weight before applying SF",100,0.,5.);
    TH1F* h_postweight = new TH1F("postweight","weight after applying SF",100,0.,5.);
    TH1F* h_SF1 = new TH1F("SF1","SF1",100,0.9,1.1);
    TH1F* h_SF2 = new TH1F("SF2","SF2",100,0.9,1.1);
    TH1F* h_SF  = new TH1F("SF","SF",100,0.9,1.1);
    TH1F* h_musize = new TH1F("musize","musize",10,0,10);
    TH1F* h_tausize = new TH1F("tausize","tausize",10,0,10);
    //    TH1F* mutausize = new TH1F("mutausize","mutausize",10,0,10);
    TH1F* corrpu;
    string Charge[2] = {"opposite_sign","same_sign"};
    string totcharge[2] = {"totCharge==0","totCharge!=0"};
    string taumucharge[2] = {"totCharge==0_and_mucharge==0","totCharge==0_and_mucharge_not_equal_to_zero_i.e_signal_region"};
    string tauCharge_[2] = {"opposite_sign_tau","same_sign_tau"};
    string muCharge_[2] = {"opposite_sign_muon","same_sign_muon"};
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
    
    
    Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();
    
    cout<<"nentries_wtn====" << nentries_wtn << "\n";


    TFile * PUData= TFile::Open("dataMoriondPU.root");
    TH1F * HistoPUData= (TH1F *) PUData->Get("pileup");
    HistoPUData->Scale(1.0/HistoPUData->Integral());
    
    TFile * PUMC= TFile::Open("mcMoriondPU.root");
    TH1F * HistoPUMC= (TH1F *) PUMC->Get("pileup");
    HistoPUMC->Scale(1.0/HistoPUMC->Integral());
    corrpu = (TH1F*)HistoPUMC->Clone();
    for ( Int_t i = 0; i < nentries_wtn; i++) {
      
      //    for ( Int_t i = 0; i < 10; i++) { 
      Run_Tree->GetEntry(i);

      if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
    
      bool PassTrigger =   (Muon) ? ((HLTEleMuX >> 14 & 1) || (HLTEleMuX >> 15 & 1)) : ( Electron ? (HLTEleMuX >> 5 & 1) : (HLTEleMuX >> 5 & 1));// || (HLTEleMuX >> 19 & 1) || (HLTEleMuX >> 20 & 1)) : (HLTEleMuX >> 34 & 1);
      if(!PassTrigger) continue;                                       
      //      cout << "PassTrigger" << endl;
      int count_bjet(0);
      for (int ijet= 0 ; ijet < nJet ; ijet++){
	if (jetPt->at(ijet) > 20 && fabs(jetEta->at(ijet)) < 2.5 && jetCSV2BJetTags->at(ijet) > 0.679){
	  count_bjet +=1;
	  break;
	}
      }
      if(count_bjet) continue; 
      
      float LumiWeight = 1;
      float GetGenWeight=1;
      float PUWeight = 1;

      if(input[k].find("DY1Jets") != string::npos) num_gen_jets =1;
      if(input[k].find("DY2Jets") != string::npos) num_gen_jets=2;
      if(input[k].find("DY3Jets") != string::npos) num_gen_jets=3;
      if(input[k].find("DY4Jets") != string::npos) num_gen_jets=4;
      if(input[k].find("W1Jets") != string::npos) num_gen_jets=1;
      if(input[k].find("W2Jets") != string::npos) num_gen_jets=2;
      if(input[k].find("W3Jets") != string::npos) num_gen_jets=3;
      if(input[k].find("W4Jets") != string::npos) num_gen_jets=4;

      if (!isData) {
	if (HistoTot) LumiWeight = weightCalc(HistoTot, input[k], num_gen_jets, W_events, DY_events);
	//	cout << "lumi=========" << LumiWeight << endl;
	  GetGenWeight=genWeight;
	  int puNUmmc=int(puTrue->at(0)*10);
	  int puNUmdata=int(puTrue->at(0)*10);
	  float PUMC_=HistoPUMC->GetBinContent(puNUmmc+1);
	  float PUData_=HistoPUData->GetBinContent(puNUmdata+1);
	  if (PUMC_ ==0)
	    cout<<"PUMC_ is zero!!! & num pileup= "<< puTrue->at(0)<<"\n";
	  else
	    PUWeight= PUData_/PUMC_;
	  corrpu->SetBinContent(puNUmmc+1,HistoPUMC->GetBinContent(puNUmmc+1)*PUWeight);
      }
      pu->Fill(PUWeight);
	
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
      else if (Electron) {

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

	  if(elept && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle <0.1) {
            firstPart = false;
            vec_muele.push_back(iele);
          }
        }
      }

      else {
      }
      
      for  (int itau=0 ; itau < nTau; itau++) {
	
	bool antielemu = (Muon) ? tauByMVA6LooseElectronRejection->at(itau) !=0 && tauByTightMuonRejection3->at(itau) !=0 :
	   ((Electron) ? tauByMVA6TightElectronRejection->at(itau) !=0 && tauByLooseMuonRejection3->at(itau) !=0  : 
	    tauByMVA6TightElectronRejection->at(itau) !=0 && tauByTightMuonRejection3->at(itau) !=0);

	if(tauPt->at(itau) > 20 && fabs(tauEta->at(itau)) < 2.3 && antielemu && taupfTausDiscriminationByDecayModeFinding->at(itau) !=0) 
	  vec_tau.push_back(itau);
      }
      
      std::string part;
      double pt[2], eta[2],ene[2],chrg[2];
      float phi[2];
      if (Muon) {
	part = "#mu_";
	for (int k=0; k<2; ++k) {
	  pt[k]  = muPt->at(vec_muele[k]);
	  eta[k] = muEta->at(vec_muele[k]);
	  phi[k] = muPhi->at(vec_muele[k]);
	  ene[k] = muEn->at(vec_muele[k]);
	  chrg[k]= muCharge->at(vec_muele[k]);
	}
      } else if(Electron) {
	part = "#ele_";
	for (int k=0; k<2; ++k) {
	  pt[k]  = elePt->at(vec_muele[k]);
	  eta[k] = eleEta->at(vec_muele[k]);
	  phi[k] = elePhi->at(vec_muele[k]);
	  ene[k] = eleEn->at(vec_muele[k]);
	  chrg[k]= eleCharge->at(vec_muele[k]);
	}
      }

      else {
	part = "#mu_#ele";
        for (int k=0; k<2; ++k) {
          pt[k]  = (k==0) ? muPt->at(vec_muele[k]) : elePt->at(vec_muele[k]);
          eta[k] = (k==0) ? muEta->at(vec_muele[k]) : eleEta->at(vec_muele[k]);
          phi[k] = (k==0) ? muPhi->at(vec_muele[k]) : elePhi->at(vec_muele[k]);
          ene[k] = (k==0) ? muEn->at(vec_muele[k]) : eleEn->at(vec_muele[k]);
          chrg[k]= (k==0) ? muCharge->at(vec_muele[k]) : eleCharge->at(vec_muele[k]);
        }
      }

      bool charge(false), samesign(false), oppositesign(false);
      charge = chrg[0]*chrg[1] >0;
      samesign = charge ? true : false;
      oppositesign = charge ? false : true;
      bool mucharge[2] = {oppositesign, samesign};
      bool zerojet = vec_tau.size() ==0;
      bool onejet = vec_tau.size() ==1;
      bool twojet = vec_tau.size() ==2;
      bool jet[3] = {zerojet, onejet, twojet};
      std::string Jet[3] = {"zero_tau_jet_region", "one_tau_jet_region", "two_tau_jet_region"};
      
      TLorentzVector m1,m2,M;
      //      cout << phi[0] << "\t" << phi[1] << endl;
      m1.SetPtEtaPhiE(pt[0],eta[0],phi[0],ene[0]);
      m2.SetPtEtaPhiE(pt[1],eta[1],phi[1],ene[1]);
      //      cout << m1.Pt() << "\t" << m2.Pt() << "\t" << m1.Eta() << "\t" << m2.Eta() << "\t" << m1.Phi() << "\t" << m2.Phi() << endl;
      M= m1+m2;

      double leading_weight = 1;
      double subleading_weight = 1;
      double muonweight = 1;
      h_preweight->Fill(weight);

      if(!isData && Muon) {
	leading_weight = scaleFactor(pt[0],eta[0]);
	subleading_weight = scaleFactor(pt[1],eta[1]);
	h_SF1->Fill(leading_weight);
	h_SF2->Fill(subleading_weight);
	muonweight = leading_weight*subleading_weight;
	h_SF->Fill(muonweight);
      }
      leading_weight *= weight;
      subleading_weight *=weight;
      muonweight *=weight;
      h_postweight->Fill(muonweight);

      for(int i=0; i<2; i++) {
	if(!mucharge[i]) continue;
	std::string title = Charge[i]+"_and_no_cut_on_#tau_leg";
	if(M.M() >=20 && M.M() <=200) {
	  plotFill("InvariantMass_of_"+part+"pair_with_" +title+"_with_mass_20-200GeV",M.M(),90,20,200,muonweight);
	  plotFill("pt_distribution_of_Leading_"+part+title+"_with_mass_20-200GeV",pt[0],50,0,100,leading_weight);
          plotFill("pt_distribution_of_SubLeading_"+part+title+"_with_mass_20-200GeV",pt[1],50,0,100,subleading_weight);
          plotFill("eta_distribution_of_Leading_"+part+title+"_with_mass_20-200GeV",eta[0],20,-2.4,2.4,leading_weight);
          plotFill("eta_distribution_of_SubLeading_"+part+title+"_with_mass_20-200GeV",eta[1],20,-2.4,2.4,subleading_weight);
        }
	
	if(M.M() >=60 && M.M() <=120) {
	  plotFill("InvariantMass_of_"+part+"pair_with_" +title+"_with_mass_60-120GeV",M.M(),30,60,120,muonweight);
	  plotFill("pt_distribution_of_Leading_"+part+title+"_with_mass_60-120GeV",pt[0],50,0,100,leading_weight);
	  plotFill("pt_distribution_of_SubLeading_"+part+title+"_with_mass_60-120GeV",pt[1],50,0,100,subleading_weight);
	  plotFill("eta_distribution_of_Leading_"+part+title+"_with_mass_60-120GeV",eta[0],20,-2.4,2.4,leading_weight);
	  plotFill("eta_distribution_of_SubLeading_"+part+title+"_with_mass_60-120GeV",eta[1],20,-2.4,2.4,subleading_weight);
	}
	for(int j=0; j<3; j++ ){
	  if(!jet[j]) continue;
	  std::string title = Charge[i]+"_in_"+Jet[j];
	  plotFill("InvariantMass_of_"+part+"pair_with_" +title,M.M(),20,70,110,muonweight);
	}
      }

      if(vec_tau.size() >1) {
	h_musize->Fill(vec_muele.size());
	h_tausize->Fill(vec_tau.size());

	bool tau1antiiso = (tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[0]) ==0 &&
                            tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[1]) !=0
                            );

        bool tau2antiiso = (tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[1]) ==0 &&
                            tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[0]) !=0
                            );

        bool tau1antiisotau2antiiso = (tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[0]) ==0 &&
                                       tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[1]) ==0
                                       );

        bool tau1iso_tau2iso = (tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[1]) !=0 && tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[0]) !=0);


	double fake(1),fake1(1),fake2(1);

        if(tau1antiiso) {
          double f = fakeweight(tauPt->at(vec_tau[0]));
          fake1 *= f/(1-f);
	  fake *= fake1;
	}

        if(tau2antiiso) {
          double f = fakeweight(tauPt->at(vec_tau[1]));
          fake2 *= f/(1-f);
	  fake *= fake2;
	}

        if(tau1antiisotau2antiiso) {
          fake1 = fakeweight(tauPt->at(vec_tau[0]));
	  fake2 = fakeweight(tauPt->at(vec_tau[1]));
          fake *= fake1/(1-fake1)*fake2/(1-fake2);
        }

	if(!tau1antiiso && !tau2antiiso && !tau1iso_tau2iso && !tau1antiisotau2antiiso) continue;

	std::string tauiso = (tau1antiiso) ? "_in_tau1anti_iso" :
          (tau2antiiso ? "_in_tau2anti_iso" :
           (tau1iso_tau2iso ? "_in_tau1iso_tau2iso" : "_in_tau1antiso_tau2antiiso"));


	TLorentzVector t1,t2,T,HH;
	t1.SetPtEtaPhiE(tauPt->at(vec_tau[0]),tauEta->at(vec_tau[0]),tauPhi->at(vec_tau[0]),tauEnergy->at(vec_tau[0]));
	t2.SetPtEtaPhiE(tauPt->at(vec_tau[1]),tauEta->at(vec_tau[1]),tauPhi->at(vec_tau[1]),tauEnergy->at(vec_tau[1]));
	T= t1+t2;
	HH = T+M;
	charge = tauCharge->at(vec_tau[0])*tauCharge->at(vec_tau[1]) >0;
	samesign = charge ? true : false;
	oppositesign = charge ? false : true;
	bool taucharge[2] = {oppositesign, samesign};
	for(int i=0; i<2; i++) {
	  if(!mucharge[i]) continue;
	  for(int j=0; j<2; j++) {
	    if(!taucharge[j]) continue;
	    std::string title = muCharge_[i]+"_"+tauCharge_[j]+"_region";
	    plotFill("InvariantMass_of_tau_pair_with_"+title+tauiso,T.M(),6,50,110,weight*fake);
	    plotFill("pt_distribution_of_Leading_#tau_with_"+title+tauiso,tauPt->at(vec_tau[0]),75,0,150,weight*fake1);
	    plotFill("pt_distribution_of_SubLeading_#tau_with_"+title+tauiso,tauPt->at(vec_tau[1]),75,0,150,weight*fake2);
	    plotFill("eta_distribution_of_Leading_#tau_with_"+title+tauiso,tauEta->at(vec_tau[0]),20,-2.4,2.4,weight*fake1);
	    plotFill("eta_distribution_of_SubLeading_#tau_with_"+title+tauiso,tauEta->at(vec_tau[1]),20,-2.4,2.4,weight*fake2);
	  }
	}
	
	bool taumuCharge;
	taumuCharge = chrg[0]+chrg[1] + tauCharge->at(vec_tau[0])+tauCharge->at(vec_tau[1]) ==0;
	bool taumucharge_[2], mucharge_[0];
	taumucharge_[0] = taumuCharge ? true : false;
	taumucharge_[1] = taumuCharge ? false : true;

	for(int i=0; i<2; i++) {
	  if(!taumucharge_[i]) continue;
	  plotFill("InvariantMass_of_4_particle_with_"+totcharge[i]+tauiso,HH.M(),7,250,900,muonweight*fake);
	}
	if(taumucharge_[0]) {
	  bool muCharge_;
	  muCharge_ = chrg[0]+chrg[1]==0;
	  mucharge_[0] = muCharge_ ? true : false;
	  mucharge_[1] = muCharge_ ? false : true;

	  for(int i=0; i<2; i++) {
	    if(!mucharge_[i]) continue;
	    if(i==1 && mucharge_[1] == true && tau1iso_tau2iso) cout << "signal" << endl;
	    plotFill("InvariantMass_of_4_particle_with_"+taumucharge[i]+tauiso,HH.M(),7,250,900,muonweight*fake);
	  }
	}
	bool taumucharge;
	for(int i=0; i<2; i++) {
	  for(int j=0; j<2; j++) {
	    weight = (j==0) ? leading_weight : subleading_weight;
	    taumucharge = tauCharge->at(vec_tau[i]) + chrg[j] ==0;
	    std::string title = (taumucharge) ? "opposite_sign" : "same_sign";
	    TLorentzVector t, m, H;
	    t.SetPtEtaPhiE(tauPt->at(vec_tau[i]),tauEta->at(vec_tau[i]),tauPhi->at(vec_tau[i]),tauEnergy->at(vec_tau[i]));
	    m.SetPtEtaPhiE(pt[j],eta[j],phi[j],ene[j]);
	    H= t+m;
	    plotFill("InvariantMass_of_taumu_with_"+title+tauiso,H.M(),16,40,200,weight*fake);
	  }
	}
      }
    }

    //end of analysis code, close and write histograms/file

    fout->cd();
    pu->Write();
    corrpu->Write();
    h_musize->Write();
    h_tausize->Write();
    h_SF1->Write();
    h_SF2->Write();
    h_SF->Write();
    h_preweight->Write();
    h_postweight->Write();
    map<string, TH1F*>::const_iterator iMap1 = myMap1->begin();
    map<string, TH1F*>::const_iterator jMap1 = myMap1->end();
    for (; iMap1 != jMap1; ++iMap1) nplot1(iMap1->first)->Write();
    fout->Close();
  }
}  


float Cor80X_TRK_Mu_Full2016(float eta, TGraphAsymmErrors * graph ) {
  // take the ratio_eff_eta3_dr030e030_corr histogram (as function of eta).
  double * ipoint=NULL;
  double x_point,y_sf;
  ipoint=graph->GetX(); 
  if (eta >=(ipoint[0]-graph->GetErrorXlow(0))  && eta < (ipoint[0]+graph->GetErrorXhigh(0)) )  etaPOINT=0 ;
  else if (eta >= (ipoint[1]-graph->GetErrorXlow(1))  && eta < (ipoint[1]+graph->GetErrorXhigh(1)) ) etaPOINT=1;
  else if (eta >= (ipoint[2]-graph->GetErrorXlow(2)) && eta < (ipoint[2]+graph->GetErrorXhigh(2)) ) etaPOINT=2;
  else if (eta >= (ipoint[3]-graph->GetErrorXlow(3)) && eta < (ipoint[3]+graph->GetErrorXhigh(3)) ) etaPOINT=3;
  else if (eta >= (ipoint[4]-graph->GetErrorXlow(4)) && eta < (ipoint[4]+graph->GetErrorXhigh(4)) ) etaPOINT=4;
  else if (eta >= (ipoint[5]-graph->GetErrorXlow(5)) && eta < (ipoint[5]+graph->GetErrorXhigh(5)) ) etaPOINT=5;
  else if (eta >= (ipoint[6]-graph->GetErrorXlow(6)) && eta < (ipoint[6]+graph->GetErrorXhigh(6)) ) etaPOINT=6;
  else if (eta >= (ipoint[7]-graph->GetErrorXlow(7)) && eta < (ipoint[7]+graph->GetErrorXhigh(7)) ) etaPOINT=7;
  else if (eta >= (ipoint[8]-graph->GetErrorXlow(8)) && eta < (ipoint[8]+graph->GetErrorXhigh(8)) ) etaPOINT=8;
  else if (eta >= (ipoint[9]-graph->GetErrorXlow(9)) && eta < (ipoint[9]+graph->GetErrorXhigh(9)) ) etaPOINT=9;
  else if (eta >= (ipoint[10]-graph->GetErrorXlow(10)) && eta < (ipoint[10]+graph->GetErrorXhigh(10)) ) etaPOINT=10;
  else if (eta >= (ipoint[11]-graph->GetErrorXlow(11)) && eta < (ipoint[11]+graph->GetErrorXhigh(11)) ) etaPOINT=11;
  else if (eta >= (ipoint[12]-graph->GetErrorXlow(12)) && eta < (ipoint[12]+graph->GetErrorXhigh(12)) ) etaPOINT=12;
  else if (eta >= (ipoint[13]-graph->GetErrorXlow(13)) && eta < (ipoint[13]+graph->GetErrorXhigh(13)) ) etaPOINT=13;
  else if (eta >= (ipoint[14]-graph->GetErrorXlow(14)) && eta < (ipoint[14]+graph->GetErrorXhigh(14)) ) etaPOINT=14;
  else return 1;

  graph->GetPoint(etaPOINT,x_point,y_sf);
  return y_sf;

}


float CalcWeightedObj(float Obj_period1, float Obj_period2){
        
  float weighted_obj= (LumiBCDEF*Obj_period1+LumiGH*Obj_period2)/(LumiBCDEF+LumiGH);     
  return weighted_obj;
}

float getCorrFactorMuon80X(bool isData, float pt, float eta, TH2F ** HistoId, TH2F ** HistoIso, TGraphAsymmErrors * graph) {

  if (isData)
    return 1;
  else{
    float Weighted_IDSF=CalcWeightedObj(Cor80X_ID_Mu_BCDEF(pt,eta,HistoId[0]), Cor80X_ID_Mu_GH(pt,eta,HistoId[1]));
    float Weighted_IsoSF=CalcWeightedObj(Cor80X_Iso_Mu_BCDEF(pt,eta,HistoIso[0]), Cor80X_Iso_Mu_GH(pt,eta,HistoIso[1]));
    //    float Weighted_TriggerSF=CalcWeightedObj(Cor80X_Trigger_Mu_BCDEF(pt,eta,HistoTrg[0]), Cor80X_Trigger_Mu_GH(pt,eta,HistoTrg[1]));
    float Weighted_TriggerSF= 1;
    float Tracking_SF=Cor80X_TRK_Mu_Full2016(eta, graph);
    return (Weighted_IDSF * Weighted_IsoSF * Tracking_SF * Weighted_TriggerSF);
  }
}

float Cor80X_Iso_Mu_BCDEF(float pt,float eta, TH2F* HistoIso){
  
  if (pt >= 20 && pt < 25 ) ptBIN=1;
  if (pt >= 25 && pt < 30 ) ptBIN=2;
  if (pt >= 30 && pt < 40 ) ptBIN=3;
  if (pt >= 40 && pt < 50 ) ptBIN=4;
  if (pt >= 50 && pt < 60 ) ptBIN=5;
  if (pt >= 60  ) ptBIN=6;
  
  
  if (fabs(eta) <= 0.9) etaBIN=1;
  if (0.9 <= fabs(eta) && fabs(eta) < 1.2) etaBIN=2;
  if (1.2 <= fabs(eta) && fabs(eta) < 2.1) etaBIN=3;
  if (2.1 <= fabs(eta) && fabs(eta) < 2.4) etaBIN=4;
  
  //    cout<< "--->Muon Iso    pt= "<<pt<<  "   eta" <<eta<< "  SF="<<HistoIso->GetBinContent(ptBIN,etaBIN)<<"\n";
  return HistoIso->GetBinContent(ptBIN,etaBIN);
}


float Cor80X_Iso_Mu_GH(float pt,float eta, TH2F* HistoIso){
  
  if (pt >= 20 && pt < 25 ) ptBIN=1;
  if (pt >= 25 && pt < 30 ) ptBIN=2;
  if (pt >= 30 && pt < 40 ) ptBIN=3;
  if (pt >= 40 && pt < 50 ) ptBIN=4;
  if (pt >= 50 && pt < 60 ) ptBIN=5;
  if (pt >= 60 ) ptBIN=6;

  if (fabs(eta) <= 0.9) etaBIN=1;
  if (0.9 <= fabs(eta) && fabs(eta) < 1.2) etaBIN=2;
  if (1.2 <= fabs(eta) && fabs(eta) < 2.1) etaBIN=3;
  if (2.1 <= fabs(eta) && fabs(eta) < 2.4) etaBIN=4;
  
  //    cout<< "--->Muon Iso    pt= "<<pt<<  "   eta" <<eta<< "  SF="<<HistoIso->GetBinContent(ptBIN,etaBIN)<<"\n";
  return HistoIso->GetBinContent(ptBIN,etaBIN);
}


float Cor80X_ID_Mu_BCDEF(float pt, float eta , TH2F * HistoId) {
  
  if (pt >= 20 && pt < 25 ) ptBIN=1;
  if (pt >= 25 && pt < 30 ) ptBIN=2;
  if (pt >= 30 && pt < 40 ) ptBIN=3;
  if (pt >= 40 && pt < 50 ) ptBIN=4;
  if (pt >= 50 && pt < 60 ) ptBIN=5;
  if (pt >= 60  ) ptBIN=6;
  
  
  if (fabs(eta) <= 0.9) etaBIN=1;
  if (0.9 <= fabs(eta) && fabs(eta) < 1.2) etaBIN=2;
  if (1.2 <= fabs(eta) && fabs(eta) < 2.1) etaBIN=3;
  if (2.1 <= fabs(eta) && fabs(eta) < 2.4) etaBIN=4;


  //       cout<< "--->Muon Id    pt= "<<pt<<  "   eta" <<eta<< "  SF="<<HistoId->GetBinContent(ptBIN,etaBIN)<<"\n";
  return HistoId->GetBinContent(ptBIN,etaBIN);
}
    

float Cor80X_ID_Mu_GH(float pt, float eta , TH2F * HistoId) {

  if (pt >= 20 && pt < 25 ) ptBIN=1;
  if (pt >= 25 && pt < 30 ) ptBIN=2;
  if (pt >= 30 && pt < 40 ) ptBIN=3;
  if (pt >= 40 && pt < 50 ) ptBIN=4;
  if (pt >= 50 && pt < 60 ) ptBIN=5;
  if (pt >= 60  ) ptBIN=6;
    

  if (fabs(eta) <= 0.9) etaBIN=1;
  if (0.9 <= fabs(eta) && fabs(eta) < 1.2) etaBIN=2;
  if (1.2 <= fabs(eta) && fabs(eta) < 2.1) etaBIN=3;
  if (2.1 <= fabs(eta) && fabs(eta) < 2.4) etaBIN=4;


  //    cout<< "--->Muon Id    pt= "<<pt<<  "   eta" <<eta<< "  SF="<<HistoId->GetBinContent(ptBIN,etaBIN)<<"\n";
  return HistoId->GetBinContent(ptBIN,etaBIN);
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

double fakeweight(double pt)  {

  //  TF1 *f = new TF1("fa2","TMath::Exp(-0.019*x)",0,150); 
  //   return TMath::Exp(-0.019*pt);
  //  [2]+[0]*TMath::Exp([1]*x 
  //  return 0.196+.229*TMath::Exp(-.0648*pt);                                                                                                 
  return .0196+.229*TMath::Exp(-.0648*pt);
}
