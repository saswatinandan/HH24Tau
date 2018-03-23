#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <map>
#include <utility>
#include <math.h>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"
#include <TGraphAsymmErrors.h>
//#include <TLorentzVector.h>
#include <TMinuit.h>
#include "TKey.h"

#include <RooRealVar.h>
#include <RooFormulaVar.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooArgSet.h>
#include <RooPlot.h>
#include <RooAddPdf.h>
#include <RooFitResult.h>
#include <RooDataHist.h>
#include <RooSimultaneous.h>
#include <RooNumConvPdf.h>
#include <RooMsgService.h>
#include <RooHistPdf.h>

#include <RooBreitWigner.h>
#include <RooCBShape.h>
#include <RooExponential.h>
#include "RooCMSShape.h"


#include <Math/Functor.h>
#include <Fit/Fitter.h>

/*! \file FitCP.cpp
    \brief Fit 21 categories of electron pairs with analytic functions or combination of analytic functions and histograms
    Adapted from ttH multilepton analysis code in
    https://github.com/stiegerb/cmgtools-lite/blob/80X_for2016basis_chargeFlips/TTHAnalysis/python/plotter/chargeFlips/chargeMisIdProb.cc
    by  Andres Tiko <andres.tiko@cern.ch>
*/

using namespace std;
using namespace RooFit;

typedef vector<string> VString;


float getErr(float m1, float m2, float e1, float e2, int dataEventsSS) {
  //float e=pow(e1/m1,2)+pow(e2/m2,2);
  //if(m2==0) return 1;
  cout << "error " << m1 << " " << m2 << " " << e1 << " " << e2 << " " << endl;
  //return (m1/m2)*sqrt(e);
  float e=pow(e1/m1,2)+pow((e1+e2)/(m1+m2),2);
  if(m2==0) return 1;
  if(m1==0 && e1==0)
    e1 = max(1, dataEventsSS);
  //cout << "error " << m1 << " " << m2 << " " << e1 << " " << e2 << " " << sqrt(pow(e1/m2,2)) << " " << endl;
  if(m1==0) return sqrt(pow(e1/m2,2));
  return (m1/(m1+m2))*sqrt(e);
}

RooHistPdf* makeRooHistPdf(string tag, TH1* templateHistogram, RooAbsReal* fitVariable)
{
  std::string templateHistogramName_normalized = std::string(templateHistogram->GetName()).append("_normalized");
  TH1* templateHistogram_normalized = (TH1*)templateHistogram->Clone(templateHistogramName_normalized.data());

  std::string templateDataHistName = std::string(templateHistogram->GetName()).append("_dataHist");
  RooDataHist* templateDataHist = 
    new RooDataHist(templateDataHistName.data(), 
                    templateDataHistName.data(), *fitVariable, templateHistogram_normalized);
  std::string templatePdfName = std::string(templateHistogram->GetName()).append("_histPdf");
  RooHistPdf* templatePdf = 
    new RooHistPdf((tag).c_str(), 
                   templatePdfName.data(), *fitVariable, *templateDataHist);
  return templatePdf;
}

RooAddPdf* makeRooHistPdfBkg(string tag, RooRealVar* fitVariable)
{

  
  string files[] = {"a.root","b.root"};
  vector<RooHistPdf*> templatehistos;
  vector<RooRealVar*> nevents;

  for(int ifile=0; ifile<2; ifile++) {
    TH1F* h = (TH1F*)files[ifile].Get(tag);
    RooHistPdf* hpdf = makeRooHistPdf(tag, h, fitVariable);
    templatehistos.push_back(hpdf);
    RooRealVar* nevent = newRooRealVar(tag,tag,h->GetIntegral(),0,10000);
    nevents.push_back(nevent);
  }


  RooArgList listPdf, listPdfVal;

  for(unsigned int i=0; i<templatehistos.size(); i++) {
    listPdf.add(*templatehistos[i]);
    listPdfVal.Add(*nevents[i]);
  }
  RooAddPdf* bkg_total=new RooAddPdf( ("bkg_pdf_"+tag).c_str(), string("bkg shape").c_str(), listPdf, listPdfVal );
  
  return bkg_total;
}
RooNumConvPdf* shapeZ(string tag, RooRealVar* x) {

  RooMsgService::instance().getStream(1).removeTopic(RooFit::Eval);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::NumIntegration);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::DataHandling);

  RooRealVar* mZ0=new RooRealVar( ("m_Z0_"+tag).c_str(),"Z0 mass", 91.188, "GeV/c^{2}" );
  RooRealVar* gammaZ0=new RooRealVar( ("gamma_Z0_"+tag).c_str(), "Z0 width",2.4952, "GeV/c^{2}" );
  RooBreitWigner* bw0=new RooBreitWigner( ("bw0"+tag).c_str(),"true BW",*x, *mZ0, *gammaZ0);

  RooRealVar* cb_bias=new RooRealVar( ("cbb_"+tag).c_str(), "bias",0.07, -3.0, 3.0 );
  RooRealVar* cb_width=new RooRealVar( ("cbw_"+tag).c_str(),"width", 1.,0.0,5 );
  RooRealVar* cb_alpha=new RooRealVar( ("cba_"+tag).c_str(),"alpha", 1.2,0.03,2.0 );
  RooRealVar* cb_power=new RooRealVar( ("cbn_"+tag).c_str(),"power", 5 );

  RooCBShape* cb_pdf=new RooCBShape( ("cb_pdf_"+tag).c_str(), "CB shape",
				     *x,*cb_bias, *cb_width, *cb_alpha, *cb_power );

  RooNumConvPdf* bw=new RooNumConvPdf( ("sig_"+tag).c_str(),"Convolution", *x, *cb_pdf, *bw0 );

  return bw;
}


RooAbsPdf* shapeSB(string tag, RooRealVar* x, bool useSignalHisto) {


  RooMsgService::instance().getStream(1).removeTopic(RooFit::Eval);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::NumIntegration);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::DataHandling);

  RooAbsPdf* sig = nullptr;
  RooAbsPdf* bkg = nullptr;
  if (useSignalHisto == true) 
    sig = makeRooHistPdf("sig_"+tag, x);
  else 
    sig = shapeZ("sig_"+tag, x);

  if (!useSignalHisto) { //Analytic for SS in hybrid
    RooRealVar* exp_alpha = new RooRealVar( ("expa_"+tag).c_str(), "alpha", 40.0, 20.0, 160.0);
    RooRealVar* exp_beta  = new RooRealVar( ("expb_"+tag).c_str(), "beta",  0.05, 0.0, 2.0);
    RooRealVar* exp_gamma = new RooRealVar( ("expg_"+tag).c_str(), "gamma", 0.02, 0.0, 0.1);
    RooRealVar* exp_peak  = new RooRealVar( ("expp_"+tag).c_str(), "peak",  91.2);
    bkg_pdf = new RooCMSShape( ("bkg_pdf_"+tag).c_str(), string("bkg shape").c_str(),*x, *exp_alpha, *exp_beta, *exp_gamma, *exp_peak);
  }
  else{
    bkg_pdf = makeRooHistPdfBkg("bkg"+tag, x);
  }
  RooArgList* listPdf=new RooArgList( *bkg_pdf, *sig );
  RooRealVar nSig, nbkg;
  if(useSignalHisto) {
    nsig = ("nsig","nsig",sig.expectedEvents(sig.coefList()),0,2*sig.expectedEvents(sig.coefList()));
  }
  else {
    nsig = ("nsig","nsig",sig.expectedEvents(sig.coefList()),0,2*sig.expectedEvents(sig.coefList()));
  }

  if(useSignalHisto) {
    nbkg = ("nbkg","nbkg",bkg_pdf.expectedEvents(bkg_pdf.coefList()),0,2*bkg_pdf.expectedEvents(bkg_pdf.coefList()));
  }
  else {
    nbkg = ("nbkg","nbkg",nbkg_pdf.expectedEvents(nbkg_pdf.coefList()),0,2*bkg_pdf.expectedEvents(bkg_pdf.coefList()));
  }

  RooArgList* listPdfVal=new RooArgList( nBkg, nSig );
  RooAddPdf* bw_tot=new RooAddPdf( "bw_EBEB_MC", "PDF ee", *listPdf, *listPdfVal );

  return bw_tot;

}

vector<float> doSingleFit(string category, string channel, bool useSignalHisto) {

  RooMsgService::instance().getStream(1).removeTopic(RooFit::Eval); // 1 for INFO
  RooMsgService::instance().getStream(1).removeTopic(RooFit::NumIntegration);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::DataHandling);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::Minimization);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::Fitting);

  RooRealVar mass("mass","m_{ll}",60,120,"GeV");

  vector<float> v(4,0);

  /*if(data.sumEntries() < 0.001 || data.numEntries() <10 ) {
    v[0]=histo->Integral(histo->GetXaxis()->FindBin(70), histo->GetXaxis()->FindBin(110) );
    v[1]=sqrt( v[0] ); //not true for MC
    v[2]=histo->Integral()-v[0];
    v[3]=sqrt( v[2] );
    cout<<"result:\t"<<histo->GetName()<<"\t"<<v[0]<<"\t"<<v[1]<<endl;
    return v;
    }*/

  RooAbsPdf* shape=shapeSB( ("s"+category+channel),&mass, useSignalHisto);

  RooFitResult* result;
  result = shape->fitTo(data, RooFit::SumW2Error(kTRUE), RooFit::Save(kTRUE), RooFit::PrintLevel(2), Minos(true) );
  double N=nSig.getVal();
  double eN=nSig.getError();

  double NB=nBkg.getVal();
  double eNB=nBkg.getError();

  cout<<"result:\t"<<histo->GetName()<<"\t"<<N<<"\t"<<eN<< " ... " << result << endl;
  cout<<"sig error :\t"<< N << " " << eN << " " << nSig.getErrorHi() << " " << nSig.getErrorLo() << endl;
  result->covarianceMatrix().Print();

  TCanvas* c=new TCanvas( ("c"+category+channel).c_str(),("c"+category+channel).c_str());
  TCanvas* clog=new TCanvas( ("clog"+category+channel).c_str(),("clog"+category+channel).c_str());
  clog->SetLogy();
  c->cd();
  RooPlot* frame=mass.frame();
  data.plotOn(frame);
  shape->plotOn(frame);  
  
  string bw_name = "sig_s"+category+channel;
  string exp_name = "bkg_pdf_s"+category+channel;
  shape->plotOn(frame,Components(bw_name.data()),LineColor(kGreen)) ;
  shape->plotOn(frame,Components(exp_name.data()),LineColor(kRed)) ;
  frame->Draw();
  
  
  string passtag = "pass";
  string type_string = "shapes";
  if(channel == "OS")
    passtag = "fail";
  if(useSignalHisto == true)
    type_string = "hybrid";
  string name=Form("%s/%s_fit_s_%s.png", plotDir.data(), passtag.data(), type_string.data());
  c->SaveAs(name.data());

  clog->cd();
  frame->Draw();
  name=Form("%s/%s_log_fit_s_%s.png", plotDir.data(), passtag.data(), type_string.data());
  clog->SaveAs(name.data());

  delete frame;
  delete c;
  delete clog;
  delete shape;
  //delete data;


  v[0]=N;
  v[1]=eN;
  v[2]=NB;
  v[3]=eNB;

  return v;
}

vector<float> doSingleFit(string category, string channel) {
  return doSingleFit(nullptr, category, channel,false);
}

map<string, vector<float> > doFits(string tag, bool isData, string singleCateg, bool useSignalHisto = true) {

  vector<float> vs;
  map<string, vector<float> > vals_ss;
  map<string, vector<float> > vals_os;
  
  VString chns = {"SS", "OS"};

  VString cats = {"BB_LL","BB_ML","BB_MM","BB_HL","BB_HM","BB_HH",
		  "EE_LL","EE_ML","EE_MM","EE_HL","EE_HM","EE_HH",
		  "BE_LL","BE_ML","EB_ML","BE_MM","BE_HL","EB_HL",
		  "BE_HM","EB_HM","BE_HH"};  

  ofstream myfile;  
  myfile.open(Form("fit_results.txt"));

  for (int i=0; i<21; i++) {
    string cat = cats[i]+"_";
    for (auto channel : chns) {
      vs = doSingleFit(cat, channel,useSignalHisto);
      if (channel == "SS")
	vals_ss[ cat ] = vs;
      else
	vals_os[ cat ] = vs;
    }
    double ratio = vals_ss[cat][0] / (vals_ss[cat][0] + vals_os[cat][0]);
    
    //Hack for both fits zero case:
    if (vals_ss[cat][0] == 0 && vals_os[cat][0] == 0)
      ratio = 0.; 
    
    //    double error = getErr(vals_ss[cat][0], vals_os[cat][0], vals_ss[cat][1], vals_os[cat][1], dataEventsSS);
    myfile << i << ", " << ratio << endl;//", " << error << ", " << error << "\n";
    
  }
  myfile.close();
  return vals_ss;

}
int main(int argc, char* argv[]) {

  bool isData=false;
  bool useSignalHisto = false;
  string singleCateg="";
  string tag = "noname";
  char c;

  while ((c = getopt(argc, argv, "s:D:t:a:h")) != -1 ) {
    switch (c) {
      //case 'd': { file=optarg; break;}
    case 's': { singleCateg=string(optarg); break;}
    case 'D': { isData=bool(atoi(optarg)); break; }
    case 't': { tag=string(optarg); break; }
    case 'a': { useSignalHisto=!bool(atoi(optarg)); break; }
    default : {
      cout<<"configuration options:\n -f : file to read (root) \n -s <categ> perform a fit over a single Z category. \n -D run on data (0 per default). \n -a use analytic function for signal (instead of histogram). \n -t tag \n -h help \n"<<endl;
      return 0; }
    }
  }
  //==============================================
  map<string, vector<float> > vals=doFits(tag, isData, singleCateg, useSignalHisto);
}
