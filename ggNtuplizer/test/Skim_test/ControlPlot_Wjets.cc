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



TFile * HZZ= TFile::Open("ScaleFactors_mu_Moriond2017_v2.root");
TH2F* SF = (TH2F*) HZZ->Get("FINAL");

double fakeweight(double pt, string type);

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
  string type = input[0];

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
    output += "_controlPlottest.root";
    size_t str = output.rfind("/");
    output.erase(0,str+1);
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
    double f1(0), f2(0);
    for ( Int_t i = 0; i < nentries_wtn; i++) {
      // for ( Int_t i = 0; i < 10; i++) {
      
    //      for ( Int_t i = 200000; i < ; i++) { 
      Run_Tree->GetEntry(i);

      if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
    
      bool PassTrigger =   (Muon) ? ((HLTEleMuX >> 19 & 1) || (HLTEleMuX >> 20 & 1)) : ( Electron ? (HLTEleMuX >> 0 & 1) : (HLTEleMuX >> 5 & 1));// || (HLTEleMuX >> 19 & 1) || (HLTEleMuX >> 20 & 1)) : (HLTEleMuX >> 34 & 1);
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

      /*      if(input[k].find("DY1Jets") != string::npos) num_gen_jets =1;
      if(input[k].find("DY2Jets") != string::npos) num_gen_jets=2;
      if(input[k].find("DY3Jets") != string::npos) num_gen_jets=3;
      if(input[k].find("DY4Jets") != string::npos) num_gen_jets=4;
      if(input[k].find("W1Jets") != string::npos) num_gen_jets=1;
      if(input[k].find("W2Jets") != string::npos) num_gen_jets=2;
      if(input[k].find("W3Jets") != string::npos) num_gen_jets=3;
      if(input[k].find("W4Jets") != string::npos) num_gen_jets=4;*/
      //cout << num_gen_jets << endl;

      if (!isData) {
	if (HistoTot) LumiWeight = weightCalc(HistoTot, input[k], num_gen_jets, W_events, DY_events);
	//	cout << "HistoTot = " << HistoTot << "\t" << input[k] << "\t" << num_gen_jets << "\t" << "lumi=========" << LumiWeight << endl;
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
      std::vector<int> vec_muele, vec_tau, vec_tauiso;

      if(Muon) {

	for  (int imu=0 ; imu < nMu; imu++){
	  bool mupt = firstPart ? (muPt->at(imu) >26) : (muPt->at(imu) >25);
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

	  bool elept = (firstPart) ? (elePt->at(iele) >26) : (elePt->at(iele) >15);

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

      else {
      }
      
      if(vec_muele.size() !=1) continue;

      float MuMetTranverseMass= (Muon) ? TMass_FNew(muPt->at(vec_muele[0]), muPhi->at(vec_muele[0]), pfMET, pfMETPhi) : TMass_FNew(elePt->at(vec_muele[0]), elePhi->at(vec_muele[0]), pfMET, pfMETPhi);

      //if(MuMetTranverseMass <50) continue;
      if(MuMetTranverseMass >40) continue;



      bool eleveto(false);

      if(Muon) {
	for (int iele = 0; iele < nEle; ++iele) {
	  
	  float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
	  if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
	    IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);
	  
	  bool eleMVAId= false;
	  if (fabs (eleSCEta->at(iele)) <= 0.8 && eleIDMVA->at(iele) > 0.837) eleMVAId= true;
	  else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <=  1.5 && eleIDMVA->at(iele) > 0.715) eleMVAId= true;
	  else if ( fabs (eleSCEta->at(iele)) >=  1.5 && eleIDMVA->at(iele) > 0.357 ) eleMVAId= true;
	  else eleMVAId= false;
	  
	  if(elePt->at(iele) >15 && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle <0.1) {
	    eleveto = true;
	    break;
	  }
	}
      }
      if(Muon && eleveto) continue;

      
      bool muveto(false);
      if(!Muon) {
	for(int imu=0; imu < nMu; imu++) {
	  
	  UShort_t id = (muIDbit->at(imu) >> 1 & 1);
	  
	  float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
	  if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
	    IsoMu= ( muPFChIso->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
	  
	  if(muPt->at(imu) >9 && fabs(muEta->at(imu)) <2.4 && id && IsoMu <0.3) {
	    muveto = true;
	    break;
	  }
	}
      }
      if(!Muon && muveto) continue;


      for  (int itau=0 ; itau < nTau; itau++) {
	
	bool antielemu = (Muon) ? tauByMVA6LooseElectronRejection->at(itau) !=0 && tauByTightMuonRejection3->at(itau) !=0 :
	   ((Electron) ? tauByMVA6TightElectronRejection->at(itau) !=0 && tauByLooseMuonRejection3->at(itau) !=0  : 
	    tauByMVA6TightElectronRejection->at(itau) !=0 && tauByTightMuonRejection3->at(itau) !=0);

	if(tauPt->at(itau) > 20 && fabs(tauEta->at(itau)) < 2.3 && antielemu && taupfTausDiscriminationByDecayModeFinding->at(itau) !=0) 
	  vec_tau.push_back(itau);
	if(tauPt->at(itau) > 20 && fabs(tauEta->at(itau)) < 2.3 && antielemu && taupfTausDiscriminationByDecayModeFinding->at(itau) !=0 && 
	   tauByLooseIsolationMVArun2v1DBoldDMwLT->at(itau) !=0)
          vec_tauiso.push_back(itau);
      }
      std::string part;
      std::vector<double> pt, eta,ene,chrg,phi;


      if (Muon) {
	part = "#mu_";
	for (int imu=0; imu<vec_muele.size(); ++imu) {
	  pt.push_back(muPt->at(vec_muele[imu]));
	  eta.push_back(muEta->at(vec_muele[imu]));
	  phi.push_back(muPhi->at(vec_muele[imu]));
	  ene.push_back(muEn->at(vec_muele[imu]));
	  chrg.push_back(muCharge->at(vec_muele[imu]));
	}
      } else if(Electron) {
	part = "#ele_";
	for (int iele=0; iele<vec_muele.size(); ++iele) {
	  pt.push_back(elePt->at(vec_muele[iele]));
	  eta.push_back(eleEta->at(vec_muele[iele]));
	  phi.push_back(elePhi->at(vec_muele[iele]));
	  ene.push_back(eleEn->at(vec_muele[iele]));
	  chrg.push_back(eleCharge->at(vec_muele[iele]));
	}
      }

      else {
	part = "#mu_#ele";
        for (int imuele=0; imuele<vec_muele.size(); ++imuele) {
          double pt_  = (k==0) ? muPt->at(vec_muele[k]) : elePt->at(vec_muele[k]);
	  pt.push_back(pt_);
          double eta_ = (k==0) ? muEta->at(vec_muele[k]) : eleEta->at(vec_muele[k]);
	  eta.push_back(eta_);
          double phi_ = (k==0) ? muPhi->at(vec_muele[k]) : elePhi->at(vec_muele[k]);
	  phi.push_back(phi_);
          double ene_ = (k==0) ? muEn->at(vec_muele[k]) : eleEn->at(vec_muele[k]);
	  ene.push_back(ene_);
          double chrg_ = (k==0) ? muCharge->at(vec_muele[k]) : eleCharge->at(vec_muele[k]);
	  chrg.push_back(chrg_);
        }
      }
      
      /*      bool charge(false), samesign(false);
      charge = chrg[0]*chrg[1] >0;
      samesign = charge ? true : false;
      bool mucharge[2] = {!samesign, samesign};
      bool zerojet = vec_tauiso.size() ==0;
      bool onejet = vec_tauiso.size() ==1;
      bool twojet = vec_tauiso.size() ==2;
      bool jet[3] = {zerojet, onejet, twojet};
      std::string Jet[3] = {"zero_tau_jet_region", "one_tau_jet_region", "two_tau_jet_region"};
      
      TLorentzVector m1,m2,M;
      m1.SetPtEtaPhiE(pt[0],eta[0],phi[0],ene[0]);
      m2.SetPtEtaPhiE(pt[1],eta[1],phi[1],ene[1]);
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

      h_postweight->Fill(muonweight);*/

      if(vec_tau.size() >1) {
	
	
	TLorentzVector t1,t2,T,HH;
	
	for(int itau=0; itau<vec_tau.size(); itau++) {
	  
	  t1.SetPtEtaPhiE(tauPt->at(vec_tau[itau]),tauEta->at(vec_tau[itau]),tauPhi->at(vec_tau[itau]),tauEnergy->at(vec_tau[itau]));

	  
	  for(int jtau=itau+1; jtau<vec_tau.size(); jtau++) {

	    t2.SetPtEtaPhiE(tauPt->at(vec_tau[jtau]),tauEta->at(vec_tau[jtau]),tauPhi->at(vec_tau[jtau]),tauEnergy->at(vec_tau[jtau]));
	    if(tauCharge->at(itau)*tauCharge->at(jtau) >0) continue;
	    
	    double deltaR  = dR_(tauEta->at(vec_tau[itau]), tauPhi->at(vec_tau[itau]), tauEta->at(vec_tau[jtau]), tauPhi->at(vec_tau[jtau]));
	    if(deltaR <0.5) continue;
	    
	    bool tau1antiiso = (tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[itau]) ==0 &&
				tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[jtau]) !=0
				);
	    
	    bool tau2antiiso = (tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[jtau]) ==0 &&
				tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[itau]) !=0
				);
	    
	    bool tau1antiisotau2antiiso = (tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[itau]) ==0 &&
					   tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[jtau]) ==0
					   );
	    
	    bool tau1iso_tau2iso = (tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[itau]) !=0 && tauByLooseIsolationMVArun2v1DBoldDMwLT->at(vec_tau[jtau]) !=0);
	    
	    double fake(1),fake1(1),fake2(1), fake12(1);
	    
	    if(tau1antiiso) {
	      double f = fakeweight(tauPt->at(vec_tau[itau]),type);
	      fake1 *= f/(1-f);
	      fake *= fake1;
	    }
	    
	    if(tau2antiiso) {
	      double f = fakeweight(tauPt->at(vec_tau[jtau]),type);
	      fake2 *= f/(1-f);
	      fake *= fake2;
	    }
	    
	    if(tau1antiisotau2antiiso) {
	      fake1 = fakeweight(tauPt->at(vec_tau[itau]),type)/(1-fakeweight(tauPt->at(vec_tau[itau]),type));
	      fake2 = fakeweight(tauPt->at(vec_tau[jtau]),type)/(1-fakeweight(tauPt->at(vec_tau[jtau]),type));
	      fake *= fake1*fake2;
	    }

	    if(!tau1antiiso && !tau2antiiso && !tau1iso_tau2iso && !tau1antiisotau2antiiso) continue;
	    
	    std::string tauiso = (tau1antiiso) ? "_in_tau1anti_iso" :
	      (tau2antiiso ? "_in_tau2anti_iso" :
	       (tau1iso_tau2iso ? "_in_tau1iso_tau2iso" : "_in_tau1antiso_tau2antiiso"));
	    
	    
	    T= t1+t2;
	    TLorentzVector m1;
	    
	    for(int imu=0; imu<vec_muele.size(); imu++) {
	      //	      deltaR = dR_(muEta->at(vec_muele[imu]),muPhi->at(vec_muele[imu]),tauEta->at(vec_tau[itau]),tauPhi->at(vec_tau[itau]));
	      deltaR = dR_(eta[imu],phi[imu],tauEta->at(vec_tau[itau]),tauPhi->at(vec_tau[itau]));
	      if(deltaR <0.5) continue;

	      //	      deltaR = dR_(muEta->at(vec_muele[imu]),muPhi->at(vec_muele[imu]),tauEta->at(vec_tau[jtau]),tauPhi->at(vec_tau[jtau]));
	      deltaR = dR_(eta[imu],phi[imu],tauEta->at(vec_tau[jtau]),tauPhi->at(vec_tau[jtau]));
	      if(deltaR <0.5) continue;

	      //	      m1.SetPtEtaPhiE(muPt->at(vec_muele[imu]),muEta->at(vec_muele[imu]),muPhi->at(vec_muele[imu]),muEn->at(vec_muele[imu]));
	      m1.SetPtEtaPhiE(pt[imu],eta[imu],phi[imu],ene[imu]); 
	      HH = T+m1;
	      
	      plotFill("InvariantMass_of_3_particle"+tauiso,HH.M(),18,20,200,weight*fake);
	      plotFill("InvariantMass_of_3_particle"+tauiso+"_nofake_weight",HH.M(),18,20,200,weight);
	      TLorentzVector H;
	      for(int i=0; i<2; i++) {
		TLorentzVector tau = (i==0) ? t1 : t2;
		H = tau+m1;
		plotFill("InvariantMass_of_taumu"+tauiso,H.M(),18,20,200,weight*fake);
		plotFill("InvariantMass_of_taumu"+tauiso+"_nofake_weight",H.M(),18,20,200,weight);
	      }
	    }
	  }
	}
      }
    }
    //end of analysis code, close and write histograms/file
    fout->cd();
    map<string, TH1F*>::const_iterator iMap1 = myMap1->begin();
    map<string, TH1F*>::const_iterator jMap1 = myMap1->end();
    for (; iMap1 != jMap1; ++iMap1) nplot1(iMap1->first)->Write();
    fout->Close();
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

double fakeweight(double pt, string type)  {

  //  TF1 *f = new TF1("fa2","TMath::Exp(-0.019*x)",0,150); 
  //   return TMath::Exp(-0.019*pt);
  //  [2]+[0]*TMath::Exp([1]*x 
  //  return 0.196+.229*TMath::Exp(-.0648*pt);                                                                                                 
  //  return .0196+.229*TMath::Exp(-.0648*pt);
  //  return .0194+.232*TMath::Exp(-.0653*pt);///L
  //  return .036+.477*TMath::Exp(-.070*pt);///VL
  //  return .002+.153*TMath::Exp(-.085*pt);
  //  return .012+.632*TMath::Exp(-.101*pt);///QCD
  //  return .009+.201*TMath::Exp(-.064*pt);///Wjet
  if(type =="Muon") {
    //    return .015+.220*TMath::Exp(-.063*pt);///MuWjets
    return .016+.244*TMath::Exp(-.066*pt);
  }

  else{
      return .01365+.33600*TMath::Exp(-.08399*pt);///EleWjets
    //    return .015+.220*TMath::Exp(-.063*pt);
  }
  
}
