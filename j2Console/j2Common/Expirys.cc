#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Expirys.h"
#include "Shmem.h"
#include "LogErr.h"
#include "CfgErr.h"
#include "ParseExpirys.h"

int	ExpiryDateCompare( const void * _first, const void * _second );
int	ExpiryIdCompare( const void * _first, const void * _second );

Expirys::
Expirys()
{
	expiryData = (ExpiryData_t *)MapSharedMem( EXPIRY_SHMEM_NAME, sizeof( ExpiryData_t ) );
}

void
Expirys::
SetToDefaults()
{
	memset( expiryData, 0, sizeof( ExpiryData_t ) );
	WriteExpirys( expiryData );
}

Expiry_t *
Expirys::
GetAdmin( char * id )
{
	int	i;
	
	for ( i = 0; i < expiryData->numAdmins; i++ )
	{
		if ( _stricmp( expiryData->admins[ i ].id, id ) == 0 )
		{
			return &expiryData->admins[ i ];
		}
	}
	
	return 0;
}

Expiry_t *
Expirys::
GetOp( char * id )
{
	int	i;
	
	for ( i = 0; i < expiryData->numOps; i++ )
	{
		if ( _stricmp( expiryData->ops[ i ].id, id ) == 0 )
		{
			return &expiryData->ops[ i ];
		}
	}
	
	return 0;
}

CfgError_t
Expirys::
AddAdmin( Expiry_t * expiry )
{
    if ( expiryData->numAdmins < MAX_ADMINS )
    {
        memcpy( &expiryData->admins[ expiryData->numAdmins ], expiry, sizeof( Expiry_t ) );
        expiryData->numAdmins++;
        
        return WriteExpirys( expiryData );
    }
    else
    {
        LogDebug( "Attempted to add an administrator when array was full" );
        return CFG_ERR_UNSPECIFIED;
    }
}

CfgError_t
Expirys::
AddOp( Expiry_t ** expiry )
{
    if ( expiryData->numOps < MAX_OPS )
    {
        memcpy( &expiryData->ops[ expiryData->numOps ], *expiry, sizeof( Expiry_t ) );
        *expiry = &expiryData->ops[ expiryData->numOps ];
        expiryData->numOps++;
    
        return WriteExpirys( expiryData );
    }
    else
    {
        LogDebug( "Attempted to add an operator when array was full" );
        return CFG_ERR_UNSPECIFIED;
    }

}

CfgError_t
Expirys::
DeleteAdmin( char * id )
{
	int	i;
	
    for ( i = 0; i < expiryData->numAdmins; i++ )
    {
        if ( _stricmp( expiryData->admins[ i ].id, id ) == 0 )
        {
            for ( ; i < expiryData->numAdmins-1; i++ )
            {
                memcpy( &expiryData->admins[ i ], &expiryData->admins[ i+1 ], sizeof( Expiry_t ) );
            }
            expiryData->numAdmins--;
            break;
        }
    }
	
	return WriteExpirys( expiryData );
}

CfgError_t
Expirys::
DeleteOp( char * id )
{
	int	i;

	for ( i = 0; i < expiryData->numOps; i++ )
	{
		if ( _stricmp( expiryData->ops[ i ].id, id ) == 0 )
		{
			for ( ; i < expiryData->numOps-1; i++ )
			{
				expiryData->ops[ i ] = expiryData->ops[ i+1 ];
			}
		    expiryData->numOps--;
            break;
		}
	}
	
	return WriteExpirys( expiryData );
}

Expiry_t *
Expirys::
GetAdminsByDate()
{
	qsort( expiryData->admins, expiryData->numAdmins, sizeof( Expiry_t ), ExpiryDateCompare );

	return expiryData->admins;
}

Expiry_t *
Expirys::
GetAdminsById()
{
	qsort( expiryData->admins, expiryData->numAdmins, sizeof( Expiry_t ), ExpiryIdCompare );

	return expiryData->admins;
}

Expiry_t *
Expirys::
GetOpsByDate()
{
	qsort( expiryData->ops, expiryData->numOps, sizeof( Expiry_t ), ExpiryDateCompare );

	return expiryData->ops;
}

Expiry_t *
Expirys::
GetOpsById()
{
	qsort( expiryData->ops, expiryData->numOps, sizeof( Expiry_t ), ExpiryIdCompare );

	return expiryData->ops;
}

CfgError_t
Expirys::
WriteExpirysFile()
{
	return WriteExpirys( expiryData );
}


CfgError_t
Expirys::
SetControlExpiration( FILETIME date )
{
	expiryData->controlExpiry = date;
	
	return WriteExpirys( expiryData );
}

int
ExpiryDateCompare( const void * _first, const void * _second )
{
	Expiry_t *	first = (Expiry_t *)_first;
	Expiry_t *	second = (Expiry_t *)_second;
	
	//if ( first->expiration != second->expiration )
	if ( CompareFileTime(&(first->expiration), &(second->expiration)) != 0 )
	{
		//return first->expiration > second->expiration ? 1 : -1;
		return (CompareFileTime(&(first->expiration), &(second->expiration)) == 1) ? 1 : -1;
	}
	else
	{
		return strcmp( first->id, second->id );
	}
}

int
ExpiryIdCompare( const void * _first, const void * _second )
{
	Expiry_t *	first = (Expiry_t *)_first;
	Expiry_t *	second = (Expiry_t *)_second;
	
	return strcmp( first->id, second->id );
}
