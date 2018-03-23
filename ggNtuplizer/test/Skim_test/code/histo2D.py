import ROOT as r
file=['zzTo4L_controlPlot.root','TT_controlPlot.root']
var='2D_pt_distribution_of_Leading_#tau_SubLeading_#tau_with_totCharge!=0_in_tau1anti_iso_nofake_weight'
hist1_Mc = r.TH2F('tau1_Mc', '', 50,0,150,50,0,150)
for i in range(2) :
 f=r.TFile(file[i])
 hist = f.Get(var)
 for ibin in range(hist.GetNbinsX()) :                   
      print ibin+1, '\t',hist.ProjectionX().GetBinContent(ibin+1)

 hist1_Mc.Add(hist)

for ibin in range(hist1_Mc.GetNbinsX()) :
    print ibin+1,'\t',hist1_Mc.ProjectionX().GetBinContent(ibin+1)

