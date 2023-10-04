###Options to run
'''
sample="MC_RecoDebug","MC_Reco_AOD","MC_MiniAOD","Data_Reco_AOD","Data_MiniAOD"
n=integer number of events
runOverStreams=False or True
usePixelTrks=False or True

To run it, please, do e.g.:
    cmsRun run_PbPb_cfg.py sample="Data_Reco_AOD" n=100 usePixelTrks=False  runOverStreams=False >& OutPut.txt &

IMPORTANT: only run runOverStreams=True together with sample="Data_Reco_AOD". FIXME: this option is not working for now

To change input files, please, look at pbpb.py file

'''

import FWCore.ParameterSet.Config as cms

process = cms.Process('TRACKANA')
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

options.register ('sample',
                  "Data_MiniAOD", # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, bool or float
                  "sample")
options.register ('n',
                  -1, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, bool or float
                  "n")
options.register ('runOverStreams',
                  False, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.bool,          # string, int, bool or float
                  "runOverStreams")
options.register ('usePixelTrks',
                  False, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.bool,          # string, int, bool or float
                  "usePixelTrks")
options.parseArguments()

from pbpb import pbpb_mc_recodebug as pbpb
if options.sample == "MC_Reco_AOD":
    from pbpb import pbpb_mc_reco_aod as pbpb
if options.sample == "Data_Reco_AOD":
    from pbpb import pbpb_data_reco_aod as pbpb
if options.sample == "Data_Reco_AOD" and options.runOverStreams==True :
    from pbpb import pbpb_data_reco_aod_streams as pbpb
if options.sample == "Data_MiniAOD":
    from pbpb import pbpb_data_miniaod as pbpb
if options.sample == "MC_MiniAOD":
    from pbpb import pbpb_mc_miniaod as pbpb


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.n)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('trk.root')
)

process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")

process.tpRecoAssocGeneralTracks = process.trackingParticleRecoTrackAsssociation.clone()
process.tpRecoAssocGeneralTracks.label_tr = cms.InputTag("generalTracks")
if options.usePixelTrks == True:
    process.tpRecoAssocGeneralTracks.label_tr = cms.InputTag("hiConformalPixelTracks")

process.load("SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi")
process.quickTrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')

process.load("SimTracker.TrackerHitAssociation.tpClusterProducerDefault_cfi")
process.tpClusterProducer  = process.tpClusterProducerDefault.clone()

# Input source
process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames =  cms.untracked.vstring('file:/eos/cms/store/group/phys_heavyions_ops/abaty/RAWPrimeChecks2023/RAWPrime/RAWPrime_Streamer_2.root'),
    skipEvents = cms.untracked.uint32(0),
    secondaryFileNames = cms.untracked.vstring()
)
if options.runOverStreams == True :
    process.source = cms.Source("NewEventStreamFileReader",
            fileNames =  cms.untracked.vstring(pbpb),
            skipEvents = cms.untracked.uint32(0)
    )

### centrality ###
process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi")
process.centralityBin.Centrality = cms.InputTag("hiCentrality")
process.centralityBin.centralityVariable = cms.string("HFtowers")

### Track cuts ###
# input collections
process.HITrackCorrections.centralitySrc = cms.InputTag("centralityBin","HFtowers")
process.HITrackCorrections.trackSrc = cms.InputTag("generalTracks")
if options.usePixelTrks == True:
    process.HITrackCorrections.trackSrc = cms.InputTag("hiConformalPixelTracks")
process.HITrackCorrections.vertexSrc = cms.InputTag("offlinePrimaryVertices")
process.HITrackCorrections.qualityString = cms.string("highPurity")
process.HITrackCorrections.pfCandSrc = cms.InputTag("particleFlow")
process.HITrackCorrections.jetSrc = cms.InputTag("ak4CaloJets")
# options
process.HITrackCorrections.useCentrality = True
process.HITrackCorrections.applyTrackCuts = True
if options.usePixelTrks == True:
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
process.GlobalTag = GlobalTag(process.GlobalTag, '132X_mcRun3_2023_realistic_HI_v2', '')
if (options.sample == "Data_Reco_AOD" or options.sample == "Data_MiniAOD"):
    process.GlobalTag = GlobalTag(process.GlobalTag, '132X_dataRun3_Express_v4', '')
###

#old track ntuple
process.anaTrackOLD = cms.EDAnalyzer('TrackAnalyzer2',
    doTrack = cms.bool(True),
    doTrackExtra = cms.bool(False),
    trackPtMin = cms.double(0.0),
    simTrackPtMin = cms.double(0.0),
    vertexSrc = cms.vstring(['offlineSlimmedPrimaryVertices']),
    trackSrc = cms.InputTag('generalTracks'),
    mvaSrc = cms.InputTag("generalTracks","MVAValues"),
    particleSrc = cms.InputTag('genParticles'),
    pfCandSrc = cms.InputTag('particleFlow'),
    beamSpotSrc = cms.InputTag('offlineBeamSpot'),
    doPFMatching = cms.bool(False),
    doSimTrack = cms.bool(False),
    doSimVertex = cms.bool(False),
    doHighestPtVertex = cms.bool(False),
    fillSimTrack = cms.bool(False),
    doDeDx = cms.bool(False),
    doDebug = cms.bool(False),
    useQuality = cms.bool(False),
    qualityStrings = cms.vstring(['highPurity','tight','loose']),
    tpFakeSrc = cms.InputTag('mix','MergedTrackTruth'),
    tpEffSrc = cms.InputTag('mix','MergedTrackTruth'),
    associatorMap = cms.InputTag('tpRecoAssocGeneralTracks'),
    doMVA = cms.bool(True),
    doTrackVtxWImpPar = cms.bool(False),
    trackVtxMaxDistance = cms.double(3.0),
    qualityString = cms.string("highPurity"),
    fiducialCut = cms.bool(False),
    tpVtxSrc = cms.InputTag("mix","MergedTrackTruth"),
    useCentrality = cms.bool(True),
    centralitySrc = cms.InputTag("centralityBin","HFtowers"),
    runMiniAOD = cms.bool(True),
    chi2Map = cms.InputTag('packedPFCandidateTrackChi2'),
    packedCandSrc = cms.InputTag('packedPFCandidates'),
    lostTracksSrc = cms.InputTag('lostTracks'),
    chi2MapLost = cms.InputTag('lostTrackChi2')
)

#forest style analyzers (anaTrack module) (not affected by HITrackCorrections code)
process.load('HITrackingStudies.AnalyzerCode.trackAnalyzer_cff')
process.anaTrack.useCentrality = True
process.anaTrack.trackSrc = 'generalTracks'
process.anaTrack.mvaSrc = cms.InputTag("generalTracks","MVAValues")
if options.usePixelTrks == True:
    process.anaTrack.trackSrc = 'hiConformalPixelTracks'
    process.anaTrack.doMVA = cms.bool(False)
process.anaTrack.doSimTrack = True
process.anaTrack.doSimVertex = True
process.anaTrack.fillSimTrack = True
process.anaTrack.doPFMatching = False
process.anaTrack.doHighestPtVertex = False
process.anaTrack.doTrackVtxWImpPar = False
process.anaSeq = cms.Sequence(process.anaTrack)
if (options.sample == "MC_Reco_AOD" or options.sample == "MC_MiniAOD" or options.sample == "Data_Reco_AOD" or options.sample == "Data_MiniAOD"):
    process.anaTrack.doSimTrack = False
    process.anaTrack.doSimVertex = False
    process.anaTrack.fillSimTrack = False
    if (options.sample == "MC_MiniAOD" or options.sample == "Data_MiniAOD"):
        process.load('HITrackingStudies.AnalyzerCode.unpackedTracksAndVertices_cfi')
        process.anaTrack.trackSrc = 'unpackedTracksAndVertices'
        process.anaTrack.vertexSrc = cms.vstring(['unpackedTracksAndVertices'])
        process.anaTrack.doMVA = cms.bool(False)
        process.hiPixelTracks = process.unpackedTracksAndVertices.clone(
                packedCandidates = cms.VInputTag("hiPixelTracks"),
                packedCandidateNormChi2Map = cms.VInputTag(""),
        )
        if options.usePixelTrks == True:
           process.anaTrack.trackSrc = 'hiPixelTracks'
           process.anaTrack.vertexSrc = cms.vstring(['hiPixelTracks'])
        process.anaSeq = cms.Sequence(process.unpackedTracksAndVertices * process.hiPixelTracks * process.anaTrack * process.anaTrackOLD)

###trigger selection for data
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltMB = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltMB.HLTPaths = ["HLT_HIMinimumBiasHF1AND_v*"]
process.hltMB.andOr = cms.bool(True)  # True = OR, False = AND between the HLT paths
process.hltMB.throw = cms.bool(False) # throw exception on unknown path names

process.p = cms.Path(
                      process.tpClusterProducer *
                      process.quickTrackAssociatorByHits *
                      process.tpRecoAssocGeneralTracks *
                      process.centralityBin *
                      process.HITrackCorrections *
                      process.anaSeq
)

if (options.sample == "MC_Reco_AOD" or options.sample == "MC_MiniAOD" or options.sample == "Data_Reco_AOD" or options.sample == "Data_MiniAOD"):
    process.p = cms.Path(process.centralityBin * process.anaSeq)
    if (options.sample == "Data_Reco_AOD"):
        process.p = cms.Path(process.hltMB * process.centralityBin * process.anaSeq)
