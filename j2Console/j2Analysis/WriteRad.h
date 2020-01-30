/*
 * FILENAME
 * 	WriteRad.h
 * 
 * MODULE DESCRIPTION
 * 	Header for WriteRad.cc, which takes care of writing the RAD file.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __WRITERAD_H_INCLUDED
#define __WRITERAD_H_INCLUDED

#include "CommonDefines.h"
#include "BarcodeData.h"		// BarcodeData_t definition.
#include "FactoryData.h"		// FactoryData_t definition.
#include "CalData.h"			// CalibrationData_t definition.
#include "RunInfo.h"			// Runinfo_t definition.
#include "ResultsData.h"		// RotorResults_t definition.
#include "RawData.h"			// RawData_t definition.
#include "SystemData.h"			// SystemData_t definition.
#include "VersionData.h"		// VersionData_t definition.

//#define RAD_FILE_VERSION_NUMBER	7	// Version right before addition of X1
//#define RAD_FILE_VERSION_NUMBER	8
#define RAD_FILE_VERSION_NUMBER     9	

const char * WriteRadFile( RunInfo_t *			runInfo,
						   FactoryData_t *		factoryData,
						   SystemData_t *		systemData,
						   VersionData_t *		versionData,
						   CalibrationData_t *	calibrationData,
						   BarcodeData_t *		barcodeData,
						   RotorResults_t *		rotorResults,
						   RawData_t *			rawData );

size_t	CrcWrite( const void * ptr, size_t size, size_t nobj, FILE * stream );
void	WriteString( const char * string, int width, FILE * fp );

extern unsigned long	crc;

#endif	// __WRITERAD_H_INCLUDED
