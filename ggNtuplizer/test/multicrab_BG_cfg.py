from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea = 'MC_BG_10May'
config.section_('JobType')
config.JobType.psetName = 'run_mc_80X.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['ggtree_mc.root']
config.JobType.inputFiles = ['Summer16_23Sep2016AllV4_DATA.db','Summer16_23Sep2016V4_MC.db']
config.section_('Data')
config.Data.unitsPerJob = 2
#config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.outLFNDirBase = '/store/user/snandan/Moriond17/MC/DYW10May'
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
    config.General.workArea = 'DYW10May'

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



config.General.requestName = "DYJetsToLL_M-50_ext1-v2"
config.Data.inputDataset = "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/MINIAODSIM"
submit(config)

config.General.requestName = "WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"
config.Data.inputDataset = "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM"
submit(config)


