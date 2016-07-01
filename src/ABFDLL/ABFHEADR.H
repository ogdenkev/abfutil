//***********************************************************************************************
//
//    Copyright (c) 1993-2004 Molecular Devices Corporation.
//    All rights reserved.
//    Permission is granted to freely use, modify and copy the code in this file.
//
//***********************************************************************************************
// HEADER:  ABFHEADR.H.
// PURPOSE: Defines the ABFFileHeader structure, and provides prototypes for
//          functions implemented in ABFHEADR.CPP for reading and writing
//          ABFFileHeader's.
// REVISIONS:
//   2.0  - This version separates the data in the file from the struct passed around within the application.

#ifndef INC_ABFHEADR2_H
#define INC_ABFHEADR2_H

//#include "ABFFIO.h"
//#include "\AxonDev\Comp\AxABFFIO32\ABFHeadr.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ABF_CONSTANTS
#include "ABFConstants.h"
#endif

//
// pack structure on byte boundaries
//
#ifndef RC_INVOKED
#pragma pack(push, 1)
#endif

//
// Definition of the ABF header structure.
//
struct ABFFileHeader
{
public:
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
   float    fEpochLevelInc[ABF_DACCOUNT][ABF_EPOCHCOUNT];
   long     lEpochInitDuration[ABF_DACCOUNT][ABF_EPOCHCOUNT];
   long     lEpochDurationInc[ABF_DACCOUNT][ABF_EPOCHCOUNT];

   // GROUP #10 - DAC Output File
   float    fDACFileScale[ABF_DACCOUNT];
   float    fDACFileOffset[ABF_DACCOUNT];
   long     lDACFileEpisodeNum[ABF_DACCOUNT];
   short    nDACFileADCNum[ABF_DACCOUNT];
   char     sDACFilePath[ABF_DACCOUNT][ABF_PATHLEN];

   // GROUP #11 - Presweep (conditioning) pulse train
   short    nConditEnable[ABF_DACCOUNT];
   long     lConditNumPulses[ABF_DACCOUNT];
   float    fBaselineDuration[ABF_DACCOUNT];
   float    fBaselineLevel[ABF_DACCOUNT];
   float    fStepDuration[ABF_DACCOUNT];
   float    fStepLevel[ABF_DACCOUNT];
   float    fPostTrainPeriod[ABF_DACCOUNT];
   float    fPostTrainLevel[ABF_DACCOUNT];
   short    nMembTestEnable[ABF_DACCOUNT];
   float    fMembTestPreSettlingTimeMS[ABF_DACCOUNT];
   float    fMembTestPostSettlingTimeMS[ABF_DACCOUNT];

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

   ABFFileHeader();
};   

inline ABFFileHeader::ABFFileHeader()
{
   // Set everything to 0.
   memset( this, 0, sizeof(ABFFileHeader) );
   
   // Set critical parameters so we can determine the version.
   fFileVersionNumber   = ABF_CURRENTVERSION;
   fHeaderVersionNumber = ABF_CURRENTVERSION;
   lHeaderSize          = sizeof(ABFFileHeader);
}

//
// Scope descriptor format.
//
#define ABF_FACESIZE 32
struct ABFLogFont
{
   short nHeight;                // Height of the font in pixels.
//   short lWidth;               // use 0
//   short lEscapement;          // use 0
//   short lOrientation;         // use 0
   short nWeight;                // MSWindows font weight value.
//   char bItalic;               // use 0
//   char bUnderline;            // use 0
//   char bStrikeOut;            // use 0
//   char cCharSet;              // use ANSI_CHARSET (0)
//   char cOutPrecision;         // use OUT_TT_PRECIS
//   char cClipPrecision;        // use CLIP_DEFAULT_PRECIS
//   char cQuality;              // use PROOF_QUALITY
   char cPitchAndFamily;         // MSWindows pitch and family mask.
   char Unused[3];               // Unused space to maintain 4-byte packing.
   char szFaceName[ABF_FACESIZE];// Face name of the font.
};     // Size = 40

struct ABFSignal
{
   char     szName[ABF_ADCNAMELEN+2];        // ABF name length + '\0' + 1 for alignment.
   short    nMxOffset;                       // Offset of the signal in the sampling sequence.
   DWORD    rgbColor;                        // Pen color used to draw trace.
   char     nPenWidth;                       // Pen width in pixels.
   char     bDrawPoints;                     // TRUE = Draw disconnected points
   char     bHidden;                         // TRUE = Hide the trace.
   char     bFloatData;                      // TRUE = Floating point pseudo channel
   float    fVertProportion;                 // Relative proportion of client area to use
   float    fDisplayGain;                    // Display gain of trace in UserUnits
   float    fDisplayOffset;                  // Display offset of trace in UserUnits

//   float    fUUTop;                          // Top of window in UserUnits
//   float    fUUBottom;                       // Bottom of window in UserUnits
};      // Size = 34

struct ABFScopeConfig
{
   // Section 1 scope configurations
   DWORD       dwFlags;                   // Flags that are meaningful to the scope.
   DWORD       rgbColor[ABF_SCOPECOLORS]; // Colors for the components of the scope.
   float       fDisplayStart;             // Start of the display area in ms.
   float       fDisplayEnd;               // End of the display area in ms.
   WORD        wScopeMode;                // Mode that the scope is in.
   char        bMaximized;                // TRUE = Scope parent is maximized.
   char        bMinimized;                // TRUE = Scope parent is minimized.
   short       xLeft;                     // Coordinate of the left edge.
   short       yTop;                      // Coordinate of the top edge.
   short       xRight;                    // Coordinate of the right edge.
   short       yBottom;                   // Coordinate of the bottom edge.
   ABFLogFont  LogFont;                   // Description of current font.
   ABFSignal   TraceList[ABF_ADCCOUNT];   // List of traces in current use.
   short       nYAxisWidth;               // Width of the YAxis region.
   short       nTraceCount;               // Number of traces described in TraceList.
   short       nEraseStrategy;            // Erase strategy.
   short       nDockState;                // Docked position.
   // Size 656
   // * Do not insert any new members above this point! *
   // Section 2 scope configurations for file version 1.68.
   short       nSizeofOldStructure;              // Unused byte to determine the offset of the version 2 data.
   DWORD       rgbColorEx[ ABF_SCOPECOLORS_EX ]; // New color settings for stored sweep and cursors.
   short       nAutoZeroState;                   // Status of the autozero selection.
   DWORD       dwCursorsVisibleState;            // Flag for visible status of cursors.
   DWORD       dwCursorsLockedState;             // Flag for enabled status of cursors.
   char        sUnasigned[61];
   // Size 113
   ABFScopeConfig();
}; // Size = 769


inline ABFScopeConfig::ABFScopeConfig()
{
   // Set everything to 0.
   memset( this, 0, sizeof(ABFScopeConfig) );
   
   // Set critical parameters so we can determine the version.
   nSizeofOldStructure = 656;
}

//
// Definition of the ABF Tag structure
//
struct ABFTag
{
   long    lTagTime;          // Time at which the tag was entered in fSynchTimeUnit units.
   char    sComment[ABF_TAGCOMMENTLEN];   // Optional tag comment.
   short   nTagType;          // Type of tag ABF_TIMETAG, ABF_COMMENTTAG, ABF_EXTERNALTAG, ABF_VOICETAG, ABF_NEWFILETAG or ABF_ANNOTATIONTAG
   union 
   {
      short   nVoiceTagNumber;   // If nTagType=ABF_VOICETAG, this is the number of this voice tag.
      short   nAnnotationIndex;  // If nTagType=ABF_ANNOTATIONTAG, this is the index of the corresponding annotation.
   };
}; // Size = 64

//
// Definition of the ABFVoiceTagInfo structure.
//
struct ABFVoiceTagInfo
{
   long  lTagNumber;          // The tag number that corresponds to this VoiceTag
   long  lFileOffset;         // Offset to this tag within the VoiceTag block
   long  lUncompressedSize;   // Size of the voice tag expanded.
   long  lCompressedSize;     // Compressed size of the tag.
   short nCompressionType;    // Compression method used.
   short nSampleSize;         // Size of the samples acquired.
   long  lSamplesPerSecond;   // Rate at which the sound was acquired.
   DWORD dwCRC;               // CRC used to check data integrity.
   WORD  wChannels;           // Number of channels in the tag (usually 1).
   WORD  wUnused;             // Unused space.
}; // Size 32

//
// Definition of the ABF Delta structure.
//
struct ABFDelta
{
   long    lDeltaTime;        // Time at which the parameter was changed in fSynchTimeUnit units.
   long    lParameterID;      // Identifier for the parameter changed
   union
   {
      long  lNewParamValue;   // Depending on the value of lParameterID
      float fNewParamValue;   // this entry may be either a float or a long.
   };
}; // Size = 12

//
// Definition of the ABF synch array structure
//
struct ABFSynch
{
   long    lStart;            // Start of the episode/event in fSynchTimeUnit units.
   long    lLength;           // Length of the episode/event in multiplexed samples.
}; // Size = 8

#ifndef RC_INVOKED
#pragma pack(pop)                      // return to default packing
#endif

// ============================================================================================
// Function prototypes for functions in ABFHEADR.C
// ============================================================================================

void WINAPI ABFH_Initialize( ABFFileHeader *pFH );

void WINAPI ABFH_InitializeScopeConfig(const ABFFileHeader *pFH, ABFScopeConfig *pCfg);

BOOL WINAPI ABFH_CheckScopeConfig(const ABFFileHeader *pFH, ABFScopeConfig *pCfg);

void WINAPI ABFH_GetADCDisplayRange( const ABFFileHeader *pFH, int nChannel, 
                                     float *pfUUTop, float *pfUUBottom);
                                     
void WINAPI ABFH_GetADCtoUUFactors( const ABFFileHeader *pFH, int nChannel, 
                                    float *pfADCToUUFactor, float *pfADCToUUShift );
void WINAPI ABFH_ClipADCUUValue(const ABFFileHeader *pFH, int nChannel, float *pfUUValue);
                                           
void WINAPI ABFH_GetDACtoUUFactors( const ABFFileHeader *pFH, int nChannel, 
                                    float *pfDACToUUFactor, float *pfDACToUUShift );
void WINAPI ABFH_ClipDACUUValue(const ABFFileHeader *pFH, int nChannel, float *pfUUValue);

BOOL WINAPI ABFH_GetMathValue(const ABFFileHeader *pFH, float fA, float fB, float *pfRval);
int WINAPI ABFH_GetMathChannelName(LPSTR psz, UINT uLen);

BOOL WINAPI ABFH_ParamReader( HANDLE hFile, ABFFileHeader *pFH, int *pnError );
BOOL WINAPI ABFH_ParamWriter( HANDLE hFile, ABFFileHeader *pFH, int *pnError );

BOOL WINAPI ABFH_GetErrorText( int nError, char *pszBuffer, UINT nBufferSize );

BOOL WINAPI ABFH_GetCreatorInfo(const ABFFileHeader *pFH, char *pszName, UINT uNameSize, char *pszVersion, UINT uVersionSize);
BOOL WINAPI ABFH_GetModifierInfo(const ABFFileHeader *pFH, char *pszName, UINT uNameSize, char *pszVersion, UINT uVersionSize);

// ABF 1 conversion functions - use with care.
struct ABFFileHeader1;
BOOL WINAPI ABFH_ConvertFromABF1( const ABFFileHeader1 *pIn, ABFFileHeader *pOut, int *pnError );
BOOL WINAPI ABFH_ConvertABF2ToABF1Header( const ABFFileHeader *pNewFH, ABFFileHeader1 *pOldFH, int *pnError );


// ABFHWAVE.CPP

// Constants for ABFH_GetEpochLimits
#define ABFH_FIRSTHOLDING  -1
#define ABFH_LASTHOLDING   ABF_EPOCHCOUNT

// Return the bounds of a given epoch in a given episode. Values returned are ZERO relative.
BOOL WINAPI ABFH_GetEpochLimits(const ABFFileHeader *pFH, int nADCChannel, UINT uDACChannel, DWORD dwEpisode, 
                                int nEpoch, UINT *puEpochStart, UINT *puEpochEnd,
                                int *pnError);

// Get the offset in the sampling sequence for the given physical channel.
BOOL WINAPI ABFH_GetChannelOffset( const ABFFileHeader *pFH, int nChannel, UINT *puChannelOffset );

// This function forms the de-multiplexed DAC output waveform for the
// particular channel in the pfBuffer, in DAC UserUnits.
BOOL WINAPI ABFH_GetWaveform( const ABFFileHeader *pFH, UINT uDACChannel, DWORD dwEpisode, 
                                float *pfBuffer, int *pnError);

// This function forms the de-multiplexed Digital output waveform for the
// particular channel in the pdwBuffer, as a bit mask. Digital OUT 0 is in bit 0.
BOOL WINAPI ABFH_GetDigitalWaveform( const ABFFileHeader *pFH, int nChannel, DWORD dwEpisode, 
                                     DWORD *pdwBuffer, int *pnError);

// Calculates the timebase array for the file.
void WINAPI ABFH_GetTimebase(const ABFFileHeader *pFH, double dTimeOffset, double *pdBuffer, UINT uBufferSize);

// Constant for ABFH_GetHoldingDuration
#define ABFH_HOLDINGFRACTION 64

// Get the duration of the first holding period.
UINT WINAPI ABFH_GetHoldingDuration(const ABFFileHeader *pFH);

// Checks whether the waveform varies from episode to episode.
BOOL WINAPI ABFH_IsConstantWaveform(const ABFFileHeader *pFH, UINT uDACChannel);

// Get the full sweep length given the length available to epochs or vice-versa.
int WINAPI ABFH_SweepLenFromUserLen(int nUserLength, int nNumChannels);
int WINAPI ABFH_UserLenFromSweepLen(int nSweepLength, int nNumChannels);

// Converts a display range to the equivalent gain and offset factors.
void WINAPI ABFH_GainOffsetToDisplayRange( const ABFFileHeader *pFH, int nChannel, 
                                           float fDisplayGain, float fDisplayOffset,
                                           float *pfUUTop, float *pfUUBottom);

// Converts a display range to the equivalent gain and offset factors.
void WINAPI ABFH_DisplayRangeToGainOffset( const ABFFileHeader *pFH, int nChannel, 
                                           float fUUTop, float fUUBottom,
                                           float *pfDisplayGain, float *pfDisplayOffset);

// Converts a time value to a synch time count or vice-versa.
void WINAPI ABFH_SynchCountToMS(const ABFFileHeader *pFH, UINT uCount, double *pdTimeMS);
UINT WINAPI ABFH_MSToSynchCount(const ABFFileHeader *pFH, double dTimeMS);

// Gets the duration of the Waveform Episode (in us), allowing for split clock etc.
void WINAPI ABFH_GetEpisodeDuration(const ABFFileHeader *pFH, double *pdEpisodeDuration);

// Returns TRUE is P/N is enabled on any output channel.
BOOL WINAPI ABFH_IsPNEnabled(const ABFFileHeader *pFH, UINT uDAC=ABF_ANY_CHANNEL);

// Returns TRUE is ADC channel is corrected during leak subtraction.
BOOL WINAPI ABFH_IsADCLeakSubtracted(const ABFFileHeader *pFH, short nADC);

// Gets the duration of a P/N sequence (in us), including settling times.
void WINAPI ABFH_GetPNDuration(const ABFFileHeader *pFH, double *pdPNDuration);

// Gets the duration of a pre-sweep train in us.
void WINAPI ABFH_GetTrainDuration (const ABFFileHeader *pFH, UINT uDAC, double *pdTrainDuration);

// Gets the duration of a post-train portion of the pre-sweep train in us.
void WINAPI ABFH_GetPostTrainDuration (const ABFFileHeader *pFH, UINT uDAC, UINT uEpisode, double *pdDuration);

// Gets the level of a post-train portion of the pre-sweep train.
void WINAPI ABFH_GetPostTrainLevel (const ABFFileHeader *pFH, UINT uDAC, UINT uEpisode, double *pdLevel);

// Gets the duration of a whole meta-episode (in us).
void WINAPI ABFH_GetMetaEpisodeDuration(const ABFFileHeader *pFH, double *pdMetaEpisodeDuration);

// Gets the start to start period for the episode in us.
void WINAPI ABFH_GetEpisodeStartToStart(const ABFFileHeader *pFH, double *pdEpisodeStartToStart);

// Checks that the user list contains valid entries for the protocol.
BOOL WINAPI ABFH_CheckUserList(const ABFFileHeader *pFH, UINT uListNum, int *pnError);

// Counts the number of changing sweeps.
UINT WINAPI ABFH_GetNumberOfChangingSweeps( const ABFFileHeader *pFH );

// // Checks whether the digital output varies from episode to episode.
BOOL WINAPI ABFH_IsConstantDigitalOutput(const ABFFileHeader *pFH, UINT uDACChannel);

int WINAPI ABFH_GetEpochDuration(const ABFFileHeader *pFH, UINT uDACChannel, UINT uEpisode, int nEpoch);

float WINAPI ABFH_GetEpochLevel(const ABFFileHeader *pFH, UINT uDACChannel, UINT uEpisode, int nEpoch);
BOOL WINAPI ABFH_GetEpochLevelRange(const ABFFileHeader *pFH, UINT uDACChannel, int nEpoch, float *pfMin, float *pfMax);
UINT WINAPI ABFH_GetMaxPNSubsweeps(const ABFFileHeader *pFH, UINT uDACChannel);


//
// Error return values that may be returned by the ABFH_xxx functions.
//

#define ABFH_FIRSTERRORNUMBER          2001
#define ABFH_EHEADERREAD               2001
#define ABFH_EHEADERWRITE              2002
#define ABFH_EINVALIDFILE              2003
#define ABFH_EUNKNOWNFILETYPE          2004
#define ABFH_CHANNELNOTSAMPLED         2005
#define ABFH_EPOCHNOTPRESENT           2006
#define ABFH_ENOWAVEFORM               2007
#define ABFH_EDACFILEWAVEFORM          2008
#define ABFH_ENOMEMORY                 2009
#define ABFH_BADSAMPLEINTERVAL         2010
#define ABFH_BADSECONDSAMPLEINTERVAL   2011
#define ABFH_BADSAMPLEINTERVALS        2012
#define ABFH_ENOCONDITTRAINS           2013
#define ABFH_EMETADURATION             2014
#define ABFH_ECONDITNUMPULSES          2015
#define ABFH_ECONDITBASEDUR            2016
#define ABFH_ECONDITBASELEVEL          2017
#define ABFH_ECONDITPOSTTRAINDUR       2018
#define ABFH_ECONDITPOSTTRAINLEVEL     2019
#define ABFH_ESTART2START              2020
#define ABFH_EINACTIVEHOLDING          2021
#define ABFH_EINVALIDCHARS             2022
#define ABFH_ENODIG                    2023
#define ABFH_EDIGHOLDLEVEL             2024
#define ABFH_ENOPNPULSES               2025
#define ABFH_EPNNUMPULSES              2026
#define ABFH_ENOEPOCH                  2027
#define ABFH_EEPOCHLEN                 2028
#define ABFH_EEPOCHINITLEVEL           2029
#define ABFH_EDIGLEVEL                 2030
#define ABFH_ECONDITSTEPDUR            2031
#define ABFH_ECONDITSTEPLEVEL          2032
#define ABFH_EINVALIDBINARYCHARS       2033
#define ABFH_EBADWAVEFORM              2034

#ifdef __cplusplus
}
#endif

#endif   /* INC_ABFHEADR2_H */
