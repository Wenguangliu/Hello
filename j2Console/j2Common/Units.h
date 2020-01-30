#ifndef __UNITS_H_INCLUDED
#define __UNITS_H_INCLUDED

#include <string.h>

#include "Analytes.h"
#include "AStrings.h"
#include "CommonDefines.h"
#include "CfgErr.h"

// Increment if we get more analytes, etc.
// NOTE: We may be able to read the old file if we just add new analytes to the
// end of the list - they need to be initialized first, then.
#define UNITS_FILE_VERSION	3

#define NON_SI_DEFAULT_NDX	0	// Index for the default for non-SI units
#define SI_DEFAULT_NDX		1	// Index for the default for SI units

/*
 * IDs for each of the possible units settings.  These values are stored in
 * the units settings file, so if new units are added they should be added only
 * to the end, and if any units are deleted some means should be used to avoid
 * shifting the entries after the deleted units.
 */
typedef enum
{
    UID_NONE = 1,

    UID_UL,
    UID_GDL,
    UID_GL,
    UID_MEQL,
    UID_MGDL,
    UID_UGDL,
    UID_MGL,
    UID_UGL,
    UID_UGML,
    UID_UKATL,
    UID_MMOLL,
    UID_UMOLL,
    UID_NKATL,
    UID_NMOLL,
    UID_MLMIN,
    UID_PCT,
    UID_FRAC,

} UnitId_t;

// Data for a given unit selection.
typedef struct
{
	UnitId_t		unitId;
	NumericFormat_t	format;
	float			mult;
} UnitData_t;

// Structure to hold the possible units for an analyte type - used by Settings/Units.
typedef struct
{
	int			count;
	UnitId_t	current;
	UnitData_t	units[5];
} UnitOptions_t;

typedef struct
{
	int			version;
	UnitId_t	current[ AID_NUM_ANALYTES ];
} UnitsConfig_t;

class Units
{
public:
					Units();
	UnitData_t		operator[]( AnalyteId_t analyteId );
    const wchar_t * ListString( UnitId_t unitId, AStrings *strings);
	UnitOptions_t *	Options( AnalyteId_t analyte );
	CfgError_t		SetAllNonSi( bool doWrite = true );
	CfgError_t		SetAllSi();
	CfgError_t		SetElectrolytes( UnitId_t unitId );
	CfgError_t		SetEnzymes( UnitId_t unitId );
	CfgError_t		SetLipids( UnitId_t unitId );
	CfgError_t		SetMinerals( UnitId_t unitId );
	CfgError_t		SetProteins( UnitId_t unitId );
	CfgError_t		SetToDefaults( bool doWrite = true );
	const wchar_t *	String( AnalyteId_t analyteId );
	const wchar_t *	String( UnitId_t unitId );
	CfgError_t		Update( AnalyteId_t analyte, UnitId_t unitId );

private:
	CfgError_t		WriteUnitsFile();

	UnitsConfig_t *	unitsConfig;
	UnitOptions_t	options[ AID_NUM_ANALYTES ];
   // AStrings        *strings;
};

#endif	// __UNITS_H_INCLUDED
