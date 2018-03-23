# Code written by:  zaixing.mao@cern.ch && edward.laird@cern.ch from Brown U.                                                                  
#!/usr/bin/env python                                                                                                                          
import ROOT as r
import numpy
import math
from sys import argv, exit, stdout, stderr
import math

histDict = {}
#variable =['InvariantMass_of_taumu_with_totCharge!=0','InvariantMass_of_4_particle_with_totCharge!=0', InvariantMass_of_4_particle_with_opposite_sign_muon_in_tau1iso_tau2iso
 #          'InvariantMass_of_4_particle_with_totCharge==0','InvariantMass_of_taumu_with_totCharge==0']
#variable =['pt_of_4_particle_with_same_sign_muon_with_totCharge==0_be4_mass_cut','deltam1t1_with_same_sign_muon_with_totCharge==0_be4_mass_cut','deltam1t2_with_same_sign_muon_with_totCharge==0_be4_mass_cut','deltat1t2_with_same_sign_muon_with_totCharge==0_be4_mass_cut','deltam1m2_with_same_sign_muon_with_totCharge==0_be4_mass_cut','deltam2t1_with_same_sign_muon_with_totCharge==0_be4_mass_cut','deltam2t2_with_same_sign_muon_with_totCharge==0_be4_mass_cut']
variable = ['InvariantMass_of_4_particle_with_opposite_sign_part_with_totCharge==0_be4_mass_cut']
#variable = ['InvariantMass_of_taumu']
f = r.TFile("QCD_bkg.root","recreate")
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

def stat(hist) :

    error =0
    for ibin in range(hist.GetNbinsX()) :
        error += hist.GetBinContent(ibin+1)**2
        if hist.GetNbinsX() ==1 : error = hist.GetBinContent(ibin+1)
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
       histDict[name].GetXaxis().SetLabelSize(0.045)

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
    histDict[name].SetMarkerSize(0.7)
    histDict[name].SetMarkerColor(r.kBlack)
    histDict[name].GetXaxis().SetNdivisions(505)
    histDict[name].GetYaxis().SetLabelFont(42)
    histDict[name].GetYaxis().SetLabelOffset(0.01)
    histDict[name].GetYaxis().SetLabelSize(0.045)
    histDict[name].GetYaxis().SetTitleSize(0.075)
    histDict[name].GetXaxis().SetTitleSize(0.05)
    histDict[name].GetXaxis().SetTitleOffset(0.8)
    histDict[name].GetYaxis().SetTitleOffset(0.8)
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

    '''filesig=['Radion300_EE_controlPlotElectron_test.root','Radion800_EE_controlPlotElectron_test.root']#,'Radion400_ME_controlPlotMuElectron.root']#,'Radion800_ME_controlPlotMuElectron.root']#,'Radion900_controlPlotsys.root']    
    #filesig=['Radion900_controlPlot0.5.root']
    color = [2,4,6,8,12]
    histsig = []
    sigmax = 0.
    for ifile in range(len(filesig)):
           try:
               file_ = r.TFile(filesig[ifile])
               h = file_.Get(variable[i]+'_in_tau1iso_tau2iso')
               assert(h), 'histogram for %s not found in %s' %(variable,filesig[ifile])
               h.SetDirectory(0)
               if sigmax < h.GetMaximum() : sigmax = h.GetMaximum()
               h.SetLineColor(color[ifile])
               h.SetMarkerColor(color[ifile])
               histsig.append(h)
           except IOError:
               print 'file %s not found!' % filesig[ifile]
               continue'''

    l = [variable[i] +'_in_tau1iso_tau2iso',variable[i] +'_in_tau1anti_iso_nofake_weight',
         variable[i] +'_in_tau2anti_iso_nofake_weight',variable[i] +'_in_tau1antiso_tau2antiiso_nofake_weight',
         variable[i] +'_in_tau1anti_iso',variable[i] +'_in_tau2anti_iso',
         variable[i] +'_in_tau1antiso_tau2antiiso']

    for var in range(len(l)) :
      nbins = -999
      pdf = ''
      for iFileName, iFileLocation in fileList:
         if pdf =='' :
             if iFileLocation.find('Muon') !=-1 : pdf = 'Muon'
             if iFileLocation.find('Electron') !=-1 : pdf ='Electron'
             if iFileLocation.find('MuElectron') !=-1 : pdf ='MuElectron'
         ifile_ = r.TFile(iFileLocation)
         if ifile_.Get(l[var]) :
            hist = ifile_.Get(l[var])
            nbins = hist.GetNbinsX()
            x_low = hist.GetBinLowEdge(1)
            x_high = hist.GetBinWidth(hist.GetNbinsX())+hist.GetBinLowEdge(hist.GetNbinsX())
            histDict = buildHistDict(int(nbins),x_low,x_high)
            break
      if nbins == -999 : 
          print 'Histogram does not exist'
          continue
      hist1_data = r.TH1F('tau1_data', '', nbins, float(x_low),float(x_high))
      hist1_data.Sumw2()
      hist2_data = r.TH1F('tau2_data', '', nbins, float(x_low),float(x_high))
      hist2_data.Sumw2()
      hist12_data = r.TH1F('tau12_data', '', nbins, float(x_low),float(x_high))
      hist12_data.Sumw2()
      hist12_datanoweight = r.TH1F('tau12_data_noweight', '', nbins, float(x_low),float(x_high))
      hist12_datanoweight.Sumw2()
      hist1_Mc = r.TH1F('tau1_Mc', '', nbins, float(x_low),float(x_high))
      hist1_Mc.Sumw2()
      hist2_Mc = r.TH1F('tau2_Mc', '', nbins, float(x_low),float(x_high))
      hist2_Mc.Sumw2()
      hist12_Mc = r.TH1F('tau12_Mc', '', nbins, float(x_low),float(x_high))
      hist12_Mc.Sumw2()
      hist12_Mcnoweight = r.TH1F('tau12_Mc_noweight', '', nbins, float(x_low),float(x_high))
      hist12_Mcnoweight.Sumw2()
      histQCDreal = r.TH1F('QCD', 'true QCD', nbins, float(x_low),float(x_high))
      histQCDreal.Sumw2()
      histdatamcsubtract = r.TH1F('datamcsubtracted', 'Data after subyracting MC', nbins, float(x_low),float(x_high))

      c = r.TCanvas("c","Test", 800, 800)

   #loop over all the samples                                                                                                                 
      for iFileName, iFileLocation in fileList:

         ifile = r.TFile(iFileLocation)

         hist = ifile.Get(l[var])
         print l[var]
         if l[var] == variable[i] +'_in_tau1iso_tau2iso': ###### we need to calculate the fake tau which is #of events with 1st tau antiisolated + # of events with 2nd tau antiisolated - # of events both tau antiisolated
             hist1antiiso = ifile.Get(variable[i] +'_in_tau1anti_iso')
             hist2antiiso = ifile.Get(variable[i] +'_in_tau2anti_iso')
             histantiiso = ifile.Get(variable[i] +'_in_tau1antiso_tau2antiiso')
             hist12antiisonoweight = ifile.Get(variable[i] +'_in_tau1antiso_tau2antiiso_nofake_weight')

             if iFileName.find('data') !=-1: 
                 if hist1antiiso :  
                     hist1_data.Add(hist1antiiso,1)
                 if hist2antiiso : 
                     hist2_data.Add(hist2antiiso,1)
                 if histantiiso :  
                     hist12_data.Add(histantiiso,1)
                     hist12_datanoweight.Add(hist12antiisonoweight)
             else: ### need to sum all the MC samples
                 if hist1antiiso : 
                     hist1_Mc.Add(hist1antiiso,1)
                 if hist2antiiso : 
                     hist2_Mc.Add(hist2antiiso,1)
                 if histantiiso :  
                     hist12_Mc.Add(histantiiso,1)
                     hist12_Mcnoweight.Add(hist12antiisonoweight)

         if hist :
            statistical_error = stat(hist)
            print ifile,'\t', hist.Integral(), 'statistical Error= ', statistical_error
            FillHisto(hist, histDict[iFileName])
      if l[var] == variable[i] +'_in_tau1iso_tau2iso' :
          hist1_data.Add(hist1_Mc,-1) ##### subtract MC from data in 3 different histograms
          hist2_data.Add(hist2_Mc,-1)
          hist12_data.Add(hist12_Mc,-1)
          hist1_data.Add(hist2_data)
          hist1_data.Add(hist12_data,-1)
          hist12_datanoweight.Add(hist12_Mcnoweight,-1)
          histdatamcsubtract.Add(hist12_datanoweight,1)

          for ibin in range(hist1_data.GetNbinsX()) :
              if hist1_data.GetBinContent(ibin+1) < 0 : ##### make _ve contribution equal to zero
                  hist1_data.SetBinContent(ibin+1,0)
          
          histQCDreal.Add(hist1_data,1)
          hist12_datanoweight.Scale(hist1_data.Integral()/hist12_datanoweight.Integral())
          histDict['QCD multijet'].Add(hist12_datanoweight,1)
          #histDict['QCD multijet'].Add(hist1_data,1)    #### 
          statistical_error = stat(histDict['QCD multijet'])
          print 'total # of events in QCD = ' , histDict['QCD multijet'].Integral(), 'statistical Error= ', statistical_error

######don't need to go further  below #########
      if nbins == -999 : continue
      '''      leftbin =[1]
      rightbin=[7,8,9]
      for key in histDict :
          for ibin in range(len(leftbin)) :
              histDict[key].SetBinContent(leftbin[ibin],0)
          for ibin in range(len(rightbin)) :
              histDict[key].SetBinContent(rightbin[ibin],0)'''
      stackDict = buildStackDict(histDict)
      legendDict = buildLegendDict(histDict, (0.73, 0.44, 0.99, 0.84))

      pad1 = r.TPad("pad1","pad1",0,0.30,1,1)
      pad1.Draw()
      pad1.cd()
      #pad1.SetLogy()
      pad1.SetFillColor(0)
      pad1.SetTicky(1)
      pad1.SetLeftMargin(0.10)
      pad1.SetRightMargin(0.05)
      pad1.SetTopMargin(0.122)
      pad1.SetBottomMargin(0.08)
      pad1.SetFrameFillStyle(0)
      pad1.SetFrameLineStyle(0)
      pad1.SetFrameLineWidth(3)
      pad1.SetFrameBorderMode(0)
      pad1.SetFrameBorderSize(1)

      max_t = 1.2*max(stackDict['s'].GetMaximum(), histDict['data_'].GetMaximum()) ##wjets 2.,1.2*, sig 5.*
      stackDict['s'].Draw('hist H')
      stackDict['s'].GetYaxis().SetTitleSize(28);
      stackDict['s'].GetYaxis().SetTitleFont(23);
      stackDict['s'].GetYaxis().SetTitleOffset(1.55);
      stackDict['s'].GetYaxis().SetLabelSize(.04);
      stackDict['s'].SetTitle(';m_{#mue#tau#tau} [GeV];Events')
      stackDict['s'].SetMaximum(max_t)
      stackDict['s'].SetMinimum(0.001)
      stackDict['s'].GetXaxis().SetLabelSize(.04)
      stackDict['s'].GetXaxis().SetTitleOffset(.94)
      #stackDict['s'].GetXaxis().SetLabelSize(100)###################################################################
      if l[var] == variable[i]+'_in_tau1iso_tau2iso' or l[var] == 'InvariantMass_of_taumu_with_same_sign_muon_with_totCharge==0_in_tau1iso_tau2iso' :
          title = ['Radion300 1pb','Radion800 1pb']#,'Radion400 1pb']#,'Radion800 1pb']#,'Radion900']
          #title =['Radion900 1pb']
          '''for sig in range(len(histsig)) :
              statistical_error = stat(histsig[sig])
              print title[sig], '\t' , histsig[sig].Integral(), 'statistical Error= ', statistical_error
              histsig[sig].Draw('HIST same')
              legendDict['T'].AddEntry(histsig[sig], title[sig], 'l')'''


      '''for ibin in range(len(leftbin)) :
          histsig[0].SetBinContent(leftbin[ibin],0)
      for ibin in range(len(rightbin)) :
          histsig[0].SetBinContent(rightbin[ibin],0)'''

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
      pad2.SetLeftMargin(0.10);
      pad2.SetRightMargin(0.05);
      pad2.SetTickx(1)
      pad2.SetTicky(1)
      pad2.SetFrameLineWidth(3)
      #pad2.SetGridx()
      pad2.SetGridy()
      pad2.Draw()
      pad2.cd()
      
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
      hbkg1.GetXaxis().SetTitleFont(23);
      hbkg1.GetYaxis().SetTitle('Data/MC');
      hbkg1.GetYaxis().SetNdivisions(505);
      hbkg1.GetYaxis().SetTitleSize(28);
      hbkg1.GetYaxis().SetTitleFont(23);
      hbkg1.GetYaxis().SetTitleOffset(1.55);
      hbkg1.GetYaxis().SetLabelFont(40)
      hbkg1.GetYaxis().SetLabelSize(.10);
      hbkg1.GetYaxis().SetRangeUser(0.5,1.5);
      
      hbkg1.GetXaxis().SetTitleSize(30);
      hbkg1.GetXaxis().SetTitleFont(48);
      hbkg1.GetXaxis().SetTitleOffset(3.);
      hbkg1.GetXaxis().SetLabelFont(40);
      hbkg1.GetXaxis().SetLabelSize(.10)
      hbkg1.GetXaxis().SetTitle('m_{e#tau} [GeV]')
      hbkg1.Draw('e2')
      h3.Draw("Epsame")


      pdf += l[var]
      c.SaveAs('%s.png' %pdf)
      c.SaveAs('%s.pdf' %pdf)      
      c.SaveAs('%s.root' %pdf)

      if l[var] == 'InvariantMass_of_4_particle_with_same_sign_part_with_totCharge==0_be4_mass_cut_in_tau1iso_tau2iso' or l[var] == 'InvariantMass_of_taumu_in_tau1iso_tau2iso' :
          file_ = r.TFile(fileList[0][1])
          hist1 = file_.Get(variable[i]+'_in_tau1anti_iso_nofake_weight')
          hist2 = file_.Get(variable[i]+'_in_tau2anti_iso_nofake_weight')
          hist12 = file_.Get(variable[i]+'_in_tau1antiso_tau2antiiso_nofake_weight')
          histqcd = histDict['QCD multijet']
          nqcd = histqcd.Integral()
          f.cd()
          histqcd.Write()
          hist1.Write()
          hist2.Write()
          hist12.Write()
          hist12_datanoweight.Write()
          hist12.Scale(nqcd/hist12.Integral())
          histqcdshape = hist12.Clone('QCDshapenosubtract')
          hist12_datanoweight.Scale(nqcd/hist12_datanoweight.Integral())
          histqcdshapesubtract = hist12_datanoweight.Clone('QCDshapesubtract')
          histQCDreal.Write()
          histdatamcsubtract.Write()
          print histqcdshape.Integral(),'\t',histqcd.Integral(),'\t',histqcdshapesubtract.Integral()
                    
   f.Write()
   f.Close()



dirName = '.'

fileList =  [('data_', 'Data_EE_controlPlotElectronLooseEle.root'),
            ('ZZTo4L', '%s/zzTo4L_EE_controlPlotElectronLooseEleRejection.root' %dirName),
            ('WW', '%s/WW_EE_controlPlotElectronLooseEleRejection.root' %dirName),
            ('t#bar{t}+jets', '%s/TT_EE_controlPlotElectronLooseEleRejection.root' %dirName),
            ('WJets', '%s/WJets_EE_controlPlotElectronLooseEleRejection.root' %dirName),                
            ('DY', '%s/DY_EE_controlPlotElectronLooseEleRejection.root' %dirName),
            ('WZ', '%s/WZ_EE_controlPlotElectronLooseEleRejection.root' %dirName),
            ('ZZ', '%s/ZZ_EE_controlPlotElectronLooseEleRejection.root' %dirName),
            ('ZH', '%s/ZH_EE_controlPlotElectronLooseEleRejection.root' %dirName),
            ('Single Top', '%s/ST_EE_controlPlotElectronLooseEleRejection.root' %dirName)]

xs_calculator(fileList = fileList)


