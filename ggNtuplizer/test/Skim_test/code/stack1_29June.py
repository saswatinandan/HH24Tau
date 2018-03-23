# Code written by:  zaixing.mao@cern.ch && edward.laird@cern.ch from Brown U.                                                                  
#!/usr/bin/env python                                                                                                                          
import ROOT as r
import numpy
from sys import argv, exit, stdout, stderr
import math

if len(argv) < 2:
   print 'Usage:python xs_calculator_prefit.py DYCrossSection[optional]'

FoundXS=1.00

histDict = {}

#variable = ['InvariantMass_of_#mu_pair_with_opposite_sign_and_no_cut_on_#tau_leg_with_mass_20-200GeV','InvariantMass_of_#mu_pair_with_same_sign_and_no_cut_on_#tau_leg_with_mass_20-200GeV', 'InvariantMass_of_#mu_pair_with_opposite_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV','InvariantMass_of_#mu_pair_with_same_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV','InvariantMass_of_#mu_pair_with_opposite_sign_in_one_tau_jet_region','InvariantMass_of_#mu_pair_with_opposite_sign_in_two_tau_jet_region','InvariantMass_of_#mu_pair_with_opposite_sign_in_zero_tau_jet_region','InvariantMass_of_#mu_pair_with_same_sign_in_one_tau_jet_region','InvariantMass_of_#mu_pair_with_same_sign_in_two_tau_jet_region','InvariantMass_of_#mu_pair_with_same_sign_in_zero_tau_jet_region','InvariantMass_of_tau_pair_with_opposite_sign_muon_same_sign_tau_region','InvariantMass_of_tau_pair_with_opposite_sign_muon_opposite_sign_tau_region','InvariantMass_of_tau_pair_with_same_sign_muon_same_sign_tau_region','InvariantMass_of_tau_pair_with_same_sign_muon_opposite_sign_tau_region','InvariantMass_of_taumu_with_same_sign','InvariantMass_of_taumu_with_opposite_sign','pt_distribution_of_Leading_#mu_opposite_sign_and_no_cut_on_#tau_leg_with_mass_20-200GeV','pt_distribution_of_Leading_#mu_same_sign_and_no_cut_on_#tau_leg_with_mass_20-200GeV','pt_distribution_of_SubLeading_#mu_opposite_sign_and_no_cut_on_#tau_leg_with_mass_20-200GeV','pt_distribution_of_SubLeading_#mu_same_sign_and_no_cut_on_#tau_leg_with_mass_20-200GeV','pt_distribution_of_Leading_#mu_opposite_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV','pt_distribution_of_Leading_#mu_same_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV','pt_distribution_of_SubLeading_#mu_opposite_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV','pt_distribution_of_SubLeading_#mu_same_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV','pt_distribution_of_Leading_#tau_with_opposite_sign_muon_opposite_sign_tau_region','pt_distribution_of_Leading_#tau_with_opposite_sign_muon_same_sign_tau_region','pt_distribution_of_SubLeading_#tau_with_opposite_sign_muon_opposite_sign_tau_region','pt_distribution_of_SubLeading_#tau_with_opposite_sign_muon_same_sign_tau_region','pt_distribution_of_Leading_#tau_with_same_sign_muon_opposite_sign_tau_region','pt_distribution_of_Leading_#tau_with_same_sign_muon_same_sign_tau_region','pt_distribution_of_SubLeading_#tau_with_same_sign_muon_opposite_sign_tau_region','pt_distribution_of_SubLeading_#tau_with_same_sign_muon_same_sign_tau_region','pt_distribution_of_SubLeading_#tau_with_opposite_sign_muon_same_sign_tau_region','InvariantMass_of_4_particle_with_totCharge==0_and_mucharge==0','InvariantMass_of_4_particle_with_totCharge==0','InvariantMass_of_4_particle_with_totCharge==0_and_mucharge_not_equal_to_zero_i.e_signal_region','eta_distribution_of_Leading_#tau_with_opposite_sign_muon_same_sign_tau_region','eta_distribution_of_Leading_#tau_with_opposite_sign_muon_opposite_sign_tau_region','eta_distribution_of_SubLeading_#tau_with_opposite_sign_muon_same_sign_tau_region','eta_distribution_of_SubLeading_#tau_with_opposite_sign_muon_opposite_sign_tau_region','eta_distribution_of_Leading_#tau_with_same_sign_muon_opposite_sign_tau_region','eta_distribution_of_Leading_#tau_with_same_sign_muon_same_sign_tau_region','eta_distribution_of_SubLeading_#tau_with_same_sign_muon_opposite_sign_tau_region','eta_distribution_of_SubLeading_#tau_with_same_sign_muon_same_sign_tau_region','eta_distribution_of_SubLeading_#mu_same_sign_and_no_cut_on_#tau_leg_with_mass_20-200GeV','eta_distribution_of_Leading_#mu_same_sign_and_no_cut_on_#tau_leg_with_mass_20-200GeV','eta_distribution_of_SubLeading_#mu_opposite_sign_and_no_cut_on_#tau_leg_with_mass_20-200GeV','eta_distribution_of_Leading_#mu_opposite_sign_and_no_cut_on_#tau_leg_with_mass_20-200GeV','eta_distribution_of_SubLeading_#mu_same_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV','eta_distribution_of_Leading_#mu_same_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV','eta_distribution_of_SubLeading_#mu_opposite_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV','eta_distribution_of_Leading_#mu_opposite_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV']
#variable=['pt_distribution_of_Leading_#mu_opposite_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV','InvariantMass_of_taumu_with_opposite_sign','InvariantMass_of_#mu_pair_with_opposite_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV','InvariantMass_of_#mu_pair_with_opposite_sign_and_no_cut_on_#tau_leg_with_mass_20-200GeV']
#variable = ['pt_distribution_of_Leading_#mu_opposite_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV','InvariantMass_of_#mu_pair_with_opposite_sign_and_no_cut_on_#tau_leg_with_mass_20-200GeV','InvariantMass_of_#mu_pair_with_opposite_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV','InvariantMass_of_#mu_pair_with_opposite_sign_in_one_tau_jet_region','InvariantMass_of_#mu_pair_with_opposite_sign_in_two_tau_jet_region','InvariantMass_of_#mu_pair_with_opposite_sign_in_zero_tau_jet_region','InvariantMass_of_tau_pair_with_opposite_sign_muon_same_sign_tau_region','InvariantMass_of_tau_pair_with_opposite_sign_muon_opposite_sign_tau_region','InvariantMass_of_tau_pair_with_same_sign_muon_same_sign_tau_region','InvariantMass_of_tau_pair_with_same_sign_muon_opposite_sign_tau_region','InvariantMass_of_taumu_with_same_sign','InvariantMass_of_taumu_with_opposite_sign']
#variable = ['InvariantMass_of_4_particle_with_totCharge==0_and_mucharge==0','InvariantMass_of_4_particle_with_totCharge==0','InvariantMass_of_4_particle_with_totCharge==0_and_mucharge_not_equal_to_zero_i.e_signal_region','InvariantMass_of_tau_pair_with_opposite_sign_muon_same_sign_tau_region','InvariantMass_of_tau_pair_with_opposite_sign_muon_opposite_sign_tau_region','InvariantMass_of_tau_pair_with_same_sign_muon_same_sign_tau_region','InvariantMass_of_tau_pair_with_same_sign_muon_opposite_sign_tau_region','pt_distribution_of_Leading_#tau_with_opposite_sign_muon_opposite_sign_tau_region','pt_distribution_of_Leading_#tau_with_opposite_sign_muon_same_sign_tau_region','pt_distribution_of_SubLeading_#tau_with_opposite_sign_muon_opposite_sign_tau_region','pt_distribution_of_SubLeading_#tau_with_opposite_sign_muon_same_sign_tau_region','pt_distribution_of_Leading_#tau_with_same_sign_muon_opposite_sign_tau_region','pt_distribution_of_Leading_#tau_with_same_sign_muon_same_sign_tau_region',
#variable=['pt_distribution_of_SubLeading_#tau_with_same_sign_muon_opposite_sign_tau_region','pt_distribution_of_SubLeading_#tau_with_same_sign_muon_same_sign_tau_region','pt_distribution_of_SubLeading_#tau_with_opposite_sign_muon_same_sign_tau_region','eta_distribution_of_Leading_#tau_with_opposite_sign_muon_same_sign_tau_region','eta_distribution_of_Leading_#tau_with_opposite_sign_muon_opposite_sign_tau_region','eta_distribution_of_SubLeading_#tau_with_opposite_sign_muon_same_sign_tau_region','eta_distribution_of_SubLeading_#tau_with_opposite_sign_muon_opposite_sign_tau_region','eta_distribution_of_Leading_#tau_with_same_sign_muon_opposite_sign_tau_region','eta_distribution_of_Leading_#tau_with_same_sign_muon_same_sign_tau_region']

#variable =['InvariantMass_of_taumu_with_opposite_sign','InvariantMass_of_taumu_with_same_sign','InvariantMass_of_tau_pair_with_same_sign_muon_same_sign_tau_region','InvariantMass_of_4_particle_with_totCharge==0']
variable = ['InvariantMass_of_tau_pair_with_same_sign_muon','InvariantMass_of_taumu_with_totCharge!=0','InvariantMass_of_4_particle_with_totCharge!=0','InvariantMass_of_4_particle_with_totCharge==0','pt_distribution_of_SubLeading_#tau_with_same_sign_muon','pt_distribution_of_Leading_#tau_with_same_sign_muon']
f = r.TFile("prefit.root","recreate")
################################################                                                                                               
# Sevreal Histograms are initiated/produced here                                                                                               
defaultOrder = [('WJets',  r.TColor.GetColor(100,182,232)),
                ('TTJets', r.TColor.GetColor(155,152,204)),
                ('DY', r.TColor.GetColor(250,202,255)),
                ('WW', r.TColor.GetColor(248,206,104)),
                ('ZZ', r.TColor.GetColor(200,225,60)),
                ('HH', r.TColor.GetColor(210,70,80)),
                ('ZZTo4L', r.TColor.GetColor(30,50,120)),
                ('ST', r.TColor.GetColor(230,90,115)),
                ('WZ', r.TColor.GetColor(240,200,108)),
                ('QCD', r.TColor.GetColor(130,130,130))]



def buildHistDict(nbins,x_low,x_high):

    histDict = {}
    for iSample, iColor in defaultOrder:
       name = iSample
       histDict[name] = r.TH1F(name, '', nbins, x_low,x_high)
       histDict[name].SetFillColor(iColor)
       histDict[name].SetMarkerColor(iColor)
       histDict[name].SetMarkerStyle(21)
       histDict[name].SetLineColor(r.kBlack)

    name = 'bkg_'
    histDict[name] = r.TH1F(name, '', nbins, x_low,x_high)
    histDict[name].Sumw2()
    histDict[name].SetFillColor(r.kGray+2)
    histDict[name].SetLineColor(r.kGray+2)
    histDict[name].SetFillStyle(3344)
    name = 'data_'
    histDict[name] = r.TH1F(name, '', nbins, float(x_low),float(x_high))
    histDict[name].Sumw2()
    histDict[name].SetMarkerStyle(8)
    histDict[name].SetMarkerSize(0.9)
    histDict[name].SetMarkerColor(r.kBlack)


    return histDict
################################################                                                                                               

def setMyLegend(lPosition, lHistList):
    l = r.TLegend(lPosition[0], lPosition[1], lPosition[2], lPosition[3])
    l.SetFillStyle(0)
    l.SetBorderSize(0)
    for i in range(len(lHistList)):
                    l.AddEntry(lHistList[i][0], lHistList[i][1], lHistList[i][2])
    return l

def getBins(hist, x_low, x_high):
    bin_low = 0
    bin_high = 0

    for i in range(hist.GetNbinsX()):
        if hist.GetBinCenter(i+1) >= x_low and bin_low == -1:
            bin_low = i+1
        if hist.GetBinCenter(i+1) >= x_high and bin_high == -1:
            bin_high = i
        if bin_low != 0 and bin_high != 0:
            return bin_low, bin_high
    return -1,-1

def buildStackDict(histDict):
    stackDict = {}
    stackDict['s'] = r.THStack()

    for iSample, iColor in defaultOrder:
        scale = 1.0
        stackDict['s'].Add(histDict[iSample])
        histDict['bkg_'].Add(histDict[iSample])
    return stackDict

def FillHisto(input, output, weight = 1.0):
#    print 'inFillHisto',input,'ou== ', output                                                                                                 
    for i in range(input.GetNbinsX()):
       currentValue = output.GetBinContent(i+1)
       currentError = output.GetBinError(i+1)
       output.SetBinContent(i+1, currentValue+input.GetBinContent(i+1)*weight)
       output.SetBinError(i+1, math.sqrt((input.GetBinError(i+1))**2 + currentError**2))
#    output.Scale(1/(input.Integral()))

def buildLegendDict(histDict, position):
    legendDict = {}
    histList = {'T': []}
#    histList['T'].append((histDict['data_'+sign], 'Observed', 'lep'))                                                                         
    for iSample, iColor in reversed(defaultOrder):
       histList['T'].append((histDict[iSample], iSample, 'f'))

    legendDict['T'] = setMyLegend(position, histList['T'])
    return legendDict


def xs_calculator(fileList = []):

   for i in range(len(variable)) :
      variable_ = variable[i] + '_in_tau1iso_tau2iso'
      for iFileName, iFileLocation in fileList:
         ifile_ = r.TFile(iFileLocation)
         if ifile_.Get(variable_) :
            hist = ifile_.Get(variable_)
            nbins = hist.GetNbinsX()
            x_low = hist.GetBinLowEdge(1)
            x_high = hist.GetBinWidth(hist.GetNbinsX())+hist.GetBinLowEdge(hist.GetNbinsX())
            histDict = buildHistDict(int(nbins),x_low,x_high)
            break

      name = 'QCD_data'
      hist_data = r.TH1F(name, '', nbins, float(x_low),float(x_high))
      hist_data.Sumw2()
      
      name = 'QCD_mc'
      hist_Mc = r.TH1F(name, '', nbins, float(x_low),float(x_high))
      hist_Mc.Sumw2()
      
      c = r.TCanvas("c","Test", 800, 600)


   #loop over all the samples                                                                                                                 
      for iFileName, iFileLocation in fileList:

         ifile = r.TFile(iFileLocation)
         weight = 1.
         tauWeight = 1.

         name = 'QCD_mc'
         hist_mc = r.TH1F(name, '', nbins, float(x_low),float(x_high))
         hist_mc.Sumw2()


         hist = ifile.Get(variable_)
         
         if iFileName.find('data') !=-1: 
            hist1antiiso = ifile.Get(variable[i] +'_in_tau1anti_iso')
            hist2antiiso = ifile.Get(variable[i] +'_in_tau2anti_iso')
            histantiiso = ifile.Get(variable[i] +'_in_tau1antiso_tau2antiiso')

            if hist1antiiso : 
               print hist1antiiso.GetBinContent(1)
               hist_data.Add(hist1antiiso,1)
            if hist2antiiso : 
               print hist2antiiso.GetBinContent(1)
               hist_data.Add(hist2antiiso,1)
            if histantiiso :  
               print histantiiso.GetBinContent(1)
               hist_data.Add(histantiiso,-1)

         if hist :
            print ifile
#            if iFileName.find('data_') ==-1 and iFileName != 'ZZTo4L' and iFileName != 'HH':
 #              print iFileName
            FillHisto(ifile.Get(variable_), histDict[iFileName], tauWeight)
            if iFileName.find('data_') ==-1 and iFileName != 'ZZTo4L' and iFileName != 'HH':
               hist_Mc.Add(hist,1)

#      histDict['data_'].Add(hist_data,1)
      histDict['QCD'].Add(hist_data,1)
      histDict['QCD'].Add(hist_Mc,-1)
      stackDict = buildStackDict(histDict)
      legendDict = buildLegendDict(histDict, (0.6, 0.8 - 0.06*4, 0.85, 0.8))

      pdf = ''
      pad1 = r.TPad("pad1", "pad1", 0, 0.3, 1, 1.0)
      pad1.SetBottomMargin(0)
      pad1.SetGridx()
      pad1.Draw()
      pad1.cd()
      max_t = 1.2*max(stackDict['s'].GetMaximum(), histDict['data_'].GetMaximum())
      stackDict['s'].Draw('hist H')
      stackDict['s'].SetTitle('%s;%s;events' %(variable[i],variable[i]))
      stackDict['s'].SetMaximum(max_t)
      stackDict['s'].GetYaxis().SetTitleOffset(1.2)
      histDict['data_'].Draw('same PE')
      histDict['bkg_'].Draw('E2 same')
      legendDict['T'].Draw('same')
      c.cd()
      pad2 = r.TPad("pad2", "pad2", 0, 0.05, 1, 0.3)
      pad2.SetTopMargin(0.)
      pad2.SetBottomMargin(0.2)
      pad2.SetGridx()
      pad2.Draw()
      pad2.cd()
      h3 = histDict['data_'].Clone("h3")
      h3.SetLineColor(r.kBlack)
      h3.SetMinimum(0.5)
      h3.SetMaximum(1.5)
      h3.Sumw2()
      h3.SetStats(0)
      h3.Divide(histDict['bkg_'])
      h3.SetMarkerStyle(21)
      h3.Draw("ep")
      h3.GetXaxis().SetTitle("")
      h3.GetYaxis().SetTitle("ratio data/Mc ");
      h3.GetYaxis().SetNdivisions(505);
      h3.GetYaxis().SetTitleSize(20);
      h3.GetYaxis().SetTitleFont(43);
      h3.GetYaxis().SetTitleOffset(1.55);
      h3.GetYaxis().SetLabelFont(43)
      h3.GetYaxis().SetLabelSize(15);

      h3.GetXaxis().SetTitleSize(20);
      h3.GetXaxis().SetTitleFont(43);
      h3.GetXaxis().SetTitleOffset(2.);
      h3.GetXaxis().SetLabelFont(40);
      h3.GetXaxis().SetLabelSize(.08)
      pdf += variable[i]+'.gif'
      c.SaveAs('%s' %pdf)
      f.Write()
      f.Close()



dirName = '.'

fileList =  [('data_', '%s/Data19_controlPlot_25June.root' %dirName),
            ('HH', '%s/HH_controlPlot.root' %dirName),
            ('ZZTo4L', '%s/zzTo4L_controlPlot_25June.root' %dirName),
            ('WW', '%s/WW_controlPlot_25June.root' %dirName),
            ('TTJets', '%s/TT_controlPlot_25June.root' %dirName),
            ('WJets', '%s/WJets_controlPlot.root' %dirName),                
            ('DY', '%s/DY_controlPlot.root' %dirName),
            ('WZ', '%s/WZ_controlPlot_25June.root' %dirName),
            ('ZZ', '%s/ZZ_controlPlot_25June.root' %dirName),
            ('ST', '%s/ST_controlPlot.root' %dirName),
#            ('QCD','%s/QCDexclude20_testcontrolPlot.root' %dirName)
           ]

xs_calculator(fileList = fileList)



