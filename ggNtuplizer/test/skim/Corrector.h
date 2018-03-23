#include "../interface/CodexAnalyzer.h"
#include "../interface/WeightCalculator.h"
#include "../interface/Corrector.h"
#include "../interface/Functions.h"
#include "../interface/makeHisto.h"
#include <string>
#include <ostream>
#include <vector>


int main(int argc, char** argv) {
  using namespace std;
    
  std::string out = *(argv + 1);
    
  cout << "\n\n\n OUTPUT NAME IS:    " << out << endl;     //PRINTING THE OUTPUT name
  TFile *fout = TFile::Open(out.c_str(), "RECREATE");
    
  myMap1 = new std::map<std::string, TH1F*>();
  myMap2 = new map<string, TH2F*>();
    
  std::vector<string> input;
  for (int f = 2; f < argc; f++) {
    input.push_back(*(argv + f));
    cout << "\n INPUT NAME IS:   " << input[f - 2] << "\n";
  }
    
  TFile * PUData= TFile::Open("../interface/pileup-hists/dataMoriondPU.root");
  TH1F * HistoPUData= (TH1F *) PUData->Get("pileup");
  HistoPUData->Scale(1.0/HistoPUData->Integral());
    
  TFile * PUMC= TFile::Open("../interface/pileup-hists/mcMoriondPU.root");
  TH1F * HistoPUMC= (TH1F *) PUMC->Get("pileup");
  HistoPUMC->Scale(1.0/HistoPUMC->Integral());
    
    
  //########################################
  // Muon Id, Iso, Trigger and Tracker Eff files
  //########################################
  TFile * MuCorrId_BCDEF= TFile::Open(("../interface/pileup-hists/ID_EfficienciesAndSF_BCDEF.root"));
  TH2F * HistoMuId_BCDEF= (TH2F *) MuCorrId_BCDEF->Get("MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
    
  TFile * MuCorrId_GH= TFile::Open(("../interface/pileup-hists/ID_EfficienciesAndSF_GH.root"));
  TH2F * HistoMuId_GH= (TH2F *) MuCorrId_GH->Get("MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
    
  TFile * MuCorrIso_BCDEF= TFile::Open(("../interface/pileup-hists/Iso_EfficienciesAndSF_BCDEF.root"));
  TH2F * HistoMuIso_BCDEF= (TH2F *) MuCorrIso_BCDEF->Get("TightISO_TightID_pt_eta/pt_abseta_ratio");
    
  TFile * MuCorrIso_GH= TFile::Open(("../interface/pileup-hists/Iso_EfficienciesAndSF_GH.root"));
  TH2F * HistoMuIso_GH= (TH2F *) MuCorrIso_GH->Get("TightISO_TightID_pt_eta/pt_abseta_ratio");
    
  TFile * MuCorrTrg_BCDEF= TFile::Open(("../interface/pileup-hists/Trigger_EfficienciesAndSF_RunBtoF.root"));
  TH2F * HistoMuTrg_BCDEF= (TH2F *) MuCorrTrg_BCDEF->Get("Mu50_OR_TkMu50_PtEtaBins/pt_abseta_ratio");
    
  TFile * MuCorrTrg_GH= TFile::Open(("../interface/pileup-hists/Trigger_EfficienciesAndSF_Period4.root"));
  TH2F * HistoMuTrg_GH= (TH2F *) MuCorrTrg_GH->Get("Mu50_OR_TkMu50_PtEtaBins/pt_abseta_ratio");
    
  TFile * MuCorrTrack= TFile::Open(("../interface/pileup-hists/Tracking_EfficienciesAndSF_BCDEFGH.root"));
  TGraphAsymmErrors * HistoMuTrack= (TGraphAsymmErrors *) MuCorrTrack->Get("ratio_eff_eta3_dr030e030_corr");
    
  TH2F* HistoMuId[2]={HistoMuId_BCDEF, HistoMuId_GH};
  TH2F* HistoMuIso[2]={HistoMuIso_BCDEF,HistoMuIso_GH};
  TH2F* HistoMuTrg[2]={HistoMuTrg_BCDEF, HistoMuTrg_GH};
    
    
  //########################################
  // Electron MVA IdIso files
  //########################################
  TFile * EleCorrMVAIdIso90= TFile::Open(("../interface/pileup-hists/egammaEffi.txt_EGM2D.root"));
  TH2F * HistoEleMVAIdIso90= (TH2F *) EleCorrMVAIdIso90->Get("EGamma_SF2D");
    
    
  //########################################
  // W and DY K-factor files
  //########################################
  TFile * KFactor= TFile::Open("../interface/pileup-hists/kfactors.root");
  TH1F * WLO= (TH1F *) KFactor->Get("WJets_LO/inv_pt");
  TH1F * WNLO= (TH1F *) KFactor->Get("EWKcorr/W");
  TH1F * ZLO= (TH1F *) KFactor->Get("ZJets_LO/inv_pt");
  TH1F * ZNLO= (TH1F *) KFactor->Get("EWKcorr/Z");
    
    
    
  for (int k = 0; k < input.size(); k++) {
        
    //std::string input = *(argv + 2);
    TFile *f_Double = TFile::Open(input[k].c_str());
    cout << "\n  Now is running on ------->   " << std::string(f_Double->GetName()) << "\n";
        
    std::string InputROOT= std::string(f_Double->GetName());
    TFile * myFile = TFile::Open(f_Double->GetName());
    TH1F * HistoTot = (TH1F*) myFile->Get("hcount");
        
    //        TTree *Run_Tree = (TTree*) f_Double->Get("ggNtuplizer/EventTree");
    TTree *Run_Tree = (TTree*) f_Double->Get("EventTree");
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(6);
        
        
    //        std::string ROOTLoc= "/Users/abdollah1/GIT_abdollah110/DM2016/ROOT80X/";
    //        vector<float> DY_Events = DY_HTBin(ROOTLoc);
    //        vector<float> W_Events = W_HTBin(ROOTLoc);
    //        vector<float> W_EventsNLO = W_PTBinNLO(ROOTLoc); //This is for the NLO samples (as the stat is too low we do not use them)
    //        vector<float> W_EventsNLO = W_HTBin(ROOTLoc);
        
    //########################################   General Info
    Run_Tree->SetBranchAddress("isData", &isData);
    Run_Tree->SetBranchAddress("run", &run);
    Run_Tree->SetBranchAddress("lumis", &lumis);
    Run_Tree->SetBranchAddress("event", &event);
    Run_Tree->SetBranchAddress("genWeight",&genWeight);
    Run_Tree->SetBranchAddress("HLTEleMuX", &HLTEleMuX);
    Run_Tree->SetBranchAddress("puTrue", &puTrue);
    Run_Tree->SetBranchAddress("nVtx",&nVtx);
        
    //########################################   MC Info
    Run_Tree->SetBranchAddress("nMC", &nMC);
    Run_Tree->SetBranchAddress("mcPID", &mcPID);
    Run_Tree->SetBranchAddress("mcStatus", &mcStatus);
    Run_Tree->SetBranchAddress("mcPt", &mcPt );
    Run_Tree->SetBranchAddress("mcEta", &mcEta );
    Run_Tree->SetBranchAddress("mcPhi", &mcPhi );
    Run_Tree->SetBranchAddress("mcE", &mcE );
    Run_Tree->SetBranchAddress("mcMass", &mcMass );
    Run_Tree->SetBranchAddress("mcMomPID", &mcMomPID );
    Run_Tree->SetBranchAddress("mcGMomPID", &mcGMomPID );
        
    Run_Tree->SetBranchAddress("nTau", &nTau);
    Run_Tree->SetBranchAddress("tauPt"  ,&tauPt);
    Run_Tree->SetBranchAddress("tauEta"  ,&tauEta);
    Run_Tree->SetBranchAddress("tauPhi"  ,&tauPhi);
    Run_Tree->SetBranchAddress("tauMass"  ,&tauMass);
    Run_Tree->SetBranchAddress("tauCharge"  ,&tauCharge);
    Run_Tree->SetBranchAddress("taupfTausDiscriminationByDecayModeFinding", &taupfTausDiscriminationByDecayModeFinding);
    Run_Tree->SetBranchAddress("tauByTightMuonRejection3", &tauByTightMuonRejection3);
    Run_Tree->SetBranchAddress("tauByLooseMuonRejection3", &tauByLooseMuonRejection3);
    Run_Tree->SetBranchAddress("tauByMVA6MediumElectronRejection"  ,&tauByMVA6MediumElectronRejection);
    Run_Tree->SetBranchAddress("tauByLooseCombinedIsolationDeltaBetaCorr3Hits",&tauByLooseCombinedIsolationDeltaBetaCorr3Hits);
    Run_Tree->SetBranchAddress("tauByMediumCombinedIsolationDeltaBetaCorr3Hits",&tauByMediumCombinedIsolationDeltaBetaCorr3Hits);
    Run_Tree->SetBranchAddress("tauByMVA6LooseElectronRejection", &tauByMVA6LooseElectronRejection);
    Run_Tree->SetBranchAddress("tauDxy",&tauDxy);
    Run_Tree->SetBranchAddress("tauDecayMode",&tauDecayMode);
    Run_Tree->SetBranchAddress("tauByLooseIsolationMVArun2v1DBoldDMwLT",&tauByLooseIsolationMVArun2v1DBoldDMwLT);
    Run_Tree->SetBranchAddress("tauByVLooseIsolationMVArun2v1DBoldDMwLT",&tauByVLooseIsolationMVArun2v1DBoldDMwLT);
        
    //########################################   Mu Info
    Run_Tree->SetBranchAddress("nMu", &nMu);
    Run_Tree->SetBranchAddress("muPt"  ,&muPt);
    Run_Tree->SetBranchAddress("muEta"  ,&muEta);
    Run_Tree->SetBranchAddress("muPhi"  ,&muPhi);
    Run_Tree->SetBranchAddress("muIsoTrk", &muIsoTrk);
    Run_Tree->SetBranchAddress("muCharge",&muCharge);
    Run_Tree->SetBranchAddress("muIDbit",&muIDbit);//NEW
    Run_Tree->SetBranchAddress("muPFChIso", &muPFChIso);
    Run_Tree->SetBranchAddress("muPFPhoIso", &muPFPhoIso);
    Run_Tree->SetBranchAddress("muPFNeuIso", &muPFNeuIso);
    Run_Tree->SetBranchAddress("muPFPUIso", &muPFPUIso);
    Run_Tree->SetBranchAddress("muD0",&muD0);
    Run_Tree->SetBranchAddress("muDz",&muDz);
        
    //########################################   Ele Info
    Run_Tree->SetBranchAddress("nEle", &nEle);
    Run_Tree->SetBranchAddress("elePt"  ,&elePt);
    Run_Tree->SetBranchAddress("eleEta"  ,&eleEta);
    Run_Tree->SetBranchAddress("elePhi"  ,&elePhi);
    Run_Tree->SetBranchAddress("elePFChIso", &elePFChIso);
    Run_Tree->SetBranchAddress("eleIDMVA", &eleIDMVA);//NEW
    Run_Tree->SetBranchAddress("eleCharge",&eleCharge);
    Run_Tree->SetBranchAddress("eleSCEta",&eleSCEta);
    Run_Tree->SetBranchAddress("elePFChIso", &elePFChIso);
    Run_Tree->SetBranchAddress("elePFPhoIso", &elePFPhoIso);
    Run_Tree->SetBranchAddress("elePFNeuIso", &elePFNeuIso);
    Run_Tree->SetBranchAddress("elePFPUIso", &elePFPUIso);
    Run_Tree->SetBranchAddress("eleD0",&eleD0);
    Run_Tree->SetBranchAddress("eleDz",&eleDz);
    Run_Tree->SetBranchAddress("eleMissHits", &eleMissHits);
    Run_Tree->SetBranchAddress("eleConvVeto", &eleConvVeto);
    Run_Tree->SetBranchAddress("eleSCEta", &eleSCEta );
        
    //########################################   Jet Info
    Run_Tree->SetBranchAddress("nJet",&nJet);
    Run_Tree->SetBranchAddress("jetPt",&jetPt);
    Run_Tree->SetBranchAddress("jetEta",&jetEta);
    Run_Tree->SetBranchAddress("jetPhi",&jetPhi);
    Run_Tree->SetBranchAddress("jetEn",&jetEn);
    Run_Tree->SetBranchAddress("jetCSV2BJetTags",&jetCSV2BJetTags);
    Run_Tree->SetBranchAddress("jetPFLooseId",&jetPFLooseId);
    Run_Tree->SetBranchAddress("jetPUID",&jetPUID);
    Run_Tree->SetBranchAddress("jetRawPt",&jetRawPt);
    Run_Tree->SetBranchAddress("jetJECUnc",&jetJECUnc);
    Run_Tree->SetBranchAddress("jetRawEn",&jetRawEn);
    Run_Tree->SetBranchAddress("jetHadFlvr",&jetHadFlvr);
        
    //########################################   MET Info
    Run_Tree->SetBranchAddress("pfMET",&pfMET);
