#ifndef __ROTORBARCODE_H_INCLUDED
#define __ROTORBARCODE_H_INCLUDED

#include "BarcodeData.h"
#include "SystemError.h"

#define	MAX_BARCODE_TRANSITIONS		600

#define DAYS_PER_MONTH				30.6
#define MIN_ROTOR_AGE				-1.0
#define MAX_ROTOR_AGE				( 30 * DAYS_PER_MONTH )
#define BAD_ROTOR_AGE				-1.1;

typedef struct 
{
    unsigned short              maxBc;
    unsigned short              minBc;
    unsigned short              peak;
    unsigned short              dac;
    unsigned short              dioadDrop;
    unsigned short              loop;
    unsigned short              diodeFactor;
    unsigned short              errorCode;
} barcodeParameters_t;

SystemErrorNumber_t ProcessRotorBarcode (const unsigned short * barcodeTransitions, unsigned short *barcodeParameters, BarcodeData_t * barcodeData, char *digitalFileName);

#endif	// __ROTORBARCODE_H_INCLUDED
