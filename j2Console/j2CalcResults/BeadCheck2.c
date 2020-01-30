// ********************************************************************
//
// FILENAME:  
// 
// 		BeadCheck2.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Top level Console thread that implements rotor algorithm processing for 
// 		checking full rotor scan raw data from the Engine at bead check 2 time
//		while outputting associated flags/parameters to a rotor results data record
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


#define	SYS_CUV_00_BEAD_LIMIT		1.5
#define	SYS_CUV_01_BEAD_LIMIT		1.5
#define	SYS_CUV_02_BEAD_LIMIT		1.5
#define	RQC_CUV_03_BEAD_LIMIT		1.5
#define	IQC_CUV_04_BEAD_LIMIT		1.5
#define	IQC_CUV_05_BEAD_LIMIT		1.5
#define	SYS_CUV_06_BEAD_LIMIT		1.5
#define	SYS_CUV_07_BEAD_LIMIT		1.5
#define	SYS_CUV_29_BEAD_LIMIT		1.5


#define INSUFFICIENT_DILUENT_CUVETTE	3       
#define INSUFFICIENT_DILUENT_LIMIT		0.30


// ********************************************************************
// FUNCTION:
//
//		InsufficientDiluentCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check for insufficient diluent in cuvette 3 (RQC cuvette)
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

void InsufficientDiluentCheck(void)
{
	unsigned long	diluentCheckError;
	float			insufficientDiluentCuvetteTransmission;

	// init for no cuvette errors

	diluentCheckError = 0;

	// get transmission at bead 2 time for cuvette 3 to compare against insufficient diluent limit

	insufficientDiluentCuvetteTransmission = CalcCuvetteCheckTransmission(BEAD2_READINGS, INSUFFICIENT_DILUENT_CUVETTE, OPTICAL_CUVETTE, WP_850_850, NUM_BEAD_CHECK_FLASHES, &diluentCheckError);

	if (diluentCheckError & CALC_ERROR_MASK)
	{	
		if (diluentCheckError & DIVIDE_BY_ZERO)
		{
			ReportSystemError(SYS_DIV_BY_0, SERR_DIVIDE_BY_0);
		}
	}
	else if (insufficientDiluentCuvetteTransmission < INSUFFICIENT_DILUENT_LIMIT)
	{
		// set system flag for all rotors affected to flag that rotor is affected

		rotorResults_g->rotorInformationResults.rotorSystemFlags |= INSUFFICIENT_DIL;
		 
		// set for rotor suppression for insufficient dil at bead 2 time only for rotor 40 (T4)
		// other than rotor 40, check at RQC time before suppressing for insufficient diluent

		if (rotorConfig_g->rotorNumber == 40)
		{
			ReportSystemError(INSUFFICIENT_DIL, SERR_INSUFFICIENT_DILUENT);
		}
	}
}


// ********************************************************************
// FUNCTION:
//
//		SystemBeadChecks2() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Console Algorithm for error checking after bead check 2 full rotor scan time
//		from Engine raw system cuvette readings & updating results flags/parameters
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

unsigned short SystemBeadChecks2(void)
{
	const BeadDistCheck_t	beadChecks[] =
	{
		{0,					 SYS_CUV_00_BEAD_LIMIT}, 
		{1,					 SYS_CUV_01_BEAD_LIMIT}, 
		{2,					 SYS_CUV_02_BEAD_LIMIT}, 
		{3,					 RQC_CUV_03_BEAD_LIMIT}, 
		{4,					 IQC_CUV_04_BEAD_LIMIT}, 
		{5,					 IQC_CUV_05_BEAD_LIMIT}, 
		{6,					 SYS_CUV_06_BEAD_LIMIT}, 
		{7,					 SYS_CUV_07_BEAD_LIMIT}, 
		{29,				 SYS_CUV_29_BEAD_LIMIT},
		 
		{BEAD_DIST_CHK_DONE, BEAD_DIST_CHK_DONE   }, 
	};

	unsigned	rotorErrorNumberIndex;

	// save rotor error number index for 1st system error number found here

	rotorErrorNumberIndex = rotorResults_g->rotorInformationResults.rotorErrorNumberIndex;

	// check for bead missing at system bead check 2 time for all rotors

	SystemBeadMissingDistCheck(BEAD2_READINGS, beadChecks, BEAD_MISSING_2, SERR_BEAD_MISSING);

	// perform insufficient diluent check on transmission value for cuvette 3

	InsufficientDiluentCheck();

	return SetAlgorithmErrorNumber(rotorErrorNumberIndex);
}



