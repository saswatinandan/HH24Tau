///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Compiling the code:   ./Make.sh ZTT_XSection.cc
//   Running the code:     ./ZTT_XSection.exe OutPut.root   Input.root
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "TreeReader.h"
#include "WeightCalculator.h"
#include "TLorentzVector.h"
#include <string> //remove space for successful compilation
#include <ostream> //remove space for successful compilation
#include "HTT-utilities/LepEffInterface/interface/ScaleFactor.h"
int  main(int argc, char** argv) {
  using namespace std;

  std::vector<string> input;
  for (int f = 1; f < argc; f++) {
    input.push_back(*(argv + f));

    cout << "\n INPUT NAME IS:   " << input[f - 1] << "\n";
  }
  ScaleFactor* myScaleFactor = new ScaleFactor();
  myScaleFactor.init_ScaleFactor("HTT-utilities/LepEffInterface/data/Muon/Muon_IdIso0p10_eff.root");
  std::vector<TH1F*> h_vec;
  for(int i=0; i<input.size(); i++) {
    TH1F* h = new TH1F("mass","",20,70,110);
    h_vec.push_back(h);
  }
  TCanvas* c  = new TCanvas("mass","",600,800);
  TFile* f = TFile::Open("prescale.root","recreate");
  TH1F* h_prescale14 = new TH1F("pre14","",3,0,3);
  TH1F* h_prescale15 = new TH1F("pre15","",3,0,3);
  TH1F* h_prescale16 = new TH1F("pre16","",3,0,3);
  
  //  TCanvas* c  = new TCanvas("mass","",600,800);

  for(int k=0; k<input.size(); k++ ) {

    TFile *f_Double = TFile::Open(input[k].c_str());
    cout << "\n  Now is running on ------->   " << std::string(f_Double->GetName()) << "\n";
        
    TFile * myFile = TFile::Open(f_Double->GetName());
    TH1F * HistoTot = (TH1F*) myFile->Get("hcount");
        
    //        TTree *Run_Tree = (TTree*) f_Double->Get("ggNtuplizer/EventTree");
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
    Run_Tree->SetBranchAddress("mcMass", &mcMass);
  
    
    /////////////////////////   MET Info
    Run_Tree->SetBranchAddress("pfMET",&pfMET);
    Run_Tree->SetBranchAddress("pfMETPhi",&pfMETPhi);
    
    
    Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();
    
    cout<<"nentries_wtn====" << nentries_wtn << "\n";


    TFile * PUData= TFile::Open("dataMoriondPU.root");
    TH1F * HistoPUData= (TH1F *) PUData->Get("pileup");
    //    HistoPUData->Scale(1.0/HistoPUData->Integral());
    
    TFile * PUMC= TFile::Open("mcMoriondPU.root");
    TH1F * HistoPUMC= (TH1F *) PUMC->Get("pileup");
    //    HistoPUMC->Scale(1.0/HistoPUMC->Integral());
    double scaldovmc = HistoPUMC->Integral()/HistoPUData->Integral();
    //    double deno(0), neu(0);

    for ( Int_t i = 0; i < nentries_wtn; i++) {
      //for ( Int_t i = 0; i < 1; i++) { 
      Run_Tree->GetEntry(i);

      if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
    
      //      bool PassTrigger =   (HLTEleMuX >> 15 & 1);
      if(isData) {
	if(!(HLTEleMuXIsPrescaled >> 14 & 1)) h_prescale14->Fill(1);
	else h_prescale14->Fill(2);
	if(!(HLTEleMuXIsPrescaled >> 15 & 1)) h_prescale15->Fill(1);
	else h_prescale15->Fill(2);
	if(!(HLTEleMuXIsPrescaled >> 16 & 1)) h_prescale16->Fill(1);
	else h_prescale16->Fill(2);
      }
      bool PassTrigger =   ((!(HLTEleMuXIsPrescaled >> 14 & 1) && (HLTEleMuX >> 14 & 1))|| (!(HLTEleMuXIsPrescaled >>15 & 1) && (HLTEleMuX >> 15 & 1 )) || (!(HLTEleMuXIsPrescaled >> 16 & 1) && (HLTEleMuX >> 16 & 1)));
      //      cout << "PassTrigger = " << PassTrigger << endl;
      if(!PassTrigger) continue;
      
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
      if (!isData) {
	if (HistoTot) LumiWeight = weightCalc(HistoTot, input[k]);
	  GetGenWeight=genWeight;
	  int puNUmmc=int(puTrue->at(0)*10);
	  int puNUmdata=int(puTrue->at(0)*10);
	  float PUMC_=HistoPUMC->GetBinContent(puNUmmc+1);
	  float PUData_=HistoPUData->GetBinContent(puNUmdata+1);
	  if (PUMC_ ==0)
	    cout<<"PUMC_ is zero!!! & num pileup= "<< puTrue->at(0)<<"\n";
	  else
	    PUWeight= scaldovmc*(PUData_/PUMC_);
      }
      
      //      double weight = GetGenWeight*PUWeight*LumiWeight*1/Scale;
      double weight = GetGenWeight*PUWeight*LumiWeight;

      ///////////////////////////////////////////////
      //Important Analysis Loop Will Happen Here!!!//
      ///////////////////////////////////////////////
      
      bool firstPart = true;
      std::vector<int> vec_muele;
      TLorentzVector m1,m2;

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

      bool samesign = muCharge->at(vec_muele[0])*muCharge->at(vec_muele[1]) >0;
      m1.SetPtEtaPhiE(muPt->at(vec_muele[0]),muEta->at(vec_muele[0]),muPhi->at(vec_muele[0]),muEn->at(vec_muele[0]));
      m2.SetPtEtaPhiE(muPt->at(vec_muele[1]),muEta->at(vec_muele[1]),muPhi->at(vec_muele[1]),muEn->at(vec_muele[1]));
      //      if(fabs((m1+m2).M() -91) >= 20) continue;
      //deno +=1;
      //if(PassTrigger) neu +=1;
      if(!samesign) h_vec[k]->Fill((m1+m2).M(),weight);
    }
    //cout << "deno== " << deno << "\t" << "neu== " << neu << endl;
    if(k==0) {
      h_vec[k]->SetLineColor(kRed);
      h_vec[k]->Draw();
    }
    else {
      h_vec[k]->SetLineColor(kBlack);
      h_vec[k]->Draw("same");
    }
   
  }
  f->cd();
  h_prescale14->Write();
  h_prescale15->Write();
  h_prescale16->Write();
  f->Close();
  c->SaveAs("mass_with_DY.pdf");
    //end of analysis code, close and write histograms/file
}


