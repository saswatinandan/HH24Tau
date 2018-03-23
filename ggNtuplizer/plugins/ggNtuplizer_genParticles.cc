#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "ggAnalysis/ggNtuplizer/interface/ggNtuplizer.h"
#include "ggAnalysis/ggNtuplizer/interface/GenParticleParentage.h"

using namespace std;
void Hadron(const reco::Candidate& d, TLorentzVector& hadron, int& npiplus, int& npiminus, int& npizero);
void dump(edm::Handle<vector<reco::GenParticle> > genParticlesHandle);
// (local) variables associated with tree branches
vector<float>    pdf_;
float            pthat_;
float            processID_;
float            genWeight_;
float            genHT_;
float            num_gen_jets_;
TString          EventTag_;
float            pdfWeight_;     
vector<float>    pdfSystWeight_;

Int_t            nPUInfo_;
vector<int>      nPU_;
vector<int>      puBX_;
vector<float>    puTrue_;

Int_t            nMC_;
vector<int>      mcPID;
vector<float>    mcVtx;
vector<float>    mcVty;
vector<float>    mcVtz;
vector<float>    mcPt;
vector<float>    mcMass;
vector<float>    mcEta;
vector<float>    mcPhi;
vector<float>    mcE;
vector<float> mcEt;
vector<int>      mcGMomPID;
vector<float>      mcGMomPt;
vector<float>      mcGMomEta;
vector<float>      mcGMomPhi;
vector<float>      mcGMomMass;
vector<int>      mcMomPID;
vector<float>    mcMomPt;
vector<float>    mcMomMass;
vector<float>    mcMomEta;
vector<float>    mcMomPhi;
vector<UShort_t> mcStatusFlag;
vector<int>      mcParentage;
vector<int>      mcStatus;
vector<float>    mcCalIsoDR03;
vector<float>    mcTrkIsoDR03;
vector<float>    mcCalIsoDR04;
vector<float>    mcTrkIsoDR04;

vector<float>    mcHadronPt;
vector<float>    mcHadronMass;
vector<float>    mcHadronEta;
vector<float>    mcHadronPhi;
vector<float>    mcHadronE;
vector<float> mcHadronEt;
vector<int>      mcHadronGMomPID;
vector<float>      mcHadronGMomPt;
vector<float>      mcHadronGMomEta;
vector<float>      mcHadronGMomPhi;
vector<float>      mcHadronGMomMass;
vector<int>      mcHadronMomPID;
vector<float>    mcHadronMomPt;
vector<float>    mcHadronMomMass;
vector<float>    mcHadronMomEta;
vector<float>    mcHadronMomPhi;
vector<float>    npiplus;
vector<float>   npiminus;
vector<float>   npizero;
//vector<const reco::Candidate*> MomRef;
//vector<int>      mcIndex;                                                                                                                    

float getGenCalIso(edm::Handle<reco::GenParticleCollection> handle,
                   reco::GenParticleCollection::const_iterator thisPart,
                   float dRMax, bool removeMu, bool removeNu) {

  // Returns Et sum.
  float etSum = 0;

  for (reco::GenParticleCollection::const_iterator p = handle->begin(); p != handle->end(); ++p) {
    if (p == thisPart) continue;
    if (p->status() != 1) continue;

    // has to come from the same collision
    if (thisPart->collisionId() != p->collisionId())
      continue;

    int pdgCode = abs(p->pdgId());

    // skip muons/neutrinos, if requested
    if (removeMu && pdgCode == 13) continue;
    if (removeNu && (pdgCode == 12 || pdgCode == 14 || pdgCode == 16)) continue;
    
    // pass a minimum Et threshold
    // if (p->et() < 0) continue;

    // must be within deltaR cone
    float dR = reco::deltaR(thisPart->momentum(), p->momentum());
    if (dR > dRMax) continue;

    etSum += p->et();
  }

  return etSum;
}

float getGenTrkIso(edm::Handle<reco::GenParticleCollection> handle,
                   reco::GenParticleCollection::const_iterator thisPart, float dRMax) {

  // Returns pT sum without counting neutral particles.
  float ptSum = 0;

  for (reco::GenParticleCollection::const_iterator p = handle->begin(); p != handle->end(); ++p) {
    if (p == thisPart) continue;
    if (p->status() != 1) continue;
    if (p->charge() == 0) continue;  // do not count neutral particles

    // has to come from the same collision
    if (thisPart->collisionId() != p->collisionId())
      continue;

    // pass a minimum pt threshold
    // if (p->pt() < 0) continue;
    // must be within deltaR cone
    float dR = reco::deltaR(thisPart->momentum(), p->momentum());
    if (dR > dRMax) continue;

    ptSum += p->pt();
  }

  return ptSum;
}

void ggNtuplizer::branchesGenInfo(TTree* tree, edm::Service<TFileService> &fs) {

  tree->Branch("pdf",           &pdf_);
  tree->Branch("pthat",         &pthat_);
  tree->Branch("processID",     &processID_);
  tree->Branch("genWeight",     &genWeight_);
  tree->Branch("genHT",         &genHT_);
  tree->Branch("num_gen_jets",         &num_gen_jets_);
  if (dumpPDFSystWeight_) {
    tree->Branch("pdfWeight",     &pdfWeight_);
    tree->Branch("pdfSystWeight", &pdfSystWeight_);
  }
  tree->Branch("EventTag",      &EventTag_);

  tree->Branch("nPUInfo",       &nPUInfo_);
  tree->Branch("nPU",           &nPU_);
  tree->Branch("puBX",          &puBX_);
  tree->Branch("puTrue",        &puTrue_);

  hPU_        = fs->make<TH1F>("hPU",        "number of pileup",      200,  0, 200);
  hPUTrue_    = fs->make<TH1F>("hPUTrue",    "number of true pilepu", 1000, 0, 200);
  hGenWeight_ = fs->make<TH1F>("hGenWeight", "Gen weights",           2,    0, 2);
}
void ggNtuplizer::branchesGenPart(TTree* tree) {

  tree->Branch("nMC",          &nMC_);
  tree->Branch("mcPID",        &mcPID);
  tree->Branch("mcVtx",        &mcVtx);
  tree->Branch("mcVty",        &mcVty);
  tree->Branch("mcVtz",        &mcVtz);
  tree->Branch("mcPt",         &mcPt);
  tree->Branch("mcMass",       &mcMass);
  tree->Branch("mcEta",        &mcEta);
  tree->Branch("mcPhi",        &mcPhi);
  tree->Branch("mcE",          &mcE);
  tree->Branch("mcEt",         &mcEt);
  tree->Branch("mcGMomPID",    &mcGMomPID);
  tree->Branch("mcGMomPt",    &mcGMomPt);
  tree->Branch("mcGMomEta",    &mcGMomEta);
  tree->Branch("mcGMomPhi",    &mcGMomPhi);
  tree->Branch("mcGMomMass",    &mcGMomMass);
  tree->Branch("mcMomPID",     &mcMomPID);
  tree->Branch("mcMomPt",      &mcMomPt);
  tree->Branch("mcMomMass",    &mcMomMass);
  tree->Branch("mcMomEta",     &mcMomEta);
  tree->Branch("mcMomPhi",     &mcMomPhi);
  tree->Branch("mcStatusFlag", &mcStatusFlag); //-999:non W or Z, 1:hardronic, 2:e, 3:mu, 4:tau
  tree->Branch("mcParentage",  &mcParentage);  // 16*lepton + 8*boson + 4*non-prompt + 2*qcd + exotics
  tree->Branch("mcStatus",     &mcStatus);     // status of the particle
  tree->Branch("mcCalIsoDR03", &mcCalIsoDR03);
  tree->Branch("mcTrkIsoDR03", &mcTrkIsoDR03);
  tree->Branch("mcCalIsoDR04", &mcCalIsoDR04);
  tree->Branch("mcTrkIsoDR04", &mcTrkIsoDR04);
  tree->Branch("mcHadronPt",         &mcHadronPt);
  tree->Branch("mcHadronMass",       &mcHadronMass);
  tree->Branch("mcHadronEta",        &mcHadronEta);
  tree->Branch("mcHadronPhi",        &mcHadronPhi);
  tree->Branch("mcHadronE",          &mcHadronE);
  tree->Branch("mcHadronGMomPID",    &mcHadronGMomPID);
  tree->Branch("mcHadronGMomPt",    &mcHadronGMomPt);
  tree->Branch("mcHadronGMomEta",    &mcHadronGMomEta);
  tree->Branch("mcHadronGMomPhi",    &mcHadronGMomPhi);
  tree->Branch("mcHadronGMomMass",    &mcHadronGMomMass);
  tree->Branch("mcHadronMomPID",     &mcHadronMomPID);
  tree->Branch("mcHadronMomPt",      &mcHadronMomPt);
  tree->Branch("mcHadronMomMass",    &mcHadronMomMass);
  tree->Branch("mcHadronMomEta",     &mcHadronMomEta);
  tree->Branch("mcHadronMomPhi",     &mcHadronMomPhi);
  tree->Branch("npiplus",     &npiplus);
  tree->Branch("npiminus",     &npiminus);
  tree->Branch("npizero",     &npizero);
  //  tree->Branch("MomRef",     &MomRef);
}

void ggNtuplizer::fillGenInfo(const edm::Event& e) {

  // cleanup from previous execution
  pthat_     = -99;
  processID_ = -99;
  genWeight_ = -99;
  genHT_     = -99;
  nPUInfo_   = 0;
  pdfWeight_ = -99;
  EventTag_  = "";
  pdf_          .clear();
  pdfSystWeight_.clear();
  nPU_          .clear();
  puBX_         .clear();
  puTrue_       .clear();

  edm::Handle<GenEventInfoProduct> genEventInfoHandle;
  e.getByToken(generatorLabel_, genEventInfoHandle);

  if (genEventInfoHandle.isValid()) {

    if (genEventInfoHandle->pdf()) {
      pdf_.push_back(genEventInfoHandle->pdf()->id.first);    // PDG ID of incoming parton #1
      pdf_.push_back(genEventInfoHandle->pdf()->id.second);   // PDG ID of incoming parton #2
      pdf_.push_back(genEventInfoHandle->pdf()->x.first);     // x value of parton #1
      pdf_.push_back(genEventInfoHandle->pdf()->x.second);    // x value of parton #2
      pdf_.push_back(genEventInfoHandle->pdf()->xPDF.first); // PDF weight for parton #1
      pdf_.push_back(genEventInfoHandle->pdf()->xPDF.second); // PDF weight for parton #2
      pdf_.push_back(genEventInfoHandle->pdf()->scalePDF);    // scale of the hard interaction
    }

    if (genEventInfoHandle->hasBinningValues())
      pthat_ = genEventInfoHandle->binningValues()[0];
    processID_ = genEventInfoHandle->signalProcessID();
    genWeight_ = genEventInfoHandle->weight();
    if (genWeight_ >= 0) hGenWeight_->Fill(0.5);    
    else hGenWeight_->Fill(1.5);
  } else
    edm::LogWarning("ggNtuplizer") << "no GenEventInfoProduct in event";
  
  // access generator level HT  
  edm::Handle<LHEEventProduct> lheEventProduct;
  e.getByToken(lheEventLabel_, lheEventProduct);
  
  double lheHt = 0.;
  double nOutgoing =0;
  if (lheEventProduct.isValid()){
    const lhef::HEPEUP& lheEvent = lheEventProduct->hepeup();
    std::vector<lhef::HEPEUP::FiveVector> lheParticles = lheEvent.PUP;
    size_t numParticles = lheParticles.size();
    for ( size_t idxParticle = 0; idxParticle < numParticles; ++idxParticle ) {
      int absPdgId = TMath::Abs(lheEvent.IDUP[idxParticle]);
      int status = lheEvent.ISTUP[idxParticle];
      if ( status == 1 && ((absPdgId >= 1 && absPdgId <= 6) || absPdgId == 21) ) { // quarks and gluons
	lheHt += TMath::Sqrt(TMath::Power(lheParticles[idxParticle][0], 2.) + TMath::Power(lheParticles[idxParticle][1], 2.)); // first entry is px, second py
	++nOutgoing;
      } 

      typedef std::vector<std::string>::const_iterator comments_const_iterator;

      comments_const_iterator c_begin = lheEventProduct->comments_begin();
      comments_const_iterator c_end = lheEventProduct->comments_end();

      TString model_params;
      for(comments_const_iterator cit=c_begin; cit!=c_end; ++cit) {
	size_t found = (*cit).find("model");
	if(found != std::string::npos)   { 
	  model_params = *cit;
	}
      }
      EventTag_ = model_params;
    }

    if (dumpPDFSystWeight_) {
      pdfWeight_ = lheEventProduct->originalXWGTUP(); // PDF weight of this event !
      for (unsigned i = 0; i < lheEventProduct->weights().size(); ++i) {
	pdfSystWeight_.push_back(lheEventProduct->weights()[i].wgt);
      }
    }
  }
  genHT_=lheHt;  
  num_gen_jets_ = nOutgoing;
  edm::Handle<vector<PileupSummaryInfo> > genPileupHandle;
  e.getByToken(puCollection_, genPileupHandle);
  
  if (genPileupHandle.isValid()) {
    for (vector<PileupSummaryInfo>::const_iterator pu = genPileupHandle->begin(); pu != genPileupHandle->end(); ++pu) {
      if (pu->getBunchCrossing() == 0) {
        hPU_->Fill(pu->getPU_NumInteractions());
        hPUTrue_->Fill(pu->getTrueNumInteractions());
      }

      nPU_ .push_back(pu->getPU_NumInteractions());
      puTrue_.push_back(pu->getTrueNumInteractions());
      puBX_  .push_back(pu->getBunchCrossing());

      nPUInfo_++;
    }
  }
  else
    edm::LogWarning("ggNtuplizer") << "no PileupSummaryInfo in event";

}

void ggNtuplizer::fillGenPart(const edm::Event& e) {

  // Fills tree branches with generated particle info.

  // cleanup from previous execution
  mcPID       .clear();
  mcVtx       .clear();
  mcVty       .clear();
  mcVtz       .clear();
  mcPt        .clear();
  mcMass      .clear();
  mcEta       .clear();
  mcPhi       .clear();
  mcE         .clear();
  mcEt        .clear();
  mcGMomPID   .clear();
  mcGMomPt   .clear();
  mcGMomEta   .clear();
  mcGMomPhi   .clear();
  mcGMomMass   .clear();
  mcMomPID    .clear();
  mcMomPt     .clear();
  mcMomMass   .clear();
  mcMomEta .clear();
  mcMomPhi    .clear();
  mcStatusFlag.clear();
  mcParentage .clear();
  mcStatus    .clear();
  mcCalIsoDR03.clear();
  mcTrkIsoDR03.clear();
  mcCalIsoDR04.clear();
  mcTrkIsoDR04.clear();
  mcHadronPt        .clear();
  mcHadronMass      .clear();
  mcHadronEta       .clear();
  mcHadronPhi       .clear();
  mcHadronE         .clear();
  mcHadronGMomPID   .clear();
  mcHadronGMomPt   .clear();
  mcHadronGMomEta   .clear();
  mcHadronGMomPhi   .clear();
  mcHadronGMomMass   .clear();
  mcHadronMomPID    .clear();
  mcHadronMomPt     .clear();
  mcHadronMomMass   .clear();
  mcHadronMomEta .clear();
  mcHadronMomPhi    .clear();
  npiplus           .clear();
  npiminus           .clear();
  npizero           .clear();
  //  MomRef.clear();
  nMC_ = 0;
  edm::Handle<vector<reco::GenParticle> > genParticlesHandle;
  e.getByToken(genParticlesCollection_, genParticlesHandle);

  if (!genParticlesHandle.isValid()) {
    edm::LogWarning("ggNtuplizer") << "no reco::GenParticles in event";
    return;
  }

  int genIndex = 0;

  for (vector<reco::GenParticle>::const_iterator ip = genParticlesHandle->begin(); ip != genParticlesHandle->end(); ++ip) {
    genIndex++;
    
    int status = ip->status();
    //bool stableFinalStateParticle = status == 1 && ip->pt() > 5.0;

    bool quarks = abs(ip->pdgId())<7;

    // keep non-FSR photons with pT > 5.0 and all leptons with pT > 3.0;
    bool photonOrLepton =
      (ip->pdgId() == 22 && (ip->isPromptFinalState() || ip->isLastCopy())) ||
      (status == 1 && abs(ip->pdgId()) == 11 && (ip->isPromptFinalState() || ip->isLastCopy())) || 
      (status == 1 && abs(ip->pdgId()) == 13 && (ip->isPromptFinalState() || ip->isLastCopy())) ||
      (status == 1 && (abs(ip->pdgId()) == 12 || abs(ip->pdgId()) == 14 || abs(ip->pdgId()) == 16)) ||
      //   (status == 1 && ( abs(ip->pdgId()) >= 11 && abs(ip->pdgId()) <= 16 ) && ip->pt() > 3.0)  ||
      //(status < 10 && abs(ip->pdgId()) == 15 && ip->pt() > 3.0);
      (status == 1 && ( abs(ip->pdgId()) >= 11 && abs(ip->pdgId()) <= 16 ))  ||   
      (status < 10 && abs(ip->pdgId()) == 15);
    
    /* if((fabs(ip->pdgId()) ==12|| fabs(ip->pdgId()) ==14||fabs(ip->pdgId()) ==16) && ip->status() !=1) {
       cout << "status== " << ip->status() << "pt= " << ip->pt() << "pz= " << ip->pz() << endl;
       const reco::Candidate *p = (const reco::Candidate*)&(*ip);
       for (size_t k=0; k < p->numberOfDaughters(); ++k) {
       const reco::Candidate *dp = p->daughter(k);
       cout << "k= " << k << "pdgid= " << dp->pdgId() << "status= " << dp->status() << "pt= " << ip->pt() << "pz= " << ip->pz() << endl;
       }
       }*/
    
    // select also Z, W, H, top and b 
    bool heavyParticle =
      ((    ip->pdgId()  == 23 && ip->isHardProcess()) || 
       (abs(ip->pdgId()) == 24 && ip->isHardProcess()) || 
       (    ip->pdgId()  == 25 && ip->isHardProcess()) ||
       (abs(ip->pdgId()) ==  6 && ip->isHardProcess()) || 
       (abs(ip->pdgId()) ==  5 && ip->isHardProcess()));
    
    bool newParticle = false;
    for (size_t inp = 0; inp < newparticles_.size(); ++inp) {
      if (abs(ip->pdgId()) == newparticles_[inp]) newParticle = true;
    }
    
    if ( heavyParticle || photonOrLepton || quarks || newParticle ) {
      
      const reco::Candidate *p = (const reco::Candidate*)&(*ip);
      if (!runOnParticleGun_ && !p->mother()) continue;
      mcPID    .push_back(p->pdgId());
      mcVtx    .push_back(p->vx());
      mcVty    .push_back(p->vy());
      mcVtz    .push_back(p->vz());
      mcPt     .push_back(p->pt());
      mcMass .push_back(p->mass());
      mcEta    .push_back(p->eta());
      mcPhi    .push_back(p->phi());
      mcE .push_back(p->energy());
      mcEt     .push_back(p->et());
      mcStatus .push_back(p->status());

      UShort_t tmpStatusFlag = 0;
      if (ip->fromHardProcessFinalState()) setbit(tmpStatusFlag, 0);
      if (ip->isPromptFinalState())        setbit(tmpStatusFlag, 1);
      if (ip->isHardProcess())  setbit(tmpStatusFlag, 2);
      
      
      // if genParticle is W or Z, check its decay type
      if ( ip->pdgId() == 23 || abs(ip->pdgId()) == 24 || abs(ip->pdgId()) == 25) {
	for (size_t k=0; k < p->numberOfDaughters(); ++k) {
	  const reco::Candidate *dp = p->daughter(k);
	  if (abs(dp->pdgId())<=6)                               setbit(tmpStatusFlag, 4);
	  else if (abs(dp->pdgId())==11 || abs(dp->pdgId())==12) setbit(tmpStatusFlag, 5);
	  else if (abs(dp->pdgId())==13 || abs(dp->pdgId())==14) setbit(tmpStatusFlag, 6);
	  else if (abs(dp->pdgId())==15 || abs(dp->pdgId())==16) setbit(tmpStatusFlag, 7);
	}
      }
      mcStatusFlag.push_back(tmpStatusFlag);
      
      int mcGMomPID_ = -999;
      float mcGMomPt_ = -999;
      float mcGMomEta_ = -999;
      float mcGMomPhi_ = -999;
      float mcGMomMass_ = -999;
      int mcMomPID_  = -999;
      float mcMomPt_    = -999.;
      float mcMomMass_  = -999.;
      float mcMomEta_   = -999.;
      float mcMomPhi_   = -999.;
      
      if (!runOnSherpa_) {
	
	reco::GenParticleRef partRef = reco::GenParticleRef(genParticlesHandle,
							    ip-genParticlesHandle->begin());
	genpartparentage::GenParticleParentage particleHistory(partRef);
     
	mcParentage.push_back(particleHistory.hasLeptonParent()*16   +
			      particleHistory.hasBosonParent()*8     +
			      particleHistory.hasNonPromptParent()*4 +
			      particleHistory.hasQCDParent()*2       +
			      particleHistory.hasExoticParent());      
	
	bool dump_(false);
	if ( particleHistory.hasRealParent() ) {
	  reco::GenParticleRef momRef = particleHistory.parent();
	  if((ip->pdgId() ==13 && momRef->pdgId() ==-15) || (ip->pdgId() ==-13 && momRef->pdgId() ==15)) { 
	 dump_ = true; 
	 //	 std::cout << "pdgid= " << ip->pdgId() << "real parent= " << momRef->pdgId() << std::endl;
	  }
	  if(dump_) dump(genParticlesHandle);
	  
	  if ( momRef.isNonnull() && momRef.isAvailable() ) {
	    mcMomPID_  = momRef->pdgId();
	    mcMomPt_   = momRef->pt();
	    mcMomMass_ = momRef->mass();
	    mcMomEta_  = momRef->eta();
	    mcMomPhi_  = momRef->phi();
	    // get Granny
	    genpartparentage::GenParticleParentage motherParticle(momRef);
	    if ( motherParticle.hasRealParent() ) {
	      reco::GenParticleRef granny = motherParticle.parent();
	      mcGMomPID_ = granny->pdgId();
	      mcGMomPt_ = granny->pt();
	      mcGMomEta_ = granny->eta();
	      mcGMomPhi_ = granny->phi();
	      mcGMomMass_ = granny->mass();
	    }
	  }
	}
	mcGMomPID.push_back(mcGMomPID_);
	mcGMomPt.push_back(mcGMomPt_);
	mcGMomEta.push_back(mcGMomEta_);
	mcGMomPhi.push_back(mcGMomPhi_);
	mcGMomMass.push_back(mcGMomMass_);
	mcMomPID.push_back(mcMomPID_);
	mcMomPt.push_back(mcMomPt_);
	mcMomMass.push_back(mcMomMass_);
	mcMomEta.push_back(mcMomEta_);
	mcMomPhi.push_back(mcMomPhi_);
      }
      
      //mcIndex.push_back(genIndex-1);
      
      
      //// keep hadron coming from tau which is coming from Z
      double mcHadronPt_ = -999.;
      double mcHadronEta_ = -999.;
      double mcHadronPhi_ = -999.;
      double mcHadronMass_ = -999.;
      double mcHadronE_ = -999.;
      int mcHadronGMomPID_ = -999.;
      float mcHadronGMomPt_ = -999.;
      float mcHadronGMomEta_ = -999.;
      float mcHadronGMomPhi_ = -999.;
      float mcHadronGMomMass_ = -999.;
      int mcHadronMomPID_ = -999.;
      double mcHadronMomPt_ = -999.;
      double mcHadronMomEta_ = -999.;
      double mcHadronMomPhi_ = -999.;
      double mcHadronMomMass_ = -999.;
      int npiplus_  =-999;
      int npiminus_ =-999;
      int npizero_ =-999;
      
      //      if(fabs(ip->pdgId()) ==15 && mcMomPID_ ==25) {	
      if(fabs(ip->pdgId()) ==15) {
	TLorentzVector hadron;
	hadron.SetPtEtaPhiE(0.,0.,0.,0.);
	bool tauhadron(false);
	npiplus_ =0;
	npiminus_ =0;
	npizero_ =0;
	for(unsigned int i=0; i<ip->numberOfDaughters(); i++){
	  const reco::Candidate& d = (*(ip->daughter(i)));
	  if(fabs(d.pdgId()) ==11 || fabs(d.pdgId()) ==12 || fabs(d.pdgId()) ==13 || fabs(d.pdgId()) ==14 || fabs(d.pdgId()) ==16) continue;
	  //	  cout << d.pdgId() << "\t" << d.status() << endl;
	  tauhadron =true;
	  Hadron(d,hadron,npiplus_,npiminus_,npizero_);
	  //	  cout << hadron.M() << endl;
	}
	
	if(tauhadron) {
	  mcHadronPt_ = hadron.Pt();
	  mcHadronEta_ = hadron.Eta();
	  mcHadronPhi_ = hadron.Phi();
	  mcHadronMass_ = hadron.M();
	  mcHadronE_ = hadron.E();
	  mcHadronGMomPID_ = mcMomPID_;
	  mcHadronGMomPt_ = mcMomPt_;
	  mcHadronGMomEta_ = mcMomEta_;
	  mcHadronGMomPhi_ = mcMomPhi_;
	  mcHadronGMomMass_ = mcMomMass_;
	  mcHadronMomPID_ = ip->pdgId();
	  mcHadronMomPt_ = p->pt();
	  mcHadronMomMass_ = p->mass();
	  mcHadronMomEta_ = p->eta();
	  mcHadronMomPhi_ = p->phi();
	}
      }
      
      mcHadronPt.push_back(mcHadronPt_);
      mcHadronEta.push_back(mcHadronEta_);
      mcHadronPhi.push_back(mcHadronPhi_);
      mcHadronMass.push_back(mcHadronMass_);
      mcHadronE.push_back(mcHadronE_);
      mcHadronGMomPID.push_back(mcHadronGMomPID_);
      mcHadronGMomPt.push_back(mcHadronGMomPt_);
      mcHadronGMomEta.push_back(mcHadronGMomEta_);
      mcHadronGMomPhi.push_back(mcHadronGMomPhi_);
      mcHadronGMomMass.push_back(mcHadronGMomMass_);
      mcHadronMomPID.push_back(mcHadronMomPID_);
      mcHadronMomPt.push_back(mcHadronMomPt_);
      mcHadronMomMass.push_back(mcHadronMomMass_);
      mcHadronMomEta.push_back(mcHadronMomEta_);
      mcHadronMomPhi.push_back(mcHadronMomPhi_);
      npiplus.push_back(npiplus_);
      npiminus.push_back(npiminus_);
      npizero.push_back(npizero_);
      
      if (photonOrLepton) {
	mcCalIsoDR03.push_back( getGenCalIso(genParticlesHandle, ip, 0.3, false, false) );
	mcTrkIsoDR03.push_back( getGenTrkIso(genParticlesHandle, ip, 0.3) );
	mcCalIsoDR04.push_back( getGenCalIso(genParticlesHandle, ip, 0.4, false, false) );
	mcTrkIsoDR04.push_back( getGenTrkIso(genParticlesHandle, ip, 0.4) );
      } else {
	mcCalIsoDR03.push_back( -999. );
	mcTrkIsoDR03.push_back( -999. );
	mcCalIsoDR04.push_back( -999. );
	mcTrkIsoDR04.push_back( -999. );
      }
      
      nMC_++;
    }
  }
}


void Hadron(const reco::Candidate& d, TLorentzVector& hadron, int& npiplus, int& npiminus, int& npizero) {
  TLorentzVector h;
  if(d.status() ==1) {
    h.SetPtEtaPhiE(d.pt(),d.eta(),d.phi(),d.energy());
    hadron +=h;
    //    cout << d.pdgId() << "\t";
    if(d.pdgId() ==111) npizero +=1;
    if(d.pdgId() ==211)npiplus +=1;
    if(d.pdgId() ==-211)npiminus +=1;
  }
  else {
    if(d.pdgId() ==111) npizero +=1;
    // cout << "pi0 ";
    for(unsigned int i=0; i<d.numberOfDaughters(); i++){
      const reco::Candidate& dd = (*(d.daughter(i)));
      //      cout << dd.pdgId() << endl;
      Hadron(dd, hadron,npiplus,npiminus,npizero);
    }
  }
  return;
}


void dump(edm::Handle<vector<reco::GenParticle> > genParticlesHandle) {
  cout << "dump the gen info" << endl;
  int indx(0);
  for (vector<reco::GenParticle>::const_iterator ip = genParticlesHandle->begin(); ip != genParticlesHandle->end(); ++ip) {
    indx +=1;
    cout << "genIndex= " << indx << "pdgid= " << ip->pdgId() << "status= " << ip->status() << "pt= " << ip->status() << "energy= " << ip->energy() << "charge= " << ip->charge() << "isDirectPromptTauDecayProductFinalState" << ip->isDirectPromptTauDecayProductFinalState() << "mother= " << ip->numberOfMothers() << "daughters= " << ip->numberOfDaughters() << endl;

    for(unsigned int i=0; i<ip->numberOfMothers(); i++) {
      const reco::Candidate& m = *(ip->mother(i));
      cout << "mompdgId= " << m.pdgId() << "charge= " << m.charge() << "status= " << m.status() << "pt= " << m.pt() << endl;
    }

    for(unsigned int i=0; i<ip->numberOfDaughters(); i++) {
      const reco::Candidate& d = *(ip->daughter(i));
      cout << "daupdgId= " << d.pdgId() << "charge= " << d.charge() << "status= " << d.status() << "pt= " << d.pt() << endl;
      if(fabs(d.pdgId()) ==15) {
	for(unsigned int j=0; j<d.numberOfDaughters(); j++) {
	  const reco::Candidate& dd = *(d.daughter(j));
	  cout << "pdgid of daughter of tau = " << dd.pdgId() << "charge= " << dd.charge() << "status= " << dd.status() << "pt= " << dd.pt() << endl;
	}
      }
    }

  }
  cout << "Exiting form dumping" << endl;
}
