#ifndef __SYSTEMERROR_H
#define __SYSTEMERROR_H

#include "AStrings.h"
#include "SystemErrorNums.h"
#include "SystemData.h"

/*
 * Error actions
 */
typedef enum
{
	SERR_ACTION_NONE = 0,		// No action.
	SERR_ACTION_WARNING,		// Post warning message and continue.
	SERR_ACTION_ABORT,			// Post error message and halt rotor run.
	SERR_ACTION_SHUTDOWN,		// Post fatal error message and shut down system.
    SERR_ACTION_RESTART,        // Post fatal error message and restart system.
} SystemErrorAction_t;

/*
 * Structure for error-to-action table.
 */
typedef struct
{
	unsigned				errNum;
	SystemErrorAction_t		normalAction;
	SystemErrorAction_t		researchAction;
	SystemErrorAction_t		demoAction;
	StringId_t				screenText;
	StringId_t				infoText;
} SystemErrorEntry_t;

class SystemError
{
public:
								SystemError();
	const SystemErrorAction_t	GetAction( unsigned errNum );
	const StringId_t			GetInfoText( unsigned errNum );
	const StringId_t			GetScreenText( unsigned errNum );

private:
	const SystemErrorEntry_t *	GetEntry( unsigned errNum );
	
	SystemData_t *	systemData;
};

#endif	// __SYSTEMERROR_H
