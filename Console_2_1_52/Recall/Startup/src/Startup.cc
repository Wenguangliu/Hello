/* Y o u r   D e s c r i p t i o n					   */
/*							AppBuilder Photon Code Lib */
/*										 Version 2.03  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <mqueue.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <stdint.h>
#include <hw/inout.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <errno.h>
#include <ctype.h>

#include "ablibs.h"
#include "abimport.h"
#include "AnalysisMessages.h"
#include "AstmMessages.h"
#include "AStrings.h"
#include "BarcodeData.h"
#include "CalcBfds.h"
#include "CalcResultsMessages.h"
#include "CalData.h"
#include "CommonDefines.h"
#include "CounterData.h"
#include "DownloadEngine.h"
#include "EngineCommMessages.h"
#include "ErrLoggerMessages.h"
#include "GetThumbKeyMode.h"
#include "Gpio.h"
#include "GuiMessages.h"
#include "Hardware.h"
#include "HardwareData.h"
#include "HeaterControlMessages.h"
#include "LogErr.h"
#include "MountThumbDrive.h"
#include "OrdersData.h"
#include "ParseCounters.h"
#include "ParseExpirys.h"
#include "ParseFactory.h"
#include "ParseHardware.h"
#include "ParseRefRanges.h"
#include "ParseSettings.h"
#include "ParseSystem.h"
#include "ParseUnits.h"
#include "RawData.h"
#include "RecallMessages.h"
#include "RefRanges.h"
#include "ResultsData.h"
#include "ResultsDb.h"
#include "RocData.h"
#include "SerialCommMessages.h"
#include "SettingsMessages.h"
#include "SetDateTime.h"
#include "Shmem.h"
#include "StartupMessages.h"
#include "SystemError.h"
#include "SystemErrorNums.h"
#include "TranslateUtf.h"
#include "VersionData.h"

void *			BackgroundHandler( void * arg );
static void *	BezelButtonHandler( void * arg );
static void		BlankScreen( bool blank );
static bool		CopyRefRangesFile( RefRangeData_t * refRanges );
static mqd_t	CreateMessageQueue( const char * queueName );
static void *	CreateResultsDatabase();
static void		CreateResultsDatabaseFiles( const char * indexFileName, const char * dataFileName );
static void	*	CreateSharedMem( const char * sharedMemName, size_t size );
static void		ErrorStop( SystemErrNum_t );
static char		GetNvramProductCode();
static void		GetVersionData();
static void		GotoManufacturing();
static void		InitOrdersList();
static void		KillAll();
static void		KillProcess( int pid );
static void		LoadRefRanges( RefRangeData_t * refRanges );
static void		LoadUnits( UnitsConfig_t * unitsConfig );
static void		Message( StringId_t stringId );
static void		Restart();
static void		SendFactoryMessage( FactoryData_t * factoryData );
static void		SendFanMessage();
static void		SendSoundMessage();
static void		SendMessageReceiverMessage();
static void		SetKeyboard( Language_t language );
static void		SetString( PtWidget_t * widget, const char * str, bool doSpace );
static void		SlayWindowManager();
static int		StartProcess( const char * processName );
static int		StartProcess( const char * processName, char * const argv[] );
static void *	TimerHandler( void * arg );
static void *	TouchScreenHandler( void * arg );
static void		WaitForInit();
static void		WaitForMessages();

// Flags
static bool					calibrationActive = false;
static bool					createResultsDatabase = false;
static bool					cursorEnabled = false;
static bool					downloadEngine = true;
static bool					inDemoMode = false;
static bool					inResearchMode = false;
static bool					killPwm = true;
static bool					noStartProcess = false;

// Comm queues
static mqd_t				startupQueue;
static mqd_t				engineCommQueue;
static mqd_t				guiQueue;

// Misc. local variables
static pthread_t			touchThread;
static AStrings	*			strings;
static SystemErrorNumber_t	systemErrNum;

// Shared memory
static FactoryData_t *		factoryData;
static HardwareData_t *		hardwareData;
static OrdersList_t *		ordersList;
static SettingsData_t *		settings;
static SystemData_t *		systemData;
static SystemError *		systemError;
static VersionData_t *		versionData;

// Process pids
static int					analysisPid = 0;
static int					calcResultsPid = 0;
static int					engineCommPid = 0;
static int					errLoggerPid = 0;
static int					guiPid = 0;
static int					heaterControlPid = 0;
static int					recallPid = 0;
static int					settingsPid = 0;

#define BEZEL_BUTTON_INTNUM		6
#define TOUCH_SCREEN_INTNUM		10
#define SLEEP_TIME				0.5		// # seconds minimum hold on bezel button
#define SHUTDOWN_TIME			2.5		// # seconds minimum hold on bezel button
#define MAX_STARTUP_TIME		30		// # seconds to wait for Engine, GUI, etc.  Needs one second for delay, too.
#define BILLION					1000000000LL;
#define	TOUCH_TIMEOUT			5		// Maximum wait time (seconds) after first touch before calibration is canceled.

int
Init( int argc, char *argv[] )
{
	int		c;
	
	// Get the command line options.
	while ( ( c = getopt( argc, argv, "cdeprx" ) ) != -1 )
	{
		switch ( c )
		{
			case 'c':		// Enable mouse cursor.
				cursorEnabled = true;
				break;

			case 'd':		// Demo mode.
				inDemoMode = true;
				break;

			case 'e':		// Don't download engine.
				downloadEngine = false;
				break;

			case 'p':		// Don't kill window manager.
				killPwm = false;
				break;

			case 'r':		// Enable research mode.
				inResearchMode = true;
				break;
			
			case 'x':		// Create results database.
				createResultsDatabase = true;
			default:
				break;
		}
	}
	
	return Pt_CONTINUE;
}

int
Setup( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	ThreadCreate( 0, BackgroundHandler, link_instance, 0 );
	
	return Pt_CONTINUE;
}

void *
BackgroundHandler( void * arg )
{
	/*
	 * We create all the shared memory objects and message queues here, whether
	 * we need them in this process or not.  In this way, we know they are there
	 * and ready to go when the other processes start and connect to them.  This
	 * avoids synchonization issues and ensures that the shared memory and
	 * message queues are in a known state.
	 */

	BarcodeData_t *			barcodeData;
	ResultsDbChanged_t *	dbChanged;
	CalibrationData_t *		calData;
	CounterData_t *			counterData;
	ExpiryData_t *			expiryData;
	GpioImage_t *			gpioImage;
	RawData_t *				rawData;
	RefRangeData_t *		refRanges;
	RotorConfig_t *			rotorConfig;
	RotorResults_t *		results;
	UnitsConfig_t *			unitsConfig;
	mqd_t					analysisQueue;
	mqd_t					astmQueue;
	mqd_t					calcResultsQueue;
	mqd_t					errLoggerQueue;
	mqd_t					heaterControlQueue;
	mqd_t					recallQueue;
	mqd_t					serialCommQueue;
	mqd_t					settingsQueue;
	CfgError_t				parseError;
	char					title[40];
	pthread_t				timerThread;
	SystemErrorNumber_t		errNum;
	PtWidget_t *			window = (PtWidget_t *)arg;
	PhPoint_t				pos = {-400, 0};
	unsigned				enginePartNum;

	// Create the message queues.
	analysisQueue		= CreateMessageQueue( ANALYSIS_QUEUE_NAME );
	astmQueue			= CreateMessageQueue( ASTM_QUEUE_NAME );
	calcResultsQueue	= CreateMessageQueue( CALC_RESULTS_QUEUE_NAME );
	engineCommQueue		= CreateMessageQueue( ENGINE_COMM_QUEUE_NAME );
	errLoggerQueue		= CreateMessageQueue( ERR_LOGGER_QUEUE_NAME );
	guiQueue			= CreateMessageQueue( GUI_QUEUE_NAME );
	heaterControlQueue	= CreateMessageQueue( HEATER_CONTROL_QUEUE_NAME );
	recallQueue			= CreateMessageQueue( RECALL_QUEUE_NAME );
	serialCommQueue		= CreateMessageQueue( SERIAL_COMM_QUEUE_NAME );
	settingsQueue		= CreateMessageQueue( SETTINGS_QUEUE_NAME );
	startupQueue		= CreateMessageQueue( STARTUP_QUEUE_NAME );

	// Create the shared memory.
	barcodeData 	= (BarcodeData_t *)		CreateSharedMem( BARCODE_SHMEM_NAME,	sizeof( BarcodeData_t ) );
	calData 		= (CalibrationData_t *)	CreateSharedMem( CAL_DATA_SHMEM_NAME,	sizeof( CalibrationData_t ) );
	counterData 	= (CounterData_t *)		CreateSharedMem( COUNTERS_SHMEM_NAME,	sizeof( CounterData_t ) );
	dbChanged		= (ResultsDbChanged_t *)CreateSharedMem( DATABASE_SHMEM_NAME,	sizeof( ResultsDbChanged_t ) );
	expiryData		= (ExpiryData_t *)		CreateSharedMem( EXPIRY_SHMEM_NAME,		sizeof( ExpiryData_t ) );
	factoryData 	= (FactoryData_t *)		CreateSharedMem( FACTORY_SHMEM_NAME,	sizeof( FactoryData_t ) );
	gpioImage		= (GpioImage_t *)		CreateSharedMem( GPIO_SHMEM_NAME,		sizeof( GpioImage_t ) );
	hardwareData	= (HardwareData_t *)	CreateSharedMem( HARDWARE_SHMEM_NAME,	sizeof( HardwareData_t ) );
	ordersList		= (OrdersList_t *)		CreateSharedMem( ORDERS_SHMEM_NAME,		sizeof( OrdersList_t ) );
	rawData 		= (RawData_t *)			CreateSharedMem( RAW_SHMEM_NAME,		sizeof( RawData_t ) );
	refRanges		= (RefRangeData_t *)	CreateSharedMem( REF_RANGES_SHMEM_NAME,	sizeof( RefRangeData_t ) );
	results 		= (RotorResults_t *)	CreateSharedMem( RESULTS_SHMEM_NAME,	sizeof( RotorResults_t ) );
	rotorConfig		= (RotorConfig_t *)		CreateSharedMem( ROC_SHMEM_NAME,		sizeof( RotorConfig_t ) );
	settings		= (SettingsData_t *)	CreateSharedMem( SETTINGS_SHMEM_NAME,	sizeof( SettingsData_t ) );
	systemData		= (SystemData_t *)		CreateSharedMem( SYSTEM_SHMEM_NAME,		sizeof( SystemData_t ) );
	unitsConfig		= (UnitsConfig_t *)		CreateSharedMem( UNITS_SHMEM_NAME,		sizeof( UnitsConfig_t ) );
	versionData		= (VersionData_t *)		CreateSharedMem( VERSION_SHMEM_NAME,	sizeof( VersionData_t ) );

	// Start the error logger so it can log any errors properly.
	errLoggerPid = StartProcess( ERRLOGGER_PROGRAM );

	// Get the time and date from the RTC and set the system clock. This depends on the ErrLogger queue to log
	// errors, so it needs to wait until that queue has been created.
	SetDateTime();
	
	// If the 'x' command line option is set, this creates empty databases for
	// both regular and manufacturing/research modes.
	if ( createResultsDatabase == true )
	{
		CreateResultsDatabase();
		exit( 0 );
	}

	// We can't create the strings object until here because it depends on Factory and Settings shared memory.
	strings = new AStrings();

	// Parse the factory config file.  We need this early on to get the product code.
	parseError = ParseFactory( factoryData );
	if ( parseError != CFG_ERR_NONE )
	{
		LogError( "error parsing factory config file: %d", parseError );
		ErrorStop( SERR_FILE_READ );
	}
	
	// Get the product code from the RTC NVRAM and compare it to the product code
	// in factory.xml.
	if ( GetNvramProductCode() != factoryData->abaxisProductCode )
	{
		LogError( "product code mismatch" );
		ErrorStop( SERR_PRODUCT_CODE );
	}
	
	// Parse the settings config file.  If there is a file open error, this
	// routine will set the settings to the defaults.  We need this early on to get the language.
	parseError = ParseSettings( settings );
	if ( parseError != CFG_ERR_NONE && parseError != CFG_ERR_FILE_OPEN )
	{
		LogError( "error parsing settings config file: %d", parseError );
		ErrorStop( SERR_FILE_READ );
	}
	
	// Parse the expirys config file if this is a piccolo.
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		parseError = ParseExpirys( expiryData );
		if ( parseError != CFG_ERR_NONE && parseError != CFG_ERR_FILE_OPEN )
		{
			LogError( "error parsing expiry config file: %d", parseError );
			ErrorStop( SERR_FILE_READ );
		}
	}

	// Parse the hardware config file. This must be done before we start the Engine.
	parseError = ParseHardware( hardwareData );
	if ( parseError != CFG_ERR_NONE && parseError != CFG_ERR_FILE_OPEN )
	{
		LogError( "error parsing hardware config file: %d", parseError );
		ErrorStop( SERR_FILE_READ );
	}	

	// Start the timer thread.  It updates the progress bar and sends a timeout
	// message if it hits the maximum time and we haven't heard from the Engine
	// yet.
	pthread_create( &timerThread, 0, TimerHandler, 0 );
	
	// Create the SystemError object now.  We can't create it until after the shared
	// memory is first created because it relies on the System shmem.
	systemError = new SystemError();
	
	// Init the GPIO port.  This must be done before the Engine code may be
	// downloaded and before the bezel button handler may be installed.  It must
	// be done after the gpioImage shared memory has been created.
	GpioInit( gpioImage );

	// Install the bezel button handler.
	ThreadCreate( 0, BezelButtonHandler, gpioImage, 0 );

	// Install the touch calibrate handler.
	// NOTE: This mustn't be done until after the settings config file has been
	// read because we pass the language to the calibration utility.
	pthread_create( &touchThread, 0, TouchScreenHandler, 0 );
	
	// Get the version data.
	GetVersionData();

	// Set the top title.
	strcpy( title, (*strings)[ STR_PRODUCT_NAME ] );
	strcat( title, " " );
	strcat( title, versionData->version );
	PtEnter( 0 );
	SetString( ABW_ptLabelTitle, title, false );
	PtLeave( 0 );
	
	// Parse the counters config file.
	parseError = ParseCounters( counterData );
	if ( parseError != CFG_ERR_NONE && parseError != CFG_ERR_CRC )
	{
		LogError( "error parsing counters config file: %d", parseError );
	}

	// Load the Reference Ranges.
	LoadRefRanges( refRanges );

	// Load the Units configuration.
	LoadUnits( unitsConfig );

	// Set the backlight level.
	SetBacklight( settings->backlightLevel );

	// Calculate the BFD calibration factors.
	errNum = CalcBfds( factoryData );
	if ( errNum != SERR_NONE )
	{
		ErrorStop( errNum );
	}
		
	// Set the system data.
	ParseSystem( systemData );
	systemData->inDemoMode = inDemoMode;
	systemData->inResearchMode = systemData->inResearchMode || inResearchMode;	// True if it's set in either system.xml or with a command line switch.
	systemData->inAdministratorMode = false;
	systemData->rotorPresent = false;	// Set this before starting the engine and waiting for its status.
	systemData->systemStatus = SYSTEM_STATUS_BUSY;
	
	if ( systemData->keepWindowManager )
	{
		killPwm = false;
	}

	if ( killPwm )
	{
		SlayWindowManager();
	}

	// Check for thumb key with mode setting.
	GetThumbKeyMode( systemData );

	// If we're set to ASTM, we need to clear the Research mode flag.
	if ( settings->comm.interface == COMM_ASTM )
	{
		systemData->inResearchMode = false;
	}

	// Initialize the ASTM orders list.
	InitOrdersList();

	if ( systemData->enableFtpTelnet )
	{
		chmod( "/usr/bin/ftpd", 0777 );
		chmod( "/usr/bin/telnetd", 0777 );
	}
	else
	{
		chmod( "/usr/bin/ftpd", 0 ); 
		chmod( "/usr/bin/telnetd", 0 ); 
	}

	// Start the EngineComm process before starting the Engine, so we're sure to
	// get start-up messages from the Engine.
	engineCommPid = StartProcess( ENGINECOMM_PROGRAM );

	// Register with EngineComm for messages.
	SendMessageReceiverMessage();
	
	//Download the firmware to the Engine.
	enginePartNum = atoi( hardwareData->assemblies.enginePcb.data );
	if ( downloadEngine )
	{
		if ( (enginePartNum == 0) || (enginePartNum == 11003200) || (enginePartNum == 11003203) )
		{
			if ( DownloadEngine( gpioImage ) == false )
			{
				// Put up the error screen and stop.
				ErrorStop( SERR_ENGINE_DOWNLOAD );
			}
		}
		else
		{
			Set196OnceMode( gpioImage, true );
			Set196OnceMode( gpioImage, false );
		}
	}

	// Start the other processes.
	if ( cursorEnabled )
	{
		char * const argv[] = { (char *)GUI_PROGRAM, (char *)"-c", 0 };
		guiPid = StartProcess( GUI_PROGRAM, argv );
	}
	else
	{
		guiPid = StartProcess( GUI_PROGRAM );
	}

	// Wait for the Engine and GUI to let us know they're both up and ready.
	WaitForInit();

	// If we're calibrating the touchscreen, wait for it to finish before
	// continuing.
	pthread_sleepon_lock();
	while ( calibrationActive == true )
	{
		pthread_sleepon_wait( &calibrationActive );
	}
	pthread_sleepon_unlock();

	// Cancel the touchscreen thread.
	pthread_cancel( touchThread );

	calcResultsPid		= StartProcess( CALCRESULTS_PROGRAM );
	heaterControlPid	= StartProcess( HEATERCONTROL_PROGRAM );
	analysisPid			= StartProcess( ANALYSIS_PROGRAM );
	recallPid			= StartProcess( RECALL_PROGRAM );
	settingsPid			= StartProcess( SETTINGS_PROGRAM );

	if ( settings->comm.interface == COMM_ASTM )
	{
		systemData->commPid = StartProcess( ASTM_PROGRAM );
	}
	else
	{
		systemData->commPid = StartProcess( SERIALCOMM_PROGRAM );
	}

	// Wait a few seconds for Analysis to get the GUI up, then stop the progress
	// bar and time-out timer.
	sleep( 4 );
	pthread_cancel( timerThread );

	// Move the Startup window offscreen, so we don't end up masking the GUI.
	PtEnter( 0 );
	PtSetResource( window, Pt_ARG_POS, &pos, 0 );
	PtLeave( 0 );

	// Send the factory config info to the Engine.
	SendFactoryMessage( factoryData );

	// Configure the keyboard.
	SetKeyboard( settings->keyboard );

	WaitForMessages();
	
	return 0;
}

void *
CreateSharedMem( const char * sharedMemName, size_t size )
{
	int		fdShm;
	void *	memPtr;

	// Try to delete it, in case it already exists.  We want to start with a clean slate.
	shm_unlink( sharedMemName );
	
	fdShm = shm_open( sharedMemName, O_RDWR | O_CREAT, 0777 );

	if ( fdShm == -1 )
	{
		LogError( "failed to create shared memory file %s", sharedMemName );
	}
	
	if ( ftruncate( fdShm, size ) == -1 )
	{
		LogError( "failed to set shared memory size %s", sharedMemName );
	}
	
	memPtr = mmap( 0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fdShm, 0 );
	if ( memPtr == MAP_FAILED )
	{
		LogError( "failed to map raw data shared memory" );
		return 0;
	}
	close( fdShm );
	
	return memPtr;
}

mqd_t
CreateMessageQueue( const char * queueName )
{
	// Try to delete it if it already exists.
	mq_unlink( queueName );

	mqd_t queue = mq_open( queueName, O_RDWR | O_CREAT, 0777, NULL );
	
	if ( queue == -1 )
	{
		LogError( "failed to create message queue %s", queueName );
		return 0;
	}
	
	return queue;
}

int
StartProcess( const char * processName )
{
	int pid = 0;
	
	pthread_sleepon_lock();
	if ( !noStartProcess )
	{
		pid = spawnlp( P_NOWAIT, processName, processName,  0 );
		
		if ( pid == -1 )
		{
			LogError( "failed to start process %s", processName );
		}
	}
	pthread_sleepon_unlock();
	
	return pid;
}

int
StartProcess( const char * processName, char * const argv[] )
{
	int pid = 0;
	
	pthread_sleepon_lock();
	if ( !noStartProcess )
	{
		pid = spawnvp( P_NOWAIT, processName, argv );
		
		if ( pid == -1 )
		{
			LogError( "failed to start process %s", processName );
		}
	}
	pthread_sleepon_unlock();
	
	return pid;
	
}

void
SendFactoryMessage( FactoryData_t * factoryData )
{
	EngineCommReceiveMsg_t	msg;
	int						i;
	
	// Create the message.
	msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
	msg.data.engineMsg.message[0] = 'F';
	msg.data.engineMsg.message[1] = ESC;
	msg.data.engineMsg.message[2] = 32;		// LSB of binary count.
	msg.data.engineMsg.message[3] = 0;		// MSB of binary count.
	
	msg.data.engineMsg.message[4] = factoryData->globalCuvetteDelay & 0xFF;
	msg.data.engineMsg.message[5] = factoryData->globalCuvetteDelay >> 8;

	for ( i = 0; i < 30; i++ )
	{
		msg.data.engineMsg.message[i+6] = factoryData->cuvetteDelays[i];
	}

	msg.data.engineMsg.length = 36;

	if ( engineCommQueue != 0 )
	{
		mq_send( engineCommQueue, (const char *)&msg, sizeof( EngineCommReceiveMsg_t ), MQ_PRIO_MAX-1 );
	}
}

void
SendSoundMessage()
{
	GuiReceiveMsg_t	msg;
	
	// Make sure the volume is up.
	msg.msgId = MSG_GUI_SOUND_SET_CLICK_VOLUME;
	msg.data.soundData.volume = 100;
	mq_send( guiQueue, (const char *)&msg, sizeof( int ) + sizeof( GuiSoundData_t ), MQ_PRIO_MAX-1 );
	
	// Do the sound.
	msg.msgId = MSG_GUI_SOUND_CLICK;
	mq_send( guiQueue, (const char *)&msg, sizeof( int ), MQ_PRIO_MAX-1 );
}

void
SendFanMessage()
{
	EngineCommReceiveMsg_t	msg;

	msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
	msg.data.engineMsg.message[0] = 'H';
	msg.data.engineMsg.message[1] = 'S';
	msg.data.engineMsg.message[2] = ESC;
	msg.data.engineMsg.message[3] = 3;		// LSB of binary count.
	msg.data.engineMsg.message[4] = 0;		// MSB of binary count.
	msg.data.engineMsg.message[5] = 0;		// Top DAC.
	msg.data.engineMsg.message[6] = 0;		// Bottom DAC.
	msg.data.engineMsg.message[7] = 255;	// Fan DAC.
	msg.data.engineMsg.length = 8;

	if ( engineCommQueue != 0 )
	{
		mq_send( engineCommQueue, (const char *)&msg, sizeof( EngineCommReceiveMsg_t ), MQ_PRIO_MAX-1 );
	}
}

void
WaitForInit()
{
	ssize_t				receivedSize = 0;
	EngineCommSendMsg_t	msg;
	struct mq_attr		mqstat;
	unsigned char		status;
	bool				engineReady = false;
	bool				guiReady = false;
	SystemErrorNumber_t	errNum = SERR_NONE;
	
	// If we're debugging the Engine, don't wait for the ready message from it.
	if ( downloadEngine == false )
	{
		engineReady = true;
	}
	
	// Get the queue's size.
	if ( mq_getattr( startupQueue, &mqstat ) == -1 )
	{
		LogError( "failure to get message queue attributes" );
		return;
	}

	while ( 1 )
	{
		receivedSize = mq_receive( startupQueue, (char *)&msg, mqstat.mq_msgsize, NULL );
		
		if ( receivedSize > 0 )
		{
			switch ( msg.msgId )
			{
				case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
					// Drawer message.
					if ( msg.data.message[0] == 'D' )
					{
						status = msg.data.message[8];

						if ( status & 0x80 )		// Drawer jammed
						{
							LogError( "drawer jammed" );
						}
						else if ( status & 0x10 )	// Drawer closed
						{
							engineReady = true;

							// Set the rotor present flag.
							systemData->rotorPresent = ((status & 0x40) != 0);
						}
						else	// Drawer open
						{
							LogDebug( "drawer open" );
						}
					}
					// Engine error message.
					else if ( msg.data.message[0] == 'E' )
					{
						errNum = (SystemErrorNumber_t)*(unsigned short *)&msg.data.message[8];
						LogError( "Engine error %04x", errNum );
					}
					break;

				case MSG_STARTUP_ERROR:
					if ( !engineReady )
					{
						LogError( "Engine did not respond in %d seconds", MAX_STARTUP_TIME );
						errNum = SERR_ENGINE_INIT_TIMEOUT;
					}
					else if ( !guiReady )
					{
						LogError( "GUI did not respond in %d seconds", MAX_STARTUP_TIME );
						errNum = SERR_PROCESS_INIT_TIMEOUT;
					}
					break;
					
				case MSG_STARTUP_GUI_READY:
					guiReady = true;
					break;

				default:
					break;
			}
		}
		
		// Error, so go to error-stop mode.
		if ( errNum != SERR_NONE )
		{
			// If touchscreen calibration in progress, wait for it to stop.
			pthread_sleepon_lock();
			while ( calibrationActive == true )
			{
				pthread_sleepon_wait( &calibrationActive );
			}
			pthread_sleepon_unlock();

			// Cancel the touch calibration thread.
			pthread_cancel( touchThread );
	
			// Put up the error screen and stop.
			ErrorStop( errNum );
		}
		
		if ( engineReady && guiReady )
		{
			return;
		}
	}
}

void
WaitForMessages()
{
	ssize_t			receivedSize = 0;
	StartupMsg_t	msg;
	struct mq_attr	mqstat;
	
	// Get the queue's size.
	if ( mq_getattr( startupQueue, &mqstat ) == -1 )
	{
		LogError( "failed to get message queue attributes" );
		return;
	}


	while ( 1 )
	{
		receivedSize = mq_receive( startupQueue, (char *)&msg, mqstat.mq_msgsize, 0 );
		
		if ( receivedSize > 0 )
		{
			if ( msg.msgId == MSG_STARTUP_GOTO_AMS )
			{
				GotoManufacturing();
			}
			else if ( msg.msgId == MSG_STARTUP_RESTART )
			{
				Restart();
			}
			else
			{
				LogDebug( "received %d message", msg.msgId );
			}
		}
	}
}


/*
 * Inform EngineComm that we are to receive the Engine messages.  We'll use the
 * GUI's message queue since the GUI hasn't been started yet anyway.
 */
 
void
SendMessageReceiverMessage()
{
	EngineCommReceiveMsg_t msg;

	if ( engineCommQueue != 0 )
	{
		// Create the message.
		msg.msgId = MSG_ENGINE_COMM_SET_MESSAGE_RECEIVER;
		strcpy( msg.data.messageReceiver.name, STARTUP_QUEUE_NAME );
	
		// Send the message.
		mq_send( engineCommQueue, (const char *)&msg,
			sizeof( int ) + sizeof( EngineCommMessageReceiverData_t ), MQ_PRIO_MAX-1 );
	}
}

void
GotoManufacturing()
{
	// Kill the processes.
	KillAll();	

	// Start the window manager.
	if ( killPwm )
		StartProcess( "pwm" );

	// Start up the manufacturing software.
	StartProcess( AMS_PROGRAM );
	
	exit( 0 );
}

void
Restart()
{
	// Kill all the other processes.
	KillAll();

	// Start up another instance of Startup_g.
	StartProcess( STARTUP_PROGRAM );
	
	exit( 0 );
}

void
KillAll()
{
	KillProcess( analysisPid );
	KillProcess( calcResultsPid );
	KillProcess( systemData->commPid );		// Either ASTM or SerialComm, depending on which was started.
	KillProcess( engineCommPid );
	KillProcess( errLoggerPid );
	KillProcess( guiPid );
	KillProcess( heaterControlPid );
	KillProcess( recallPid );
	KillProcess( settingsPid );
	
	// Delete the message queues.
	mq_unlink( ANALYSIS_QUEUE_NAME );
	mq_unlink( ASTM_QUEUE_NAME );
	mq_unlink( CALC_RESULTS_QUEUE_NAME );
	mq_unlink( ENGINE_COMM_QUEUE_NAME );
	mq_unlink( ERR_LOGGER_QUEUE_NAME );
	mq_unlink( GUI_QUEUE_NAME );
	mq_unlink( HEATER_CONTROL_QUEUE_NAME );
	mq_unlink( RECALL_QUEUE_NAME );
	mq_unlink( SERIAL_COMM_QUEUE_NAME );
	mq_unlink( SETTINGS_QUEUE_NAME );
	mq_unlink( STARTUP_QUEUE_NAME );

	// Delete the shared memory.
	shm_unlink( BARCODE_SHMEM_NAME );
	shm_unlink( CAL_DATA_SHMEM_NAME );
	shm_unlink( COUNTERS_SHMEM_NAME );
	shm_unlink( FACTORY_SHMEM_NAME );
	shm_unlink( GPIO_SHMEM_NAME );
	shm_unlink( ORDERS_SHMEM_NAME );
	shm_unlink( RAW_SHMEM_NAME );
	shm_unlink( RESULTS_SHMEM_NAME );
	shm_unlink( ROC_SHMEM_NAME );
	shm_unlink( SETTINGS_SHMEM_NAME );
	shm_unlink( SYSTEM_SHMEM_NAME );
}

void
KillProcess( int pid )
{
	if ( pid != 0 )
	{
		kill( pid, SIGKILL );
	}
}

void *
BezelButtonHandler( void * arg )
{
	GpioImage_t *	gpioImage = (GpioImage_t *)arg;
	GpioImage_t		localImage;
	struct sigevent event;
	int				id;
	uintptr_t		handle;
	uint8_t			gpioRead;
	struct timespec	pressTime;
	struct timespec	releaseTime;
	double			elapsedTime;
	bool			shutdownReached;
	
	// Enable I/O privilege
	if ( ThreadCtl( _NTO_TCTL_IO, 0 ) == -1 )
	{
		LogError( "ThreadCtl failed" );
		return 0;
	}
	
	handle = mmap_device_io( 128, GPIO_BASE_ADDR );
	if ( handle == MAP_DEVICE_FAILED )
	{
		LogError( "mmap_device_io gpio failed" );
		return 0;
	}

	// Fill in the event structure.
	memset( &event, 0, sizeof(event) );
	event.sigev_notify = SIGEV_INTR;

	// Attach to the interrupt.
	id = InterruptAttachEvent( BEZEL_BUTTON_INTNUM, &event, 0 );

	while (1)
	{
		// Wait for the button interrupt.
		InterruptWait( 0, 0 );
		shutdownReached = false;
		
		// Read the button.
		gpioRead = in8( GPIO_PORT_REG );

		// Button pressed.
		if ( (gpioRead & GPIO_PUSH_BTN) == 0 )
		{
			// Get the press time.
			if ( clock_gettime( CLOCK_REALTIME, &pressTime ) == -1 )
			{
				LogError( "clock_gettime failed" );
				return 0;
			}
			
			// Wait for the release.
			while ( (gpioRead & GPIO_PUSH_BTN) == 0 )
			{
				// Get the release time.
				clock_gettime( CLOCK_REALTIME, &releaseTime );
				
				// Calculate the elapsed time.
				elapsedTime = (double)(releaseTime.tv_sec - pressTime.tv_sec) +
							  (double)(releaseTime.tv_nsec - pressTime.tv_nsec) / (double)1000000000;

				if ( !shutdownReached && (elapsedTime > SHUTDOWN_TIME) )
				{
					shutdownReached = true;

					// Do the turn-off sound.
					SendSoundMessage();

					// Blank the screen and set the backlight to its lowest setting.
					BlankScreen( true );
					SetBacklight( BACKLIGHT_OFF );

					// Turn off the fan.
					SendFanMessage();

					// Save a local image of the gpio port, since KillAll unmaps the shared memory gpioImage is stored in.
					localImage = *gpioImage;

					// Make sure the main thread doesn't start any processes while we're killing all of them.
					pthread_sleepon_lock();
					noStartProcess = true;
					pthread_sleepon_unlock();

					// Kill all the Console processes. We're mainly interested in killing ASTM so that the OS can
					// close the socket connection after the process dies, but this may also clean up open files, etc.
					KillAll();
					sleep( 1 );

					// Wait for the button to be released. Otherwise, the instrument will restart immediately.
					while ( (in8( GPIO_PORT_REG ) & GPIO_PUSH_BTN) == 0 )
					{
						;
					}

					// Turn off the instrument.
					localImage &= ~GPIO_POWER_OFF;
					out8( GPIO_PORT_REG, localImage );
				}
				
				// Read the button again.
				gpioRead = in8( GPIO_PORT_REG );
			}
				
			// Send message to GUI
			if ( guiQueue != 0 )
			{
				int msgId = MSG_GUI_SCREENSAVE_BUTTON_PRESS;
				mq_send( guiQueue, (const char *)&msgId, sizeof( int ), MQ_PRIO_MAX-1 );
			}
		}

		// Unmask the interrupt when done.
		InterruptUnmask( BEZEL_BUTTON_INTNUM, id );
	}

	return 0;
}

void *
TouchScreenHandler( void * arg )
{
	struct sigevent	interruptEvent;
	struct sigevent	timerEvent;
	int				id;
	int				intReturn;
	struct timespec	start, next;
	bool			done = false;
	double			accum;
	uint64_t		timeout;
	char			argBuff[10];
	int				pid;

	// Enable I/O privilege
	if ( ThreadCtl( _NTO_TCTL_IO, 0 ) == -1 )
	{
		LogError( "ThreadCtl failed" );
		return 0;
	}

	// Fill in the interrupt event structure.
	memset( &interruptEvent, 0, sizeof( interruptEvent ) );
	interruptEvent.sigev_notify = SIGEV_INTR;

	// Attach to the interrupt.
	id = InterruptAttachEvent( TOUCH_SCREEN_INTNUM, &interruptEvent,
							   _NTO_INTR_FLAGS_END | _NTO_INTR_FLAGS_TRK_MSK );

	// Wait for the first touch and get its time.
	intReturn = InterruptWait( 0, 0 );
	InterruptUnmask( TOUCH_SCREEN_INTNUM, id );

	// Signal to the other threads that we're active and they need to wait.
	pthread_sleepon_lock();
	calibrationActive = true;
	pthread_sleepon_unlock();

	// Put up the message window.
	Message( MSTR_TOUCH_SCREEN_TO_CALIB );

	// Wait until after we've put up the message, which takes a fair amount of
	// time, to get the initial time.
	clock_gettime( CLOCK_REALTIME, &start );

	// Wait for subsequent touch interrupts.  Any that we get right away are
	// leftovers from the first touch (user could drag finger around and create
	// quite a few interrupts), and just reset the time for the first touch.
	while ( !done )
	{
		// Set a timer for the maximum wait between the first press and the
		// second one.
		memset( &timerEvent, 0, sizeof( timerEvent ) );
		timerEvent.sigev_notify = SIGEV_UNBLOCK;
		timeout = TOUCH_TIMEOUT * BILLION;
		TimerTimeout( CLOCK_REALTIME, _NTO_TIMEOUT_INTR, &timerEvent, &timeout, 0 );

		intReturn = InterruptWait( 0, 0 );
		InterruptUnmask( TOUCH_SCREEN_INTNUM, id );
		clock_gettime( CLOCK_REALTIME, &next );
		
		accum = ( next.tv_sec - start.tv_sec )  + (double)( next.tv_nsec - start.tv_nsec ) / (double)BILLION;
		
		if ( accum > 0.5 )
		{
			done = true;
		}
		else
		{
			start = next;
		}
	}

	InterruptDetach( id );	

	if ( intReturn == 0 )
	{
		sprintf( argBuff, "%d", settings->language );
		pid = spawnlp( P_WAIT, "Tcalib", "Tcalib",  "-l", argBuff, 0 );
		if ( pid == -1 )
		{
			LogError( "failed to start Tcalib" );
		}
	}

	Message( STR_NONE );

	pthread_sleepon_lock();
	calibrationActive = false;
	pthread_sleepon_signal( &calibrationActive );
	pthread_sleepon_unlock();

	return 0;
}

void *
TimerHandler( void * arg )
{
	int				elapsedTime;
	int				progBarValue;
	int				progBarIncrement = 100 / MAX_STARTUP_TIME;
	StartupMsg_t	msg;

	PtEnter( 0 );
//	PtSetResource( ABW_ptTextProgBar, Pt_ARG_TEXT_STRING, (*strings)[ BSTR_PERFORMING_IQC ], 0 );
	SetString( ABW_ptTextProgBar, (*strings)[ BSTR_PERFORMING_IQC ], true );
	PtLeave( 0 );
	
	for ( elapsedTime = 0, progBarValue = 0; elapsedTime < MAX_STARTUP_TIME; elapsedTime++, progBarValue += progBarIncrement )
	{
		PtEnter( 0 );
		PtSetResource( ABW_ptProgBar, Pt_ARG_GAUGE_VALUE, progBarValue, 0 );
		PtLeave( 0 );
		sleep( 1 );
	}

	/*
	 * If everything starts up properly, this thread will be canceled and we'll
	 * never get to this point.
	 */

	// Inform the main thread that we timed out.
	msg.msgId = MSG_STARTUP_ERROR;
	mq_send( startupQueue, (const char *)&msg, sizeof( int ), MQ_PRIO_MAX-1 );

	return 0;
}

// Callback for the Restart button on the Critical Stop window.
int
CriticalStopButtonPress( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	// Kill all the other processes.
	KillAll();
	
	// Start up another instance of Startup_g.
	StartProcess( STARTUP_PROGRAM );
	
	exit( 0 );
}

void
ErrorStop( SystemErrorNumber_t errNum )
{
	PtWidget_t *	window;
	PhPoint_t		pos = {240, 0};
	char			buff[500];
	
	// Make the error number available for the info window.
	systemErrNum = errNum;

	// Cancel the touch thread if it's running (if not, this will just return
	// an error which we can ignore).
	pthread_cancel( touchThread );

	PtEnter( 0 );

	// Get a pointer to the error window.
	window = ApCreateModule( ABM_winCriticalStop, 0, 0 );

	// Add the error number to the error message string.
	sprintf( buff, "%x\n%s", errNum, (*strings)[ systemError->GetScreenText( errNum ) ] );

	// Set the title, message, and button strings in the window.
	SetString( ABW_ptLabelCriticalStopTopLabel, (*strings)[ TSTR_CRITICAL_STOP ], false );
	SetString( ABW_ptLabelCriticalStopMessage, buff, true );
	SetString( ABW_ptButtonCriticalStop, (*strings)[ BSTR_RESTART ], false );

	// Add the callback for the Restart button.
	PtAddCallback( ABW_ptButtonCriticalStop, Pt_CB_ACTIVATE, CriticalStopButtonPress, 0 );

	// Move the error window to make it visible.
	PtSetResource( window, Pt_ARG_POS, &pos, 0 );

	PtLeave( 0 );

	// Wait until reset or power-off.
	while ( 1 )
	{
		sleep( 100 );
	}
}

int
ErrorInfo( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	SetString( ABW_ptLabelErrorInfo, (*strings)[ TSTR_INFORMATION ], false );
	SetString( ABW_ptMultiTextErrorInfo, (*strings)[ systemError->GetInfoText( systemErrNum ) ], false );
	SetString( ABW_ptButtonErrorInfo, (*strings)[ BSTR_CONTINUE ], true );

	return Pt_CONTINUE;
}

void
BlankScreen( bool blank )
{
	static PtWidget_t *	window = 0;
	const PhPoint_t		activePos = {240, 0};
	const PhPoint_t		storePos = {-400, 0};
	PhPoint_t			pos = blank ? activePos : storePos;
	
	PtEnter( 0 );

	// Get a pointer to the window, if we don't already have it.
	if ( window == 0 )
	{
		window = ApCreateModule( ABM_winBlack, 0, 0 );
	}

	PtSetResource( window, Pt_ARG_POS, &pos, 0 );

	PtFlush();
	PtLeave( 0 );
}

void
Message( StringId_t stringId )
{
	static PtWidget_t *	window = 0;
	const PhPoint_t		activePos = {240, 0};
	const PhPoint_t		storePos = {-400, 0};
	PhPoint_t			pos;
	
 	if ( stringId == STR_NONE )
 	{
 		pos = storePos;
 	}
 	else
 	{
 		pos = activePos;
 	}

	PtEnter( 0 );

	// Get a pointer to the window, if we don't already have it.
	if ( window == 0 )
	{
		window = ApCreateModule( ABM_winMessage, 0, 0 );
	}

	PtSetResource( window, Pt_ARG_POS, &pos, 0 );
	if ( stringId != STR_NONE )
	{
		SetString( ABW_ptLabelMessage, (*strings)[ stringId ], false );
	}

	PtLeave( 0 );
}

// GLD
// This is common to GUI, too.  Maybe put in Common?
void
SetString( PtWidget_t * widget, const char * str, bool doSpace )
{
	char			buff[ 1000 ];
	const char *	p;
	char *			utf;

// GLD
// This doesn't test for buffer overrun!!!

	if ( doSpace )
	{
		strcpy( buff, " " );
		
		while ( (p = strchr( str, '\n' )) != 0 )
		{
			strncat( buff, str, p - str );
			strcat( buff, " \n " );
			str = ++p;
		}
		strcat( buff, str );
		strcat( buff, " " );
		p = buff;
	}
	else
	{
		p = str;
	}

	// Do the ISO-8859-1 to UTF-8 translation.  If it succeeds, we use the
	// return value.  Else, we use the untranslated string.
	utf = TranslateToUtf( p );
	if ( utf != 0 )
	{
		p = utf;
	}

	// Set the string in the widget.
	PtSetResource( widget, Pt_ARG_TEXT_STRING, p, 0 );

	if ( utf != 0 )
	{
		free( utf );
	}
}

void
LoadRefRanges( RefRangeData_t * refRanges )
{
	CfgError_t	cfgErr;
	int			i;
	
	// Set the ref ranges to the defaults, but clear all the isDefined flags. This way, if there's a new analyte it'll
	// get the default value for those species/demographics that are defined in the XML file.
	SetRefRangesToDefaults( refRanges, factoryData->abaxisProductCode );
	for ( i = 0; i < STYPE_NUM_SPECIES; i++ )
	{
		refRanges->isDefined[ i ] = false;
	}

	// Look for a ref ranges file on an attached USB drive. If found, load it and also
	// write it to the local disk. If the call returns TRUE, then we've got the new
	// ref ranges in our data struct and there's no need to do another read from disk.
	if ( !CopyRefRangesFile( refRanges ) )
	{
		// Either there is no file on a USB drive, the file is bad, or there was an error
		// copying it to the local disk. So, reset the ref ranges to the defaults and
		// then read the local file.
		SetRefRangesToDefaults( refRanges, factoryData->abaxisProductCode );
		for ( i = 0; i < STYPE_NUM_SPECIES; i++ )
		{
			refRanges->isDefined[ i ] = false;
		}
	
		// Parse the file.
		cfgErr = ParseRefRanges( REF_RANGES_CURRENT_FILE, refRanges );
		if ( cfgErr != CFG_ERR_NONE )
		{
			// If there's a file open error, set the ref ranges to the defaults.
			if ( cfgErr == CFG_ERR_FILE_OPEN )
			{
				SetRefRangesToDefaults( refRanges, factoryData->abaxisProductCode );
			}
	
			// Any other error means a stop.
			else
			{
				LogError( "error parsing current reference ranges file: %d", cfgErr );
				ErrorStop( SERR_FILE_READ );
			}
		}
	}
}

bool
CopyRefRangesFile( RefRangeData_t * refRanges )
{
	CfgError_t			cfgErr;

	if ( MountThumbDrive() )
	{
		// Read and parse the ref ranges file.
		cfgErr = ParseRefRanges( IMPORT_REF_RANGES_FILE, refRanges );

		// File not successfully read.
		if ( cfgErr != CFG_ERR_NONE )
		{
			switch ( cfgErr )
			{
				case CFG_ERR_CRC:
					LogError( "CRC error importing USB drive ref ranges file" );
					break;
				case CFG_ERR_FILE_OPEN:
					break;	// File not found, so just return.
				default:
					LogError( "Parse error importing USB drive ref ranges file" );
					break;
			}
			return false;
		}
			
		// No error parsing the ref ranges file, so write it to the local disk.
		cfgErr = WriteRefRanges( REF_RANGES_CURRENT_FILE, refRanges );
		if ( cfgErr != CFG_ERR_NONE )
		{
			LogError( "Error writing USB drive ref ranges file to local disk" );
			return false;
		}
		else
		{
			// Ref ranges on thumb drive found and successfully read in and written
			// to the local disk.
			return true;
		}
	}
	else
	{
		LogError( "mount of USB drive failed" );
		return false;
	}
	
	return false;
}

void
LoadUnits( UnitsConfig_t * unitsConfig )
{
	CfgError_t	cfgErr;
	Units		units;
	
	// Start with the units set to defaults.  Don't write the file, though!
	units.SetToDefaults( false );
	
	// Parse the file.
	cfgErr = ParseUnits( unitsConfig );
	
	// If there's an error other than file not found, stop.
	if ( cfgErr != CFG_ERR_NONE && cfgErr != CFG_ERR_FILE_OPEN )
	{
		ErrorStop( SERR_FILE_READ );
	}
}

void
GetVersionData()
{
	FILE *	fp;
	char *	p;
	
	memset( versionData, 0, sizeof( VersionData_t ) );
	
	fp = fopen( VERSION_CONFIG_FILE_NAME, "r" );
	if ( fp != 0 )
	{
		// Read the version string.
		fgets( versionData->version, sizeof( char ) * MAX_VERSION_LEN, fp );
		fclose( fp );
		
		// Trim off trailing spaces, tabs, newlines, etc.
		p = versionData->version + strlen( versionData->version ) - 1;
		while( isspace( *p ) )
		{
			*p = '\0';
			p--;
		}
		
		// Get the release type.
		versionData->releaseType = (char)fgetc( fp );
		if ( !isalpha( versionData->releaseType ) )
			versionData->releaseType = 'R';
	}
}

char
GetNvramProductCode()
{
    uintptr_t	handle;
    unsigned	i;
    char		buff[10];

	if ( ThreadCtl( _NTO_TCTL_IO, 0 ) == -1 )
	{
		LogError( "failed to get memory control privilege" );
		return VETSCAN_PRODUCT_CODE;
	}
	
	handle = mmap_device_io( 16, 0x0240 );
	if ( handle == MAP_DEVICE_FAILED )
	{
		LogError( "failed to map I/O" );
		return VETSCAN_PRODUCT_CODE;
	}
	
	for ( i = 0; i < 10; i++ )
	{
		out8( handle + 0x10, i );			// Set address
		buff[ i ] = in8( handle + 0x13 );	// Get value
	}
	
	if ( strncmp( "PICCOLO", buff, 7 ) == 0 )
	{
		return PICCOLO_PRODUCT_CODE;
	}

	return VETSCAN_PRODUCT_CODE;
}

void
SetKeyboard( Language_t language )
{
	int					pid;
	static const char *	kbd;

	switch ( language )
	{
		default:
		case 0:	kbd = "en_US_101.kbd";	break;
		case 1:	kbd = "fr_FR_102.kbd";	break;
		case 2:	kbd = "de_DE_102.kbd";	break;
		case 3:	kbd = "it_IT_102.kbd";	break;
		case 4:	kbd = "es_ES_102.kbd";	break;
	}

	// Slay the driver.
	pid = spawnlp( P_WAIT, "slay", "slay", "-f", "-Q", "devi-hid", 0 );
	if ( pid == -1 )
	{
		LogError( "failed to run 'slay devi-hid'" );
	}
	else
	{	
		// Restart the driver.
		pid = spawnlp( P_WAIT, "devi-hid", "devi-hid", "mouse", "kbd", "keyboard", "-k", kbd, 0 );
		if ( pid == -1 )
		{
			LogError( "failed to start devi-hid" );
		}
	}
}

void
SlayWindowManager()
{
	int	pid;

	pid = spawnlp( P_WAIT, "slay", "slay", "-f", "-Q", "pwm",  0 );
	if ( pid == -1 )
	{
		LogError( "failed to run 'slay pwm'" );
	}
}

void *
CreateResultsDatabase()
{
	int	lock;

	// Lock the database.
	lock = open( RESULTS_LOCK, O_WRONLY | O_CREAT );
	if ( lock == -1 )
	{
		LogError( "unable to lock database" );
		return 0;
	}
	flock( lock, LOCK_EX );

	CreateResultsDatabaseFiles( CUSTOMER_DB_INDEX, CUSTOMER_DB_DATA );
	CreateResultsDatabaseFiles( OTHER_DB_INDEX, OTHER_DB_DATA );

	// Unlock the database
	flock( lock, LOCK_UN );

	return 0;
}

void
CreateResultsDatabaseFiles( const char * indexFileName, const char * dataFileName )
{
	int							i;
	FILE *						indexFile;
	FILE *						dataFile;
	// These are static because they're large and making them static avoids having
	// to increase the size of the thread's stack.
	static 	ResultsDbIndex_t	resultsIndex;
	static 	ResultsDbEntry_t	resultsDbEntry;

	// Determine whether the index file exists. 
	indexFile = fopen( indexFileName, "r+" );
	if ( indexFile == 0 )
	{
		// Index file doesn't exist, so create new database files.
		if ( errno == ENOENT )
		{
			LogInfo( "creating results database (%s)", dataFileName );

			// Create the index file.
			indexFile = fopen( indexFileName, "w" );
			if ( indexFile == 0 )
			{
				LogError( "error creating results index file %s", indexFileName );
				return;
			}
			memset( &resultsIndex, 0xFF, sizeof(ResultsDbIndex_t ) );
			resultsIndex.dbVersion = RESULTS_DB_VERSION;
			resultsIndex.lastEntry = RESULTS_MAX - 1;
			fwrite( &resultsIndex, sizeof( ResultsDbIndex_t ), 1, indexFile );
			fclose( indexFile );
		
			// Create the data file.
			dataFile = fopen( dataFileName, "w" );
			if ( dataFile == 0 )
			{
				LogError( "error creating results database file %s", dataFileName );
				return;
			}
		
			memset( &resultsDbEntry, 0, sizeof( ResultsDbEntry_t) );
		
			for ( i = 0; i < RESULTS_MAX; i++ )
			{
				fwrite( &resultsDbEntry, sizeof( ResultsDbEntry_t ), 1, dataFile );
			}
		
			fclose( dataFile );
		}
		else
		{
			LogError( "error opening results index" );
				return;
		}
	}
	
	else
	{
		fclose( indexFile );
	}
}

void
InitOrdersList()
{
	memset( ordersList, 0, sizeof( OrdersList_t ) );
	memset( ordersList->index, -1, sizeof( ordersList->index ) );
	
	if ( settings->comm.interface == COMM_ASTM )
	{
		ReadOrdersFile( ordersList );
	}
}