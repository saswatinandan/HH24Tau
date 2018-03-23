import FWCore.ParameterSet.Config as cms

externalLHEProducer = cms.EDProducer("ExternalLHEProducer",
    args = cms.vstring('/cvmfs/cms.cern.ch/phys_generator/gridpacks/slc6_amd64_gcc481/13TeV/madgraph/V5_2.2.2/DYJets_HT_LO_MLM/DYJets_HT-incl/V1/DYJets_HT-incl_tarball.tar.xz'),
    nEvents = cms.untracked.uint32(5000),
    numberOfParameters = cms.uint32(1),
    outputFile = cms.string('cmsgrid_final.lhe'),
    scriptName = cms.FileInPath('GeneratorInterface/LHEInterface/data/run_generic_tarball_cvmfs.sh')
)

#Link to datacards:
#https://github.com/cms-sw/genproductions/tree/9adb22e84bea40915d27a7ed94a5a6e740423efc/bin/MadGraph5_aMCatNLO/cards/production/13TeV/DYJets_HT_LO_MLM/DYJets_HT_mll50/DYJets_HT-incl