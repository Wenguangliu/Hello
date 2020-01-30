#include <stdio.h>

#include "AlgGlobals.h"
#include "FactoryData.h"
#include "FormatResults.h"
#include "Shmem.h"
#include "Species.h"
#include "AStrings.h"
#include "SystemData.h"
#include "Units.h"

// What to print in the value field of a formatted result.
typedef enum
{
	PV_RESULT,
	PV_TILDES,
	PV_LOW_SYSTEM_LIMIT,
	PV_HIGH_SYSTEM_LIMIT,
	PV_LOW_DYNAMIC_LIMIT,
	PV_HIGH_DYNAMIC_LIMIT,
	PV_HEM,
	PV_LIP,
	PV_ICT,
	PV_POS_NEG,
	PV_NOT_COMPUTED,
} PrintedValue_t;

// All the elements of a formatted result.
typedef struct
{
	char			prefix;
	PrintedValue_t	printedValue;
	char			flag;
} PrintLine_t;

// Element of a check-order array for determining what to print for a result.
typedef struct
{
	unsigned		bitMask;
	PrintLine_t		vetScanPrintLine;
	PrintLine_t		piccoloPrintLine;
} CheckOrder_t;

// These are convenience constants for the check order array.
const PrintLine_t RESULT_ONLY			= { ' ',	PV_RESULT,				' ' };
const PrintLine_t TILDES				= { ' ',	PV_TILDES,				' ' };
const PrintLine_t BELOW_SYSTEM			= { '<',	PV_LOW_SYSTEM_LIMIT,	'*' };
const PrintLine_t ABOVE_SYSTEM			= { '>',	PV_HIGH_SYSTEM_LIMIT,	'*' };
const PrintLine_t BELOW_DYNAMIC			= { '<',	PV_LOW_DYNAMIC_LIMIT,	'*' };
const PrintLine_t BELOW_DYNAMIC_SPECIAL	= { '<',	PV_LOW_DYNAMIC_LIMIT,	' ' };
const PrintLine_t ABOVE_DYNAMIC			= { '>',	PV_HIGH_DYNAMIC_LIMIT,	'*' };
const PrintLine_t ABOVE_DYNAMIC_SPECIAL	= { '>',	PV_HIGH_DYNAMIC_LIMIT,	' ' };
const PrintLine_t HEM_ONLY				= { ' ',	PV_HEM,					' ' };	// Normal report, Hemolytic-suppressed
const PrintLine_t LIP_ONLY				= { ' ',	PV_LIP,					' ' };	// Normal report, Lipemic-supressed
const PrintLine_t ICT_ONLY				= { ' ',	PV_ICT,					' ' };	// Normal report, Icteric-suppressed
const PrintLine_t REF_RANGE				= { ' ',	PV_RESULT,				'*' };	// Out of reference range
const PrintLine_t HEM_FLAG				= { ' ',	PV_RESULT,				'H' };	// Error report, Hemolytic-suppressed
const PrintLine_t LIP_FLAG				= { ' ',	PV_RESULT,				'L' };	// Error report, Lipemic-suppressed
const PrintLine_t ICT_FLAG				= { ' ',	PV_RESULT,				'I' };	// Error report, Icteric-suppressed
const PrintLine_t POS_NEG				= { ' ',	PV_POS_NEG,				' ' };
const PrintLine_t NOT_COMPUTED			= { ' ',	PV_NOT_COMPUTED,		' ' };

static Analytes			analytes;
static Species			species;
static Units			units;
static FactoryData_t *	factoryData = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
static SystemData_t *	systemData = (SystemData_t *)MapSharedMem( SYSTEM_SHMEM_NAME, sizeof( SystemData_t ) );
//static AStrings			strings;

static PrintLine_t		GetResultFormat( unsigned resultFlags, AnalyteId_t analyteId, ResultFormatType_t formatType );

/*
 * If both high and low reference range limits are zero, we don't print a
 * reference range.  Also, we only print ref ranges for equine rotors (51 & 59)
 * if the sample type is an equine; and we only print ref ranges for avian/reptilian
 * rotors (45 & 56) if the sample type is a bird or a reptile.  Finally, we
 * don't print ref ranges for cholesterol for rotors 46 & 54 (Lipid Panel &
 * Lipid Panel Plus).
 * 
 * Removed CHOL/rotors 46&54 test, 5/5/2010, per Audie.
 */

bool
SuppressRefRange( float low, float high, int rotorId, AnalyteId_t analyteId, SampleType_t sampleType )
{
     return ( ( (low == 0.0) && (high == 0.0) )                              ||
             ( (rotorId == 45) && !species.IsAvianReptilian( sampleType ) ) ||
             ( (rotorId == 51) && !species.IsEquine( sampleType ) )         ||
             ( (rotorId == 56) && !species.IsAvianReptilian( sampleType ) ) ||
             ( (rotorId == 59) && !species.IsEquine( sampleType ) )         ||
             ( (rotorId == 60) && !species.IsCanine( sampleType ) )/*           ||
             ( (rotorId == 46) && (analyteId == AID_CHOL) )                 ||
             ( (rotorId == 54) && (analyteId == AID_CHOL) ) */);
}

void
FormatValue( float value, char * valueBuff, NumericFormat_t format )
{
    if ( value > 9999.499999 )
    {
        sprintf( valueBuff, "%dK", (int)(value + 500.0) / 1000 );
    }
    else
    {
        switch ( format )
        {
            case Xxxx:
                if ( value < 0.9995)
                {
                    sprintf( valueBuff, ".%03.0f", value * 1000.0 );
                    break;
                }
            // Note: Fall-through in the case where the above test is false is intentional.
            case Xxx:
                if ( value < 9.995 )
                {
                    sprintf( valueBuff, "%0.2f", value );
                    break;
                }
            // Fall-through intentional.
            case Xx:
                if ( value < 99.95 )
                {
                    sprintf( valueBuff, "%0.1f", value );
                    break;
                }
                // Fall-through intentional.
            case X:
                sprintf( valueBuff, "%0.0f", value );
                break;
            case X0:
                sprintf( valueBuff, "%d0", (int)(value + 5.0) / 10 );
                break;
            case X00:
                sprintf( valueBuff, "%d00", (int)(value + 50.0) / 100 );
                break;
        }
    }
}

char *
FormatResult( ResultsDbEntry_t * entry, int resultIndex, ResultFormatType_t formatType, AStrings *strings )
{
	static char 			resultBuff[20];
	char					valueBuff[20];
	PrintLine_t				format;
	float					result = 0.0;
	AnalyteId_t				analyteId;
	RotorAnalyteResult_t *	resultRecord;
	AnalyteResultType_t		resultId;
	
	// Point to the result record.
	resultRecord = &entry->rotorAnalyteResults[ resultIndex ];

	// Get the result ID, which is the cuvette result type (e.g., CK, CK_1, CK_2, etc.)
	resultId = (AnalyteResultType_t)resultRecord->analyteType;
	
	// Get the analyte ID.
	analyteId = analytes.GetAnalyteId( resultId );

	// Get the result format.
	format = GetResultFormat( resultRecord->resultPrintFlags, analyteId, formatType );

	// Get the value we're going to print in the result field.
	switch ( format.printedValue )
	{
		case PV_LOW_SYSTEM_LIMIT:	result = resultRecord->lowSystemRangeLimit;		break;
		case PV_HIGH_SYSTEM_LIMIT:	result = resultRecord->highSystemRangeLimit;	break;
		case PV_LOW_DYNAMIC_LIMIT:	result = resultRecord->lowDynamicRangeLimit;	break;
		case PV_HIGH_DYNAMIC_LIMIT:	result = resultRecord->highDynamicRangeLimit;	break;
		case PV_POS_NEG:
			if ( formatType == FMT_ERROR_RESEARCH )
			{
				format.printedValue = PV_RESULT;
				result = resultRecord->analyteResult;
			}
			break;
		default:					result = resultRecord->analyteResult;			break;
	}

	// Convert the result to the current units.
	result *= units[ analyteId ].mult;
	
	// If the result or range limit that we're printing is less than zero, we use
	// zero for the value and clear the prefix.
	if ( result <= 0.0 )
	{
		result = 0.0;
		format.prefix = ' ';
	}
	
/*
 * NOTE: The SendXml code depends on the format of the result, so any changes to the format will probably
 * require changes to that code, too.
 */

	// Format the result value.
	switch ( format.printedValue )
	{
		case PV_RESULT:
		case PV_LOW_SYSTEM_LIMIT:	
		case PV_HIGH_SYSTEM_LIMIT:
		case PV_LOW_DYNAMIC_LIMIT:
		case PV_HIGH_DYNAMIC_LIMIT:
		default:
            FormatValue( result, valueBuff, units[ analyteId ].format );
			break;
		case PV_TILDES:
			strcpy( valueBuff, "~~~" );
			break;
		case PV_HEM:
			strcpy( valueBuff, "HEM" );
			break;
		case PV_LIP:
			strcpy( valueBuff, "LIP" );
			break;
		case PV_ICT:
			strcpy( valueBuff, "ICT" );
			break;
		case PV_POS_NEG:
			sprintf( valueBuff, "%S", (resultRecord->analyteFlags & POS_NEG_RESULT_POSITIVE) ? (*strings)[ RSTR_POS ] : (*strings)[ RSTR_NEG ] );
			break;
		case PV_NOT_COMPUTED:
			// strcpy( valueBuff, (*strings)[ RSTR_NC ] );
            sprintf (valueBuff, "%S",(*strings)[ RSTR_NC ]);
			break;
	}


	// Create the full result, with prefix, value and flag fields.
	if ( format.printedValue == PV_RESULT )	// No prefix for normal results.
	{
		sprintf( resultBuff, "%5s %c", valueBuff, format.flag );
	}
	else
	{
        // If our result rounded to 0.00, and we've got a '<' prefix, we need to
        // print " 0.00".
        if ( (format.prefix == '<') && (strcmp( valueBuff, "0.00" ) == 0) )
        {
            format.prefix = ' ';
        }
		sprintf( resultBuff, "%c%4s %c", format.prefix, valueBuff, format.flag );
	}
		
	return resultBuff;
}


PrintLine_t
GetResultFormat( unsigned resultFlags, AnalyteId_t analyteId, ResultFormatType_t formatType )
{
	int						i;
	PrintLine_t				format = RESULT_ONLY;
	bool					isPiccolo = (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE);
	const CheckOrder_t *	checkOrder;
	const CheckOrder_t		normalCheckOrder[] =
	{
//		  Bit mask						// VetScan					// piccolo
		{ MASK_NOT_COMPUTED,			NOT_COMPUTED,				NOT_COMPUTED },		
		{ MASK_HEMOLYZED_SUPPRESSED,	HEM_ONLY, 					HEM_ONLY },				
		{ MASK_LIPEMIC_SUPPRESSED,		LIP_ONLY, 					LIP_ONLY },
		{ MASK_ICTERIC_SUPPRESSED,		ICT_ONLY, 					ICT_ONLY },
		{ MASK_RESULTS_SUPPRESSED,		TILDES, 					TILDES },
        { MASK_LOW_SYSTEM_LIMIT,        TILDES,                     TILDES },
        { MASK_HIGH_SYSTEM_LIMIT,       TILDES,                     TILDES },
		{ MASK_LOW_DYNAMIC_SPECIAL,		BELOW_DYNAMIC_SPECIAL,		BELOW_DYNAMIC_SPECIAL },
		{ MASK_LOW_DYNAMIC_LIMIT,		BELOW_DYNAMIC,				BELOW_DYNAMIC },
		{ MASK_HIGH_DYNAMIC_SPECIAL,	ABOVE_DYNAMIC_SPECIAL,		ABOVE_DYNAMIC_SPECIAL },
		{ MASK_HIGH_DYNAMIC_LIMIT,		ABOVE_DYNAMIC,				ABOVE_DYNAMIC },
		{ MASK_HEMOLYZED_LOW,			RESULT_ONLY,				{ '<', PV_RESULT, 'H' } },
		{ MASK_HEMOLYZED_HIGH,			RESULT_ONLY,				{ '>', PV_RESULT, 'H' } },	
		{ MASK_HEMOLYZED,				RESULT_ONLY,				HEM_FLAG },
		{ MASK_LIPEMIC_LOW,				RESULT_ONLY,				{ '<', PV_RESULT, 'L' } },
		{ MASK_LIPEMIC_HIGH,			RESULT_ONLY,				{ '>', PV_RESULT, 'L' } },
		{ MASK_LIPEMIC,					RESULT_ONLY,				LIP_FLAG },
		{ MASK_ICTERIC_LOW,				RESULT_ONLY,				{ '<', PV_RESULT, 'I' } },	
		{ MASK_ICTERIC_HIGH,			RESULT_ONLY,				{ '>', PV_RESULT, 'I' } },
		{ MASK_ICTERIC,					RESULT_ONLY,				ICT_FLAG },
		{ MASK_REF_RANGE_LIMIT,			REF_RANGE,					REF_RANGE },
		{ MASK_DERIVED_RESULT,			RESULT_ONLY,				{ ' ', PV_RESULT, 'c' } },
		{ MASK_POS_NEG_RESULT,			POS_NEG,					POS_NEG },					
		{ MASK_END_FLAG, 				{}, {} },
	};

	const CheckOrder_t	errorCheckOrder[] =
	{
//		  Bit mask						// VetScan					// piccolo
		{ MASK_NOT_COMPUTED,			NOT_COMPUTED,				NOT_COMPUTED },		
		{ MASK_HEMOLYZED_SUPPRESSED,	HEM_FLAG, 					HEM_ONLY },					
		{ MASK_LIPEMIC_SUPPRESSED,		LIP_FLAG, 					LIP_ONLY },
		{ MASK_ICTERIC_SUPPRESSED,		ICT_FLAG, 					ICT_ONLY },
		{ MASK_RESULTS_SUPPRESSED,		{ ' ', PV_RESULT, '~' },	TILDES },
        { MASK_LOW_SYSTEM_LIMIT,        TILDES,                     TILDES },
        { MASK_HIGH_SYSTEM_LIMIT,       TILDES,                     TILDES },
		{ MASK_LOW_DYNAMIC_SPECIAL,		RESULT_ONLY,				BELOW_DYNAMIC_SPECIAL },
		{ MASK_LOW_DYNAMIC_LIMIT,		RESULT_ONLY,				BELOW_DYNAMIC },
		{ MASK_HIGH_DYNAMIC_SPECIAL,	RESULT_ONLY,				ABOVE_DYNAMIC_SPECIAL },
		{ MASK_HIGH_DYNAMIC_LIMIT,		RESULT_ONLY,				ABOVE_DYNAMIC },
		{ MASK_HEMOLYZED_LOW,			RESULT_ONLY,				{ '<', PV_RESULT, 'H' } },
		{ MASK_HEMOLYZED_HIGH,			RESULT_ONLY,				{ '>', PV_RESULT, 'H' } },	
		{ MASK_HEMOLYZED,				RESULT_ONLY,				HEM_FLAG },
		{ MASK_LIPEMIC_LOW,				RESULT_ONLY,				{ '<', PV_RESULT, 'L' } },
		{ MASK_LIPEMIC_HIGH,			RESULT_ONLY,				{ '>', PV_RESULT, 'L' } },
		{ MASK_LIPEMIC,					RESULT_ONLY,				LIP_FLAG },
		{ MASK_ICTERIC_LOW,				RESULT_ONLY,				{ '<', PV_RESULT, 'I' } },	
		{ MASK_ICTERIC_HIGH,			RESULT_ONLY,				{ '>', PV_RESULT, 'I' } },
		{ MASK_ICTERIC,					RESULT_ONLY,				ICT_FLAG },
		{ MASK_REF_RANGE_LIMIT,			REF_RANGE,					REF_RANGE },
		{ MASK_DERIVED_RESULT,			RESULT_ONLY,				{ ' ', PV_RESULT, 'c' } },
		{ MASK_POS_NEG_RESULT,			POS_NEG,					POS_NEG },				
		{ MASK_END_FLAG, 				{}, {} },
	};

    /* saved for future used
    const CheckOrder_t  errorCheckOrder[] =
    {
//        Bit mask                      // VetScan                  // piccolo
        { MASK_NOT_COMPUTED,            NOT_COMPUTED,               NOT_COMPUTED },
        { MASK_HEMOLYZED_SUPPRESSED,    HEM_FLAG,                   HEM_ONLY },
        { MASK_LIPEMIC_SUPPRESSED,      LIP_FLAG,                   LIP_ONLY },
        { MASK_ICTERIC_SUPPRESSED,      ICT_FLAG,                   ICT_ONLY },
        { MASK_RESULTS_SUPPRESSED,      { ' ', PV_RESULT, '~' },    { ' ', PV_RESULT, '~' } },
        { MASK_LOW_SYSTEM_LIMIT,        BELOW_SYSTEM,               BELOW_SYSTEM },
        { MASK_HIGH_SYSTEM_LIMIT,       ABOVE_SYSTEM,               ABOVE_SYSTEM },
        { MASK_LOW_DYNAMIC_SPECIAL,     RESULT_ONLY,                RESULT_ONLY },
        { MASK_LOW_DYNAMIC_LIMIT,       RESULT_ONLY,                RESULT_ONLY },
        { MASK_HIGH_DYNAMIC_SPECIAL,    RESULT_ONLY,                RESULT_ONLY },
        { MASK_HIGH_DYNAMIC_LIMIT,      RESULT_ONLY,                RESULT_ONLY },
        { MASK_HEMOLYZED_LOW,           RESULT_ONLY,                RESULT_ONLY },
        { MASK_HEMOLYZED_HIGH,          RESULT_ONLY,                RESULT_ONLY },
        { MASK_HEMOLYZED,               RESULT_ONLY,                RESULT_ONLY },
        { MASK_LIPEMIC_LOW,             RESULT_ONLY,                RESULT_ONLY },
        { MASK_LIPEMIC_HIGH,            RESULT_ONLY,                RESULT_ONLY },
        { MASK_LIPEMIC,                 RESULT_ONLY,                RESULT_ONLY },
        { MASK_ICTERIC_LOW,             RESULT_ONLY,                RESULT_ONLY },
        { MASK_ICTERIC_HIGH,            RESULT_ONLY,                RESULT_ONLY },
        { MASK_ICTERIC,                 RESULT_ONLY,                RESULT_ONLY },
        { MASK_REF_RANGE_LIMIT,         REF_RANGE,                  REF_RANGE },
        { MASK_DERIVED_RESULT,          RESULT_ONLY,                { ' ', PV_RESULT, 'c' } },
        { MASK_POS_NEG_RESULT,          POS_NEG,                    POS_NEG },
        { MASK_END_FLAG,                {}, {} },
    };
  */

	
	// For CHW, we only use the normal format if we're not in Research mode.
	if ( (analyteId == AID_CHW) && !systemData->inResearchMode )
	{
		checkOrder = &normalCheckOrder[0];
	}
	else
	{
		checkOrder = (formatType == FMT_NORMAL) ? &normalCheckOrder[0] : &errorCheckOrder[0];
	}
	
	for ( i = 0; checkOrder[i].bitMask != MASK_END_FLAG; i++ )
	{
		if ( (resultFlags & checkOrder[i].bitMask) != 0 )
		{
			if ( isPiccolo )
			{
				format = checkOrder[i].piccoloPrintLine;
			}
			else
			{
				format = checkOrder[i].vetScanPrintLine;
			}
			break;
		}
	}

	// If we have a dilution check error and this is a piccolo, the flag is always
	// a bang regardless of what else we've got for the value.
	if ( isPiccolo && ( (resultFlags & MASK_DILUTION_CHECK) != 0 ) )
	{
		format.flag = '!';
	}

	return format;
}
	
char *
FormatRefRange( ResultsDbEntry_t * entry, int resultIndex )
{
	static char buff[20];
	AnalyteId_t	analyteId = analytes.GetAnalyteId( (AnalyteResultType_t)entry->rotorAnalyteResults[ resultIndex ].analyteType );
	float		low;
	float		high;

	// Get the range limits.
	low = entry->rotorAnalyteResults[ resultIndex ].lowReferenceRangeLimit;
	high = entry->rotorAnalyteResults[ resultIndex ].highReferenceRangeLimit;

	if ( SuppressRefRange( low, high, entry->rotorId, analyteId, (SampleType_t)entry->sampleType ) )
	{
		sprintf( buff, "%9s", " " );
	}

	else
	{
		return FormatRefRange( analyteId, low, high );
	}
		
	return buff;
}

char *
FormatRefRange( AnalyteId_t analyteId, float low, float high )
{
	static char buff[20];
    char        lowBuff[20];
    char        highBuff[20];
    char        highRange[20];

	// Convert the range values to the current units for this analyte.
	low *= units[ analyteId ].mult;
	high *= units[ analyteId ].mult;

    if ( high > 9999.499999 )
    {
        // The range high is formatted separately so we can end up with a left-justified
        // value with the 'K' at the end, and no spaces between the digits and the 'K'.
        sprintf( highRange, "%dK", (int)(high + 500.0) / 1000 );
        sprintf( buff, "%3dK-%-4s", (int)(low + 500.0) / 1000, highRange );
    }
    else
    {
        FormatValue( low, lowBuff, units[ analyteId ].format );
        FormatValue( high, highBuff, units[ analyteId ].format );
        sprintf( buff, "%4s-%-4s", lowBuff, highBuff );
    }
	return buff;
}
