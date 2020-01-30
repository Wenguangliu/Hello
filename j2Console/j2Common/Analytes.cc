#include <string.h>

#include "Analytes.h"
#include "LogErr.h"

typedef struct
{
	AnalyteId_t	analyteId;
	StringId_t	stringId;
} AnalyteEntry_t;

AnalyteEntry_t	ANALYTES[] =
{
	{ AID_AG		, ASTR_AG },
	{ AID_ALB		, ASTR_ALB },
	{ AID_ALP		, ASTR_ALP },
	{ AID_ALT		, ASTR_ALT },
	{ AID_AMY		, ASTR_AMY },
	{ AID_AST		, ASTR_AST },
	{ AID_BA		, ASTR_BA },
	{ AID_BUN		, ASTR_BUN },
	{ AID_CA		, ASTR_CA },
	{ AID_CHOL		, ASTR_CHOL },
	{ AID_CHW		, ASTR_CHW },
	{ AID_CK		, ASTR_CK },
	{ AID_CL		, ASTR_CL },
	{ AID_CRE		, ASTR_CRE },
	{ AID_CRP		, ASTR_CRP },
	{ AID_DBIL		, ASTR_DBIL },
	{ AID_EGFR		, ASTR_EGFR },
	{ AID_GGT		, ASTR_GGT },
	{ AID_GLOB		, ASTR_GLOB },
	{ AID_GLU		, ASTR_GLU },
	{ AID_HB		, ASTR_HB },	
	{ AID_HCT		, ASTR_HCT },	
	{ AID_HDL		, ASTR_HDL },
	{ AID_IBIL		, ASTR_IBIL },
	{ AID_K			, ASTR_K },
	{ AID_LAC		, ASTR_LAC },	// GLD
	{ AID_LD		, ASTR_LD },
	{ AID_LDL		, ASTR_LDL },
	{ AID_MG		, ASTR_MG },
	{ AID_NA		, ASTR_NA },
	{ AID_PHOS		, ASTR_PHOS },
	{ AID_T4		, ASTR_T4 },
	{ AID_TBIL		, ASTR_TBIL },
	{ AID_TCH		, ASTR_TCH },
	{ AID_TCO2		, ASTR_TCO2 },
	{ AID_TP		, ASTR_TP },
	{ AID_TRIG		, ASTR_TRIG },
	{ AID_UA		, ASTR_UA },
	{ AID_VLDL		, ASTR_VLDL },
	{ AID_NHDLC		, ASTR_NHDLC },
    { AID_PHB       , ASTR_PHB },
};


Analytes::
Analytes()
{
	int i;
	
	memset( analyteNames, 0, sizeof( analyteNames ) );
	
	for ( i = 0; i < AID_NUM_ANALYTES; i++ )
	{
		analyteNames[ ANALYTES[ i ].analyteId ] = ANALYTES[ i ].stringId;
	}
}

const wchar_t * 
Analytes:: GetAnalysisString (AnalyteId_t analyteId, AStrings *strings)
{
	if ( analyteId >= 0 && analyteId < AID_NUM_ANALYTES )
	{
		return (*strings)[ analyteNames[ analyteId ] ];
	}
	else
	{
		return L"";
	}
}


AnalyteId_t
Analytes::
GetAnalyteId( AnalyteResultType_t resultAnalyte )
{
	AnalyteId_t analyte;
	
	switch ( resultAnalyte )
	{
		case AG:		analyte = AID_AG;		break;		// 4
		case ALB_BCG:	analyte = AID_ALB;		break;		// 5
		case ALB_BCP:	analyte = AID_ALB;		break;		// 6
		case ALP:		analyte = AID_ALP;		break;		// 7
		case ALT_1:		analyte = AID_ALT;		break;		// 8
		case AMY:		analyte = AID_AMY;		break;		// 9
		case AST_1:		analyte = AID_AST;		break;		// 10
		case BA_1:		analyte = AID_BA;		break;		// 11
		case BA_2:		analyte = AID_BA;		break;		// 12
		case BUN_1:		analyte = AID_BUN;		break;		// 13
		case CA:		analyte = AID_CA;		break;		// 14
		case CHOL:		analyte = AID_CHOL;		break;		// 15
		case CK_1:		analyte = AID_CK;		break;		// 16
		case CL_1:		analyte = AID_CL;		break;		// 17
		case CL_2:		analyte = AID_CL;		break;		// 18
		case CRE_1:		analyte = AID_CRE;		break;		// 19
		case DBIL:		analyte = AID_DBIL;		break;		// 20
		case GGT:		analyte = AID_GGT;		break;		// 21
		case GLOB:		analyte = AID_GLOB;		break;		// 22
		case GLU:		analyte = AID_GLU;		break;		// 23
 		case HDL_1:		analyte = AID_HDL;		break;		// 24
		case HDL_2:		analyte = AID_HDL;		break;		// 25
		case IBIL:		analyte = AID_IBIL;		break;		// 26
		case K:			analyte = AID_K;		break;		// 27
		case LDH:		analyte = AID_LD;		break;		// 28
		case LDL:		analyte = AID_LDL;		break;		// 29
		case MG:		analyte = AID_MG;		break;		// 30
		case NA_1:		analyte = AID_NA;		break;		// 31
		case NA_2:		analyte = AID_NA;		break;		// 32
		case PHOS:		analyte = AID_PHOS;		break;		// 33
		case T4:		analyte = AID_T4;		break;		// 34
		case TBIL:		analyte = AID_TBIL;		break;		// 35
		case TCH:		analyte = AID_TCH;		break;		// 36
		case TCO2:		analyte = AID_TCO2;		break;		// 37
		case TP:		analyte = AID_TP;		break;		// 38
		case TRIG:		analyte = AID_TRIG;		break;		// 39
		case UA:		analyte = AID_UA;		break;		// 40
		case VLDL:		analyte = AID_VLDL;		break;		// 41
		case CTBIL:		analyte = AID_TBIL;		break;		// 42
		case BA:		analyte = AID_BA;		break;		// 43
		case CL:		analyte = AID_CL;		break;		// 44
		case HDL:		analyte = AID_HDL;		break;		// 45
		case NA:		analyte = AID_NA;		break;		// 46
		case ALT_2:		analyte = AID_ALT;		break;		// 47
		case ALT:		analyte = AID_ALT;		break;		// 48
		case BUN_2:		analyte = AID_BUN;		break;		// 49
		case BUN:		analyte = AID_BUN;		break;		// 50
		case CRE_2:		analyte = AID_CRE;		break;		// 51
		case CRE:		analyte = AID_CRE;		break;		// 52
		case CHW:		analyte = AID_CHW;		break;		// 53
		case ALB_BCP_A:	analyte = AID_ALB;		break;		// 54
		case CRP_1:		analyte = AID_CRP;		break;		// 55
		case CRP_2:		analyte = AID_CRP;		break;		// 56
		case CRP:		analyte = AID_CRP;		break;		// 57
		case EGFR:		analyte = AID_EGFR;		break;		// 58
// NACL - no results printed.
		case LAC:		analyte = AID_LAC;		break;		// 60
		case HB:		analyte = AID_HB;		break;		// 61
		case HCT:		analyte = AID_HCT;		break;		// 62
		case NHDLC:		analyte = AID_NHDLC;	break;		// 63
		case AST_2:		analyte = AID_AST;		break;		// 64
		case AST:		analyte = AID_AST;		break;		// 65
		case CK_2:		analyte = AID_CK;		break;		// 66
		case CK:		analyte = AID_CK;		break;		// 67
        case PHB:       analyte = AID_PHB;      break;      // 68
		default:
			LogError( "unrecognized result analyte %d", resultAnalyte );
			analyte = AID_ALB;
			break;
	}
	
	return analyte;
}

/*
 * Used by Settings/Ref Ranges when setting reference ranges, and by ResultsPrint
 * when printing all reference ranges.
 */

const AnalyteList_t *
Analytes::
VetScanAnalytesWithRefRanges()
{
	static AnalyteId_t	idList[] = {	AID_ALB,
										AID_ALP,
										AID_ALT,
										AID_AMY,
										AID_AST,
										AID_BA,
										AID_BUN,
										AID_CA,
										AID_CHOL,
										AID_CK,
										AID_CL,
										AID_CRE,
										AID_GGT,
										AID_GLOB,
										AID_GLU,
										AID_K,
										AID_MG,
										AID_NA,
                                        AID_PHB,
										AID_PHOS,
										AID_T4,
										AID_TBIL,
										AID_TCO2,
										AID_TP,
										AID_UA };
	static AnalyteList_t list	= { sizeof( idList ) / sizeof( AnalyteId_t ), idList };
								
	return &list;
}

/*
 * Used by Settings/Units when setting units.  Leaves out any analytes which
 * have no units because they are ratios.
 */

const AnalyteList_t *
Analytes::
VetScanAnalytesWithUnits()
{
	static AnalyteId_t	idList[] = {	AID_ALB,
										AID_ALP,
										AID_ALT,
										AID_AMY,
										AID_AST,
										AID_BA,
										AID_BUN,
										AID_CA,
										AID_CHOL,
										AID_CK,
										AID_CL,
										AID_CRE,
										AID_GGT,
										AID_GLOB,
										AID_GLU,
										AID_K,
										AID_MG,
										AID_NA,
                                        AID_PHB,
										AID_PHOS,
										AID_T4,
										AID_TBIL,
										AID_TCO2,
										AID_TP,
										AID_UA };
	static AnalyteList_t list	= { sizeof( idList ) / sizeof( AnalyteId_t ), idList };
								
	return &list;
}

/*
 * Used by Settings/Ref Ranges when setting reference ranges, and by ResultsPrint
 * when printing all reference ranges.
 */

const AnalyteList_t *
Analytes::
PiccoloAnalytesWithRefRanges()
{
	static AnalyteId_t	idList[] = {	AID_ALB,
										AID_ALP,
										AID_ALT,
										AID_AMY,
										AID_AST,
										AID_BUN,
										AID_CA,
										AID_CHOL,
										AID_CK,
										AID_CL,
										AID_CRE,
										AID_CRP,
										AID_DBIL,
										AID_GGT,
										AID_GLU,
										//AID_HB,
										//AID_HCT,
										AID_HDL,
										AID_K,
										AID_LAC,
										AID_LD,
										AID_LDL,
										AID_MG,
										AID_NA,
										AID_NHDLC,
										AID_PHOS,
										AID_TBIL,
										AID_TCH,
										AID_TCO2,
										AID_TP,
										AID_TRIG,
										AID_UA,
										AID_VLDL,
										 };

	static AnalyteList_t list	= { sizeof( idList ) / sizeof( AnalyteId_t ), idList };
								
	return &list;
}

/*
 * Used by Settings/Units when setting units.  Leaves out any analytes which 
 * have no units because they are ratios.
 */

const AnalyteList_t *
Analytes::
PiccoloAnalytesWithUnits()
{
	static AnalyteId_t	idList[] = {	AID_ALB,
										AID_ALP,
										AID_ALT,
										AID_AMY,
										AID_AST,
										AID_BUN,
										AID_CA,
										AID_CHOL,
										AID_CK,
										AID_CL,	
										AID_CRE,
										AID_CRP,
										AID_DBIL,
										AID_GGT,
										AID_GLU,
									//	AID_HB,
									//	AID_HCT,
										AID_HDL,
										AID_K,
										AID_LAC,
										AID_LD,
										AID_LDL,
										AID_MG,
										AID_NA,
										AID_NHDLC,
										AID_PHOS,
										AID_TBIL,
										AID_TCO2,
										AID_TP,
										AID_TRIG,
										AID_UA,
										AID_VLDL };

	static AnalyteList_t list	= { sizeof( idList ) / sizeof( AnalyteId_t ), idList };
								
	return &list;
}
