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
#include "TF1.h"

const int ptBIN=0;
const int etaBIN=0;
const int etaPOINT=-1;

const float LumiB=5.93;
const float LumiC=2.65;
const float LumiD=4.35;
const float LumiE=4.12;
const float LumiF=3.19;
const float LumiG=7.72;
const float LumiHv2=8.64;
const float LumiHv3=0.22;

float LumiBCDEF=LumiB+LumiC+LumiD+LumiE+LumiF;
float LumiGH=LumiG+LumiHv2+LumiHv3;

std::map<std::string, TH1F*>* myMap1;
std::map<std::string, TH2F*>* myMap2;
std::map<std::string, TH3F*>* myMap3;

void plotFill(std::string name, float x, int nx, float nxmin, float nxmax, double weight=1) {
  if (myMap1->find(name) == myMap1->end())
    (*myMap1)[name] = new TH1F(name.c_str(), name.c_str(), nx, nxmin, nxmax);
  if(name.find("Nor") ==std::string::npos && name.find("Dn") ==std::string::npos && name.find("Up")==std::string::npos)  (*myMap1)[name]->SetDefaultSumw2();
  (*myMap1)[name]->Fill(x,weight);
}

void plotFill(std::string name, float x, float y, int nx, float nxmin, float nxmax, int ny, float nymin, float nymax, double weight=1) {
  if (myMap2->find(name) == myMap2->end())
    (*myMap2)[name] = new TH2F(name.c_str(), name.c_str(), nx, nxmin, nxmax, ny, nymin, nymax);
  (*myMap2)[name]->SetDefaultSumw2();
  (*myMap2)[name]->Fill(x, y,weight);
}

void plotFill(std::string name, float x, float y, float z, int nx, float nxmin, float nxmax, int ny, float nymin, float nymax, int nz, float nzmin, float nzmax, double weight=1) {
  if (myMap3->find(name) == myMap3->end())
    (*myMap3)[name] = new TH3F(name.c_str(), name.c_str(), nx, nxmin, nxmax, ny, nymin, nymax, nz, nzmin, nzmax);
  (*myMap3)[name]->SetDefaultSumw2();
  (*myMap3)[name]->Fill(x, y, z,weight);
}


TH1F* nplot1(std::string name) {
  if (myMap1->find(name) != myMap1->end())
    return (*myMap1)[name];
  else
    return 0;
}

TH3F* nplot3(std::string name) {
  if (myMap3->find(name) != myMap3->end())
    return (*myMap3)[name];
  else
    return 0;
}

TH2F* nplot2(std::string name) {
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
Int_t                             run;
Long64_t                          event;
Int_t                             lumis;
Bool_t                            isData;
Int_t                             nVtx;
Int_t                             nTrksPV;
Bool_t                            isPVGood;
Bool_t                            hasGoodVtx;
Float_t                           vtx;
Float_t                           vty;
Float_t                           vtz;
Float_t                           rho;
Float_t                           rhoCentral;
ULong64_t                         HLTEleMuX;
ULong64_t                         HLTPho;
ULong64_t                         HLTJet;
ULong64_t                         HLTEleMuXIsPrescaled;
ULong64_t                         HLTPhoIsPrescaled;
ULong64_t                         HLTJetIsPrescaled;
std::vector<float>               *pdf;
Float_t                           pthat;
Float_t                           processID;
Float_t                           genWeight;
Float_t                           num_gen_jets;
Float_t                           genHT;
TString                          *EventTag;
Int_t                             nPUInfo;
std::vector<int>                 *nPU;
std::vector<int>                 *puBX;
std::vector<float>               *puTrue;
Int_t                             nMC;
std::vector<int>                 *mcPID;
std::vector<float>               *mcVtx;
std::vector<float>               *mcVty;
std::vector<float>               *mcVtz;
std::vector<float>               *mcPt;
std::vector<float>               *mcMass;
std::vector<float>               *mcEta;
std::vector<float>               *mcPhi;
std::vector<float>               *mcE;
std::vector<float>               *mcEt;
std::vector<int>                 *mcGMomPID;
std::vector<int>                 *mcMomPID;
std::vector<float>               *mcMomPt;
std::vector<float>               *mcMomMass;
std::vector<float>               *mcMomEta;
std::vector<float>               *mcMomPhi;
std::vector<int>                 *mcIndex;
std::vector<unsigned short>      *mcStatusFlag;
std::vector<int>                 *mcParentage;
std::vector<int>                 *mcStatus;
std::vector<float>               *mcCalIsoDR03;
std::vector<float>               *mcTrkIsoDR03;
std::vector<float>               *mcCalIsoDR04;
std::vector<float>               *mcTrkIsoDR04;
std::vector<float>               *mcHadronPt;
std::vector<float>               *mcHadronEta;
std::vector<float>               *mcHadronPhi;
std::vector<float>               *mcHadronMass;
std::vector<float>               *mcHadronE;
std::vector<float>               *mcHadronGMomPID;
std::vector<float>               *mcHadronMomPID;
std::vector<float>               *mcHadronMomPt;
std::vector<float>               *mcHadronMomMass;
std::vector<float>               *mcHadronMomEta;
std::vector<float>               *mcHadronMomPhi;

Int_t                             metFilters;
Float_t                           genMET;
Float_t                           genMETPhi;
Float_t                           pfMET;
Double_t                          signif_dxx;
Double_t                          signif_dyy;
Double_t                          signif_dxy;
Double_t                          signif_dyx;
Float_t                           pfMETPhi;
Float_t                           pfMETsumEt;
Float_t                           pfMETmEtSig;
Float_t                           pfMETSig;
Float_t                           pfMET_T1JERUp;
Float_t                           pfMET_T1JERDo;
Float_t                           pfMET_T1JESUp;
Float_t                           pfMET_T1JESDo;
Float_t                           pfMET_T1MESUp;
Float_t                           pfMET_T1MESDo;
Float_t                           pfMET_T1EESUp;
Float_t                           pfMET_T1EESDo;
Float_t                           pfMET_T1PESUp;
Float_t                           pfMET_T1PESDo;
Float_t                           pfMET_T1TESUp;
Float_t                           pfMET_T1TESDo;
Float_t                           pfMET_T1UESUp;
Float_t                           pfMET_T1UESDo;
Int_t                             nEle;
std::vector<int>                 *eleCharge;
std::vector<int>                 *eleChargeConsistent;
std::vector<int>                 *ele2ndChargeConsistent;
std::vector<float>               *eleEn;
std::vector<float>               *eleSCEn;
std::vector<float>               *eleESEn;
std::vector<float>               *eleESEnP1;
std::vector<float>               *eleESEnP2;
std::vector<float>               *eleD0;
std::vector<float>               *eleDz;
std::vector<float>               *elePt;
std::vector<float>               *eleEta;
std::vector<float>               *elePhi;
std::vector<float>               *eleR9;
std::vector<float>               *eleCalibPt;
std::vector<float>               *eleCalibEn;
std::vector<float>               *eleSCEta;
std::vector<float>               *eleSCPhi;
std::vector<float>               *eleSCRawEn;
std::vector<float>               *eleSCEtaWidth;
std::vector<float>               *eleSCPhiWidth;
std::vector<float>               *eleHoverE;
std::vector<float>               *eleEoverP;
std::vector<float>               *eleEoverPout;
std::vector<float>               *eleEoverPInv;
std::vector<float>               *eleBrem;
std::vector<float>               *eledEtaAtVtx;
std::vector<float>               *eledPhiAtVtx;
std::vector<float>               *eledEtaAtCalo;
std::vector<float>               *eleSigmaIEtaIEta;
std::vector<float>               *eleSigmaIEtaIPhi;
std::vector<float>               *eleSigmaIPhiIPhi;
std::vector<float>               *eleSigmaIEtaIEtaFull5x5;
std::vector<float>               *eleSigmaIPhiIPhiFull5x5;
std::vector<int>                 *eleConvVeto;
std::vector<int>                 *eleMissHits;
std::vector<float>               *eleESEffSigmaRR;
std::vector<float>               *elePFChIso;
std::vector<float>               *elePFPhoIso;
std::vector<float>               *elePFNeuIso;
std::vector<float>               *elePFPUIso;
std::vector<float>               *elePFClusEcalIso;
std::vector<float>               *elePFClusHcalIso;
std::vector<float>               *elePFMiniIso;
std::vector<float>               *eleIDMVANonTrg;
std::vector<float>               *eleIDMVA;
std::vector<float>               *eleIDMVATrg;
std::vector<float>               *eledEtaseedAtVtx;
std::vector<float>               *eleE1x5;
std::vector<float>               *eleE2x5;
std::vector<float>               *eleE5x5;
std::vector<float>               *eleE1x5Full5x5;
std::vector<float>               *eleE2x5Full5x5;
std::vector<float>               *eleE5x5Full5x5;
std::vector<float>               *eleR9Full5x5;
std::vector<int>                 *eleEcalDrivenSeed;
std::vector<float>               *eleDr03EcalRecHitSumEt;
std::vector<float>               *eleDr03HcalDepth1TowerSumEt;
std::vector<float>               *eleDr03HcalDepth2TowerSumEt;
std::vector<float>               *eleDr03HcalTowerSumEt;
std::vector<float>               *eleDr03TkSumPt;
std::vector<float>               *elecaloEnergy;
std::vector<float>               *eleTrkdxy;
std::vector<float>               *eleKFHits;
std::vector<float>               *eleKFChi2;
std::vector<std::vector<float> > *eleGSFPt;
std::vector<std::vector<float> > *eleGSFEta;
std::vector<std::vector<float> > *eleGSFPhi;
std::vector<std::vector<float> > *eleGSFCharge;
std::vector<std::vector<int> >   *eleGSFHits;
std::vector<std::vector<int> >   *eleGSFMissHits;
std::vector<std::vector<int> >   *eleGSFNHitsMax;
std::vector<std::vector<float> > *eleGSFVtxProb;
std::vector<std::vector<float> > *eleGSFlxyPV;
std::vector<std::vector<float> > *eleGSFlxyBS;
std::vector<std::vector<float> > *eleBCEn;
std::vector<std::vector<float> > *eleBCEta;
std::vector<std::vector<float> > *eleBCPhi;
std::vector<std::vector<float> > *eleBCS25;
std::vector<std::vector<float> > *eleBCS15;
std::vector<std::vector<float> > *eleBCSieie;
std::vector<std::vector<float> > *eleBCSieip;
std::vector<std::vector<float> > *eleBCSipip;
std::vector<int>                 *eleFiredTrgs;
std::vector<unsigned short>      *eleIDbit;
Int_t                             nMu;
std::vector<float>               *muPt;
std::vector<float>               *muEn;
std::vector<float>               *muEta;
std::vector<float>               *muPhi;
std::vector<int>                 *muCharge;
std::vector<int>                 *muType;
std::vector<bool>                *muIsLooseID;
std::vector<bool>                *muIsMediumID;
std::vector<bool>                *muIsTightID;
std::vector<bool>                *muIsSoftID;
std::vector<bool>                *muIsHighPtID;
std::vector<float>               *muD0;
std::vector<float>               *muDz;
std::vector<float>               *muChi2NDF;
std::vector<float>               *muInnerD0;
std::vector<float>               *muInnerDz;
std::vector<int>                 *muTrkLayers;
std::vector<int>                 *muPixelLayers;
std::vector<int>                 *muPixelHits;
std::vector<int>                 *muMuonHits;
std::vector<int>                 *muStations;
std::vector<int>                 *muMatches;
std::vector<int>                 *muTrkQuality;
std::vector<float>               *muIsoTrk;
std::vector<float>               *muPFChIso;
std::vector<float>               *muPFPhoIso;
std::vector<float>               *muPFNeuIso;
std::vector<float>               *muPFPUIso;
std::vector<float>               *muPFMiniIso;
std::vector<int>                 *muFiredTrgs;
std::vector<float>               *muInnervalidFraction;
std::vector<float>               *musegmentCompatibility;
std::vector<float>               *muchi2LocalPosition;
std::vector<float>               *mutrkKink;
std::vector<float>               *muBestTrkPtError;
std::vector<float>               *muBestTrkPt;
std::vector<UShort_t>            *muIDbit;
std::vector<UShort_t>            *muFiredTrgs_;
Int_t                             nTau;
std::vector<bool>                *taupfTausDiscriminationByDecayModeFinding;
std::vector<bool>                *taupfTausDiscriminationByDecayModeFindingNewDMs;
std::vector<bool>                *tauByMVA6VLooseElectronRejection;
std::vector<bool>                *tauByMVA6LooseElectronRejection;
std::vector<bool>                *tauByMVA6MediumElectronRejection;
std::vector<bool>                *tauByMVA6TightElectronRejection;
std::vector<bool>                *tauByMVA6VTightElectronRejection;
std::vector<bool>                *tauByLooseMuonRejection3;
std::vector<bool>                *tauByTightMuonRejection3;
std::vector<bool>                *tauByLooseCombinedIsolationDeltaBetaCorr3Hits;
std::vector<bool>                *tauByMediumCombinedIsolationDeltaBetaCorr3Hits;
std::vector<bool>                *tauByTightCombinedIsolationDeltaBetaCorr3Hits;
std::vector<float>               *tauCombinedIsolationDeltaBetaCorrRaw3Hits;
std::vector<float>               *tauByIsolationMVArun2v1DBnewDMwLTraw;
std::vector<float>               *tauByIsolationMVArun2v1DBoldDMwLTraw;
std::vector<float>               *tauByIsolationMVArun2v1PWnewDMwLTraw;
std::vector<float>               *tauByIsolationMVArun2v1PWoldDMwLTraw;
std::vector<bool>                *tauByVTightIsolationMVArun2v1DBnewDMwLT;
std::vector<bool>                *tauByVTightIsolationMVArun2v1DBoldDMwLT;
std::vector<bool>                *tauByVTightIsolationMVArun2v1PWnewDMwLT;
std::vector<bool>                *tauByVTightIsolationMVArun2v1PWoldDMwLT;
std::vector<bool>                *tauByTightIsolationMVArun2v1DBnewDMwLT;
std::vector<bool>                *tauByTightIsolationMVArun2v1DBoldDMwLT;
std::vector<bool>                *tauByTightIsolationMVArun2v1PWnewDMwLT;
std::vector<bool>                *tauByTightIsolationMVArun2v1PWoldDMwLT;
std::vector<bool>                *tauByMediumIsolationMVArun2v1DBnewDMwLT;
std::vector<bool>                *tauByMediumIsolationMVArun2v1DBoldDMwLT;
std::vector<bool>                *tauByMediumIsolationMVArun2v1PWnewDMwLT;
std::vector<bool>                *tauByMediumIsolationMVArun2v1PWoldDMwLT;
std::vector<bool>                *tauByLooseIsolationMVArun2v1DBnewDMwLT;
std::vector<bool>                *tauByLooseIsolationMVArun2v1DBoldDMwLT;
std::vector<bool>                *tauByLooseIsolationMVArun2v1PWnewDMwLT;
std::vector<bool>                *tauByLooseIsolationMVArun2v1PWoldDMwLT;
std::vector<bool>                *tauByVLooseIsolationMVArun2v1DBnewDMwLT;
std::vector<bool>                *tauByVLooseIsolationMVArun2v1DBoldDMwLT;
std::vector<bool>                *tauByVLooseIsolationMVArun2v1PWnewDMwLT;
std::vector<bool>                *tauByVLooseIsolationMVArun2v1PWoldDMwLT;
std::vector<float>               *tauEta;
std::vector<float>               *tauPhi;
std::vector<float>               *tauPt;
std::vector<float>               *tauEt;
std::vector<float>               *tauCharge;
std::vector<float>               *tauP;
std::vector<float>               *tauPx;
std::vector<float>               *tauPy;
std::vector<float>               *tauPz;
std::vector<float>               *tauVz;
std::vector<float>               *tauEnergy;
std::vector<float>               *tauMass;
std::vector<float>               *tauDxy;
std::vector<float>               *tauZImpact;
std::vector<int>                 *tauDecayMode;
std::vector<bool>                *tauLeadChargedHadronExists;
std::vector<float>               *tauLeadChargedHadronEta;
std::vector<float>               *tauLeadChargedHadronPhi;
std::vector<float>               *tauLeadChargedHadronPt;
std::vector<float>               *tauChargedIsoPtSum;
std::vector<float>               *tauNeutralIsoPtSum;
std::vector<float>               *tauPuCorrPtSum;
std::vector<int>                 *tauNumSignalPFChargedHadrCands;
std::vector<int>                 *tauNumSignalPFNeutrHadrCands;
std::vector<int>                 *tauNumSignalPFGammaCands;
std::vector<int>                 *tauNumSignalPFCands;
std::vector<int>                 *tauNumIsolationPFChargedHadrCands;
std::vector<int>                 *tauNumIsolationPFNeutrHadrCands;
std::vector<int>                 *tauNumIsolationPFGammaCands;
std::vector<int>                 *tauNumIsolationPFCands;
std::vector<float>               *taufootprintCorrection;
std::vector<float>               *tauphotonPtSumOutsideSignalCone;
std::vector<float>               *taudz;
std::vector<float>               *taudxy;
Int_t                             nJet;
std::vector<float>               *jetPt;
std::vector<float>               *jetEn;
std::vector<float>               *jetEta;
std::vector<float>               *jetPhi;
std::vector<float>               *jetRawPt;
std::vector<float>               *jetRawEn;
std::vector<float>               *jetMt;
std::vector<float>               *jetArea;
std::vector<float>               *jetLeadTrackPt;
std::vector<float>               *jetLeadTrackEta;
std::vector<float>               *jetLeadTrackPhi;
std::vector<int>                 *jetLepTrackPID;
std::vector<float>               *jetLepTrackPt;
std::vector<float>               *jetLepTrackEta;
std::vector<float>               *jetLepTrackPhi;
std::vector<float>               *jetpfCombinedInclusiveSecondaryVertexV2BJetTags;
std::vector<float>               *jetJetProbabilityBJetTags;
std::vector<float>               *jetpfCombinedMVAV2BJetTags;
std::vector<int>                 *jetPartonID;
std::vector<int>                 *jetHadFlvr;
std::vector<int>                 *jetGenJetIndex;
std::vector<float>               *jetGenJetEn;
std::vector<float>               *jetGenJetPt;
std::vector<float>               *jetGenJetEta;
std::vector<float>               *jetGenJetPhi;
std::vector<int>                 *jetGenPartonID;
std::vector<float>               *jetGenEn;
std::vector<float>               *jetGenPt;
std::vector<float>               *jetGenEta;
std::vector<float>               *jetGenPhi;
std::vector<int>                 *jetGenPartonMomID;
std::vector<bool>                *jetPFLooseId;
std::vector<float>               *jetPUidFullDiscriminant;
std::vector<float>               *jetJECUnc;
std::vector<int>                 *jetFiredTrgs;
std::vector<float>               *jetCHF;
std::vector<float>               *jetCSV2BJetTags;
std::vector<float>               *jetNHF;
std::vector<float>               *jetCEF;
std::vector<float>               *jetNEF;
std::vector<int>                 *jetNCH;
std::vector<float>               *jetVtxPt;
std::vector<float>               *jetVtxMass;
std::vector<float>               *jetVtxNtrks;
std::vector<float>               *jetVtx3DVal;
std::vector<float>               *jetVtx3DSig;

#endif	/* TREE_READER_H */

