// ********************************************************************
//
// FILENAME:  
// 
// 		CalcBfds.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Calculates BFD factors from wavelength filter curves in the factory
//		config file.
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include <stdio.h>			// for terminal window debugging - to be removed later
#include <math.h>			// required standard C library 

#include "CalcBfdData.h"
#include "FactoryData.h"


// #include "SystemError.h"
#include "SystemErrorNums.h"


// #include "LogErr.h"


// misc. definitions

#define DYE_CURVE_CENTER_POINT			10

// (also defined in AlgGlobals.h - should this be in CommonDefines.h?

#define	UNINITIALIZED_FILTER_CAL		0xffff


FactoryData_t *	factoryData_m;


// ********************************************************************
// FUNCTION:
//
//		CalFactorCalc() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calc cal factor for either a primary or secondary wavelength filter/dye indices pair
//
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

void CalFactorCalc( DyeCurveIndex_t			dyeIdx,
					WavelengthFilterIndex_t	filterIdx,
					float *					filterSum,
					float *					dyeAbs,
					float *					calFactor,
					SystemErrorNumber_t *	errorNumber )
{
	unsigned		i;
	unsigned short 	filter;
	float			dyeAdjustedFilterSum;
	float			dyeCenter;
	float			absRatio;
	float			sysAbs;

	// init BFD cal factor calc accummulators

	*filterSum = 0.0;
	dyeAdjustedFilterSum = 0.0;

	// calc for the point with maximum absorbance (center of dye curve points)

	dyeCenter = bfdDyeCurves[dyeIdx][DYE_CURVE_CENTER_POINT];

	if (dyeCenter <= 0.0)
	{
		*errorNumber = SERR_BFD_ILLEGAL_LOG_PARAM;
		return;
	}

	*dyeAbs = -log10(dyeCenter);

	// accummulate (wavelength filter cals) & (wavelength filter cals * dye cals)

	for (i = 0; i < MAX_BFD_POINTS; i++)
	{
		filter = factoryData_m->wavelengthFilterCurves[filterIdx][i];

		if (filter == UNINITIALIZED_FILTER_CAL)
		{
			*errorNumber = SERR_MISSING_FILTER_CURVE_FACTORS; 
			return;
		}

		*filterSum           +=  (float) filter;
		dyeAdjustedFilterSum += ((float) filter) * bfdDyeCurves[dyeIdx][i];
	}

	// detect divide by 0 errors

	if (*filterSum == 0.0)
	{
		*errorNumber = SERR_BFD_DIVIDE_BY_ZERO;
		return;
	}

	// calculate the term to convert to absorbance

	absRatio = dyeAdjustedFilterSum / *filterSum;

	// make sure we have legal param for log call

	if (absRatio <= 0.0)
	{
		*errorNumber = SERR_BFD_ILLEGAL_LOG_PARAM;
		return;
	}

	// calculate system absorbance for combined system / dye absorbance ratio term

	sysAbs = -log10(absRatio);

	// calculate cal factor	for wavelength filter

	*calFactor = *dyeAbs / sysAbs;
}


// ********************************************************************
// FUNCTION:
//
//		CalcBfdCalFactor() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calc BFD calibration factor of BFD index (used by analyte algorithms)
//
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

void CalcBfdCalFactor( unsigned					bfdIdx,
					   float *					bfdAvgCurveFactors,
					   float *					calFactor,
					   SystemErrorNumber_t *	errorNumber )
{
	float 			filterSum;
	float			calFactor1; 
	float			calFactor2;
	float			dye1Abs;
	float			dye2Abs;
	
	DyeCurveIndex_t				dyeIdx;
	WavelengthFilterIndex_t		filterIdx;
		
	// init this bfd calibration factor

	*calFactor = 1.0;     

	// make sure that this primary BFD record is used by algorithms (valid indices in table?)

	dyeIdx     = bfdFactorIndices[bfdIdx].primaryDyeCurveIndex;
	filterIdx  = bfdFactorIndices[bfdIdx].primaryWavelengthFilterCurveIndex;

	if ((dyeIdx >= NO_DYE_IDX) || (filterIdx >= NO_FILTER_IDX))
	{
		return;
	}

	// valid primary indices so calc bfd calibration factor for primary

	CalFactorCalc(dyeIdx, filterIdx, &filterSum, &dye1Abs, &calFactor1, errorNumber);

	if (*errorNumber != SERR_NONE)
	{
		return;
	}

	// Calculate the running average of the cal factors for IQC algorithm check (uses primary filter sum calc only)

	*bfdAvgCurveFactors += filterSum / MAX_BFD_POINTS; 

	// check if secondary BFD record is used by algorithms (valid indices in table?)

	dyeIdx     = bfdFactorIndices[bfdIdx].secondaryDyeCurveIndex;
	filterIdx  = bfdFactorIndices[bfdIdx].secondaryWavelengthFilterCurveIndex;

	if ((dyeIdx < NO_DYE_IDX) && (filterIdx < NO_FILTER_IDX))
	{
		// valid secondary indices so calc bfd calibration factor for secondary wavelength filter

		CalFactorCalc(dyeIdx, filterIdx, &filterSum, &dye2Abs, &calFactor2, errorNumber);

		if (*errorNumber != SERR_NONE)
		{
			return;
		}

		// combine the primary & secondary cal factors to generate overall BFD calibration factor

		*calFactor = (dye1Abs - dye2Abs) / ((dye1Abs / calFactor1) - (dye2Abs / calFactor2));
	}
	else
	{
		// bfd calibration factor uses primary cal factor only

		*calFactor = calFactor1;
	}
}


// ********************************************************************
// FUNCTION:
//
//		CalcBfds() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		BFD Calculations updates for factory configuration & IQC algorithm
//		using factory wavelength filter curve data & BFD configuration/dye curve data
//
// _____________________________________________________________________
// PARAMETERS:
//
//		factoryData
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

SystemErrorNumber_t CalcBfds( FactoryData_t * factoryData )
{
	unsigned			i;
	SystemErrorNumber_t	errorNumber;
	float				calFactor;
	float				bfdAvgCurveFactors;

	factoryData_m = factoryData;
	
	// initialize for no error & for average of bfd curve cal factors (used in IQC algorithm check)

	errorNumber = SERR_NONE;

	bfdAvgCurveFactors = 0.0;							

	 // Scan thru all 32 BFD table indices to update BFD cal factors used in analyte algorithms

	for (i = 0; i < MAX_BFD_CALS; i++)
	{
		// calculate this bfd calibration factor

		CalcBfdCalFactor( i, &bfdAvgCurveFactors, &calFactor, &errorNumber );
		
		if ( errorNumber != SERR_NONE )
		{


//			LogError( "error %x calculating BFD[%d]", errorNumber, i );


			calFactor = 1.0; 
		}

		// save this calculated BFD calibration factor in factory data area
		
		factoryData->bfdCalibrationFactors[i] = calFactor; 
	}

	/* Calculate the final average of all the cal factors and save for IQC check */

	factoryData->bfdAvgCurveFactors = bfdAvgCurveFactors / MAX_BFD_CALS; 
	
	return errorNumber;
}
