// ********************************************************************
//
// FILENAME:  
// 
// 		Egfr.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		EGFR calculation algorithm (aka eGFR)
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//

#include <math.h>				// required standard C library 

#include "CommonAlgFuncs.h"

#define EGFR_CONTROL				0x00000100			// eGFR suppressed due to Control sample

#define	EGFR_NO_AGE					0x00000080			// eGFR suppressed due to no age    entered for calc 
#define	EGFR_NO_GENDER				0x00000040			// eGFR suppressed due to no gender entered for calc 
#define	EGFR_NO_RACE				0x00000020			// eGFR suppressed due to no race   entered for calc 
#define	EGFR_AGE_OUT_OF_RANGE		0x00000010			// eGFR suppressed due to (age < 18) or (age > 119)  for age out of range for calc 
#define	EGFR_CRE_SUPPRESSED			0x00000008			// eGFR suppressed due to CRE suppressed 
#define	EGFR_CRE_TOO_LOW			0x00000004			// eGFR suppressed due to CRE value too small for calc 
#define	EGFR_CRE_NEGATIVE			0x00000002			// eGFR suppressed due to CRE negative value 
#define	EGFR_AGE_LT_1YR				0x00000001			// eGFR suppressed due to age < 1  

#define EFGR_FEMALE_GENDER_MULT		0.742				// eGFR result multiplier if gender is female
#define EFGR_BLACK_RACE_MULT		1.212				// eGFR result multiplier if race   is black

#define EFGR_CRE_LOW_LIMIT			0.1					// eGFR CRE result low limit check for no EFGR calc


// ********************************************************************
// FUNCTION:
//
//		CalcEgfr() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		EGFR calculation algorithm 
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

void	CalcEgfr(void)
{
	int				age;
	Gender_t		gender;
	Race_t			race;

	unsigned		analyteType;
	float			creFloatResult;
	int				creIntResult;	   
	float			creResult;			// printed result rounded to 1 significant digit after decimal point
	unsigned long	creFlags;
	unsigned long	creSuppressionMask;

	float			ageFactor;
	float			femaleFactor;
	float			blackFactor;
	unsigned		egfrCalcFlag;
			   
	float			egfrResult;	   
	unsigned long	egfrFlags;

	CommonFactorsLimits_t	*factorsLimitsPtr;

	// set up derived calc alg data & factors & limits for result store

	factorsLimitsPtr = SetupAnalyteAlgInfo(EGFR, DERIVED_CALC_ALG);

	// init result & flags values

	egfrResult  = NOVAL_NEG_FLOAT;
	egfrFlags   = 0;

	// set up CRE type algs flags/result input to eGFR alg ( default CRE if 2 cuvette alg)

	analyteType = CRE;

	if (rotorResults_g->rotorAnalyteResult[CRE_1].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// if only 1 cuvette processed for CRE_1 algorithm this rotor

		analyteType = CRE_1;
	}
	else if (rotorResults_g->rotorAnalyteResult[CRE_2].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// if only 1 cuvette processed for CRE_2 algorithm this rotor

		analyteType = CRE_2;
	}

	creFloatResult      = rotorResults_g->rotorAnalyteResult[analyteType].analyteResult;
	creIntResult		= (int) (100.0 * (creFloatResult + 0.005));
	creResult			= ((float) creIntResult) / 100.0;

	creFlags            = rotorResults_g->rotorAnalyteResult[analyteType].analyteFlags;
	creSuppressionMask  = rotorResults_g->rotorAnalyteResult[analyteType].analyteSuppressionMask | ANALYTE_RESULT_LIMITS_ERR_MASK;

	// process eGFR exceptions 

	age    = commonAlgGlobals_g->age;
	gender = commonAlgGlobals_g->gender;
	race   = commonAlgGlobals_g->race;

	femaleFactor = 1.0;
	blackFactor  = 1.0;
	ageFactor = (float) age;

	egfrCalcFlag = TRUE;
		

	// process "Control" sample exception

	if ( commonAlgGlobals_g->controlType == TRUE )
	{
		egfrCalcFlag = FALSE;
		egfrFlags   |= EGFR_CONTROL; 
		rotorResults_g->rotorAnalyteResult[EGFR].resultPrintFlags |= MASK_NOT_COMPUTED;
	}

	// process age exceptions

	if ( age == NO_AGE_ENTERED )
	{
		egfrCalcFlag = FALSE;
		egfrFlags   |= EGFR_NO_AGE; 
		rotorResults_g->rotorAnalyteResult[EGFR].resultPrintFlags |= MASK_NOT_COMPUTED;
	}
	else if ( age < 1 )
	{
		egfrCalcFlag = FALSE;
		egfrFlags   |= EGFR_AGE_LT_1YR; 
	}
	else if (( age < 18 ) || ( age > 119 ))
	{
		egfrCalcFlag = FALSE;
		egfrFlags   |= EGFR_AGE_OUT_OF_RANGE;
		rotorResults_g->rotorAnalyteResult[EGFR].resultPrintFlags |= MASK_NOT_COMPUTED;
	}


	// process gender exceptions

	if (( gender == GENDER_NONE ) || ( gender == GENDER_UNKNOWN ))
	{
		egfrCalcFlag = FALSE;
		egfrFlags   |= EGFR_NO_GENDER; 
		rotorResults_g->rotorAnalyteResult[EGFR].resultPrintFlags |= MASK_NOT_COMPUTED;
	}
	else if ( gender == GENDER_FEMALE )
	{
		femaleFactor *= EFGR_FEMALE_GENDER_MULT;
	}


	// process race exceptions

	if (( race == RACE_NONE ) || ( race == RACE_UNKNOWN ))
	{
		egfrCalcFlag = FALSE;
		egfrFlags   |= EGFR_NO_RACE; 
		rotorResults_g->rotorAnalyteResult[EGFR].resultPrintFlags |= MASK_NOT_COMPUTED;
	}
	else if ( race == RACE_BLACK )
	{
		blackFactor *= EFGR_BLACK_RACE_MULT;
	}


	// process CRE exceptions

	if ( creFlags & creSuppressionMask )
	{
		egfrFlags   |= EGFR_CRE_SUPPRESSED;
		
		if ( creResult == NOVAL_NEG_FLOAT )		// allow calc if suppressed and creResult not NOVAL_NEG_FLOAT
		{
			egfrCalcFlag = FALSE;
		} 
	}

	if ( creResult < 0.0 )
	{
		egfrCalcFlag = FALSE;
		egfrFlags   |= EGFR_CRE_NEGATIVE; 
	}
	else if ( creResult < EFGR_CRE_LOW_LIMIT )
	{
		egfrCalcFlag = FALSE;
		egfrFlags   |= EGFR_CRE_TOO_LOW; 
	}
	 

	// calc the eGFR result if no errors/issues to preclude calculation 

	if ( egfrCalcFlag == TRUE )
	{
		egfrResult = 175.0 * pow(creResult, -1.154) * pow(ageFactor, -0.203) * femaleFactor * blackFactor;

		// check EGFR result limits

		ResultLimitsCheck(EGFR, egfrResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &egfrFlags);
	}

	// save EGFR results

	StoreAnalyteResultsRecord(EGFR, egfrResult, egfrFlags, factorsLimitsPtr);


#ifndef __QNX__
	printf("EGFR invoked\n");
#endif
}
