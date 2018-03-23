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
variable = ["BB_LL_OS","BB_ML_OS","BB_MM_OS","BB_HL_OS","BB_HM_OS","BB_HH_OS",
                  "EE_LL_OS","EE_ML_OS","EE_MM_OS","EE_HL_OS","EE_HM_OS","EE_HH_OS",
                  "BE_LL_OS","BE_ML_OS","EB_ML_OS","BE_MM_OS","BE_HL_OS","EB_HL_OS",
                  "BE_HM_OS","EB_HM_OS","BE_HH_OS",
            "BB_LL_SS","BB_ML_SS","BB_MM_SS","BB_HL_SS","BB_HM_SS","BB_HH_SS",
                  "EE_LL_SS","EE_ML_SS","EE_MM_SS","EE_HL_SS","EE_HM_SS","EE_HH_SS",
                  "BE_LL_SS","BE_ML_SS","EB_ML_SS","BE_MM_SS","BE_HL_SS","EB_HL_SS",
                  "BE_HM_SS","EB_HM_SS","BE_HH_SS",'inclusive_OS','inclusive_SS']
#variable = ['inclusive_OS']

f = r.TFile("prefit.root","recreate")
################################################                                                                                               
# Sevreal Histograms are initiated/produced here                                                                                               
'''defaultOrder = [('DY', r.TColor.GetColor(250,202,255)),
                ('WZ', r.TColor.GetColor(200,225,60)),
                ('ZZ', r.TColor.GetColor(210,70,80)),
                ('ZZTo4L', r.TColor.GetColor(30,50,120)),
                ('bkg',r.TColor.GetColor(130,130,130)),
                ('ZH', r.TColor.GetColor(230,90,115))]'''

defaultOrder = [('DY', r.TColor.GetColor(250,202,255)),         ##500,500,500;100,10,100;200,20,200;300,30,300;600,60,60;40,40,40;
                ('WZ', r.TColor.GetColor(230,225,60)),
                ('ZZ', r.TColor.GetColor(220,70,80)),
                ('ZZTo4L', r.TColor.GetColor(40,50,120)),
                ('bkg',r.TColor.GetColor(600,60,60)),
                ('ZH', r.TColor.GetColor(240,90,115))]


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

def FillHisto(input, output) :
   #print 'inFillHisto',input,'ou== ', histDict, output                                                                                                 
    for i in range(input.GetNbinsX()):
       currentValue = output.GetBinContent(i+1)
       currentError = output.GetBinError(i+1)
       output.SetBinContent(i+1, currentValue+input.GetBinContent(i+1))
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


#   ifile = r.TFile('chargemisid.root')

   for i in range(len(variable)) :

      histDict = {}
      '''for isample, sample in sampleList:
         if ifile.Get('%s/%s' %(variable[i],sample)) :
            hist = ifile.Get('%s/%s' %(variable[i],sample))
            nbins = hist.GetNbinsX()
            x_low = hist.GetBinLowEdge(1)
            x_high = hist.GetBinWidth(hist.GetNbinsX())+hist.GetBinLowEdge(hist.GetNbinsX())
            histDict = buildHistDict(int(nbins),x_low,x_high)
            print histDict
            break'''
      ifile = r.TFile('chargemisid2.root')
      if ifile.Get('%s' %variable[i]) == None : 
         print 'NNN'
         continue
      for isample, sample,  in sampleList:
#         ifile_ = r.TFile('chargemisid.root')
         if ifile.Get('%s/%s' %(variable[i],sample)) :
            hist = ifile.Get('%s/%s' %(variable[i],sample))
            nbins = hist.GetNbinsX()
            x_low = hist.GetBinLowEdge(1)
            x_high = hist.GetBinWidth(hist.GetNbinsX())+hist.GetBinLowEdge(hist.GetNbinsX())
            histDict = buildHistDict(int(nbins),x_low,x_high)
            break

      
      
   #loop over all the samples
#      ifile = r.TFile('chargemisid.root')
      
      bkg_hist = ['WZ','ZZ','ZH','TT','WJetsToLNu','zzTo4L','WW','DY1JetsToLL','DYJetsToLL','DY2JetsToLL','DY3JetsToLL','DY4JetsToLL','ST_tW_antitop_5f',
                  'ST_tW_top_5f','ST_t-channel_antitop_4f','ST_t-channel_top_4f']

      hbkg = r.TH1F('Bkg_','',nbins,x_low,x_high)

      for ibkg in range(len(bkg_hist)) :
         h = ifile.Get('%s/%s' %(variable[i],bkg_hist[ibkg]))
         if h : 
            hbkg.Add(h,1)

      for isample, sample in sampleList:

         print variable[i],isample
         if ifile.Get('%s/%s' %(variable[i],sample)) :
            FillHisto(ifile.Get('%s/%s' %(variable[i],sample)), histDict[isample])

      if len(histDict) == 0 : continue
      histDict['bkg'].Add(hbkg,1)
      stackDict = buildStackDict(histDict)
      legendDict = buildLegendDict(histDict, (0.65, 0.44, 0.92, 0.84))

      pdf = ''
      c = r.TCanvas("c","Test", 800, 600)
      pad1 = r.TPad("pad1", "pad1", 0, 0.3, 1, 1.0)
      pad1.SetLogy()
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
      h3.GetXaxis().SetTitle("M_{ee}")
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
      print pdf
      c.SaveAs('%s' %pdf)
      f.Write()
      f.Close()



dirName = '.'

sampleList = [('data_', 'Data'),
            ('DY', 'DY_Zee'),
            ('WZ', 'WZ_Zee'),
            ('ZZ', 'ZZ_Zee'),
            ('ZH', 'ZH_Zee'),
#            ('bkg', 'bkg'),
            ('ZZTo4L', 'zzTo4L_Zee')]

xs_calculator(fileList = sampleList)



