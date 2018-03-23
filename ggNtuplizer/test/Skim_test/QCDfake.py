import ROOT as r
file_ = ['QCDFakein_QCD_15to20.root','QCDFakein_QCD_20to30.root','QCDFakein_QCD_30to50.root','QCDFakein_QCD_50to80.root','QCDFakein_QCD_80to120.root','QCDFakein_QCD_120to170.root',
         'QCDFakein_QCD_170to300.root','QCDFakein_QCD_300to470.root','QCDFakein_QCD_470to600.root','QCDFakein_QCD_600to800.root','QCDFakein_QCD_800to1000.root','QCDFakein_QCD_1000toinf.root']
deno = r.TH1F('deno','',14,10,150)
neo= r.TH1F('neo','',14,10,150)
c=r.TCanvas('c','',600,800)
for i in range(len(file_)) :
    f = r.TFile(file_[i])
    deno.Add(f.Get('deno'))
    neo.Add(f.Get('neo'))

neo.Divide(deno)
neo.Draw()
c.Print('QCDfake.png')
c.Print('QCDfake.pdf')
