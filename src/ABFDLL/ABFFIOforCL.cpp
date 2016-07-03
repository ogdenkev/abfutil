#define DllExport
#include "ABFFIOforCL.h"
#include "abffiles.h"

//Forward function declarations
int ConvertCLHeaderToABFHeader (const CL_ABFHeader *clFH, ABFFileHeader *abfFH);
int ConvertABFHeaderToCLHeader (const ABFFileHeader *abfFH, CL_ABFHeader *clFH);

//global variables
//ABFFileHeader FH;
//int currentFileHandle=0;

//*************************************************************************************************
/*
(bool CheckFileHandle(const int fileHandle)
{
	if( fileHandle != currentFileHandle )
		return false;

	return true;
}
*/
//*************************************************************************************************
int OpenABFFile (char *fileName, int *pFileHandle, int openMode, int IsParamFile,
				 CL_ABFHeader *pCL_ABFFH, unsigned int *puMaxSamples, unsigned long *pdwMaxEpisodes)
{
	int nError=0;
	ABFFileHeader FH;

	ABFH_Initialize (&FH);
	ConvertCLHeaderToABFHeader (pCL_ABFFH, &FH);

	switch(openMode){
		case READ_OPEN:
			if(IsParamFile){
				if( !ABF_ReadOpen(fileName, pFileHandle, ABF_PARAMFILE, &FH, puMaxSamples, pdwMaxEpisodes, &nError) )
					return nError;
			}
			else{
				if( !ABF_ReadOpen(fileName, pFileHandle, ABF_DATAFILE, &FH, puMaxSamples, pdwMaxEpisodes, &nError) )
					return nError;
			}
			break;
		case WRITE_OPEN:
			if(IsParamFile){
				if( !ABF_WriteOpen(fileName, pFileHandle, ABF_PARAMFILE, &FH, &nError) )
					return nError;
			}
			else{
				if( !ABF_WriteOpen(fileName, pFileHandle, ABF_DATAFILE, &FH, &nError) )
					return nError;
			}
			break;
		/*	wanted to include this, but there is no link to ABF_AppendOpen	*/
		/*case APPEND_OPEN:
			if(IsParamFile)
				return ABF_EBADPARAMETERS;
			else
				if( !ABF_AppendOpen(fileName, pFileHandle, &FH, puMaxSamples, pdwMaxEpisodes, &nError) )
					 return nError;
			break;
		*/
	}

	ConvertABFHeaderToCLHeader (&FH, pCL_ABFFH);

	//currentFileHandle = *pFileHandle;

	return 1;
}

//*************************************************************************************************
int IsABFFile(const char *fileName, int *pDataFormat, int *pnError)
{
	if (pDataFormat != 0)
		return ABF_IsABFFile(fileName, pDataFormat, pnError);
	return 0;
}

//*************************************************************************************************
int GetActualSamples (int fileHandle, unsigned int *pNumActualSamples)
{
	//if( !CheckFileHandle(fileHandle) )
	//	return 0;

	if (pNumActualSamples != 0)
		*pNumActualSamples = ABF_GetActualSamples(fileHandle);

	return 1;
}

//*************************************************************************************************
int ReadSweep(int fileHandle, const CL_ABFHeader *pCL_ABFFH, int nChannel, unsigned long dwEpisode, float *pfBuffer, unsigned int *pNumSamplesRead)
{
	int nError=0;
	ABFFileHeader FH;

	//if( !CheckFileHandle(fileHandle) )
	//	return 0;

	ConvertCLHeaderToABFHeader (pCL_ABFFH, &FH);

	if( !ABF_ReadChannel(fileHandle, &FH, nChannel, dwEpisode, pfBuffer, pNumSamplesRead, &nError) )
		return nError;

	return 1;
}

//*************************************************************************************************
int ReadMultiplexSweep (int fileHandle, const CL_ABFHeader *pCL_ABFFH, unsigned long episode, void *pvBuffer, unsigned int *pNumSamplesRead)
{
	int nError=0;
	ABFFileHeader FH;

	ConvertCLHeaderToABFHeader (pCL_ABFFH, &FH);

	if (!ABF_MultiplexRead (fileHandle, &FH, episode, pvBuffer, pNumSamplesRead, &nError))
		return nError;

	return 1;
}
//*************************************************************************************************
int ReadRawSweep (int fileHandle, const CL_ABFHeader *pCL_ABFFH, int nChannel, unsigned long episode, void *pvBuffer, unsigned int *pNumSamplesRead)
{
	int error=0;
	ABFFileHeader FH;

	ConvertCLHeaderToABFHeader (pCL_ABFFH, &FH);

	if (!ABF_ReadRawChannel (fileHandle, &FH, nChannel, episode, pvBuffer, pNumSamplesRead, &error))
		return error;

	return 1;
}
//*************************************************************************************************
int WriteMultiplexSweep(int fileHandle, const CL_ABFHeader *pCL_ABFFH, const void *pvBuffer, unsigned long dwEpiStartInSamples, unsigned int uSizeInSamples, int appendData)
{
	unsigned int uFlags=0;
	int nError=0;
	ABFFileHeader FH;

	//if( !CheckFileHandle(fileHandle) )
	//	return 0;

	ConvertCLHeaderToABFHeader (pCL_ABFFH, &FH);

	if(appendData)
		uFlags = ABF_APPEND;

	if( !ABF_MultiplexWrite(fileHandle, &FH, uFlags, pvBuffer, dwEpiStartInSamples, uSizeInSamples, &nError) )
		return nError;

	return 1;
}

//*************************************************************************************************
int WriteRawSweep(int fileHandle, const void *pvBuffer, unsigned long dwSizeInBytes)
{
	int nError=0;

	//if( !CheckFileHandle(fileHandle) )
	//	return 0;

	if( !ABF_WriteRawData(fileHandle, pvBuffer, dwSizeInBytes, &nError) )
		return nError;

	return 1;
}

//*************************************************************************************************
int WriteWaveform(int fileHandle, CL_ABFHeader *pCL_ABFFH, unsigned int uDACChannel, const short *pnDACArray)
{
	int nError=0;
	ABFFileHeader FH;

	//if( !CheckFileHandle(fileHandle) )
	//	return 0;

	ConvertCLHeaderToABFHeader (pCL_ABFFH, &FH);

	if( !ABF_WriteDACFileEpi(fileHandle, &FH, uDACChannel, pnDACArray, &nError) )
		return nError;

	ConvertABFHeaderToCLHeader (&FH, pCL_ABFFH);

	return 1;
}

//*************************************************************************************************
int UpdateABFHeader(int fileHandle, CL_ABFHeader *pCL_ABFFH)
{
	int nError=0;
	ABFFileHeader FH;

	//if( !CheckFileHandle(fileHandle) )
	//	return 0;

	ConvertCLHeaderToABFHeader(pCL_ABFFH, &FH);

	if( !ABF_UpdateHeader(fileHandle, &FH, &nError) )
		return nError;

	ConvertABFHeaderToCLHeader (&FH, pCL_ABFFH);

	return 1;
}

//*************************************************************************************************
int GetWaveform(int fileHandle, const CL_ABFHeader *pCL_ABFFH, unsigned int uDACChannel, unsigned long dwEpisode, float *pfBuffer)
{
	int nError=0;
	ABFFileHeader FH;

	//if( !CheckFileHandle( fileHandle) )
	//	return 0;

	ConvertCLHeaderToABFHeader (pCL_ABFFH, &FH);

	if( !ABF_GetWaveform(fileHandle, &FH, uDACChannel, dwEpisode, pfBuffer, &nError) )
		return nError;

	return 1;
}

//*************************************************************************************************
int CloseABFFile(int fileHandle)
{
	int nError=0;

	//if( !CheckFileHandle(fileHandle) )
	//	return 0;

	if( !ABF_Close(fileHandle, &nError) )
		return nError;

	//currentFileHandle=0;
	//ABFH_Initialize(&FH);

	return 1;
}

/*-----------------------------------------------------------------------------------------------*/
int InitializeHeader (CL_ABFHeader *pCL_ABFFH)
{
	int nError=0;
	ABFFileHeader fileHeader;

	ABFH_Initialize (&fileHeader);

	ConvertABFHeaderToCLHeader (&fileHeader, pCL_ABFFH);

	return 1;
}

/*-----------------------------------------------------------------------------------------------*/
int GetABFError (int nError, const char *fileName, char *textBuffer, UINT uMaxLen)
{
	if( !ABF_BuildErrorText(nError, fileName, textBuffer, uMaxLen))
		return 0;

	return 1;
}

/*-----------------------------------------------------------------------------------------------*/
int GetADCScaleFactor(const CL_ABFHeader *pCL_ABFFH, int nChannel, float *pfADCToUUFactor, float *fADCToUUShift)
{
	ABFFileHeader ABFFH;

	ConvertCLHeaderToABFHeader (pCL_ABFFH, &ABFFH);

	ABFH_GetADCtoUUFactors (&ABFFH, nChannel, pfADCToUUFactor, fADCToUUShift);

	return 1;
}

/*-----------------------------------------------------------------------------------------------*/
int ReadParams (int fileHandle, CL_ABFHeader *pCL_ABFFH, int *pError)
{
	ABFFileHeader fileHeader;

	ConvertCLHeaderToABFHeader (pCL_ABFFH, &fileHeader);

	if( !ABF_ParamReader(fileHandle, &fileHeader, pError))
		return 0;

	ConvertABFHeaderToCLHeader (&fileHeader, pCL_ABFFH);

	return 1;
}

/*-----------------------------------------------------------------------------------------------*/
int WriteParams (const char *pFilename, CL_ABFHeader *pCL_ABFFH, int *pError)
{
	ABFFileHeader abfFH;

	ConvertCLHeaderToABFHeader (pCL_ABFFH, &abfFH);

	if(!ABF_ParamWriter(pFilename, &abfFH, pError))
		return 0;

	return 1;
}

/*-----------------------------------------------------------------------------------------------*/
int GetEpisodeInfo (int fileHandle, const CL_ABFHeader *pCL_ABFFH, int nChannel, unsigned long dwEpisode, unsigned int *pNumSamples,
										  double *pStartTime, double *pDuration, int *pnError)
{
	ABFFileHeader FH;

	//if( !CheckFileHandle(fileHandle) )
	//	return 0;

	ConvertCLHeaderToABFHeader (pCL_ABFFH, &FH);

	if (pNumSamples)
		ABF_GetNumSamples (fileHandle, &FH, dwEpisode, pNumSamples, pnError);
	if (pStartTime)
		ABF_GetStartTime (fileHandle, &FH, nChannel, dwEpisode, pStartTime, pnError);
	if (pDuration)
		ABF_GetEpisodeDuration (fileHandle, &FH, dwEpisode, pDuration, pnError);

	return 1;
}

/*-----------------------------------------------------------------------------------------------*/
int GetTrialDuration (int fileHandle, const CL_ABFHeader *pCL_ABFFH, double *pTrialDuration, int *pnError)
{
	ABFFileHeader FH;

	//if(!CheckFileHandle(fileHandle))
	//	return 0;

	ConvertCLHeaderToABFHeader (pCL_ABFFH, &FH);

	if( !ABF_GetTrialDuration(fileHandle, &FH, pTrialDuration, pnError) )
		return 0;

	return 1;
}

//*************************************************************************************************
int ConvertCLHeaderToABFHeader(const CL_ABFHeader *pCLFH, ABFFileHeader *pABFFH)
{
	int i=0, j=0;

	// GROUP #1 - File ID and size information
	pABFFH->fFileVersionNumber = pCLFH->fFileVersionNumber;
	pABFFH->nOperationMode = pCLFH->nOperationMode;
	pABFFH->lActualAcqLength = pCLFH->lActualAcqLength;
	pABFFH->nNumPointsIgnored = pCLFH->nNumPointsIgnored;
	pABFFH->lActualEpisodes = pCLFH->lActualEpisodes;
	pABFFH->uFileStartDate = pCLFH->uFileStartDate;
	pABFFH->uFileStartTimeMS = pCLFH->uFileStartTimeMS;
	pABFFH->lStopwatchTime = pCLFH->lStopwatchTime;
	pABFFH->fHeaderVersionNumber = pCLFH->fHeaderVersionNumber;
	pABFFH->nFileType = pCLFH->nFileType;

	// GROUP #2 - File Structure
	pABFFH->lDataSectionPtr = pCLFH->lDataSectionPtr;
	pABFFH->lTagSectionPtr = pCLFH->lTagSectionPtr;
	pABFFH->lNumTagEntries = pCLFH->lNumTagEntries;
	pABFFH->lScopeConfigPtr = pCLFH->lScopeConfigPtr;
	pABFFH->lNumScopes = pCLFH->lNumScopes;
	pABFFH->lDeltaArrayPtr = pCLFH->lDeltaArrayPtr;
	pABFFH->lNumDeltas = pCLFH->lNumDeltas;
	pABFFH->lVoiceTagPtr = pCLFH->lVoiceTagPtr;
	pABFFH->lVoiceTagEntries = pCLFH->lVoiceTagEntries;
	pABFFH->lSynchArrayPtr = pCLFH->lSynchArrayPtr;
	pABFFH->lSynchArraySize = pCLFH->lSynchArraySize;
	pABFFH->nDataFormat = pCLFH->nDataFormat;
	pABFFH->nSimultaneousScan = pCLFH->nSimultaneousScan;
	pABFFH->lStatisticsConfigPtr = pCLFH->lStatisticsConfigPtr;
	pABFFH->lAnnotationSectionPtr = pCLFH->lAnnotationSectionPtr;
	pABFFH->lNumAnnotations = pCLFH->lNumAnnotations;
	for(i=0;i<ABF_DACCOUNT;++i){
		pABFFH->lDACFilePtr[i] = pCLFH->lDACFilePtr[i];
		pABFFH->lDACFileNumEpisodes[i] = pCLFH->lDACFileNumEpisodes[i];
	}

	// GROUP #3 - Trial hierarchy information
	pABFFH->nADCNumChannels = pCLFH->nADCNumChannels;
	pABFFH->fADCSequenceInterval = pCLFH->fADCSequenceInterval;
	pABFFH->uFileCompressionRatio = pCLFH->uFileCompressionRatio;
	pABFFH->bEnableFileCompression = pCLFH->bEnableFileCompression;
	pABFFH->fSynchTimeUnit = pCLFH->fSynchTimeUnit;
	pABFFH->fSecondsPerRun = pCLFH->fSecondsPerRun;
	pABFFH->lNumSamplesPerEpisode = pCLFH->lNumSamplesPerEpisode;
	pABFFH->lPreTriggerSamples = pCLFH->lPreTriggerSamples;
	pABFFH->lEpisodesPerRun = pCLFH->lEpisodesPerRun;
	pABFFH->lRunsPerTrial = pCLFH->lRunsPerTrial;
	pABFFH->lNumberOfTrials = pCLFH->lNumberOfTrials;
	pABFFH->nAveragingMode = pCLFH->nAveragingMode;
	pABFFH->nUndoRunCount = pCLFH->nUndoRunCount;
	pABFFH->nFirstEpisodeInRun = pCLFH->nFirstEpisodeInRun;
	pABFFH->fTriggerThreshold = pCLFH->fTriggerThreshold;
	pABFFH->nTriggerSource = pCLFH->nTriggerSource;
	pABFFH->nTriggerAction = pCLFH->nTriggerAction;
	pABFFH->nTriggerPolarity = pCLFH->nTriggerPolarity;
	pABFFH->fScopeOutputInterval = pCLFH->fScopeOutputInterval;
	pABFFH->fEpisodeStartToStart = pCLFH->fEpisodeStartToStart;
	pABFFH->fRunStartToStart = pCLFH->fRunStartToStart;
	pABFFH->fTrialStartToStart = pCLFH->fTrialStartToStart;
	pABFFH->lAverageCount = pCLFH->lAverageCount;
	pABFFH->nAutoTriggerStrategy = pCLFH->nAutoTriggerStrategy;
	pABFFH->fFirstRunDelayS = pCLFH->fFirstRunDelayS;

	// GROUP #4 - Display Parameters
	pABFFH->nDataDisplayMode = pCLFH->nDataDisplayMode;
	pABFFH->nChannelStatsStrategy = pCLFH->nChannelStatsStrategy;
	pABFFH->lSamplesPerTrace = pCLFH->lSamplesPerTrace;
	pABFFH->lStartDisplayNum = pCLFH->lStartDisplayNum;
	pABFFH->lFinishDisplayNum = pCLFH->lFinishDisplayNum;
	pABFFH->nShowPNRawData = pCLFH->nShowPNRawData;
	pABFFH->fStatisticsPeriod = pCLFH->fStatisticsPeriod;
	pABFFH->lStatisticsMeasurements = pCLFH->lStatisticsMeasurements;
	pABFFH->nStatisticsSaveStrategy = pCLFH->nStatisticsSaveStrategy;

	// GROUP #5 - Hardware information
	pABFFH->fADCRange = pCLFH->fADCRange;
	pABFFH->fDACRange = pCLFH->fDACRange;
	pABFFH->lADCResolution = pCLFH->lADCResolution;
	pABFFH->lDACResolution = pCLFH->lDACResolution;
	pABFFH->nDigitizerADCs = pCLFH->nDigitizerADCs;
	pABFFH->nDigitizerDACs = pCLFH->nDigitizerDACs;
	pABFFH->nDigitizerTotalDigitalOuts = pCLFH->nDigitizerTotalDigitalOuts;
	pABFFH->nDigitizerSynchDigitalOuts = pCLFH->nDigitizerSynchDigitalOuts;
	pABFFH->nDigitizerType = pCLFH->nDigitizerType;

	// GROUP #6 Environmental Information
	pABFFH->nExperimentType = pCLFH->nExperimentType;
	pABFFH->nManualInfoStrategy = pCLFH->nManualInfoStrategy;
	pABFFH->fCellID1 = pCLFH->fCellID1;
	pABFFH->fCellID2 = pCLFH->fCellID2;
	pABFFH->fCellID3 = pCLFH->fCellID3;
	strncpy(pABFFH->sProtocolPath, pCLFH->sProtocolPath, ABF_PATHLEN);
	strncpy(pABFFH->sCreatorInfo, pCLFH->sCreatorInfo, ABF_CREATORINFOLEN);
	strncpy(pABFFH->sModifierInfo, pCLFH->sModifierInfo, ABF_CREATORINFOLEN);
	pABFFH->nCommentsEnable = pCLFH->nCommentsEnable;
	strncpy(pABFFH->sFileComment, pCLFH->sFileComment, ABF_FILECOMMENTLEN);
	for(i=0;i<ABF_ADCCOUNT;++i){
		pABFFH->nTelegraphEnable[i] = pCLFH->nTelegraphEnable[i];
		pABFFH->nTelegraphInstrument[i] = pCLFH->nTelegraphInstrument[i];
		pABFFH->fTelegraphAdditGain[i] = pCLFH->fTelegraphAdditGain[i];
		pABFFH->fTelegraphFilter[i] = pCLFH->fTelegraphFilter[i];
		pABFFH->fTelegraphMembraneCap[i] = pCLFH->fTelegraphMembraneCap[i];
		pABFFH->fTelegraphAccessResistance[i] = pCLFH->fTelegraphAccessResistance[i];
		pABFFH->nTelegraphMode[i] = pCLFH->nTelegraphMode[i];
	}
	for(i=0;i<ABF_DACCOUNT;++i)
		pABFFH->nTelegraphDACScaleFactorEnable[i] = pCLFH->nTelegraphDACScaleFactorEnable[i];

	pABFFH->nAutoAnalyseEnable = pCLFH->nAutoAnalyseEnable;

	pABFFH->FileGUID = pCLFH->FileGUID;
	for(i=0;i<ABF_DACCOUNT;++i)
		pABFFH->fInstrumentHoldingLevel[i] = pCLFH->fInstrumentHoldingLevel[i];
	pABFFH->ulFileCRC = pCLFH->ulFileCRC;
	pABFFH->nCRCEnable = pCLFH->nCRCEnable;

	// GROUP #7 - Multi-channel information
	pABFFH->nSignalType = pCLFH->nSignalType;
	for(i=0;i<ABF_ADCCOUNT;++i){
		pABFFH->nADCPtoLChannelMap[i] = pCLFH->nADCPtoLChannelMap[i];
		pABFFH->nADCSamplingSeq[i] = pCLFH->nADCSamplingSeq[i];
		pABFFH->fADCProgrammableGain[i] = pCLFH->fADCProgrammableGain[i];
		pABFFH->fADCDisplayAmplification[i] = pCLFH->fADCDisplayAmplification[i];
		pABFFH->fADCDisplayOffset[i] = pCLFH->fADCDisplayOffset[i];
		pABFFH->fInstrumentScaleFactor[i] = pCLFH->fInstrumentScaleFactor[i];
		pABFFH->fInstrumentOffset[i] = pCLFH->fInstrumentOffset[i];
		pABFFH->fSignalGain[i] = pCLFH->fSignalGain[i];
		pABFFH->fSignalOffset[i] = pCLFH->fSignalOffset[i];
		pABFFH->fSignalLowpassFilter[i] = pCLFH->fSignalLowpassFilter[i];
		pABFFH->fSignalHighpassFilter[i] = pCLFH->fSignalHighpassFilter[i];
		pABFFH->nLowpassFilterType[i] = pCLFH->nLowpassFilterType[i];
		pABFFH->nHighpassFilterType[i] = pCLFH->nHighpassFilterType[i];
		pABFFH->bHumFilterEnable[i] = pCLFH->bHumFilterEnable[i];

		strncpy(pABFFH->sADCChannelName[i], pCLFH->sADCChannelName[i], ABF_ADCNAMELEN);
		strncpy(pABFFH->sADCUnits[i], pCLFH->sADCUnits[i], ABF_ADCUNITLEN);
	}
	for(i=0;i<ABF_DACCOUNT;++i){
		pABFFH->fDACScaleFactor[i] = pCLFH->fDACScaleFactor[i];
		pABFFH->fDACHoldingLevel[i] = pCLFH->fDACHoldingLevel[i];
		pABFFH->fDACCalibrationFactor[i] = pCLFH->fDACCalibrationFactor[i];
		pABFFH->fDACCalibrationOffset[i] = pCLFH->fDACCalibrationOffset[i];
		strncpy (pABFFH->sDACChannelName[i], pCLFH->sDACChannelName[i], ABF_DACNAMELEN);
		strncpy (pABFFH->sDACChannelUnits[i], pCLFH->sDACChannelUnits[i], ABF_DACUNITLEN);
	}

	// GROUP #9 - Epoch Waveform and Pulses
	pABFFH->nDigitalEnable = pCLFH->nDigitalEnable;
	pABFFH->nActiveDACChannel = pCLFH->nActiveDACChannel;
	pABFFH->nDigitalDACChannel = pCLFH->nDigitalDACChannel;
	pABFFH->nDigitalHolding = pCLFH->nDigitalHolding;
	pABFFH->nDigitalInterEpisode = pCLFH->nDigitalInterEpisode;
	pABFFH->nDigitalTrainActiveLogic = pCLFH->nDigitalTrainActiveLogic;
	for(i=0;i<ABF_EPOCHCOUNT;++i){
		pABFFH->nDigitalValue[i] = pCLFH->nDigitalValue[i];
		pABFFH->nDigitalTrainValue[i] = pCLFH->nDigitalTrainValue[i];
		pABFFH->bEpochCompression[i] = pCLFH->bEpochCompression[i];
	}
	for(i=0;i<ABF_DACCOUNT;++i){
		pABFFH->nWaveformEnable[i] = pCLFH->nWaveformEnable[i];
		pABFFH->nWaveformSource[i] = pCLFH->nWaveformSource[i];
		pABFFH->nInterEpisodeLevel[i] = pCLFH->nInterEpisodeLevel[i];
		for(j=0;j<ABF_EPOCHCOUNT;++j){
			pABFFH->nEpochType[i][j] = pCLFH->nEpochType[i][j];
			pABFFH->fEpochInitLevel[i][j] = pCLFH->fEpochInitLevel[i][j];
			pABFFH->fEpochLevelInc[i][j] = pCLFH->fEpochLevelInc[i][j];
			pABFFH->lEpochInitDuration[i][j] = pCLFH->lEpochInitDuration[i][j];
			pABFFH->lEpochDurationInc[i][j] = pCLFH->lEpochDurationInc[i][j];
		}
		pABFFH->nEpochTableRepetitions[i] = pCLFH->nEpochTableRepetitions[i];
		pABFFH->fEpochTableStartToStartInterval[i] = pCLFH->fEpochTableStartToStartInterval[i];

		// GROUP #10 - DAC Output File
    pABFFH->fDACFileScale[i] = pCLFH->fDACFileScale[i];
		pABFFH->fDACFileOffset[i] = pCLFH->fDACFileOffset[i];
		pABFFH->lDACFileEpisodeNum[i] = pCLFH->lDACFileEpisodeNum[i];
		pABFFH->nDACFileADCNum[i] = pCLFH->nDACFileADCNum[i];
		strncpy (pABFFH->sDACFilePath[i], pCLFH->sDACFilePath[i], ABF_PATHLEN);

		// GROUP #11a - Presweep (conditioning) pulse train
    pABFFH->nConditEnable[i] = pCLFH->nConditEnable[i];
		pABFFH->lConditNumPulses[i] = pCLFH->lConditNumPulses[i];
		pABFFH->fBaselineDuration[i] = pCLFH->fBaselineDuration[i];
		pABFFH->fBaselineLevel[i] = pCLFH->fBaselineLevel[i];
		pABFFH->fStepDuration[i] = pCLFH->fStepDuration[i];
		pABFFH->fStepLevel[i] = pCLFH->fStepLevel[i];
		pABFFH->fPostTrainPeriod[i] = pCLFH->fPostTrainPeriod[i];
		pABFFH->fPostTrainLevel[i] = pCLFH->fPostTrainLevel[i];
		for (j=0; j<=ABF_EPOCHCOUNT; ++j) {
			pABFFH->fCTStartLevel[i][j] = pCLFH->fCTStartLevel[i][j];
	    pABFFH->fCTEndLevel[i][j] = pCLFH->fCTEndLevel[i][j];
	    pABFFH->fCTIntervalDuration[i][j] = pCLFH->fCTIntervalDuration[i][j];
		}
		pABFFH->fCTStartToStartInterval[i] = pCLFH->fCTStartToStartInterval[i];

		// GROUP #11b - Membrane Test Between Sweeps
    pABFFH->nMembTestEnable[i] = pCLFH->nMembTestEnable[i];
		pABFFH->fMembTestPreSettlingTimeMS[i] = pCLFH->fMembTestPreSettlingTimeMS[i];
		pABFFH->fMembTestPostSettlingTimeMS[i] = pCLFH->fMembTestPostSettlingTimeMS[i];

		// GROUP #11c - PreSignal test pulse
	 pABFFH->nPreSignalEnable[i] = pCLFH->nPreSignalEnable[i];
	 pABFFH->fPreSignalPreStepDuration[i] = pCLFH->fPreSignalPreStepDuration[i];
	 pABFFH->fPreSignalPreStepLevel[i] = pCLFH->fPreSignalPreStepLevel[i];
	 pABFFH->fPreSignalStepDuration[i] = pCLFH->fPreSignalStepDuration[i];
	 pABFFH->fPreSignalStepLevel[i] = pCLFH->fPreSignalStepLevel[i];
	 pABFFH->fPreSignalPostStepDuration[i] = pCLFH->fPreSignalPostStepDuration[i];
	 pABFFH->fPreSignalPostStepLevel[i] = pCLFH->fPreSignalPostStepLevel[i];
 }

	// GROUP #12 - Variable parameter user list
	for(i=0;i<ABF_USERLISTCOUNT;++i){
		pABFFH->nULEnable[i] = pCLFH->nULEnable[i];
		pABFFH->nULParamToVary[i] = pCLFH->nULParamToVary[i];
		pABFFH->nULRepeat[i] = pCLFH->nULRepeat[i];
		strncpy(pABFFH->sULParamValueList[i], pCLFH->sULParamValueList[i], ABF_USERLISTLEN);
	}

	// GROUP #13 - Statistics measurements
	pABFFH->nStatsEnable = pCLFH->nStatsEnable;
	pABFFH->nStatsActiveChannels = pCLFH->nStatsActiveChannels;
	pABFFH->nStatsSearchRegionFlags = pCLFH->nStatsSearchRegionFlags;
	pABFFH->nStatsSmoothing = pCLFH->nStatsSmoothing;
	pABFFH->nStatsSmoothingEnable = pCLFH->nStatsSmoothingEnable;
	pABFFH->nStatsBaseline = pCLFH->nStatsBaseline;
	pABFFH->nStatsBaselineDAC = pCLFH->nStatsBaselineDAC;
	pABFFH->lStatsBaselineStart = pCLFH->lStatsBaselineStart;
	pABFFH->lStatsBaselineEnd = pCLFH->lStatsBaselineEnd;
	for(i=0;i<ABF_STATS_REGIONS;++i){
		pABFFH->lStatsMeasurements[i] = pCLFH->lStatsMeasurements[i];
		pABFFH->lStatsStart[i] = pCLFH->lStatsStart[i];
		pABFFH->lStatsEnd[i] = pCLFH->lStatsEnd[i];
		pABFFH->nRiseBottomPercentile[i] = pCLFH->nRiseBottomPercentile[i];
		pABFFH->nRiseTopPercentile[i] = pCLFH->nRiseTopPercentile[i];
		pABFFH->nDecayBottomPercentile[i] = pCLFH->nDecayBottomPercentile[i];
		pABFFH->nDecayTopPercentile[i] = pCLFH->nDecayTopPercentile[i];
		pABFFH->nStatsSearchMode[i] = pCLFH->nStatsSearchMode[i];
		pABFFH->nStatsSearchDAC[i] = pCLFH->nStatsSearchDAC[i];
	}
	for (int i=0; i<ABF_ADCCOUNT; ++i)
		pABFFH->nStatsChannelPolarity[i] = pCLFH->nStatsChannelPolarity[i];

	// GROUP #14 - Channel Arithmetic
	pABFFH->nArithmeticEnable = pCLFH->nArithmeticEnable;
	pABFFH->nArithmeticExpression = pCLFH->nArithmeticExpression;
	pABFFH->fArithmeticUpperLimit = pCLFH->fArithmeticUpperLimit;
	pABFFH->fArithmeticLowerLimit = pCLFH->fArithmeticLowerLimit;
	pABFFH->nArithmeticADCNumA = pCLFH->nArithmeticADCNumA;
	pABFFH->nArithmeticADCNumB = pCLFH->nArithmeticADCNumB;
	pABFFH->fArithmeticK1 = pCLFH->fArithmeticK1;
	pABFFH->fArithmeticK2 = pCLFH->fArithmeticK2;
	pABFFH->fArithmeticK3 = pCLFH->fArithmeticK3;
	pABFFH->fArithmeticK4 = pCLFH->fArithmeticK4;
	pABFFH->fArithmeticK5 = pCLFH->fArithmeticK5;
	pABFFH->fArithmeticK6 = pCLFH->fArithmeticK6;
	strncpy(pABFFH->sArithmeticOperator, pCLFH->sArithmeticOperator, ABF_ARITHMETICOPLEN);
	strncpy(pABFFH->sArithmeticUnits, pCLFH->sArithmeticUnits, ABF_ARITHMETICUNITSLEN);

	// GROUP #15 - Leak subtraction
	pABFFH->nPNPosition = pCLFH->nPNPosition;
	pABFFH->nPNNumPulses = pCLFH->nPNNumPulses;
	pABFFH->nPNPolarity = pCLFH->nPNPolarity;
	pABFFH->fPNSettlingTime = pCLFH->fPNSettlingTime;
	pABFFH->fPNInterpulse = pCLFH->fPNInterpulse;
	for(i=0;i<ABF_DACCOUNT;++i){
		pABFFH->nLeakSubtractType[i] = pCLFH->nLeakSubtractType[i];
		pABFFH->fPNHoldingLevel[i] = pCLFH->fPNHoldingLevel[i];
		pABFFH->nLeakSubtractADCIndex[i] = pCLFH->nLeakSubtractADCIndex[i];
	}

	// GROUP #16 - Miscellaneous variables
	pABFFH->nLevelHysteresis = pCLFH->nLevelHysteresis;
	pABFFH->lTimeHysteresis = pCLFH->lTimeHysteresis;
	pABFFH->nAllowExternalTags = pCLFH->nAllowExternalTags;
	pABFFH->nAverageAlgorithm = pCLFH->nAverageAlgorithm;
	pABFFH->fAverageWeighting = pCLFH->fAverageWeighting;
	pABFFH->nUndoPromptStrategy = pCLFH->nUndoPromptStrategy;
	pABFFH->nTrialTriggerSource = pCLFH->nTrialTriggerSource;
	pABFFH->nStatisticsDisplayStrategy = pCLFH->nStatisticsDisplayStrategy;
	pABFFH->nExternalTagType = pCLFH->nExternalTagType;
	pABFFH->lHeaderSize = pCLFH->lHeaderSize;
	pABFFH->nStatisticsClearStrategy = pCLFH->nStatisticsClearStrategy;
	pABFFH->nEnableFirstLastHolding = pCLFH->nEnableFirstLastHolding;

	// GROUP #17 - Trains parameters
	for(i=0;i<ABF_DACCOUNT;++i){
		for(j=0;j<ABF_EPOCHCOUNT;++j){
			pABFFH->lEpochPulsePeriod[i][j] = pCLFH->lEpochPulsePeriod[i][j];
			pABFFH->lEpochPulseWidth[i][j] = pCLFH->lEpochPulseWidth[i][j];
		}
	}

	// GROUP #18 - Application version data
	pABFFH->nCreatorMajorVersion = pCLFH->nCreatorMajorVersion;
	pABFFH->nCreatorMinorVersion = pCLFH->nCreatorMinorVersion;
	pABFFH->nCreatorBugfixVersion = pCLFH->nCreatorBugfixVersion;
	pABFFH->nCreatorBuildVersion = pCLFH->nCreatorBuildVersion;
	pABFFH->nModifierMajorVersion = pCLFH->nModifierMajorVersion;
	pABFFH->nModifierMinorVersion = pCLFH->nModifierMinorVersion;
	pABFFH->nModifierBugfixVersion = pCLFH->nModifierBugfixVersion;
	pABFFH->nModifierBuildVersion = pCLFH->nModifierBuildVersion;

	// GROUP #19 - LTP protocol
	pABFFH->nLTPType = pCLFH->nLTPType;
	for(i=0;i<ABF_DACCOUNT;++i){
		pABFFH->nLTPUsageOfDAC[i] = pCLFH->nLTPUsageOfDAC[i];
		pABFFH->nLTPPresynapticPulses[i] = pCLFH->nLTPPresynapticPulses[i];
	}

	// GROUP #20 - Digidata 132x Trigger out flag
	pABFFH->nScopeTriggerOut = pCLFH->nScopeTriggerOut;

	// GROUP #22 - Alternating episodic mode
	pABFFH->nAlternateDACOutputState = pCLFH->nAlternateDACOutputState;
	pABFFH->nAlternateDigitalOutputState = pCLFH->nAlternateDigitalOutputState;
	for(i=0;i<ABF_EPOCHCOUNT;++i){
		pABFFH->nAlternateDigitalValue[i] = pCLFH->nAlternateDigitalValue[i];
		pABFFH->nAlternateDigitalTrainValue[i] = pCLFH->nAlternateDigitalTrainValue[i];
	}

	// GROUP #23 - Post-processing actions
	for(i=0;i<ABF_ADCCOUNT;++i){
		pABFFH->fPostProcessLowpassFilter[i] = pCLFH->fPostProcessLowpassFilter[i];
		pABFFH->nPostProcessLowpassFilterType[i] = pCLFH->nPostProcessLowpassFilterType[i];
	}

	// GROUP #24 - Legacy gear shift info
	pABFFH->fLegacyADCSequenceInterval = pCLFH->fLegacyADCSequenceInterval;
	pABFFH->fLegacyADCSecondSequenceInterval = pCLFH->fLegacyADCSecondSequenceInterval;
	pABFFH->lLegacyClockChange = pCLFH->lLegacyClockChange;
	pABFFH->lLegacyNumSamplesPerEpisode = pCLFH->lLegacyNumSamplesPerEpisode;

return 1;
}

//*************************************************************************************************
int ConvertABFHeaderToCLHeader(const ABFFileHeader *pABFFH, CL_ABFHeader *pCLFH)
{
	int i=0, j=0;

	// GROUP #1 - File ID and size information
	pCLFH->fFileVersionNumber = pABFFH->fFileVersionNumber;
	pCLFH->nOperationMode = pABFFH->nOperationMode;
	pCLFH->lActualAcqLength = pABFFH->lActualAcqLength;
	pCLFH->nNumPointsIgnored = pABFFH->nNumPointsIgnored;
	pCLFH->lActualEpisodes = pABFFH->lActualEpisodes;
	pCLFH->uFileStartDate = pABFFH->uFileStartDate;
	pCLFH->uFileStartTimeMS = pABFFH->uFileStartTimeMS;
	pCLFH->lStopwatchTime = pABFFH->lStopwatchTime;
	pCLFH->fHeaderVersionNumber = pABFFH->fHeaderVersionNumber;
	pCLFH->nFileType = pABFFH->nFileType;

	// GROUP #2 - File Structure
	pCLFH->lDataSectionPtr = pABFFH->lDataSectionPtr;
	pCLFH->lTagSectionPtr = pABFFH->lTagSectionPtr;
	pCLFH->lNumTagEntries = pABFFH->lNumTagEntries;
	pCLFH->lScopeConfigPtr = pABFFH->lScopeConfigPtr;
	pCLFH->lNumScopes = pABFFH->lNumScopes;
	pCLFH->lDeltaArrayPtr = pABFFH->lDeltaArrayPtr;
	pCLFH->lNumDeltas = pABFFH->lNumDeltas;
	pCLFH->lVoiceTagPtr = pABFFH->lVoiceTagPtr;
	pCLFH->lVoiceTagEntries = pABFFH->lVoiceTagEntries;
	pCLFH->lSynchArrayPtr = pABFFH->lSynchArrayPtr;
	pCLFH->lSynchArraySize = pABFFH->lSynchArraySize;
	pCLFH->nDataFormat = pABFFH->nDataFormat;
	pCLFH->nSimultaneousScan = pABFFH->nSimultaneousScan;
	pCLFH->lStatisticsConfigPtr = pABFFH->lStatisticsConfigPtr;
	pCLFH->lAnnotationSectionPtr = pABFFH->lAnnotationSectionPtr;
	pCLFH->lNumAnnotations = pABFFH->lNumAnnotations;
	for(i=0;i<ABF_DACCOUNT;++i){
		pCLFH->lDACFilePtr[i] = pABFFH->lDACFilePtr[i];
		pCLFH->lDACFileNumEpisodes[i] = pABFFH->lDACFileNumEpisodes[i];
	}

	// GROUP #3 - Trial hierarchy information
	pCLFH->nADCNumChannels = pABFFH->nADCNumChannels;
	pCLFH->fADCSequenceInterval = pABFFH->fADCSequenceInterval;
	pCLFH->uFileCompressionRatio = pABFFH->uFileCompressionRatio;
	pCLFH->bEnableFileCompression = pABFFH->bEnableFileCompression;
	pCLFH->fSynchTimeUnit = pABFFH->fSynchTimeUnit;
	pCLFH->fSecondsPerRun = pABFFH->fSecondsPerRun;
	pCLFH->lNumSamplesPerEpisode = pABFFH->lNumSamplesPerEpisode;
	pCLFH->lPreTriggerSamples = pABFFH->lPreTriggerSamples;
	pCLFH->lEpisodesPerRun = pABFFH->lEpisodesPerRun;
	pCLFH->lRunsPerTrial = pABFFH->lRunsPerTrial;
	pCLFH->lNumberOfTrials = pABFFH->lNumberOfTrials;
	pCLFH->nAveragingMode = pABFFH->nAveragingMode;
	pCLFH->nUndoRunCount = pABFFH->nUndoRunCount;
	pCLFH->nFirstEpisodeInRun = pABFFH->nFirstEpisodeInRun;
	pCLFH->fTriggerThreshold = pABFFH->fTriggerThreshold;
	pCLFH->nTriggerSource = pABFFH->nTriggerSource;
	pCLFH->nTriggerAction = pABFFH->nTriggerAction;
	pCLFH->nTriggerPolarity = pABFFH->nTriggerPolarity;
	pCLFH->fScopeOutputInterval = pABFFH->fScopeOutputInterval;
	pCLFH->fEpisodeStartToStart = pABFFH->fEpisodeStartToStart;
	pCLFH->fRunStartToStart = pABFFH->fRunStartToStart;
	pCLFH->fTrialStartToStart = pABFFH->fTrialStartToStart;
	pCLFH->lAverageCount = pABFFH->lAverageCount;
	pCLFH->nAutoTriggerStrategy = pABFFH->nAutoTriggerStrategy;
	pCLFH->fFirstRunDelayS = pABFFH->fFirstRunDelayS;

	// GROUP #4 - Display Parameters
	pCLFH->nDataDisplayMode = pABFFH->nDataDisplayMode;
	pCLFH->nChannelStatsStrategy = pABFFH->nChannelStatsStrategy;
	pCLFH->lSamplesPerTrace = pABFFH->lSamplesPerTrace;
	pCLFH->lStartDisplayNum = pABFFH->lStartDisplayNum;
	pCLFH->lFinishDisplayNum = pABFFH->lFinishDisplayNum;
	pCLFH->nShowPNRawData = pABFFH->nShowPNRawData;
	pCLFH->fStatisticsPeriod = pABFFH->fStatisticsPeriod;
	pCLFH->lStatisticsMeasurements = pABFFH->lStatisticsMeasurements;
	pCLFH->nStatisticsSaveStrategy = pABFFH->nStatisticsSaveStrategy;

	// GROUP #5 - Hardware information
	pCLFH->fADCRange = pABFFH->fADCRange;
	pCLFH->fDACRange = pABFFH->fDACRange;
	pCLFH->lADCResolution = pABFFH->lADCResolution;
	pCLFH->lDACResolution = pABFFH->lDACResolution;
	pCLFH->nDigitizerADCs = pABFFH->nDigitizerADCs;
	pCLFH->nDigitizerDACs = pABFFH->nDigitizerDACs;
	pCLFH->nDigitizerTotalDigitalOuts = pABFFH->nDigitizerTotalDigitalOuts;
	pCLFH->nDigitizerSynchDigitalOuts = pABFFH->nDigitizerSynchDigitalOuts;
	pCLFH->nDigitizerType = pABFFH->nDigitizerType;

	// GROUP #6 Environmental Information
	pCLFH->nExperimentType = pABFFH->nExperimentType;
	pCLFH->nManualInfoStrategy = pABFFH->nManualInfoStrategy;
	pCLFH->fCellID1 = pABFFH->fCellID1;
	pCLFH->fCellID2 = pABFFH->fCellID2;
	pCLFH->fCellID3 = pABFFH->fCellID3;
	strncpy(pCLFH->sProtocolPath, pABFFH->sProtocolPath, ABF_PATHLEN);
	strncpy(pCLFH->sCreatorInfo, pABFFH->sCreatorInfo, ABF_CREATORINFOLEN);
	strncpy(pCLFH->sModifierInfo, pABFFH->sModifierInfo, ABF_CREATORINFOLEN);
	pCLFH->nCommentsEnable = pABFFH->nCommentsEnable;
	strncpy(pCLFH->sFileComment, pABFFH->sFileComment, ABF_FILECOMMENTLEN);
	for(i=0;i<ABF_ADCCOUNT;++i){
		pCLFH->nTelegraphEnable[i] = pABFFH->nTelegraphEnable[i];
		pCLFH->nTelegraphInstrument[i] = pABFFH->nTelegraphInstrument[i];
		pCLFH->fTelegraphAdditGain[i] = pABFFH->fTelegraphAdditGain[i];
		pCLFH->fTelegraphFilter[i] = pABFFH->fTelegraphFilter[i];
		pCLFH->fTelegraphMembraneCap[i] = pABFFH->fTelegraphMembraneCap[i];
		pCLFH->fTelegraphAccessResistance[i] = pABFFH->fTelegraphAccessResistance[i];
		pCLFH->nTelegraphMode[i] = pABFFH->nTelegraphMode[i];
	}
	for(i=0;i<ABF_DACCOUNT;++i)
		pCLFH->nTelegraphDACScaleFactorEnable[i] = pABFFH->nTelegraphDACScaleFactorEnable[i];

	pCLFH->nAutoAnalyseEnable = pABFFH->nAutoAnalyseEnable;

	pCLFH->FileGUID = pABFFH->FileGUID;
	for(i=0;i<ABF_DACCOUNT;++i)
		pCLFH->fInstrumentHoldingLevel[i] = pABFFH->fInstrumentHoldingLevel[i];
	pCLFH->ulFileCRC = pABFFH->ulFileCRC;
	pCLFH->nCRCEnable = pABFFH->nCRCEnable;

	// GROUP #7 - Multi-channel information
	pCLFH->nSignalType = pABFFH->nSignalType;
	for(i=0;i<ABF_ADCCOUNT;++i){
		pCLFH->nADCPtoLChannelMap[i] = pABFFH->nADCPtoLChannelMap[i];
		pCLFH->nADCSamplingSeq[i] = pABFFH->nADCSamplingSeq[i];
		pCLFH->fADCProgrammableGain[i] = pABFFH->fADCProgrammableGain[i];
		pCLFH->fADCDisplayAmplification[i] = pABFFH->fADCDisplayAmplification[i];
		pCLFH->fADCDisplayOffset[i] = pABFFH->fADCDisplayOffset[i];
		pCLFH->fInstrumentScaleFactor[i] = pABFFH->fInstrumentScaleFactor[i];
		pCLFH->fInstrumentOffset[i] = pABFFH->fInstrumentOffset[i];
		pCLFH->fSignalGain[i] = pABFFH->fSignalGain[i];
		pCLFH->fSignalOffset[i] = pABFFH->fSignalOffset[i];
		pCLFH->fSignalLowpassFilter[i] = pABFFH->fSignalLowpassFilter[i];
		pCLFH->fSignalHighpassFilter[i] = pABFFH->fSignalHighpassFilter[i];
		pCLFH->nLowpassFilterType[i] = pABFFH->nLowpassFilterType[i];
		pCLFH->nHighpassFilterType[i] = pABFFH->nHighpassFilterType[i];
		pCLFH->bHumFilterEnable[i] = pABFFH->bHumFilterEnable[i];

		strncpy(pCLFH->sADCChannelName[i], pABFFH->sADCChannelName[i], ABF_ADCNAMELEN);
		strncpy(pCLFH->sADCUnits[i], pABFFH->sADCUnits[i], ABF_ADCUNITLEN);
	}
	for(i=0;i<ABF_DACCOUNT;++i){
		pCLFH->fDACScaleFactor[i] = pABFFH->fDACScaleFactor[i];
		pCLFH->fDACHoldingLevel[i] = pABFFH->fDACHoldingLevel[i];
		pCLFH->fDACCalibrationFactor[i] = pABFFH->fDACCalibrationFactor[i];
		pCLFH->fDACCalibrationOffset[i] = pABFFH->fDACCalibrationOffset[i];
		strncpy (pCLFH->sDACChannelName[i], pABFFH->sDACChannelName[i], ABF_DACNAMELEN);
		strncpy (pCLFH->sDACChannelUnits[i], pABFFH->sDACChannelUnits[i], ABF_DACUNITLEN);
	}

	// GROUP #9 - Epoch Waveform and Pulses
	pCLFH->nDigitalEnable = pABFFH->nDigitalEnable;
	pCLFH->nActiveDACChannel = pABFFH->nActiveDACChannel;
	pCLFH->nDigitalDACChannel = pABFFH->nDigitalDACChannel;
	pCLFH->nDigitalHolding = pABFFH->nDigitalHolding;
	pCLFH->nDigitalInterEpisode = pABFFH->nDigitalInterEpisode;
	pCLFH->nDigitalTrainActiveLogic = pABFFH->nDigitalTrainActiveLogic;
	for(i=0;i<ABF_EPOCHCOUNT;++i){
		pCLFH->nDigitalValue[i] = pABFFH->nDigitalValue[i];
		pCLFH->nDigitalTrainValue[i] = pABFFH->nDigitalTrainValue[i];
		pCLFH->bEpochCompression[i] = pABFFH->bEpochCompression[i];
	}
	for(i=0;i<ABF_DACCOUNT;++i){
		pCLFH->nWaveformEnable[i] = pABFFH->nWaveformEnable[i];
		pCLFH->nWaveformSource[i] = pABFFH->nWaveformSource[i];
		pCLFH->nInterEpisodeLevel[i] = pABFFH->nInterEpisodeLevel[i];
		for(j=0;j<ABF_EPOCHCOUNT;++j){
			pCLFH->nEpochType[i][j] = pABFFH->nEpochType[i][j];
			pCLFH->fEpochInitLevel[i][j] = pABFFH->fEpochInitLevel[i][j];
			pCLFH->fEpochLevelInc[i][j] = pABFFH->fEpochLevelInc[i][j];
			pCLFH->lEpochInitDuration[i][j] = pABFFH->lEpochInitDuration[i][j];
			pCLFH->lEpochDurationInc[i][j] = pABFFH->lEpochDurationInc[i][j];
		}
		pCLFH->nEpochTableRepetitions[i] = pABFFH->nEpochTableRepetitions[i];
		pCLFH->fEpochTableStartToStartInterval[i] = pABFFH->fEpochTableStartToStartInterval[i];

		pCLFH->fDACFileScale[i] = pABFFH->fDACFileScale[i];
		pCLFH->fDACFileOffset[i] = pABFFH->fDACFileOffset[i];
		pCLFH->lDACFileEpisodeNum[i] = pABFFH->lDACFileEpisodeNum[i];
		pCLFH->nDACFileADCNum[i] = pABFFH->nDACFileADCNum[i];
		strncpy (pCLFH->sDACFilePath[i], pABFFH->sDACFilePath[i], ABF_PATHLEN);

		// GROUP #11a - Presweep (conditioning) pulse train
    pCLFH->nConditEnable[i] = pABFFH->nConditEnable[i];
		pCLFH->lConditNumPulses[i] = pABFFH->lConditNumPulses[i];
		pCLFH->fBaselineDuration[i] = pABFFH->fBaselineDuration[i];
		pCLFH->fBaselineLevel[i] = pABFFH->fBaselineLevel[i];
		pCLFH->fStepDuration[i] = pABFFH->fStepDuration[i];
		pCLFH->fStepLevel[i] = pABFFH->fStepLevel[i];
		pCLFH->fPostTrainPeriod[i] = pABFFH->fPostTrainPeriod[i];
		pCLFH->fPostTrainLevel[i] = pABFFH->fPostTrainLevel[i];
		for (j=0; j<ABF_EPOCHCOUNT; ++j) {
			pCLFH->fCTStartLevel[i][j] = pABFFH->fCTStartLevel[i][j];
	    pCLFH->fCTEndLevel[i][j] = pABFFH->fCTEndLevel[i][j];
	    pCLFH->fCTIntervalDuration[i][j] = pABFFH->fCTIntervalDuration[i][j];
		}
    pCLFH->fCTStartToStartInterval[i];

	  // GROUP #11b - Membrane Test Between Sweeps
	  pCLFH->nMembTestEnable[i] = pABFFH->nMembTestEnable[i];
		pCLFH->fMembTestPreSettlingTimeMS[i] = pABFFH->fMembTestPreSettlingTimeMS[i];
		pCLFH->fMembTestPostSettlingTimeMS[i] = pABFFH->fMembTestPostSettlingTimeMS[i];

		// GROUP #11c - PreSignal test pulse
   pCLFH->nPreSignalEnable[i] = pABFFH->nPreSignalEnable[i];
   pCLFH->fPreSignalPreStepDuration[i] = pABFFH->fPreSignalPreStepDuration[i];
   pCLFH->fPreSignalPreStepLevel[i] = pABFFH->fPreSignalPreStepLevel[i];
   pCLFH->fPreSignalStepDuration[i] = pABFFH->fPreSignalStepDuration[i];
   pCLFH->fPreSignalStepLevel[i] = pABFFH->fPreSignalStepLevel[i];
   pCLFH->fPreSignalPostStepDuration[i] = pABFFH->fPreSignalPostStepDuration[i];
   pCLFH->fPreSignalPostStepLevel[i] = pABFFH->fPreSignalPostStepLevel[i];
	}

	// GROUP #12 - Variable parameter user list
	for(i=0;i<ABF_USERLISTCOUNT;++i){
		pCLFH->nULEnable[i] = pABFFH->nULEnable[i];
		pCLFH->nULParamToVary[i] = pABFFH->nULParamToVary[i];
		pCLFH->nULRepeat[i] = pABFFH->nULRepeat[i];
		strncpy(pCLFH->sULParamValueList[i], pABFFH->sULParamValueList[i], ABF_USERLISTLEN);
	}

	// GROUP #13 - Statistics measurements
	pCLFH->nStatsEnable = pABFFH->nStatsEnable;
	pCLFH->nStatsActiveChannels = pABFFH->nStatsActiveChannels;
	pCLFH->nStatsSearchRegionFlags = pABFFH->nStatsSearchRegionFlags;
	pCLFH->nStatsSmoothing = pABFFH->nStatsSmoothing;
	pCLFH->nStatsSmoothingEnable = pABFFH->nStatsSmoothingEnable;
	pCLFH->nStatsBaseline = pABFFH->nStatsBaseline;
	pCLFH->nStatsBaselineDAC = pABFFH->nStatsBaselineDAC;
	pCLFH->lStatsBaselineStart = pABFFH->lStatsBaselineStart;
	pCLFH->lStatsBaselineEnd = pABFFH->lStatsBaselineEnd;
	for(i=0;i<ABF_STATS_REGIONS;++i){
		pCLFH->lStatsMeasurements[i] = pABFFH->lStatsMeasurements[i];
		pCLFH->lStatsStart[i] = pABFFH->lStatsStart[i];
		pCLFH->lStatsEnd[i] = pABFFH->lStatsEnd[i];
		pCLFH->nRiseBottomPercentile[i] = pABFFH->nRiseBottomPercentile[i];
		pCLFH->nRiseTopPercentile[i] = pABFFH->nRiseTopPercentile[i];
		pCLFH->nDecayBottomPercentile[i] = pABFFH->nDecayBottomPercentile[i];
		pCLFH->nDecayTopPercentile[i] = pABFFH->nDecayTopPercentile[i];
		pCLFH->nStatsSearchMode[i] = pABFFH->nStatsSearchMode[i];
		pCLFH->nStatsSearchDAC[i] = pABFFH->nStatsSearchDAC[i];
	}
	for (int i=0; i<ABF_ADCCOUNT; ++i)
		pCLFH->nStatsChannelPolarity[i] = pABFFH->nStatsChannelPolarity[i];

	// GROUP #14 - Channel Arithmetic
	pCLFH->nArithmeticEnable = pABFFH->nArithmeticEnable;
	pCLFH->nArithmeticExpression = pABFFH->nArithmeticExpression;
	pCLFH->fArithmeticUpperLimit = pABFFH->fArithmeticUpperLimit;
	pCLFH->fArithmeticLowerLimit = pABFFH->fArithmeticLowerLimit;
	pCLFH->nArithmeticADCNumA = pABFFH->nArithmeticADCNumA;
	pCLFH->nArithmeticADCNumB = pABFFH->nArithmeticADCNumB;
	pCLFH->fArithmeticK1 = pABFFH->fArithmeticK1;
	pCLFH->fArithmeticK2 = pABFFH->fArithmeticK2;
	pCLFH->fArithmeticK3 = pABFFH->fArithmeticK3;
	pCLFH->fArithmeticK4 = pABFFH->fArithmeticK4;
	pCLFH->fArithmeticK5 = pABFFH->fArithmeticK5;
	pCLFH->fArithmeticK6 = pABFFH->fArithmeticK6;
	strncpy(pCLFH->sArithmeticOperator, pABFFH->sArithmeticOperator, ABF_ARITHMETICOPLEN);
	strncpy(pCLFH->sArithmeticUnits, pABFFH->sArithmeticUnits, ABF_ARITHMETICUNITSLEN);

	// GROUP #15 - Leak subtraction
	pCLFH->nPNPosition = pABFFH->nPNPosition;
	pCLFH->nPNNumPulses = pABFFH->nPNNumPulses;
	pCLFH->nPNPolarity = pABFFH->nPNPolarity;
	pCLFH->fPNSettlingTime = pABFFH->fPNSettlingTime;
	pCLFH->fPNInterpulse = pABFFH->fPNInterpulse;
	for(i=0;i<ABF_DACCOUNT;++i){
		pCLFH->nLeakSubtractType[i] = pABFFH->nLeakSubtractType[i];
		pCLFH->fPNHoldingLevel[i] = pABFFH->fPNHoldingLevel[i];
		pCLFH->nLeakSubtractADCIndex[i] = pABFFH->nLeakSubtractADCIndex[i];
	}

	// GROUP #16 - Miscellaneous variables
	pCLFH->nLevelHysteresis = pABFFH->nLevelHysteresis;
	pCLFH->lTimeHysteresis = pABFFH->lTimeHysteresis;
	pCLFH->nAllowExternalTags = pABFFH->nAllowExternalTags;
	pCLFH->nAverageAlgorithm = pABFFH->nAverageAlgorithm;
	pCLFH->fAverageWeighting = pABFFH->fAverageWeighting;
	pCLFH->nUndoPromptStrategy = pABFFH->nUndoPromptStrategy;
	pCLFH->nTrialTriggerSource = pABFFH->nTrialTriggerSource;
	pCLFH->nStatisticsDisplayStrategy = pABFFH->nStatisticsDisplayStrategy;
	pCLFH->nExternalTagType = pABFFH->nExternalTagType;
	pCLFH->lHeaderSize = pABFFH->lHeaderSize;
	pCLFH->nStatisticsClearStrategy = pABFFH->nStatisticsClearStrategy;
	pCLFH->nEnableFirstLastHolding = pABFFH->nEnableFirstLastHolding;

	// GROUP #17 - Trains parameters
	for(i=0;i<ABF_DACCOUNT;++i){
		for(j=0;j<ABF_EPOCHCOUNT;++j){
			pCLFH->lEpochPulsePeriod[i][j] = pABFFH->lEpochPulsePeriod[i][j];
			pCLFH->lEpochPulseWidth[i][j] = pABFFH->lEpochPulseWidth[i][j];
		}
	}

	// GROUP #18 - Application version data
	pCLFH->nCreatorMajorVersion = pABFFH->nCreatorMajorVersion;
	pCLFH->nCreatorMinorVersion = pABFFH->nCreatorMinorVersion;
	pCLFH->nCreatorBugfixVersion = pABFFH->nCreatorBugfixVersion;
	pCLFH->nCreatorBuildVersion = pABFFH->nCreatorBuildVersion;
	pCLFH->nModifierMajorVersion = pABFFH->nModifierMajorVersion;
	pCLFH->nModifierMinorVersion = pABFFH->nModifierMinorVersion;
	pCLFH->nModifierBugfixVersion = pABFFH->nModifierBugfixVersion;
	pCLFH->nModifierBuildVersion = pABFFH->nModifierBuildVersion;

	// GROUP #19 - LTP protocol
	pCLFH->nLTPType = pABFFH->nLTPType;
	for(i=0;i<ABF_DACCOUNT;++i){
		pCLFH->nLTPUsageOfDAC[i] = pABFFH->nLTPUsageOfDAC[i];
		pCLFH->nLTPPresynapticPulses[i] = pABFFH->nLTPPresynapticPulses[i];
	}

	// GROUP #20 - Digidata 132x Trigger out flag
	pCLFH->nScopeTriggerOut = pABFFH->nScopeTriggerOut;

	// GROUP #22 - Alternating episodic mode
	pCLFH->nAlternateDACOutputState = pABFFH->nAlternateDACOutputState;
	pCLFH->nAlternateDigitalOutputState = pABFFH->nAlternateDigitalOutputState;
	for(i=0;i<ABF_EPOCHCOUNT;++i){
		pCLFH->nAlternateDigitalValue[i] = pABFFH->nAlternateDigitalValue[i];
		pCLFH->nAlternateDigitalTrainValue[i] = pABFFH->nAlternateDigitalTrainValue[i];
	}

	// GROUP #23 - Post-processing actions
	for(i=0;i<ABF_ADCCOUNT;++i){
		pCLFH->fPostProcessLowpassFilter[i] = pABFFH->fPostProcessLowpassFilter[i];
		pCLFH->nPostProcessLowpassFilterType[i] = pABFFH->nPostProcessLowpassFilterType[i];
	}

	// GROUP #24 - Legacy gear shift info
	pCLFH->fLegacyADCSequenceInterval = pABFFH->fLegacyADCSequenceInterval;
	pCLFH->fLegacyADCSecondSequenceInterval = pABFFH->fLegacyADCSecondSequenceInterval;
	pCLFH->lLegacyClockChange = pABFFH->lLegacyClockChange;
	pCLFH->lLegacyNumSamplesPerEpisode = pABFFH->lLegacyNumSamplesPerEpisode;

return 1;
}
