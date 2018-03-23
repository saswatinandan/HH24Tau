#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <TLorentzVector.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

using namespace std;

class GenParticleInfo : public edm::EDAnalyzer
{
public:
  explicit GenParticleInfo(const edm::ParameterSet& iConfig);
  virtual ~GenParticleInfo();
  virtual void beginJob();
  virtual void endJob();
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
  void finddaughter(const reco::Candidate& part, double genWeight_,bool z, bool fill);
  bool findmother(const reco::Candidate& part);
  void getdaughter(const reco::Candidate& part);
private:
  const edm::InputTag genParticleTag_;
  edm::EDGetTokenT<reco::GenParticleCollection> tok_Genparticle;
  edm::EDGetTokenT<GenEventInfoProduct>            generatorLabel_;
  int pdgId_;
  TH1F *h_ZmuonPt, *h_taumuonPt, *h_Zmass, *h_ZZmass, *h_ZZmass2, *h_1stzmuonpt, *h_1sttaumuonpt, *h_1stzmass, *h_counter, *h_zdaughter, *h_z2daughter, *h_status, *h_zonedaughter, *h_zstatus, *h_massanyz, *h_Z34counter;
  std::vector<TLorentzVector> Muon;
};


GenParticleInfo::GenParticleInfo(const edm::ParameterSet& iConfig) :
  genParticleTag_(iConfig.getUntrackedParameter<edm::InputTag>("genParticleSrc", edm::InputTag("prunedGenParticles"))),
  tok_Genparticle(consumes<reco::GenParticleCollection>(genParticleTag_)),
  generatorLabel_(consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("generatorLabel")))
{
}

GenParticleInfo::~GenParticleInfo() {
}

void GenParticleInfo::beginJob() {
  edm::Service<TFileService> fs;
  h_ZmuonPt = fs->make<TH1F>("pt_ZMuon","pt distribution of Muon with status 1 at gen label coming from z",200,0.,200.);
  h_ZmuonPt->GetXaxis()->SetTitle("muonGenPt");
  h_taumuonPt = fs->make<TH1F>("pt_tauMuon","pt distribution of Muon with status 1 at gen label coming from tau & tau coming from z",200,0.,200.);
  h_Zmass = fs->make<TH1F>("ZMass","Mass distribution of z reconstructed from its daughter muon with status 1",150,0.,150.);
  h_ZZmass = fs->make<TH1F>("ZZMass","Mass distribution of ZZ reconstructed from 4 muons",400,0.,400.);
  h_ZZmass2 = fs->make<TH1F>("ZZMass2","Mass distribution of ZZ at gen label",400,0.,400.);
  h_1stzmuonpt = fs->make<TH1F>("pt_1stZMuon","pt distribution of Muon at gen label coming from directly z with arbitrary status",200,0.,200.);
  h_1sttaumuonpt = fs->make<TH1F>("pt_1sttauMuon","pt distribution of Muon at gen label coming from directly tau with arbitrary status where tau coming from z",200,0.,200.);
  h_1stzmass = fs->make<TH1F>("zmass","True mass distribution of z at gen label",200,0.,200.);
  h_counter= fs->make<TH1F>("zcounter","z counter",5,0.,5.);
  h_Z34counter= fs->make<TH1F>("z34counter","z34 counter",5,0.,5.);
  h_zdaughter= fs->make<TH1F>("zdaughter","# of events having 4 daughter of z",2,0.,2.);
  h_z2daughter= fs->make<TH1F>("z2daughter","# of daughter when there is two z",5,0.,5.);
  h_status = fs->make<TH1F>("zstatus","z status when decaying two another particle",100,0.,100);
  h_zstatus = fs->make<TH1F>("allzstatus","z status",100,0.,100);
  h_zonedaughter = fs->make<TH1F>("zonedaughter","# of daughter when there is one z ",5,0.,5);
  h_massanyz = fs->make<TH1F>("mass_anyz","mass of z with any status ",100,0.,100);

}

void GenParticleInfo::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<reco::GenParticleCollection> genParticles;
  bool found = iEvent.getByToken(tok_Genparticle, genParticles);
  if (!found) return;
  
  edm::Handle<GenEventInfoProduct> genEventInfoHandle;
  iEvent.getByToken(generatorLabel_, genEventInfoHandle);
  double genWeight_(1.0);
  if (genEventInfoHandle.isValid()) genWeight_ = genEventInfoHandle->weight();

  //  if(iEvent.id().event() != 5511205 && iEvent.id().event() != 5511232 && iEvent.id().event() != 5511217 ) return;
  //std::cout << "Event== " << iEvent.id().event() << "run= " << iEvent.id().run() << "lumi" << iEvent.id().luminosityBlock() << std::endl;
  std::vector<TLorentzVector> zp, zp2;
  int counter(0), zdaughter(0);

  for (const reco::GenParticle& v: *genParticles) {
    
    const reco::Candidate& part = dynamic_cast<const reco::Candidate&>(v);

    //    std::cout << "part= " << &v << "\t" << "pdgid= " << v.pdgId() << "\t" << "status= " << v.status() << "\t" << "mass= " << v.mass() << "\t" << "pt= " << v.pt() << "pz= " << v.pz() << "\t" << "energy= " << v.energy() << "\t" << "daughter= " << v.numberOfDaughters();
    if(v.numberOfMothers()) {
      //      std::cout << "mom= " << v.mother(0) << "\t" << "mompdgId= " << v.mother(0)->pdgId() << "\t" << "momstatus= " << v.mother(0)->status() << std::endl;
    }
    else {
      //std::cout << "\t" << "no mother found" << std::endl;
    }

    if(fabs(v.pdgId()) == 23) {
      h_zstatus->Fill(v.status());
      getdaughter(v);
      h_massanyz->Fill(v.mass());
    }

    if(fabs(v.pdgId()) == 23 && v.status() ==62) {

      counter +=1;
      zdaughter = v.numberOfDaughters();
      if(v.numberOfDaughters() ==3) {h_Z34counter->Fill(3);std::cout << "# of daughter3" << "Event= " << iEvent.id().event() << std::endl;}
      if(v.numberOfDaughters() >4) {h_Z34counter->Fill(4);std::cout << "# of daughter4" << "Event= " << iEvent.id().event() << std::endl;}
      if(v.numberOfDaughters() ==4) {
	//	std::cout << "1st daughter= " << v.daughter(0)->pdgId() << "2nd daughter= " << v.daughter(1)->pdgId() << "3rd daughter= " << v.daughter(2)->pdgId() << "4th daughter= "  << v.daughter(3)->pdgId() << std::endl;
	h_zdaughter->Fill(1);
      }
      h_1stzmass->Fill(v.mass());
      Muon.clear();
      finddaughter(part, genWeight_,true,true);
      if(Muon.size()==2) {
	zp.push_back(Muon[0]);
	zp.push_back(Muon[1]);
	h_Zmass->Fill((Muon[0]+Muon[1]).M(),genWeight_);
	TLorentzVector z;
	z.SetPtEtaPhiE(v.pt(),v.eta(),v.phi(),v.energy());
	zp2.push_back(z);
      }
      for (unsigned k=0; k<Muon.size(); ++k) {
	h_ZmuonPt->Fill(Muon[k].Pt(), genWeight_);
      }
    }
    else if(fabs(v.pdgId()) == 15 && v.status() ==2) {
      bool mom = findmother(part);
      if(mom) {
	Muon.clear();
	finddaughter(part, genWeight_,false,true);
	for (unsigned k=0; k<Muon.size(); ++k) {
	  h_taumuonPt->Fill(Muon[k].Pt(), genWeight_);
	}
      }
    }
  }
  if (zp.size() == 4) h_ZZmass->Fill((zp[0]+zp[1]+zp[2]+zp[3]).M(),genWeight_);
  if (zp2.size() == 2) h_ZZmass2->Fill((zp2[0]+zp2[1]).M(),genWeight_);
  if(counter==1) h_zonedaughter->Fill(zdaughter);
  h_counter->Fill(counter);
  //  if(counter==0) std::cout << "Event== " << iEvent.id().event() << std::endl;
  if(counter==2) {
    for (const reco::GenParticle& v: *genParticles) {
      if(fabs(v.pdgId()) == 23 && v.status() ==62) {
        h_z2daughter->Fill(v.numberOfDaughters());
      }
    }
  }
}

  

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(GenParticleInfo);

void GenParticleInfo::finddaughter(const reco::Candidate& v, double genWeight_, bool z, bool fill) {

  for(unsigned int i=0; i<v.numberOfDaughters(); i++) {
    const reco::Candidate& daughter = *(v.daughter(i));
    if(fabs(daughter.pdgId()) != 13) continue;
    if(fill && z) {
      h_1stzmuonpt->Fill(daughter.pt(),genWeight_);
    }
    else if(fill && !z) {
      h_1sttaumuonpt->Fill(daughter.pt(),genWeight_);
    }
    if(daughter.status() !=1) finddaughter(daughter,genWeight_,z,false);
    if (daughter.status() ==1) {
      TLorentzVector muon;
      muon.SetPtEtaPhiE(daughter.pt(),daughter.eta(),daughter.phi(),daughter.energy());
      Muon.push_back(muon);
    }
  }
}

void GenParticleInfo::getdaughter(const reco::Candidate& v) {

  for(unsigned int i=0; i<v.numberOfDaughters(); i++) {
    const reco::Candidate& daughter = *(v.daughter(i));
    if(daughter.pdgId() == 22) continue;
    if(daughter.pdgId() == v.pdgId()) {
      getdaughter(daughter);
    }
    else {
      h_status->Fill(v.status());
    }
    break;
  }
}


bool GenParticleInfo::findmother(const reco::Candidate& v) {

  for(unsigned int i=0; i<v.numberOfMothers(); i++){
    const reco::Candidate& mom = *(v.mother(i));
    if(fabs(mom.pdgId()) !=23 || mom.status() !=62) findmother(mom);
    if(fabs(mom.pdgId()) ==23 && mom.status() ==62) return true;
  }
  return false;
} 

void GenParticleInfo::endJob() {

}
