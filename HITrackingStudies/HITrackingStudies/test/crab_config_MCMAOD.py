###For a description of the crabConfig.py parameters. See:
###https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile

import CRABClient
from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.workArea        = 'TrackingStudies_Run3'
config.General.requestName     = 'GeneralTracks_MCMAOD_v1'
config.General.transferLogs    = True 
config.General.transferOutputs = True
################################
config.section_('JobType')
config.JobType.pluginName      = 'Analysis'
config.JobType.psetName        = 'run_PbPb_cfg_MC.py'
config.JobType.inputFiles      = ['CentralityTable_HFtowers200_HydjetDrum5F_v1302x04_RECODEBUG_MC2023.db']
################################
config.section_('Data')
config.Data.inputDataset       = '/MinBias_PbPb_5p36TeV_Hydjet_v1/sarteaga-MinBias_PbPb_5p36TeV_Hydjet_MINIAOD_v5-45d226c4498e665b330b684cce3e6ab4/USER'
config.Data.splitting          = 'FileBased'
config.Data.unitsPerJob        = 40
config.Data.totalUnits         = -1
config.Data.inputDBS           = 'phys03'
config.Data.outLFNDirBase      = '/store/group/phys_heavyions/rpradhan/TrackingEffTables2022PbPbRun'
config.Data.outputDatasetTag   = 'GeneralTracks_MCMAOD_v1'
################################
config.section_('Site')
config.Site.storageSite        = 'T2_CH_CERN'
#config.Site.storageSite        = 'T2_IN_TIFR'

