// ********************************************************************
//
// FILENAME:  
// 
// 		NaCl.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		NACL calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


#define NACL_ERR_CHK_LIM		120.0		// updated from original 125.0

// ********************************************************************
// FUNCTION:
//
//		CalcNaCl() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		NACL calculation algorithm 
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

void	CalcNaCl(void)
{
	unsigned		naAnalyteType;
	unsigned		clAnalyteType;

	float			naResult;	   
	float			clResult;

	float			naConfidenceLimit;
		   
	unsigned long	naFlags;
	unsigned long	naMask;

	unsigned long	clFlags;
	unsigned long	clMask;


	// init alg specific values

	naConfidenceLimit  = NOVAL_NEG_FLOAT;


	// IMV - start of NACL algorithm to here for paragraph number & alg version

	StoreAlgParagraphNumImv( NACL, 1.0 );

	StoreAlgVersionNumImv( NACL );

	// end IMV


	// process NA type algs flags/result (typically NA for 2 cuvette alg)

	naAnalyteType = NA;

	if (rotorResults_g->rotorAnalyteResult[NA_1].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// only 1 cuvette processed for NA_1 algorithm this rotor

		naAnalyteType = NA_1;
	}
	else if (rotorResults_g->rotorAnalyteResult[NA_2].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// only 1 cuvette processed for NA_2 algorithm this rotor

		naAnalyteType = NA_2;
	}

	naResult = rotorResults_g->rotorAnalyteResult[naAnalyteType].analyteResult;
	naFlags  = rotorResults_g->rotorAnalyteResult[naAnalyteType].analyteFlags;
	naMask   = rotorResults_g->rotorAnalyteResult[naAnalyteType].analyteSuppressionMask;


	// process CL type algs flags/result (typically CL for 2 cuvette alg)

	clAnalyteType = CL;

	if (rotorResults_g->rotorAnalyteResult[CL_1].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// only 1 cuvette processed for CL_1 algorithm this rotor

		clAnalyteType = CL_1;
	}
	else if (rotorResults_g->rotorAnalyteResult[CL_2].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// only 1 cuvette processed for CL_2 algorithm this rotor

		clAnalyteType = CL_2;
	}

	clResult = rotorResults_g->rotorAnalyteResult[clAnalyteType].analyteResult;	
	clFlags  = rotorResults_g->rotorAnalyteResult[clAnalyteType].analyteFlags;
	clMask   = rotorResults_g->rotorAnalyteResult[clAnalyteType].analyteSuppressionMask;


	// process NACL alg logic using NA and CL related result data/flags/masks

	if ( ( clFlags & clMask ) == NO_ANALYTE_ERROR )
	{
		// clResult not suppressed, so calculate lower 95% confidence limit for NA
				
		naConfidenceLimit = ( -0.0181 * clResult * clResult ) + ( 4.3802 * clResult ) - 128.89;


		// IMV - clResult not suppressed to here and save lower 95% confidence limit calculated for NA

		StoreAlgParagraphNumImv( NACL, 2.0 );

		StoreIntermediateValue( NACL, IMV_NACL_NA_CONF_LIMIT, IMV_DIRECT_SET_METHOD, naConfidenceLimit );

		// end IMV


		if ( ( naFlags & naMask ) == NO_ANALYTE_ERROR )
		{
			// naResult is not suppressed so check if naResult is low enough to continue


			// IMV - naResult not suppressed to here

			StoreAlgParagraphNumImv( NACL, 3.0 );

			// end IMV


			if ( naResult < NACL_ERR_CHK_LIM )
			{
				// naResult low enough so check if CONTROL rotor


				// IMV - naResult low enough to here

				StoreAlgParagraphNumImv( NACL, 4.0 );

				// end IMV


 				if ( !commonAlgGlobals_g->controlType )
 				{
					// not a CONTROL rotor so check naResult against calculated lower 95% confidence limit for NA 


					// IMV - not a CONTROL rotor to here

					StoreAlgParagraphNumImv( NACL, 5.0 );

					// end IMV


					if ( naResult < naConfidenceLimit )
					{
						// NA less than confidence limit so suppress NA result for NACL alg, NA "Low Recovery"


						// IMV - naResult less than confidence limit to here

						StoreAlgParagraphNumImv( NACL, 6.0 );

						// end IMV


						rotorResults_g->rotorAnalyteResult[naAnalyteType].analyteFlags |= NACL_NA_LOW_RECOVERY;
					}
				}
			}
		}
	}


#ifndef __QNX__
	printf("NACL invoked\n");
#endif
}
