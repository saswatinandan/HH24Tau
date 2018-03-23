#ifndef TREE_READER_H
#define	TREE_READER_H

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "math.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TInterpreter.h"
#include "TSystem.h"
#include "TNtuple.h"
#include "TPaveLabel.h"
#include "TPaveText.h"
#include "TFrame.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <utility>
#include <iostream>
#include <map>
#include "TLorentzVector.h"
#include "../../../../HTT-utilities/LepEffInterface/interface/ScaleFactor.h"
#include "TF1.h"

using namespace std;
double scaleFactor(double pt, double eta);
int ptBIN=0;
int etaBIN=0;
int etaPOINT=-1;

float LumiB=5.93;
float LumiC=2.65;
float LumiD=4.35;
float LumiE=4.12;
float LumiF=3.19;
float LumiG=7.72;
float LumiHv2=8.64;
float LumiHv3=0.22;

float LumiBCDEF=LumiB+LumiC+LumiD+LumiE+LumiF;
float LumiGH=LumiG+LumiHv2+LumiHv3;

map<string, TH1F*>* myMap1;
map<string, TH2F*>* myMap2;
map<string, TH3F*>* myMap3;

void plotFill(string name, float x, int nx, float nxmin, float nxmax, double weight=1) {
  if (myMap1->find(name) == myMap1->end())
    (*myMap1)[name] = new TH1F(name.c_str(), name.c_str(), nx, nxmin, nxmax);
  if(name.find("Nor") ==string::npos && name.find("Dn") ==string::npos && name.find("Up")==string::npos)  (*myMap1)[name]->SetDefaultSumw2();
  (*myMap1)[name]->Fill(x,weight);
}

void plotFill(string name, float x, float y, int nx, float nxmin, float nxmax, int ny, float nymin, float nymax, double weight=1) {
  if (myMap2->find(name) == myMap2->end())
    (*myMap2)[name] = new TH2F(name.c_str(), name.c_str(), nx, nxmin, nxmax, ny, nymin, nymax);
  (*myMap2)[name]->SetDefaultSumw2();
  (*myMap2)[name]->Fill(x, y,weight);
}

void plotFill(string name, float x, float y, float z, int nx, float nxmin, float nxmax, int ny, float nymin, float nymax, int nz, float nzmin, float nzmax, double weight=1) {
  if (myMap3->find(name) == myMap3->end())
    (*myMap3)[name] = new TH3F(name.c_str(), name.c_str(), nx, nxmin, nxmax, ny, nymin, nymax, nz, nzmin, nzmax);
  (*myMap3)[name]->SetDefaultSumw2();
  (*myMap3)[name]->Fill(x, y, z,weight);
}


TH1F* nplot1(string name) {
  if (myMap1->find(name) != myMap1->end())
    return (*myMap1)[name];
  else
    return 0;
}

TH3F* nplot3(string name) {
  if (myMap3->find(name) != myMap3->end())
    return (*myMap3)[name];
  else
    return 0;
}

TH2F* nplot2(string name) {
  if (myMap2->find(name) != myMap2->end())
    return (*myMap2)[name];
  else
    return 0;
}


float deltaPhi(float a, float b) {
    float result = a - b;
    while (result > M_PI) result -= 2 * M_PI;
    while (result <= -M_PI) result += 2 * M_PI;
    return fabs(result);
}

float TMass_F(float pt3lep, float px3lep, float py3lep, float met, float metPhi) {
    return sqrt(pow(pt3lep + met, 2) - pow(px3lep + met * cos(metPhi), 2) - pow(py3lep + met * sin(metPhi), 2));
}

float TMass_FNew(float pt3lep, float philep, float met, float metPhi) {
    return sqrt(2*pt3lep * met *(1-cos(deltaPhi(metPhi,philep))));
}



float dR_(float ieta, float iphi, float jeta, float jphi){
    
    float deta=ieta-jeta;
    float dphi=deltaPhi(iphi,jphi);
    return sqrt(pow(deta,2)+pow(dphi,2));
}



// Declaration of leaf types
// Declaration of leaf types
Int_t           run;
Long64_t        event;
Int_t           lumis;
Bool_t          isData;
Int_t           nVtx;
Int_t           nTrksPV;
Bool_t          isPVGood;
Bool_t          hasGoodVtx;
Float_t         vtx;
Float_t         vty;
Float_t         vtz;
Float_t         rho;
Float_t         rhoCentral;
ULong64_t       HLTEleMuX;
ULong64_t       HLTPho;
ULong64_t       HLTJet;
ULong64_t       HLTEleMuXIsPrescaled;
ULong64_t       HLTPhoIsPrescaled;
ULong64_t       HLTJetIsPrescaled;
vector<float>   *pdf;
Float_t         pthat;
Float_t         processID;
Float_t         genWeight;
Float_t         num_gen_jets;
Float_t         genHT;
TString         *EventTag;
Int_t           nPUInfo;
vector<int>     *nPU;
vector<int>     *puBX;
vector<float>   *puTrue;
Int_t           nMC;
vector<int>     *mcPID;
vector<float>   *mcVtx;
vector<float>   *mcVty;
vector<float>   *mcVtz;
vector<float>   *mcPt;
vector<float>   *mcMass;
vector<float>   *mcEta;
vector<float>   *mcPhi;
vector<float>   *mcE;
vector<float>   *mcEt;
vector<int>     *mcGMomPID;
vector<int>     *mcMomPID;
vector<float>   *mcMomPt;
vector<float>   *mcMomMass;
vector<float>   *mcMomEta;
vector<float>   *mcMomPhi;
vector<int>     *mcIndex;
vector<unsigned short> *mcStatusFlag;
vector<int>     *mcParentage;
vector<int>     *mcStatus;
vector<float>   *mcCalIsoDR03;
vector<float>   *mcTrkIsoDR03;
vector<float>   *mcCalIsoDR04;
vector<float>   *mcTrkIsoDR04;
vector<float>   *mcHadronPt;
vector<float>   *mcHadronEta;
vector<float>   *mcHadronPhi;
vector<float>   *mcHadronMass;
vector<float>   *mcHadronE;
vector<float>   *mcHadronGMomPID;
vector<float>   *mcHadronMomPID;
vector<float>   *mcHadronMomPt;
vector<float>   *mcHadronMomMass;
vector<float>   *mcHadronMomEta;
vector<float>   *mcHadronMomPhi;

Int_t           metFilters;
Float_t         genMET;
Float_t         genMETPhi;
Float_t         pfMET;
Double_t         signif_dxx;
Double_t         signif_dyy;
Double_t         signif_dxy;
Double_t         signif_dyx;
Float_t         pfMETPhi;
Float_t         pfMETsumEt;
Float_t         pfMETmEtSig;
Float_t         pfMETSig;
Float_t         pfMET_T1JERUp;
Float_t         pfMET_T1JERDo;
Float_t         pfMET_T1JESUp;
Float_t         pfMET_T1JESDo;
Float_t         pfMET_T1MESUp;
Float_t         pfMET_T1MESDo;
Float_t         pfMET_T1EESUp;
Float_t         pfMET_T1EESDo;
Float_t         pfMET_T1PESUp;
Float_t         pfMET_T1PESDo;
Float_t         pfMET_T1TESUp;
Float_t         pfMET_T1TESDo;
Float_t         pfMET_T1UESUp;
Float_t         pfMET_T1UESDo;
Int_t           nEle;
vector<int>     *eleCharge;
vector<int>     *eleChargeConsistent;
vector<int>     *ele2ndChargeConsistent;
vector<float>   *eleEn;
vector<float>   *eleSCEn;
vector<float>   *eleESEn;
vector<float>   *eleESEnP1;
vector<float>   *eleESEnP2;
vector<float>   *eleD0;
vector<float>   *eleDz;
vector<float>   *elePt;
vector<float>   *eleEta;
vector<float>   *elePhi;
vector<float>   *eleR9;
vector<float>   *eleCalibPt;
vector<float>   *eleCalibEn;
vector<float>   *eleSCEta;
vector<float>   *eleSCPhi;
vector<float>   *eleSCRawEn;
vector<float>   *eleSCEtaWidth;
vector<float>   *eleSCPhiWidth;
vector<float>   *eleHoverE;
vector<float>   *eleEoverP;
vector<float>   *eleEoverPout;
vector<float>   *eleEoverPInv;
vector<float>   *eleBrem;
vector<float>   *eledEtaAtVtx;
vector<float>   *eledPhiAtVtx;
vector<float>   *eledEtaAtCalo;
vector<float>   *eleSigmaIEtaIEta;
vector<float>   *eleSigmaIEtaIPhi;
vector<float>   *eleSigmaIPhiIPhi;
vector<float>   *eleSigmaIEtaIEtaFull5x5;
vector<float>   *eleSigmaIPhiIPhiFull5x5;
vector<int>     *eleConvVeto;
vector<int>     *eleMissHits;
vector<float>   *eleESEffSigmaRR;
vector<float>   *elePFChIso;
vector<float>   *elePFPhoIso;
vector<float>   *elePFNeuIso;
vector<float>   *elePFPUIso;
vector<float>   *elePFClusEcalIso;
vector<float>   *elePFClusHcalIso;
vector<float>   *elePFMiniIso;
vector<float>   *eleIDMVANonTrg;
vector<float>   *eleIDMVA;
vector<float>   *eleIDMVATrg;
vector<float>   *eledEtaseedAtVtx;
vector<float>   *eleE1x5;
vector<float>   *eleE2x5;
vector<float>   *eleE5x5;
vector<float>   *eleE1x5Full5x5;
vector<float>   *eleE2x5Full5x5;
vector<float>   *eleE5x5Full5x5;
vector<float>   *eleR9Full5x5;
vector<int>     *eleEcalDrivenSeed;
vector<float>   *eleDr03EcalRecHitSumEt;
vector<float>   *eleDr03HcalDepth1TowerSumEt;
vector<float>   *eleDr03HcalDepth2TowerSumEt;
vector<float>   *eleDr03HcalTowerSumEt;
vector<float>   *eleDr03TkSumPt;
vector<float>   *elecaloEnergy;
vector<float>   *eleTrkdxy;
vector<float>   *eleKFHits;
vector<float>   *eleKFChi2;
vector<vector<float> > *eleGSFPt;
vector<vector<float> > *eleGSFEta;
vector<vector<float> > *eleGSFPhi;
vector<vector<float> > *eleGSFCharge;
vector<vector<int> > *eleGSFHits;
vector<vector<int> > *eleGSFMissHits;
vector<vector<int> > *eleGSFNHitsMax;
vector<vector<float> > *eleGSFVtxProb;
vector<vector<float> > *eleGSFlxyPV;
vector<vector<float> > *eleGSFlxyBS;
vector<vector<float> > *eleBCEn;
vector<vector<float> > *eleBCEta;
vector<vector<float> > *eleBCPhi;
vector<vector<float> > *eleBCS25;
vector<vector<float> > *eleBCS15;
vector<vector<float> > *eleBCSieie;
vector<vector<float> > *eleBCSieip;
vector<vector<float> > *eleBCSipip;
vector<int>     *eleFiredTrgs;
vector<unsigned short> *eleIDbit;
Int_t           nMu;
vector<float>   *muPt;
vector<float>   *muEn;
vector<float>   *muEta;
vector<float>   *muPhi;
vector<int>     *muCharge;
vector<int>     *muType;
vector<bool>    *muIsLooseID;
vector<bool>    *muIsMediumID;
vector<bool>    *muIsTightID;
vector<bool>    *muIsSoftID;
vector<bool>    *muIsHighPtID;
vector<float>   *muD0;
vector<float>   *muDz;
vector<float>   *muChi2NDF;
vector<float>   *muInnerD0;
vector<float>   *muInnerDz;
vector<int>     *muTrkLayers;
vector<int>     *muPixelLayers;
vector<int>     *muPixelHits;
vector<int>     *muMuonHits;
vector<int>     *muStations;
vector<int>     *muMatches;
vector<int>     *muTrkQuality;
vector<float>   *muIsoTrk;
vector<float>   *muPFChIso;
vector<float>   *muPFPhoIso;
vector<float>   *muPFNeuIso;
vector<float>   *muPFPUIso;
vector<float>   *muPFMiniIso;
vector<int>     *muFiredTrgs;
vector<float>   *muInnervalidFraction;
vector<float>   *musegmentCompatibility;
vector<float>   *muchi2LocalPosition;
vector<float>   *mutrkKink;
vector<float>   *muBestTrkPtError;
vector<float>   *muBestTrkPt;
vector<UShort_t>   *muIDbit;
vector<UShort_t>   *muFiredTrgs_;
Int_t           nTau;
vector<bool>    *taupfTausDiscriminationByDecayModeFinding;
vector<bool>    *taupfTausDiscriminationByDecayModeFindingNewDMs;
vector<bool>    *tauByMVA6VLooseElectronRejection;
vector<bool>    *tauByMVA6LooseElectronRejection;
vector<bool>    *tauByMVA6MediumElectronRejection;
vector<bool>    *tauByMVA6TightElectronRejection;
vector<bool>    *tauByMVA6VTightElectronRejection;
vector<bool>    *tauByLooseMuonRejection3;
vector<bool>    *tauByTightMuonRejection3;
vector<bool>    *tauByLooseCombinedIsolationDeltaBetaCorr3Hits;
vector<bool>    *tauByMediumCombinedIsolationDeltaBetaCorr3Hits;
vector<bool>    *tauByTightCombinedIsolationDeltaBetaCorr3Hits;
vector<float>   *tauCombinedIsolationDeltaBetaCorrRaw3Hits;
vector<float>   *tauByIsolationMVArun2v1DBnewDMwLTraw;
vector<float>   *tauByIsolationMVArun2v1DBoldDMwLTraw;
vector<float>   *tauByIsolationMVArun2v1PWnewDMwLTraw;
vector<float>   *tauByIsolationMVArun2v1PWoldDMwLTraw;
vector<bool>    *tauByVTightIsolationMVArun2v1DBnewDMwLT;
vector<bool>    *tauByVTightIsolationMVArun2v1DBoldDMwLT;
vector<bool>    *tauByVTightIsolationMVArun2v1PWnewDMwLT;
vector<bool>    *tauByVTightIsolationMVArun2v1PWoldDMwLT;
vector<bool>    *tauByTightIsolationMVArun2v1DBnewDMwLT;
vector<bool>    *tauByTightIsolationMVArun2v1DBoldDMwLT;
vector<bool>    *tauByTightIsolationMVArun2v1PWnewDMwLT;
vector<bool>    *tauByTightIsolationMVArun2v1PWoldDMwLT;
vector<bool>    *tauByMediumIsolationMVArun2v1DBnewDMwLT;
vector<bool>    *tauByMediumIsolationMVArun2v1DBoldDMwLT;
vector<bool>    *tauByMediumIsolationMVArun2v1PWnewDMwLT;
vector<bool>    *tauByMediumIsolationMVArun2v1PWoldDMwLT;
vector<bool>    *tauByLooseIsolationMVArun2v1DBnewDMwLT;
vector<bool>    *tauByLooseIsolationMVArun2v1DBoldDMwLT;
vector<bool>    *tauByLooseIsolationMVArun2v1PWnewDMwLT;
vector<bool>    *tauByLooseIsolationMVArun2v1PWoldDMwLT;
vector<bool>    *tauByVLooseIsolationMVArun2v1DBnewDMwLT;
vector<bool>    *tauByVLooseIsolationMVArun2v1DBoldDMwLT;
vector<bool>    *tauByVLooseIsolationMVArun2v1PWnewDMwLT;
vector<bool>    *tauByVLooseIsolationMVArun2v1PWoldDMwLT;
vector<float>   *tauEta;
vector<float>   *tauPhi;
vector<float>   *tauPt;
vector<float>   *tauEt;
vector<float>   *tauCharge;
vector<float>   *tauP;
vector<float>   *tauPx;
vector<float>   *tauPy;
vector<float>   *tauPz;
vector<float>   *tauVz;
vector<float>   *tauEnergy;
vector<float>   *tauMass;
vector<float>   *tauDxy;
vector<float>   *tauZImpact;
vector<int>     *tauDecayMode;
vector<bool>    *tauLeadChargedHadronExists;
vector<float>   *tauLeadChargedHadronEta;
vector<float>   *tauLeadChargedHadronPhi;
vector<float>   *tauLeadChargedHadronPt;
vector<float>   *tauChargedIsoPtSum;
vector<float>   *tauNeutralIsoPtSum;
vector<float>   *tauPuCorrPtSum;
vector<int>     *tauNumSignalPFChargedHadrCands;
vector<int>     *tauNumSignalPFNeutrHadrCands;
vector<int>     *tauNumSignalPFGammaCands;
vector<int>     *tauNumSignalPFCands;
vector<int>     *tauNumIsolationPFChargedHadrCands;
vector<int>     *tauNumIsolationPFNeutrHadrCands;
vector<int>     *tauNumIsolationPFGammaCands;
vector<int>     *tauNumIsolationPFCands;
vector<float>   *taufootprintCorrection;
vector<float>   *tauphotonPtSumOutsideSignalCone;
vector<float>   *taudz;
vector<float>   *taudxy;
Int_t           nJet;
vector<float>   *jetPt;
vector<float>   *jetEn;
vector<float>   *jetEta;
vector<float>   *jetPhi;
vector<float>   *jetRawPt;
vector<float>   *jetRawEn;
vector<float>   *jetMt;
vector<float>   *jetArea;
vector<float>   *jetLeadTrackPt;
vector<float>   *jetLeadTrackEta;
vector<float>   *jetLeadTrackPhi;
vector<int>     *jetLepTrackPID;
vector<float>   *jetLepTrackPt;
vector<float>   *jetLepTrackEta;
vector<float>   *jetLepTrackPhi;
vector<float>   *jetpfCombinedInclusiveSecondaryVertexV2BJetTags;
vector<float>   *jetJetProbabilityBJetTags;
vector<float>   *jetpfCombinedMVAV2BJetTags;
vector<int>     *jetPartonID;
vector<int>     *jetHadFlvr;
vector<int>     *jetGenJetIndex;
vector<float>   *jetGenJetEn;
vector<float>   *jetGenJetPt;
vector<float>   *jetGenJetEta;
vector<float>   *jetGenJetPhi;
vector<int>     *jetGenPartonID;
vector<float>   *jetGenEn;
vector<float>   *jetGenPt;
vector<float>   *jetGenEta;
vector<float>   *jetGenPhi;
vector<int>     *jetGenPartonMomID;
vector<bool>    *jetPFLooseId;
vector<float>   *jetPUidFullDiscriminant;
vector<float>   *jetJECUnc;
vector<int>     *jetFiredTrgs;
vector<float>   *jetCHF;
vector<float>   *jetCSV2BJetTags;
vector<float>   *jetNHF;
vector<float>   *jetCEF;
vector<float>   *jetNEF;
vector<int>     *jetNCH;
vector<float>   *jetVtxPt;
vector<float>   *jetVtxMass;
vector<float>   *jetVtxNtrks;
vector<float>   *jetVtx3DVal;
vector<float>   *jetVtx3DSig;

/*
 
 // Declaration of leaf types
 Int_t           run;
 Long64_t        event;
 Int_t           lumis;
 Bool_t          isData;
 Int_t           nVtx;
 Int_t           nTrksPV;
 Float_t         rho;
 ULong64_t       HLT;
 ULong64_t       HLTIsPrescaled;
 Float_t         genMET;
 Float_t         genMETPhi;
 Float_t         pfMET;
 Float_t         pfMETPhi;
 Float_t         pfMETsumEt;
 Float_t         pfMETmEtSig;
 Float_t         pfMETSig;
 vector<float>   *pdf;
 Float_t         pthat;
 Float_t         processID;
 Int_t           nPUInfo;
 vector<int>     *nPU;
 vector<int>     *puBX;
 vector<float>   *puTrue;
 Int_t           nMC;
 vector<int>     *mcPID;
 vector<float>   *mcVtx_x;
 vector<float>   *mcVtx_y;
 vector<float>   *mcVtx_z;
 vector<float>   *mcPt;
 vector<float>   *mcMass;
 vector<float>   *mcEta;
 vector<float>   *mcPhi;
 vector<float>   *mcE;
 vector<float>   *mcEt;
 vector<int>     *mcGMomPID;
 vector<int>     *mcMomPID;
 vector<float>   *mcMomPt;
 vector<float>   *mcMomMass;
 vector<float>   *mcMomEta;
 vector<float>   *mcMomPhi;
 vector<int>     *mcIndex;
 vector<int>     *mcDecayType;
 vector<int>     *mcParentage;
 vector<int>     *mcStatus;
 vector<float>   *mcCalIsoDR03;
 vector<float>   *mcTrkIsoDR03;
 vector<float>   *mcCalIsoDR04;
 vector<float>   *mcTrkIsoDR04;
 Int_t           nPho;
 vector<float>   *phoE;
 vector<float>   *phoEt;
 vector<float>   *phoEta;
 vector<float>   *phoPhi;
 vector<float>   *phoSCE;
 vector<float>   *phoSCRawE;
 vector<float>   *phoESEn;
 vector<float>   *phoSCEta;
 vector<float>   *phoSCPhi;
 vector<float>   *phoSCEtaWidth;
 vector<float>   *phoSCPhiWidth;
 vector<float>   *phoSCBrem;
 vector<int>     *phohasPixelSeed;
 vector<int>     *phoEleVeto;
 vector<float>   *phoR9;
 vector<float>   *phoHoverE;
 vector<float>   *phoSigmaIEtaIEta;
 vector<float>   *phoSigmaIEtaIPhi;
 vector<float>   *phoSigmaIPhiIPhi;
 vector<float>   *phoE1x3;
 vector<float>   *phoE2x2;
 vector<float>   *phoE2x5Max;
 vector<float>   *phoE5x5;
 vector<float>   *phoESEffSigmaRR;
 vector<float>   *phoSigmaIEtaIEtaFull5x5;
 vector<float>   *phoSigmaIEtaIPhiFull5x5;
 vector<float>   *phoSigmaIPhiIPhiFull5x5;
 vector<float>   *phoE1x3Full5x5;
 vector<float>   *phoE2x2Full5x5;
 vector<float>   *phoE2x5MaxFull5x5;
 vector<float>   *phoE5x5Full5x5;
 vector<float>   *phoR9Full5x5;
 vector<float>   *phoBC1E;
 vector<float>   *phoBC1Eta;
 vector<float>   *phoEcalRecHitSumEtConeDR03;
 vector<float>   *phohcalDepth1TowerSumEtConeDR03;
 vector<float>   *phohcalDepth2TowerSumEtConeDR03;
 vector<float>   *phohcalTowerSumEtConeDR03;
 vector<float>   *photrkSumPtHollowConeDR03;
 vector<ULong64_t> *phoIDbit;
 Int_t           nEle;
 vector<int>     *eleCharge;
 vector<int>     *eleChargeConsistent;
 vector<float>   *eleEn;
 vector<float>   *eleSCEn;
 vector<float>   *eleESEn;
 vector<float>   *eleD0;
 vector<float>   *eleDz;
 vector<float>   *elePt;
 vector<float>   *eleEta;
 vector<float>   *elePhi;
 vector<float>   *eleR9;
 vector<float>   *eleSCEta;
 vector<float>   *eleSCPhi;
 vector<float>   *eleSCRawEn;
 vector<float>   *eleSCEtaWidth;
 vector<float>   *eleSCPhiWidth;
 vector<float>   *eleHoverE;
 vector<float>   *eleEoverP;
 vector<float>   *eleEoverPInv;
 vector<float>   *eleBrem;
 vector<float>   *eledEtaAtVtx;
 vector<float>   *eledPhiAtVtx;
 vector<float>   *eleSigmaIEtaIEta;
 vector<float>   *eleSigmaIEtaIPhi;
 vector<float>   *eleSigmaIPhiIPhi;
 vector<float>   *eleSigmaIEtaIEtaFull5x5;
 vector<int>     *eleConvVeto;
 vector<int>     *eleMissHits;
 vector<float>   *eleESEffSigmaRR;
 vector<float>   *elePFChIso;
 vector<float>   *elePFPhoIso;
 vector<float>   *elePFNeuIso;
 vector<float>   *elePFPUIso;
 vector<float>   *eledEtaseedAtVtx;
 vector<float>   *eleE1x5;
 vector<float>   *eleE2x5;
 vector<float>   *eleE5x5;
 vector<float>   *eleRelIsoWithDBeta;
 vector<float>   *eleE1x5Full5x5;
 vector<float>   *eleE2x5Full5x5;
 vector<float>   *eleE5x5Full5x5;
 vector<float>   *eleR9Full5x5;
 vector<int>     *eleEcalDrivenSeed;
 vector<float>   *eleDr03EcalRecHitSumEt;
 vector<float>   *eleDr03HcalDepth1TowerSumEt;
 vector<float>   *eleDr03HcalDepth2TowerSumEt;
 vector<float>   *eleDr03HcalTowerSumEt;
 vector<float>   *eleDr03TkSumPt;
 vector<float>   *elecaloEnergy;
 vector<float>   *eleTrkdxy;
 vector<ULong64_t> *eleIDbit;
 Int_t           nMu;
 vector<float>   *muPt;
 vector<float>   *muEta;
 vector<float>   *muPhi;
 vector<int>     *muCharge;
 vector<int>     *muType;
 vector<int>     *muIsGood;
 vector<float>   *muD0;
 vector<float>   *muDz;
 vector<float>   *muChi2NDF;
 vector<float>   *muInnerD0;
 vector<float>   *muInnerDz;
 vector<int>     *muTrkLayers;
 vector<int>     *muPixelLayers;
 vector<int>     *muPixelHits;
 vector<int>     *muMuonHits;
 vector<int>     *muStations;
 vector<int>     *muTrkQuality;
 vector<float>   *muIsoTrk;
 vector<float>   *muPFChIso;
 vector<float>   *muPFPhoIso;
 vector<float>   *muPFNeuIso;
 vector<float>   *muPFPUIso;
 vector<float>   *muInnervalidFraction;
 vector<float>   *musegmentCompatibility;
 vector<float>   *muchi2LocalPosition;
 vector<float>   *mutrkKink;
 vector<float>   *muBestTrkPtError;
 vector<float>   *muBestTrkPt;
 Int_t           nTau;
 //   vector<bool>    *pfTausDiscriminationByDecayModeFinding;
 vector<bool>    *pfTausDiscriminationByDecayModeFinding;
 vector<bool>    *tauByLooseElectronRejection;
 vector<bool>    *tauByMediumElectronRejection;
 vector<bool>    *tauByTightElectronRejection;
 vector<bool>    *tauByMVA5LooseElectronRejection;
 vector<bool>    *tauByMVA5MediumElectronRejection;
 vector<bool>    *tauByMVA5TightElectronRejection;
 vector<bool>    *tauByMVA5VTightElectronRejection;
 vector<bool>    *tauByLooseMuonRejection3;
 vector<bool>    *tauByTightMuonRejection3;
 vector<bool>    *tauByMVALooseMuonRejection;
 vector<bool>    *tauByMVAMediumMuonRejection;
 vector<bool>    *tauByMVATightMuonRejection;
 vector<bool>    *tauByLooseCombinedIsolationDeltaBetaCorr3Hits;
 vector<bool>    *tauByMediumCombinedIsolationDeltaBetaCorr3Hits;
 vector<bool>    *tauByTightCombinedIsolationDeltaBetaCorr3Hits;
 vector<float>   *tauCombinedIsolationDeltaBetaCorrRaw3Hits;
 vector<bool>    *tauByVLooseIsolationMVA3newDMwoLT;
 vector<bool>    *tauByLooseIsolationMVA3newDMwoLT;
 vector<bool>    *tauByMediumIsolationMVA3newDMwoLT;
 vector<bool>    *tauByTightIsolationMVA3newDMwoLT;
 vector<bool>    *tauByVTightIsolationMVA3newDMwoLT;
 vector<bool>    *tauByVVTightIsolationMVA3newDMwoLT;
 vector<float>   *tauByIsolationMVA3newDMwoLTraw;
 vector<bool>    *tauByVLooseIsolationMVA3oldDMwLT;
 vector<bool>    *tauByLooseIsolationMVA3oldDMwLT;
 vector<bool>    *tauByMediumIsolationMVA3oldDMwLT;
 vector<bool>    *tauByTightIsolationMVA3oldDMwLT;
 vector<bool>    *tauByVTightIsolationMVA3oldDMwLT;
 vector<bool>    *tauByVVTightIsolationMVA3oldDMwLT;
 vector<float>   *tauByIsolationMVA3oldDMwLTraw;
 vector<bool>    *tauByVLooseIsolationMVA3oldDMwoLT;
 vector<bool>    *tauByLooseIsolationMVA3oldDMwoLT;
 vector<bool>    *tauByTightIsolationMVA3oldDMwoLT;
 vector<bool>    *tauByVTightIsolationMVA3oldDMwoLT;
 vector<bool>    *tauByVVTightIsolationMVA3oldDMwoLT;
 //   vector<float>   *tauByIsolationMVA3newDMwoLTraw;
 vector<bool>    *tauByLooseIsolationMVA3newDMwLT;
 vector<bool>    *tauByVLooseIsolationMVA3newDMwLT;
 vector<bool>    *tauByMediumIsolationMVA3newDMwLT;
 vector<bool>    *tauByTightIsolationMVA3newDMwLT;
 vector<bool>    *tauByVTightIsolationMVA3newDMwLT;
 vector<bool>    *tauByVVTightIsolationMVA3newDMwLT;
 vector<float>   *tauByIsolationMVA3newDMwLTraw;
 vector<float>   *tauEta;
 vector<float>   *tauPhi;
 vector<float>   *tauPt;
 vector<float>   *tauEt;
 vector<float>   *tauCharge;
 vector<float>   *tauP;
 vector<float>   *tauPx;
 vector<float>   *tauPy;
 vector<float>   *tauPz;
 vector<float>   *tauVz;
 vector<float>   *tauEnergy;
 vector<float>   *tauMass;
 vector<float>   *tauDxy;
 vector<float>   *tauZImpact;
 vector<int>     *tauDecayMode;
 vector<bool>    *tauLeadChargedHadronExists;
 vector<float>   *tauLeadChargedHadronEta;
 vector<float>   *tauLeadChargedHadronPhi;
 vector<float>   *tauLeadChargedHadronPt;
 vector<float>   *tauChargedIsoPtSum;
 vector<float>   *tauNeutralIsoPtSum;
 vector<float>   *tauPuCorrPtSum;
 vector<float>   *tauNumSignalPFChargedHadrCands;
 vector<float>   *tauNumSignalPFNeutrHadrCands;
 vector<float>   *tauNumSignalPFGammaCands;
 vector<float>   *tauNumSignalPFCands;
 vector<float>   *tauNumIsolationPFChargedHadrCands;
 vector<float>   *tauNumIsolationPFNeutrHadrCands;
 vector<float>   *tauNumIsolationPFGammaCands;
 vector<float>   *tauNumIsolationPFCands;
 Int_t           nJet;
 vector<float>   *jetPt;
 vector<float>   *jetEta;
 vector<float>   *jetPhi;
 vector<float>   *jetCHF;
 vector<float>   *jetNHF;
 vector<float>   *jetCEF;
 vector<float>   *jetNEF;
 vector<int>     *jetNCH;
 vector<float>   *jetHFHAE;
 vector<float>   *jetHFEME;
 vector<int>     *jetNConstituents;
 vector<float>   *jetCombinedSecondaryVtxBJetTags;
 vector<float>   *jetJetProbabilityBJetTags;
 vector<float>   *jetJetBProbabilityBJetTags;
 vector<float>   *jetTrackCountingHighPurBJetTags;
 vector<float>   *jetTrackCountingHighEffBJetTags;
 vector<float>   *jetSimpleSecondaryVertexHighEffBJetTags;
 vector<float>   *jetSimpleSecondaryVertexHighPurBJetTags;
 vector<int>     *jetPartonID;
 vector<bool>    *jetPFLooseId;
 Int_t           nAK8Jet;
 vector<float>   *AK8JetPt;
 vector<float>   *AK8JetEta;
 vector<float>   *AK8JetPhi;
 vector<float>   *AK8JetMass;
 vector<float>   *AK8Jet_tau1;
 vector<float>   *AK8Jet_tau2;
 vector<float>   *AK8Jet_tau3;
 vector<float>   *AK8JetCHF;
 vector<float>   *AK8JetNHF;
 vector<float>   *AK8JetCEF;
 vector<float>   *AK8JetNEF;
 vector<int>     *AK8JetNCH;
 vector<int>     *AK8Jetnconstituents;
 vector<float>   *AK8CHSSoftDropJetMass;
 
 
 Int_t     run;
 Long64_t  event;
 Int_t     lumis;
 Bool_t    isData;
 Int_t     nVtx;
 Int_t     nTrksPV;
 float     rho;
 ULong64_t  HLT50ns;
 ULong64_t HLTIsPrescaled;
 float     genMET;
 float     genMETPhi;
 float     pfMET;
 float     pfMETPhi;
 float     pfMETsumEt;
 float     pfMETmEtSig;
 float     pfMETSig;
 Int_t   nTau_;
 ULong64_t HLT;
 ULong64_t  HLTEleMuX_;
 
 vector<int> *    mcPID;
 vector<int> *    mcStatus;
 vector<float> *  mcPt;
 vector<float>  * mcMass;
 vector<float>   *mcEta;
 vector<float>  * mcPhi;
 vector<float>  * mcE;
 vector<float>  * mcEt;
 
 //Tau Id & Isolation
 vector<bool> *   pfTausDiscriminationByDecayModeFinding_;
 vector<bool> *   pfTausDiscriminationByDecayModeFindingNewDMs_;
 vector<bool> *   tauByLooseElectronRejection_;
 vector<bool> *   tauByMediumElectronRejection_;
 vector<bool> *   tauByTightElectronRejection_;
 vector<bool> *   tauByMVA5LooseElectronRejection_;
 vector<bool> *   tauByMVA5MediumElectronRejection_;
 vector<bool> *   tauByMVA5TightElectronRejection_;
 vector<bool> *   tauByMVA5VTightElectronRejection_;
 vector<bool> *   tauByLooseMuonRejection3_;
 vector<bool> *   tauByTightMuonRejection3_;
 vector<bool> *   tauByMVALooseMuonRejection_;
 vector<bool> *   tauByMVAMediumMuonRejection_;
 vector<bool> *   tauByMVATightMuonRejection_;
 vector<bool> *   tauByLooseCombinedIsolationDeltaBetaCorr3Hits_;
 vector<bool> *   tauByMediumCombinedIsolationDeltaBetaCorr3Hits_;
 vector<bool> *   tauByTightCombinedIsolationDeltaBetaCorr3Hits_;
 vector<float> *   tauCombinedIsolationDeltaBetaCorrRaw3Hits_;
 vector<bool> *   tauByVLooseIsolationMVA3newDMwoLT_;
 vector<bool> *   tauByLooseIsolationMVA3newDMwoLT_;
 vector<bool> *   tauByMediumIsolationMVA3newDMwoLT_;
 vector<bool> *   tauByTightIsolationMVA3newDMwoLT_;
 vector<bool> *   tauByVTightIsolationMVA3newDMwoLT_;
 vector<bool> *   tauByVVTightIsolationMVA3newDMwoLT_;
 vector<float> *   tauByIsolationMVA3newDMwoLTraw_;
 vector<bool> *   tauByVLooseIsolationMVA3oldDMwLT_;
 vector<bool> *   tauByLooseIsolationMVA3oldDMwLT_;
 vector<bool> *   tauByMediumIsolationMVA3oldDMwLT_;
 vector<bool> *   tauByTightIsolationMVA3oldDMwLT_;
 vector<bool> *   tauByVTightIsolationMVA3oldDMwLT_;
 vector<bool> *   tauByVVTightIsolationMVA3oldDMwLT_;
 vector<float> *   tauByIsolationMVA3oldDMwLTraw_;
 vector<bool> *   tauByVLooseIsolationMVA3oldDMwoLT_;
 vector<bool> *   tauByLooseIsolationMVA3oldDMwoLT_;
 vector<bool> *   tauByTightIsolationMVA3oldDMwoLT_;
 vector<bool> *   tauByVTightIsolationMVA3oldDMwoLT_;
 vector<bool> *   tauByVVTightIsolationMVA3oldDMwoLT_;
 vector<float> *   tauByIsolationMVA3oldDMwoLTraw_;
 vector<bool> *   tauByVLooseIsolationMVA3newDMwLT_;
 vector<bool> *   tauByLooseIsolationMVA3newDMwLT_;
 vector<bool> *   tauByMediumIsolationMVA3newDMwLT_;
 vector<bool> *   tauByTightIsolationMVA3newDMwLT_;
 vector<bool> *   tauByVTightIsolationMVA3newDMwLT_;
 vector<bool> *   tauByVVTightIsolationMVA3newDMwLT_;
 vector<float> *   tauByIsolationMVA3newDMwLTraw_;
 
 //Tau Kinematics
 vector<float> * tauEta_;
 vector<float> * tauPhi_;
 vector<float> * tauPt_;
 vector<float> * tauEt_;
 vector<float> * tauCharge_;
 vector<int> *   tauDecayMode_;
 vector<float> * tauP_;
 vector<float> * tauPx_;
 vector<float> * tauPy_;
 vector<float> * tauPz_;
 vector<float> * tauVz_;
 vector<float> * tauEnergy_;
 vector<float> * tauMass_;
 vector<float> * tauDxy_;
 vector<float> * tauZImpact_;
 
 //Tau Ingredients
 vector<float> * tauChargedIsoPtSum_;
 vector<float> * tauNeutralIsoPtSum_;
 vector<float> * tauPuCorrPtSum_;
 vector<float> * tauNumSignalPFChargedHadrCands_;
 vector<float> * tauNumSignalPFNeutrHadrCands_;
 vector<float> * tauNumSignalPFGammaCands_;
 vector<float> * tauNumSignalPFCands_;
 vector<float> * tauNumIsolationPFChargedHadrCands_;
 vector<float> * tauNumIsolationPFNeutrHadrCands_;
 vector<float> * tauNumIsolationPFGammaCands_;
 vector<float> * tauNumIsolationPFCands_;
 //vector<float> * tauEMFraction_;
 //vector<float> * tauHCAL3x3OverPLead_;
 //vector<float> * tauHCALMaxOverPLead_;
 //vector<float> * tauHCALTotOverPLead_;
 //vector<float> * tauIsolationPFChargedHadrCandsPtSum_;
 //vector<float> * tauIsolationPFGammaCandsEtSum_;
 //vector<float> * tauLeadPFChargedHadrCandsignedSipt_;
 vector<bool> *  tauLeadChargedHadronExists_;
 vector<float> * tauLeadChargedHadronEta_;
 vector<float> * tauLeadChargedHadronPhi_;
 vector<float> * tauLeadChargedHadronPt_;
 
 
 
 //Mu
 Int_t          nMu_;
 vector<float> *  muPt_;
 vector<float> *  muEta_;
 vector<float> *  muPhi_;
 vector<int> *    muCharge_;
 vector<int> *    muType_;
 vector<int> *    muIsGood_;
 //vector<int> *    muID_;
 vector<float> *  muD0_;
 vector<float> *  muDz_;
 vector<float> *  muChi2NDF_;
 vector<float> *  muInnerD0_;
 vector<float> *  muInnerDz_;
 vector<int> *    muTrkLayers_;
 vector<int> *    muPixelLayers_;
 vector<int> *    muPixelHits_;
 vector<int> *    muMuonHits_;
 vector<int> *    muStations_;
 vector<int> *    muTrkQuality_;
 vector<float> *  muIsoTrk_;
 vector<float> *  muPFChIso_;
 vector<float> *  muPFPhoIso_;
 vector<float> *  muPFNeuIso_;
 vector<float> *  muPFPUIso_;
 vector<int> *    muFiredTrgs_;
 
 
 Int_t   nEle_;
 vector<float> * elePt_;
 vector<float> * eleEta_;
 vector<float> * elePhi_;
 vector<float> * elePFChIso_;
 vector<float> * eleIDMVANonTrg_;
 vector<int> * eleCharge_;
 
 
 
 ///SJ
 vector<float> *  muInnervalidFraction_;
 vector<float> *  musegmentCompatibility_;
 vector<float> *  muchi2LocalPosition_;
 vector<float> *  mutrkKink_;
 vector<float> *  muBestTrkPtError_;
 vector<float> *  muBestTrkPt_;
 
 Int_t nJet_;
 
 */

#endif	/* TREE_READER_H */

