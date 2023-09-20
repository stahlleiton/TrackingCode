###Options to run
'''
sample="MC_RecoDebug","MC_Reco_AOD","MC_MiniAOD","Data_Reco_AOD","Data_MiniAOD"
n=integer number of events

To run it, please, do e.g.:
    cmsRun run_PbPb_cfg.py sample="Data_Reco_AOD" n=100 runOverStreams=False >& OutPut.txt &

IMPORTANT: only run runOverStreams=True together with sample="Data_Reco_AOD"
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
                  "MC_RecoDebug", # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, bool or float
                  "sample")
options.register ('n',
                  10, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, bool or float
                  "n")
options.register ('runOverStreams',
                  False, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.bool,          # string, int, bool or float
                  "runOverStreams")
options.parseArguments()

from pbpb import pbpb_mc_recodebug as pbpb
if options.sample == "MC_Reco_AOD":
    from pbpb import pbpb_mc_reco_aod as pbpb
if options.sample == "Data_Reco_AOD":
    from pbpb import pbpb_data_reco_aod as pbpb
if options.sample == "Data_Reco_AOD" and options.runOverStreams==True :
    from pbpb import pbpb_data_reco_aod_streams as pbpb

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

process.load("SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi")
process.quickTrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')

process.load("SimTracker.TrackerHitAssociation.tpClusterProducerDefault_cfi")
process.tpClusterProducer  = process.tpClusterProducerDefault.clone()

# Input source
process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames =  cms.untracked.vstring(pbpb),
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
process.HITrackCorrections.vertexSrc = cms.InputTag("offlinePrimaryVertices")
process.HITrackCorrections.qualityString = cms.string("highPurity")
process.HITrackCorrections.pfCandSrc = cms.InputTag("particleFlow")
process.HITrackCorrections.jetSrc = cms.InputTag("ak4CaloJets")
# options
process.HITrackCorrections.useCentrality = True
process.HITrackCorrections.applyTrackCuts = True
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
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2022_realistic_hi', '')
if (options.sample == "Data_Reco_AOD" or options.sample == "Data_MiniAOD"):
    process.GlobalTag = GlobalTag(process.GlobalTag, '124X_dataRun3_Prompt_v10', '')
###

#forest style analyzers (anaTrack module) (not affected by HITrackCorrections code)
process.load('HITrackingStudies.AnalyzerCode.trackAnalyzer_cff')
process.anaTrack.useCentrality = True
process.anaTrack.trackSrc = 'generalTracks'
process.anaTrack.mvaSrc = cms.InputTag("generalTracks","MVAValues")
process.anaTrack.doSimTrack = True
process.anaTrack.doSimVertex = True
process.anaTrack.fillSimTrack = True
process.anaTrack.doPFMatching = False
process.anaTrack.doHighestPtVertex = False
process.anaTrack.doTrackVtxWImpPar = False
if (options.sample == "MC_Reco_AOD" or options.sample == "MC_MiniAOD" or options.sample == "Data_Reco_AOD" or options.sample == "Data_MiniAOD"):
    process.anaTrack.doSimTrack = False
    process.anaTrack.doSimVertex = False
    process.anaTrack.fillSimTrack = False
###

###trigger selection for data
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltMB = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltMB.HLTPaths = ["HLT_HIMinimumBias_v2"]
process.hltMB.andOr = cms.bool(True)  # True = OR, False = AND between the HLT paths
process.hltMB.throw = cms.bool(False) # throw exception on unknown path names

process.p = cms.Path(
                      process.tpClusterProducer *
                      process.quickTrackAssociatorByHits *
                      process.tpRecoAssocGeneralTracks *
                      process.centralityBin *
                      process.HITrackCorrections *
                      process.anaTrack
)

if (options.sample == "MC_Reco_AOD" or options.sample == "MC_MiniAOD" or options.sample == "Data_Reco_AOD" or options.sample == "Data_MiniAOD"):
    process.p = cms.Path(process.centralityBin * process.anaTrack)
    if (options.sample == "Data_Reco_AOD" or options.sample == "Data_MiniAOD"):
        process.p = cms.Path(process.hltMB * process.centralityBin * process.anaTrack)
