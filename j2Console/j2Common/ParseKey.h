#ifndef __PARSEKEY_H_INCLUDED
#define __PARSEKEY_H_INCLUDED

#include "CommonDefines.h"
#include "CfgErr.h"

typedef enum
{
	KEY_MODE_NONE,
	KEY_MODE_DEMO,
	KEY_MODE_RESEARCH,
	KEY_MODE_MANUFACTURING,
	KEY_MODE_SERVICE,
	KEY_MODE_ADMINISTRATOR,
	KEY_MODE_LOCKOUT,
    KEY_MODE_R_AND_D,
} KeyMode_t;

typedef struct
{
	char		serialNumber[256];
	KeyMode_t	mode;
    bool        demoFromKey;
    bool        useFastDemo;
	bool		enableFtpTelnet;
} KeyData_t;

CfgError_t ParseKey( const char * path, KeyData_t * keyData );
CfgError_t ParseKey( const char * path, KeyData_t * keyData, unsigned * version );


#endif	// __PARSEKEY_H_INCLUDED
