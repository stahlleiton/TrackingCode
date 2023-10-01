import FWCore.ParameterSet.Config as cms

unpackedTracksAndVertices = cms.EDProducer('TrackAndVertexUnpacker',
  packedCandidates = cms.VInputTag(
    'packedPFCandidates',
    'lostTracks'
  ),
  packedCandidateNormChi2Map = cms.VInputTag(
    'packedPFCandidateTrackChi2',
    'lostTrackChi2'
  ),
  primaryVertices = cms.InputTag('offlineSlimmedPrimaryVertices'),
  secondaryVertices = cms.InputTag('slimmedSecondaryVertices'),
  recoverTracks = cms.bool(True),
  mightGet = cms.optional.untracked.vstring
)
