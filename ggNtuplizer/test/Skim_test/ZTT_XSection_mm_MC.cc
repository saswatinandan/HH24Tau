///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Compiling the code:   ./Make.sh ZTT_XSection.cc
//   Running the code:     ./ZTT_XSection.exe OutPut.root   Input.root
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "TreeReader.h"
#include "Corrector.h"
#include "WeightCalculator.h"
#include "TLorentzVector.h"
#include "TStyle.h"
#include <string> //remove space for successful compilation
#include <ostream> //remove space for successful compilation

int  main(int argc, char** argv) {
  using namespace std;

  std::string out = *(argv + 1);
    
  cout << "\n\n\n OUTPUT NAME IS:    " << out << endl;     //PRINTING THE OUTPUT FILE NAME
  TFile *fout = TFile::Open(out.c_str(), "RECREATE");
  ofstream myfile;
  myfile.open("geninformation.txt");
  std::string input = *(argv + 2);
  std::string muon = *(argv + 3);
  bool Muon = (muon =="Muon");
  cout << "\n\n\n INPUT NAME IS:    " << input << endl;     //PRINTING THE INPUT FILE NAME
  TFile * myFile = TFile::Open(input.c_str());
  TH1F * HistoTot = (TH1F*) myFile->Get("hcount");
 
  //add the histrograms of muon and tau visible mass (both for opposite sign and same sign pair )
  TH1F *    visibleMassOS = new TH1F ("visibleMassOS","visibleMassOS", 30, 0, 300);
  TH1F *    visibleMassSS = new TH1F ("visibleMassSS","visibleMassSS", 30, 0, 300);
  TH1F*     Met           = new TH1F ("TransverseMassOS","M_{T}", 20, 0, 200);
  TH1F*     MetSS         = new TH1F ("TransverseMassSS","M_{T}", 20, 0, 200);
  TH1F*     hpt_e         = new TH1F ("pt_eOS","p_{Te}", 20, 0, 200);
  TH1F*     hpt_tau       = new TH1F ("pt_tauOS","p_{T#tau}", 20, 0, 200);
  TH1F*     heta_e        = new TH1F ("eta_eOS","#eta_{e}", 50, -2.5, 2.5);
  TH1F*     heta_tau      = new TH1F ("eta_tauOS","#eta_{#tau}", 50, -2.5, 2.5);
  TH1F*     hptSS_e       = new TH1F ("pt_eSS","p_{Te}", 20, 0, 200);
  TH1F*     hptSS_tau     = new TH1F ("pt_tauSS","p_{T#tau}", 20, 0, 200);
  TH1F*     hetaSS_e      = new TH1F ("eta_eSS","#eta_{e}", 50, -2.5, 2.5);
  TH1F*     hetaSS_tau    = new TH1F ("eta_tauSS","#eta_{#tau}", 50, -2.5, 2.5);
  TH1F*     h_bjet      = new TH1F ("bjet_OS","# of bjet", 5, 0., 5.);
  TH1F*     hSS_bjet    = new TH1F ("bjet_SS","# of bjet", 5, 0., 5.);
  TH1F* h_counter   = new TH1F("counter","counter",33,0.,33);
  TH1F* h_Mccounter   = new TH1F("Mccounter","Mccounter",3,0.,3.);
  TH1F* h_Zcount   = new TH1F("Zcount","Zdecay",7,0.,7);
  TH1F* h_taucount   = new TH1F("taucount","taudecay",16,0.,16);
  TH1F* h_neutrino   = new TH1F("neutrinocount","neutrino",4,0.,4.);
  TH1F* h_mupt   = new TH1F("muPt","Gen muPt",50,0.,50.);
  TH1F* h_mu1pt   = new TH1F("mu1Pt","",50,0.,100.);
  h_mu1pt->GetXaxis()->SetTitle("Leading #mu p_{t}");
  TH1F* h_mu2pt   = new TH1F("mu2Pt","",50,0.,100.);
  h_mu2pt->GetXaxis()->SetTitle("Subleading #mu p_{t}");
  TH1F* h_tau1pt   = new TH1F("tau1Pt","",75,0.,150.);
  h_tau1pt->GetXaxis()->SetTitle("Leading #tau p_{t}");
  TH1F* h_tau2pt   = new TH1F("tau2Pt","",75,0.,150.);
  h_tau2pt->GetXaxis()->SetTitle("Subleading #tau p_{t}");
  TH1F* h_mueta   = new TH1F("muEta","",30,-2.4,2.4);
  TH1F* h_mu1eta   = new TH1F("mu1Eta","",30,-2.4,2.4);
  h_mu1eta->GetXaxis()->SetTitle("Leading #mu #eta");
  TH1F* h_mu2eta   = new TH1F("mu2Eta","",30,-2.4,2.4);
  h_mu2eta->GetXaxis()->SetTitle("Subleading #mu #eta");
  TH1F* h_tau1eta   = new TH1F("tau1Eta","",30,-2.4,2.4);
  h_tau1eta->GetXaxis()->SetTitle("Leading #tau #eta");
  TH1F* h_tau2eta   = new TH1F("tau2Eta","",30,-2.4,2.4);
  h_tau2eta->GetXaxis()->SetTitle("Subleading #tau #eta");
  TH1F* h_taupt   = new TH1F("tauPt","Gen tauPt",100,0.,100.);
  TH1F* h_taueta   = new TH1F("tauEta","Gen tauEta",60,-2.4,2.4);
  TH1F* h_reco1mupt   = new TH1F("reco1muPt","",25,0.,50.);
  TH1F* h_reco1mueta   = new TH1F("reco1muEta","",30,-2.4,2.4);
  TH1F* h_reco2mupt   = new TH1F("reco2muPt","",25,0.,50.);
  TH1F* h_reco2mueta   = new TH1F("reco2muEta","",30,-2.4,2.4);
  TH1F* h_reco1taupt   = new TH1F("reco1tauPt","",50,0.,100.);
  TH1F* h_reco1taueta   = new TH1F("reco1tauEta","",30,-2.4,2.4);
  TH1F* h_reco2taupt   = new TH1F("reco2tauPt","",50,0.,100.);
  TH1F* h_reco2taueta   = new TH1F("reco2tauEta","",30,-2.4,2.4);
  TH1F* h_charge1   = new TH1F("tau1charge","taucharge",4,-2,2);
  TH1F* h_charge2   = new TH1F("tau2charge","taucharge",4,-2,2);
  TH1F* h_musize   = new TH1F("musize","musize",5,0,5.);
  TH1F* h_elesize   = new TH1F("elesize","elesize",5,0.,5.);
  TH1F* h_genmu   = new TH1F("genmu","genmu",62,-32.,30.);
  TH1F* h_genmumom   = new TH1F("genmumom","genmumom",62,-32.,30);
  TH1F* h_genmuGmom   = new TH1F("genmuGmom","genmuGmom",62,-32,30);
  TH1F* h_genele   = new TH1F("genele","genele",62,-32.,30.);
  TH1F* h_genelemom   = new TH1F("genelemom","genelemom",62,-32,30);
  TH1F* h_geneleGmom   = new TH1F("geneleGmom","geneleGmom",62,-32,30);
  TH1F* h_gentau   = new TH1F("gentau","gentau",62,-32.,30.);
  TH1F* h_gentaumom   = new TH1F("gentaumom","gentaumom",62,-32,30);
  TH1F* h_nele  = new TH1F("nele","nele",5,0,5);
  TH1F* h_mcmomele = new TH1F("mcmomele","mc mom of ele",62,-32,30);
  TH1F* h_mcGmomele = new TH1F("mcGmomele","mc Gmom of ele",62,-32,30);
  TH1F* h_mcmommu = new TH1F("mcmommu","mc mom of mu",62,-32,30);
  TH1F* h_mcGmommu = new TH1F("mcGmommu","mc Gmom of mu",62,-32,30);
  TH1F* h_mcmomtau = new TH1F("mcmomtau","mc mom of tau",62,-32,30);
  TH1F* h_mcGmomtau = new TH1F("mcGmomtau","mc Gmom of tau",62,-32,30);
  TH1F* h_ntaupt = new TH1F("ntauPt","reco tau pt",100,0.,100.);
  TH1F* h_ntaueta = new TH1F("ntauEta","reco tau eta",60,-3.,3.);
  TH1F* h_ntauele = new TH1F("ntauele","reco tau ele rejectn",2,0.,2.);
  TH1F* h_ntaumu = new TH1F("ntaumu","reco tau mu rejectn",2,0.,2.);
  TH1F* h_ntauiso = new TH1F("ntauiso","reco tau iso",2,0.,2.);
  TH1F* h_ntaudecay = new TH1F("ntaudecay","reco tau decay",2,0.,2.);
  TH1F* h_ntau = new TH1F("ntau","ntau",5,0.,5.);
  TH1F* h_ntaudr = new TH1F("ntaudr","ntaudr",50,0.,5.);
  TH1F* h_taugenmu = new TH1F("ntaugenmu","ngenmu",5,0.,5.);
  TH1F* h_zpt = new TH1F("zpt","zpt",200,0.,200.);
  TH1F* h_massNor = new TH1F("mass","nor",9,100,1000);
  h_massNor->GetXaxis()->SetTitle("M_{#mu#mu#tau#tau}");
  gStyle->SetOptStat(11111);
  h_massNor->SetTitle("");
  TH1F* h_massUp = new TH1F("mass_sysUp","Up",6,300,900);
  TH1F* h_massDn = new TH1F("mass_sysDn","Dn",6,300,900);
  TH1F* hmm = new TH1F("deltamm","",40,0.,5.);
  TH1F* htt = new TH1F("deltatt","",40,0.,5.);
  TH1F* hmt = new TH1F("deltamt","",40,0.,5.);
  TH1F* htaucountwthchrg   = new TH1F("taucount_withchrg","taudecay",15,0.,15);
  
  std::string title[6]= {"mu","ele","tau","muele","mutau","eletau"};
  std::string tautitle[15]= {"2mu2tauh","2mu1ele1tauh","1mu1ele2tauh","1mu2ele2tauh","3mu1tauh","1mu3tauh","2mu2ele","1mu3ele","3mu1ele","2ele2tauh","3ele1tauh","1ele3tauh","4mu","4ele","4tauh"};
  std::string chargetitle[14] = {"totEvnt","2mu2tauhSS","2mu2tauhOS","2e2tauhSS","2e2tauhOS","mue2tauhSS","mue2tauhOS","mu3tauh","e3tauh","4tauh","4mu","4ele","3mu1tauh","3ele1tauh"};
  std::string neutrinotitle[3]= {"mu","ele","tau"};

  for(int i=0; i<14;i++) htaucountwthchrg->GetXaxis()->SetBinLabel(i+1,chargetitle[i].c_str());
  for(int i=0; i<6; i++) {
    h_Zcount->GetXaxis()->SetBinLabel(i+1,title[i].c_str());
  }

  for(int i=0; i<15; i++) {
    h_taucount->GetXaxis()->SetBinLabel(i+1,tautitle[i].c_str());
  }
  for(int i=0; i<3; i++) {
    h_neutrino->GetXaxis()->SetBinLabel(i+1,neutrinotitle[i].c_str());
  }


  visibleMassOS->SetDefaultSumw2();
  visibleMassSS->SetDefaultSumw2();
    
  TTree *Run_Tree = (TTree*) myFile->Get("ggNtuplizer/EventTree");
  // TTree *Run_Tree = (TTree*) myFile->Get("EventTree");
  cout.setf(ios::fixed, ios::floatfield);
  /////////////////////////   General Info
  Run_Tree->SetBranchAddress("isData", &isData);
  Run_Tree->SetBranchAddress("run", &run);
  Run_Tree->SetBranchAddress("lumis", &lumis);
  Run_Tree->SetBranchAddress("event", &event);
  //  Run_Tree->SetBranchAddress("genWeight",&genWeight);
  Run_Tree->SetBranchAddress("HLTEleMuX", &HLTEleMuX);
  //  Run_Tree->SetBranchAddress("puTrue", &puTrue);        
        

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
  Run_Tree->SetBranchAddress("jetCSV2BJetTags",&jetCSV2BJetTags);
  Run_Tree->SetBranchAddress("mcMomPID" ,&mcMomPID);
  Run_Tree->SetBranchAddress("mcGMomPID" ,&mcGMomPID);
  Run_Tree->SetBranchAddress("mcGMomPt", &mcGMomPt);
  Run_Tree->SetBranchAddress("mcGMomEta", &mcGMomEta);
  Run_Tree->SetBranchAddress("mcGMomPhi", &mcGMomPhi);
  Run_Tree->SetBranchAddress("mcGMomMass", &mcGMomMass);
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
  Run_Tree->SetBranchAddress("mcHadronGMomPt", &mcHadronGMomPt);
  Run_Tree->SetBranchAddress("mcHadronGMomEta", &mcHadronGMomEta);
  Run_Tree->SetBranchAddress("mcHadronGMomPhi", &mcHadronGMomPhi);
  Run_Tree->SetBranchAddress("mcHadronGMomMass", &mcHadronGMomMass);
  Run_Tree->SetBranchAddress("mcHadronMomPt", &mcHadronMomPt);
  Run_Tree->SetBranchAddress("mcHadronMomMass", &mcHadronMomMass);
  Run_Tree->SetBranchAddress("mcHadronMomEta", &mcHadronMomEta);
  Run_Tree->SetBranchAddress("mcHadronMomPhi", &mcHadronMomPhi);
  Run_Tree->SetBranchAddress("npiplus", &npiplus);
  Run_Tree->SetBranchAddress("npiminus", &npiminus);
  Run_Tree->SetBranchAddress("npizero", &npizero);
  Run_Tree->SetBranchAddress("mcStatusFlag", &mcStatusFlag);
  Run_Tree->SetBranchAddress("mcMass", &mcMass);
        
  /////////////////////////   MET Info
  Run_Tree->SetBranchAddress("pfMET",&pfMET);
  Run_Tree->SetBranchAddress("pfMETPhi",&pfMETPhi);
  Run_Tree->SetBranchAddress("genMET",&genMET);
  Run_Tree->SetBranchAddress("genMETPhi",&genMETPhi);
  Run_Tree->SetBranchAddress("signif_dxx",&signif_dxx);
  Run_Tree->SetBranchAddress("signif_dyy",&signif_dyy);
  Run_Tree->SetBranchAddress("signif_dxy",&signif_dxy);
  Run_Tree->SetBranchAddress("signif_dyx",&signif_dyx);

  Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();

  cout<<"nentries_wtn====" << nentries_wtn << "\n";
  float LumiWeight = 1;
  if (HistoTot) LumiWeight = weightCalc(HistoTot, input);
  cout << "LumiWeight is " << LumiWeight << "\n";

  std::string key[33] = {"Event","Trigger","bjetveto","genpart_matched","partPt","partEta","Iso","ID","genpart_matched","dR","partPt","partEta","Iso","ID","genTau_matched","dR","tauPt","tauEta","taueleRejection","taumuonRejection","tauDecayMode","tauIso","genTau_matched","dR","tauPt","tauEta","taueleRejection","taumuonRejection","tauDecayMode","tauIso","taucharge","partcharge","taupartcharge"};

  //std::string key[31] = {"Event","genMuon_matched","mPt","mEta","Isom","isMediumMuon","genMuon_matched","dR","mPt","mEta","Isom","isMediumMuon","genTau_matched","dR","tauPt","tauEta","taueleRejection","taumuonRejection","tauDecayMode","tauIso","genTau_matched","dR","tauPt","tauEta","taueleRejection","taumuonRejection","tauDecayMode","tauIso","taucharge","muoncharge","taumuoncharge"};
  //std::string key[27] = {"Event","mPt","mEta","Isom","isMediumMuon","dR","mPt","mEta","Isom","isMediumMuon","dR","tauPt","tauEta","taueleRejection","taumuonRejection","tauIso","tauDecayMode","dR","tauPt","tauEta","taueleRejection","taumuonRejection","tauIso","tauDecayMode","taucharge","muoncharge","taumuoncharge"}; 
  for (int i=0; i<33; i++) h_counter->GetXaxis()->SetBinLabel(i+1,key[i].c_str());
  int evteleno(0),evtmuno(0),evttauno(0),evtno(0),mom(0),evtzno(0),taufound(0);
  TLorentzVector H, firstHiggs,genPart;

  for ( Int_t i = 0; i < nentries_wtn; i++) {

    //  for ( Int_t i = 0; i < 1000000; i++) { 

    std::ostringstream s;
    int nmu(0),ntau(0),nele(0),ntaumu(0),ntauele(0),ntauh(0),hadron(0),z(0);
    bool found(false);
    Run_Tree->GetEntry(i);
    
    if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
    
    fflush(stdout);
    float PUWeight = 1;
    bool PassTrigger = (Muon) ?  ((HLTEleMuX >> 14 & 1) || (HLTEleMuX >> 15 & 1)) : (HLTEleMuX >> 5 & 1);

    int count_bjet(0);
    for (int ijet= 0 ; ijet < nJet ; ijet++){
      if (jetPt->at(ijet) > 20 && fabs(jetEta->at(ijet)) < 2.5 && jetCSV2BJetTags->at(ijet) > 0.800){
	count_bjet +=1;
	break;
      }
    }

    ///////////////////////////////////////////////
    //Important Analysis Loop Will Happen Here!!!//
    ///////////////////////////////////////////////
    
    int donotTake(0), mucharge(0),tauhad(0),tauele(0),taumu(0),elecharge(0);
    std::vector<TLorentzVector> genmu, gentau,gentauhadron,tau,neu;
    bool findMC(false), findH(false),trueMu(false), truetau(false);

    for(int imc=0; imc<nMC; imc++) {
      //            if(mcPID->at(imc) ==23 && mcMass->at(imc) >80 && mcMass->at(imc) <100) {
      if(mcPID->at(imc) ==25) {
	z+=1;
	h_zpt->Fill(mcPt->at(imc));
	cout << mcMass->at(imc) << endl;
      }
      /////gen mu/ele/tau coming from Z/////
    if(fabs(mcPID->at(imc)) ==11) {
	if(fabs(mcMomPID->at(imc)) ==25)  nele +=1;
      }
      if(fabs(mcPID->at(imc)) ==13) {
        if(fabs(mcMomPID->at(imc)) ==25)  nmu +=1;
      }
      if(fabs(mcPID->at(imc)) ==16 && fabs(mcMomPID->at(imc)) ==15 && mcGMomPID->at(imc) ==25) ntau +=1;
    }

    /////tau decay products ////
    if(ntau ==4) {
      h_Mccounter->Fill(1);
      for(int imc=0; imc<nMC; imc++) {
	if(fabs(mcPID->at(imc)) ==14 && fabs(mcMomPID->at(imc)) ==15 && mcGMomPID->at(imc) ==25) {
	  ntaumu +=1;
	  int charge = (mcMomPID->at(imc)==15) ? -1 : 1;
	  mucharge +=charge;
	}

	if(fabs(mcPID->at(imc)) ==12 && fabs(mcMomPID->at(imc)) ==15 && mcGMomPID->at(imc) ==25) {
	  ntauele +=1;
	  elecharge += (mcMomPID->at(imc) ==15) ? -1 : 1;
	}
	if(fabs(mcPID->at(imc)) ==16 && fabs(mcMomPID->at(imc)) ==15 && mcGMomPID->at(imc) ==25) ntauh +=1;

      }
    }
    
    if(nmu ==4) h_Zcount->Fill(0);
    if(nele ==4) h_Zcount->Fill(1);
    if(ntau ==4) h_Zcount->Fill(2);
    if(nmu ==2 && nele==2) h_Zcount->Fill(3);
    if(nmu ==2 && ntau==2) h_Zcount->Fill(4);
    if(nele ==2 && ntau==2) h_Zcount->Fill(5);
    htaucountwthchrg->Fill(0);
    if(ntau==4) {
      
      if(ntaumu==2 && ntauele==0 && ntauh==4) {
	h_taucount->Fill(0);
	if(fabs(mucharge) ==2) htaucountwthchrg->Fill(1);
	else htaucountwthchrg->Fill(2);
      }
      if(ntaumu==2 && ntauele==1&& ntauh==4) h_taucount->Fill(1);
      if(ntaumu ==1 && ntauele==1&&ntauh==4) {
	h_taucount->Fill(2);
	if(fabs(mucharge+elecharge)==2) htaucountwthchrg->Fill(5);
        else htaucountwthchrg->Fill(6);
      }

      if(ntaumu ==1 && ntauele==2&&ntauh==4) h_taucount->Fill(3);
      if(ntaumu ==3 && ntauele==0 && ntauh==4) h_taucount->Fill(4);
      if(ntaumu==1 && ntauele==0 && ntauh==4) {
	h_taucount->Fill(5);
	htaucountwthchrg->Fill(7);
      }
      if(ntaumu==2 && ntauele==2&& ntauh==4) h_taucount->Fill(6);
      if(ntaumu ==1 && ntauele==3&&ntauh==4) h_taucount->Fill(7);
      if(ntaumu ==3 && ntauele==1&&ntauh==4) h_taucount->Fill(8);
      if(ntauele==2 && ntaumu==0&&ntauh==4) {
	h_taucount->Fill(9);
	if(fabs(elecharge) ==2) htaucountwthchrg->Fill(3);
        else htaucountwthchrg->Fill(4);
      }

      if(ntauele==3&&ntaumu==0&&ntauh==4) h_taucount->Fill(10);
      if(ntauele==1&& ntaumu==0&&ntauh==4) h_taucount->Fill(11);
      if(ntaumu ==4&&ntauele==0&&ntauh==4) h_taucount->Fill(12);
      if(ntauele==4&&ntaumu==0&&ntauh==4) h_taucount->Fill(13);
      if(ntauh==4&&ntaumu==0&&ntauele==0) h_taucount->Fill(14);
      if(ntaumu==0 && ntauele==1 && ntauh==4) htaucountwthchrg->Fill(8);
      if(ntaumu==0 && ntauele==0 && ntauh==4) htaucountwthchrg->Fill(9);
      if(ntaumu==4 && ntauele==0 && ntauh==4) htaucountwthchrg->Fill(10);
      if(ntaumu==0 && ntauele==4 && ntauh==4) htaucountwthchrg->Fill(11);
      if(ntaumu==3 && ntauele==0 && ntauh==4) htaucountwthchrg->Fill(12);
      if(ntaumu==0 && ntauele==3 && ntauh==4) htaucountwthchrg->Fill(13);
      if(ntaumu) h_neutrino->Fill(0);
      if(ntauele) h_neutrino->Fill(1);
      if(ntauh) h_neutrino->Fill(2);
    }


    if( z==2 && ntaumu==2 && ntauele==0 && ntauh==4) h_Mccounter->Fill(2);
    if( z==2 && ntaumu==2 && ntauele==0 && ntauh==4 && mucharge) findMC =true;
    //if( z==2) findMC =true;
    if(!findMC) continue;

    bool firstmom(false);
    s << "****New Event************" << "\n";
    for(int imc=0; imc<nMC; imc++) {
      if(fabs(mcPID->at(imc)) !=13 && mcHadronPt->at(imc) <0 && mcHadronPt->at(imc) !=0) continue;
      if(fabs(mcPID->at(imc)) ==13 && fabs(mcMomPID->at(imc)) ==15 && fabs(mcGMomPID->at(imc)) ==25) {
	genPart.SetPtEtaPhiE(mcPt->at(imc),mcEta->at(imc),mcPhi->at(imc),mcE->at(imc));
	genmu.push_back(genPart);
	H.SetPtEtaPhiM(mcGMomPt->at(imc),mcGMomEta->at(imc),mcGMomPhi->at(imc),mcGMomMass->at(imc));
	s << "pt = " << genPart.Pt() << "\t" << "eta = " << genPart.Eta() << "\t" << "phi = " << genPart.Phi() << "\t" << "pdgId = " << mcPID->at(imc) << "\t" << "mom= " << mcMomPID->at(imc);
      }
      
      if((mcHadronPt->at(imc) >0 || mcHadronPt->at(imc) ==0) && fabs(mcHadronMomPID->at(imc)) ==15 && mcHadronGMomPID->at(imc)==25) {
        genPart.SetPtEtaPhiE(mcHadronPt->at(imc),mcHadronEta->at(imc),mcHadronPhi->at(imc),mcHadronE->at(imc));
	gentauhadron.push_back(genPart);
	H.SetPtEtaPhiM(mcHadronGMomPt->at(imc),mcHadronGMomEta->at(imc),mcHadronGMomPhi->at(imc),mcHadronGMomMass->at(imc));
	int charge = (mcHadronMomPID->at(imc) == 15) ? -1 : 1;
	s << "pt = " << genPart.Pt() << "\t" << "eta = " << genPart.Eta() << "\t" << "phi = " << genPart.Phi() << "\t" << "charge = " << charge << "\t" << "mom= " << mcHadronMomPID->at(imc) << 
	  "\t" << "npiplus = " << npiplus->at(imc) << "\t" << "npimus = " << npiminus->at(imc) << "\t" << "npizero = " << npizero->at(imc) <<"\t" << "mass= " << genPart.M();
      }
      if(!firstmom) {
	  firstmom = true;
	  firstHiggs = H;
	  s << "\t" << "firstHiggs" << endl;
	}
      else if(H==firstHiggs) {
	s << "\t" << "first Higgs" << endl;
      }
      else {
	s << "\t" << "second Higgs" << endl;
      }
    }

    hmm->Fill(genmu[0].DeltaR(genmu[1]));
    htt->Fill(gentauhadron[0].DeltaR(gentauhadron[1]));
    hmt->Fill(genmu[0].DeltaR(gentauhadron[0]));
    hmt->Fill(genmu[0].DeltaR(gentauhadron[1]));
    hmt->Fill(genmu[1].DeltaR(gentauhadron[0]));
    hmt->Fill(genmu[1].DeltaR(gentauhadron[0]));

     /////gen level distribution ///
    h_mupt->Fill(genmu[0].Pt());
    h_mupt->Fill(genmu[1].Pt());
    h_mueta->Fill(genmu[0].Eta());
    h_mueta->Fill(genmu[1].Eta());
    
    ////pt ordering for gen mu ////
    if(genmu[0].Pt() > genmu[1].Pt()) {
      h_mu1pt->Fill(genmu[0].Pt());
      h_mu2pt->Fill(genmu[1].Pt());
      h_mu1eta->Fill(genmu[0].Eta());
      h_mu2eta->Fill(genmu[1].Eta());
    }
    else {
      h_mu1pt->Fill(genmu[1].Pt());
      h_mu2pt->Fill(genmu[0].Pt());
      h_mu1eta->Fill(genmu[1].Eta());
      h_mu2eta->Fill(genmu[0].Eta());
    }

    ////pt ordering for gen tau //// 
    if(gentauhadron[0].Pt() > gentauhadron[1].Pt()) {
      h_tau1pt->Fill(gentauhadron[0].Pt());
      h_tau2pt->Fill(gentauhadron[1].Pt());
      h_tau1eta->Fill(gentauhadron[0].Eta());
      h_tau2eta->Fill(gentauhadron[1].Eta());
    }
    else {
      h_tau1pt->Fill(gentauhadron[1].Pt());
      h_tau2pt->Fill(gentauhadron[0].Pt());
      h_tau1eta->Fill(gentauhadron[1].Eta());
      h_tau2eta->Fill(gentauhadron[0].Eta());
    }

    
    h_Mccounter->Fill(1);
    h_musize->Fill(nMu);
    h_elesize->Fill(nEle);
    
    h_counter->Fill(0);
    if(!PassTrigger) continue;
    h_counter->Fill(1);
    if(count_bjet) continue;
    h_counter->Fill(2);

    if(nMu >=2) {
      h_reco1mupt->Fill(muPt->at(0));
      h_reco1mueta->Fill(muEta->at(0));
      h_reco2mupt->Fill(muPt->at(1));
      h_reco2mueta->Fill(muEta->at(1));
    }
    if(nTau >=2) {
      h_reco1taupt->Fill(tauPt->at(0));
      h_reco1taueta->Fill(tauEta->at(0));
      h_reco2taupt->Fill(tauPt->at(1));
      h_reco2taueta->Fill(tauEta->at(1));
    }

    double tauEUp = 1.03;
    double tauEDn = 0.95;

    int iend = (Muon) ? nMu : nEle;
    for  (int imu=0 ; imu < iend; imu++){
      
      TLorentzVector reco1mu;
      if(Muon) {
	reco1mu.SetPtEtaPhiE(muPt->at(imu),muEta->at(imu),muPhi->at(imu),muEn->at(imu));
      }
      else {
	reco1mu.SetPtEtaPhiE(elePt->at(imu),eleEta->at(imu),elePhi->at(imu),eleEn->at(imu));
      }

      for(int imc=0; imc<nMC; imc++) {
	if(fabs(mcPID->at(imc)) ==13 && fabs(mcMomPID->at(imc)) ==15 && fabs(mcGMomPID->at(imc)) ==25) {
	  TLorentzVector genmu;
	  genmu.SetPtEtaPhiE(mcPt->at(imc),mcEta->at(imc),mcPhi->at(imc),mcE->at(imc));
	  if(genmu.DeltaR(reco1mu) <0.5) { trueMu = true; break;}
	}
      }
      if(!trueMu) continue;
      h_counter->Fill(3);
      if((Muon && muPt->at(imu) <= 18) || (!Muon && elePt->at(imu) <24)) continue;
      h_counter->Fill(4);
      if((Muon && fabs(muEta->at(imu)) >= 2.4) || (!Muon && fabs(eleEta->at(imu)) >= 2.5)) continue;
      h_counter->Fill(5);
      //      if(fabs(muD0->at(imu)) > 0.045 || fabs(muDz->at(imu)) > 0.2) continue;
      //h_counter->Fill(3);
      //////IsoCut/////////
      if(Muon) {
      float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
      if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))  > 0.0)
	IsoMu= (muPFChIso->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
      if(IsoMu >=0.3) continue;
      h_counter->Fill(6);
      }

      else {
	float IsoMu=elePFChIso->at(imu)/elePt->at(imu);
	if ( (elePFNeuIso->at(imu) + elePFPhoIso->at(imu) - 0.5* elePFPUIso->at(imu))  > 0.0)
	  IsoMu= (elePFChIso->at(imu) + elePFNeuIso->at(imu) + elePFPhoIso->at(imu) - 0.5* elePFPUIso->at(imu))/elePt->at(imu);
	if(IsoMu >=0.25) continue;
	h_counter->Fill(6);

      }
      //////eleId/////
      if(Muon) {
	UShort_t id = (muIDbit->at(imu) >> 1 & 1);
	if(!id) continue;
	h_counter->Fill(7);
      }
      else {
	bool eleMVAId= false;
	if (fabs (eleSCEta->at(imu)) <= 0.8 && eleIDMVA->at(imu) > 0.837) eleMVAId= true;
	else if (fabs (eleSCEta->at(imu)) >  0.8 &&fabs (eleSCEta->at(imu)) <=  1.5 && eleIDMVA->at(imu) > 0.715) eleMVAId= true;
	else if ( fabs (eleSCEta->at(imu)) >=  1.5 && eleIDMVA->at(imu) > 0.357 ) eleMVAId= true;
	else eleMVAId= false;
	if(!eleMVAId) continue;
	h_counter->Fill(7);
      }
      
      trueMu = false;
      for  (int jmu=imu+1 ; jmu < iend; jmu++){
	
	
	TLorentzVector reco2mu;
	if(Muon) {
	  reco2mu.SetPtEtaPhiE(muPt->at(jmu),muEta->at(jmu),muPhi->at(jmu),muEn->at(jmu));
	}
	else  {
	  reco2mu.SetPtEtaPhiE(elePt->at(jmu),eleEta->at(jmu),elePhi->at(jmu),eleEn->at(jmu));
	}

	for(int imc=0; imc<nMC; imc++) {
	  if(fabs(mcPID->at(imc)) ==13 && fabs(mcMomPID->at(imc)) ==15 && fabs(mcGMomPID->at(imc)) ==25) {
	    TLorentzVector genmu;
	    genmu.SetPtEtaPhiE(mcPt->at(imc),mcEta->at(imc),mcPhi->at(imc),mcE->at(imc));
	    if(genmu.DeltaR(reco2mu) <0.5) { trueMu = true; break;}
	  }
	}
	if(!trueMu) continue;
	h_counter->Fill(8);
	if(reco1mu.DeltaR(reco2mu) <0.3) continue;
	h_counter->Fill(9);
	if((Muon && muPt->at(jmu) <= 9) || (!Muon && elePt->at(jmu) <= 13)) continue;
	h_counter->Fill(10);
	if((Muon && fabs(muEta->at(jmu)) >=2.4) || (!Muon && fabs(eleEta->at(jmu))) >= 2.5) continue;
	h_counter->Fill(11);
	//	if(fabs(muD0->at(jmu)) > 0.045 || fabs(muDz->at(jmu)) > 0.2) continue;
	//	h_counter->Fill(9);
	
	//////IsoCut/////////      
	if(Muon) {
	  float IsoMu=muPFChIso->at(jmu)/muPt->at(jmu);
	  if ( (muPFNeuIso->at(jmu) + muPFPhoIso->at(jmu) - 0.5* muPFPUIso->at(jmu))  > 0.0)
	    IsoMu= (muPFChIso->at(jmu) + muPFNeuIso->at(jmu) + muPFPhoIso->at(jmu) - 0.5* muPFPUIso->at(jmu))/muPt->at(jmu);
	  if(IsoMu >= 0.3) continue;
	  h_counter->Fill(12);
	}

	else {

	  float IsoMu=elePFChIso->at(jmu)/elePt->at(jmu);
          if ( (elePFNeuIso->at(jmu) + elePFPhoIso->at(jmu) - 0.5* elePFPUIso->at(jmu))  > 0.0)
            IsoMu= (elePFChIso->at(jmu) + elePFNeuIso->at(jmu) + elePFPhoIso->at(jmu) - 0.5* elePFPUIso->at(jmu))/elePt->at(jmu);
          if(IsoMu >= 0.25) continue;
          h_counter->Fill(12);
        }

	//////eleId/////  
	if(Muon) {
	UShort_t id = (muIDbit->at(jmu) >> 1 & 1);
	if(!id) continue;
	h_counter->Fill(13);
	}
	else {

	  bool eleMVAId= false;
          if (fabs (eleSCEta->at(jmu)) <= 0.8 && eleIDMVA->at(jmu) > 0.837) eleMVAId= true;
          else if (fabs (eleSCEta->at(jmu)) >  0.8 &&fabs (eleSCEta->at(jmu)) <=  1.5 && eleIDMVA->at(jmu) > 0.715) eleMVAId= true;
          else if ( fabs (eleSCEta->at(jmu)) >=  1.5 && eleIDMVA->at(jmu) > 0.357 ) eleMVAId= true;
          else eleMVAId= false;
	  if(!eleMVAId) continue;
	  h_counter->Fill(13);

	}
	for  (int itau=0 ; itau < nTau; itau++){

	  TLorentzVector reco1tau;
	  reco1tau.SetPtEtaPhiE(tauPt->at(itau),tauEta->at(itau),tauPhi->at(itau),tauEnergy->at(itau));
	  for(int i=0; i<2; i++) {
	    if(gentauhadron[i].DeltaR(reco1tau) <0.5) { truetau = true; break;}
	  }
	  if(!truetau) continue;
	  h_counter->Fill(14);
	  h_charge1->Fill(tauCharge->at(itau));
	  //if(tauDxy->at(itau) > 0.05) continue;
	  if(reco1tau.DeltaR(reco1mu) <0.3 || reco1tau.DeltaR(reco2mu) <0.3) continue;
	  h_counter->Fill(15);
	  if(tauPt->at(itau) <= 20) continue;
	  h_counter->Fill(16);
	  if(fabs(tauEta->at(itau)) >= 2.3) continue;
	  h_counter->Fill(17);
	  if((Muon && tauByMVA6LooseElectronRejection->at(itau) ==0) || (!Muon && tauByMVA6LooseElectronRejection->at(itau) ==0)) continue;
	  h_counter->Fill(18);
	  if((Muon && tauByLooseMuonRejection3->at(itau) ==0) || (!Muon && tauByLooseMuonRejection3->at(itau) ==0)) continue;
	  h_counter->Fill(19);
	  //	   if(tauByLooseCombinedIsolationDeltaBetaCorr3Hits->at(itau) ==0) continue;
	  if(taupfTausDiscriminationByDecayModeFinding->at(itau) ==0) continue;
	  h_counter->Fill(20);
	  if(tauByLooseIsolationMVArun2v1DBoldDMwLT->at(itau) ==0 ) continue;
	  h_counter->Fill(21);
	  
	  truetau = false;
	    for  (int jtau=itau+1 ; jtau < nTau; jtau++){
	    
	    TLorentzVector reco2tau;
	    reco2tau.SetPtEtaPhiE(tauPt->at(jtau),tauEta->at(jtau),tauPhi->at(jtau),tauEnergy->at(jtau));
	    for(int i=0; i<2; i++) {
	      if(gentauhadron[i].DeltaR(reco2tau) <0.5) { truetau = true; break;}
	    }
	    if(!truetau) continue;
	    h_counter->Fill(22);
	    h_charge2->Fill(tauCharge->at(jtau));
	    //if(tauDxy->at(jtau) > 0.05) continue;
	    //h_counter->Fill(21);
	    if(reco1tau.DeltaR(reco2tau) <0.3 || reco2tau.DeltaR(reco2mu) <0.3 || reco2tau.DeltaR(reco1mu) <0.3) continue;
	    h_counter->Fill(23);
	    if(tauPt->at(jtau) <= 20) continue;
	    h_counter->Fill(24);
	    if(fabs(tauEta->at(jtau)) >= 2.3) continue;
	    h_counter->Fill(25);
	    if((Muon && tauByMVA6LooseElectronRejection->at(jtau) ==0) || (!Muon && tauByMVA6LooseElectronRejection->at(jtau) ==0)) continue;
	    h_counter->Fill(26);
	    if((Muon && tauByLooseMuonRejection3->at(jtau) ==0) || (!Muon && tauByLooseMuonRejection3->at(jtau) ==0)) continue;
	    h_counter->Fill(27);
	    //if(tauByLooseCombinedIsolationDeltaBetaCorr3Hits->at(jtau) ==0) continue;
	    if(taupfTausDiscriminationByDecayModeFinding->at(jtau) ==0) continue;
	    h_counter->Fill(28);
	    if(tauByLooseIsolationMVArun2v1DBoldDMwLT->at(jtau) ==0 ) continue;
	    h_counter->Fill(29);
	    if(tauCharge->at(itau)*tauCharge->at(jtau) <0) continue;
	    h_counter->Fill(30);
	    if((Muon && muCharge->at(imu)*muCharge->at(jmu) <0) || (!Muon && eleCharge->at(imu)*eleCharge->at(jmu) <0)) continue;
	    h_counter->Fill(31);
	    if((Muon && muCharge->at(imu)*tauCharge->at(jtau) >0) || (!Muon && eleCharge->at(imu)*tauCharge->at(jtau) >0)) continue;
	     h_counter->Fill(32);
	     found = true;
	     s << "signif_dxx= " << signif_dxx << "\t" << "signif_dxy= " << "\t" << signif_dxy << "\t" << "signif_dyy= " << signif_dyy << "\t" << "signif_dyx= " << signif_dyx << "\n";
	     s << "pfMET_x = " << genMET*TMath::Cos(genMETPhi) << "\t" << "pfMET_y = " << genMET*TMath::Sin(genMETPhi) << endl;
	     string str = s.str();
	     myfile << str << endl;
	     TLorentzVector hh;
	     hh = reco1mu+reco2mu+reco1tau+reco2tau;
	     h_massNor->Fill(hh.M());
	     break;
	  }
	  if(found) break;
	}
	if(found) break;
      }
      if(found) break;
      }
  }

  //end of analysis code, close and write histograms/file
    fout->cd();
    h_counter->Write();
    htaucountwthchrg->Write();
  h_Mccounter->Write();
  h_Zcount->Write();
  h_taucount->Write();
  h_neutrino->Write();
  h_mupt->Write();
  h_mueta->Write();
  h_taupt->Write();
  h_taueta->Write();
  h_mu1pt->Write();
  h_mu1eta->Write();
  h_tau1pt->Write();
  h_tau1eta->Write();
  h_mu2pt->Write();
  h_mu2eta->Write();
  h_tau2pt->Write();
  h_tau2eta->Write();
  h_charge1->Write();
  h_charge2->Write();
  h_musize->Write();
  h_elesize->Write();
  h_genmu->Write();
  h_genmumom->Write();
  h_genmuGmom->Write();
  h_genele->Write();
  h_genelemom->Write();
  h_gentau->Write();
  h_gentaumom->Write();
  h_geneleGmom->Write();
  h_nele->Write();
  h_mcmomele->Write();
  h_mcGmomele->Write();
  h_mcmommu->Write();
  h_mcGmommu->Write();
  h_mcmomtau->Write();
  h_mcGmomtau->Write();
  h_ntaupt->Write();
  h_ntaueta->Write();
  h_ntauele->Write();
  h_ntaumu->Write();
  h_ntauiso->Write();
  h_ntaudecay->Write();
  h_ntau->Write();
  h_ntaudr->Write();
  h_taugenmu->Write();
  h_zpt->Write();
  h_reco1mupt->Write();
  h_reco2mupt->Write();
  h_reco1mueta->Write();
  h_reco2mueta->Write();
  h_reco1taupt->Write();
  h_reco2taupt->Write();
  h_reco1taueta->Write();
  h_reco2taueta->Write();
  h_massNor->Write();
  hmm->Write();
  htt->Write();
  hmt->Write();
  fout->Close();

}

