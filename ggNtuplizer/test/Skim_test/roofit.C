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

int roofit() {
  RooRealVar* x = new RooRealVar("x", "alpha", 40.0, 20.0, 160.0);
  TCanvas c;
  RooRealVar* cb_bias=new RooRealVar( "cbb_", "bias",0.07, -3.0, 3.0 );
  std::cout << cb_bias << std::endl;
  RooCMSShape* bkg_pdf;
  RooRealVar* exp_alpha = new RooRealVar( "expa_", "alpha", 40.0, 20.0, 160.0);
  RooRealVar* exp_beta  = new RooRealVar( "expb_", "beta",  0.05, 0.0, 2.0);
  RooRealVar* exp_gamma = new RooRealVar( "expg_", "gamma", 0.02, 0.0, 0.1);
  RooRealVar* exp_peak  = new RooRealVar( "expp_", "peak",  91.2);
  bkg_pdf = new RooCMSShape( string("bkg_pdf_").c_str(), string("bkg shape").c_str(),
                             *x, *exp_alpha, *exp_beta, *exp_gamma, *exp_peak);

  RooPlot* xframe = x->frame() ;
  bkg_pdf->plotOn(xframe) ;
  xframe->Draw() ;
  c.SaveAs("roofit.eps");
  return 0;
}

