from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'ZZTo4L'
config.General.workArea = 'ZZTo4L'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.allowUndistributedCMSSW = True 
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/uscms_data/d3/snandan/CMSSW_8_0_26_patch1/src/ggAnalysis/ggNtuplizer/test/run_mc_80X.py'
config.JobType.outputFiles = ['ggtree_mc.root']
config.JobType.inputFiles = ['Summer16_23Sep2016V4_MC.db']

config.section_("Data")
config.Data.inputDataset = '/ZZTo4L_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
config.Data.ignoreLocality = True
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 5
#NJOBS = 1000
#config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
#config.Data.totalUnits = 996
config.Data.totalUnits = 96
config.Data.outLFNDirBase = '/store/user/snandan/ZZTo4L_abdollah' # or '/store/group/<subdir>'
config.Data.publication = False
#config.Data.publishDataName = 'CRAB3_MCPhiSymmetry'

config.section_("Site")
config.Site.storageSite = '/eos/uscms/store/user/snandan/'
config.Site.storageSite = 'T3_US_FNALLPC'
config.Site.whitelist = ["T3_US_FNALLPC"]
config.Site.whitelist = ["T2_US_Nebraska"]   
config.Site.whitelist = ["T2_HU_Budapest"]
config.Site.whitelist = ["T2_UK_SGrid_Bristol"]
config.Site.whitelist = ["T2_RU_JINR"]                                                                                                      


#print config
