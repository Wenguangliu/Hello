/*
 * FILENAME
 * 	DateTimeStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine CD ROM update
 * 	states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "CdUpdateStates.h"
#include "SettingsMessages.h"
#include "LogErr.h"
#include "ResultsDb.h"

#define SOFTWARE_UPDATER	    "/CDROM Drive/SoftwareUpdaterNew.bat"
#define STARTUP_UPDATER	        "/CDROM Drive/StartupUpdaterNew.bat"
#define DBASE_OK_FILE		    "/CDROM Drive/dbase.dat"

#define CF_SOFTWARE_UPDATER	    "/Hard Disk/tmp/Updater.bat"
#define CF_STARTUP_UPDATEER     "/Hard Disk/tmp/Startup.bat"
#define BATCH_FILE_DIRECTORY    L"/Hard Disk/tmp"
#define RUNNER                  "/C \\\"Hard Disk\"\\tmp\\Updater.bat"

#define CALIBRATION_DATA        "\\Hard Disk\\CalibrationData.txt"	
#define TOUCH_CALIBRATE_REG	    L"SOFTWARE\\Hampshire\\CurrentVersion\\Global"
#define ENGINE_UPDATE_FILE      "/Hard Disk/Abaxis/bin/EngineUpdate.dat"
#define UPDATE_STATUS_FILE      "/Hard Disk/updateState.dat"

#define MAX_STRING              256

extern "C" DWORD RegSaveKey(HKEY, LPCTSTR, LPSECURITY_ATTRIBUTES);

typedef enum
{
	ERR_UPDATE_OPEN_SOURCE = 1,
	ERR_UPDATE_OPEN_DESTINATION,
	ERR_UPDATE_SET_PERMS,
	ERR_UPDATE_EXECUTE,
	ERR_UPDATE_WAIT,
    ERR_UPDATE_ENGINE_CODE,
} UpdateErr_t;

void *
StateCdUpdate::
UpdateSoftwareThread (void * arg)
{
	int 			        retVal;
	FILE *			        ifp;
	FILE *			        ofp;
	char			        buff[ 4096 ];
	int				        count;
	SettingsMsg_t	        msg;
	PROCESS_INFORMATION		proce_info;


    // backup touch screen calibration configuration data
    // take this out, because calibration data is saved during bsp updating 
    //WriteCalibrationData ();

    if (CreateDirectory (BATCH_FILE_DIRECTORY, NULL) == 0) {
        LogError ("Fail to create  tmp  directory");
    }


	ifp = fopen (SOFTWARE_UPDATER, "rb");
	if (ifp == 0) {
        LogDebug ("Thread fail to open SOFTWARE_UPDATER");
		msg.msgId = MSG_SETTINGS_CDUPDATE_ERROR;
		msg.data = ERR_UPDATE_OPEN_SOURCE;
		SendSettingsMessage( &msg, sizeof(int));
		return 0;
	}

	// Create updater on file system.
	ofp = fopen (CF_SOFTWARE_UPDATER, "wb");
	if  (ofp == 0) {
        LogDebug ("Thread fail to open CF_SOFTWARE_UPDATER");
		msg.msgId = MSG_SETTINGS_CDUPDATE_ERROR;
		msg.data = ERR_UPDATE_OPEN_DESTINATION;
		SendSettingsMessage( &msg, sizeof(int) );
        fclose (ifp);
		return 0;
	}
		
	// Copy the updater to the file systerm.	
	while ((count = fread( buff, 1, sizeof( buff ), ifp )) > 0) {
		fwrite (buff, 1, count, ofp);
	}
	fclose (ifp);
	fclose (ofp);


	ifp = fopen (STARTUP_UPDATER, "rb");
	if (ifp == 0) {
        LogDebug ("Thread fail to open STARTUP_UPDATER");
		msg.msgId = MSG_SETTINGS_CDUPDATE_ERROR;
		msg.data = ERR_UPDATE_OPEN_SOURCE;
		SendSettingsMessage (&msg, sizeof(int));
		return 0;
	}

	// Create updater on file system.
	ofp = fopen (CF_STARTUP_UPDATEER, "wb");
	if (ofp == 0) {
        LogDebug ("Thread fail to open CF_STARTUP_UPDATEER");
		msg.msgId = MSG_SETTINGS_CDUPDATE_ERROR;
		msg.data = ERR_UPDATE_OPEN_DESTINATION;
		SendSettingsMessage( &msg, sizeof(int));
        fclose (ifp);

		return 0;
	}
		
	// Copy the updater to the file systerm.	
	while ((count = fread( buff, 1, sizeof( buff ), ifp )) > 0) {
		fwrite (buff, 1, count, ofp);
	}
	fclose (ifp);
	fclose (ofp);
		
    // disable watchdog before update software
    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);
#if 0
    // trun off heater control DAC before update software
    SendHeaterControlMessage (MSG_HEATER_TURN_OFF_HEATER);
#endif
    // execute update batch file
    retVal = CreateProcess (_T("cmd.exe"), _T(RUNNER), NULL, NULL,FALSE, 0, NULL, NULL, NULL, &proce_info);
    // normally, system should reboot here and it should not be here
    // in the case of update fail, log message should be generated
    if (retVal == 0) {
		msg.msgId = MSG_SETTINGS_CDUPDATE_ERROR;
		msg.data = ERR_UPDATE_EXECUTE;
		SendSettingsMessage( &msg, sizeof (int) );
        LogDebug ("Fail to excute update batch file");
    } else {
        WaitForSingleObject (proce_info.hProcess, INFINITE);
        CloseHandle (proce_info.hThread);
        CloseHandle (proce_info.hProcess);
        // check update message 
        ifp = fopen (UPDATE_STATUS_FILE, "rb");
        if (ifp == 0) {
            msg.msgId = MSG_SETTINGS_CDUPDATE_ERROR;
            msg.data = ERR_UPDATE_EXECUTE;
            SendSettingsMessage( &msg, sizeof (int) );
        } else {
            memset (buff, 0, sizeof (buff));
            fread (buff, sizeof(buff), 1, ifp);
            LogDebug ("%s", buff);
            fclose (ifp);
        }
    }
    // enable watchdog after update software
    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
	return 0;
}

// backup touch screen calibration configuration 
void
StateCdUpdate::
WriteCalibrationData ()
{
    FILE    *pFile;
    HKEY    hKey = NULL;
    LONG    hRes;
    wchar_t valueName [MAX_STRING];
    BYTE    *data = NULL;
    DWORD   type;
    DWORD   valueNameSize;
    DWORD   dataSize;
    char    toWrite [MAX_STRING];
    DWORD   i = 0, j = 0;

    data = (LPBYTE) malloc(MAX_STRING);
    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, TOUCH_CALIBRATE_REG, 0, 0, &hKey);
    if ((ERROR_SUCCESS == hRes) && (hKey != NULL)) {
        if (DeleteFile (TEXT(CALIBRATION_DATA)) == 0) {
            LogError ("Failed to delete file %S, error =%d", CALIBRATION_DATA, GetLastError());
        }
        pFile = fopen (CALIBRATION_DATA, "wb");
        if (pFile != NULL) {
            valueNameSize = MAX_STRING;
            dataSize=MAX_STRING;
            hRes = RegEnumValue (hKey, i++, valueName, &valueNameSize, NULL, &type, (LPBYTE)data, &dataSize);

            while(hRes == ERROR_SUCCESS) {
                sprintf(toWrite, "%S\n", valueName);
                fwrite(toWrite, 1, strlen(toWrite), pFile);
                sprintf(toWrite, "%d\n", type);
                fwrite(toWrite, 1, strlen(toWrite), pFile);
                sprintf(toWrite, "%d\n", dataSize);
                fwrite(toWrite, 1, strlen(toWrite), pFile);

                for (j = 0; j < dataSize; j++) {
                    sprintf (toWrite, "0x%.2x\n", data[j]);
                    fwrite (toWrite, 1, strlen(toWrite), pFile);
                }
                fwrite("\n", 1, 1, pFile);
                valueNameSize=MAX_STRING;
                dataSize=MAX_STRING;
                hRes = RegEnumValue (hKey, ++i, valueName, &valueNameSize, NULL, &type, data, &dataSize);
            }
            fclose(pFile);
        }
        hRes = RegCloseKey(hKey);
    } else {
        LogError ("Failed to Backup Calibration Registry Settings" );
    }
    free(data);
}

/*
 * class StateCdUpdateSelect
 */
 
void
StateCdUpdateSelect::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel (strings[ TSTR_CDROM_UPDATE ]);

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_UPDATE_SOFTWARE ] );
	wcscpy( data->buttonText[1], strings[ MSTR_ADD_CONTROL_VALUES ] );
	data->buttonHighlighted[0] = true;
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateCdUpdateSelect::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_CDUPDATE_SELECT;
	int		idata = *(int *)data;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			if ( idata == 0 )	// Update Software
			{
				retVal = STATE_CDUPDATE_LOAD_SOFTWARE_CD;
			}
			else				// Add Control Values
			{
				retVal = STATE_CDUPDATE_LOAD_CONTROLS_CD;
			}
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( idata == 0 )	// Back
			{
				retVal = stateStack.Pop();
			}
			else				// Home
			{
				retVal = stateStack.Home();
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = L"CD Update Select";
			retVal = STATE_INFO;
			stateStack.Push( STATE_CDUPDATE_SELECT );
			break;		
		default:
			break;
	}	
	return retVal;
}

/*
 * class StateCdUpdateLoadSoftwareCd
 */
 
void
StateCdUpdateLoadSoftwareCd::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleMessageData_t *	data = &msg.data.middleMessage;

	SetTopLabel( strings[ TSTR_LOAD_CD ] );

	msg.msgId = MSG_GUI_MIDDLE_MESSAGE_INFO;
	wcscpy( data->message, strings[ MSTR_INSERT_CD ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_CONTINUE ], true );
}

state_t
StateCdUpdateLoadSoftwareCd::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_CDUPDATE_LOAD_SOFTWARE_CD;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
                    // in the case of sync Engine code, push the state back
                    if (retVal  == STATE_CDUPDATE_LOAD_SOFTWARE_CD) {
			            stateStack.Push (STATE_CDUPDATE_LOAD_SOFTWARE_CD);
                    }
					break;
				case 1:		// Continue
					retVal = STATE_CDUPDATE_READING_SOFTWARE_CD;
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_CD_UPDATE ];
			retVal = STATE_INFO;
			stateStack.Push (STATE_CDUPDATE_LOAD_SOFTWARE_CD);
			break;
		default:
			break;
	}
	
	return retVal;
}


/*
 * class StateCdUpdateReadingSoftwareCd
 */
 
void
StateCdUpdateReadingSoftwareCd::
Enter()
{
	GuiReceiveMsg_t guiMsg;
	FILE *			fp;
	SettingsMsg_t	settingsMsg;
	char			buff[1024];
	char *			s;
    char            reTry = 0;
    DWORD           attribs;
    wchar_t         fileName [255];
	
	SetTopLabel( 0 );

	guiMsg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	wcscpy( guiMsg.data.middleMessage.message, strings[ MSTR_READING_CD ] );
	SendGuiMessage( &guiMsg, sizeof( GuiMiddleMessageData_t ) );
	
	guiMsg.msgId = MSG_GUI_BOTTOM_NONE;
	SendGuiMessage (&guiMsg, 0);

    // set delay time for CD detection
    while (reTry < 5) {
        wsprintf (fileName, L"%S", SOFTWARE_UPDATER);
        attribs = GetFileAttributes (fileName);
        if (attribs == INVALID_FILE_ATTRIBUTES) {
            Sleep (3000);
            reTry++;
        } else {
            break;
        }
    }
	// See if the updater file is on the CD.  If not, trigger an error message.
	fp = fopen (SOFTWARE_UPDATER, "rb");
	if (fp == 0) {
        LogDebug ("STATE_CDUPDATE_READING_SOFTWARE_CD error triger");
		settingsMsg.msgId = MSG_SETTINGS_CDUPDATE_ERROR;
		settingsMsg.data = ERR_UPDATE_OPEN_SOURCE;
		SendSettingsMessage (&settingsMsg, sizeof (int));
		return;
	}
	fclose (fp);

	// See if the database version conversion info file is on the CD.  If not,
	// trigger a warning message.
	fp = fopen (DBASE_OK_FILE, "rb");
	if (fp == 0) {
		SendSettingsMessage (MSG_SETTINGS_CDUPDATE_WARNING);
		return;
	}

	// Read the file and check to see if the current version of the database is
	// in there.  If it is, trigger a move on to the update state.
	fread (buff, sizeof( char ), 1024, fp);
	s = strtok (buff, ",");
	while (s != 0) {
		int version = atoi (s);
		if  (version == RESULTS_DB_VERSION) {
			fclose (fp);
            // CDROM is detected and read OK, goto update software state
			SendSettingsMessage (MSG_SETTINGS_CDUPDATE_OK);
			return;
		}
		
		s = strtok (0, ",");
	}

	// No converter for the current database version, so trigger a warning
	// message.
	SendSettingsMessage (MSG_SETTINGS_CDUPDATE_WARNING);
	fclose (fp);
}

state_t
StateCdUpdateReadingSoftwareCd::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_CDUPDATE_READING_SOFTWARE_CD;

	switch ( msgId )
	{
		case MSG_SETTINGS_CDUPDATE_ERROR:
            LogDebug ("STATE_CDUPDATE_READING_SOFTWARE_CD error");
			LogError( "CdUpdate error #%d", *(int *)data );
			retVal = STATE_CDUPDATE_ERROR;
			break;
		
		case MSG_SETTINGS_CDUPDATE_WARNING:
			retVal = STATE_CDUPDATE_WARNING;
			break;
		
		case MSG_SETTINGS_CDUPDATE_OK:
			retVal = STATE_CDUPDATE_UPDATE_SOFTWARE;
			break;

        default:
            LogError ("Get unexpected msgId = 0x%x", msgId);
            break;
	}
	
	return retVal;
}
/*
 * class StateCdUpdateUpdateSoftware
 */
 
void
StateCdUpdateUpdateSoftware::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleProgBarMessageData_t *	data = &msg.data.middleProgBarMessage;

	SetTopLabel (strings[ TSTR_SOFTWARE_UPDATE ]);

	msg.msgId = MSG_GUI_MIDDLE_PROG_BAR_MESSAGE;
	wcscpy (data->progBarText, strings [MSTR_UPDATING_SOFTWARE]);
	data->progBarValue = 0;
	data->backgroundColor = 0xFFFFFFFF;
	data->fontColor = 0x004E4E5E;
	
	wcscpy (data->message, strings [MSTR_PLEASE_WAIT_CDUPDATE]);
	
	SendGuiMessage (&msg, sizeof( GuiMiddleMessageData_t ));

	SetBottomButtons ();

	progBarValue = 0;
	done = false;

    //LogDebug ("Got StateCdUpdateUpdateSoftware state");

	// Timer thread for progress bar updates.
	CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)TimerThread, NULL, 0, NULL);

	// Update thread to do the actual work.
	CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)UpdateSoftwareThread, NULL, 0, NULL);
}

state_t
StateCdUpdateUpdateSoftware::
ProcessMessage (int msgId, char * data)
{
	state_t retVal = STATE_CDUPDATE_UPDATE_SOFTWARE;
	
	switch ( msgId )
	{
		case MSG_SETTINGS_TIMER:
			if (!done) {
				progBarValue += 10;
				if (progBarValue > 100) {
					progBarValue = 0;
				}
				GuiReceiveMsg_t msg;
				msg.msgId = MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS;
				msg.data.middleProgBarMessage.progBarValue = progBarValue;
				SendGuiMessage (&msg, sizeof( GuiMiddleProgBarMessageData_t));
				CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)TimerThread, NULL, 0, NULL);
			} else {
				stateStack.Home();
			}
			break;

		case MSG_SETTINGS_CDUPDATE_ERROR:
            LogDebug ("STATE_CDUPDATE_UPDATE_SOFTWARE error");
			LogError ("CdUpdate error #%d", *(int *)data);
			retVal = STATE_CDUPDATE_ERROR;
			break;

        case MSG_SETTINGS_ENGINE_DOWNLOAD_ERROR:
			LogError ("CdUpdate error #%d", *(int *)data);
            retVal = STATE_ENGINE_CODE_DOWNLOAD_ERROR;
            break;

		case MSG_SETTINGS_CDUPDATE_DONE:
			// We ignore this message because we just want to stay in the
			// moving prog bar mode until the update resets the system.
			break;
	}

	return retVal;
}


/*
 * StateCdUpdateError
 */

void
StateCdUpdateError::
Enter ()
{
	GuiReceiveMsg_t	guiMsg;

    //ZZZ
    LogDebug ("get into CD error state");

	guiMsg.msgId = MSG_GUI_WIN_ERROR;
	wcscpy( guiMsg.data.winError.topLabel, strings[ TSTR_ERROR ] );

	// Add the error number to the beginning of the error message.
	wsprintf( guiMsg.data.winError.message, L"%x\n%s", SERR_CD_UPDATE, strings[ MSTR_ERROR_READING_CD ] );

	wcscpy( guiMsg.data.winError.buttonText, strings[ BSTR_CONTINUE ] );
	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
}

state_t
StateCdUpdateError::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_CDUPDATE_ERROR;
	SystemError	systemError;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Continue
			retVal = stateStack.Pop();
            // in the case of Engine Sync code, push back the load software stack
            if (retVal == STATE_CDUPDATE_LOAD_SOFTWARE_CD) {
                stateStack.Push (STATE_CDUPDATE_LOAD_SOFTWARE_CD);
            }
			break;

		case MSG_GUI_OUT_INFO:
			infoText = strings[ systemError.GetInfoText( SERR_CD_UPDATE ) ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_CDUPDATE_ERROR );
			break;
			
		default:
			break;
	}
	
	return retVal;
}

void
StateEngineDownloadError::
Enter()
{
	GuiReceiveMsg_t	guiMsg;

	guiMsg.msgId = MSG_GUI_WIN_ERROR;
	wcscpy( guiMsg.data.winError.topLabel, strings[ TSTR_ERROR ] );

	// Add the error number to the beginning of the error message.
	wsprintf( guiMsg.data.winError.message, L"%x\n%s", SERR_ENGINE_CODE_DOWNLOAD, strings[ ESTR_INTERNAL_SOFTWARE ] );

	wcscpy( guiMsg.data.winError.buttonText, strings[ BSTR_CONTINUE ] );
	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
}

state_t
StateEngineDownloadError::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_ENGINE_CODE_DOWNLOAD_ERROR;
	SystemError	systemError;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Continue
			retVal = stateStack.Pop();
			break;

		case MSG_GUI_OUT_INFO:
			infoText = strings[ systemError.GetInfoText( SERR_ENGINE_CODE_DOWNLOAD ) ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_ENGINE_CODE_DOWNLOAD_ERROR );
			break;
			
		default:
			break;
	}
	
	return retVal;
}

/*
 * StateCdUpdateWarning
 */

void
StateCdUpdateWarning::
Enter()
{
	GuiReceiveMsg_t		guiMsg;
	
    //ZZZ
    LogDebug ("got StateCdUpdateWarning state");
	guiMsg.msgId = MSG_GUI_WIN_WARNING2;
	wcscpy( guiMsg.data.winWarning.topLabel, strings[ TSTR_WARNING ] );
	wcscpy( guiMsg.data.winWarning.message, strings[ ESTR_DATA_LOSS ] );
	wcscpy( guiMsg.data.winWarning.buttonText[0], strings[ BSTR_CONTINUE ] );
	wcscpy( guiMsg.data.winWarning.buttonText[1], strings[ BSTR_CANCEL ] );
	
	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
}

state_t
StateCdUpdateWarning::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_CDUPDATE_WARNING;
	int			idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_INFO:
			infoText = strings[ EISTR_DATA_LOSS ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_CDUPDATE_WARNING );
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Continue
					retVal = STATE_CDUPDATE_UPDATE_SOFTWARE;
					break;
				case 1:		// Cancel
					retVal = stateStack.Pop();
					break;
			}
			break;

		default:
			break;
	}

	return retVal;
}

