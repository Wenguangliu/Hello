/*
 * FILENAME
 * 	WriteRadX1.h
 * 
 * MODULE DESCRIPTION
 * 	Header for WriteRadX1.cc, which takes care of writing the X1 section of the RAD file.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __WRITERADX1_H_INCLUDED
#define __WRITERADX1_H_INCLUDED


void	WriteRadX1( FILE * fp,
					RunInfo_t *			runInfo,
		// 			 FactoryData_t *		factoryData,
					SystemData_t *		systemData,
					VersionData_t *		versionData,
		//			CalibrationData_t *	calibrationData,
					BarcodeData_t *		barcodeData,
					RotorResults_t *		rotorResults,
		  			RawData_t *			rawData  );

#endif	// __WRITERADX1_H_INCLUDED
