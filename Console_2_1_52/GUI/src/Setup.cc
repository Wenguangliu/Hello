/*
 * FILENAME
 * 	Setup.cc
 * 
 * MODULE DESCRIPTION
 * 	Contains setup code called at program start-up.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/neutrino.h>

/* Local headers */
#include "ablibs.h"
#include "Global.h"
#include "abimport.h"
#include "Controller.h"
#include "ScreenSaver.h"
#include "MiddleSection.h"
#include "Shmem.h"
#include "SystemData.h"

unsigned short		cursorType_g = Ph_CURSOR_NONE;
Controller *		controller_g = 0;
double				inactionTime_g = 0;
bool				screenSaveEnabled_g = true;
bool				screenSaveActive_g = false;
SettingsData_t *	settingsData_g = 0;

static void	CreateKeyboardInputRegion();
static int	KeyboardCb( PtWidget_t *widget, void *data, PtCallbackInfo_t *cbinfo );

/*
 * FUNCTION
 * 	Setup
 * 
 * DESCRIPTION
 * 	Starts up the GUI system.
 *
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon return value.
 */

int
Setup( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	SystemData_t * systemData = (SystemData_t *)MapSharedMem( SYSTEM_SHMEM_NAME, sizeof( SystemData_t ) );

	/*
	 *  Get the pointer to the background image shared by all the middle section
	 * 	modules.  This must be done before we create the Controller, which creates
	 *	all the modules that use this image.
	 */
	MiddleSection::GetBackground();

	/*
	 *  Create the Controller.  It initializes the windows and then starts a
	 * 	thread which handles incoming messages from other processes.
	 */
	controller_g = new Controller();

	// If we're not in development mode, create a region to intercept all
	// keyboard input.
	if ( !systemData->keepWindowManager )
	{
		CreateKeyboardInputRegion();
	}

	// Connect to the settings shared memory and pass it as an argument to the
	// thread that does the screen saver.
	settingsData_g = (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );
	ThreadCreate( 0, ScreenSaver, settingsData_g, 0 );

	return Pt_CONTINUE;
}

/*
 * FUNCTION
 * 	Init
 * 
 * DESCRIPTION
 * 	Handles command-line arguments.
 *
 * PARAMETERS
 * 	Command line arguments, as passed from main().
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon return value.
 */

int
Init( int argc, char *argv[] )
{
	int c;
	
	// Get the command line options.
    while ( ( c = getopt( argc, argv, "c" ) ) != -1 )
    {
        switch ( c )
        {
            case 'c':		// Enable mouse cursor.
                cursorType_g = Ph_CURSOR_FINGER;
                break;

			default:
				break;
		}
	}
	

	return Pt_CONTINUE;
}

/*
 * FUNCTION
 * 	KeyboardCb
 * 
 * DESCRIPTION
 * 	Callback for keyboard input to our front region which traps all keyboard
 * 	input. Forwards keyboard input to the text widget in the Enter ID window.
 *
 * PARAMETERS
 * 	Standard Photon callback parameters.
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon return value.
 */

int
KeyboardCb( PtWidget_t *widget, void *data, PtCallbackInfo_t *cbinfo )
{
	PtSendEventToWidget( ABW_ptTextEnterId, cbinfo->event );

	return Pt_CONTINUE;
}


#define	WM_REGION_FIELDS  Ph_REGION_HANDLE | Ph_REGION_FLAGS | Ph_REGION_ORIGIN   | \
                          Ph_REGION_PARENT | Ph_REGION_RECT  | Ph_REGION_EV_SENSE | \
                          Ph_REGION_EV_OPAQUE
#define	WM_REGION_FLAGS   Ph_WND_MGR_REGION | Ph_FORCE_FRONT
#define	WM_REGION_SENSE   Ph_EV_KEY | Ph_EV_BUT_PRESS | Ph_EV_SYSTEM | Ph_EV_INFO

/*
 * FUNCTION
 * 	CreateKeyboardInputRegion
 * 
 * DESCRIPTION
 * 	Creates a region at the front to intercept all keyboard input so we can
 * 	control where it goes.
 */

void
CreateKeyboardInputRegion()
{
	PtRawCallback_t	cb;
	PhRegion_t		region;
	PhArea_t		area;
	PtWidget_t *	regionWgt;
	PtArg_t			args[15];
	int				n = 0;

	// The region covers all positive coordinates.
	area.pos.x  = area.pos.y  = 0;
	area.size.w = area.size.h = SHRT_MAX;

	// Set up the keyboard event handler. */
	cb.event_mask = Ph_EV_KEY;
	cb.event_f    = KeyboardCb;
	cb.data       = NULL;

	// No parent widget.
	PtSetParentWidget( NULL );

	// Set the region parameters and create it.
	PtSetArg( args + n++, Pt_ARG_REGION_PARENT, Ph_ROOT_RID, 0 );
	PtSetArg( args + n++, Pt_ARG_REGION_OWNER, 0, 0 );
	PtSetArg( args + n++, Pt_ARG_REGION_HANDLE, 0, 0 );
	PtSetArg( args + n++, Pt_ARG_REGION_FIELDS, WM_REGION_FIELDS, WM_REGION_FIELDS );
	PtSetArg( args + n++, Pt_ARG_REGION_FLAGS, WM_REGION_FLAGS, WM_REGION_FLAGS );
	PtSetArg( args + n++, Pt_ARG_REGION_SENSE, WM_REGION_SENSE, WM_REGION_SENSE );
	PtSetArg( args + n++, Pt_ARG_REGION_OPAQUE, Ph_EV_KEY, Ph_EV_KEY );
	PtSetArg( args + n++, Pt_ARG_AREA, &area, 0 );
	PtSetArg( args + n++, Pt_ARG_FILL_COLOR, Pg_TRANSPARENT, 0 );
	PtSetArg( args + n++, Pt_ARG_REGION_CURSOR, 0, 0 );
	PtSetArg( args + n++, Pt_ARG_REGION_CURSOR_COLOR, Pg_TRANSPARENT, 0 );
	PtSetArg( args + n++, Pt_ARG_RAW_CALLBACKS, &cb, 0 );
	regionWgt = PtCreateWidget( PtRegion, NULL, n, args );
	PtRealizeWidget( regionWgt );

	// Set the region's rectangle.
	region.rid = regionWgt->rid;
	regionWgt->extent.ul.x = regionWgt->extent.ul.y = SHRT_MIN;
	PhRegionChange( Ph_REGION_RECT, 0, &region, &regionWgt->extent, NULL );
}
