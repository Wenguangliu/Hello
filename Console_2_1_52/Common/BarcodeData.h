#ifndef __BARCODEDATA_H_INCLUDED
#define __BARCODEDATA_H_INCLUDED

#include "CommonDefines.h"

#pragma pack(1)	// Pack the structure tightly to facilitate upload transfers from a PC in Research mode.
typedef struct
{
	unsigned char	lsdOfYear;		// 0 to 9  (convert to ASCII as part of rotor lot code string)
	unsigned char	weekOfYear;		// 0 to 51 (convert to ASCII as part of rotor lot code string)
	unsigned char	dayOfWeek;		// 0 to 6  (convert to ASCII as part of rotor lot code string)
	char			code[4];		// 3 char ASCII code string: 1st 2 chars converted '0'-'9' to 'A'-'J'; 3rd char '0'-'9'; 4th char NULL 
} MfgLot_t;


typedef struct
{
	unsigned char	rotorId;							// 1 to 99
	unsigned char	rotorFormat;						// 0 to 9
	unsigned char	rotorExpiryMonth;					// 1 to 12		
	unsigned char	rotorExpiryDayOfMonth;				// 1 to 31
	unsigned char	rotorExpiryYear;					// 00 to 99 (hundreds/tens places of year only)
	float			rotorAgeInDays;
	MfgLot_t		rotorMfgLot;						// rotor manufacturing lot code ID data
	float			rotorAnalyteFactor[MAX_BC_FACTORS];	// 0.0 to 99.0
} BarcodeData_t;
#pragma pack()

#endif	// __BARCODEDATA_H_INCLUDED
