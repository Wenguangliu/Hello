/*
 * FILENAME
 * 	OrdersData.cc
 * 
 * MODULE DESCRIPTION
 * 	This file contains code to read and write the ASTM orders file.
 * 
 */

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include "LogErr.h"
#include "OrdersData.h"

#define ORDERS_FILE			"/Hard Disk/Abaxis/cfg/astmOrders.dat"
#define	ORDERS_DB_VERSION	1

// Orders database index-file structure.
typedef struct
{
	int				dbVersion;
	OrdersList_t	ordersList;
	unsigned 		signature;
} OrdersDb_t;

unsigned 	CalculateSignature( unsigned char * p, unsigned len );

/*
 * FUNCTION
 * 	DeleteOrdersFile
 * 
 * DESCRIPTION
 * 	Deletes the orders file.
 */

void
DeleteOrdersFile()
{
	int	    status;
    DWORD   errNum;

	status = DeleteFile (TEXT(ORDERS_FILE));
	if (status == 0) {
        errNum = GetLastError();
        // skip errorn num 0x02 for file does not exist
        if (errNum != 0x02) {
	        LogError( "error deleting file %s: %d", ORDERS_FILE, errNum);
        }
	}
}

/*
 * FUNCTION
 * 	ReadOrdersFile
 * 
 * DESCRIPTION
 * 	Reads the file, verifies its integrity, and checks for the correct version
 *	number. If there is a problem, the file is deleted.
 * 
 * PARAMETERS
 * 	ordersList - Pointer to a data structure to receive the orders data.
 */

void
ReadOrdersFile( OrdersList_t * ordersList )
{
	FILE *			fp;
	OrdersDb_t		ordersDb;
	unsigned		calcSignature;
	
	// Open the file. If it doesn't exist, just return. If there's an error
	// opening it, log the error and return.
	fp = fopen (ORDERS_FILE, "rb");
	if (fp == 0)
	{
		//LogError( "error reading %s", ORDERS_FILE);
		LogError( "%s file does not exist", ORDERS_FILE);
		return;
	}

	// Read the file.
	fread( &ordersDb, sizeof( OrdersDb_t ), 1, fp );
	fclose( fp );

	// If the version of the file is different from the current version being used,
	// delete both files and return.
	if ( ordersDb.dbVersion != ORDERS_DB_VERSION )
	{
		LogError( "orders db files are wrong version" );
		return;
	}

	// Make sure the value for numOrders is within range. If not, delete the file and return.
	if ( (ordersDb.ordersList.numOrders < 0) || (ordersDb.ordersList.numOrders > MAX_QUEUED_ORDERS) )
	{
		LogError( "%s has bad numOrders value: %d", ORDERS_FILE, ordersDb.ordersList.numOrders );
		DeleteOrdersFile();
		return;
	}	

	// Verify the signature of the data. If it doesn't match the signature in the
	// file, delete the file and return.
	calcSignature = CalculateSignature( (unsigned char *)&ordersDb.ordersList, sizeof( OrdersList_t ) );
	if ( calcSignature != ordersDb.signature )
	{
		LogError( "orders file CRC mismatch" );
		DeleteOrdersFile();
		return;
	}

	// Copy the data into the orders list.
	memcpy( ordersList, &ordersDb.ordersList, sizeof( OrdersList_t ) );
}

/*
 * FUNCTION
 * 	WriteOrdersFile
 * 
 * DESCRIPTION
 * 	Writes the index and data files. Calculates the signature for each, and includes
 * 	that in the files.
 * 
 * PARAMETERS
 * 	ordersList - Data structure containing both the index and the data.
 */

void
WriteOrdersFile( OrdersList_t * ordersList )
{
	FILE *		fp;
	OrdersDb_t	ordersDb;
	
	// Calculate the signature and put everything in the index file data struct.
	ordersDb.signature = CalculateSignature( (unsigned char *)ordersList, sizeof( OrdersList_t ) );
	ordersDb.dbVersion = ORDERS_DB_VERSION;
	memcpy( &ordersDb.ordersList, ordersList, sizeof( OrdersList_t ) );
	
	// Open the file. If there's an error, log the error and return.
	fp = fopen( ORDERS_FILE, "wb" );
	if ( fp == 0 )
	{
		LogError( "error opening %s", ORDERS_FILE );
		return;
	}

	// Write the  file.
	fwrite( &ordersDb, sizeof( OrdersDb_t ), 1, fp );
	fclose( fp );
}

/*
 * FUNCTION
 * 	CalculateSignature
 * 
 * DESCRIPTION
 * 	Calculates the signature (byte-wise checksum) for the given data.
 * 
 * PARAMETERS
 * 	p - pointer to the data
 *  len - length of the data in bytes
 */

unsigned
CalculateSignature( unsigned char * p, unsigned len )
{
	unsigned	checksum = 0;
	unsigned	i;
	
	for ( i = 0; i < len; i++ )
	{
		checksum += p[ i ];
	}
	
	return checksum;
}
