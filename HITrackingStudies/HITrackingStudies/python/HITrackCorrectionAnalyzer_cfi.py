import FWCore.ParameterSet.Config as cms


HITrackCorrections = cms.EDAnalyzer('HITrackCorrectionAnalyzer',
    trackSrc = cms.InputTag("generalTracks"),
    vertexSrc = cms.InputTag("offlinePrimaryVertices"),
    pfCandSrc = cms.InputTag("particleFlowTmp"),
    jetSrc = cms.InputTag("akPu4CaloJets"),
    tpEffSrc = cms.InputTag('mix','MergedTrackTruth'),
    tpFakSrc = cms.InputTag('mix','MergedTrackTruth'),
    associatorMap = cms.InputTag('tpRecoAssocGeneralTracks'),
    ptBins = cms.vdouble(
        0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45,
        0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,
        1.0, 1.05, 1.1, 1.15, 1.2,
        1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0,
        2.5, 3.0, 4.0, 5.0, 7.5, 10.0, 12.0, 15.0,
        20.0, 25.0, 30.0, 45.0, 60.0, 90.0, 120.0, 
        180.0, 300.0, 500.0
    ),
    etaBins = cms.vdouble( 
        -3.0, -2.4, -2.0, -1.6, -1.2, -0.8, -0.4, 0.0,
        0.4, 0.8, 1.2, 1.6, 2.0, 2.4, 3.0
    ),
    occBins = cms.vdouble(0.0, 20.0, 40.0, 60.0, 80.0, 100.0, 120.0, 140.0, 160.0, 180.0, 200.0),
    vtxWeightParameters = cms.vdouble( 4.49636e-02, 1.36629e-01, 5.30010e+00,
                                       2.50170e-02, 4.59123e-01, 9.64888e+00 ),
    algoParameters = cms.vint32(4,5,6,7),
    doCaloMatched = cms.bool(True),
    reso = cms.double(2.0),
    crossSection = cms.double(1.0),
    doVtxReweighting = cms.bool(False),
    applyVertexZCut = cms.bool(False),
    vertexZMax = cms.double(15.),
    applyTrackCuts = cms.bool(True),
    qualityString = cms.string("highPurity"),
    dxyErrMax = cms.double(3.0),
    dzErrMax = cms.double(3.0),
    ptErrMax = cms.double(0.1),
    nhitsMin = cms.int32(11),
    chi2nMax = cms.double(0.15),
    doMomRes = cms.bool(False),
    fillNTuples = cms.bool(False),
    useCentrality = cms.bool(False),
    centralitySrc = cms.InputTag("centralityBin","HFTowers")
)
