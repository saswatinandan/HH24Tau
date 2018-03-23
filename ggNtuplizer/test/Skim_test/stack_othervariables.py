# Code written by:  zaixing.mao@cern.ch && edward.laird@cern.ch from Brown U.                                                                  
#!/usr/bin/env python                                                                                                                          
import ROOT as r
import numpy
from sys import argv, exit, stdout, stderr
import math

histDict = {}

#variable =['InvariantMass_of_taumu_with_totCharge!=0','InvariantMass_of_4_particle_with_totCharge!=0', InvariantMass_of_4_particle_with_opposite_sign_muon_in_tau1iso_tau2iso
 #          'InvariantMass_of_4_particle_with_totCharge==0','InvariantMass_of_taumu_with_totCharge==0']
#variable = ['InvariantMass_of_taumu','InvariantMass_of_3_particle']
variable =[#'InvariantMass_of_#ele_pair_with_opposite_sign_and_no_cut_on_#tau_leg_with_mass_60-120GeV',
    'pt_distribution_of_Leading_lepton_with_opposite_sign_part_with_totCharge==0','pt_distribution_of_SubLeading_lepton_with_opposite_sign_part_with_totCharge==0',
           'pt_distribution_of_Leading_#tau_with_opposite_sign_part_with_totCharge==0','pt_distribution_of_SubLeading_#tau_with_opposite_sign_part_with_totCharge==0',
           'eta_distribution_of_Leading_lepton_with_opposite_sign_part_with_totCharge==0','eta_distribution_of_SubLeading_lepton_with_opposite_sign_part_with_totCharge==0',
           'eta_distribution_of_Leading_#tau_with_opposite_sign_part_with_totCharge==0','eta_distribution_of_SubLeading_#tau_with_opposite_sign_part_with_totCharge==0',
           'InvariantMass_of_4_particle_with_same_sign_part_with_totCharge==0_be4_mass_cut','InvariantMass_of_4_particle_with_same_sign_part_with_totCharge!=0_be4_mass_cut',
           'InvariantMass_of_4_particle_with_opposite_sign_part_with_totCharge==0_be4_mass_cut','InvariantMass_of_4_particle_with_opposite_sign_part_with_totCharge!=0_be4_mass_cut']
title =['Leading lepton p_{T} [GeV]','Sub-leading lepton p_{T} [GeV]','Leading #tau p_{T} [GeV]','Sub-leading #tau p_{T} [GeV]','Leading lepton #eta','Sub-leading lepton #eta','Leading #tau #eta','Sub-leading #tau #eta','m_{#mue#tau#tau} [GeV]','m_{#mue#tau#tau} [GeV]','m_{#mue#tau#tau} [GeV]','m_{#mue#tau#tau} [GeV]']
f = r.TFile("prefit.root","recreate")
################################################                                                                                               
# Sevreal Histograms are initiated/produced here                                                                                               
defaultOrder = [('WJets',  r.TColor.GetColor(100,182,232)),
                ('t#bar{t}+jets', r.TColor.GetColor(155,152,204)),
                ('DY', r.TColor.GetColor(250,202,255)),
                ('WW', r.TColor.GetColor(248,206,104)),
                ('ZZ', r.TColor.GetColor(200,225,60)),
                ('ZZTo4L', r.TColor.GetColor(30,50,120)),
                ('ZH', r.TColor.GetColor(260,60,150)),
                ('Single Top', r.TColor.GetColor(230,90,115)),
                ('WZ', r.TColor.GetColor(200, 2, 285)),
                ('QCD multijet', r.TColor.GetColor(130,130,130))]



def buildHistDict(nbins,x_low,x_high):

    histDict = {}
    for iSample, iColor in defaultOrder:
       name = iSample
       histDict[name] = r.TH1F(name, '', nbins, x_low,x_high)
       histDict[name].SetFillColor(iColor)
       histDict[name].SetMarkerColor(iColor)
       histDict[name].SetMarkerStyle(21)
       histDict[name].SetLineColor(r.kBlack)
       histDict[name].GetXaxis().SetLabelSize(100)
       

    name = 'bkg_'
    histDict[name] = r.TH1F(name, '', nbins, x_low,x_high)
    histDict[name].Sumw2()
    histDict[name].SetMarkerSize(0)
    histDict[name].SetFillColor(r.kGray+2)
    histDict[name].SetLineColor(r.kGray+2)
    histDict[name].SetFillStyle(3344)
    name = 'data_'
    histDict[name] = r.TH1F(name, '', nbins, float(x_low),float(x_high))
    histDict[name].Sumw2()
    histDict[name].SetMarkerStyle(8)
    histDict[name].SetMarkerSize(0.9)
    histDict[name].SetMarkerColor(r.kBlack)
    histDict[name].GetXaxis().SetNdivisions(505)
    histDict[name].GetYaxis().SetLabelFont(42)
    histDict[name].GetYaxis().SetLabelOffset(0.01)
    histDict[name].GetYaxis().SetLabelSize(0.16)
    histDict[name].GetYaxis().SetTitleSize(0.075)
    histDict[name].GetYaxis().SetTitleOffset(1.04)
    histDict[name].SetMarkerStyle(20)
    histDict[name].SetMarkerSize(1)

    return histDict
################################################                                                                                               

def setMyLegend(lPosition, lHistList):
    l = r.TLegend(lPosition[0], lPosition[1], lPosition[2], lPosition[3])
    l.SetFillStyle(0)
    l.SetLineWidth(0)
    l.SetLineStyle(0)
    l.SetBorderSize(0)
    l.SetTextFont(62)

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

def FillHisto(input, output):

    for i in range(input.GetNbinsX()):
       currentValue = output.GetBinContent(i+1)
       currentError = output.GetBinError(i+1)
       output.SetBinContent(i+1, currentValue+input.GetBinContent(i+1))
       output.SetBinError(i+1, math.sqrt((input.GetBinError(i+1))**2 + currentError**2))

def buildLegendDict(histDict, position):
    legendDict = {}
    histList = {'T': []}

    for iSample, iColor in reversed(defaultOrder):
        histList['T'].append((histDict[iSample], iSample, 'f'))

    histList['T'].append((histDict['bkg_'], 'Uncertainity', 'f'))
    histList['T'].append((histDict['data_'], 'Observed','elp'))
    legendDict['T'] = setMyLegend(position, histList['T'])
    return legendDict


def add_lumi():
    lowX=0.65
    lowY=0.82
    lumi  = r.TPaveText(lowX, lowY+0.06, lowX+0.30, lowY+0.16, "NDC")
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.SetTextSize(0.06)
    lumi.SetTextFont (   42 )
    lumi.AddText("35.9 fb^{-1} (13 TeV)")
    return lumi

def add_CMS():
    lowX=0.21
    lowY=0.82
    lumi  = r.TPaveText(lowX, lowY+0.06, lowX+0.15, lowY+0.16, "NDC")
    lumi.SetTextFont(61)
    lumi.SetTextSize(0.08)
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.AddText("CMS Preliminary")
    return lumi



def xs_calculator(fileList = []):

   r.gStyle.SetFrameLineWidth(3)
   r.gStyle.SetLineWidth(3)
   r.gStyle.SetOptStat(0)

   for i in range(len(variable)) :

    l = [variable[i] +'_in_tau1iso_tau2iso']
       
#    l = [variable[i]]
 
    for var in range(len(l)) :
      for iFileName, iFileLocation in fileList:
         ifile_ = r.TFile(iFileLocation)
         if ifile_.Get(l[var]) :
            hist = ifile_.Get(l[var])
            nbins = hist.GetNbinsX()
            x_low = hist.GetBinLowEdge(1)
            x_high = hist.GetBinWidth(hist.GetNbinsX())+hist.GetBinLowEdge(hist.GetNbinsX())
            histDict = buildHistDict(int(nbins),x_low,x_high)
            break

      hist1_data = r.TH1F('tau1_data', '', nbins, float(x_low),float(x_high))
      hist1_data.Sumw2()
      hist2_data = r.TH1F('tau2_data', '', nbins, float(x_low),float(x_high))
      hist2_data.Sumw2()
      hist12_data = r.TH1F('tau12_data', '', nbins, float(x_low),float(x_high))
      hist12_data.Sumw2()
      hist1_Mc = r.TH1F('tau1_Mc', '', nbins, float(x_low),float(x_high))
      hist1_Mc.Sumw2()
      hist2_Mc = r.TH1F('tau2_Mc', '', nbins, float(x_low),float(x_high))
      hist2_Mc.Sumw2()
      hist12_Mc = r.TH1F('tau12_Mc', '', nbins, float(x_low),float(x_high))
      hist12_Mc.Sumw2()
      xbin = numpy.array([40,50,60,70,80,90,100,110,120,200])
      c = r.TCanvas("c","Test", 600, 600)
      print l[var]
      pdf = ''

   #loop over all the samples                                                                                                                 
      for iFileName, iFileLocation in fileList:
          
         if pdf =='' :
             if iFileLocation.find('Muon') !=-1 : pdf = 'Muon'
             if iFileLocation.find('Electron') !=-1 : pdf ='Electron'
             if iFileLocation.find('MuElectron') !=-1 : pdf ='MuElectron'

         ifile = r.TFile(iFileLocation)

         hist = ifile.Get(l[var])

         if l[var] == variable[i] +'_in_tau1iso_tau2iso': ###### we need to calculate the fake tau which is #of events with 1st tau antiisolated + # of events with 2nd tau antiisolated - # of events both tau antiisolated
             hist1antiiso = ifile.Get(variable[i] +'_in_tau1anti_iso')
             hist2antiiso = ifile.Get(variable[i] +'_in_tau2anti_iso')
             histantiiso = ifile.Get(variable[i] +'_in_tau1antiso_tau2antiiso')
         
             if iFileName.find('data') !=-1: 
                 if hist1antiiso :  
                     hist1_data.Add(hist1antiiso,1)
                 if hist2antiiso : 
                     hist2_data.Add(hist2antiiso,1)
                 if histantiiso :  
                     hist12_data.Add(histantiiso,1)
             else: ### need to sum all the MC samples
                 if hist1antiiso : hist1_Mc.Add(hist1antiiso,1)
                 if hist2antiiso : hist2_Mc.Add(hist2antiiso,1)
                 if histantiiso :  hist12_Mc.Add(histantiiso,1)

         if hist :
            print ifile,'\t', hist.Integral()
            FillHisto(hist, histDict[iFileName])
      if l[var] == variable[i] +'_in_tau1iso_tau2iso' :
          hist1_data.Add(hist1_Mc,-1) ##### subtract MC from data in 3 different histograms
          hist2_data.Add(hist2_Mc,-1)
          hist12_data.Add(hist12_Mc,-1)
          hist1_data.Add(hist2_data)
          hist1_data.Add(hist12_data,-1)
          for ibin in range(hist1_data.GetNbinsX()) :
              if hist1_data.GetBinContent(ibin+1) < 0 : ##### make _ve contribution equal to zero
                  hist1_data.SetBinContent(ibin+1,0)
          histDict['QCD multijet'].Add(hist1_data,1)    #### 
          print 'total # of events in QCD = ' , histDict['QCD multijet'].Integral()

######don't need to go further  below #########
##      for key in histDict :
 #         histDict[key] = histDict[key].Rebin(9,'',xbin)
 #     ifile = r.TFile('dataMoriondPU.root')
  #    hist = ifile.Get('pileup')
   #   hist.Scale(1/hist.Integral())
    #  histDict['data_'].Add(hist,1)
      stackDict = buildStackDict(histDict)
      legendDict = buildLegendDict(histDict, (0.72, 0.48, 0.99, 0.86))

      pad1 = r.TPad("pad1","pad1",0,0.35,1,1)
      pad1.Draw()
      pad1.cd()
      pad1.SetFillColor(0)
      pad1.SetBorderMode(0)
      pad1.SetBorderSize(10)
      pad1.SetTickx(1)
      pad1.SetTicky(1)
      pad1.SetLeftMargin(0.18)
      pad1.SetRightMargin(0.05)
      pad1.SetTopMargin(0.122)
      pad1.SetBottomMargin(0.026)
      pad1.SetFrameFillStyle(0)
      pad1.SetFrameLineStyle(0)
      pad1.SetFrameLineWidth(3)
      pad1.SetFrameBorderMode(0)
      pad1.SetFrameBorderSize(10)
      pad1.SetLogy() ;# pad1.SetLogx()
      
      max_t = 5.*max(stackDict['s'].GetMaximum(), histDict['data_'].GetMaximum())
#      stackDict['s'].SetMaximum(999); #
 #     stackDict['s'].SetMinimum(0.00001)
      stackDict['s'].Draw('hist H')
      stackDict['s'].GetYaxis().SetTitleSize(24);
      stackDict['s'].GetYaxis().SetTitleFont(45);
      stackDict['s'].SetTitle(';;Events')
      stackDict['s'].SetMaximum(max_t)
      stackDict['s'].GetYaxis().SetTitleOffset(1.2)
      stackDict['s'].GetXaxis().SetLabelSize(100)

      if l[var] == 'InvariantMass_of_4_particle_with_totCharge==0_in_tau1iso_tau2iso' or l[var] == 'InvariantMass_of_taumu_with_totCharge==0_in_tau1iso_tau2iso' :
          signalfile = r.TFile('officialHH_gluglu_controlPlot.root')
          hist = signalfile.Get(l[var])
#          hist.SetLineStyle(11)                                                                                                       
          hist.SetLineWidth(3)                                                                                                        
          hist.SetLineColor(4)                                                                                                        
          hist.SetMarkerColor(4) 
          print 'Total # of events in HH = ', hist.Integral()
#          hist.Scale(10)
          hist.Draw('same')
          legendDict['T'].AddEntry(hist, 'Signal', 'l')
      histDict['bkg_'].Draw('E2 same')
      histDict['data_'].Draw('same PE')
      histDict['bkg_'].Draw('E2 same')
      legendDict['T'].Draw('same')
      l1=add_lumi()
      l1.Draw("same")
      l2=add_CMS()
      l2.Draw("same")
      c.cd()

      pad2 = r.TPad("pad2","pad2",0,0,1,0.35);
      pad2.SetTopMargin(0.1);
      pad2.SetBottomMargin(0.4);
      pad2.SetLeftMargin(0.18);
      pad2.SetRightMargin(0.05);
      pad2.SetTickx(1)
      pad2.SetTicky(1)
      pad2.SetFrameLineWidth(3)
      #pad2.SetGridx()
      pad2.SetGridy()
      pad2.Draw()
      pad2.cd()
#      pad2.SetLogx()
      
      h3 = histDict['data_'].Clone("h3")
      h3.SetLineColor(r.kBlack)
      h3.SetMinimum(0.5)
      h3.SetMaximum(1.5)
      h3.Sumw2()
      h3.SetStats(0)
      hbkgzeroerror  = histDict['bkg_'].Clone("h2zeroerror")
      hbkg1  = histDict['bkg_'].Clone("h2")
      hbkg2 = hbkg1.Clone('h22')
      hbkg1.Divide(hbkg2)
      for ibin in range(hbkgzeroerror.GetNbinsX()) :
          hbkgzeroerror.SetBinError(ibin+1,0)
      h3.Divide(hbkgzeroerror)
      hbkg1.SetMarkerStyle(21)
#      hbkg1.GetXaxis().SetTitle(l[var])
      hbkg1.GetXaxis().SetTitleSize(5);
      hbkg1.GetXaxis().SetTitleFont(23);
      hbkg1.GetYaxis().SetTitle("Data/MC ");
      hbkg1.GetYaxis().SetNdivisions(505);
      hbkg1.GetYaxis().SetTitleSize(20);
      hbkg1.GetYaxis().SetTitleFont(23);
      hbkg1.GetYaxis().SetTitleOffset(1.55);
      hbkg1.GetYaxis().SetLabelFont(43)
      hbkg1.GetYaxis().SetLabelSize(18);
      hbkg1.GetYaxis().SetRangeUser(0.5,1.5);
      
      hbkg1.GetXaxis().SetTitleSize(20);
      hbkg1.GetXaxis().SetTitleFont(43);
      hbkg1.GetXaxis().SetTitleOffset(2.);
      #hbkg1.GetXaxis().SetLabelFont(10); 
      hbkg1.GetXaxis().SetLabelSize(.08)
      #aaaaahbkg1.GetXaxis().SetLabelFont(40);
      #aaaaaaaaaaaaaaahbkg1.GetXaxis().SetLabelSize(.08)
      hbkg1.GetXaxis().SetTitle(title[i])
      hbkg1.Draw('e2')
      h3.Draw("Epsame")


      pdf += l[var]
      c.SaveAs('%s.png' %pdf)
      c.SaveAs('%s.pdf' %pdf)
      f.Write()
      f.Close()



dirName = '.'

fileList =  [('data_', 'Data_ME_controlPlotMuElectron_test.root'),
            ('ZZTo4L', '%s/zzTo4L_ME_controlPlotMuElectron_test.root' %dirName),
            ('WW', '%s/WW_ME_controlPlotMuElectron_test.root' %dirName),
            ('t#bar{t}+jets', '%s/TT_ME_controlPlotMuElectron_test.root' %dirName),
            ('WJets', '%s/WJets_controlPlotMuElectrontest.root' %dirName),                
            ('DY', '%s/DY_controlPlotMuElectrontest.root' %dirName),
            ('WZ', '%s/WZ_ME_controlPlotMuElectron_test.root' %dirName),
            ('ZZ', '%s/ZZ_ME_controlPlotMuElectron_test.root' %dirName),
            ('ZH', '%s/ZH_ME_controlPlotMuElectron_test.root' %dirName),
            ('Single Top', '%s/ST_controlPlotMuElectrontest.root' %dirName)]

xs_calculator(fileList = fileList)




