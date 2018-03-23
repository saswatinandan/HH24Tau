import math
import ROOT as r
fileList =  ['zzTo4L_controlPlot.root','zzTo4L_controlPlot.root','ZH_controlPlot.root']


hist = 'InvariantMass_of_4_particle_with_totCharge==0_in_tau1iso_tau2iso'
sys = ['_lumi','_mu','_tau']
histvar=''
for ifile in range(len(fileList)) :
    for ivar in range(0,2) :
        for isys in range(len(sys)) :
            var = ''
            if ivar ==0 : var = 'Up'
            else : var = 'Dn'
            error =[]
            histvar = hist+sys[isys]+var
            file_ = r.TFile(fileList[ifile])
            histo = file_.Get(histvar)
            nominal = file_.Get(hist)
            cont = 0
            for ibin in range(histo.GetNbinsX()) :
                if nominal.GetBinContent(ibin+1) !=0 :
                    cont += (1-histo.GetBinContent(ibin+1)/nominal.GetBinContent(ibin+1))**2
            error.append(cont)
        toterror = math.sqrt(sum(error_ for error_ in error))
        print 'error for ', histvar, '\t' , 'in ' , ifile, toterror
