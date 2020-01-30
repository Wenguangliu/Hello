#include "Loinc.h"
#include "Units.h"

const char *
GetLoinc( AnalyteId_t analyteId )
{
	Units		units;
	UnitId_t	unitId;
	
	unitId = units[ analyteId ].unitId;

	switch ( analyteId )
	{
		case AID_AG:
			switch ( unitId )
			{
				case UID_MEQL:	return "^^^Anion Gap4 SerPl-cCnc";				break;
				default:		return "1863-0^^LN^Anion Gap4 SerPl-sCnc";		break;
			}
			break;
		case AID_ALB:
			switch ( unitId )
			{
				case UID_UMOLL:	return "^^^Alb SerPl-sCnc";						break;
				default:		return "1751-7^^LN^Alb SerPl-mCnc";				break;
			}
			break;
		case AID_ALP:	return "6768-6^^LN^ALP SerPl-cCnc";						break;
		case AID_ALT:	return "1742-6^^LN^ALT SerPl-cCnc";						break;
		case AID_AMY:	return "1798-8^^LN^Amylase SerPl-cCnc";					break;
		case AID_AST:	return "1920-8^^LN^AST SerPl-cCnc";						break;
		case AID_BA:
			switch ( unitId )
			{
				case UID_UMOLL:
				case UID_MMOLL:	return "^^^Bile Ac SerPl-sCnc";					break;
				default:		return "^^^Bile Ac SerPl-mCnc";					break;
			}
			break;
		case AID_BUN:
			switch ( unitId )
			{
				case UID_MGDL:	return "3094-0^^LN^BUN SerPl-mCnc";				break;
				default:		return "14937-7^^LN^BUN SerPl-sCnc";			break;
			}
			break;
		case AID_CA:
			switch ( unitId )
			{
				case UID_MGDL:	return "17861-6^^LN^Calcium SerPl-mCnc";		break;
				case UID_MMOLL:	return "2000-8^^LN^Calcium SerPl-sCnc";			break;
				default:		return "^^^Calcium SerPl-cCnc";					break;
			}
			break;
		case AID_CHOL:
			switch ( unitId )
			{
				case UID_MMOLL:	return "14647-2^^LN^Cholest SerPl-sCnc";		break;
				default:		return "2093-3^^LN^Cholest SerPl-mCnc";			break;
			}
			break;
		case AID_CHW:	return "^^^CHWc SerPl-mRto";							break;
		case AID_CK:	return "2157-6^^LN^CK SerPl-cCnc";						break;
		case AID_CL:
			switch ( unitId )
			{
				case UID_MEQL:	return "^^^Chloride SerPl-cCnc";				break;
				default:		return "2075-0^^LN^Chloride SerPl-sCnc";		break;
			}
			break;
		case AID_CRE:
			switch ( unitId )
			{
				case UID_MGDL:	return "2160-0^^LN^Creat SerPl-mCnc";			break;
				default:		return "14682-9^^LN^Creat SerPl-sCnc";			break;
			}
			break;
		case AID_CRP:	return "1988-5^^LN^CRP SerPl-mCnc";						break;
		case AID_DBIL:
			switch ( unitId )
			{
				case UID_MGDL:	return "1968-7^^LN^Bilirub Direct SerPl-mCnc";	break;
				default:		return "14629-0^^LN^Bilirub Direct SerPl-sCnc";	break;
			}
			break;
		case AID_EGFR:	return "33914-3^^LN^Pred GFR SerPl-vRate";				break;
		case AID_GGT:	return "2324-2^^LN^GGT SerPl-cCnc";						break;
		case AID_GLOB:	return "^^^Globulin SerPl Calc-mCnc";					break;
		case AID_GLU:
			switch ( unitId )
			{
				case UID_MMOLL:	return "14749-6^^LN^Glucose SerPl-sCnc";		break;
				default:		return "2345-7^^LN^Glucose SerPl-mCnc";			break;
			}
			break;
		case AID_HB:
			switch ( unitId )
			{
				case UID_MMOLL:	return "^^^Hgb Bld--sCnc";						break;
				default:		return "718-7^^LN^Hgb Bld-mCnc";				break;
			}
			break;
		case AID_HCT:			return "^^^Hct fr Bld Calc";					break;
		case AID_HDL:
			switch ( unitId )
			{
				case UID_MMOLL:	return "14646-4^^LN^HDLc SerPl-sCnc";			break;
				default:		return "2085-9^^LN^HDLc SerPl-mCnc";			break;
			}
			break;
		case AID_IBIL:	return " ";												break;
		case AID_K:
			switch ( unitId )
			{
				case UID_MMOLL:	return "2823-3^^LN^Potassium SerPl-sCnc";		break;
				default:		return "^^^Potassium SerPl-cCnc";				break;
			}
			break;
		case AID_LAC:
			switch ( unitId )
			{
				case UID_MMOLL:	return "2524-7^^LN^Lactate SerPl-sCnc";			break;
				default:		return "14118-4^^LN^Lactate SerPl-mCnc";		break;
			}
			break;
		case AID_LD:	return "14804-9^^LN^LDH SerPL L to P-cCnc";				break;
		case AID_LDL:
			switch ( unitId )
			{
				case UID_MGDL:	return "13457-7^^LN^LDLc SerPl Calc-mCnc";		break;
				default:		return "^^^LDLc SerPl Calc-sCnc";				break;
			}
			break;
		case AID_MG:
			switch ( unitId )
			{
				case UID_MGDL:	return "19123-9^^LN^Magnesium SerPl-mCnc";		break;
				case UID_MMOLL:	return "2601-3^^LN^Magnesium SerPl-sCnc";		break;
				default:		return "^^^Magnesium SerPl-cCnc";				break;
			}
			break;
		case AID_NA:
			switch ( unitId )
			{
				case UID_MMOLL:	return "2951-2^^LN^Sodium SerPl-sCnc";			break;
				default:		return "^^^Sodium SerPl-cCnc";					break;
			}
			break;
		case AID_NHDLC:
			switch ( unitId )
			{
				case UID_MMOLL:	return "^^^NonHDLc SerPl-sCnc";					break;
				case UID_MGL:	return "43396-1^^LN^NonHDLc SerPl-mCnc";		break;
				default:		return "43396-1^^LN^NonHDLc SerPl-mCnc";		break;
			}
			break;
		case AID_PHB:
			switch ( unitId )
			{
				case UID_UGML:	return "3948-7^^LN^Phenobarb SerPl-mCnc";		break;
				case UID_UMOLL:	return "14874-2^^LN^Phenobarb SerPl-sCnc";		break;
				default:		return "14874-2^^LN^Phenobarb SerPl-sCnc";		break;
			}
			break;
		case AID_PHOS:
			switch ( unitId )
			{
				case UID_MGDL:	return "2777-1^^LN^Phosphate SerPl-mCnc";		break;
				default:		return "14879-1^^LN^Phosphate SerPl-sCnc";		break;
			}
			break;
		case AID_T4:
			switch ( unitId )
			{
				case UID_NMOLL:	return "14921-1^^LN^T4 SerPl-sCnc";				break;
				default:		return "3026-2^^LN^T4 SerPl-mCnc";				break;
			}
			break;
		case AID_TBIL:
			switch ( unitId )
			{
				case UID_MGDL:	return "1975-2^^LN^Bilirub SerPl-mCnc";			break;
				default:		return "14631-6^^LN^Bilirub SerPl-sCnc";		break;
			}
			break;
		case AID_TCH:	return "9830-1^^LN^Cholest/HDLc SerPl-mRto";			break;
		case AID_TCO2:
			switch ( unitId )
			{
				case UID_MMOLL:	return "2028-9^^LN^CO2 SerPl-sCnc";				break;
				default:		return "^^^CO2 SerPl-cCnc";						break;
			}
			break;
		case AID_TP:	return "2885-2^^LN^Prot SerPl-mCnc";					break;
		case AID_TRIG:
			switch ( unitId )
			{
				case UID_MGDL:	return "2571-8^^LN^Trigl SerPl-mCnc";			break;
				default:		return "14927-8^^LN^Trigl SerPl-sCnc";			break;
			}
			break;
		case AID_UA:
			switch ( unitId )
			{
				case UID_MGDL:	return "3084-1^^LN^Urate SerPl-mCnc";			break;
				default:		return "14933-6^^LN^Urate SerPl-sCnc";			break;
			}
			break;
		case AID_VLDL:
			switch ( unitId )
			{
				case UID_MGDL:	return "13458-5^^LN^VLDLc SerPl Calc-mCnc";		break;
				default:		return "^^^VLDLc SerPl Calc-sCnc";				break;
			}
			break;
		default:	return "";													break;
	}
}


