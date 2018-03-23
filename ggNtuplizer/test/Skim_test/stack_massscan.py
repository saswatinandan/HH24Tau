# Code written by:  zaixing.mao@cern.ch && edward.laird@cern.ch from Brown U.                                                                  
#!/usr/bin/env python                                                                                                                          
import ROOT as r
import numpy
import math
from array import array
from sys import argv, exit, stdout, stderr
import math

histDict = {}


#variable =['InvariantMass_of_taumu_with_totCharge!=0','InvariantMass_of_4_particle_with_totCharge!=0', InvariantMass_of_4_particle_with_opposite_sign_muon_in_tau1iso_tau2iso
 #          'InvariantMass_of_4_particle_with_totCharge==0','InvariantMass_of_taumu_with_totCharge==0']
variable =['InvariantMass_of_4_particle_with_same_sign_part_with_totCharge==0_be4_mass_cut']
f = r.TFile("massscan.root","recreate")
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

def stat(hist,binlow,binhigh) :

    error =0
    for ibin in range(binlow,binhigh+1) :
        error += hist.GetBinContent(ibin)**2
        if binlow == binhigh : error = hist.GetBinContent(ibin)
    return math.sqrt(error)

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

def chargemisid(fileList =[]) :

    h_chargemisid = r.TH1F('','',nbins,x_low,x_high)
    return h_chargemisid

def xs_calculator(fileList = []):

   r.gStyle.SetFrameLineWidth(3)
   r.gStyle.SetLineWidth(3)
   r.gStyle.SetOptStat(0)

   histsig=[]
   for i in range(len(variable)) :

       filesig = ['Radion250_ME_controlPlotMuElectron0.3.root','Radion260_ME_controlPlotMuElectron0.3.root','Radion270_ME_controlPlotMuElectron0.3.root','Radion280_ME_controlPlotMuElectron0.3.root','Radion300_ME_controlPlotMuElectron0.3.root','Radion320_ME_controlPlotMuElectron0.3.root','Radion340_ME_controlPlotMuElectron0.3.root','Radion350_ME_controlPlotMuElectron0.3.root','Radion400_ME_controlPlotMuElectron0.3.root','Radion450_ME_controlPlotMuElectron0.3.root','Radion500_ME_controlPlotMuElectron0.3.root','Radion550_ME_controlPlotMuElectron0.3.root','Radion600_ME_controlPlotMuElectron0.3.root','Radion650_ME_controlPlotMuElectron0.3.root','Radion700_ME_controlPlotMuElectron0.3.root','Radion750_ME_controlPlotMuElectron0.3.root','Radion800_ME_controlPlotMuElectron0.3.root','Radion900_ME_controlPlotMuElectron0.3.root'] 
       for ifile in range(0,len(filesig)):
           try:
               file_ = r.TFile(filesig[ifile])
               h = file_.Get(variable[i]+'_in_tau1iso_tau2iso')
               assert(h), 'histogram for %s not found in %s' %(variable,filesig[ifile])
               h.SetDirectory(0)
               histsig.append(h)
           except IOError:
               print 'file %s not found!' % filesig[ifile]
               continue
       


   for iFileName, iFileLocation in fileList:
       ifile_ = r.TFile(iFileLocation)
       if ifile_.Get(variable[i]+'_in_tau1iso_tau2iso') :
           hist = ifile_.Get(variable[i]+'_in_tau1iso_tau2iso')
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

   #loop over all the samples                                                                                                                 
   for iFileName, iFileLocation in fileList:

       ifile = r.TFile(iFileLocation)
       
       hist = ifile.Get(variable[i]+'_in_tau1iso_tau2iso')
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
           if hist1antiiso : 
               hist1_Mc.Add(hist1antiiso,1)
           if hist2antiiso : 
               hist2_Mc.Add(hist2antiiso,1)
           if histantiiso :  
               hist12_Mc.Add(histantiiso,1)
           
       if hist :
           FillHisto(hist, histDict[iFileName])
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

   stackDict = buildStackDict(histDict)

   binlow = [7,7,7]
   binhigh = [9,8,7]
   binlabel = ['700-1000','700-900','700-800']
   histname = ['250','260','270','280','300','320','340','350','400','450','500','550','600','650','700','750','800','900']

   for isig in range(len(histsig)) :
       hmassscan = r.TH1F(histname[isig],'',len(binlow),0,len(binlow))
       for ibin in range(len(binlow)) :
           hmassscan.GetXaxis().SetBinLabel(ibin+1,binlabel[ibin])
       for ibin in range(len(binlow)) :
           sigcont = histsig[isig].Integral(binlow[ibin],binhigh[ibin])
           bkgcont = histDict['bkg_'].Integral(binlow[ibin],binhigh[ibin]) 
           sigerror = stat(histsig[isig],binlow[ibin],binhigh[ibin])
           bkgerror = stat(histDict['bkg_'],binlow[ibin],binhigh[ibin])
           if bkgcont ==0 : continue
           sigbkg = math.sqrt(2*((sigcont+bkgcont)*math.log(1+(1.0*sigcont/bkgcont))-sigcont))
           if sigbkg ==0 : continue
           error = math.sqrt(((math.log(1+(sigcont/bkgcont))**2 * sigerror**2) + ((math.log(1+(sigcont/bkgcont))-sigcont/bkgcont)**2 * bkgerror**2))*(1./sigbkg**2))
           print histname[isig],'\t',ibin,'\t','sig= ', sigcont,'\t','bkg= ',bkgcont,'\t',sigbkg
           hmassscan.SetBinContent(ibin+1,sigbkg)
           hmassscan.SetBinError(ibin+1,error)
       f.cd()
       hmassscan.Write()
   f.Close()



dirName = '.'

fileList =  [('data_', 'Data_ME_controlPlotMuElectron0.3.root'),
            ('ZZTo4L', '%s/zzTo4L_ME_controlPlotMuElectron0.3.root' %dirName),
            ('WW', '%s/WW_ME_controlPlotMuElectron0.3.root' %dirName),
            ('t#bar{t}+jets', '%s/TT_ME_controlPlotMuElectron0.3.root' %dirName),
            ('WJets', '%s/WJets_controlPlotMuElectron0.3.root' %dirName),                
            ('DY', '%s/DY_controlPlotMuElectron0.3.root' %dirName),
            ('WZ', '%s/WZ_ME_controlPlotMuElectron0.3.root' %dirName),
            ('ZZ', '%s/ZZ_ME_controlPlotMuElectron0.3.root' %dirName),
            ('ZH', '%s/ZH_ME_controlPlotMuElectron0.3.root' %dirName),
            ('Single Top', '%s/ST_controlPlotMuElectron0.3.root' %dirName)]

xs_calculator(fileList = fileList)



