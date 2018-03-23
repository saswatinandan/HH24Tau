import FWCore.ParameterSet.Config as cms

externalLHEProducer = cms.EDProducer("ExternalLHEProducer",
    args = cms.vstring('/cvmfs/cms.cern.ch/phys_generator/gridpacks/slc6_amd64_gcc481/13TeV/madgraph/V5_2.2.2/DY1Jets_madgraph_5f_LO/v1/DY1Jets_madgraph_5f_LO_tarball.tar.xz'),
    nEvents = cms.untracked.uint32(5000),
    numberOfParameters = cms.uint32(1),
    outputFile = cms.string('cmsgrid_final.lhe'),
    scriptName = cms.FileInPath('GeneratorInterface/LHEInterface/data/run_generic_tarball_cvmfs.sh')
)

#Link to datacards:
#https://github.com/cms-sw/genproductions/tree/a890fbd3614b0b4ea2b13e31e24eb608184dab2f/bin/MadGraph5_aMCatNLO/cards/production/13TeV/DYJets_jetbinning_LO_MLM/DY1Jets_madgraph_5f_LO