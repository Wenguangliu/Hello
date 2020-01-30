// ********************************************************************
//
// FILENAME:  
// 
// 		BarcodeProcessing.cc
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Top level Console process main() that implements Engine bar code data processing
//		for interleaved 2 of 5 (I 2 of 5) bar code types
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//

#include <time.h>
#include <stdlib.h>
#include <stdio.h>	// GLD - for debugging
#include <string.h>	// GLD - for memset

#include "CommonDefines.h"
#include "RotorBarcode.h"
#include "Crc16.h"
#include "SystemError.h"
#include "LogErr.h"

// misc. definitions

#define UNKNOWN_ROTOR_ID					0xff
#define NOT_I2OF5_CHAR						0xff

#define NUM_BARCODE_DIGITS_TO_CRC			104

#define BAR_TRANSITION_MASK					0x8000

#define	NUM_START_STOP_TRANSITIONS			4
#define NUM_2_DIGIT_TRANSITIONS				10

#define	TRANSITION_MATCH_MASK				0x00000003		// mask 2 lsb to match bar/space type

// match values to start & stop char created / concatenated
															//									   nb ns nb ns                                                       
#define START_CHAR_PATTERN					0x00000022		// 00 00 00 00 00 00 00 00 00 00 00 00 00 10 00 10
															//                                     wb ns nb ws                
#define STOP_CHAR_PATTERN					0x00000063		// 00 00 00 00 00 00 00 00 00 00 00 00 01 10 00 11


// bar/space transition types

typedef enum
{
	NARROW_BAR,							
	WIDE_BAR,							
	NARROW_SPACE,						
	WIDE_SPACE
} BarSpaceType_t;							


//	processed rotor barcode data

static BarcodeData_t *	barcodeData_m;

// conversion table for interleaved I2of5 code
//   1 = wide bar or wide space
//   0 = narrow bar or narrow space

static const char	i2Of5ConvertTable_m[32] =
{
	// 00000           00001           00010		   00011		   00100		   00101		   00110		   00111
	NOT_I2OF5_CHAR, NOT_I2OF5_CHAR, NOT_I2OF5_CHAR,      7,         NOT_I2OF5_CHAR,      4,              0,         NOT_I2OF5_CHAR,

	// 01000           01001           01010		   01011		   01100		   01101		   01110		   01111
	NOT_I2OF5_CHAR,      2,              9,         NOT_I2OF5_CHAR,      6,         NOT_I2OF5_CHAR, NOT_I2OF5_CHAR, NOT_I2OF5_CHAR,

	// 10000           10001           10010		   10011		   10100		   10101		   10110		   10111
	NOT_I2OF5_CHAR,      1,              8,         NOT_I2OF5_CHAR,      5,         NOT_I2OF5_CHAR, NOT_I2OF5_CHAR, NOT_I2OF5_CHAR,

	// 11000           11001           11010		   11011		   11100		   11101		   11110		   11111
	     3,         NOT_I2OF5_CHAR, NOT_I2OF5_CHAR, NOT_I2OF5_CHAR, NOT_I2OF5_CHAR, NOT_I2OF5_CHAR, NOT_I2OF5_CHAR, NOT_I2OF5_CHAR
};


typedef struct
{
	unsigned short	barcodeSetpointBar;
	unsigned short	barcodeSetpointSpace;
}BarcodeSetpoints_t;

static BarcodeSetpoints_t barcodeSetpoints_m;


typedef struct
{
	short	barAdjust;
	short	spaceAdjust;
}SetpointsAdjusts_t;

static const SetpointsAdjusts_t	setpointsAdjustTable[] = 
{
	{    0,   0 },
	{    2,   2 },
	{   -2,  -2 },
	{    4,   4 },
	{   -4,  -4 },
	{    8,   8 },
	{   -8,  -8 },
	{   16,  16 },
	{  -16, -16 },
	{   32,  32 },
	{  -32, -32 },
	{   64,  64 },
	{  -64, -64 },
	{  128, 128 },
	{ -128, -128 },
	{  256,  256 },
	{ -256, -256 },
};

	
// ********************************************************************
// FUNCTION:
//
//		GetBarSpaceType() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get and check transition to set type for narrow/wide bar & narrow/wide space			    
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

static void
GetBarSpaceType( const unsigned short **barcodeTransitions, BarSpaceType_t *barSpaceType )
{
	if (**barcodeTransitions & BAR_TRANSITION_MASK)
	{
		if ((**barcodeTransitions & (~BAR_TRANSITION_MASK)) < barcodeSetpoints_m.barcodeSetpointBar)
		{
			*barSpaceType = NARROW_BAR;
		}
		else
		{
			*barSpaceType = WIDE_BAR;
		}
	}
	else
	{
		if (**barcodeTransitions < barcodeSetpoints_m.barcodeSetpointSpace)
		{
			*barSpaceType = NARROW_SPACE;
		}
		else
		{
			*barSpaceType = WIDE_SPACE;
		}
	}

	++*barcodeTransitions;
}


// ********************************************************************
// FUNCTION:
//
//		GetTransitionMatchPattern() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get and set transition pattern to match for number of transitions			    
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

static void
GetTransitionMatchPattern( const unsigned short ** barcodeTransitions, unsigned numTransitions, unsigned *transitionsPattern)
{
	unsigned		i;

	BarSpaceType_t	barSpaceType;

	// reset startStopTransitions for no transition types

	*transitionsPattern = 0;

	// process 4 transitions for a start/stop char

	for (i = 1; i <= numTransitions; i++)
	{
		// get a transition

		GetBarSpaceType( barcodeTransitions, &barSpaceType);

		// create & concatonate 2 bit bar/space types for number of transitions to match

		*transitionsPattern |= barSpaceType;

		if (i < numTransitions)
		{
			*transitionsPattern <<= 2;
		}
	}
}


// ********************************************************************
// FUNCTION:
//
//		GetStartChar() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Gets a valid start character.  Advances barcodeTransitions to
//		the next character.
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

static SystemErrorNumber_t
GetStartChar( const unsigned short ** barcodeTransitions )
{
	unsigned	transitionsStart;

	// get & check to match 4 bar space transition types for start char

	GetTransitionMatchPattern( barcodeTransitions, NUM_START_STOP_TRANSITIONS, &transitionsStart ); 

	if ( transitionsStart != START_CHAR_PATTERN )
	{
		return SERR_BC_NO_START;
	}

	return SERR_NONE;	
}


// ********************************************************************
// FUNCTION:
//
//		Decode2Digits() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get 2 digits from raw barcode transitions (digit 1 bars, digit 2 spaces)			    
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

static SystemErrorNumber_t
Decode2Digits( const unsigned short **barcodeTransitions, unsigned char *digit1, unsigned char *digit2 )
{
	unsigned		i;
	unsigned		transitions2Digits;
	char			bitMask;

	// get transitions for decode of next 2 digits

	transitions2Digits = 0;

	GetTransitionMatchPattern( barcodeTransitions, NUM_2_DIGIT_TRANSITIONS, &transitions2Digits ); 

	// set up & decode transitions to set the next 2 digits

	bitMask = 0x01;

	*digit1 = 0;
	*digit2 = 0;

	for (i = 1; i <= (NUM_2_DIGIT_TRANSITIONS / 2); i++)
	{
		// check for space 

		if ((transitions2Digits & TRANSITION_MATCH_MASK) < NARROW_SPACE)
		{
			// bar so error

			return SERR_BC_BAD_DECODE;
		}
		else if ((transitions2Digits & TRANSITION_MATCH_MASK) == WIDE_SPACE)
		{
			*digit2 |= bitMask;
		}
		
		// check for bar 

		// shift for next match check 

		transitions2Digits >>= 2;

		if ((transitions2Digits & TRANSITION_MATCH_MASK) > WIDE_BAR)
		{
			// space so error

			return SERR_BC_BAD_DECODE;
		}
		else if ((transitions2Digits & TRANSITION_MATCH_MASK) == WIDE_BAR)
		{
			*digit1 |= bitMask;
		}

		// shift for next match check 

		transitions2Digits >>= 2;

		// shift for next I2of5 digit bit

		bitMask <<= 1;
	}

	// set digit1 and digit 2 by converting from I2of5 bit map to digit xref table

	*digit1 = (char) i2Of5ConvertTable_m[*digit1];

	*digit2 = (char) i2Of5ConvertTable_m[*digit2];

	// check to make sure digit 1 and digit 2 are valid I2of5 digits

	if ((*digit1 == NOT_I2OF5_CHAR) || (*digit2 == NOT_I2OF5_CHAR))
	{
		return SERR_BC_BAD_DECODE;
	}

	return SERR_NONE;
}


// ********************************************************************
// FUNCTION:
//
//		GetBarcodeDigits() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get all 110 usuable bar code digits for bar code data processing			    
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

static SystemErrorNumber_t
GetBarcodeDigits( const unsigned short **barcodeTransitions, char *barcodeDigits )
{
	unsigned			i;
	unsigned char		digit1;
	unsigned char		digit2;
	SystemErrorNumber_t	errNum = SERR_NONE;

	// get all 110 (55 pairs) usable barcode digits for bar code data processing & storage

	for (i = 1; (i <= NUM_BARCODE_DIGITS / 2) && (errNum == SERR_NONE); i++)
	{
		// get & save 2 decoded barcode digits

		errNum = Decode2Digits( barcodeTransitions, &digit1, &digit2 );

		*barcodeDigits++ = digit1;
		*barcodeDigits++ = digit2;
	}

	return errNum;
}


// ********************************************************************
// FUNCTION:
//
//		GetStopChar() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get & check for valid stop char			    
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

static SystemErrorNumber_t
GetStopChar( const unsigned short **barcodeTransitions )
{
	unsigned transitionsStop;

	// get & check to match 4 bar space transition types for stop char

	GetTransitionMatchPattern( barcodeTransitions, NUM_START_STOP_TRANSITIONS, &transitionsStop ); 

	if (transitionsStop != STOP_CHAR_PATTERN)
	{
		return SERR_BC_NO_STOP;
	}

	return SERR_NONE;	
}


// ********************************************************************
// FUNCTION:
//
//		VerifyBarcodeCheckDigit() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Verify bar code digits for bar code data processing
//		by checking 109 digits calculated I2of5 check digit versus check digit read 
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

static SystemErrorNumber_t
VerifyBarcodeCheckDigit(char *barcodeDigits )
{
	unsigned		i;
	unsigned		sum;
	char			calculatedCheckDigit;

	// initialize sum for calculating I2of5 check digit for 109 digits (index 0 to 108)

	sum = 0;

	for (i = 0; i < (NUM_BARCODE_DIGITS - 1); i++)
	{
		if (i % 2)
		{
			// odd so just sum digit
		
			sum += *barcodeDigits++;
		}
		else
		{
			// even so multiply digit by 3 and then sum
		
			sum += ((*barcodeDigits++) * 3);
		}
	}

	calculatedCheckDigit = (10 - (sum % 10)) % 10;
	
	// compare versus check digit in last digit of 110 digits
	
	if (calculatedCheckDigit != *barcodeDigits)
	{
		return SERR_BC_BAD_DECODE;
	}
	
	return SERR_NONE;
}


// ********************************************************************
// FUNCTION:
//
//		VerifyBarcodeDigitsCrc() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Verify bar code digits for bar code data processing
//		by calculating CRC16 of 104 digits used versus embedded 5 digit 
//		CRCvalue at end of barcode			    
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

static SystemErrorNumber_t
VerifyBarcodeDigitsCrc(char *barcodeDigits )
{
	unsigned		i;
	unsigned char	combinedDigitPair;
	unsigned short	calculatedBarcodeCrc;
	unsigned short	barcodeCrcDigits;

	calculatedBarcodeCrc = 0;

	// calculate CRC on barcode digit pairs

	for (i = 1; i <= (NUM_BARCODE_DIGITS_TO_CRC / 2); i++)
	{
		combinedDigitPair  = (*barcodeDigits++) << 4;
		combinedDigitPair += (*barcodeDigits++);
		CalcCrc16(combinedDigitPair,&calculatedBarcodeCrc);
	}

	// compute read barcode digits CRC, and compare to calculated CRC value

	barcodeCrcDigits =  (*barcodeDigits++) * 10000;
	barcodeCrcDigits += (*barcodeDigits++) * 1000;
	barcodeCrcDigits += (*barcodeDigits++) * 100;
	barcodeCrcDigits += (*barcodeDigits++) * 10;
	barcodeCrcDigits += (*barcodeDigits++);

	if (barcodeCrcDigits != calculatedBarcodeCrc)
	{
		return SERR_BC_BAD_DECODE;
	}

	return SERR_NONE;
}


// ********************************************************************
// FUNCTION:
//
//		VerifyBarcodeDigits() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Verify 110 bar code digits for bar code data processing
//		by checking check digit for all digits read and
//		by checking CRC of digits used			    
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

static SystemErrorNumber_t
VerifyBarcodeDigits(char *barcodeDigits )
{
	SystemErrorNumber_t errNum;
	
	errNum = VerifyBarcodeCheckDigit(barcodeDigits );

	if ( errNum == SERR_NONE )
	{
		errNum = VerifyBarcodeDigitsCrc( barcodeDigits );
	}

	return errNum;
}


// ********************************************************************
// FUNCTION:
//
//		Compare() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Compares the values of two shorts.  Used for qsort().
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

static int
Compare( const void * a, const void * b )
{
	unsigned short as = *(unsigned short *)a;
	unsigned short bs = *(unsigned short *)b;
	
	if ( as == bs )
	{
		return 0;
	}
	else if ( as > bs )
	{
		return 1;
	}

	return -1;
}


// ********************************************************************
// FUNCTION:
//
//		InitBarcodeSetpoints() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Determine setpoints based on start char bars and spaces
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

static SystemErrorNumber_t
InitBarcodeSetpoints( const unsigned short *barcodeTransitions )
{
	int				i;
	unsigned short	sortBuff[ 557 ];
	
	// Make sure transition 558 is at least 3000.
	if ( (barcodeTransitions[557] & ~BAR_TRANSITION_MASK) < 3000 )
	{
		return SERR_BC_END_TRANSITION;
	}

	// Make sure all the other transitions are less than 3000.
	for ( i = 0; i < 557; i++ )
	{
		if ( (barcodeTransitions[i] & ~BAR_TRANSITION_MASK) > 3000 )
		{
			return SERR_BC_TRANSITIONS;
		}
	}
	
	/*
	 *  Copy the transitions to our local storage and sort them.  This will
	 * 	place the spaces at the beginning and the bars at the end because the
	 * 	bar values all have the high bit set as a flag to indicate a bar.
	 */
	memcpy( sortBuff, barcodeTransitions, 557 * sizeof( short ) );
	qsort( sortBuff, 557, sizeof( short ), Compare );

	barcodeSetpoints_m.barcodeSetpointSpace = sortBuff[167] + 1;
	barcodeSetpoints_m.barcodeSetpointBar = sortBuff[279 + 167] & ~BAR_TRANSITION_MASK;
	
	return SERR_NONE;
}


// ********************************************************************
// FUNCTION:
//
//		DecodeBarcode() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Decode bar code start char, usable digits & stop char from raw bar code transition period data
//		from Engine message 'B' and verify data CRC and check digit
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

static SystemErrorNumber_t
DecodeBarcode( const unsigned short *barcodeTransitions, char *barcodeDigits )
{
	SystemErrorNumber_t	errNum;
	
	// make sure valid start char is first char 

	errNum = GetStartChar( &barcodeTransitions );

	if ( errNum == SERR_NONE )
	{
		// get all barcode digits including header, data, and trailer CRC/checkdigit
	
		errNum = GetBarcodeDigits( &barcodeTransitions, barcodeDigits );
	
		if ( errNum == SERR_NONE )
		{
			// make sure valid stop char is last char
	
			errNum = GetStopChar( &barcodeTransitions );
			if ( errNum == SERR_NONE )
			{
				// verify barcode digits for correct CRC and check digit 
		
				errNum = VerifyBarcodeDigits( barcodeDigits );
			}
		}
	}

	return errNum;
}


// ********************************************************************
// FUNCTION:
//
//		Combine2BarcodeDigits() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Convert 2 bar code digits to unsigned char value 
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

static char *
Combine2BarcodeDigits(char *barcodeDigits, unsigned char *combinedChar)
{
	*combinedChar =  *barcodeDigits++ * 10;
	*combinedChar += *barcodeDigits++;

	return barcodeDigits;
}


// ********************************************************************
// FUNCTION:
//
//		ConvertLotCodeDigit() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Convert manufacturing lot code digit to ASCII value
//		- 'A' convert code passed converts digit from 0 to 8 to 'A' to 'I'
//		- '0' convert code passed converts digit from 0 to 8 to '0' to '8'
//		- if digit passed is 9, converts to ASCII space ' ' 
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

static char *
ConvertLotCodeDigit(char *barcodeDigits, char asciiConvertCode, char *convertedCode)
{
	if (*barcodeDigits < 9)
	{
		*convertedCode = *barcodeDigits + asciiConvertCode;		// converted to ASCII  
	}
	else
	{
		*convertedCode = ' ';		// set to space char if bar code digit is 9
	}

	barcodeDigits++;

	return barcodeDigits;
}


// ********************************************************************
// FUNCTION:
//
//		StoreBarcodeData() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process decoded bar code digits to store in barcodeData shared memory 
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

static void
StoreBarcodeData(char *barcodeDigits)
{
	unsigned		i;
	unsigned char	rotorAnalyteFactor;	

	// store rotor ID & format 

	barcodeDigits = Combine2BarcodeDigits(barcodeDigits, &barcodeData_m->rotorId);
	barcodeData_m->rotorFormat = *barcodeDigits++;

	// store rotor expiry date/time
	 
	barcodeDigits = Combine2BarcodeDigits(barcodeDigits, &barcodeData_m->rotorExpiryMonth);
	barcodeDigits = Combine2BarcodeDigits(barcodeDigits, &barcodeData_m->rotorExpiryDayOfMonth);
	barcodeDigits = Combine2BarcodeDigits(barcodeDigits, &barcodeData_m->rotorExpiryYear);

	// store manufacturing lot number information

	barcodeData_m->rotorMfgLot.lsdOfYear  = *barcodeDigits++; 
	barcodeDigits = Combine2BarcodeDigits(barcodeDigits, &barcodeData_m->rotorMfgLot.weekOfYear);
	barcodeData_m->rotorMfgLot.dayOfWeek  = *barcodeDigits++; 

	barcodeDigits = ConvertLotCodeDigit(barcodeDigits, 'A', &barcodeData_m->rotorMfgLot.code[0]);
	barcodeDigits = ConvertLotCodeDigit(barcodeDigits, 'A', &barcodeData_m->rotorMfgLot.code[1]);
	barcodeDigits = ConvertLotCodeDigit(barcodeDigits, '0', &barcodeData_m->rotorMfgLot.code[2]);

	barcodeData_m->rotorMfgLot.code[3] = '\0';
	
	// store the analyte calibration factors converted from 2 digits to floating point factors

	for (i = 0; i < MAX_BC_FACTORS; i++)
	{
		barcodeDigits = Combine2BarcodeDigits(barcodeDigits, &rotorAnalyteFactor);
		 
		barcodeData_m->rotorAnalyteFactor[i] = (float) rotorAnalyteFactor; 
	}
}

// ********************************************************************
// FUNCTION:
//
//		IsValidDate 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Determines whether the specified date is a valid one. 
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

static bool
IsValidDate( char month, char day, char year )
{
	const int validDays[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
// GLD
// Should we up the 20?
	// Valid year.
	if ( year >= 0 && year <= 20 )
	{
		// Valid month.
		if ( month > 0 && month <= 12 )
		{
			// Valid day.
			if ( day > 0 && day <= validDays[ month - 1 ] )
				return true;
		}
	}

	return false;
}

// ********************************************************************
// FUNCTION:
//
//		RotorExpired 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Determines whether the rotor has expired. 
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

static bool
RotorExpired( char month, char day, char year )
{
	// Get the current date.
	time_t		t = time( 0 );
	struct tm *	currentDate = gmtime( &t );

	// Check if current year greater than rotor expiry year.  currentDate
	// has the number of years since 1900, so we subtract 100 to get the number
	// of years since 2000.	
	if ( currentDate->tm_year - 100 > year )
	{
		return true;
	}
	
	// If the expiration year is more than 3 years greater than the current year,
	// we consider the rotor expired. (Legacy logic.)
	if ( currentDate->tm_year - 100 < year - 3 )
	{
		return true;
	}
	
	// If the current year is the same as the expiry year and the current month
	// is greater than expiry month, rotor is expired.
	// currentDate has the month in the range 0-11, vs. 1-12 in the argument.
	if ( (currentDate->tm_year - 100 == year) && (currentDate->tm_mon + 1 > month) )
	{
		return true;
	}
	
	// If the current month is the same as the expiry month and the current day
	// is greater than the expiry day, the rotor is expired.
	if ( (currentDate->tm_year - 100 == year) &&
		 (currentDate->tm_mon + 1 == month) && 
		 (currentDate->tm_mday > day) )
	{
		return true;
	}
	
	return false;
}

// ********************************************************************
// FUNCTION:
//
//		JulianDate 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Determines the Julian date for a given date. 
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

unsigned
JulianDate( char month, char day, unsigned short year )
{
	unsigned jDate;
	unsigned temp;
	
	temp = 12 * ( year + 4800 ) + month - 3;
	jDate = ( 2 * ( temp - ( temp / 12 ) * 12 ) + 7 + 365 * temp ) / 12;
	jDate += day + temp / 48 - 32045 + temp / 4800 - temp / 1200;
	
	return jDate;
}
	

// ********************************************************************
// FUNCTION:
//
//		CalcRotorAgeInDays 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Determines the rotor age in days. 
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

float
CalcRotorAgeInDays( MfgLot_t lotInfo )
{
	time_t			t;
	struct tm *		currentDate;
	unsigned short	currentYear;
	int				lsdOfCurrentYear;
	unsigned short	mfgYear;
	unsigned		currentJulian;
	unsigned		mfgJulian;
	float			rotorAgeInDays;
	
	// Get the current date, and extract the current year.
	t = time( 0 );
	currentDate = gmtime( &t );
	currentYear = currentDate->tm_year + 1900;
	
	// Determine the manufacturing year.
	lsdOfCurrentYear = currentYear % 10;
	mfgYear = currentYear - ( lsdOfCurrentYear - lotInfo.lsdOfYear );
	if ( lsdOfCurrentYear < lotInfo.lsdOfYear )
	{
		mfgYear -= 10;
	}

	// Get the Julian date for the current date.
	currentJulian = JulianDate( currentDate->tm_mon+1, currentDate->tm_mday, currentYear );
	
	// Get the Julian date for January 1 of the manufacturing year.
	mfgJulian = JulianDate( 1, 1, mfgYear );
	
	// Add the manufacturing week and day-of-week, and subtract the day of week
	// that the manufacturing year started on.
	// Subtract 1 from weekOfYear so we get 0-based value.
	mfgJulian += (unsigned)(lotInfo.weekOfYear - 1) * 7 + (unsigned)lotInfo.dayOfWeek - ( ( mfgJulian % 7 ) + 1 );
	
	// Get the rotor age in days.
	rotorAgeInDays = (long)(currentJulian - mfgJulian);
	
	return rotorAgeInDays;
}

// ********************************************************************
// FUNCTION:
//
//		ValidateBarcodeData() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Validate expiry date & manufacturing dates and
//		compute rotor age in days for rotor results record 
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

static SystemErrorNumber_t
ValidateBarcodeData( BarcodeData_t * barcodeData )
{
	
	// Check for a valid expiration date.
	if ( !IsValidDate( barcodeData->rotorExpiryMonth,
					   barcodeData->rotorExpiryDayOfMonth,
					   barcodeData->rotorExpiryYear ) )
	{
		return SERR_BC_ILLEGAL_DATE;
	}
	
	// Calculate the rotor age before we might return an expired rotor error, because
	// in Research mode we continue even if the rotor is expired.
	barcodeData->rotorAgeInDays = CalcRotorAgeInDays( barcodeData->rotorMfgLot );
	
	// Check for an expired rotor.
	if ( RotorExpired(  barcodeData->rotorExpiryMonth,
						barcodeData->rotorExpiryDayOfMonth,
						barcodeData->rotorExpiryYear ) )
	{
		return SERR_EXPIRED_ROTOR;
	}
	
	if ( barcodeData->rotorAgeInDays < MIN_ROTOR_AGE || barcodeData->rotorAgeInDays > MAX_ROTOR_AGE )
	{
		barcodeData->rotorAgeInDays = BAD_ROTOR_AGE;
		return SERR_IMPROPER_DATE;
	}

	return SERR_NONE;
}


// ********************************************************************
// FUNCTION:
//
//		ProcessRotorBarcode() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process Bar Code raw transition periods to create rotor barcodeData 
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

SystemErrorNumber_t
ProcessRotorBarcode( const unsigned short * barcodeTransitions, BarcodeData_t * barcodeData )
{
	char				barcodeDigits[NUM_BARCODE_DIGITS];
	SystemErrorNumber_t	errNum = SERR_NONE;
	FILE *				fp;
	int					i;
	
	memset( barcodeData, 0, sizeof( BarcodeData_t ) );
	barcodeData_m = barcodeData;
	
	barcodeData_m->rotorId = UNKNOWN_ROTOR_ID;

	{
		fp = fopen( BC_TRANSITIONS_FILE, "w+" );
		if ( fp == 0 )
		{
			LogDebug( "unable to open barcode transitions file" );
		}
		else
		{
			for ( i = 0; i < 4; i++ )
			{
				fprintf( fp, "%04x ", barcodeTransitions[i] );
			}
			fprintf( fp, "\n" );
			for ( i = 4; i < NUM_BARCODE_TRANSITIONS; i++ )
			{
				fprintf( fp, "%04x ", barcodeTransitions[i] );
				if ( ( i % 10 ) == 9 )
				{
					fprintf( fp, "\n" );
				}
			}
			fclose( fp );
		}
		
	}

	// Initialize bar and space setpoints

	errNum = InitBarcodeSetpoints( barcodeTransitions );
	if ( errNum != SERR_NONE )
	{
		LogError( "InitBarcodeSetpoints failed" );
		return errNum;
	}
	
	errNum = DecodeBarcode( barcodeTransitions, barcodeDigits );
	if ( errNum != SERR_NONE )
	{
		return errNum;
	}
	
	fp = fopen( BC_DIGITS_FILE, "w+" );
	if ( fp == 0 )
	{
		LogDebug( "unable to open barcode digits file" );
	}
	else
	{
		for ( i = 0; i < NUM_BARCODE_DIGITS; i++ )
		{
			fprintf( fp, "%d", barcodeDigits[i] );
		}
		fclose( fp );
	}

	StoreBarcodeData( barcodeDigits );

	errNum = ValidateBarcodeData( barcodeData );

	LogDebug( "ID: %u, format: %u", barcodeData->rotorId, barcodeData->rotorFormat );
	LogDebug( "Expiry: %u/%u/%u", barcodeData->rotorExpiryMonth, barcodeData->rotorExpiryDayOfMonth, barcodeData->rotorExpiryYear );
	LogDebug( "Rotor age: %f", barcodeData->rotorAgeInDays );

	return errNum;
}
