from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea = 'MC_QCD'
config.section_('JobType')
config.JobType.psetName = 'run_mc_80X.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['ggtree_mc.root']
config.JobType.inputFiles = ['Summer16_23Sep2016AllV4_DATA.db','Summer16_23Sep2016V4_MC.db']
config.section_('Data')
config.Data.unitsPerJob = 2
#config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.outLFNDirBase = '/store/user/snandan/Moriond17/MC/QCD'
config.Data.allowNonValidInputDataset = True
config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T3_US_FNALLPC'



if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from CRABClient.ClientExceptions import ClientException
    from httplib import HTTPException

    # We want to put all the CRAB project directories from the tasks we submit here into one common directory.
    # That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).
    config.General.workArea = 'QCD'

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException as hte:
            print "Failed submitting task: %s" % (hte.headers)
        except ClientException as cle:
            print "Failed submitting task: %s" % (cle)

    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################



config.General.requestName = "QCD_Pt-1000toInf_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"
config.Data.inputDataset = "/QCD_Pt-1000toInf_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "QCD_Pt-120to170_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"
config.Data.inputDataset = "/QCD_Pt-120to170_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "QCD_Pt-15to20_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"
config.Data.inputDataset = "/QCD_Pt-15to20_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "QCD_Pt-170to300_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"
config.Data.inputDataset = "/QCD_Pt-170to300_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "QCD_Pt-20to30_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"
config.Data.inputDataset = "/QCD_Pt-20to30_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "QCD_Pt-300to470_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"
config.Data.inputDataset = "/QCD_Pt-300to470_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "QCD_Pt-30to50_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"
config.Data.inputDataset = "/QCD_Pt-30to50_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "QCD_Pt-470to600_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"
config.Data.inputDataset = "/QCD_Pt-470to600_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "QCD_Pt-50to80_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"
config.Data.inputDataset = "/QCD_Pt-50to80_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "QCD_Pt-600to800_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"
config.Data.inputDataset = "/QCD_Pt-600to800_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "QCD_Pt-800to1000_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"
config.Data.inputDataset = "/QCD_Pt-800to1000_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "QCD_Pt-80to120_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"
config.Data.inputDataset = "/QCD_Pt-80to120_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)


