/*
 * FILENAME
 * 	Crc.c
 * 
 * MODULE DESCRIPTION
 * 	Set of functions to calculate a 32-bit CRC value for a sequence of byte
 * 	values.  To use this module, CrcInit should be called first, and the CRC
 * 	value initialized to the return value from this funtion.  CrcUpdate may then
 * 	be called one or more times to add byte sequences to the data included in
 * 	the CRC.  Finally, CrcFinalize must be called to get the final CRC value.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdlib.h>
#include <stdio.h>

#include "Crc32.h"

// This is the official polynomial used in PKZip, WinZip and Ethernet. 
const unsigned long CRC_POLYNOMIAL = 0x04C11DB7;

static unsigned long	crcTable[256];
static int				tableInitialized = 0;

static void				Crc32GenerateTable( void );
static unsigned long	Crc32Reflect( unsigned long ref, char ch );

/*
 * FUNCTION
 * 	CrcInit
 * 
 * DESCRIPTION
 * 	Initializes CRC calculation.
 * 
 * RETURNS
 * 	The initialization value for the CRC.
 */

unsigned long Crc32Init( void )
{
	if ( !tableInitialized )
	{
		Crc32GenerateTable();
	}
	
	return 0xFFFFFFFF;
}

/*
 * FUNCTION
 * 	CrcUpdate
 * 
 * DESCRIPTION
 * 	Updates the CRC value by including the referenced data.
 * 
 * PARAMETERS
 * 	crc - Previous value of the CRC.
 * 	buffer - Additional data to add to the CRC.
 * 	size - Size of the data.
 * 
 * RETURNS
 * 	Updated CRC value.
 */

unsigned long Crc32Update( unsigned long crc, const unsigned char * buffer, size_t size )
{
	while ( size-- )
	{
		crc = (crc >> 8) ^ crcTable[(crc & 0xFF) ^ *buffer++];
	}
	
	return crc;
}

/*
 * FUNCTION
 * 	CrcFinalize
 * 
 * DESCRIPTION
 * 	Finalizes the CRC calculation.
 * 
 * PARAMETERS
 * 	crc - Previous value of the CRC.
 * 
 * RETURNS
 * 	Finalized CRC value.
 */
 
unsigned long Crc32Finalize( unsigned long crc )
{
	return crc ^ 0xFFFFFFFF;
}

/*
 * FUNCTION
 * 	CrcGenerateTable
 * 
 * DESCRIPTION
 * 	Generates the table used for CRC calculation.
 */
 

static void Crc32GenerateTable( void )
{
	int i,j;

	// 256 values representing ASCII character codes.
	for ( i = 0; i <= 256; i++ )
	{
		crcTable[i] = Crc32Reflect( i, 8 ) << 24;
		for ( j = 0; j < 8; j++ )
		{
			crcTable[i] = (crcTable[i] << 1) ^ (crcTable[i] & (1 << 31) ? CRC_POLYNOMIAL : 0);
		}
		crcTable[i] = Crc32Reflect( crcTable[i], 32 );
	}
	
	tableInitialized = 1;
}

/*
 * FUNCTION
 * 	Crc32Reflect
 * 
 * DESCRIPTION
 * 	Performs the reflection calculation used in generating the CRC calculation
 * 	table.
 * 
 * PARAMETERS
 * 	ref - Value to reflect.
 * 	ch - Length of the reflection value.
 * 
 * RETURNS
 * 	Reflection value.
 */

static unsigned long Crc32Reflect( unsigned long ref, char ch )
{
	unsigned long value = 0;
	int i;

	for ( i = 1; i < (ch + 1); i++)
	{
		if ( (ref & 1) != 0 )
		{
			value |= 1 << (ch - i);
		}
		ref >>= 1;
	}

	return value;
}

