#ifndef __CALCBFDDATA_H_INCLUDED
#define __CALCBFDDATA_H_INCLUDED


#include "CommonDefines.h"


#define	MAX_BFD_POINTS		21	


typedef enum
{
	NADH_CURVE_IDX		= 0, 
	PNP_CURVE_IDX		= 1,	
	ALB_A_CURVE_IDX		= 2,
	ALB_B_CURVE_IDX		= 3,
	ALB_C_CURVE_IDX		= 4,
	TP_A_CURVE_IDX		= 5,
	TP_B_CURVE_IDX		= 6,
	TBIL_A_CURVE_IDX	= 7,
	TBIL_B_CURVE_IDX	= 8,
	CRE_A_CURVE_IDX		= 9,
	CRE_B_CURVE_IDX		= 10,
	AMY_A_CURVE_IDX		= 11,
	AMY_B_CURVE_IDX		= 12,
	CA_A_CURVE_IDX		= 13,
	CA_B_CURVE_IDX		= 14,
	CA_C_CURVE_IDX		= 15,
	UA_A_CURVE_IDX		= 16,
	UA_B_CURVE_IDX		= 17,
	GGT_A_CURVE_IDX		= 18,
	GGT_B_CURVE_IDX		= 19,
	HDL_A_CURVE_IDX		= 20,
	HDL_B_CURVE_IDX		= 21,
	TRIG_CURVE_IDX		= 22,
	ALB_D_CURVE_IDX		= 23,
	ALB_E_CURVE_IDX		= 24,
	ALB_F_CURVE_IDX		= 25,
	NA_CURVE_IDX		= 26,
	LDH_A_CURVE_IDX		= 27,
	LDH_B_CURVE_IDX		= 28,
	BA_A_CURVE_IDX		= 29,
	BA_B_CURVE_IDX		= 30,
	ALB_G_CURVE_IDX		= 31,
	LAC_A_CURVE_IDX		= 32,
	LAC_B_CURVE_IDX		= 33,
	HB_CURVE_IDX		= 34,

	MAX_DYE_CURVES,

	NO_DYE_IDX   = MAX_DYE_CURVES
} DyeCurveIndex_t;


typedef enum
{
	WL_340_FILTER_IDX = 0, 
	WL_405_FILTER_IDX = 1,	
	WL_467_FILTER_IDX = 2,
	WL_500_FILTER_IDX = 3,
	WL_515_FILTER_IDX = 4,
	WL_550_FILTER_IDX = 5,
	WL_600_FILTER_IDX = 6,
	WL_630_FILTER_IDX = 7,
	WL_850_FILTER_IDX = 8,

	NO_FILTER_IDX
} WavelengthFilterIndex_t;


typedef struct
{
	DyeCurveIndex_t				primaryDyeCurveIndex;
	WavelengthFilterIndex_t		primaryWavelengthFilterCurveIndex;

	DyeCurveIndex_t				secondaryDyeCurveIndex;
	WavelengthFilterIndex_t		secondaryWavelengthFilterCurveIndex;

} BfdFactorIndices_t;

extern const BfdFactorIndices_t	bfdFactorIndices[ MAX_BFD_CALS ];
extern const float				bfdDyeCurves[ MAX_DYE_CURVES ][ MAX_BFD_POINTS ];

#endif	// __CALCBFDDATA_H_INCLUDED
