#ifndef __PARSEREFRANGES_H
#define __PARSEREFRANGES_H

#include "RefRanges.h"
#include "CfgErr.h"

CfgError_t	ParseRefRanges( const char * fileName, RefRangeData_t * refRanges );
CfgError_t	WriteRefRanges( const char * fileName, RefRangeData_t * refRanges );

#endif	// __PARSEREFRANGES_H
