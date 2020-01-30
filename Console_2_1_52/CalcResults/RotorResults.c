// ********************************************************************
//
// FILENAME:  
// 
// 		RotorResults.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Top level Console thread that implements rotor algorithm analyte processing 
// 		& associated calculations per the ROC file using raw data from the Engine
//		with output of rotor results & flags to a results data record
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//

#include "CommonAlgFuncs.h"

#include "SystemCheck.h"
#include "SampleBlank.h"
#include "Rqc.h"
#include "Iqc.h"
#include "Ag.h"
#include "AlbBcg.h"
#include "AlbBcp.h"
#include "Alp.h"
#include "Alt.h"
#include "Amy.h"
#include "Ast.h"
#include "Ba1.h"
#include "Ba2.h"
#include "Bun.h"
#include "Ca.h"
#include "Chol.h"
#include "Ck.h"
#include "Cl1.h"
#include "Cl2.h"
#include "Cre.h"
#include "Dbil.h"
#include "Ggt.h"
#include "Glob.h"
#include "Glu.h"
#include "Hdl1.h"
#include "Hdl2.h"
#include "Ibil.h"
#include "K.h"
#include "Ldh.h"
#include "Ldl.h"
#include "Mg.h"
#include "Na1.h"
#include "Na2.h"
#include "Phos.h"
#include "T4.h"
#include "Tbil.h"
#include "Tch.h"
#include "Tco2.h"
#include "Tp.h"
#include "Trig.h"
#include "Ua.h"
#include "Vldl.h"
#include "Ctbil.h"
#include "Ba.h"
#include "Cl.h"
#include "Hdl.h"
#include "Na.h"
#include "Alt1.h"
#include "Alt2.h"
#include "Bun1.h"
#include "Bun2.h"
#include "Cre1.h"
#include "Cre2.h"
#include "Chw.h"
#include "AlbBcpA.h"
#include "Crp1.h"
#include "Crp2.h"
#include "Crp.h"
#include "Egfr.h"
#include "NaCl.h"
#include "Lac.h"
#include "Hb.h"
#include "Hct.h"
#include "Nhdlc.h"
#include "Ast1.h"
#include "Ast2.h"
#include "Ck1.h"
#include "Ck2.h"
#include "PHb.h"

#include "Rotor850Checks.h"
#include "RotorDilutionChecks.h"


typedef void (*AlgFunctionPtr_t)(void);

const	AlgFunctionPtr_t	algorithmFunctionTable_m[]= 
{
	CalcSystemCheck,	// analyte type: 	0	SYSTEM_CHECK	(per ROC file) 
	CalcSampleBlank,	//					1	SAMPLE_BLANK	
	CalcRqc,			//					2	RQC
	CalcIqc,			//					3	IQC
	CalcAg,				//					4	AG
	CalcAlbBcg,			//					5	ALB_BCG
	CalcAlbBcp,			//					6	ALB_BCP
	CalcAlp,			//					7	ALP
	CalcAlt1,			//					8	ALT_1
	CalcAmy,			//					9	AMY
	CalcAst1,			//					10	AST_1
	CalcBa1,			//					11	BA_1
	CalcBa2,			//					12	BA_2
	CalcBun1,			//					13	BUN_1
	CalcCa,				//					14	CA
	CalcChol,			//					15	CHOL
	CalcCk1,			//					16	CK_1
	CalcCl1,			//					17	CL_1
	CalcCl2,			//					18	CL_2
	CalcCre1,			//					19	CRE_1
	CalcDbil,			//					20	DBIL	
	CalcGgt,			//					21	GGT
	CalcGlob,			//					22	GLOB
	CalcGlu,			//					23	GLU
	CalcHdl1,			//					24	HDL_1
	CalcHdl2,			//					25	HDL_2
	CalcIbil,			//					26	IBIL
	CalcK,				//					27	K
	CalcLdh,			//					28	LDH
	CalcLdl,			//					29	LDL
	CalcMg,				//					30	MG
	CalcNa1,			//					31	NA_1
	CalcNa2,			//					32	NA_2
	CalcPhos,			//					33	PHOS
	CalcT4,				//					34	T4
	CalcTbil,			//					35	TBIL
	CalcTch,			//					36	TC/H
	CalcTco2,			//					37	TCO2
	CalcTp,				//					38	TP
	CalcTrig,			//					39	TRIG
	CalcUa,				//					40	UA
	CalcVldl,			//					41	VLDL
	CalcCtbil,			//					42	CTBIL
	CalcBa,				//					43	BA
	CalcCl,				//					44	CL
	CalcHdl,			//					45	HDL
	CalcNa,				//					46	NA
	CalcAlt2,			//					47	ALT_2
	CalcAlt,			//					48	ALT
	CalcBun2,			//					49	BUN_2
	CalcBun,			//					50	BUN
	CalcCre2,			//					51	CRE_2
	CalcCre,			//					52	CRE
	CalcChw,			//					53	CHW
	CalcAlbBcpA,		//					54	ALB_BCP_A
	CalcCrp1,			//					55	CRP_1
	CalcCrp2,			//					56	CRP_2
	CalcCrp,			//					57	CRP
	CalcEgfr,			//					58	EGFR
	CalcNaCl,			//					59	NACL
	CalcLac,			//					60	LAC
	CalcHb,				//					61	HB
	CalcHct,			//					62	HCT
	CalcNhdlc,			//					63	NHDLC
	CalcAst2,			//					64	AST_2
	CalcAst,			//					65	AST
	CalcCk2,			//					66	CK_2
	CalcCk,				//					67	CK
	CalcPhb,			//					68	PHB
};

#define MAX_DEFINED_ALGS		(sizeof algorithmFunctionTable_m / sizeof(AlgFunctionPtr_t)) 


unsigned char	rotorAnalyteTypesByCalcIndex_m[MAX_ROTOR_ALGORITHMS];	


// ********************************************************************
// FUNCTION:
//
//		SetUpAnalyteAlgIdxListFromRoc() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Creates list of indirect function calls indices for defined rotor algorithms from ROC file
//		- scans through ROC data to set up analyteType algorithm order by calculationIndex
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

void SetUpAnalyteAlgIdxListFromRoc(void)
{
	unsigned	i;
	unsigned	calcIdx;

	// init for no rotor analyte calculation for analyteType

	for	(i = 0;	i < MAX_ROTOR_ALGORITHMS; i++)
	{
		rotorAnalyteTypesByCalcIndex_m[i] = NO_ANALYTE_CALC;
	}

	// scan through ROC structure to fill list of algorithm analyteType by calculationIndex

	for	(i = 0;	i < MAX_ANALYTE_TYPES; i++)
	{
		calcIdx = rotorConfig_g->analyteCalcRecord[i].calculationIndex;

		if (calcIdx < MAX_ROTOR_ALGORITHMS)
		{
			rotorAnalyteTypesByCalcIndex_m[calcIdx] = rotorConfig_g->analyteCalcRecord[i].analyteType;
		}
	}	
}


// ********************************************************************
// FUNCTION:
//
//		SuppressAnalyteResultAllowed() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check if suppression of analyteResult in resultPrintFlags is allowed
//		for analyteType
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

unsigned SuppressAnalyteResultAllowed(unsigned char analyteType)
{
	unsigned	i;

	i = 0;

	while ( i < commonAlgGlobals_g->rotorNoSuppressAnalyteResultListIdx ) 
	{
		if (commonAlgGlobals_g->rotorNoSuppressAnalyteResultList[i++] == analyteType)
		{
			return FALSE;
		}
	}

	return TRUE;
}

// ********************************************************************
// FUNCTION:
//
//		ScanUpdateRotorAnalyteResultFlags() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Scans and updates result flags for defined rotor analyte results defined from ROC file 
//		by type selected for analytes that have identified printOrder:
//		- update final resultSuppressed flag in resultPrintFlags from analyteFlags
//		- update counters for bead missing & distribution check errors
//		- update short & long sample probabilities for enabled final results
//		- set dilutionCheckError flag in resultPrintFlags for all analyteType if rotor dilution error
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

void ScanUpdateRotorAnalyteResultFlags(unsigned updateType, unsigned char *rotorAnalyteTypesByCalcIndex) 
{
	unsigned		i;
	unsigned char	analyteType;

	RotorAnalyteResult_t	*resultPtr;

	// init scan index and scan through all analytes defined for rotor

	i = 0;

	while(rotorAnalyteTypesByCalcIndex[i] != NO_ANALYTE_CALC)
	{
		analyteType = rotorAnalyteTypesByCalcIndex[i];

		resultPtr = &rotorResults_g->rotorAnalyteResult[analyteType];

		// only update if print order is defined in rotor ROC file for analyteType

		if (resultPtr->printOrder != NO_PRINT_ORDER_FLAG)
		{
			switch (updateType)
			{
				case SCAN_FOR_ANALYTE_SUPPRESSED:
					if (resultPtr->analyteFlags & (resultPtr->analyteSuppressionMask & ~(HEMOLYZED_LIMIT_ERROR | LIPEMIC_LIMIT_ERROR | ICTERIC_LIMIT_ERROR)))
					{
						// check if allowed to suppress result print for analyteType

						if ( SuppressAnalyteResultAllowed(analyteType) == TRUE )
						{
							// if allowed, print result suppressed flag set for all masked errors except for system & dynamic range flags and endogenous limits flags in analyte flags
							// system & dynamic range flags and endogenous limits flags are handled separately in resultPrintFlags

							resultPtr->resultPrintFlags |= MASK_RESULTS_SUPPRESSED;
						} 
					}
					break;
				case SCAN_BEAD_DISTRIBUTION_COUNTS:
					resultPtr->analyteFlags = UpdateBeadMissingDistributionErrorCounts(resultPtr->analyteFlags, resultPtr->analyteSuppressionMask);
					break;
				case SCAN_SHORT_LONG_SAMPLE_PROBS:
					UpdateRotorDilutionCheckProbs(analyteType, resultPtr->analyteFlags, resultPtr->analyteSuppressionMask, resultPtr->analyteResult);
					break;
				case SCAN_SET_DILUTION_CHECK_FLAGS:
					resultPtr->analyteFlags |= DILUTION_VERIFICATION_ERROR;		// for legacy bit set/match in analyte flags
					resultPtr->resultPrintFlags |= MASK_DILUTION_CHECK;			// for print card
					break;
				default:
					break;
			}
		}

		i++;
	}
}


// ********************************************************************
// FUNCTION:
//
//		PostProcessRotorChecksErrors() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Post process rotor specific checks/errors after analyte algs completed 
//		- init update counters & accummulators
//		- update resultPrintFlags based on analyteSuppression Mask
//		- based on analyteFlags, update counters for bead errors & distribution errors
//		  for analytes with printOrder
//		- based on analyteFlags and dilutionCheckEnabled, update dilution check
//		  short and long sample probability accummulators for final dilution check
//
//		Check final counters versus rotor limits for bead & distribution checks 
//		to process any errors
//
//		Check final short & long sample accummulated probabilities versus rotor limits
//		- to process set of rotor dilution check error and
//		- set dilutionCheckError flag in resultPrintFlags for each analyte with printOrder
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

void PostProcessRotorChecksErrors(void)
{
	ProcessRotorAnalyteBeadMissingDistributionErrors(rotorAnalyteTypesByCalcIndex_m);

	ScanUpdateRotorAnalyteResultFlags(SCAN_FOR_ANALYTE_SUPPRESSED, rotorAnalyteTypesByCalcIndex_m); 

	ProcessRotorDilutionChecksErrors(rotorAnalyteTypesByCalcIndex_m);
}


// ********************************************************************
// FUNCTION:
//
//		CalcRotorAnalyteResults() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Console Algorithms process top level main function
//		- loops through rotor analyte algorithms from rotor ROC file 
//		  using indirect function call table indexed by analyteType in algorithm order
//		- processes rotor post analyte algorithm checks/errors 
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

unsigned short CalcRotorAnalyteResults(void)
{
	unsigned 		i;
	unsigned		algIdx;

	unsigned		rotorErrorNumberIndex;

	// save rotor error number index for 1st system error number found here

	rotorErrorNumberIndex = rotorResults_g->rotorInformationResults.rotorErrorNumberIndex;

	// set up algorithm calculation order from rotor ROC

	SetUpAnalyteAlgIdxListFromRoc();

	// execute algorithms by calculation index in ROC file
		
	for	(i = 0;	i < MAX_ROTOR_ALGORITHMS; i++)
	{
		algIdx = rotorAnalyteTypesByCalcIndex_m[i];

		if ((algIdx < MAX_DEFINED_ALGS))
		{
			// invoke rotor analyte algorithm indirect function

			(*algorithmFunctionTable_m[algIdx])();
		}
	}


	//
	// analyteResult & analyteFlags for an analyte can in some cases be modified 
	// by algorithms (e.g. derived calcs) invoked after
	// a specific analyte algorithm sets its analyteResult & analyteFlags
	//
	// Thus, the following functions handle rotor error conditions and specific analyte error suppressions after
	// all algorithms for a rotor have been individually processed.
	//
	// Post Process rotor specific errors here for:
	//
	//	- rotor resultPrintFlags for specific analyteSuppression
	//	- rotor bead checks & rotor distribution checks
	//	- rotor dilution check processing
	//

	PostProcessRotorChecksErrors();


	// report first system error number found in algorithm (if any)  

	return SetAlgorithmErrorNumber(rotorErrorNumberIndex);
}



