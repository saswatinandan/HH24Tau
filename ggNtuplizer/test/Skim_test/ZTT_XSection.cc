////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Compiling the code:   ./Make.sh ZTT_XSection.cc
//   Running the code:     ./ZTT_XSection.exe OutPut.root   Input.root
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "TreeReader.h"
#include "Corrector.h"
#include "WeightCalculator.h"
#include <string> //remove space for successful compilation
#include <ostream> //remove space for successful compilation

int main(int argc, char** argv) {
  using namespace std;

  std::string out = *(argv + 1);
    
  cout << "\n\n\n OUTPUT NAME IS:    " << out << endl;     //PRINTING THE OUTPUT FILE NAME
  TFile *fout = TFile::Open(out.c_str(), "RECREATE");
    
  std::string input = *(argv + 2);
  cout << "\n\n\n INPUT NAME IS:    " << input << endl;     //PRINTING THE INPUT FILE NAME
  TFile * myFile = TFile::Open(input.c_str());
  TH1F * HistoTot = (TH1F*) myFile->Get("hcount");
 
  //add the histrograms of muon and tau visible mass (both for opposite sign and same sign pair )
  TH1F *    visibleMassOS = new TH1F ("visibleMassOS","visibleMassOS", 30, 0, 300);
  TH1F *    visibleMassSS = new TH1F ("visibleMassSS","visibleMassSS", 30, 0, 300);
  visibleMassSS->SetDefaultSumw2();
  visibleMassOS->SetDefaultSumw2();
    
  TTree *Run_Tree = (TTree*) myFile->Get("EventTree");
  cout.setf(ios::fixed, ios::floatfield);
  /////////////////////////   General Info
  Run_Tree->SetBranchAddress("isData", &isData);
  Run_Tree->SetBranchAddress("run", &run);
  Run_Tree->SetBranchAddress("lumis", &lumis);
  Run_Tree->SetBranchAddress("event", &event);
  Run_Tree->SetBranchAddress("genWeight",&genWeight);
  Run_Tree->SetBranchAddress("HLTEleMuX", &HLTEleMuX);
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
  Run_Tree->SetBranchAddress("tauByMVA6MediumElectronRejection"  ,&tauByMVA6MediumElectronRejection);
  Run_Tree->SetBranchAddress("tauByLooseCombinedIsolationDeltaBetaCorr3Hits",&tauByLooseCombinedIsolationDeltaBetaCorr3Hits);
  Run_Tree->SetBranchAddress("tauByMediumCombinedIsolationDeltaBetaCorr3Hits",&tauByMediumCombinedIsolationDeltaBetaCorr3Hits);
  Run_Tree->SetBranchAddress("tauByMVA6LooseElectronRejection", &tauByMVA6LooseElectronRejection);
  Run_Tree->SetBranchAddress("tauDxy",&tauDxy);
  Run_Tree->SetBranchAddress("tauEnergy",&tauEnergy);

  /////////////////////////   Mu Info
  Run_Tree->SetBranchAddress("nMu", &nMu);
  Run_Tree->SetBranchAddress("muPt"  ,&muPt);
  Run_Tree->SetBranchAddress("muEta"  ,&muEta);
  Run_Tree->SetBranchAddress("muEn"  ,&muEn);
  Run_Tree->SetBranchAddress("muPhi"  ,&muPhi);
  Run_Tree->SetBranchAddress("muIsoTrk", &muIsoTrk);
  Run_Tree->SetBranchAddress("muCharge",&muCharge);
  Run_Tree->SetBranchAddress("muIsMediumID",&muIsMediumID);
  Run_Tree->SetBranchAddress("muIsLooseID",&muIsLooseID);
  Run_Tree->SetBranchAddress("muPFChIso", &muPFChIso);
  Run_Tree->SetBranchAddress("muPFPhoIso", &muPFPhoIso);
  Run_Tree->SetBranchAddress("muPFNeuIso", &muPFNeuIso);
  Run_Tree->SetBranchAddress("muPFPUIso", &muPFPUIso);
  Run_Tree->SetBranchAddress("muD0",&muD0);
  Run_Tree->SetBranchAddress("muDz",&muDz);

  /////////////////////////   Ele Info
  Run_Tree->SetBranchAddress("nEle", &nEle);
  Run_Tree->SetBranchAddress("elePt"  ,&elePt);
  Run_Tree->SetBranchAddress("eleEta"  ,&eleEta);
  Run_Tree->SetBranchAddress("elePhi"  ,&elePhi);
  Run_Tree->SetBranchAddress("elePFChIso", &elePFChIso);
  Run_Tree->SetBranchAddress("eleIDMVANonTrg", &eleIDMVANonTrg);
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
        
        
  /////////////////////////   MET Info
  Run_Tree->SetBranchAddress("pfMET",&pfMET);
  Run_Tree->SetBranchAddress("pfMETPhi",&pfMETPhi);
    
  /////////////////////////   Jet Info
  Run_Tree->SetBranchAddress("nJet",&nJet);
  Run_Tree->SetBranchAddress("jetPt",&jetPt);
  Run_Tree->SetBranchAddress("jetEta",&jetEta);
  Run_Tree->SetBranchAddress("jetPhi",&jetPhi);
  Run_Tree->SetBranchAddress("jetEn",&jetEn);
  Run_Tree->SetBranchAddress("jetpfCombinedInclusiveSecondaryVertexV2BJetTags",&jetpfCombinedInclusiveSecondaryVertexV2BJetTags);

  float MuMass= 0.10565837;
  float eleMass= 0.000511;
  Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();

  cout<<"nentries_wtn====" << nentries_wtn << "\n";
  TFile * PUData= new TFile("MyDataPileupHistogram2016.root");
  TH1F * HistoPUData= (TH1F *) PUData->Get("pileup");
  HistoPUData->Scale(1.0/HistoPUData->Integral());

    
  TFile * PUMC= new TFile("Sprin16_MC.root");
  TH1F * HistoPUMC= (TH1F *) PUMC->Get("pileup");
  HistoPUMC->Scale(1.0/HistoPUMC->Integral());

  TH1F* visibleMassOSRelaxedTauIso = new TH1F("OS","",100,50,100);
  TH1F* visibleMassSSRelaxedTauIso = new TH1F("SS","",100,50,100);
  TLorentzVector Jet4Momentum,Z4Momentum,Mu4Momentum, Tau4Momentum;
  for ( Int_t i = 0; i < nentries_wtn; i++) {

    Run_Tree->GetEntry(i);

    if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
    fflush(stdout);
    float PUWeight = 1;
    float LumiWeight =1;
    float GetGenWeight =1.;
    if (!isData){
      if (HistoTot) LumiWeight = weightCalc(HistoTot, input);
      GetGenWeight=genWeight;
      int puNUmmc=int(puTrue->at(0)*10);
      int puNUmdata=int(puTrue->at(0)*10);
      float PUMC_=HistoPUMC->GetBinContent(puNUmmc+1);
      float PUData_=HistoPUData->GetBinContent(puNUmdata+1);
      PUWeight= PUData_/PUMC_;
    }

    ///////////////////////////////////////////////
    //Important Analysis Loop Will Happen Here!!!//
    ///////////////////////////////////////////////

    bool PassTrigger =   ((HLTEleMuX >> 31 & 1) == 1 && isData )|| (1 && !isData ); 
    if(!PassTrigger) continue;
    bool mufound(false),taufound(false);
    bool OS(false), SS(false);

    for  (int imu=0 ; imu < nMu; imu++){
      if(muPt->at(imu) <= 30  || fabs(muEta->at(imu)) > 2.1) continue;
      float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
      if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
	IsoMu= ( muPFChIso->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
      if(IsoMu >0.1) continue;
      if(muIsMediumID->at(imu) == 0) continue;
      float MuMetTranverseMass= TMass_F(muPt->at(imu), muPt->at(imu)*cos(muPhi->at(imu)),muPt->at(imu)*sin(muPhi->at(imu)) ,  pfMET, pfMETPhi);
      if(MuMetTranverseMass >=40) continue;
      float TrgEffWeight = trigEff_Mu(isData, muPt->at(imu), muEta->at(imu));
      mufound = true;
      Mu4Momentum.SetPtEtaPhiE(muPt->at(imu),muEta->at(imu),muPhi->at(imu),muEn->at(imu));

      for  (int itau=0 ; itau < nTau; itau++){
	if(tauPt->at(itau) <=30 || fabs(tauEta->at(itau)) > 2.3) continue;
	if(tauByMediumCombinedIsolationDeltaBetaCorr3Hits->at(itau) ==0 || tauByTightMuonRejection3->at(itau) ==0 || tauByMVA6LooseElectronRejection->at(itau)==0) continue;
        OS = muCharge->at(imu) * tauCharge->at(itau) < 0;

	SS = muCharge->at(imu) * tauCharge->at(itau) > 0;
	taufound = true;
	Tau4Momentum.SetPtEtaPhiE(tauPt->at(itau),tauEta->at(itau),tauPhi->at(itau),tauEnergy->at(itau));
	break;
      } // End of tau loop
      break;
    }  // End of muon loop
    if(!mufound || !taufound) continue;
    bool IsthereDiMuon= false;
        
    for  (int imu=0 ; imu < nMu; imu++){
      for  (int jmu=0 ; jmu < nMu; jmu++){
                
                
	// Select first good muon
	bool MuPtCut1 = muPt->at(imu) > 30 && fabs(muEta->at(imu)) < 2.1 ;
	float IsoMu1=muPFChIso->at(imu)/muPt->at(imu);
	if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
	  IsoMu1= ( muPFChIso->at(imu)/muPt->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
	bool MuIdIso1=(muIsLooseID->at(imu) > 0 && IsoMu1 < 0.30 && fabs(muD0->at(imu)) < 0.045 && fabs(muDz->at(imu)) < 0.2);
                
                
	// Select second good muon
	bool MuPtCut2 = muPt->at(jmu) > 15 && fabs(muEta->at(jmu)) < 2.4 ;
	float IsoMu2=muPFChIso->at(jmu)/muPt->at(jmu);
	if ( (muPFNeuIso->at(jmu) + muPFPhoIso->at(jmu) - 0.5* muPFPUIso->at(jmu) )  > 0.0)
	  IsoMu2= ( muPFChIso->at(jmu)/muPt->at(jmu) + muPFNeuIso->at(jmu) + muPFPhoIso->at(jmu) - 0.5* muPFPUIso->at(jmu))/muPt->at(jmu);
	bool MuIdIso2=(muIsLooseID->at(jmu) > 0 && IsoMu2 < 0.30 && fabs(muD0->at(jmu)) < 0.045 && fabs(muDz->at(jmu)) < 0.2);
                
                
	bool  OS_ = muCharge->at(imu) * muCharge->at(jmu) < 0;
                
	if(MuIdIso1 && MuIdIso2 && OS_)
	  IsthereDiMuon=true;
                
      }
    }
    if(IsthereDiMuon) continue;
    bool bjetveto(false);

    for (int ijet= 0 ; ijet < nJet ; ijet++){

      Jet4Momentum.SetPtEtaPhiE(jetPt->at(ijet),jetEta->at(ijet),jetPhi->at(ijet),jetEn->at(ijet));
      if (jetPt->at(ijet) > 20 && fabs(jetEta->at(ijet)) < 2.5 && Jet4Momentum.DeltaR(Tau4Momentum) > 0.5 && Jet4Momentum.DeltaR(Mu4Momentum) > 0.5  && jetpfCombinedInclusiveSecondaryVertexV2BJetTags->at(ijet) > 0.679) bjetveto =true;
    }
    if(bjetveto) continue;
    Z4Momentum = Mu4Momentum+ Tau4Momentum;
    if(OS) visibleMassOS->Fill(Z4Momentum.M(),LumiWeight*GetGenWeight*PUWeight);

    //Check if there is a SS  muTau pair with dR > 0.5 and TMass(mu.MET) < 40 and then fill the weighted histogram as below:
    if(SS) visibleMassSS->Fill(Z4Momentum.M(),LumiWeight*GetGenWeight*PUWeight);


  } //End Processing all entries


  //end of analysis code, close and write histograms/file
  fout->cd();
  visibleMassOS->Write();
  visibleMassSS->Write();
  visibleMassOSRelaxedTauIso->Write();
  visibleMassSSRelaxedTauIso->Write();
  fout->Close();
}
