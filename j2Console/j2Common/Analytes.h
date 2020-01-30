#ifndef __ANALYTES_H_INCLUDED
#define __ANALYTES_H_INCLUDED

#include "AStrings.h"
#include "CommonDefines.h"

/*
 *  Index values used for display of results, Settings/Units and
 *	Settings/RefRanges.  Note that there is also an AnalyteResultType_t in
 * 	CommonDefines.h.  Those index values are used only for the results data.
 * 	The saved config files for both Units and RefRanges use these values, so
 * 	additional analytes should only be added to the end of this list unless
 * 	the config files are either converted or discarded.
 */
 
typedef enum
{
    AID_NONE = -1,

    AID_AG = 0,
    AID_ALB,
    AID_ALP,
    AID_ALT,
    AID_AMY,
    AID_AST,
    AID_BA,
    AID_BUN,
    AID_CA,
    AID_CHOL,
    AID_CHW,
    AID_CK,
    AID_CL,
    AID_CRE,
    AID_CRP,
    AID_DBIL,
    AID_EGFR,
    AID_GGT,
    AID_GLOB,
    AID_GLU,
    AID_HB,
    AID_HCT,
    AID_HDL,
    AID_IBIL,
    AID_K,
    AID_LAC,
    AID_LD,
    AID_LDL,
    AID_MG,
    AID_NA,
    AID_NHDLC,
    AID_PHB,
    AID_PHOS,
    AID_T4,
    AID_TBIL,
    AID_TCH,
    AID_TCO2,
    AID_TP,
    AID_TRIG,
    AID_UA,
    AID_VLDL,
    
    AID_NUM_ANALYTES

} AnalyteId_t;

typedef struct
{
	int				len;
	AnalyteId_t *	list;
} AnalyteList_t;

class Analytes
{
public:
							Analytes();
	//const char *			operator[]( AnalyteId_t analyteId );
    const wchar_t *         GetAnalysisString ( AnalyteId_t analyteId, AStrings *string);
	const AnalyteList_t *	VetScanAnalytesWithRefRanges();
	const AnalyteList_t *	VetScanAnalytesWithUnits();
	const AnalyteList_t *	PiccoloAnalytesWithRefRanges();
	const AnalyteList_t *	PiccoloAnalytesWithUnits();
	static AnalyteId_t		GetAnalyteId( AnalyteResultType_t resultAnalyte );

private:
	StringId_t	analyteNames[ AID_NUM_ANALYTES ];
	//AStrings	strings;
};

#endif	// __ANALYTES_H_INCLUDED
