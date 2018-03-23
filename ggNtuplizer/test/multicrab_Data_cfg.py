from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea = 'Data19'
config.section_('JobType')
config.JobType.psetName = 'run_data_80X.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['ggtree_data.root']
config.JobType.inputFiles = ['Summer16_23Sep2016AllV4_DATA.db','Summer16_23Sep2016V4_MC.db']
config.section_('Data')
config.Data.unitsPerJob = 20
config.Data.splitting = 'LumiBased'
config.Data.lumiMask = 'Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
config.Data.runRange = '271036-284044'
config.Data.outLFNDirBase = '/store/user/snandan/Data19/'
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
    config.General.workArea = 'Data19'

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



    config.General.requestName = "DoubleMuon_Run2016B-03Feb2017_ver2-v2"
    config.Data.inputDataset = "/DoubleMuon/Run2016B-03Feb2017_ver2-v2/MINIAOD"
    submit(config)


    config.General.requestName = "DoubleMuon_Run2016C-03Feb2017-v1"
    config.Data.inputDataset = "/DoubleMuon/Run2016C-03Feb2017-v1/MINIAOD"
    submit(config)

    config.General.requestName = "DoubleMuon_Run2016D-03Feb2017-v1"
    config.Data.inputDataset = "/DoubleMuon/Run2016D-03Feb2017-v1/MINIAOD"
    submit(config)


    config.General.requestName = "DoubleMuon_Run2016E-03Feb2017-v1"
    config.Data.inputDataset = "/DoubleMuon/Run2016E-03Feb2017-v1/MINIAOD"
    submit(config)


    config.General.requestName = "DoubleMuon_Run2016F-03Feb2017-v1"
    config.Data.inputDataset = "/DoubleMuon/Run2016F-03Feb2017-v1/MINIAOD"
    submit(config)


    config.General.requestName = "DoubleMuon_Run2016G-03Feb2017-v1"
    config.Data.inputDataset = "/DoubleMuon/Run2016G-03Feb2017-v1/MINIAOD"
    submit(config)


    config.General.requestName = "DoubleMuon_Run2016H-03Feb2017_ver2-v1"
    config.Data.inputDataset = "/DoubleMuon/Run2016H-03Feb2017_ver2-v1/MINIAOD"
    submit(config)


    config.General.requestName = "DoubleMuon_Run2016H-03Feb2017_ver3-v1"
    config.Data.inputDataset = "/DoubleMuon/Run2016H-03Feb2017_ver3-v1/MINIAOD"
    submit(config)

