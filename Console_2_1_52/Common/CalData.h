#ifndef __CALDATA_H_INCLUDED
#define __CALDATA_H_INCLUDED

#include "CommonDefines.h"

typedef struct
{
	unsigned char	barcodeDacs[ NUM_BARCODE_DACS ];
	unsigned char	cuvetteMarkDacs[ NUM_CUVETTE_MARK_DACS ];
	unsigned char	wavelengthDacTrims[ NUM_WAVELENGTHS ];
} CalibrationData_t;

#endif	// __CALDATA_H_INCLUDED
