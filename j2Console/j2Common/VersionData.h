#ifndef __VERSIONDATA_H_INCLUDED
#define __VERSIONDATA_H_INCLUDED

#include "CommonDefines.h"

typedef struct
{
	char	version[ MAX_VERSION_LEN + 1 ];
    char    engSoftwareVersion [MAX_ENG_SOFTWARE_VERSION_LEN + 1];
    char    engFpgaVersion [MAX_ENG_SOFTWARE_VERSION_LEN + 1];
    char    bspVersion [MAX_VERSION_LEN + 1];
    char    apsVersion [MAX_VERSION_LEN + 1];
	char	releaseType;
} VersionData_t;

#endif	// __VERSIONDATA_H_INCLUDED
