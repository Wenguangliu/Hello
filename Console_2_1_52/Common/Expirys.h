#ifndef __EXPIRYS_H_INCLUDED
#define __EXPIRYS_H_INCLUDED

#include <time.h>

#include "CommonDefines.h"
#include "CfgErr.h"

#define EXPIRYS_FILE_VERSION	1
#define MAX_ADMINS				100
#define MAX_OPS					100

typedef struct
{
	char	id[ ID_LEN ];
	char	password[ ID_LEN ];
	time_t	expiration;
} Expiry_t;

typedef struct
{
	time_t		controlExpiry;
	int			numAdmins;
	int			numOps;
	Expiry_t	admins[ MAX_ADMINS ];
	Expiry_t	ops[ MAX_OPS ];
} ExpiryData_t;

class Expirys
{
public:
						Expirys();
	Expiry_t *			GetAdmin( char * id );
	Expiry_t *			GetOp( char * id );
	int					NumAdmins() { return expiryData->numAdmins; };
	int					NumOps() { return expiryData->numOps; };
	CfgError_t			AddAdmin( Expiry_t * expiry );
	CfgError_t			AddOp( Expiry_t ** expiry );
	CfgError_t			DeleteAdmin( char * id );
	CfgError_t			DeleteOp( char * id );
	Expiry_t *			GetAdminsByDate();
	Expiry_t *			GetAdminsById();
	Expiry_t *			GetOpsByDate();
	Expiry_t *			GetOpsById();
	CfgError_t			WriteExpirysFile();
	CfgError_t			SetControlExpiration( time_t expiration );
	time_t				GetControlExpiration() { return expiryData->controlExpiry; };
	void				SetToDefaults();

private:

	ExpiryData_t *	expiryData;
};

#endif	// __EXPIRYS_H_INCLUDED
