#ifndef __ASTMORDERS_H_INCLUDED
#define __ASTMORDERS_H_INCLUDED


#include "CommonDefines.h"
#include "OrdersData.h"


// data structure for an ASTM record to be used to transmit analyzer pending order list to LIS
// (ASTM 1394-97 paragraph reference in comments below, as applicable, as related to basis of M n PO field)

typedef struct
{
	// 'M'- PO pending order type record specific fields
	char  mnOrderDateTime[ ID_LEN ];						// 9.4.7    'O' record: Requested/Ordered Date and Time 
	char  mnOrderPatientControlId[ ID_LEN ];				// 8.1.3    'P' record: Practice Assigned Patient ID
	char  mnOrderRotorNumber[ 3 ];							// 9.4.5.4  'O' record: Universal Test ID - Manufacturer's or Local Code (rotor number)
	char  mnOrderLaboratoryField1[ 18 ];					// 9.4.21   'O' record: Laboratory Field No. 1 (order label)

} AstmMfgPendingOrder_t;


typedef struct
{
	// 'M' - PO record
	AstmMfgPendingOrder_t   mPendingOrders[ MAX_QUEUED_ORDERS + 1 ];	// up to 100 stored in pending order queue
																		//    mnOrderDateTime is set to NULL '\0' char for end of pending orders list (no more pending orders)    
} AstmOrders_t;

#endif	// __ASTMORDERS_H_INCLUDED


