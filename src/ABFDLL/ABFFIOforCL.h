/************************************************************
*					ABFFIOforCL.h							*
*															*
*	Includes prototypes of functions for reading ABF 10.0	*
*	files in ChanneLab using the ABFDLL.lib					*
*															*
************************************************************/

#ifndef ABFFIOFORCL_HEADER
#define ABFFIOFORCL_HEADER

#include <windows.h>

// Include the constants for the ABFFileHeader struct
#ifndef ABF_CONSTANTS
#include "ABFConstants.h"
#endif

#define READ_OPEN	0
#define WRITE_OPEN	1
/*	#define APPEND_OPEN	2	*/

typedef struct CL_ABFHeader_Tag{
   // GROUP #1 - File ID and size information
   float    fFileVersionNumber;
   short    nOperationMode;
   long     lActualAcqLength;
   short    nNumPointsIgnored;
   long     lActualEpisodes;
   UINT     uFileStartDate;         // YYYYMMDD
   UINT     uFileStartTimeMS;
   long     lStopwatchTime;
   float    fHeaderVersionNumber;
   short    nFileType;

   // GROUP #2 - File Structure
   long     lDataSectionPtr;
   long     lTagSectionPtr;
   long     lNumTagEntries;
   long     lScopeConfigPtr;
   long     lNumScopes;
   long     lDeltaArrayPtr;
   long     lNumDeltas;
   long     lVoiceTagPtr;
   long     lVoiceTagEntries;
   long     lSynchArrayPtr;
   long     lSynchArraySize;
   short    nDataFormat;
   short    nSimultaneousScan;
   long     lStatisticsConfigPtr;
   long     lAnnotationSectionPtr;
   long     lNumAnnotations;
   long     lDACFilePtr[ABF_DACCOUNT];
   long     lDACFileNumEpisodes[ABF_DACCOUNT];

   // GROUP #3 - Trial hierarchy information
   short    nADCNumChannels;
   float    fADCSequenceInterval;
   UINT     uFileCompressionRatio;
   bool     bEnableFileCompression;
   float    fSynchTimeUnit;
   float    fSecondsPerRun;
   long     lNumSamplesPerEpisode;
   long     lPreTriggerSamples;
   long     lEpisodesPerRun;
   long     lRunsPerTrial;
   long     lNumberOfTrials;
   short    nAveragingMode;
   short    nUndoRunCount;
   short    nFirstEpisodeInRun;
   float    fTriggerThreshold;
   short    nTriggerSource;
   short    nTriggerAction;
   short    nTriggerPolarity;
   float    fScopeOutputInterval;
   float    fEpisodeStartToStart;
   float    fRunStartToStart;
   float    fTrialStartToStart;
   long     lAverageCount;
   short    nAutoTriggerStrategy;
   float    fFirstRunDelayS;

   // GROUP #4 - Display Parameters
   short    nDataDisplayMode;
   short    nChannelStatsStrategy;
   long     lSamplesPerTrace;
   long     lStartDisplayNum;
   long     lFinishDisplayNum;
   short    nShowPNRawData;
   float    fStatisticsPeriod;
   long     lStatisticsMeasurements;
   short    nStatisticsSaveStrategy;

   // GROUP #5 - Hardware information
   float    fADCRange;
   float    fDACRange;
   long     lADCResolution;
   long     lDACResolution;
   short    nDigitizerADCs;
   short    nDigitizerDACs;
   short    nDigitizerTotalDigitalOuts;
   short    nDigitizerSynchDigitalOuts;
   short    nDigitizerType;

   // GROUP #6 Environmental Information
   short    nExperimentType;
   short    nManualInfoStrategy;
   float    fCellID1;
   float    fCellID2;
   float    fCellID3;
   char     sProtocolPath[ABF_PATHLEN];
   char     sCreatorInfo[ABF_CREATORINFOLEN];
   char     sModifierInfo[ABF_CREATORINFOLEN];
   short    nCommentsEnable;
   char     sFileComment[ABF_FILECOMMENTLEN];
   short    nTelegraphEnable[ABF_ADCCOUNT];
   short    nTelegraphInstrument[ABF_ADCCOUNT];
   float    fTelegraphAdditGain[ABF_ADCCOUNT];
   float    fTelegraphFilter[ABF_ADCCOUNT];
   float    fTelegraphMembraneCap[ABF_ADCCOUNT];
   float    fTelegraphAccessResistance[ABF_ADCCOUNT];
   short    nTelegraphMode[ABF_ADCCOUNT];
   short    nTelegraphDACScaleFactorEnable[ABF_DACCOUNT];

   short    nAutoAnalyseEnable;

   GUID     FileGUID;
   float    fInstrumentHoldingLevel[ABF_DACCOUNT];
   unsigned long ulFileCRC;
   short    nCRCEnable;

   // GROUP #7 - Multi-channel information
   short    nSignalType;                        // why is this only single channel ?
   short    nADCPtoLChannelMap[ABF_ADCCOUNT];
   short    nADCSamplingSeq[ABF_ADCCOUNT];
   float    fADCProgrammableGain[ABF_ADCCOUNT];
   float    fADCDisplayAmplification[ABF_ADCCOUNT];
   float    fADCDisplayOffset[ABF_ADCCOUNT];
   float    fInstrumentScaleFactor[ABF_ADCCOUNT];
   float    fInstrumentOffset[ABF_ADCCOUNT];
   float    fSignalGain[ABF_ADCCOUNT];
   float    fSignalOffset[ABF_ADCCOUNT];
   float    fSignalLowpassFilter[ABF_ADCCOUNT];
   float    fSignalHighpassFilter[ABF_ADCCOUNT];
   char     nLowpassFilterType[ABF_ADCCOUNT];
   char     nHighpassFilterType[ABF_ADCCOUNT];
   bool     bHumFilterEnable[ABF_ADCCOUNT];

   char     sADCChannelName[ABF_ADCCOUNT][ABF_ADCNAMELEN];   // extra chars so name can be modified for P/N
   char     sADCUnits[ABF_ADCCOUNT][ABF_ADCUNITLEN];
   float    fDACScaleFactor[ABF_DACCOUNT];
   float    fDACHoldingLevel[ABF_DACCOUNT];
   float    fDACCalibrationFactor[ABF_DACCOUNT];
   float    fDACCalibrationOffset[ABF_DACCOUNT];
   char     sDACChannelName[ABF_DACCOUNT][ABF_DACNAMELEN];
   char     sDACChannelUnits[ABF_DACCOUNT][ABF_DACUNITLEN];

   // GROUP #9 - Epoch Waveform and Pulses
   short    nDigitalEnable;
   short    nActiveDACChannel;                     // should retire !
   short    nDigitalDACChannel;
   short    nDigitalHolding;
   short    nDigitalInterEpisode;
   short    nDigitalTrainActiveLogic;
   short    nDigitalValue[ABF_EPOCHCOUNT];
   short    nDigitalTrainValue[ABF_EPOCHCOUNT];
   bool     bEpochCompression[ABF_EPOCHCOUNT];
   short    nWaveformEnable[ABF_DACCOUNT];
   short    nWaveformSource[ABF_DACCOUNT];
   short    nInterEpisodeLevel[ABF_DACCOUNT];
   short    nEpochType[ABF_DACCOUNT][ABF_EPOCHCOUNT];
   float    fEpochInitLevel[ABF_DACCOUNT][ABF_EPOCHCOUNT];
   float    fEpochFinalLevel[ABF_DACCOUNT][ABF_EPOCHCOUNT]; // Only used for ABF_EPOCHSLOPE.
   float    fEpochLevelInc[ABF_DACCOUNT][ABF_EPOCHCOUNT];
   long     lEpochInitDuration[ABF_DACCOUNT][ABF_EPOCHCOUNT];
   long     lEpochDurationInc[ABF_DACCOUNT][ABF_EPOCHCOUNT];
   short    nEpochTableRepetitions[ABF_DACCOUNT];
   float    fEpochTableStartToStartInterval[ABF_DACCOUNT];

   // GROUP #10 - DAC Output File
   float    fDACFileScale[ABF_DACCOUNT];
   float    fDACFileOffset[ABF_DACCOUNT];
   long     lDACFileEpisodeNum[ABF_DACCOUNT];
   short    nDACFileADCNum[ABF_DACCOUNT];
   char     sDACFilePath[ABF_DACCOUNT][ABF_PATHLEN];

   // GROUP #11a - Presweep (conditioning) pulse train
   short    nConditEnable[ABF_DACCOUNT];
   long     lConditNumPulses[ABF_DACCOUNT];
   float    fBaselineDuration[ABF_DACCOUNT];
   float    fBaselineLevel[ABF_DACCOUNT];
   float    fStepDuration[ABF_DACCOUNT];
   float    fStepLevel[ABF_DACCOUNT];
   float    fPostTrainPeriod[ABF_DACCOUNT];
   float    fPostTrainLevel[ABF_DACCOUNT];
   float    fCTStartLevel[ABF_DACCOUNT][ABF_EPOCHCOUNT];
   float    fCTEndLevel[ABF_DACCOUNT][ABF_EPOCHCOUNT];
   float    fCTIntervalDuration[ABF_DACCOUNT][ABF_EPOCHCOUNT];
   float    fCTStartToStartInterval[ABF_DACCOUNT];

   // GROUP #11b - Membrane Test Between Sweeps
   short    nMembTestEnable[ABF_DACCOUNT];
   float    fMembTestPreSettlingTimeMS[ABF_DACCOUNT];
   float    fMembTestPostSettlingTimeMS[ABF_DACCOUNT];

    // GROUP #11c - PreSignal test pulse
   short    nPreSignalEnable[ABF_DACCOUNT];
   float    fPreSignalPreStepDuration[ABF_DACCOUNT];
   float    fPreSignalPreStepLevel[ABF_DACCOUNT];
   float    fPreSignalStepDuration[ABF_DACCOUNT];
   float    fPreSignalStepLevel[ABF_DACCOUNT];
   float    fPreSignalPostStepDuration[ABF_DACCOUNT];
   float    fPreSignalPostStepLevel[ABF_DACCOUNT];

   // GROUP #12 - Variable parameter user list
   short    nULEnable[ABF_USERLISTCOUNT];
   short    nULParamToVary[ABF_USERLISTCOUNT];
   short    nULRepeat[ABF_USERLISTCOUNT];
   char     sULParamValueList[ABF_USERLISTCOUNT][ABF_USERLISTLEN];

   // GROUP #13 - Statistics measurements
   short    nStatsEnable;
   unsigned short nStatsActiveChannels;             // Active stats channel bit flag
   unsigned short nStatsSearchRegionFlags;          // Active stats region bit flag
   short    nStatsSmoothing;
   short    nStatsSmoothingEnable;
   short    nStatsBaseline;
   short    nStatsBaselineDAC;                      // If mode is epoch, then this holds the DAC
   long     lStatsBaselineStart;
   long     lStatsBaselineEnd;
   long     lStatsMeasurements[ABF_STATS_REGIONS];  // Measurement bit flag for each region
   long     lStatsStart[ABF_STATS_REGIONS];
   long     lStatsEnd[ABF_STATS_REGIONS];
   short    nRiseBottomPercentile[ABF_STATS_REGIONS];
   short    nRiseTopPercentile[ABF_STATS_REGIONS];
   short    nDecayBottomPercentile[ABF_STATS_REGIONS];
   short    nDecayTopPercentile[ABF_STATS_REGIONS];
   short    nStatsChannelPolarity[ABF_ADCCOUNT];
   short    nStatsSearchMode[ABF_STATS_REGIONS];    // Stats mode per region: mode is cursor region, epoch etc
   short    nStatsSearchDAC[ABF_STATS_REGIONS];     // If mode is epoch, then this holds the DAC

   // GROUP #14 - Channel Arithmetic
   short    nArithmeticEnable;
   short    nArithmeticExpression;
   float    fArithmeticUpperLimit;
   float    fArithmeticLowerLimit;
   short    nArithmeticADCNumA;
   short    nArithmeticADCNumB;
   float    fArithmeticK1;
   float    fArithmeticK2;
   float    fArithmeticK3;
   float    fArithmeticK4;
   float    fArithmeticK5;
   float    fArithmeticK6;
   char     sArithmeticOperator[ABF_ARITHMETICOPLEN];
   char     sArithmeticUnits[ABF_ARITHMETICUNITSLEN];

   // GROUP #15 - Leak subtraction
   short    nPNPosition;
   short    nPNNumPulses;
   short    nPNPolarity;
   float    fPNSettlingTime;
   float    fPNInterpulse;
   short    nLeakSubtractType[ABF_DACCOUNT];
   float    fPNHoldingLevel[ABF_DACCOUNT];
   short    nLeakSubtractADCIndex[ABF_DACCOUNT];

   // GROUP #16 - Miscellaneous variables
   short    nLevelHysteresis;
   long     lTimeHysteresis;
   short    nAllowExternalTags;
   short    nAverageAlgorithm;
   float    fAverageWeighting;
   short    nUndoPromptStrategy;
   short    nTrialTriggerSource;
   short    nStatisticsDisplayStrategy;
   short    nExternalTagType;
   long     lHeaderSize;
   short    nStatisticsClearStrategy;
   short    nEnableFirstLastHolding;            // First & Last Holding are now optional.

   // GROUP #17 - Trains parameters
   long     lEpochPulsePeriod[ABF_DACCOUNT][ABF_EPOCHCOUNT];
   long     lEpochPulseWidth [ABF_DACCOUNT][ABF_EPOCHCOUNT];

   // GROUP #18 - Application version data
   short    nCreatorMajorVersion;
   short    nCreatorMinorVersion;
   short    nCreatorBugfixVersion;
   short    nCreatorBuildVersion;
   short    nModifierMajorVersion;
   short    nModifierMinorVersion;
   short    nModifierBugfixVersion;
   short    nModifierBuildVersion;

   // GROUP #19 - LTP protocol
   short    nLTPType;
   short    nLTPUsageOfDAC[ABF_DACCOUNT];
   short    nLTPPresynapticPulses[ABF_DACCOUNT];

   // GROUP #20 - Digidata 132x Trigger out flag
   short    nScopeTriggerOut;

   // GROUP #22 - Alternating episodic mode
   short    nAlternateDACOutputState;
   short    nAlternateDigitalOutputState;
   short    nAlternateDigitalValue[ABF_EPOCHCOUNT];
   short    nAlternateDigitalTrainValue[ABF_EPOCHCOUNT];

   // GROUP #23 - Post-processing actions
   float    fPostProcessLowpassFilter[ABF_ADCCOUNT];
   char     nPostProcessLowpassFilterType[ABF_ADCCOUNT];

   // GROUP #24 - Legacy gear shift info
   float    fLegacyADCSequenceInterval;
   float    fLegacyADCSecondSequenceInterval;
   long     lLegacyClockChange;
   long     lLegacyNumSamplesPerEpisode;
}CL_ABFHeader;

#ifdef __cplusplus
extern "C" {
#endif

#if defined DllExport
#define DLLFUN   __declspec( dllexport )
#elif defined DllImport
#define DLLFUN   __declspec( dllimport )
#endif

DLLFUN int OpenABFFile (char *fileName, int *pFileHandle, int openMode, int IsParamFile,
									   CL_ABFHeader *pCL_ABFFH, unsigned int *puMaxSamples, unsigned long *pdwMaxEpisodes);
DLLFUN int IsABFFile (const char *fileName, int *pDataFormat, int *pnError);
DLLFUN int GetActualSamples (int fileHandle, unsigned int *pNumActualSamples);
DLLFUN int ReadSweep (int fileHandle, const CL_ABFHeader *pCL_ABFFH, int nChannel, unsigned long dwEpisode, float *pfBuffer, unsigned int *pNumSamplesRead);
DLLFUN int ReadMultiplexSweep (int fileHandle, const CL_ABFHeader *pCL_ABFFH, unsigned long episode, void *pvBuffer, unsigned int *pNumSamplesRead);
DLLFUN int ReadRawSweep (int fileHandle, const CL_ABFHeader *pCL_ABFFH, int nChannel, unsigned long episode, void *pvBuffer, unsigned int *pNumSamplesRead);
DLLFUN int WriteMultiplexSweep (int fileHandle, const CL_ABFHeader *pCL_ABFFH, const void *pvBuffer, unsigned long dwEpiStartInSamples, unsigned int uSizeInSamples,
											   int appendData);
DLLFUN int WriteRawSweep (int fileHandle, const void *pvBuffer, unsigned long dwSizeInBytes);
DLLFUN int WriteWaveform ( int fileHandle, CL_ABFHeader *pCL_ABFFH, unsigned int uDACChannel, const short *pnDACArray);
DLLFUN int UpdateABFHeader (int fileHandle, CL_ABFHeader *pCL_ABFFH);
DLLFUN int GetWaveform (int fileHandle, const CL_ABFHeader *pCL_ABFFH, unsigned int uDACChannel, unsigned long dwEpisode, float *pfBuffer);
DLLFUN int CloseABFFile (int fileHandle);

DLLFUN int InitializeHeader (CL_ABFHeader *pCL_ABFFH);
DLLFUN int GetABFError (int nError, const char *fileName, char *textBuffer, UINT uMaxLen);
DLLFUN int GetADCScaleFactor (const CL_ABFHeader *pCL_ABFFH, int nChannel, float *pfADCToUUFactor,
											 float *fADCToUUShift);
DLLFUN int ReadParams (int fileHandle, CL_ABFHeader *pCL_ABFFH, int *pError);
DLLFUN int WriteParams (const char *pFilename, CL_ABFHeader *pCL_ABFFH, int *pError);
DLLFUN int GetEpisodeInfo (int fileHandle, const CL_ABFHeader *pCL_ABFFH, int nChannel, unsigned long dwEpisode, unsigned int *pNumSamples,
										  double *pStartTime, double *pDuration, int *pnError);
DLLFUN int GetTrialDuration (int fileHandle, const CL_ABFHeader *pCL_ABFFH, double *pTrialDuration, int *pnError);
DLLFUN int CopyCLHeaders (const CL_ABFHeader *pSourceFH, CL_ABFHeader *pTargetFH);
DLLFUN int OldABF_OpenWrite (char *fileName, int *pFileHandle, CL_ABFHeader *pCL_ABFFH, int *pnError);
DLLFUN int OldABF_WriteRawSweep (int fileHandle, const void *pvBuffer, unsigned long sizeInBytes);
DLLFUN int OldABF_WriteMultiplexSweep (int fileHandle, const CL_ABFHeader *pCL_ABFFH, const void *pvBuffer, unsigned long episodeStart, unsigned int numSamples);
DLLFUN int OldABF_Close (int fileHandle);
DLLFUN int OldABF_UpdateHeader (int fileHandle, CL_ABFHeader *pCL_ABFFH);

#ifdef __cplusplus
}
#endif

#endif //End of ABFFIOFORCL_HEADER
