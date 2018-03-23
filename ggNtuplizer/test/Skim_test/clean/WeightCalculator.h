#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"
#include "TF1.h"
#include "TSystem.h"
#include "TMath.h" //M_PI is in TMath
#include "TRandom3.h"
#include <TLorentzVector.h>
using namespace std;


float WScaleFactor=1.0; // computed on Apr 20th
//float TTScaleFactor=0.91;
//float WScaleFactor=1.22;
//float TTScaleFactor=0.856;
vector <float> W_EvenetMultiplicity(){

  vector<float> W_events;
  W_events.clear();
  //TFile * myFile_W0 = new TFile("WJetsToLNu.root");
  TFile * myFile_W0 = new TFile("/eos/uscms/store/user/snandan/WJetsToLNu_EE.root");
  //TFile * myFile_W0 = new TFile("/eos/uscms/store/user/snandan/WJetsToLNu_ME.root");
  //TFile * myFile_W0 = new TFile("/eos/uscms/store/user/snandan/WJetsToLNu_Wjets.root");
  //TFile * myFile_W0 = new TFile("/eos/uscms/store/user/snandan/WJetsToLNu_Wjets_EE.root");
  TH1F * Histo_W0 = (TH1F*) myFile_W0->Get("hcount");
  W_events.push_back(Histo_W0->GetBinContent(2));
    
  //TFile * myFile_W1 = new TFile("W1JetsToLNu.root");
  //TFile * myFile_W1 = new TFile("/eos/uscms/store/user/snandan/W1JetsToLNu_Wjets.root");
  TFile * myFile_W1 = new TFile("/eos/uscms/store/user/snandan/W1JetsToLNu_EE.root");
  //TFile * myFile_W1 = new TFile("/eos/uscms/store/user/snandan/W1JetsToLNu_ME.root");
  //TFile * myFile_W1 = new TFile("/eos/uscms/store/user/snandan/W1JetsToLNu_Wjets_EE.root");
  TH1F * Histo_W1 = (TH1F*) myFile_W1->Get("hcount");
  W_events.push_back(Histo_W1->GetBinContent(2));
    
  //TFile * myFile_W2 = new TFile("W2JetsToLNu.root");
  //TFile * myFile_W2 = new TFile("/eos/uscms/store/user/snandan/W2JetsToLNu_Wjets.root");
  TFile * myFile_W2 = new TFile("/eos/uscms/store/user/snandan/W2JetsToLNu_EE.root");
  //TFile * myFile_W2 = new TFile("/eos/uscms/store/user/snandan/W2JetsToLNu_ME.root"); 
  //TFile * myFile_W2 = new TFile("/eos/uscms/store/user/snandan/W2JetsToLNu_Wjets_EE.root");
  TH1F * Histo_W2 = (TH1F*) myFile_W2->Get("hcount");
  W_events.push_back(Histo_W2->GetBinContent(2));
    
  //TFile * myFile_W3 = new TFile("W3JetsToLNu.root");
  //TFile * myFile_W3 = new TFile("/eos/uscms/store/user/snandan/W3JetsToLNu_Wjets.root");
  TFile * myFile_W3 = new TFile("/eos/uscms/store/user/snandan/W3JetsToLNu_EE.root");
  //TFile * myFile_W3 = new TFile("/eos/uscms/store/user/snandan/W3JetsToLNu_ME.root");
    //TFile * myFile_W3 = new TFile("/eos/uscms/store/user/snandan/W3JetsToLNu_Wjets_EE.root");
  TH1F * Histo_W3 = (TH1F*) myFile_W3->Get("hcount");
  W_events.push_back(Histo_W3->GetBinContent(2));
    
  //TFile * myFile_W4 = new TFile("W4JetsToLNu.root");
  //TFile * myFile_W4 = new TFile("/eos/uscms/store/user/snandan/W4JetsToLNu_Wjets.root");
  TFile * myFile_W4 = new TFile("/eos/uscms/store/user/snandan/W4JetsToLNu_EE.root");
  //TFile * myFile_W4 = new TFile("/eos/uscms/store/user/snandan/W4JetsToLNu_ME.root");
  //TFile * myFile_W4 = new TFile("/eos/uscms/store/user/snandan/W4JetsToLNu_Wjets_EE.root");
  TH1F * Histo_W4 = (TH1F*) myFile_W4->Get("hcount");
  W_events.push_back(Histo_W4->GetBinContent(2));
    
  return W_events ;
    
}

vector <float> DY_EvenetMultiplicity(){
  vector<float> DY_events;
  DY_events.clear();
  //TFile * myFile_DY0 = new TFile("DYJetsToLL.root");
  //TFile * myFile_DY0 = new TFile("/eos/uscms/store/user/snandan/DYJetsToLL_Wjets.root");
  TFile * myFile_DY0 = new TFile("/eos/uscms/store/user/snandan/DYJetsToLL_EE.root");
  //TFile * myFile_DY0 = new TFile("/eos/uscms/store/user/snandan/DYJetsToLL_ME.root");
  //TFile * myFile_DY0 = new TFile("/eos/uscms/store/user/snandan/DYJetsToLL_Wjets_EE.root");
  TH1F * Histo_DY0 = (TH1F*) myFile_DY0->Get("hcount");
  DY_events.push_back(Histo_DY0->GetBinContent(2));
    
  //TFile * myFile_DY1 = new TFile("DY1JetsToLL.root");
  //TFile * myFile_DY1 = new TFile("/eos/uscms/store/user/snandan/DY1JetsToLL_Wjets.root");
    TFile * myFile_DY1 = new TFile("/eos/uscms/store/user/snandan/DY1JetsToLL_EE.root");
  //TFile * myFile_DY1 = new TFile("/eos/uscms/store/user/snandan/DY1JetsToLL_ME.root"); 
  //TFile * myFile_DY1 = new TFile("/eos/uscms/store/user/snandan/DY1JetsToLL_Wjets_EE.root");
  TH1F * Histo_DY1 = (TH1F*) myFile_DY1->Get("hcount");
  DY_events.push_back(Histo_DY1->GetBinContent(2));
    
  //TFile * myFile_DY2 = new TFile("DY2JetsToLL.root");
  //  TFile * myFile_DY2 = new TFile("/eos/uscms/store/user/snandan/DY2JetsToLL_Wjets.root");
  TFile * myFile_DY2 = new TFile("/eos/uscms/store/user/snandan/DY2JetsToLL_EE.root");
  //TFile * myFile_DY2 = new TFile("/eos/uscms/store/user/snandan/DY2JetsToLL_ME.root");
  //TFile * myFile_DY2 = new TFile("/eos/uscms/store/user/snandan/DY2JetsToLL_Wjets_EE.root"); 
  TH1F * Histo_DY2 = (TH1F*) myFile_DY2->Get("hcount");
  DY_events.push_back(Histo_DY2->GetBinContent(2));
    
  //TFile * myFile_DY3 = new TFile("DY3JetsToLL.root");
  //  TFile * myFile_DY3 = new TFile("/eos/uscms/store/user/snandan/DY3JetsToLL_Wjets.root");
    TFile * myFile_DY3 = new TFile("/eos/uscms/store/user/snandan/DY3JetsToLL_EE.root");
  //TFile * myFile_DY3 = new TFile("/eos/uscms/store/user/snandan/DY3JetsToLL_ME.root"); 
  //TFile * myFile_DY3 = new TFile("/eos/uscms/store/user/snandan/DY3JetsToLL_Wjets_EE.root");
  TH1F * Histo_DY3 = (TH1F*) myFile_DY3->Get("hcount");
  DY_events.push_back(Histo_DY3->GetBinContent(2));
    
  //TFile * myFile_DY4 = new TFile("DY4JetsToLL.root");
  //  TFile * myFile_DY4 = new TFile("/eos/uscms/store/user/snandan/DY4JetsToLL_Wjets.root");
  TFile * myFile_DY4 = new TFile("/eos/uscms/store/user/snandan/DY4JetsToLL_EE.root");
  //TFile * myFile_DY4 = new TFile("/eos/uscms/store/user/snandan/DY4JetsToLL_ME.root");
  //TFile * myFile_DY4 = new TFile("/eos/uscms/store/user/snandan/DY4JetsToLL_Wjets_EE.root"); 
  TH1F * Histo_DY4 = (TH1F*) myFile_DY4->Get("hcount");
  DY_events.push_back(Histo_DY4->GetBinContent(2));
    
  return DY_events ;
    
}

float XSection(std::string OutName) {
    
    
  ////////////////////////////////////////////////////////////////
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns
    
  ////////////////////////////////////////////////////////////////
    
  //    https://docs.google.com/spreadsheets/d/1rWM3AlFKO8IJVaeoQkWZYWwSvicQ1QCXYSzH74QyZqE/edit?alt=json#gid=398123591
    
    
    
  if (OutName.find("WJetsToLNu") != string::npos) {//cout << "Wjet " << endl;
    return 50690;  } // As we have large cut at Skim, this one is not needed

  else if (OutName.find("W1JetsToLNu") != string::npos) {
    //cout << "W1" << endl;
    return 9644.5 ;
  }
  else if (OutName.find("W2JetsToLNu") != string::npos) return 3144.5 ;
  else if (OutName.find("W3JetsToLNu") != string::npos) return 954.8 ;
  else if (OutName.find("W4JetsToLNu") != string::npos) return 485.6 ;

  else if (OutName.find("WJetsToLNu_HT-70to100") != string::npos) return 0;
  else if (OutName.find("WJetsToLNu_HT-100to200") != string::npos) return 1343;
  else if (OutName.find("WJetsToLNu_HT-200to400") != string::npos) return 359.6;
  else if (OutName.find("WJetsToLNu_HT-400to600") != string::npos) return 48.85;
  else if (OutName.find("WJetsToLNu_HT-600to800") != string::npos) return 12.05;
  else if (OutName.find("WJetsToLNu_HT-800to1200") != string::npos) return 5.501;
  else if (OutName.find("WJetsToLNu_HT-1200to2500") != string::npos) return 1.329;
  else if (OutName.find("WJetsToLNu_HT-2500toInf") != string::npos) return 0.03216;
    
    
  else if (OutName.find("DYJetsToLL") != string::npos) {
    //        cout << "DY" << endl;
    //    return 5765.4; // As we have large cut at Skim, this one is not needed
    return 4895;
  }
  else if (OutName.find("DY1JetsToLL") != string::npos) {
    //    cout << "DY1111111111111111" << endl;
    return 1012.5;}
  else if (OutName.find("DY2JetsToLL") != string::npos) return 332.8;
  else if (OutName.find("DY3JetsToLL") != string::npos) return 101.8;
  else if (OutName.find("DY4JetsToLL") != string::npos) return 54.8;
  
  else if (OutName.find("DYJetsToLL_M-50_HT-70to100") != string::npos) return 0;
  else if (OutName.find("DYJetsToLL_M-50_HT-100to200") != string::npos) return 148;
  else if (OutName.find("DYJetsToLL_M-50_HT-200to400") != string::npos) return 40.94;
  else if (OutName.find("DYJetsToLL_M-50_HT-400to600") != string::npos) return 5.497;
  else if (OutName.find("DYJetsToLL_M-50_HT-600to800") != string::npos) return 1.354;
  else if (OutName.find("DYJetsToLL_M-50_HT-800to1200") != string::npos) return 0.625;
  else if (OutName.find("DYJetsToLL_M-50_HT-1200to2500") != string::npos) return 0.151;
  else if (OutName.find("DYJetsToLL_M-50_HT-2500toInf") != string::npos) return 0.003647;

    
  //Di-boson
  //  else if (OutName.find("WW") != string::npos) return 115.0;
  else if (OutName.find("WW") != string::npos) return 63.21;
  //  else if (OutName.find("WZ") != string::npos) return 47.13;
  else if (OutName.find("WZ") != string::npos) return 22.82;
  //  else if (OutName.find("ZZ") != string::npos) return 16.523;
  else if (OutName.find("ZZ") != string::npos) return 10.32;
  else if (OutName.find("zzTo4L") != string::npos) return 1.2;
  else if (OutName.find("HH") != string::npos) return 0.05;
  else if (OutName.find("Radion") != string::npos || OutName.find("BulkGraviton") != string::npos || OutName.find("nonreso") != string::npos || OutName.find("RSGraviton") != string::npos) return 1;
  else if (OutName.find("ZH") != string::npos) return 0.0559;
    
  //    /ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM
  //    /ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM
  //    /ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
  //    /ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
    
    
  //    -rw-r--r--  1 abdollah1  228379582 Feb 27 13:15 ../ROOT80X/ST_t-channel_antitop_4f_inclusiveDecays.root
  //    -rw-r--r--  1 abdollah1  450329489 Feb 27 13:15 ../ROOT80X/ST_t-channel_top_4f_inclusiveDecays.root
  //    -rw-r--r--  1 abdollah1  312186239 Feb 27 13:15 ../ROOT80X/ST_tW_antitop_5f.root
  //    -rw-r--r--  1 abdollah1  317698343 Feb 27 13:16 ../ROOT80X/ST_tW_top_5f.root
  //
  //    
    
  //SingleTop
  else if (OutName.find("ST_t-channel_antitop") != string::npos) return 80.95;
  else if (OutName.find("ST_t-channel_top") != string::npos) return 136.02;
  //  else if (OutName.find("ST_tW_antitop_5f") != string::npos) return 35.6;
  else if (OutName.find("ST_tW_antitop_5f") != string::npos) return 38.09;
  //  else if (OutName.find("ST_tW_top_5f") != string::npos) return 35.6;
  else if (OutName.find("ST_tW_top_5f") != string::npos) return 38.09;
    
    
  //  else if (OutName.find("TT") != string::npos) return (831.76);
  else if (OutName.find("TT") != string::npos) return (730);

  else if (OutName.find("Codex") != string::npos ) return      1.0;
    
    
  /*  else if (OutName.find("QCD_Pt-20toInf_MuEnrichedPt15") != string::npos) return     720648000  * 0.00042 ;
  else if (OutName.find("QCD_1000") != string::npos) return     10.4305*0.15544;
  else if (OutName.find("QCD_120to170") != string::npos) return     469797*0.05362;
  //else if (OutName.find("QCD_120to170") != string::npos) return     469797;
  else if (OutName.find("QCD_15to20") != string::npos) return     1273190000*.003;
  //else if (OutName.find("QCD_15to20") != string::npos) return     1273190000;
  else if (OutName.find("QCD_170") != string::npos) return  117989*0.07335;
  else if (OutName.find("QCD_20to30") != string::npos) return  558528000*.0053;
  //else if (OutName.find("QCD_20to30") != string::npos) return  558528000;
  else if (OutName.find("QCD_300") != string::npos) return  7820.25*0.10196;
  else if (OutName.find("QCD_30to50") != string::npos) return  139803000*0.01182;
  //else if (OutName.find("QCD_30to50") != string::npos) return  139803000;
  else if (OutName.find("QCD_470") != string::npos) return  645.528*.071;
  else if (OutName.find("QCD_50t080") != string::npos) return  19222500*0.02276;
  //else if (OutName.find("QCD_50t080") != string::npos) return  19222500;
  else if (OutName.find("QCD_600") != string::npos) return  187.109*0.13412;
  else if (OutName.find("QCD_800") != string::npos) return  32.3486*0.14552;
  else if (OutName.find("QCD_80to120") != string::npos) return  2758420*0.03844;
  //else if (OutName.find("QCD_80to120") != string::npos) return  2758420;*/
  else if (OutName.find("QCD_Pt-20toInf_MuEnrichedPt15") != string::npos) return     720648000;
  else if (OutName.find("QCD_1000") != string::npos) return     10.4305;
  else if (OutName.find("QCD_120to170") != string::npos) return     469797;
  else if (OutName.find("QCD_15to20") != string::npos) return     1273190000;
  else if (OutName.find("QCD_170") != string::npos) return  117989;
  else if (OutName.find("QCD_20to30") != string::npos) return  558528000;
  else if (OutName.find("QCD_300") != string::npos) return  7820.25;
  else if (OutName.find("QCD_30to50") != string::npos) return  139803000;
  else if (OutName.find("QCD_470") != string::npos) return  645.528;
  else if (OutName.find("QCD_50t080") != string::npos) return  19222500;
  else if (OutName.find("QCD_600") != string::npos) return  187.109;
  else if (OutName.find("QCD_800") != string::npos) return  32.3486;
  else if (OutName.find("QCD_80to120") != string::npos) return  2758420;
  else if (OutName.find("QCD") != string::npos) {
    //cout << "QCD" << endl;
    return 720648000 * 0.00042 ;
  }
  else {
    cout<<"\n\n*********\nNot Listed in XSection menu !!!! Watch cout    "<<OutName<< "\n\n*********\n";
    return 1;
  }
}

//float weightCalc(TH1F *Histo,std::string outputName) {
float weightCalc(TH1F *Histo, std::string outputName, int njet, vector<float> W_events, vector<float> DY_events) {
    
  //    cout<< "outputName is "<<outputName << "  and histoname is " <<Histo->GetName()<<  " Histo->GetBinContent(1)="<<Histo->GetBinContent(1)<< " XSection(wjet)=" <<XSection("WJets")<<"\n";
    
    
  //    cout<<"--->  Check Name is "<<newOut<<"\n";
  double LOtoNLO_DY = 1.177814096;
  //  float LOtoNLO_W = 1.233848684;
  double LOtoNLO_W = 1.189386467;
  //  vector<float> LOtoNLO_DY[5] = {1.177814096,1.0,1.0,1.0,1.0};
  //vector<double> LOtoNLO_W[5] = {1.189386467,1.,1.,1.,1.};
  //    float LOtoNLO_DY = 1.230888662;
  // float LOtoNLO_DY = 1; // Now we boson have pt dependent SF
  //    float LOtoNLO_W = 1.213783784;
  //float LOtoNLO_W = 1;  // Now we boson have pt dependent SF
  //    float luminosity=2154;
  //    float luminosity=    3990;
  //    float luminosity=    6260;
  //    float luminosity=    9235;
  //    float luminosity=    12900;
  //  5.761+2.573+4.248+4.009+3.102+7.540+8.391+0.215
  //78173197+33279413+26691984+27025933+20178544+44581284+46809967+1218674=277958996
  //ME 5780+2573+4248+4009+3102+7540+8391+215;
  //EE 5785+2573+4248+4009+3102+7540+8391+215
  //SE 5597+2562+4239+3955+3035+7391+8387+215
   double luminosity=    35867;//Mu
  //  double luminosity = 35381;//SE
   // double luminosity = 35858;//MuE
  //float luminosity=    35839;
    
  size_t isDoubleMu = outputName.find("DoubleMuon");
  if (isDoubleMu != string::npos) return 1;
  else if(outputName.find("JetsToLL") != string::npos) {
    if (njet == 0) return luminosity*LOtoNLO_DY / (DY_events[0] / XSection("DYJetsToLL"));
    else if (njet == 1) return luminosity*LOtoNLO_DY / (DY_events[1] / XSection("DY1JetsToLL") + DY_events[0] / XSection("DYJetsToLL"));
    else if (njet == 2) return luminosity*LOtoNLO_DY / (DY_events[2] / XSection("DY2JetsToLL") + DY_events[0] / XSection("DYJetsToLL"));
    else if (njet == 3) return luminosity*LOtoNLO_DY / (DY_events[3] / XSection("DY3JetsToLL") + DY_events[0] / XSection("DYJetsToLL"));
    else if (njet == 4) return luminosity*LOtoNLO_DY / (DY_events[4] / XSection("DY4JetsToLL") + DY_events[0] / XSection("DYJetsToLL"));
    else {cout<<"**********   wooow  ********* There is a problem here\n";return 0;}
  }

  else if (outputName.find("JetsToLNu") != string::npos) {
    if (njet == 0) return luminosity / (W_events[0] / (LOtoNLO_W*XSection("WJetsToLNu")));
    else if (njet == 1) return luminosity*LOtoNLO_W / (W_events[1] / XSection("W1JetsToLNu") + W_events[0] / XSection("WJetsToLNu"));
    else if (njet == 2) return luminosity*LOtoNLO_W / (W_events[2] / XSection("W2JetsToLNu") + W_events[0] / XSection("WJetsToLNu"));
    else if (njet == 3) return luminosity*LOtoNLO_W / (W_events[3] / XSection("W3JetsToLNu") + W_events[0] / XSection("WJetsToLNu"));
    else if (njet == 4) return luminosity*LOtoNLO_W / (W_events[4] / XSection("W4JetsToLNu") + W_events[0] / XSection("WJetsToLNu"));
    else {cout<<"**********   wooow  ********* There is a problem here\n";return 0;}
  }
  else
    return  XSection(outputName)*luminosity / Histo->GetBinContent(2);
}

