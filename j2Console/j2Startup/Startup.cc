#include "StartupGlobal.h"
#include <notify.h>
#include "Pm.h"

//Local functions
static void *	            BackgroundHandler( void * arg );
static char		            GetNvramProductCode();
static void		            GetVersionData(char bspVersion);
static void		            LoadRefRanges( RefRangeData_t * refRanges );
static void		            LoadUnits( UnitsConfig_t * unitsConfig );
static void		            SetKeyboard( Language_t language );
static void		            SlayWindowManager();
static void *	            TimerHandler( void * arg );
static void		            WaitForGUI();
static void		            WaitForEngineInit();
static void		            WaitForMessages();
static void                 InitOrdersList();
static bool                 CopyRefRangesFile (RefRangeData_t * refRanges);


//Local  Flags
static bool					createResultsDatabase = false;
static bool					cursorEnabled = false;
static bool					inDemoMode = false;
static bool					inResearchMode = false;			// for clinical trials only
static bool					inManufacturingMode = false;
static bool					killPwm = true;
static bool					noStartProcess = false;


//Misc. global variables
SystemError *		        systemError;
AStrings    *               strings;

//Misc local variable
static char                 engineVersion [MAX_ENG_SOFTWARE_VERSION_LEN + 1];

// reboot process
//#define SHUTDOWN_PROCESS              L"/Hard Disk/Abaxis/bin/myShutdown.exe"
#define SHUTDOWN_PROCESS                L"shutdown.exe"
#define NOTIFICATION_PROCESS            L"/Hard disk/Abaxis/Util/Notification.exe"

//Debug flag
#undef                      FORCE_DEMO_MODE                     //force to be demo mode software 
#undef                      FORCE_RESEARCH_MODE                 //force to be research mode software
#undef                      REBOOT_TEST
#undef                      FORCE_TELNET_ON
#undef                      FORCE_CURSOR_ENABLE 
#undef                      CHECKSUM_SET_ERROR_TEST             //Engine-console communication checksum error detection test
#undef                      FORCE_WATCHDOG_OFF                  //disable watchdog reset  
#undef                      FORCE_VETSCAN_MODE                  //for fan setting debugging only
#undef                      CPU_TEMP_TEST                       //cpu board temperature test
#undef                      FORCE_MOUSE_ON                      //always turn mouse on
#undef                      FORCE_ENG_DEBUG_PORT_ON             //force to enable Engine debug port 
#undef                      FTDI_NOT_INITALIZATION              //do not initial ftdi chip for debug
#define                     CLEAN_WINCE_DATABASE                //enable wince default.vol database clean up

bool                        guiInitFlag = 0;

static char *noChineseBspVersion = "3.1.2.9";


// initial Gui process with mouse off
void
InitialGui (void)
{
    HANDLE                  timerThread;

    CreateProcess (GUI_PROGRAM);

    //create timer thread
    timerThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)TimerHandler, NULL, 0, NULL);

    //wait for gui init
    WaitForGUI ();
    //pthread_cancel (timerThread);
    TerminateThread (timerThread, 0);
    CloseHandle (timerThread);
    guiInitFlag = 1;
}


// main function 
int
main(int argc, char *argv[])
{
	int i=0;


	// Get the command line options.
	while ( i < argc )
	{
		if (argv[i] == "-c")
		{
			// Enable mouse cursor.
			cursorEnabled = true;
		}
		else if (argv[i] == "-d")
		{
			// Demo mode.
			inDemoMode = true;
		}
		else if (argv[i] == "-m")
		{
			// Manufacturing mode.
			inManufacturingMode = true;
		}
		else if (argv[i] == "-p")
		{
			// Don't kill window manager.
			killPwm = false;
		}
		else if (argv[i] == "-r")
		{
			// Enable research mode.
			inResearchMode = true;
		}
		else if (argv[i] == "-x")
		{
			// Create results database.
			createResultsDatabase = true;
		}

		i++;
	}


	BackgroundHandler(0);

	return EXIT_SUCCESS;
}

// analyzer initialization 
void *
BackgroundHandler (void * arg)
{
	/*
	 * We create all the shared memory objects and message queues here, whether
	 * we need them in this process or not.  In this way, we know they are there
	 * and ready to go when the other processes start and connect to them.  This
	 * avoids synchonization issues and ensures that the shared memory and
	 * message queues are in a known state.
	 */

	CfgError_t				parseError;
	HANDLE					timerThread;
	SystemErrorNumber_t		errNum;
	GuiReceiveMsg_t			msg;
	wchar_t					wBuff [512];
    int                     ret;
    PROCESS_INFORMATION     proce_info;
	DWORD enable;
	HKEY hKey = NULL;

	// Create the message queues.
    CreateMessageQueues ();

    // Create Share memories
    CreateShareMemories ();


    //get last time power off log file size
    SetLogFileSize ();

	// Start the error logger so it can log any errors properly.
	CreateProcess (ERRLOGGER_PROGRAM);

	// Set the system data. this parse must be called before GetDataBaseFileSize () 
	ParseSystem (systemData);

	// Parse the settings config file.  If there is a file open error, this
	// routine will set the settings to the defaults.  We need this early on to get the language.
    // parse settings before InitGui to make sure Gui get sounds settings
	parseError = ParseSettings (settings);
	if (parseError != CFG_ERR_NONE && parseError != CFG_ERR_FILE_OPEN) {
		LogError ("error parsing settings config file: %d", parseError);
		ErrorStop (SERR_FILE_READ,systemData);
	}


	// We can't create the strings object until here because it depends on Factory shared memory.
	strings = new AStrings();

	// Parse the factory config file.  We need this early on to get the product code.
	parseError = ParseFactory (factoryData);
	if (parseError != CFG_ERR_NONE) {
		LogError ("error parsing factory config file: %d", parseError);
		ErrorStop (SERR_FILE_READ, systemData);
	}


    //get windows ce database file size
    ret = GetDataBaseFileSize (NULL, true, systemData->maxDataBaseSize);
#ifdef CLEAN_WINCE_DATABASE
    // if database (default.vol) size > 100000, clean database file and warm reboot
    if (ret == 1) {
        CreateProcess (NOTIFICATION_PROCESS, L"", NULL, NULL,FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proce_info);
        // it nees wait here to prevent DisableEventNotifications() function call before finish NOTIFICATION_PROCESS
       // Sleep (2000);
        WaitForSingleObject(proce_info.hProcess, INFINITE);
        CloseHandle (proce_info.hThread);
        CloseHandle (proce_info.hProcess);
    }
#endif
    // disable all driver event to prevent database size increased 
    LogDebug ("Disable Event Notifications");
    DisableEventNotifications ();

    //Start Gui process
    InitialGui ();


	// Parse the expirys config file if this is a piccolo.
	if (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE)
	{
		parseError = ParseExpirys (expiryData);
		if (parseError != CFG_ERR_NONE && parseError != CFG_ERR_FILE_OPEN) {
			LogError ("error parsing expiry config file: %d", parseError);
			ErrorStop (SERR_FILE_READ, systemData);
		}
	}

	// Parse the hardware config file. This must be done before we start the Engine.
    // it also must be called after parse settings  to get hl7ConfigIdx
	parseError = ParseHardware (hardwareData);
	if (parseError != CFG_ERR_NONE && parseError != CFG_ERR_FILE_OPEN) {
		LogError ("error parsing hardware config file: %d", parseError);
		ErrorStop (SERR_FILE_READ, systemData);
	}
    
    // Parse HL7 configuratin file list before parse HL7 configuration file
    parseError = ParseHL7ConfigurationFileName (hl7FileList);
    if (parseError != CFG_ERR_NONE ) {
		LogError ("error parsing HL7 config name list file: %d", parseError);
		ErrorStop (SERR_FILE_READ, systemData);
    }
    
    // Parse HL7 config file, it must be done before HL7 process started
    //LogDebug ("Get hl7 xml file index =%d", settings->hl7ConfigIdx);
    parseError = ParseHL7ConfigurationByIndex (hl7Config, settings->hl7ConfigIdx);
	if (parseError != CFG_ERR_NONE && parseError != CFG_ERR_FILE_OPEN) {
		LogError ("error parsing hl7 config file: %d", parseError);
		ErrorStop (SERR_FILE_READ, systemData);
	}

#ifdef FORCE_DEMO_MODE
    inDemoMode = TRUE;
#endif
#ifdef FORCE_RESEARCH_MODE
    inResearchMode = TRUE;
#endif
	systemData->inDemoMode = inDemoMode;
	systemData->inResearchMode = systemData->inResearchMode || inResearchMode;	// True if it's set in either system.xml or with a command line switch.
	systemData->inAdministratorMode = false;
	systemData->rotorPresent = false;	// Set this before starting the engine and waiting for its status.
    systemData->inRandDMode = false;    // Set it false before GetThumb Key
    systemData->systemStatus = SYSTEM_STATUS_BUSY;
#ifdef FORCE_TELNET_ON 
	systemData->enableFtpTelnet = true;		// True for clinical trials
#endif



    // Detect CPU board here
    ActiveDefaultIpAddress (settings, systemData);

	// Get the analzyer software and hardware version data.
	GetVersionData (0);

	// Set the backlight level.
    LcdBackLightSet (systemData->cpuBoard, settings->backlightLevel);

    // Display iQC gui
	msg.msgId = MSG_GUI_STARTUP_SPLASH;
	wcscpy (msg.data.winSplashScreen.productCode, (*strings)[STR_PRODUCT_NAME]);
	mbstowcs (wBuff, versionData->version, 512);
	wcscpy (msg.data.winSplashScreen.version,  wBuff);
	wcscpy (msg.data.winSplashScreen.progBarText, (*strings)[BSTR_PERFORMING_IQC]);
	SendGuiMessage (&msg, sizeof(msg));


    // delay time for usb thumb detection
    if (systemData->cpuBoard == ADLINK_BT_CPU_BOARD) { 
        Sleep (25000);
    } else {
        Sleep (10000);
    }

    // parse thumb key 
	GetThumbKeyMode (systemData);

	// Create the SystemError object now.  We can't create it until after the shared
	// memory is first created because it relies on the System shmem.
	systemError = new SystemError ();

    // Start error process 
    CreateProcess (ERROR_PROCESS_PROGRAM);

    /*
     * check product code
     */
    LogInfo  ("Product Code = %c", GetNvramProductCode());
    LogInfo  ("Factory Code =%c",  factoryData->abaxisProductCode);

#ifdef FORCE_VETSCAN_MODE
    factoryData->abaxisProductCode = VETSCAN_PRODUCT_CODE;
#else
    if (GetNvramProductCode() != factoryData->abaxisProductCode) {
        LogError ("product code mismatch");
        ErrorStop (SERR_PRODUCT_CODE, systemData);
    }
#endif

	// Parse the counters config file.
	parseError = ParseCounters (counterData);
	if (parseError != CFG_ERR_NONE && parseError != CFG_ERR_CRC) {
		LogError ("error parsing counters config file: %d", parseError);
	}

	// Load the Reference Ranges.
	LoadRefRanges (refRanges);

	// Load the Units configuration.
	LoadUnits (unitsConfig);

	// Calculate the BFD calibration factors.
	errNum = CalcBfds (factoryData);
	if (errNum != SERR_NONE) {
		ErrorStop(errNum, systemData);
	}

	// If we're set to ASTM or HL7, we need to clear the Research mode flag.
	if ((settings->comm.commInterface == COMM_ASTM) || (settings->comm.commInterface == COMM_HL7)) {
		systemData->inResearchMode = false;
	}

    // Initialize the ASTM orders list.
    InitOrdersList ();


    // active deault ip address in the case of new system
    // ZZZZ
    //ActiveDefaultIpAddress (settings, systemData);

    // telnet/ftp setting (enable/disable)
    // seperate telnet enable flag
    // in R&D mode, telnet enable depends on rdEnableFtpTelnet flag
    // in ohter mode, telnet enable depends on enableFtpTelnet flag
    if (systemData->inRandDMode) {
        if (systemData->rdEnableFtpTelnet || systemData->enableFtpTelnet) {
            enable = 1;
        } else {
            enable = 0;
        }
    } else {
        if (systemData->enableFtpTelnet) {
            enable = 1;
        } else {
            enable = 0;
        }
    }
    // enable/disable telnet/ftp	
    SetTelentFtp (enable);

	// Get the analzyer software and hardware version data.
    // BSP version must called after ethernet port is detected 
	GetVersionData (1);
    // put current console software version number into log file
    LogInfo ("Console Software Version = %s", versionData->version);
    // put current BSP  version number into log file
    LogInfo ("Console BSP Version = %s", versionData->bspVersion);
    //ZZZZ
    if (strcmp (versionData->bspVersion, noChineseBspVersion) == 0) {
        settings->enableChinese = false;
        LogInfo ("Chinese display disable");
    } else {
        settings->enableChinese = true;
        LogInfo ("Chinese display enable");
    }


#ifndef FTDI_NOT_INITALIZATION  
	// Initialize the serial port(s) and the Engine.
    // ZZZ disable FTDI chip initilization for debug purpose
	if (InitialEngineBoardInterface() == -1) {
        LogError ("Fail to initial Engine board interface");
    }
#endif
	// Start EngineComm and the GUI, and wait to hear from them that we're up and running.
	CreateProcess (ENGINECOMM_PROGRAM);
	
	// Register with EngineComm for messages.
	SendMessageReceiverMessage();

    LogDebug ("Wait for Engine msg"); 


	// Wait for the Engine and GUI to let us know they're both up and ready.
	timerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TimerHandler, NULL, 0, NULL);
	WaitForEngineInit();
	TerminateThread(timerThread, 0);
	CloseHandle(timerThread);

#ifdef FORCE_ENG_DEBUG_PORT_ON
    //force to turn on Engine board debug port
    SendTestRequestMessage ('D');
#else
    // in the case of R&D key and Engine debug port configuration enable, 
    // send engine port to enable debug port
    if ((systemData->inRandDMode) && (systemData->engDebugPortEanble)) { 
        SendTestRequestMessage ('D');
    }
#endif


    // check expire date
    // in the case of expired, this fuction shall never return 
    ExpireDateCheck ();

	// Start the other programs.hj
	CreateProcess (CALCRESULTS_PROGRAM);
	CreateProcess (HEATERCONTROL_PROGRAM);
    // in the case of aaeon board, slow down bootup time   
    // to let adlink and aaeon cpu board boot up at same speed
    if (systemData->cpuBoard == AAEON_CPU_BOARD) {
        Sleep (2500);
    }
	CreateProcess (ANALYSIS_PROGRAM);
	CreateProcess (RECALL_PROGRAM);
	CreateProcess (SETTINGS_PROGRAM);
    CreateProcess (RANDD_PROGRAM);


	if (settings->comm.commInterface == COMM_ASTM) {
		systemData->commPid = CreateProcess (ASTM_PROGRAM);
    } else if (settings->comm.commInterface == COMM_HL7) {
		systemData->commPid = CreateProcess (HL7_PROGRAM);
	} else {
		systemData->commPid = CreateProcess (SERIALCOMM_PROGRAM);
	}

	// Send the factory config info to the Engine.
	SendFactoryMessage (factoryData);

	// Configure the keyboard.
    SetKeyboard (settings->keyboard);

#ifdef FORCE_MOUSE_ON
        //enable mouse
        cursorEnabled = true;
#endif
    if (cursorEnabled || systemData->inManufacturingMode || systemData->inRandDMode) {
        msg.msgId = MSG_GUI_ENABLE_MOUSE;
        SendGuiMessage (&msg, sizeof(msg));
    } else {
        //ZZZ, current GUI does not support this command
        // disable it for future to use
        // msg.msgId = MSG_GUI_DISABLE_MOUSE;
        // SendGuiMessage (&msg, sizeof(msg));
    }

#ifdef FAN_IDLE_TEST
    // create speical for lower speed in all mode to reduce 
    // developer heard noise
    SendFanMessage ('F', settings->fanIdleSpeed);
#else
    if (IN_CUSTOMER_MODE) {
        // Setup idle FanSpeed only on normal customer mode
        SendFanMessage ('F', settings->fanIdleSpeed);
    } else {
        // special mode, always turn idle fan on high speed 
        SendFanMessage ('F', IDLE_FAN_HIGH_SPEED);
    }
#endif

    // create device notification thread for debug purpose
    // disable device notification debug
    //CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)DeviceNotificationHandler, NULL, 0, NULL);
    // finally, create network detection thread for cable plug in detection
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)NetworkDetectionThread, NULL, 0, NULL);

    // finally, initial printer program to avoid other usb initial conflict
    Sleep (1000);
    CreateProcess  (PRINTER_PROGRAM);


    //ZZZ added event detection, debug only
    OutputNotifications (); 

    //main loop for prcess start up queue 
	WaitForMessages ();

	return 0;
}


// wait for GUI to finish initilization
void
WaitForGUI()
{
	DWORD				receivedSize = 0;
	EngineCommSendMsg_t	msg;
	DWORD				flags;
	SystemErrorNumber_t	errNum = SERR_NONE;

	while (1) {
		ReadMsgQueue(startupQueue, &msg, sizeof(msg), &receivedSize, INFINITE, &flags);

		if (receivedSize > 0) {
			switch (msg.msgId) {
				
				case MSG_STARTUP_ERROR:
					LogError( "GUI did not respond in %d seconds", MAX_STARTUP_TIME );
					errNum = SERR_PROCESS_INIT_TIMEOUT;
					ErrorStop(errNum, systemData);
					break;

				case MSG_STARTUP_GUI_READY:
					return;
					break;

				default:
					break;
			}
		}
	}
}

// Wait for Engine board initialization to be finished 
void
WaitForEngineInit()
{
	DWORD				receivedSize = 0;
	EngineCommSendMsg_t	msg;
	unsigned char		status;
	DWORD				flags;
	bool				engineReady = false;
    bool                engineVer   = false;
    bool                dacFlag     = false;
	SystemErrorNumber_t	errNum = SERR_NONE;
    int                 count;

	while (1) {
		//receivedSize = mq_receive (startupQueue, (char *)&msg, mqstat.mq_msgsize, NULL);
		ReadMsgQueue(startupQueue, &msg, sizeof(msg), &receivedSize, INFINITE, &flags);

		if (receivedSize > 0) {
			switch (msg.msgId) {
				case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
					if (msg.data.message[0] == 'D') {
					    // Drawer message.
						status = msg.data.message[8];
						if (status & 0x80) {
                            // Drawer jammed
							LogError( "drawer jammed" );
						} else if (status & 0x10) {
                             // Drawer closed
							engineReady = true;
							// Set the rotor present flag.
							systemData->rotorPresent = ((status & 0x40) != 0);
						} else	{
                            // Drawer open
							LogDebug( "drawer open" );
						}
					} else if (msg.data.message[0] == 'V') {
                        LogInfo ("Engine Software  version=%s", &msg.data.message [4]);
                        LogInfo ("FPGA version =%s", (char *)&msg.data.message [20]);
                        engineVer = true;
                        strcpy ((char *)&versionData->engSoftwareVersion [0], (char *)&msg.data.message [4]);
                        strcpy ((char *)&versionData->engFpgaVersion [0], (char *)&msg.data.message [20]);
                        systemData->engSoftwareUpdate = false;
                        // check engine version, in the case of engine code out of sync, goto update software windows
                        if (strcmp (engineVersion, &versionData->engSoftwareVersion [0]) != 0) { 
                            LogError("Startup:: Error:%x. Engine software out sync", SERR_ENGINE_OUT_SYNC); 
                            SoftwareErrorState (SERR_ENGINE_OUT_SYNC);
                        }
                    } else if ( msg.data.message[0] == 'E') {
						errNum = (SystemErrorNumber_t)*(unsigned short *)&msg.data.message[8];
						LogError( "Engine error %04x", errNum );
					} else if ( msg.data.message[0] =='?') {
                        count = *(unsigned short *)&msg.data.message[2];
                        LogInfo( "Engine ? msg: %*.*s", count, count, &msg.data.message[4] );
                    } else if (msg.data.message[0] == 'Z') {
                        dacFlag = true;
                        // set systeData structure to know which DAC is used in Engine board
                        systemData->dac10BitsFlag = msg.data.message[6];
                        LogDebug ("10 Bit DAC Flag = %d", msg.data.message[6]);
                    }
					break;

				case MSG_STARTUP_ERROR:
					if (!engineReady || !engineVer) {
						LogError( "Engine did not respond in %d seconds", MAX_STARTUP_TIME );
						errNum = SERR_ENGINE_INIT_TIMEOUT;
#ifdef CHECKSUM_SET_ERROR_TEST
                        SoftwareErrorState (errNum);
#endif
					}
					break;

				default:
					break;
			}
		}

		// Error, so go to error-stop mode.
		if (errNum != SERR_NONE) {
			// Put up the error screen and stop.
			ErrorStop (errNum, systemData);
		}

		if (engineReady && engineVer && dacFlag) {
            Sleep (5000);
			return;
		}
	}
}

#ifdef CPU_TEMP_TEST
void *
CpuTemperatureTest (void *arg)
{
    while (1)  {
        if (systemData->cpuBoard  == 'A') {
            LogDebug ("remote Temp =%d local Temp=%d", GetCpuTemperature (systemData->cpuBoard, 1), GetCpuTemperature (systemData->cpuBoard, 0));
        } else {
            LogDebug ("Cpu Temp=%d", GetCpuTemperature (systemData->cpuBoard, 1));
        }
        // every 15 seconds, pull one time
        Sleep (15000);
    }
    
}
#endif


#ifdef REBOOT_TEST

void
RebootSystem (void)
{
    PROCESS_INFORMATION     proce_info;
    CreateProcess (SHUTDOWN_PROCESS, L"",       NULL, NULL,FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proce_info);

    CloseHandle (proce_info.hThread);
    CloseHandle (proce_info.hProcess);
}


// system keep reboot. When R&D key or AMS key plug in, reboot stopped
void
WaitForMessages()
{
     int i;

    //reboot only without AMS key or R&D key
    if ((systemData->inManufacturingMode == false) && (systemData->inRandDMode == false)) {
        for (i = 0; i < 30; i++) {
            Sleep (1000);
        }
        RebootSystem ();
    } else {
        while (1) {
            Sleep (60000);
        }
    }

}

#else

// wait for message queue
void
WaitForMessages()
{
	DWORD			            receivedSize = 0;
	DWORD			            flags;
	EngineCommSendMsg_t	        msg;
    int                         count;
    static SystemErrorNumber_t  errNum = SERR_NONE;
    PrinterMsg_t                printerMsg;

#ifdef CPU_TEMP_TEST
    // create cpu test polling thread
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CpuTemperatureTest, NULL, 0, NULL);

#endif

	while (1) {

		ReadMsgQueue(startupQueue, &msg, sizeof(msg), &receivedSize, INFINITE, &flags);

		if (receivedSize > 0) {
            switch (msg.msgId) {
                case MSG_STARTUP_GOTO_AMS:
				    GotoManufacturing();
                break;

                case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
                    if (msg.data.message[0] =='?') {
                        count = *(unsigned short *)&msg.data.message[2];
                        LogInfo ("Engine ? msg: %*.*s", count, count, &msg.data.message[4]);
                    } else {
                        LogInfo ("Receive Engine Command %c", msg.data.message[0]);
                    }
                break;

                case MSG_STARTUP_WATCH_DOG:
#ifndef FORCE_WATCHDOG_OFF
                    errNum = SERR_ENGINE_RESET;
                    LogError ("Watchdog timeout: error %x",errNum); 
                    // in the case of watchdog time out, send printer command to 
                    // get internal printer id to see if USB hub is dead or not.
                    printerMsg.msgId = MSG_PRINT_GET_PRINTER_ID;
                    SendPrinterMessage (&printerMsg);
                    if ((count = EngineBoardReset ()) != 0) {
                        LogError ("Fail to reset engine board %d", count);
                    }  else {
                        LogDebug ("Engine board reset success");
                    }
                    // kill processes except j2Gui, j2Printer and j2Startup for watchdog error
                    KillAll (WATCHDOG_STOP);
                    // display the Error Screen
                    SendCriticalErrorMessage (errNum);
                    //set up communication between GUI and start up process
                    RegisterGuiComm (STARTUP_QUEUE_NAME);
                    // Register with EngineComm for messages.
                    //SendMessageReceiverMessage ();
#else
                    LogDebug ("Watchdog force disable"); 
#endif
                break;

                case MSG_GUI_OUT_INFO:
                    if (errNum != SERR_NONE) {
                        ErrorInfo (errNum);
                    }
                break;

                // received restart command
                case MSG_STARTUP_RESTART:
                    // shutdown system and reboot
                    GotoRestart (1);
                break;

                case  MSG_GUI_OUT_BOTTOM_BUTTON:
                    if (errNum != SERR_NONE) {
                        SendCriticalErrorMessage (errNum);
                    }
                break;
            
                default:
				    LogDebug ("received %d message", msg.msgId);
                break;
			}
		}
	}
}
#endif


// start AMS 
void
GotoManufacturing ()
{
	// Kill the processes.
	KillAll (GOTO_MANUFACTURE);

	// Start up the manufacturing software.
	CreateProcess (AMS_PROGRAM);

	exit( 0 );
}


// restart program
void
GotoRestart (bool shutdownFlag)
{

    int retVal;    
    PROCESS_INFORMATION     proce_info;

    if (shutdownFlag == 1) {
        // soft shutdown system and reboot    
        retVal = CreateProcess (SHUTDOWN_PROCESS, L"", NULL, NULL,FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proce_info);

        if (retVal == 0) {
            LogError ("fail to run %S\n", SHUTDOWN_PROCESS);
        } else {
            // get signal when new process is killed or terminated
            WaitForSingleObject (proce_info.hProcess, INFINITE);
            // close thread and process
            CloseHandle (proce_info.hThread);
            CloseHandle (proce_info.hProcess);
        }

     }else {
        // kill process and restart j2Startup
         KillAll (GOTO_MANUFACTURE);

        // Create reboot process to start startup process 
        CreateProcess (STARTUP_PROGRAM);
        exit( 0 );
    }
}

// Timer Thread 
void *
TimerHandler( void * arg )
{
	int				elapsedTime;
	StartupMsg_t	msg;

	for ( elapsedTime = 0; elapsedTime < MAX_STARTUP_TIME; elapsedTime++ )
	{
		Sleep( 1000 );
	}

	/*
	 * If everything starts up properly, this thread will be canceled and we'll
	 * never get to this point.
	 */

	// Inform the main thread that we timed out.
	msg.msgId = MSG_STARTUP_ERROR;
	WriteMsgQueue(startupTimeoutQueue, (void *)&msg, sizeof( int ), INFINITE, NULL);

	return 0;
}


// load reference configuration file
void
LoadRefRanges (RefRangeData_t * refRanges)
{
    CfgError_t  cfgErr;
    int         i;

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
    if (!CopyRefRangesFile( refRanges )) {
        // Either there is no file on a USB drive, the file is bad, or there was an error
        // copying it to the local disk. So, reset the ref ranges to the defaults and
        // then read the local file.
        SetRefRangesToDefaults (refRanges, factoryData->abaxisProductCode);
        for (i = 0; i < STYPE_NUM_SPECIES; i++) {
            refRanges->isDefined[ i ] = false;
        }

        // Parse the file.
        cfgErr = ParseRefRanges (REF_RANGES_CURRENT_FILE, refRanges);
        if ( cfgErr != CFG_ERR_NONE )
        {
            // If there's a file open error, set the units to the defaults.  We have
            // to wait until here to set the values to defaults, because we don't want
            // to define a species that's not in the ref ranges files.
            if ( cfgErr == CFG_ERR_FILE_OPEN )
            {
                SetRefRangesToDefaults( refRanges, factoryData->abaxisProductCode );
            }

            // Any other error means a stop.
            else
            {
                LogError( "error parsing current reference ranges file: %d", cfgErr );
                ErrorStop(SERR_FILE_READ, systemData);
            }
        }
    }

}

// load Units configuration file 
void
LoadUnits( UnitsConfig_t * unitsConfig )
{
	CfgError_t	cfgErr;
    static 	    Units	units;

	// Start with the units set to defaults.  Don't write the file, though!
	units.SetToDefaults( false );

	// Parse the file.
	cfgErr = ParseUnits( unitsConfig );

	// If there's an error other than file not found, stop.
	if ( cfgErr != CFG_ERR_NONE && cfgErr != CFG_ERR_FILE_OPEN )
	{
		ErrorStop (SERR_FILE_READ, systemData);
	}
}


//  read console and engine software version number
void
GetVersionData (char bspVersion)
{
	FILE *	fp;
	char *	p;
    int     strLen;


    if (bspVersion == 0) {
        // get console software version 
	    memset (versionData, 0, sizeof (VersionData_t));
	    fp = fopen (VERSION_CONFIG_FILE_NAME, "rb");
	    if (fp != 0) {
		    // Read the version string.
		    fgets( versionData->version, sizeof( char ) * MAX_VERSION_LEN, fp );
		    // Trim off trailing spaces, tabs, newlines, etc.
		    p = versionData->version + strlen( versionData->version ) - 1;
		    while (isspace( *p )) {
			    *p = '\0';
			    p--;
		    }
		    fclose(fp);

		    // Get the release type.
		    versionData->releaseType = (char)fgetc (fp);
		    if (!isalpha (versionData->releaseType)) {
			    versionData->releaseType = 'R';
            }
	    } else {    
            LogError ("Fail to open console version file %s", VERSION_CONFIG_FILE_NAME);
        }
        // get Engine software version 
        memset (engineVersion, 0, MAX_ENG_SOFTWARE_VERSION_LEN + 1);
        fp = fopen (ENGINE_VERSION_FILE_NAME, "rb");
        if (fp != 0) {
            fgets (engineVersion, sizeof (char) * MAX_ENG_SOFTWARE_VERSION_LEN, fp);
            strLen = strlen (engineVersion);
            engineVersion [strLen -1] = '\0';
            fclose (fp);
        } else {
            LogError ("Fail to open Engine version file %s", ENGINE_VERSION_FILE_NAME);
        }
    } else {
        // get bsp version
        if (systemData->cpuBoard == 'B') {
            fp = fopen (BSP_BT_VERSION_FILE_NAME, "rb");
        } else {
            fp = fopen (BSP_VERSION_FILE_NAME, "rb");
        }
        if (fp != 0) {
            fgets (versionData->bspVersion, MAX_VERSION_LEN, fp);
		    p = versionData->bspVersion + strlen (versionData->bspVersion ) - 1;
            while (isspace( *p )) {
                *p = '\0';
                p--;
            }
            fclose (fp);
        } else {
         LogError ("Fail to open BSP version file %s", VERSION_CONFIG_FILE_NAME);
        }
    }
}

// read eeprom system info 
char
GetNvramProductCode ()
{
    unsigned	i;
    char		buff[10];

	for ( i = 0; i < 10; i++ )
	{
        EepromRead (i, (char *)&buff [i]);
	}

	if ( strncmp( "PICCOLO", buff, 7 ) == 0 )
	{
		return PICCOLO_PRODUCT_CODE;
	}

	return VETSCAN_PRODUCT_CODE;
}

// set language keyboard
void
SetKeyboard (Language_t language)
{
	GuiReceiveMsg_t	msg;

	msg.msgId = MSG_GUI_SET_LANGUAGE;

	switch ( language )
	{
		case LANGUAGE_ENGLISH:	
            msg.data.languageData.kbd = 0x00000409; 
        break;

		case LANGUAGE_FRENCH:	
            msg.data.languageData.kbd = 0x0000040c; 
        break;

		case LANGUAGE_GERMAN:	
            msg.data.languageData.kbd = 0x00000407; 
        break;

		case LANGUAGE_SPANISH:	
            msg.data.languageData.kbd = 0x00000410; 
        break;

		case LANGUAGE_ITALIAN:	
            msg.data.languageData.kbd = 0x0000040a; 
        break;

        case LANGUAGE_PORTUGUESE_B:
            msg.data.languageData.kbd = 0x00000416; 
        break;

        case LANGUAGE_PORTUGUESE:
            msg.data.languageData.kbd = 0x00000816; 
        break;

        case LANGUAGE_CHINESE: //chinese is english keyboard
            msg.data.languageData.kbd = 0x00000407; 
        break;

		default:
        break;
        
	}
             
	SendGuiMessage(&msg, sizeof(msg));
}

void
InitOrdersList()
{
    memset (ordersList, 0, sizeof (OrdersList_t));
    memset (ordersList->index, -1, sizeof(ordersList->index));

    if (settings->comm.commInterface == COMM_ASTM) {
        ReadOrdersFile (ordersList);
    }
}

bool
CopyRefRangesFile (RefRangeData_t * refRanges)
{
    CfgError_t          cfgErr;

    if (MountThumbDrive()) {
        // Read and parse the ref ranges file.
        cfgErr = ParseRefRanges (IMPORT_REF_RANGES_FILE, refRanges);

        // File not successfully read.
        if (cfgErr != CFG_ERR_NONE) {
            switch (cfgErr) {
                case CFG_ERR_CRC:
                    LogError ("CRC error importing USB drive ref ranges file");
                    break;
                case CFG_ERR_FILE_OPEN:
                    break;  // File not found, so just return.
                default:
                    LogError ("Parse error importing USB drive ref ranges file");
                    break;
            }
            return false;
        }

        // No error parsing the ref ranges file, so write it to the local disk.
        cfgErr = WriteRefRanges (REF_RANGES_CURRENT_FILE, refRanges);
        if (cfgErr != CFG_ERR_NONE) {
            LogError("Error writing USB drive ref ranges file to local disk");
            return false;
        } else {
            // Ref ranges on thumb drive found and successfully read in and written
            // to the local disk.
            return true;
        }
    } else {
        LogError ("mount of USB drive failed");
        return false;
    }

    return false;
}
