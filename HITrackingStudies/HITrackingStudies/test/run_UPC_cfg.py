###Options to run
'''
sample="MC_RecoDebug","MC_Reco_AOD","MC_MiniAOD","Data_Reco_AOD","Data_MiniAOD"
n=integer number of events

To run it, please, do e.g.:
    cmsRun run_PbPb_cfg.py sample="Data_Reco_AOD" n=100 runOverStreams=False >& OutPut.txt &

'''

import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
process = cms.Process('TRACKANA', eras.Run3_2023_UPC)
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('HITrackingStudies.HITrackingStudies.HITrackCorrectionAnalyzer_cfi')

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing('analysis')

options.register ('n',
                  -1, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, bool or float
                  "n")
options.parseArguments()

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.n)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('trk_GeneralTrack.root')
)

# Input source
process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames =  cms.untracked.vstring('file:/eos/cms/store/group/phys_heavyions/anstahll/CERN/PbPb2023/MC/STARLIGHT/2024_01_19/STARLIGHT_5p36TeV_2023Run3/single_diff_STARLIGHT_5p36TeV_2023Run3_UPCRECO_2024_01_19/240119_020810/0000/STARLIGHT_single_diff_RECO_1.root'),
    skipEvents = cms.untracked.uint32(0),
    secondaryFileNames = cms.untracked.vstring()
)

### Track cuts ###
# input collections
process.HITrackCorrections.centralitySrc = cms.InputTag("")
process.HITrackCorrections.trackSrc = cms.InputTag("generalTracks")
process.HITrackCorrections.vertexSrc = cms.InputTag("offlinePrimaryVertices")
process.HITrackCorrections.qualityString = cms.string("highPurity")
process.HITrackCorrections.pfCandSrc = cms.InputTag("particleFlow")
process.HITrackCorrections.jetSrc = cms.InputTag("ak4CaloJets")
process.HITrackCorrections.associatorMap = cms.InputTag("trackingParticleRecoTrackAsssociation")

# options
process.HITrackCorrections.useCentrality = False
process.HITrackCorrections.applyTrackCuts = False
process.HITrackCorrections.fillNTuples = False
process.HITrackCorrections.applyVertexZCut = False
process.HITrackCorrections.doVtxReweighting = False
process.HITrackCorrections.doCaloMatched = False
# cut values
process.HITrackCorrections.dxyErrMax = 3.0
process.HITrackCorrections.dzErrMax = 3.0
process.HITrackCorrections.ptErrMax = 0.1
process.HITrackCorrections.nhitsMin = 11
process.HITrackCorrections.chi2nMax = 0.18
process.HITrackCorrections.reso = 0.5

#process.HITrackCorrections.crossSection = 1.0 #1.0 is no reweigh
#algo 
process.HITrackCorrections.algoParameters = cms.vint32(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46)
# vertex reweight parameters
process.HITrackCorrections.vtxWeightParameters = cms.vdouble(0.0306789, 0.427748, 5.16555, 0.0228019, -0.02049, 7.01258 )
###
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '132X_mcRun3_2023_realistic_HI_v7')
###

#forest style analyzers (anaTrack module) (not affected by HITrackCorrections code)
process.load('HITrackingStudies.AnalyzerCode.trackAnalyzer_cff')
process.anaTrack.associatorMap = cms.InputTag("trackingParticleRecoTrackAsssociation")
process.anaTrack.useCentrality = False
process.anaTrack.trackSrc = 'generalTracks'
process.anaTrack.mvaSrc = cms.InputTag("generalTracks","MVAValues")
process.anaTrack.doMVA = False
process.anaTrack.doSimTrack = True
process.anaTrack.doSimVertex = True
process.anaTrack.fillSimTrack = True
process.anaTrack.doPFMatching = False
process.anaTrack.doHighestPtVertex = False
process.anaTrack.doTrackVtxWImpPar = False

process.load('SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi')
process.trackingParticleRecoTrackAsssociation.label_tr = cms.InputTag("generalTracks")
process.load('SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi')
process.quickTrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')
process.load('SimTracker.TrackerHitAssociation.tpClusterProducer_cfi')
process.simRecoTrackAssocSeq = cms.Sequence(process.tpClusterProducer * process.quickTrackAssociatorByHits * process.trackingParticleRecoTrackAsssociation)

process.p = cms.Path(
                      process.simRecoTrackAssocSeq *
                      process.HITrackCorrections *
                      process.anaTrack
)
