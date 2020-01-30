/*
 * FILENAME
 * 	Screensaver.cc
 * 
 * MODULE DESCRIPTION
 * 	Handles the screen saver functionality.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <Ph.h>
#include <Pt.h>
#include <photon/PxImage.h>
#include <sys/neutrino.h>
#include <hw/inout.h.>

/* Local headers */
#include "Global.h"
#include "SettingsData.h"
#include "Shmem.h"
#include "LogErr.h"
#include "Hardware.h"
#include "Gpio.h"
#include "FactoryData.h"

#define PICCOLO_SCREEN_SAVER_DIR	"/Abaxis/ScreenSaver/Piccolo/"
#define VETSCAN_SCREEN_SAVER_DIR	"/Abaxis/ScreenSaver/VetScan/"

typedef struct
{
	int		len;
	char *	files[20];
} FileList_t;
	
void 			DoScreenSave( FileList_t * fileList );
FileList_t *	GetScreenSaverFiles();
void			LoadImage( char * fileName );

static PtWidget_t *		window = 0;
static PtWidget_t *		label = 0;
static const PhPoint_t	showPos = {240, 0};
static const PhPoint_t	hidePos = {-400, 0};
static SettingsData_t *	settingsData;
static GpioImage_t *	gpioImage;
static FactoryData_t *	factoryData = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );

void *
ScreenSaver( void * arg )
{
	double			inactionTime = 0;
	bool			screenSaveEnabled = false;
	FileList_t *	fileList;
	
	settingsData = (SettingsData_t *)arg;
	if ( settingsData == 0 )
	{
		return 0;
	}

	// Get the list of screen saver image files.
	fileList = GetScreenSaverFiles();
	if ( fileList->len == 0 )
	{
		LogError( "screen saver didn't find any image files" );
		return 0;
	}
	
	// Get the GPIO image so we can set the front panel LED.
	gpioImage = (GpioImage_t *)MapSharedMem( GPIO_SHMEM_NAME, sizeof( GpioImage_t ) );
	if ( gpioImage == 0 )
	{
		return 0;
	}

	// Enable I/O privilege for setting the front panel LED.
	if ( ThreadCtl( _NTO_TCTL_IO, 0 ) == -1 )
	{
		LogError( "ThreadCtl failed" );
		return 0;
	}
	
	PtEnter( 0 );

	// Create the window connection and get the label that holds the image.
	window = ApCreateModule( ABM_winScreenSaver, 0, 0 );
	label = ABW_ptLabelScreenSaver;

	// Pre-load the first mage.
	LoadImage( fileList->files[0] );

	PtLeave( 0 );
		
	while ( 1 )
	{
		// Check every 0.5 seconds to see if it's time to display the screen saver. 
		delay( 500 );

		// Increment the inactive time (in minutes) by 1/2 second and get the
		// value for use in this thread.
		pthread_sleepon_lock();
		screenSaveEnabled = ( screenSaveEnabled_g && !errorScreenActive_g );
		inactionTime_g += 0.0083333;
		inactionTime = inactionTime_g;
		pthread_sleepon_unlock();
		
		// If the screen saver timeout is not zero (Never) and the inaction time
		// has exceeded the timeout, activate.
		if ( settingsData->screenSaverTimeout > 0 &&
			 inactionTime > settingsData->screenSaverTimeout &&
			 screenSaveEnabled )
		{
			DoScreenSave( fileList );
		}
		
	}
}

void
DoScreenSave( FileList_t * fileList )
{
	double		inactionTime = 0;
	static int	index = 0;
	static int	currentImageTime;
	bool		powerSaveStart = true;
// GLD
// This isn't really the save on time, it's the delay for save time plus the save on time.
	double		saverOnTime = settingsData->screenSaverTimeout;

	PtEnter( 0 );
	
	// Move the screen saver window out of hiding and make sure it's at the
	// front.
	PtSetResource( window, Pt_ARG_POS, &showPos, 0 );
	PtWindowToFront( window );

	PtFlush();
	PtLeave( 0 );

	// Flag the screen saver as active.
	pthread_sleepon_lock();
	screenSaveActive_g = true;
	pthread_sleepon_unlock();

	// Loop until some action triggers the screen saver to go away.
	while ( 1 )
	{
		// Delay 1/2 second between checks for activity.
		delay( 500 );
		
		// Increment saver on time by 1/2 second.
		saverOnTime += 0.0083333;

		// Get our elapsed time.
		pthread_sleepon_lock();
		inactionTime = inactionTime_g;
		pthread_sleepon_unlock();
		
		// Action detected.
		if ( inactionTime == 0 )
		{
			PtEnter( 0 );

			// Hide the screen saver window.
			PtSetResource( window, Pt_ARG_POS, &hidePos, 0 );

			// Restore the currently active window to the front.
			controller_g->RestoreCurrentWindow();

			// Pre-load the next image, so it's ready the next time the
			// screen-saver is activated. (If we don't do this and we're
			// currently in power-save mode, the next time we won't have an
			// image at the beginning of screen-save.)
			if ( ++index >= fileList->len )
			{
				index = 0;
			}
			LoadImage( fileList->files[ index ]  );

			PtFlush();
			PtLeave( 0 );
				
			// Restore the backlight to its proper setting.
			SetBacklight( settingsData->backlightLevel );

			// Put the front panel LED in solid-on mode.
			*gpioImage |= GPIO_LED;
			out8( GPIO_PORT_REG, *gpioImage );

			// Flag the screen saver as inactive.
			pthread_sleepon_lock();
			screenSaveActive_g = false;
			pthread_sleepon_unlock();
			return;
		}

		else
		{
			// Still in screen saver if power save timeout is off or if the time
			// is less than the power save timeout time.
			if ( (settingsData->powerSaveTimeout == 0) ||
				 (saverOnTime < settingsData->powerSaveTimeout) )
			{
				if ( ++currentImageTime >= 120 )
				{
					// Load the next image.
					if ( ++index >= fileList->len )
					{
						index = 0;
					}
					PtEnter( 0 );
					LoadImage( fileList->files[ index ]  );
					PtFlush();
					PtLeave( 0 );
	
					currentImageTime = 0;
				}
			}
			
			// Power save mode.
			else
			{
				if ( powerSaveStart )
				{
					PtEnter( 0 );
					PtSetResource( label, Pt_ARG_LABEL_IMAGE, 0, 0 );
					PtFlush();
					PtLeave( 0 );

					// Set the backlight to its lowest setting.
					SetBacklight( 0 );
	
					// Put the front panel LED in flash mode.
					*gpioImage &= ~GPIO_LED;
					out8( GPIO_PORT_REG, *gpioImage );

					powerSaveStart = false;
				}
			}
		}
	}
}

// GLD
// Note: This gets called from the Photon thread, so it can do stuff in that
// thread without locking Photon.  InactionTime_g is also in the main thread,
// so it can clear it without using a lock.
int
Touch( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	inactionTime_g = 0;

	PtSetResource( window, Pt_ARG_POS, &hidePos, 0 );
	PtSetResource( ABW_winLcd, Pt_ARG_POS, &showPos, 0 );

	return Pt_CONTINUE;
}

FileList_t *
GetScreenSaverFiles()
{
	static FileList_t	fileList;
	DIR *				dirp;
	struct dirent *		direntp;
	char *				ext;
	const char *		screenSaverDir;
	
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		screenSaverDir = PICCOLO_SCREEN_SAVER_DIR;
	}
	else
	{
		screenSaverDir = VETSCAN_SCREEN_SAVER_DIR;
	}
	
	memset( &fileList, 0, sizeof( FileList_t ) );
	
	dirp = opendir( screenSaverDir );
	if ( dirp != 0 )
	{
		for ( direntp = readdir( dirp ); fileList.len < 20 && direntp != 0; direntp = readdir( dirp ) )
		{
			ext = strrchr( direntp->d_name, '.' );
			if ( ext != 0 && ( stricmp( ext, ".jpg" ) == 0 ||
							   stricmp( ext, ".jpeg" ) == 0 ||
							   stricmp( ext, ".bmp" ) == 0 ||
							   stricmp( ext, ".gif" ) == 0 ||
							   stricmp( ext, ".pcx" ) == 0 ) )
			{
				fileList.files[ fileList.len ] = (char *)malloc( strlen( screenSaverDir ) + strlen( direntp->d_name ) + 1 );
				strcpy( fileList.files[ fileList.len ], screenSaverDir );
				strcat( fileList.files[ fileList.len ], direntp->d_name );
				fileList.len++;
			}
		}
		closedir( dirp );
	}
	
	return &fileList;
}

void *
MemoryAllocate( long nbytes, int type )
{
	if ( type == PX_IMAGE )
	{
		return PgShmemCreate( nbytes, 0 );
	}
	else
	{
		return calloc( 1, nbytes );
	}
}

void *
MemoryFree( void * memory, int type )
{
	if ( type == PX_IMAGE )
	{
		PgShmemDestroy( memory );
	}
	else
	{
		free( memory );
	}
	
	return 0;
}

void
LoadImage( char * fileName )
{
	PhImage_t *	img;
	PxMethods_t	methods;
	
	memset( &methods, 0, sizeof( PxMethods_t ) );
	methods.px_alloc = MemoryAllocate;
	methods.px_free = MemoryFree;
	methods.flags |= PX_LOAD;

	img = PxLoadImage( fileName, &methods );
	if ( img == 0 )
	{
		LogError( "screen saver image load error\n" );
		return;
	}
	
	img->flags |= Ph_RELEASE_IMAGE_ALL;
	
	PtSetResource( label, Pt_ARG_LABEL_IMAGE, img, 0 );
	
	free( img );
}
