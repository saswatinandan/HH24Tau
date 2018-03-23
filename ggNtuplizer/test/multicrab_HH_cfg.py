from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.workArea = 'MC_HH'
config.section_('JobType')
config.JobType.psetName = 'run_mc_80X.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['ggtree_mc.root']
config.JobType.inputFiles = ['Summer16_23Sep2016AllV4_DATA.db','Summer16_23Sep2016V4_MC.db']
config.section_('Data')
config.Data.unitsPerJob = 2
#config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.outLFNDirBase = '/store/user/snandan/officialHHMc'
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
    config.General.workArea = 'MC_HH'

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


'''
config.General.requestName = "GluGluToHHTo4Tau_node_5_13TeV"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_5_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-550"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-550_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)


config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-250_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-250_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "ZHToTauTau_M125_13TeV_powheg_pythia8"
config.Data.inputDataset = "/ZHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM" 
submit(config)
'''


config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-250_narrow"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-250_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-260"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-260_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-270"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-270_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-280"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-280_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-300"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-300_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-320"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-320_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-340"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-340_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-350"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-350_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-400"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-400_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-450"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-450_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-500"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-500_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-550"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-550_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-600"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-600_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-650"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-650_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-700"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-700_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-750"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-750_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-800"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-800_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToBulkGravitonToHHTo4Tau_M-900"
config.Data.inputDataset = "/GluGluToBulkGravitonToHHTo4Tau_M-900_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToHHTo4Tau_node_10_13TeV"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_10_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToHHTo4Tau_node_11_13TeV"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_11_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToHHTo4Tau_node_12_13TeV"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_12_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToHHTo4Tau_node_2_13TeV"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_2_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToHHTo4Tau_node_3_13TeV"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_3_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToHHTo4Tau_node_4_13TeV"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_4_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToHHTo4Tau_node_5_13TeV"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_5_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToHHTo4Tau_node_6_13TeV"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_6_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToHHTo4Tau_node_7_13TeV"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_7_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToHHTo4Tau_node_8_13TeV"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_8_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToHHTo4Tau_node_9_13TeV"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_9_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToHHTo4Tau_node_SM_13TeV-madgraph"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_SM_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToHHTo4Tau_node_box_13TeV-madgraph"
config.Data.inputDataset = "/GluGluToHHTo4Tau_node_box_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-250_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-250_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-260_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-260_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-270_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-270_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-280_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-280_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-300_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-300_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-320_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-320_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-340_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-340_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-350_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-350_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-400_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-400_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-450_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-450_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-500_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-500_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-550_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-550_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-600_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-600_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-650_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-650_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-700_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-700_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-750_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-750_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-800_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-800_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRSGravitonToHHTo4Tau_M-900_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRSGravitonToHHTo4Tau_M-900_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-250_narrow_13TeV"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-250_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-260" 
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-260_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-270"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-270_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-280"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-280_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-300"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-300_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-320"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-320_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)


config.General.requestName = "GluGluToRadionToHHTo4Tau_M-340"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-340_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-350"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-350_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-400"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-400_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-450"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-450_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-500"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-500_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-550"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-550_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-600"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-600_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)


config.General.requestName = "GluGluToRadionToHHTo4Tau_M-650"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-650_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-700"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-700_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-750"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-750_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

config.General.requestName = "GluGluToRadionToHHTo4Tau_M-800"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-800_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)


config.General.requestName = "GluGluToRadionToHHTo4Tau_M-900"
config.Data.inputDataset = "/GluGluToRadionToHHTo4Tau_M-900_narrow_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
submit(config)

