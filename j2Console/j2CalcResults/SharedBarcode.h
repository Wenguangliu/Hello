#ifndef __SHAREDBARCODE_H
#define __SHAREDBARCODE_H

#if defined(__cplusplus)
extern "C" {
#endif


#include "BarcodeData.h"


extern	BarcodeData_t *	barcodeData_g;


void LoadTestBarcode(void);


#if defined(__cplusplus)
}
#endif

#endif	// __SHAREDBARCODE_H
