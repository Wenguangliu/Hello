#ifndef __DEMO_H_INCLUDED
#define __DEMO_H_INCLUDED

#include "ResultsData.h"
#include "BarcodeData.h"

#define DEMO_MAX_RESULTS	50
#define DEMO_MAX_ANALYTES	20

#define PICCOLO_DEMO_FILE_NAME	"/Hard Disk/Abaxis/cfg/piccoloDemo.xml"
#define VETSCAN_DEMO_FILE_NAME	"/Hard Disk/Abaxis/cfg/vetscanDemo.xml"
#define PICCOLO_DEMO_FILE_NAME_FROM_USB "/Hard Disk2/piccoloDemo.xml"
#define VETSCAN_DEMO_FILE_NAME_FROM_USB "/Hard Disk2/vetscanDemo.xml"

typedef struct
{
	int					analyteIndex;
	float				result;
	ResultPrintFlags_t	resultPrintFlags;
	bool				posNegValue;
	float				lowDynamicRangeLimit;
	float				highDynamicRangeLimit;
	float				lowSystemRangeLimit;
	float				highSystemRangeLimit;
} DemoAnalyteInfo_t;

typedef struct
{
	char				rotorName[31];
	int					rotorId;
	int					rotorFormat;
	MfgLot_t			lotNum;
	float				hemValue;
	float				lipValue;
	float				ictValue;
	float				rqcValue;
	float				acceptableMinimum;
	int					numAnalytes;
	DemoAnalyteInfo_t	analytes[ DEMO_MAX_ANALYTES ];
} DemoResult_t;

typedef struct
{
	int					numResults;
	DemoResult_t		results[ DEMO_MAX_RESULTS ];
} DemoData_t;

DemoResult_t *	GetFirstDemoResult( DemoResult_t * exclude );
DemoResult_t *	GetNextDemoResult( DemoResult_t * exclude );
DemoResult_t *	GetRandomDemoResult();
DemoResult_t *  GetDemoResultById( int RotorId );
void			PopulateDemoResults( DemoResult_t * demoResult, RotorResults_t * results );

#endif	// __DEMO_H_INCLUDED
