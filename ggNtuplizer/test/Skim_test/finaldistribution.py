import ROOT as r

h=[]
color = [2,4,6,8]
binlow= [1,1,1,1]
binhigh = [1,2,2,4]
file_ = ['Radion260_controlPlot0.5.root','Radion300_controlPlot0.5.root','Radion350_controlPlot0.5.root','Radion400_controlPlot0.5.root']
c=r.TCanvas('','',600,800)
variable = 'InvariantMass_of_4_particle_with_same_sign_muon_with_totCharge==0_be4_mass_cut_in_tau1iso_tau2iso'
for i in range(len(file_)) :
    ifile = r.TFile(file_[i])
    hist = ifile.Get(variable)
    print hist
    hist.SetDirectory(0)
    hist.SetLineColor(color[i])
    hist.SetMarkerColor(color[i])
    hist.GetXaxis().SetRangeUser(binlow[i],binhigh[i])
    if i == 0: hist.Draw('HIST')
    else : hist.Draw('same HIST')
c.Print('final.png')
    
        

