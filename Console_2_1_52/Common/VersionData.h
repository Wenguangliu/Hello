#ifndef __VERSIONDATA_H_INCLUDED
#define __VERSIONDATA_H_INCLUDED

#include "CommonDefines.h"

typedef struct
{
	char	version[ MAX_VERSION_LEN + 1 ];
	char	releaseType;
} VersionData_t;

#endif	// __VERSIONDATA_H_INCLUDED
