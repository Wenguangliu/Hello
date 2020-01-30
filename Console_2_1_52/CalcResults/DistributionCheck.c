// ********************************************************************
//
// FILENAME:  
// 
// 		DistributionCheck.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Top level Console thread that implements rotor algorithm processing for 
// 		checking full rotor scan raw data from the Engine at distribution check time
//		while outputting associated flags/parameters to a rotor results data record
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


#define	IQC_CUV_04_DIST_LIMIT		0.30
#define	IQC_CUV_05_DIST_LIMIT		0.30
#define	SYS_CUV_06_DIST_LIMIT		0.30
#define	SYS_CUV_29_DIST_LIMIT		0.30


// ********************************************************************
// FUNCTION:
//
//		SystemDistributionChecks() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Console Algorithm for error checking after distribution check full rotor scan time
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

unsigned short SystemDistributionChecks(void)
{
	const BeadDistCheck_t	distributionChecks[] =
	{
		{4,					 IQC_CUV_04_DIST_LIMIT}, 
		{5,					 IQC_CUV_05_DIST_LIMIT}, 
		{6,					 SYS_CUV_06_DIST_LIMIT}, 
		{29,				 SYS_CUV_29_DIST_LIMIT},
		 
		{BEAD_DIST_CHK_DONE, BEAD_DIST_CHK_DONE   }, 
	};

	unsigned	rotorErrorNumberIndex;

	// save rotor error number index for 1st system error number found here

	rotorErrorNumberIndex = rotorResults_g->rotorInformationResults.rotorErrorNumberIndex;

	// distribution check is disabled for the T4 rotor

	if ( (rotorConfig_g->rotorNumber != 40) &&
	     (rotorConfig_g->rotorNumber != 1 ) )
	{
		// check for distribution error for cuvette at system distribution check time

		SystemBeadMissingDistCheck(DISTRIBUTION_READINGS, distributionChecks, DIST_CHECK_FAILED, SERR_DISTRIBUTION);  
	}

	return SetAlgorithmErrorNumber(rotorErrorNumberIndex);
}



