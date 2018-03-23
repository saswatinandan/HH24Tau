//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Compiling the code:   ./Make.sh ZTT_XSection.cc
//   Running the code:     ./ZTT_XSection.exe OutPut.root   Input.root
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "TreeReader1.h"
#include "WeightCalculator.h"
#include "TLorentzVector.h"
#include <string> //remove space for successful compilation
#include <ostream> //remove space for successful compilation
#include <iostream>
#include <iomanip>
#include <sstream>



TFile * HZZ= TFile::Open("ScaleFactors_mu_Moriond2017_v2.root");
TH2F* SF = (TH2F*) HZZ->Get("FINAL");
bool taumatch(double eta,double phi, std::vector<TLorentzVector>& taugen);
std::pair<double,double> tauPtE(double taupt, double tauenergy,int sys);
double fakeweight(double pt);
void makedir(TFile* f, std::string dir);
float chargemisid( double pt, double eta);
float chargemisid (double eta) ;
int  main(int argc, char** argv) {
  using namespace std;

  std::vector<std::string> input;
  for (int f = 1; f < argc; f++) {
    input.push_back(*(argv + f));

    cout << "\n INPUT NAME IS:   " << input[f - 1] << "\n";
  }

  if(input[0] != "Muon" && input[0] != "Electron" && input[0] != "MuElectron") {
    cout << "*************upppsssss************** pls type either Muon or Electron or MuElectron"   << endl;
    return 1;
  }
  bool Muon = (input[0]=="Muon");
  bool Electron = (input[0] == "Electron");
  bool Muelectron = (input[0] == "MuElectron");

  std::vector <float> W_events = W_EvenetMultiplicity();
  std::vector <float> DY_events = DY_EvenetMultiplicity();

  std::string output = "chargemisidtest.root";
  cout << "\n\n\n OUTPUT NAME IS:    " << output << endl;
  TFile *fout = TFile::Open(output.c_str(), "Update");
  myMap1 = new std::map<std::string, map<string, TH1F*>* >();

  for(int k=1; k<input.size(); k++ ) {

    TFile *f_Double = TFile::Open(input[k].c_str());
    cout << "\n  Now is running on ------->   " << std::string(f_Double->GetName()) << "\n";

    TFile * myFile = TFile::Open(f_Double->GetName());
    TH1F * HistoTot = (TH1F*) myFile->Get("hcount");
    
    //TTree *Run_Tree = (TTree*) f_Double->Get("ggNtuplizer/EventTree");
    TTree *Run_Tree = (TTree*) f_Double->Get("EventTree");   


    std::string sample = input[k];
    size_t str = sample.rfind("/");
    sample.erase(0,str+1);
    size_t root = sample.find("_EE.root");
    sample.erase(root);
    /*output += "chargemisid.root";
    cout << "\n\n\n OUTPUT NAME IS:    " << output << endl;     //PRINTING THE OUTPUT FILE NAME
    TFile *fout = TFile::Open(output.c_str(), "RECREATE");*/

    TLorentzVector e1, e1gen, e2, e2gen, M;

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
    Run_Tree->SetBranchAddress("muBestTrkPtError", &muBestTrkPtError);
    Run_Tree->SetBranchAddress("muBestTrkPt", &muBestTrkPt);
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
    Run_Tree->SetBranchAddress("eleChargeConsistent",&eleChargeConsistent);
    Run_Tree->SetBranchAddress("ele2ndChargeConsistent",&ele2ndChargeConsistent);
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
    Run_Tree->SetBranchAddress("isPVGood", &isPVGood);
    
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

    for ( Int_t i =0; i < nentries_wtn; i++) {
      
      //  for ( Int_t i =0; i < 1000; i++) { 
      Run_Tree->GetEntry(i);

      if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);

      if(!isPVGood) continue;
      bool mutrigger = (HLTEleMuX >> 14 & 1) || (HLTEleMuX >> 15 & 1);
      bool eletrigger = (HLTEleMuX >> 5 & 1);
      bool mueletrigger = ((HLTEleMuX >> 41 & 1) || (HLTEleMuX >> 42 & 1)) && (!(HLTEleMuX >> 14 & 1) && !(HLTEleMuX >> 15 & 1));
      //      bool PassTrigger =   (Muon) ? mutrigger : ( Electron ? eletrigger : mueletrigger);
      bool PassTrigger =   (HLTEleMuX >> 5 & 1);
      if(!PassTrigger) continue;                                       
      int count_bjet(0);
      for (int ijet= 0 ; ijet < nJet ; ijet++){
	if (jetPt->at(ijet) > 20 && fabs(jetEta->at(ijet)) < 2.5 && jetCSV2BJetTags->at(ijet) > 0.800){
	  count_bjet +=1;
	  break;
	}
      }
      if(count_bjet) continue; 
      
      float LumiWeight = 1;
      float GetGenWeight=1;
      float PUWeight = 1;

      if (!isData) {
	if (HistoTot) LumiWeight = weightCalc(HistoTot, input[k], num_gen_jets, W_events, DY_events);
	//	  cout << "lumi=========" << LumiWeight << input[k] << endl;
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
      

      bool b1(false),b2(false),bh1(false),bh2(false),bm1(false),bm2(false),en1(false),en2(false),el1(false),el2(false),em1(false),em2(false), genele1(false), genele2(false), masscut(false);
      int e1indx(-1), e2indx(-1);
      for (int iele = 0; iele < nEle; ++iele) {

	//	bl1 = bh1 = bm1 = eh1 = el1 = em1 = genele1 = false;
	b1 = en1 = false;
	if((elePt->at(iele) <=24) || (fabs(eleEta->at(iele)) >=2.5 )) continue;

	  float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
	  if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
	    IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);

	  if(IsoEle >= 0.25) continue;
	  bool eleMVAId= false;
	  if (fabs (eleSCEta->at(iele)) <= 0.8 && eleIDMVA->at(iele) > 0.837) eleMVAId= true;
	  else if (fabs (eleSCEta->at(iele)) >  0.8 &&fabs (eleSCEta->at(iele)) <=  1.5 && eleIDMVA->at(iele) > 0.715) eleMVAId= true;
	  else if ( fabs (eleSCEta->at(iele)) >=  1.5 && eleIDMVA->at(iele) > 0.357 ) eleMVAId= true;
	  else eleMVAId= false;
	  if(!eleMVAId) continue;

	  e1.SetPtEtaPhiE(elePt->at(iele),eleEta->at(iele),elePhi->at(iele),eleEn->at(iele));


	  for(int imc=0; imc<nMC; imc++) {
	    if(fabs(mcPID->at(imc)) !=11 || mcMomPID->at(imc) !=23) continue;
	    e1gen.SetPtEtaPhiE(mcPt->at(imc),mcEta->at(imc),mcPhi->at(imc),mcE->at(imc));
	    float deltar = e1gen.DeltaR(e1);
	    float deltapt = (fabs(mcPt->at(imc)-elePt->at(iele))/mcPt->at(imc));
	    if(deltar <0.5 && deltapt <0.5) {
	      genele1 = true;
	      break;
	    }
	  }

	  //	  if(elePt->at(iele) <25) {
	  if(fabs(eleEta->at(iele)) <=1.479) {
	    b1 = true;
	  }
	  else if(fabs(eleEta->at(iele)) >1.479 && fabs(eleEta->at(iele)) <2.5) {
	    en1 = true;
	  }
	  //}
	  //else if(elePt->at(iele) >=25 && elePt->at(iele) <50) {
	  /*if(fabs(eleEta->at(iele)) <=1.479) {
	      bm1 = true;
            }
            else if(fabs(eleEta->at(iele)) >1.479 && fabs(eleEta->at(iele)) <2.5) {
              em1 = true;
            }
          }
	  
	  else {
	    if(fabs(eleEta->at(iele)) <=1.479) {
	      bh1 = true;
	    }
	    else if(fabs(eleEta->at(iele)) >1.479 && fabs(eleEta->at(iele)) <2.5) {
	      eh1 = true;
	    }
	    }*/
	  
	  for (int jele = iele+1; jele < nEle; ++jele) {

	    //	    bl2 = bm2 = bh2 = el2 = em2 = eh2 = genele2 = false;
	    b2 = en2 = false;

	    if((elePt->at(jele) <13) || (fabs(eleEta->at(jele)) >2.5 )) continue;

	    float IsoEle=elePFChIso->at(jele)/elePt->at(jele);
	    if ( (elePFNeuIso->at(jele) + elePFPhoIso->at(jele) - 0.5* elePFPUIso->at(jele))  > 0.0)
	      IsoEle= (elePFChIso->at(jele) + elePFNeuIso->at(jele) + elePFPhoIso->at(jele) - 0.5* elePFPUIso->at(jele))/elePt->at(jele);

	    if(IsoEle >= 0.25) continue;
	    bool eleMVAId= false;
	    if (fabs (eleSCEta->at(jele)) <= 0.8 && eleIDMVA->at(jele) > 0.837) eleMVAId= true;
	    else if (fabs (eleSCEta->at(jele)) >  0.8 &&fabs (eleSCEta->at(jele)) <=  1.5 && eleIDMVA->at(jele) > 0.715) eleMVAId= true;
	    else if ( fabs (eleSCEta->at(jele)) >=  1.5 && eleIDMVA->at(jele) > 0.357 ) eleMVAId= true;
	    else eleMVAId= false;
	    if(!eleMVAId) continue;
	    e2.SetPtEtaPhiE(elePt->at(jele),eleEta->at(jele),elePhi->at(jele),eleEn->at(jele));

	    for(int imc=0; imc<nMC; imc++) {
	      if(fabs(mcPID->at(imc)) !=11 || mcMomPID->at(imc) !=23) continue;
	      e2gen.SetPtEtaPhiE(mcPt->at(imc),mcEta->at(imc),mcPhi->at(imc),mcE->at(imc));
	      float deltar = e2gen.DeltaR(e2);
	      float deltapt = (fabs(mcPt->at(imc)-elePt->at(jele))/mcPt->at(imc));
	      if(deltar <0.5 && deltapt <0.5) {
		genele2 = true;
		break;
	      }
	    }

	    //if(elePt->at(jele) <25) {
	    if(fabs(eleEta->at(jele)) <=1.479) {
		b2 = true;
	    }
	    else if(fabs(eleEta->at(jele)) >1.479 && fabs(eleEta->at(jele)) <2.5) {
	      en2 = true;
	    }
	    //	    }
	    /*	    else if(elePt->at(jele) >=25 && elePt->at(jele) <50) {
	      if(fabs(eleEta->at(jele)) <=1.479) {
		bm2 = true;
	      }
	      else if(fabs(eleEta->at(jele)) >1.479 && fabs(eleEta->at(jele)) <2.5) {
		em2 = true;
	      }
	    }

	    else {
	      if(fabs(eleEta->at(jele)) <=1.479) {
		bh2 = true;
	      }
	      else if(fabs(eleEta->at(jele)) >1.479 && fabs(eleEta->at(jele)) <2.5) {
		eh2 = true;
	      }
	      }*/

	    M = e1+e2;
	    if(60 < M.M() < 120) {
	      e1indx = iele;
	      e2indx = jele;
	      masscut = true;
	      break;
	    }
	  }
	  if(masscut) break;
      }
      
      if(!masscut) continue;

      bool eleveto(false);
      
      for (int iele = 0; iele < nEle; ++iele) {
	
	if(iele == e1indx || iele == e2indx) continue;
	float IsoEle=elePFChIso->at(iele)/elePt->at(iele);
	if ( (elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))  > 0.0)
	  IsoEle= (elePFChIso->at(iele) + elePFNeuIso->at(iele) + elePFPhoIso->at(iele) - 0.5* elePFPUIso->at(iele))/elePt->at(iele);
	
	bool eleMVAId= false;
	if (fabs (eleSCEta->at(iele)) <= 0.8 && eleIDMVA->at(iele) > 0.837) eleMVAId= true;
	else if (fabs (eleSCEta->at(iele)) >  0.8 && fabs (eleSCEta->at(iele)) <=  1.5 && eleIDMVA->at(iele) > 0.715) eleMVAId= true;
	else if ( fabs (eleSCEta->at(iele)) >=  1.5 && eleIDMVA->at(iele) > 0.357 ) eleMVAId= true;
	else eleMVAId= false;
	
	if(elePt->at(iele) >13 && eleMVAId && fabs(eleEta->at(iele)) <2.5 && IsoEle <0.25) {
	  eleveto = true;
	  break;
	}
      }
      
      if(eleveto) continue;
      
      bool muveto(false);
      
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
      
      if(muveto) continue;
      
      
      bool samesign = eleCharge->at(e1indx) * eleCharge->at(e2indx) >0;
      std::string type = (isData) ? "Data" : ((genele1 && genele2) ? ((sample.find("DY") != string::npos) ? "DY_Zee" : sample+"_Zee") : sample) ;
      
      std::string title = (samesign) ? "_SS" : "_OS";

      float chargemisidfake(1.);
      if(!isData) {
        chargemisidfake = chargemisid(eleEta->at(e1indx));
	//cout << "pt1= " << elePt->at(e1indx) << "\t" << "eta1= " << eleEta->at(e1indx) ;
	chargemisidfake += chargemisid(eleEta->at(e2indx));
	//cout << "\t" << "pt2= " << elePt->at(e2indx) << "\t" << "eta2= " << eleEta->at(e2indx) << "\t" << "weight= " << chargemisidfake << endl;
      }

      plotFill("inclusive"+title,type,M.M(),30,60,120,weight);
      if(b1 && b2) {
	plotFill("BB"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("BB"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2 && title.find("OS") != string::npos) {
	  weight *=  chargemisidfake;
	  plotFill("BB"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      /*if((bl1 && bm2) || (bm1 && bl2)) {
	plotFill("BB_ML"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("BB_ML"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *=chargemisidfake;
	  plotFill("BB_ML"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
	
      }
      else if (bm1 && bm2) {
	plotFill("BB_MM"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("BB_MM"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *=  chargemisidfake;
	  plotFill("BB_MM"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      else if((bh1 && bl2) || (bl1 && bh2)) {
	plotFill("BB_HL"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("BB_HL"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *=  chargemisidfake;
	  plotFill("BB_HL"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      else if((bh1 && bm2) || (bm1 && bh2)) {
	plotFill("BB_HM"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("BB_HM"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *= chargemisidfake;
	  plotFill("BB_HM"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      else if(bh1 && bh2) {
	plotFill("BB_HH"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("BB_HH"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *= chargemisidfake;
	  plotFill("BB_HH"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
	}*/
      else if(en1 && en2) {
	plotFill("EE"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("EE"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *= chargemisidfake;
	  plotFill("EE"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      /*else if((el1 && em2) || (em1 && el2)) {
	plotFill("EE_ML"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("EE_ML"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *= chargemisidfake;
	  plotFill("EE_ML"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      else if(em1 && em2) {
	plotFill("EE_MM"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("EE_MM"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *= chargemisidfake;
	  plotFill("EE_MM"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      else if((el1 && eh2) || (eh1 && el2)) {
	plotFill("EE_HL"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("EE_HL"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *=  chargemisidfake;
	  plotFill("EE_HL"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      else if((em1 && eh2) || (em2 && eh1)) {
	plotFill("EE_HM"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("EE_HM"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *= chargemisidfake;
	  plotFill("EE_HM"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      
      else if (eh1 && eh2) {
	plotFill("EE_HH"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("EE_HH"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *= chargemisidfake;
	  plotFill("EE_HH"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
	}*/
      else if ((b1 && en2)) {// || (bl2 && el1)) {
	plotFill("BE"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("BE"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *=  chargemisidfake;
	  plotFill("BE"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }

      else if ((en1 && b2)) {// || (bl2 && el1)) {                                                                                                                                                           
        plotFill("EB"+title,type,M.M(),30,60,120,weight);
        if(!isData) plotFill("EB"+title,"pseudodata",M.M(),30,60,120,weight);
        if(genele1 && genele2  && title.find("OS") != string::npos) {
          weight *=  chargemisidfake;
          plotFill("EB"+title,type+"_chargemisid",M.M(),30,60,120,weight);
        }
      }


      /*      else if ((bl1 && em2) ||(bl2 && em1)) {
	plotFill("EB_ML"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("EB_ML"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *= chargemisidfake;
	  plotFill("EB_ML"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      else if ((bm1 && el2) || (bm2 && el1)) {
	plotFill("BE_ML"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("BE_ML"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *=  chargemisidfake;
	  plotFill("BE_ML"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      
      else if ((bm1 && em2) || (bm2 && em1)) {
	plotFill("BE_MM"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("BE_MM"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *=  chargemisidfake;
	  plotFill("BE_MM"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      else if ((bh1 && el2) || (bh2 && el1)) {
	plotFill("BE_HL"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("BE_HL"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *= chargemisidfake;
	  plotFill("BE_HL"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      else if ((bl1 && eh2) || (bl2 && eh1)) {
	plotFill("EB_HL"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("EB_HL"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *=  chargemisidfake;
	  plotFill("EB_HL"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      else if ((bh1 && em2) || (bh2 && em1)) {
	plotFill("BE_HM"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("BE_HM"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2  && title.find("OS") != string::npos) {
	  weight *= chargemisidfake;
	  plotFill("BE_HM"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      else if ((bm1 && eh2) || (bm2 && eh1)) {
	plotFill("EB_HM"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("EB_HM"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2 && title.find("OS") != string::npos) {
	  weight *= chargemisidfake;
	  plotFill("EB_HM"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
      }
      else if ((bh1 && eh2) || (bh2 && eh1)) {
	plotFill("BE_HH"+title,type,M.M(),30,60,120,weight);
	if(!isData) plotFill("BE_HH"+title,"pseudodata",M.M(),30,60,120,weight);
	if(genele1 && genele2 && title.find("OS") != string::npos) {
	  weight *=  chargemisidfake;
	  plotFill("BE_HH"+title,type+"_chargemisid",M.M(),30,60,120,weight);
	}
	}*/
    }
  }

  fout->cd();
  std::map<string, map<string, TH1F* >* >::const_iterator iMap1 = myMap1->begin();
  map<string, map<string, TH1F*>* >::const_iterator jMap1 = myMap1->end();
  for (; iMap1 != jMap1; ++iMap1) {
    makedir(fout,iMap1->first);
    map<string, TH1F* > ::const_iterator iMap2 = (iMap1->second)->begin(); 
    map<string, TH1F* > ::const_iterator jMap2 = (iMap1->second)->end(); 
    for (; iMap2 != jMap2; ++iMap2) nplot1(iMap1->first,iMap2->first)->Write();
  }
  //fout->Close();
  fout->Close();
}  


void makedir(TFile* f, std::string dir) {
  //  cout << dir << "\t" << f->cd() << endl;
  TDirectory* d = (TDirectory*) f->Get(dir.c_str());
  if(!d) f->mkdir(dir.c_str());
  f->cd(dir.c_str());
}

float chargemisid( double pt, double eta) {

  float chargemisid_(1.);

  if(fabs(eta) < 1.479) {
    if(pt <25) {
      chargemisid_ = 0.000138;
    }
    else if (pt >=25 && pt <50 ) {
      chargemisid_ = 0.000225;
    }
    else  {
      chargemisid_ = 0.000319;
    }

  }
  else if(fabs(eta) >= 1.479 && eta <2.5) {
    if(pt <25) {
      chargemisid_ = 0.001090;
    }
    else if (pt >=25 && pt <50 ) {
      chargemisid_ = 0.002122;
    }
    else  {
      chargemisid_ = 0.003108;
    }
  }
  return chargemisid_;
}

float chargemisid(double eta) {


  if(fabs(eta) < 1.479) //return 0.001028;
    return .002153;
  else return 0.025105;//return 0.023980;
}
