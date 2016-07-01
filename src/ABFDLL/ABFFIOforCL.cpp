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

	pABFFH->nDataDisplayMode = pCLFH->nDataDisplayMode;
	pABFFH->nChannelStatsStrategy = pCLFH->nChannelStatsStrategy;
	pABFFH->lSamplesPerTrace = pCLFH->lSamplesPerTrace;
	pABFFH->lStartDisplayNum = pCLFH->lStartDisplayNum;
	pABFFH->lFinishDisplayNum = pCLFH->lFinishDisplayNum;
	pABFFH->nShowPNRawData = pCLFH->nShowPNRawData;
	pABFFH->fStatisticsPeriod = pCLFH->fStatisticsPeriod;
	pABFFH->lStatisticsMeasurements = pCLFH->lStatisticsMeasurements;
	pABFFH->nStatisticsSaveStrategy = pCLFH->nStatisticsSaveStrategy;

	pABFFH->fADCRange = pCLFH->fADCRange;
	pABFFH->fDACRange = pCLFH->fDACRange;
	pABFFH->lADCResolution = pCLFH->lADCResolution;
	pABFFH->lDACResolution = pCLFH->lDACResolution;
	pABFFH->nDigitizerADCs = pCLFH->nDigitizerADCs;
	pABFFH->nDigitizerDACs = pCLFH->nDigitizerDACs;
	pABFFH->nDigitizerTotalDigitalOuts = pCLFH->nDigitizerTotalDigitalOuts;
	pABFFH->nDigitizerSynchDigitalOuts = pCLFH->nDigitizerSynchDigitalOuts;
	pABFFH->nDigitizerType = pCLFH->nDigitizerType;

	pABFFH->nExperimentType = pCLFH->nExperimentType;
	pABFFH->nManualInfoStrategy = pCLFH->nManualInfoStrategy;
	pABFFH->fCellID1 = pCLFH->fCellID1;
	pABFFH->fCellID2 = pCLFH->fCellID2;
	pABFFH->fCellID3 = pCLFH->fCellID3;
	strcpy(pABFFH->sProtocolPath, pCLFH->sProtocolPath);
	strcpy(pABFFH->sCreatorInfo, pCLFH->sCreatorInfo);
	strcpy(pABFFH->sModifierInfo, pCLFH->sModifierInfo);
	pABFFH->nCommentsEnable = pCLFH->nCommentsEnable;
	strcpy(pABFFH->sFileComment, pCLFH->sFileComment);
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


		strcpy(pABFFH->sADCChannelName[i], pCLFH->sADCChannelName[i]);
		strcpy(pABFFH->sADCUnits[i], pCLFH->sADCUnits[i]);
	}
	for(i=0;i<ABF_DACCOUNT;++i){
		pABFFH->fDACScaleFactor[i] = pCLFH->fDACScaleFactor[i];
		pABFFH->fDACHoldingLevel[i] = pCLFH->fDACHoldingLevel[i];
		pABFFH->fDACCalibrationFactor[i] = pCLFH->fDACCalibrationFactor[i];
		pABFFH->fDACCalibrationOffset[i] = pCLFH->fDACCalibrationOffset[i];
		strcpy (pABFFH->sDACChannelName[i], pCLFH->sDACChannelName[i]);
		strcpy (pABFFH->sDACChannelUnits[i], pCLFH->sDACChannelUnits[i]);
	}

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

		pABFFH->fDACFileScale[i] = pCLFH->fDACFileScale[i];
		pABFFH->fDACFileOffset[i] = pCLFH->fDACFileOffset[i];
		pABFFH->lDACFileEpisodeNum[i] = pCLFH->lDACFileEpisodeNum[i];
		pABFFH->nDACFileADCNum[i] = pCLFH->nDACFileADCNum[i];
		strcpy (pABFFH->sDACFilePath[i], pCLFH->sDACFilePath[i]);

		pABFFH->nConditEnable[i] = pCLFH->nConditEnable[i];
		pABFFH->lConditNumPulses[i] = pCLFH->lConditNumPulses[i];
		pABFFH->fBaselineDuration[i] = pCLFH->fBaselineDuration[i];
		pABFFH->fBaselineLevel[i] = pCLFH->fBaselineLevel[i];
		pABFFH->fStepDuration[i] = pCLFH->fStepDuration[i];
		pABFFH->fStepLevel[i] = pCLFH->fStepLevel[i];
		pABFFH->fPostTrainPeriod[i] = pCLFH->fPostTrainPeriod[i];
		pABFFH->fPostTrainLevel[i] = pCLFH->fPostTrainLevel[i];
		pABFFH->nMembTestEnable[i] = pCLFH->nMembTestEnable[i];
		pABFFH->fMembTestPreSettlingTimeMS[i] = pCLFH->fMembTestPreSettlingTimeMS[i];
		pABFFH->fMembTestPostSettlingTimeMS[i] = pCLFH->fMembTestPostSettlingTimeMS[i];
	}

	for(i=0;i<ABF_USERLISTCOUNT;++i){
		pABFFH->nULEnable[i] = pCLFH->nULEnable[i];
		pABFFH->nULParamToVary[i] = pCLFH->nULParamToVary[i];
		pABFFH->nULRepeat[i] = pCLFH->nULRepeat[i];
		strcpy(pABFFH->sULParamValueList[i], pCLFH->sULParamValueList[i]);
	}

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
	strcpy(pABFFH->sArithmeticOperator, pCLFH->sArithmeticOperator);
	strcpy(pABFFH->sArithmeticUnits, pCLFH->sArithmeticUnits);

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

	for(i=0;i<ABF_DACCOUNT;++i){
		for(j=0;j<ABF_EPOCHCOUNT;++j){
			pABFFH->lEpochPulsePeriod[i][j] = pCLFH->lEpochPulsePeriod[i][j];
			pABFFH->lEpochPulseWidth[i][j] = pCLFH->lEpochPulseWidth[i][j];
		}
	}

	pABFFH->nCreatorMajorVersion = pCLFH->nCreatorMajorVersion;
	pABFFH->nCreatorMinorVersion = pCLFH->nCreatorMinorVersion;
	pABFFH->nCreatorBugfixVersion = pCLFH->nCreatorBugfixVersion;
	pABFFH->nCreatorBuildVersion = pCLFH->nCreatorBuildVersion;
	pABFFH->nModifierMajorVersion = pCLFH->nModifierMajorVersion;
	pABFFH->nModifierMinorVersion = pCLFH->nModifierMinorVersion;
	pABFFH->nModifierBugfixVersion = pCLFH->nModifierBugfixVersion;
	pABFFH->nModifierBuildVersion = pCLFH->nModifierBuildVersion;

	pABFFH->nLTPType = pCLFH->nLTPType;
	for(i=0;i<ABF_DACCOUNT;++i){
		pABFFH->nLTPUsageOfDAC[i] = pCLFH->nLTPUsageOfDAC[i];
		pABFFH->nLTPPresynapticPulses[i] = pCLFH->nLTPPresynapticPulses[i];
	}

	pABFFH->nScopeTriggerOut = pCLFH->nScopeTriggerOut;

	pABFFH->nAlternateDACOutputState = pCLFH->nAlternateDACOutputState;
	pABFFH->nAlternateDigitalOutputState = pCLFH->nAlternateDigitalOutputState;
	for(i=0;i<ABF_EPOCHCOUNT;++i){
		pABFFH->nAlternateDigitalValue[i] = pCLFH->nAlternateDigitalValue[i];
		pABFFH->nAlternateDigitalTrainValue[i] = pCLFH->nAlternateDigitalTrainValue[i];
	}

	for(i=0;i<ABF_ADCCOUNT;++i){
		pABFFH->fPostProcessLowpassFilter[i] = pCLFH->fPostProcessLowpassFilter[i];
		pABFFH->nPostProcessLowpassFilterType[i] = pCLFH->nPostProcessLowpassFilterType[i];
	}

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

	pCLFH->nDataDisplayMode = pABFFH->nDataDisplayMode;
	pCLFH->nChannelStatsStrategy = pABFFH->nChannelStatsStrategy;
	pCLFH->lSamplesPerTrace = pABFFH->lSamplesPerTrace;
	pCLFH->lStartDisplayNum = pABFFH->lStartDisplayNum;
	pCLFH->lFinishDisplayNum = pABFFH->lFinishDisplayNum;
	pCLFH->nShowPNRawData = pABFFH->nShowPNRawData;
	pCLFH->fStatisticsPeriod = pABFFH->fStatisticsPeriod;
	pCLFH->lStatisticsMeasurements = pABFFH->lStatisticsMeasurements;
	pCLFH->nStatisticsSaveStrategy = pABFFH->nStatisticsSaveStrategy;

	pCLFH->fADCRange = pABFFH->fADCRange;
	pCLFH->fDACRange = pABFFH->fDACRange;
	pCLFH->lADCResolution = pABFFH->lADCResolution;
	pCLFH->lDACResolution = pABFFH->lDACResolution;
	pCLFH->nDigitizerADCs = pABFFH->nDigitizerADCs;
	pCLFH->nDigitizerDACs = pABFFH->nDigitizerDACs;
	pCLFH->nDigitizerTotalDigitalOuts = pABFFH->nDigitizerTotalDigitalOuts;
	pCLFH->nDigitizerSynchDigitalOuts = pABFFH->nDigitizerSynchDigitalOuts;
	pCLFH->nDigitizerType = pABFFH->nDigitizerType;

	pCLFH->nExperimentType = pABFFH->nExperimentType;
	pCLFH->nManualInfoStrategy = pABFFH->nManualInfoStrategy;
	pCLFH->fCellID1 = pABFFH->fCellID1;
	pCLFH->fCellID2 = pABFFH->fCellID2;
	pCLFH->fCellID3 = pABFFH->fCellID3;
	strcpy(pCLFH->sProtocolPath, pABFFH->sProtocolPath);
	strcpy(pCLFH->sCreatorInfo, pABFFH->sCreatorInfo);
	strcpy(pCLFH->sModifierInfo, pABFFH->sModifierInfo);
	pCLFH->nCommentsEnable = pABFFH->nCommentsEnable;
	strcpy(pCLFH->sFileComment, pABFFH->sFileComment);
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


		strcpy(pCLFH->sADCChannelName[i], pABFFH->sADCChannelName[i]);
		strcpy(pCLFH->sADCUnits[i], pABFFH->sADCUnits[i]);
	}
	for(i=0;i<ABF_DACCOUNT;++i){
		pCLFH->fDACScaleFactor[i] = pABFFH->fDACScaleFactor[i];
		pCLFH->fDACHoldingLevel[i] = pABFFH->fDACHoldingLevel[i];
		pCLFH->fDACCalibrationFactor[i] = pABFFH->fDACCalibrationFactor[i];
		pCLFH->fDACCalibrationOffset[i] = pABFFH->fDACCalibrationOffset[i];
		strcpy (pCLFH->sDACChannelName[i], pABFFH->sDACChannelName[i]);
		strcpy (pCLFH->sDACChannelUnits[i], pABFFH->sDACChannelUnits[i]);
	}

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

		pCLFH->fDACFileScale[i] = pABFFH->fDACFileScale[i];
		pCLFH->fDACFileOffset[i] = pABFFH->fDACFileOffset[i];
		pCLFH->lDACFileEpisodeNum[i] = pABFFH->lDACFileEpisodeNum[i];
		pCLFH->nDACFileADCNum[i] = pABFFH->nDACFileADCNum[i];
		strcpy (pCLFH->sDACFilePath[i], pABFFH->sDACFilePath[i]);

		pCLFH->nConditEnable[i] = pABFFH->nConditEnable[i];
		pCLFH->lConditNumPulses[i] = pABFFH->lConditNumPulses[i];
		pCLFH->fBaselineDuration[i] = pABFFH->fBaselineDuration[i];
		pCLFH->fBaselineLevel[i] = pABFFH->fBaselineLevel[i];
		pCLFH->fStepDuration[i] = pABFFH->fStepDuration[i];
		pCLFH->fStepLevel[i] = pABFFH->fStepLevel[i];
		pCLFH->fPostTrainPeriod[i] = pABFFH->fPostTrainPeriod[i];
		pCLFH->fPostTrainLevel[i] = pABFFH->fPostTrainLevel[i];
		pCLFH->nMembTestEnable[i] = pABFFH->nMembTestEnable[i];
		pCLFH->fMembTestPreSettlingTimeMS[i] = pABFFH->fMembTestPreSettlingTimeMS[i];
		pCLFH->fMembTestPostSettlingTimeMS[i] = pABFFH->fMembTestPostSettlingTimeMS[i];
	}

	for(i=0;i<ABF_USERLISTCOUNT;++i){
		pCLFH->nULEnable[i] = pABFFH->nULEnable[i];
		pCLFH->nULParamToVary[i] = pABFFH->nULParamToVary[i];
		pCLFH->nULRepeat[i] = pABFFH->nULRepeat[i];
		strcpy(pCLFH->sULParamValueList[i], pABFFH->sULParamValueList[i]);
	}

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
	strcpy(pCLFH->sArithmeticOperator, pABFFH->sArithmeticOperator);
	strcpy(pCLFH->sArithmeticUnits, pABFFH->sArithmeticUnits);

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

	for(i=0;i<ABF_DACCOUNT;++i){
		for(j=0;j<ABF_EPOCHCOUNT;++j){
			pCLFH->lEpochPulsePeriod[i][j] = pABFFH->lEpochPulsePeriod[i][j];
			pCLFH->lEpochPulseWidth[i][j] = pABFFH->lEpochPulseWidth[i][j];
		}
	}

	pCLFH->nCreatorMajorVersion = pABFFH->nCreatorMajorVersion;
	pCLFH->nCreatorMinorVersion = pABFFH->nCreatorMinorVersion;
	pCLFH->nCreatorBugfixVersion = pABFFH->nCreatorBugfixVersion;
	pCLFH->nCreatorBuildVersion = pABFFH->nCreatorBuildVersion;
	pCLFH->nModifierMajorVersion = pABFFH->nModifierMajorVersion;
	pCLFH->nModifierMinorVersion = pABFFH->nModifierMinorVersion;
	pCLFH->nModifierBugfixVersion = pABFFH->nModifierBugfixVersion;
	pCLFH->nModifierBuildVersion = pABFFH->nModifierBuildVersion;

	pCLFH->nLTPType = pABFFH->nLTPType;
	for(i=0;i<ABF_DACCOUNT;++i){
		pCLFH->nLTPUsageOfDAC[i] = pABFFH->nLTPUsageOfDAC[i];
		pCLFH->nLTPPresynapticPulses[i] = pABFFH->nLTPPresynapticPulses[i];
	}

	pCLFH->nScopeTriggerOut = pABFFH->nScopeTriggerOut;

	pCLFH->nAlternateDACOutputState = pABFFH->nAlternateDACOutputState;
	pCLFH->nAlternateDigitalOutputState = pABFFH->nAlternateDigitalOutputState;
	for(i=0;i<ABF_EPOCHCOUNT;++i){
		pCLFH->nAlternateDigitalValue[i] = pABFFH->nAlternateDigitalValue[i];
		pCLFH->nAlternateDigitalTrainValue[i] = pABFFH->nAlternateDigitalTrainValue[i];
	}

	for(i=0;i<ABF_ADCCOUNT;++i){
		pCLFH->fPostProcessLowpassFilter[i] = pABFFH->fPostProcessLowpassFilter[i];
		pCLFH->nPostProcessLowpassFilterType[i] = pABFFH->nPostProcessLowpassFilterType[i];
	}

	pCLFH->fLegacyADCSequenceInterval = pABFFH->fLegacyADCSequenceInterval;
	pCLFH->fLegacyADCSecondSequenceInterval = pABFFH->fLegacyADCSecondSequenceInterval;
	pCLFH->lLegacyClockChange = pABFFH->lLegacyClockChange;
	pCLFH->lLegacyNumSamplesPerEpisode = pABFFH->lLegacyNumSamplesPerEpisode;

return 1;
}

/*------------------------------------------------------------------------*/
int CopyCLHeaders (const CL_ABFHeader *pSourceFH, CL_ABFHeader *pTargetFH)
{
	int i=0, j=0;
	
	pTargetFH->fFileVersionNumber = pSourceFH->fFileVersionNumber;
	pTargetFH->nOperationMode = pSourceFH->nOperationMode;
	pTargetFH->lActualAcqLength = pSourceFH->lActualAcqLength;
	pTargetFH->nNumPointsIgnored = pSourceFH->nNumPointsIgnored;
	pTargetFH->lActualEpisodes = pSourceFH->lActualEpisodes;
	pTargetFH->uFileStartDate = pSourceFH->uFileStartDate;
	pTargetFH->uFileStartTimeMS = pSourceFH->uFileStartTimeMS;
	pTargetFH->lStopwatchTime = pSourceFH->lStopwatchTime;
	pTargetFH->fHeaderVersionNumber = pSourceFH->fHeaderVersionNumber;
	pTargetFH->nFileType = pSourceFH->nFileType;

	pTargetFH->lDataSectionPtr = pSourceFH->lDataSectionPtr;
	pTargetFH->lTagSectionPtr = pSourceFH->lTagSectionPtr;
	pTargetFH->lNumTagEntries = pSourceFH->lNumTagEntries;
	pTargetFH->lScopeConfigPtr = pSourceFH->lScopeConfigPtr;
	pTargetFH->lNumScopes = pSourceFH->lNumScopes;
	pTargetFH->lDeltaArrayPtr = pSourceFH->lDeltaArrayPtr;
	pTargetFH->lNumDeltas = pSourceFH->lNumDeltas;
	pTargetFH->lVoiceTagPtr = pSourceFH->lVoiceTagPtr;
	pTargetFH->lVoiceTagEntries = pSourceFH->lVoiceTagEntries;
	pTargetFH->lSynchArrayPtr = pSourceFH->lSynchArrayPtr;
	pTargetFH->lSynchArraySize = pSourceFH->lSynchArraySize;
	pTargetFH->nDataFormat = pSourceFH->nDataFormat;
	pTargetFH->nSimultaneousScan = pSourceFH->nSimultaneousScan;
	pTargetFH->lStatisticsConfigPtr = pSourceFH->lStatisticsConfigPtr;
	pTargetFH->lAnnotationSectionPtr = pSourceFH->lAnnotationSectionPtr;
	pTargetFH->lNumAnnotations = pSourceFH->lNumAnnotations;
	for(i=0;i<ABF_DACCOUNT;++i){
		pTargetFH->lDACFilePtr[i] = pSourceFH->lDACFilePtr[i];
		pTargetFH->lDACFileNumEpisodes[i] = pSourceFH->lDACFileNumEpisodes[i];
	}

	pTargetFH->nADCNumChannels = pSourceFH->nADCNumChannels;
	pTargetFH->fADCSequenceInterval = pSourceFH->fADCSequenceInterval;
	pTargetFH->uFileCompressionRatio = pSourceFH->uFileCompressionRatio;
	pTargetFH->bEnableFileCompression = pSourceFH->bEnableFileCompression;
	pTargetFH->fSynchTimeUnit = pSourceFH->fSynchTimeUnit;
	pTargetFH->fSecondsPerRun = pSourceFH->fSecondsPerRun;
	pTargetFH->lNumSamplesPerEpisode = pSourceFH->lNumSamplesPerEpisode;
	pTargetFH->lPreTriggerSamples = pSourceFH->lPreTriggerSamples;
	pTargetFH->lEpisodesPerRun = pSourceFH->lEpisodesPerRun;
	pTargetFH->lRunsPerTrial = pSourceFH->lRunsPerTrial;
	pTargetFH->lNumberOfTrials = pSourceFH->lNumberOfTrials;
	pTargetFH->nAveragingMode = pSourceFH->nAveragingMode;
	pTargetFH->nUndoRunCount = pSourceFH->nUndoRunCount;
	pTargetFH->nFirstEpisodeInRun = pSourceFH->nFirstEpisodeInRun;
	pTargetFH->fTriggerThreshold = pSourceFH->fTriggerThreshold;
	pTargetFH->nTriggerSource = pSourceFH->nTriggerSource;
	pTargetFH->nTriggerAction = pSourceFH->nTriggerAction;
	pTargetFH->nTriggerPolarity = pSourceFH->nTriggerPolarity;
	pTargetFH->fScopeOutputInterval = pSourceFH->fScopeOutputInterval;
	pTargetFH->fEpisodeStartToStart = pSourceFH->fEpisodeStartToStart;
	pTargetFH->fRunStartToStart = pSourceFH->fRunStartToStart;
	pTargetFH->fTrialStartToStart = pSourceFH->fTrialStartToStart;
	pTargetFH->lAverageCount = pSourceFH->lAverageCount;
	pTargetFH->nAutoTriggerStrategy = pSourceFH->nAutoTriggerStrategy;
	pTargetFH->fFirstRunDelayS = pSourceFH->fFirstRunDelayS;

	pTargetFH->nDataDisplayMode = pSourceFH->nDataDisplayMode;
	pTargetFH->nChannelStatsStrategy = pSourceFH->nChannelStatsStrategy;
	pTargetFH->lSamplesPerTrace = pSourceFH->lSamplesPerTrace;
	pTargetFH->lStartDisplayNum = pSourceFH->lStartDisplayNum;
	pTargetFH->lFinishDisplayNum = pSourceFH->lFinishDisplayNum;
	pTargetFH->nShowPNRawData = pSourceFH->nShowPNRawData;
	pTargetFH->fStatisticsPeriod = pSourceFH->fStatisticsPeriod;
	pTargetFH->lStatisticsMeasurements = pSourceFH->lStatisticsMeasurements;
	pTargetFH->nStatisticsSaveStrategy = pSourceFH->nStatisticsSaveStrategy;

	pTargetFH->fADCRange = pSourceFH->fADCRange;
	pTargetFH->fDACRange = pSourceFH->fDACRange;
	pTargetFH->lADCResolution = pSourceFH->lADCResolution;
	pTargetFH->lDACResolution = pSourceFH->lDACResolution;
	pTargetFH->nDigitizerADCs = pSourceFH->nDigitizerADCs;
	pTargetFH->nDigitizerDACs = pSourceFH->nDigitizerDACs;
	pTargetFH->nDigitizerTotalDigitalOuts = pSourceFH->nDigitizerTotalDigitalOuts;
	pTargetFH->nDigitizerSynchDigitalOuts = pSourceFH->nDigitizerSynchDigitalOuts;
	pTargetFH->nDigitizerType = pSourceFH->nDigitizerType;

	pTargetFH->nExperimentType = pSourceFH->nExperimentType;
	pTargetFH->nManualInfoStrategy = pSourceFH->nManualInfoStrategy;
	pTargetFH->fCellID1 = pSourceFH->fCellID1;
	pTargetFH->fCellID2 = pSourceFH->fCellID2;
	pTargetFH->fCellID3 = pSourceFH->fCellID3;
	strcpy(pTargetFH->sProtocolPath, pSourceFH->sProtocolPath);
	strcpy(pTargetFH->sCreatorInfo, pSourceFH->sCreatorInfo);
	strcpy(pTargetFH->sModifierInfo, pSourceFH->sModifierInfo);
	pTargetFH->nCommentsEnable = pSourceFH->nCommentsEnable;
	strcpy(pTargetFH->sFileComment, pSourceFH->sFileComment);
	for(i=0;i<ABF_ADCCOUNT;++i){
		pTargetFH->nTelegraphEnable[i] = pSourceFH->nTelegraphEnable[i];
		pTargetFH->nTelegraphInstrument[i] = pSourceFH->nTelegraphInstrument[i];
		pTargetFH->fTelegraphAdditGain[i] = pSourceFH->fTelegraphAdditGain[i];
		pTargetFH->fTelegraphFilter[i] = pSourceFH->fTelegraphFilter[i];
		pTargetFH->fTelegraphMembraneCap[i] = pSourceFH->fTelegraphMembraneCap[i];
		pTargetFH->fTelegraphAccessResistance[i] = pSourceFH->fTelegraphAccessResistance[i];
		pTargetFH->nTelegraphMode[i] = pSourceFH->nTelegraphMode[i];
	}
	for(i=0;i<ABF_DACCOUNT;++i)
		pTargetFH->nTelegraphDACScaleFactorEnable[i] = pSourceFH->nTelegraphDACScaleFactorEnable[i];

	pTargetFH->nAutoAnalyseEnable = pSourceFH->nAutoAnalyseEnable;

	pTargetFH->FileGUID = pSourceFH->FileGUID;
	for(i=0;i<ABF_DACCOUNT;++i)
		pTargetFH->fInstrumentHoldingLevel[i] = pSourceFH->fInstrumentHoldingLevel[i];
	pTargetFH->ulFileCRC = pSourceFH->ulFileCRC;
	pTargetFH->nCRCEnable = pSourceFH->nCRCEnable;

	pTargetFH->nSignalType = pSourceFH->nSignalType;
	for(i=0;i<ABF_ADCCOUNT;++i){
		pTargetFH->nADCPtoLChannelMap[i] = pSourceFH->nADCPtoLChannelMap[i];
		pTargetFH->nADCSamplingSeq[i] = pSourceFH->nADCSamplingSeq[i];
		pTargetFH->fADCProgrammableGain[i] = pSourceFH->fADCProgrammableGain[i];
		pTargetFH->fADCDisplayAmplification[i] = pSourceFH->fADCDisplayAmplification[i];
		pTargetFH->fADCDisplayOffset[i] = pSourceFH->fADCDisplayOffset[i];
		pTargetFH->fInstrumentScaleFactor[i] = pSourceFH->fInstrumentScaleFactor[i];
		pTargetFH->fInstrumentOffset[i] = pSourceFH->fInstrumentOffset[i];
		pTargetFH->fSignalGain[i] = pSourceFH->fSignalGain[i];
		pTargetFH->fSignalOffset[i] = pSourceFH->fSignalOffset[i];
		pTargetFH->fSignalLowpassFilter[i] = pSourceFH->fSignalLowpassFilter[i];
		pTargetFH->fSignalHighpassFilter[i] = pSourceFH->fSignalHighpassFilter[i];
		pTargetFH->nLowpassFilterType[i] = pSourceFH->nLowpassFilterType[i];
		pTargetFH->nHighpassFilterType[i] = pSourceFH->nHighpassFilterType[i];


		strcpy(pTargetFH->sADCChannelName[i], pSourceFH->sADCChannelName[i]);
		strcpy(pTargetFH->sADCUnits[i], pSourceFH->sADCUnits[i]);
	}
	for(i=0;i<ABF_DACCOUNT;++i){
		pTargetFH->fDACScaleFactor[i] = pSourceFH->fDACScaleFactor[i];
		pTargetFH->fDACHoldingLevel[i] = pSourceFH->fDACHoldingLevel[i];
		pTargetFH->fDACCalibrationFactor[i] = pSourceFH->fDACCalibrationFactor[i];
		pTargetFH->fDACCalibrationOffset[i] = pSourceFH->fDACCalibrationOffset[i];
		strcpy (pTargetFH->sDACChannelName[i], pSourceFH->sDACChannelName[i]);
		strcpy (pTargetFH->sDACChannelUnits[i], pSourceFH->sDACChannelUnits[i]);
	}

	pTargetFH->nDigitalEnable = pSourceFH->nDigitalEnable;
	pTargetFH->nActiveDACChannel = pSourceFH->nActiveDACChannel;
	pTargetFH->nDigitalDACChannel = pSourceFH->nDigitalDACChannel;
	pTargetFH->nDigitalHolding = pSourceFH->nDigitalHolding;
	pTargetFH->nDigitalInterEpisode = pSourceFH->nDigitalInterEpisode;
	pTargetFH->nDigitalTrainActiveLogic = pSourceFH->nDigitalTrainActiveLogic;
	for(i=0;i<ABF_EPOCHCOUNT;++i){
		pTargetFH->nDigitalValue[i] = pSourceFH->nDigitalValue[i];
		pTargetFH->nDigitalTrainValue[i] = pSourceFH->nDigitalTrainValue[i];
		pTargetFH->bEpochCompression[i] = pSourceFH->bEpochCompression[i];
	}
	for(i=0;i<ABF_DACCOUNT;++i){
		pTargetFH->nWaveformEnable[i] = pSourceFH->nWaveformEnable[i];
		pTargetFH->nWaveformSource[i] = pSourceFH->nWaveformSource[i];
		pTargetFH->nInterEpisodeLevel[i] = pSourceFH->nInterEpisodeLevel[i];
		for(j=0;j<ABF_EPOCHCOUNT;++j){
			pTargetFH->nEpochType[i][j] = pSourceFH->nEpochType[i][j];
			pTargetFH->fEpochInitLevel[i][j] = pSourceFH->fEpochInitLevel[i][j];
			pTargetFH->fEpochLevelInc[i][j] = pSourceFH->fEpochLevelInc[i][j];
			pTargetFH->lEpochInitDuration[i][j] = pSourceFH->lEpochInitDuration[i][j];
			pTargetFH->lEpochDurationInc[i][j] = pSourceFH->lEpochDurationInc[i][j];
		}

		pTargetFH->fDACFileScale[i] = pSourceFH->fDACFileScale[i];
		pTargetFH->fDACFileOffset[i] = pSourceFH->fDACFileOffset[i];
		pTargetFH->lDACFileEpisodeNum[i] = pSourceFH->lDACFileEpisodeNum[i];
		pTargetFH->nDACFileADCNum[i] = pSourceFH->nDACFileADCNum[i];
		strcpy (pTargetFH->sDACFilePath[i], pSourceFH->sDACFilePath[i]);

		pTargetFH->nConditEnable[i] = pSourceFH->nConditEnable[i];
		pTargetFH->lConditNumPulses[i] = pSourceFH->lConditNumPulses[i];
		pTargetFH->fBaselineDuration[i] = pSourceFH->fBaselineDuration[i];
		pTargetFH->fBaselineLevel[i] = pSourceFH->fBaselineLevel[i];
		pTargetFH->fStepDuration[i] = pSourceFH->fStepDuration[i];
		pTargetFH->fStepLevel[i] = pSourceFH->fStepLevel[i];
		pTargetFH->fPostTrainPeriod[i] = pSourceFH->fPostTrainPeriod[i];
		pTargetFH->fPostTrainLevel[i] = pSourceFH->fPostTrainLevel[i];
		pTargetFH->nMembTestEnable[i] = pSourceFH->nMembTestEnable[i];
		pTargetFH->fMembTestPreSettlingTimeMS[i] = pSourceFH->fMembTestPreSettlingTimeMS[i];
		pTargetFH->fMembTestPostSettlingTimeMS[i] = pSourceFH->fMembTestPostSettlingTimeMS[i];
	}

	for(i=0;i<ABF_USERLISTCOUNT;++i){
		pTargetFH->nULEnable[i] = pSourceFH->nULEnable[i];
		pTargetFH->nULParamToVary[i] = pSourceFH->nULParamToVary[i];
		pTargetFH->nULRepeat[i] = pSourceFH->nULRepeat[i];
		strcpy(pTargetFH->sULParamValueList[i], pSourceFH->sULParamValueList[i]);
	}

	pTargetFH->nStatsEnable = pSourceFH->nStatsEnable;
	pTargetFH->nStatsActiveChannels = pSourceFH->nStatsActiveChannels;
	pTargetFH->nStatsSearchRegionFlags = pSourceFH->nStatsSearchRegionFlags;
	pTargetFH->nStatsSmoothing = pSourceFH->nStatsSmoothing;
	pTargetFH->nStatsSmoothingEnable = pSourceFH->nStatsSmoothingEnable;
	pTargetFH->nStatsBaseline = pSourceFH->nStatsBaseline;
	pTargetFH->nStatsBaselineDAC = pSourceFH->nStatsBaselineDAC;
	pTargetFH->lStatsBaselineStart = pSourceFH->lStatsBaselineStart;
	pTargetFH->lStatsBaselineEnd = pSourceFH->lStatsBaselineEnd;
	for(i=0;i<ABF_STATS_REGIONS;++i){
		pTargetFH->lStatsMeasurements[i] = pSourceFH->lStatsMeasurements[i];
		pTargetFH->lStatsStart[i] = pSourceFH->lStatsStart[i];
		pTargetFH->lStatsEnd[i] = pSourceFH->lStatsEnd[i];
		pTargetFH->nRiseBottomPercentile[i] = pSourceFH->nRiseBottomPercentile[i];
		pTargetFH->nRiseTopPercentile[i] = pSourceFH->nRiseTopPercentile[i];
		pTargetFH->nDecayBottomPercentile[i] = pSourceFH->nDecayBottomPercentile[i];
		pTargetFH->nDecayTopPercentile[i] = pSourceFH->nDecayTopPercentile[i];
		pTargetFH->nStatsSearchMode[i] = pSourceFH->nStatsSearchMode[i];
		pTargetFH->nStatsSearchDAC[i] = pSourceFH->nStatsSearchDAC[i];
	}
	for (i=0; i<ABF_ADCCOUNT; ++i)
		pTargetFH->nStatsChannelPolarity[i] = pSourceFH->nStatsChannelPolarity[i];

	pTargetFH->nArithmeticEnable = pSourceFH->nArithmeticEnable;
	pTargetFH->nArithmeticExpression = pSourceFH->nArithmeticExpression;
	pTargetFH->fArithmeticUpperLimit = pSourceFH->fArithmeticUpperLimit;
	pTargetFH->fArithmeticLowerLimit = pSourceFH->fArithmeticLowerLimit;
	pTargetFH->nArithmeticADCNumA = pSourceFH->nArithmeticADCNumA;
	pTargetFH->nArithmeticADCNumB = pSourceFH->nArithmeticADCNumB;
	pTargetFH->fArithmeticK1 = pSourceFH->fArithmeticK1;
	pTargetFH->fArithmeticK2 = pSourceFH->fArithmeticK2;
	pTargetFH->fArithmeticK3 = pSourceFH->fArithmeticK3;
	pTargetFH->fArithmeticK4 = pSourceFH->fArithmeticK4;
	pTargetFH->fArithmeticK5 = pSourceFH->fArithmeticK5;
	pTargetFH->fArithmeticK6 = pSourceFH->fArithmeticK6;
	strcpy(pTargetFH->sArithmeticOperator, pSourceFH->sArithmeticOperator);
	strcpy(pTargetFH->sArithmeticUnits, pSourceFH->sArithmeticUnits);

	pTargetFH->nPNPosition = pSourceFH->nPNPosition;
	pTargetFH->nPNNumPulses = pSourceFH->nPNNumPulses;
	pTargetFH->nPNPolarity = pSourceFH->nPNPolarity;
	pTargetFH->fPNSettlingTime = pSourceFH->fPNSettlingTime;
	pTargetFH->fPNInterpulse = pSourceFH->fPNInterpulse;
	for(i=0;i<ABF_DACCOUNT;++i){
		pTargetFH->nLeakSubtractType[i] = pSourceFH->nLeakSubtractType[i];
		pTargetFH->fPNHoldingLevel[i] = pSourceFH->fPNHoldingLevel[i];
		pTargetFH->nLeakSubtractADCIndex[i] = pSourceFH->nLeakSubtractADCIndex[i];
	}

	pTargetFH->nLevelHysteresis = pSourceFH->nLevelHysteresis;
	pTargetFH->lTimeHysteresis = pSourceFH->lTimeHysteresis;
	pTargetFH->nAllowExternalTags = pSourceFH->nAllowExternalTags;
	pTargetFH->nAverageAlgorithm = pSourceFH->nAverageAlgorithm;
	pTargetFH->fAverageWeighting = pSourceFH->fAverageWeighting;
	pTargetFH->nUndoPromptStrategy = pSourceFH->nUndoPromptStrategy;
	pTargetFH->nTrialTriggerSource = pSourceFH->nTrialTriggerSource;
	pTargetFH->nStatisticsDisplayStrategy = pSourceFH->nStatisticsDisplayStrategy;
	pTargetFH->nExternalTagType = pSourceFH->nExternalTagType;
	pTargetFH->lHeaderSize = pSourceFH->lHeaderSize;
	pTargetFH->nStatisticsClearStrategy = pSourceFH->nStatisticsClearStrategy;

	for(i=0;i<ABF_DACCOUNT;++i){
		for(j=0;j<ABF_EPOCHCOUNT;++j){
			pTargetFH->lEpochPulsePeriod[i][j] = pSourceFH->lEpochPulsePeriod[i][j];
			pTargetFH->lEpochPulseWidth[i][j] = pSourceFH->lEpochPulseWidth[i][j];
		}
	}

	pTargetFH->nCreatorMajorVersion = pSourceFH->nCreatorMajorVersion;
	pTargetFH->nCreatorMinorVersion = pSourceFH->nCreatorMinorVersion;
	pTargetFH->nCreatorBugfixVersion = pSourceFH->nCreatorBugfixVersion;
	pTargetFH->nCreatorBuildVersion = pSourceFH->nCreatorBuildVersion;
	pTargetFH->nModifierMajorVersion = pSourceFH->nModifierMajorVersion;
	pTargetFH->nModifierMinorVersion = pSourceFH->nModifierMinorVersion;
	pTargetFH->nModifierBugfixVersion = pSourceFH->nModifierBugfixVersion;
	pTargetFH->nModifierBuildVersion = pSourceFH->nModifierBuildVersion;

	pTargetFH->nLTPType = pSourceFH->nLTPType;
	for(i=0;i<ABF_DACCOUNT;++i){
		pTargetFH->nLTPUsageOfDAC[i] = pSourceFH->nLTPUsageOfDAC[i];
		pTargetFH->nLTPPresynapticPulses[i] = pSourceFH->nLTPPresynapticPulses[i];
	}

	pTargetFH->nScopeTriggerOut = pSourceFH->nScopeTriggerOut;

	pTargetFH->nAlternateDACOutputState = pSourceFH->nAlternateDACOutputState;
	pTargetFH->nAlternateDigitalOutputState = pSourceFH->nAlternateDigitalOutputState;
	for(i=0;i<ABF_EPOCHCOUNT;++i){
		pTargetFH->nAlternateDigitalValue[i] = pSourceFH->nAlternateDigitalValue[i];
		pTargetFH->nAlternateDigitalTrainValue[i] = pSourceFH->nAlternateDigitalTrainValue[i];
	}

	for(i=0;i<ABF_ADCCOUNT;++i){
		pTargetFH->fPostProcessLowpassFilter[i] = pSourceFH->fPostProcessLowpassFilter[i];
		pTargetFH->nPostProcessLowpassFilterType[i] = pSourceFH->nPostProcessLowpassFilterType[i];
	}

	pTargetFH->fLegacyADCSequenceInterval = pSourceFH->fLegacyADCSequenceInterval;
	pTargetFH->fLegacyADCSecondSequenceInterval = pSourceFH->fLegacyADCSecondSequenceInterval;
	pTargetFH->lLegacyClockChange = pSourceFH->lLegacyClockChange;
	pTargetFH->lLegacyNumSamplesPerEpisode = pSourceFH->lLegacyNumSamplesPerEpisode;

return 1;
}
