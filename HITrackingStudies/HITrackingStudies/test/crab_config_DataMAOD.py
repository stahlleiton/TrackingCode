###For a description of the crabConfig.py parameters. See:
###https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile

import CRABClient
from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.workArea        = 'TrackingStudies_Run3'
config.General.requestName     = 'GeneralTracks_DataMAOD_v1'
config.General.transferLogs    = True 
config.General.transferOutputs = True
################################
config.section_('JobType')
config.JobType.pluginName      = 'Analysis'
config.JobType.psetName        = 'run_PbPb_cfg_Data.py'
config.JobType.inputFiles      = ['CentralityTable_HFtowers200_DataPbPb_periHYDJETshape_run3v1302x04_offline_374810.db']
################################
config.section_('Data')
config.Data.inputDataset       = '/HIPhysicsRawPrime0/HIRun2023A-PromptReco-v2/MINIAOD'
config.Data.runRange           = '374810'
config.Data.splitting          = 'LumiBased'
config.Data.unitsPerJob        = 10
config.Data.totalUnits         = -1
config.Data.inputDBS           = 'global'
config.Data.outLFNDirBase      = '/store/group/phys_heavyions/rpradhan/TrackingEffTables2022PbPbRun'
config.Data.outputDatasetTag   = 'GeneralTracks_DataMAOD_v1'
################################
config.section_('Site')
config.Site.storageSite        = 'T2_CH_CERN'
#config.Site.storageSite        = 'T2_IN_TIFR'

