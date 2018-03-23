# Code written by:  zaixing.mao@cern.ch && edward.laird@cern.ch from Brown U.                                                                  
#!/usr/bin/env python                                                                                                                          
import ROOT as r
import numpy
from sys import argv, exit, stdout, stderr
import math

if len(argv) < 2:
   print 'Usage:python xs_calculator_prefit.py DYCrossSection[optional]'

if len(argv)>1:
   FoundXS= numpy.array([argv[1]],dtype=float)
else:
   FoundXS=1.00

histDict = {}
variable = ['deno','neo']

f = r.TFile("prefit.root","recreate")
################################################                                                                                               
# Sevreal Histograms are initiated/produced here                                                                                               
defaultOrder = [('WJets',  r.TColor.GetColor(100,182,232)),
                ('t#bar{t}+jets', r.TColor.GetColor(155,152,204)),
                ('DY', r.TColor.GetColor(250,202,255)),
                ('WW', r.TColor.GetColor(248,206,104)),
                ('WZ', r.TColor.GetColor(200,225,60)),
                ('ZZ', r.TColor.GetColor(210,70,80)),
                ('ZZTo4L', r.TColor.GetColor(30,50,120)),
                ('Single Top', r.TColor.GetColor(230,90,115))]


def buildHistDict(nbins,x_low,x_high):
    print nbins
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

   for i in range(len(variable)) :
      for iFileName, iFileLocation in fileList:
         ifile_ = r.TFile(iFileLocation)
         if ifile_.Get(variable[i]) :
            hist = ifile_.Get(variable[i])
            nbins = hist.GetNbinsX()
            x_low = hist.GetBinLowEdge(1)
            x_high = hist.GetBinWidth(hist.GetNbinsX())+hist.GetBinLowEdge(hist.GetNbinsX())
            histDict = buildHistDict(int(nbins),x_low,x_high)
            break
   #loop over all the samples                                                                                                                 
      for iFileName, iFileLocation in fileList:
         ifile = r.TFile(iFileLocation)
         weight = 1.
         tauWeight = 1.

         if ifile.Get(variable[i]) :
            print ifile
            FillHisto(ifile.Get(variable[i]), histDict[iFileName], tauWeight)
      stackDict = buildStackDict(histDict)
      legendDict = buildLegendDict(histDict, (0.65, 0.44, 0.92, 0.84))

      pdf = ''
      c = r.TCanvas("c","Test", 800, 600)
      pad1 = r.TPad("pad1", "pad1", 0, 0.3, 1, 1.0)
      pad1.SetBottomMargin(0)
      pad1.SetGridx()
      pad1.Draw()
      pad1.cd()
      max_t = 1.2*max(stackDict['s'].GetMaximum(), histDict['data_'].GetMaximum())
      stackDict['s'].Draw('hist H')
      stackDict['s'].SetTitle(';%s;Events' %(variable[i]))
      stackDict['s'].SetMaximum(max_t)
      stackDict['s'].GetYaxis().SetTitleOffset(1.2)
      histDict['data_'].Draw('same PE')
      histDict['bkg_'].Draw('E2 same')
      legendDict['T'].Draw('same')
      l1=add_lumi()
      l1.Draw("same")
      l2=add_CMS()
      l2.Draw("same")
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
      h3.GetXaxis().SetTitle("#tau pt")
      h3.GetYaxis().SetTitle("ratio data/Mc ");
      h3.GetYaxis().SetNdivisions(505);
      h3.GetYaxis().SetTitleSize(20);
      h3.GetYaxis().SetTitleFont(43);
      h3.GetYaxis().SetTitleOffset(1.55);
      h3.GetYaxis().SetLabelFont(43)
      h3.GetYaxis().SetLabelSize(15);

      h3.GetXaxis().SetTitleSize(20);
      h3.GetXaxis().SetTitleFont(46);
      h3.GetXaxis().SetTitleOffset(2.);
      h3.GetXaxis().SetLabelFont(40);
      h3.GetXaxis().SetLabelSize(.11)
      pdf += variable[i]+'DY'
      c.SaveAs('%s.png' %pdf)
      c.SaveAs('%s.pdf' %pdf)
      f.Write()
      f.Close()



dirName = '.'

fileList = [('data_', '%s/DYFakeinData19.root' %dirName),
            ('t#bar{t}+jets', '%s/DYFakeinTT.root' %dirName),
            ('WJets', '%s/DYFakein_WJetsToLNu.root' %dirName),       
            ('DY', '%s/DYFakein_DYtest.root' %dirName),
            ('WW', '%s/DYFakeinWW.root' %dirName),
            ('WZ', '%s/DYFakeinWZ.root' %dirName),
            ('ZZ', '%s/DYFakeinZZ.root' %dirName),
            ('Single Top', '%s/DYFakein_STtest.root' %dirName),
            ('ZZTo4L', '%s/DYFakeinzzTo4L.root' %dirName)]

xs_calculator(fileList = fileList)



