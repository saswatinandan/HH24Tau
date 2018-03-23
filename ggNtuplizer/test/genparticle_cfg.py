import FWCore.ParameterSet.Config as cms

process = cms.Process('GEN')

process.load('FWCore.MessageService.MessageLogger_cfi')


process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
#'/store/mc/RunIISpring16MiniAODv2/ZZTo4L_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/00000/38182EE2-2352-E611-B861-00259073E3F2.root',
#'/store/mc/RunIISummer16MiniAODv2/ZZTo4L_13TeV_powheg_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000/221CC46F-2FC6-E611-8FFC-0CC47A1E0488.root',
  )
)

process.TFileService = cms.Service("TFileService",
                                    fileName = cms.string('GenParticle_LO.root')
)

process.myanalyzer = cms.EDAnalyzer("GenParticleInfo",
  genParticleSrc = cms.untracked.InputTag("prunedGenParticles"),
  generatorLabel = cms.InputTag("generator"),
  verbosity = cms.untracked.int32(1)
)
process.p = cms.Path(process.myanalyzer)






