#ifndef __ROTORBARCODE_H_INCLUDED
#define __ROTORBARCODE_H_INCLUDED

#include "BarcodeData.h"

#define	MAX_BARCODE_TRANSITIONS		600

#define DAYS_PER_MONTH				30.6
#define MIN_ROTOR_AGE				-1.0
#define MAX_ROTOR_AGE				( 30 * DAYS_PER_MONTH )
#define BAD_ROTOR_AGE				-1.1;

// Error return values

#define ERR_EXPIRED_ROTOR			0x4054
#define ERR_BC_ILLEGAL_DATE			0x4056
#define ERR_IMPROPER_DATE			0x406C
#define	ERR_BC_NO_START				0x410f
#define	ERR_BC_NO_STOP				0x4110
#define	ERR_BC_BAD_DECODE			0x4111
#define	ERR_BC_BAD_CHECK			0x4112
#define	ERR_BC_BAD_CRC				0x4113
#define	ERR_BC_NONE					0

#define NUM_BARCODE_DIGITS          110
#endif	// __ROTORBARCODE_H_INCLUDED
