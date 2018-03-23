import ROOT as r
file_ = ['Radion260_controlPlotsys.root','Radion300_controlPlotsys.root','Radion350_controlPlotsys.root','Radion400_controlPlotsys.root']
title=['260','300','350','400']
c=r.TCanvas('c','',600,800)
hist=[]
color=[2,4,6,8]
l = r.TLegend(0.65, 0.44, 0.92, 0.84)
for i in range(len(file_)) :
    File = r.TFile(file_[i])
    h=File.Get('deltam1t1_with_same_sign_muon_with_totCharge==0_be4_mass_cut_in_tau1iso_tau2iso')
    h.SetDirectory(0)
    h.GetYaxis().SetRangeUser(0,1.)
    h.Rebin(5)
    h.SetLineColor(color[i])
    hist.append(h)

r.gStyle.SetOptStat(0)
for  i in range(len(hist)) :
    l.AddEntry(hist[i], title[i], 'l')
    if i==0 : 
        hist[i].Draw('HIST')
    else : 
        hist[i].Draw('HIST same')
l.Draw('same')
c.Print('deltam1t1low.png')
