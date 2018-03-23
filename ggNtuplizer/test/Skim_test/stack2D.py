# Code written by:  zaixing.mao@cern.ch && edward.laird@cern.ch from Brown U.                                                                  
#!/usr/bin/env python                                                                                                                          
import ROOT as r
import numpy
from sys import argv, exit, stdout, stderr
import math

histDict = {}

#variable =['InvariantMass_of_taumu_with_totCharge!=0','InvariantMass_of_4_particle_with_totCharge!=0',
 #          'InvariantMass_of_4_particle_with_totCharge==0','InvariantMass_of_taumu_with_totCharge==0']
variable =['2D_pt_distribution_of_Leading_#tau_SubLeading_#tau_with_totCharge!=0']
f = r.TFile("prefit.root","recreate")
################################################                                                    

def fakeweight(pt)  :
    return .0194+.232*r.TMath.Exp(-.0653*pt)

# Sevreal Histograms are initiated/produced here

defaultOrder = [('WJets',  r.TColor.GetColor(100,182,232)),
                ('t#bar{t}+jets', r.TColor.GetColor(155,152,204)),
                ('DY', r.TColor.GetColor(250,202,255)),
                ('WW', r.TColor.GetColor(248,206,104)),
                ('ZZ', r.TColor.GetColor(200,225,60)),
                ('ZZTo4L', r.TColor.GetColor(30,50,120)),
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
    histDict[name].GetYaxis().SetLabelSize(0.06)
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
    lumi.AddText("36 fb^{-1} (13 TeV)")
    return lumi

def add_CMS():
    lowX=0.21
    lowY=0.70
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

    l = [variable[i] +'_in_tau1iso_tau2iso_nofake_weight']
 
    for var in range(len(l)) :
      for iFileName, iFileLocation in fileList:
         ifile_ = r.TFile(iFileLocation)
         if ifile_.Get(l[var]) :
            hist = ifile_.Get(l[var])
            nbins = hist.GetNbinsX()
            x_low = hist.GetXaxis().GetBinLowEdge(1)
            x_high = hist.GetXaxis().GetBinWidth(hist.GetNbinsX())+hist.GetXaxis().GetBinLowEdge(hist.GetNbinsX())
            histDict = buildHistDict(int(nbins),x_low,x_high)
            break

         


      hist1_data = r.TH2F('tau1_data', '', nbins, int(x_low),int(x_high), nbins, int(x_low),int(x_high))
      hist1_data.Sumw2()
      hist2_data = r.TH2F('tau2_data', '', nbins, int(x_low),int(x_high), nbins, int(x_low),int(x_high))
      hist2_data.Sumw2()
      hist12_data = r.TH2F('tau12_data', '', nbins, int(x_low),int(x_high),nbins, int(x_low),int(x_high))
      hist12_data.Sumw2()
      hist1_Mc = r.TH2F('tau1_Mc', '', 50,0,150,50,0,150)
      hist1_Mc.Sumw2()
      hist2_Mc = r.TH2F('tau2_Mc', '', nbins, int(x_low),int(x_high), nbins, int(x_low),int(x_high))
      hist2_Mc.Sumw2()
      hist12_Mc = r.TH2F('tau12_Mc', '', nbins, int(x_low),int(x_high),nbins, int(x_low),int(x_high))
      hist12_Mc.Sumw2()
      xbin = numpy.array([40,50,60,70,80,90,100,110,120,200])
      c = r.TCanvas("c","Test", 600, 600)

   #loop over all the samples                                                                       
      sum_ = 0
      for iFileName, iFileLocation in fileList:

         ifile = r.TFile(iFileLocation)

         hist = ifile.Get(l[var])
         if l[var] == variable[i] +'_in_tau1iso_tau2iso_nofake_weight': ###### we need to calculate the fake tau which is #of events with 1st tau antiisolated + # of events with 2nd tau antiisolated - # of events both tau antiisolated
             hist1antiiso = ifile.Get(variable[i] +'_in_tau1anti_iso_nofake_weight')
    #         if hist1antiiso : print variable[i] +'_in_tau1anti_iso_nofake_weight'
             hist2antiiso = ifile.Get(variable[i] +'_in_tau2anti_iso_nofake_weight')
             histantiiso =  ifile.Get(variable[i] +'_in_tau1antiso_tau2antiiso_nofake_weight')

             if iFileName.find('data') !=-1: 
                 if hist1antiiso :  
                     hist1_data.Add(hist1antiiso,1)
                 if hist2antiiso : 
                     hist2_data.Add(hist2antiiso,1)
                 if histantiiso :  
                     hist12_data.Add(histantiiso,1)
             else: ### need to sum all the MC samples
                 if hist1antiiso : 
#                     print ifile, '\t', hist1antiiso.GetName()
 #                    for ibin in range(hist1antiiso.GetNbinsX()) :
  #                       print ibin+1, hist1antiiso.ProjectionX().GetBinContent(ibin+1)
#
 #                    for ibin in range(hist1_Mc.GetNbinsX()) :
  #                       print 'MCCCCC',ibin+1, hist1_Mc.ProjectionX().GetBinContent(ibin+1)
                     hist1_Mc.Add(hist1antiiso,1)
   #                  sum_ += hist1_Mc.ProjectionX().GetBinContent(8)
    #                 print hist1_Mc.ProjectionX().GetBinContent(8),'SUmmmmmmmmmmmmm= ',sum_
     #                for ibin in range(hist1_Mc.GetNbinsX()) :
      #                   print 'SUMMMMMM',ibin+1, hist1_Mc.ProjectionX().GetBinContent(ibin+1)

                 if hist2antiiso : 
                     hist2_Mc.Add(hist2antiiso,1)
                 if histantiiso :  
                     hist12_Mc.Add(histantiiso,1)

         if hist :
            hist = hist.ProjectionX()
            print ifile
            FillHisto(hist, histDict[iFileName])
      if l[var] == variable[i] +'_in_tau1iso_tau2iso_nofake_weight' :
#          print hist1_data.GetBinContent(1),'\t', hist2_data.GetBinContent(1),'\t',hist12_data.GetBinContent(1)
          hist1_data.Add(hist1_Mc,-1) ##### subtract MC from data in 3 different histograms
          hist2_data.Add(hist2_Mc,-1)
          hist12_data.Add(hist12_Mc,-1)

          for ibin in range(hist1_data.GetNbinsX()) :
              for jbin in range(hist1_data.GetNbinsY()) :
                  pt = hist1_data.GetYaxis().GetBinCenter(jbin+1)
                  fake = fakeweight(pt)
                  bincont = hist1_data.GetBinContent(ibin+1,jbin+1)
                  hist1_data.SetBinContent(ibin+1,jbin+1,bincont*fake/(1-fake))
                  bincont = hist2_data.GetBinContent(ibin+1,jbin+1)
                  hist2_data.SetBinContent(ibin+1,jbin+1,bincont*fake/(1-fake))
                  bincont = hist12_data.GetBinContent(ibin+1,jbin+1)
                  hist12_data.SetBinContent(ibin+1,jbin+1,bincont*fake/(1-fake))


          hist1_data = hist1_data.ProjectionX()
          hist2_data = hist2_data.ProjectionX()
          hist12_data = hist12_data.ProjectionX()
          hist1_data.Add(hist2_data) 
          hist1_data.Add(hist12_data,-1)
          for ibin in range(hist1_Mc.GetNbinsX()) :
              print ibin+1, hist1_Mc.ProjectionX().GetBinContent(ibin+1)
          hist1_Mc.ProjectionX().Draw()


      pdf = l[var]+'.gif'
      c.SaveAs('%s' %pdf)
      f.Write()
      f.Close()



dirName = '.'

fileList =  [('data_', '/eos/uscms/store/user/snandan/Data19_controlPlot.root'),
            ('ZZTo4L', '%s/zzTo4L_controlPlot.root' %dirName),
            ('WW', '%s/WW_controlPlot.root' %dirName),
            ('t#bar{t}+jets', '%s/TT_controlPlot.root' %dirName)]
 #           ('WJets', '%s/WJets_controlPlot.root' %dirName),                
#            ('DY', '%s/DY_controlPlot.root' %dirName)]
   #         ('WZ', '%s/WZ_controlPlot.root' %dirName),
    #        ('ZZ', '%s/ZZ_controlPlot.root' %dirName),
     #       ('Single Top', '%s/ST_controlPlot.root' %dirName)]

xs_calculator(fileList = fileList)



