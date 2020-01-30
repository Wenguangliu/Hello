/*
 i FILENAME
 * 	CommStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine set communication
 * 	parameters states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <winsock2.h>
#include <winioctl.h>
#include <ntddndis.h>	
#include "CommonDefines.h"
#include "CommStates.h"
#include "SettingsMessages.h"
#include "ParseSettings.h"
#include "CfgErr.h"
#include "LogErr.h"
#include "Shmem.h"
#include "GetThumbKeyMode.h"
#include "ParseHL7Configuration.h"
#include "MountThumbDrive.h"
#include "ParseFuseSettings.h"

CommInterface_t	StateComm::currentCommInterface;
CommProtocol_t	StateComm::currentCommProtocol;
char			StateComm::currentAnalyzerIp[30];
char			StateComm::currentGateway[30];
char			StateComm::currentIpMask[30];
char			StateComm::currentServerIp[30];
int				StateComm::currentServerSocket;
int				StateComm::currentClientSocket;
bool            StateComm::displayFuseOption = false;
StringId_t		StateComm::errString = STR_BLANK;
StringId_t		StateComm::errInfoString = STR_BLANK;
bool			StateComm::analyzerIpChanged = false;
bool			StateComm::gatewayChanged = false;
bool			StateComm::ipMaskChanged = false;
char            StateComm::hl7SetFlag = 1;
wchar_t         StateComm::displayText [128];
socketType_t    StateComm::socketSetType = NONE_SOCKET;
Hl7ConfigurationFileList_t * StateCommHl7ConfigSelect::hl7FileList   = (Hl7ConfigurationFileList_t *)MapSharedMem (HL7_FILE_SHEME_NAME, sizeof (Hl7ConfigurationFileList_t));


/*
 * StateComm
 */

/*
 * FUNCTION
 * 	StateComm::SaveConfiguration
 * 
 * DESCRIPTION
 * 	If any of the Comm settings has been changed, this writes the settings
 *  config file and starts a thread that updates the Ethernet port settings
 * 	and kills/starts/restarts ASTM and SerialComm.
 * 
 * PARAMETERS
 *	returnState - State the state machine should go to next if
 * 				  the config file write is successful.
 * _doStartComm - Flag to indicate whether the comm program should be started.
 * 
 * RETURN
 * 	The value of the returnState parameter passed in if there is no write
 * 	error, else STATE_ERROR.
 */

state_t
StateComm::
SaveConfiguration (state_t returnState, bool _doStartComm)
{
	CfgError_t	cfgErr;
	static bool	doStartComm;

	if (ConfigurationChanged()) {
		cfgErr = WriteSettings (settingsData);
		if (cfgErr != CFG_ERR_NONE) {
			errNum = SERR_FILE_WRITE;
			returnState = STATE_ERROR;
		}
    }
	
	doStartComm = _doStartComm;
	CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)ConfigurationThread, (void *)&doStartComm, 0, NULL);
		
	// Update the saved values for Server IP and Server Socket.
	strcpy (currentServerIp, settingsData->comm.serverIp);
	currentServerSocket = settingsData->comm.serverSocket;
	currentClientSocket = settingsData->comm.clientSocket;
		
	// If we're not in ASTM now, we may have come from there, so recheck the thumb key in case
	// we need to go back to Research mode.
	if (_doStartComm && (settingsData->comm.commInterface != COMM_ASTM) && (settingsData->comm.commInterface != COMM_HL7)) {
		GetThumbKeyMode (systemData);
        if (!IN_CUSTOMER_MODE) {
            // if not in customer mode, set fan to be high speed
            StateSettings::SendFanMessage ('F', IDLE_FAN_HIGH_SPEED);
        }
	}
	
	return returnState;
}
	
/*
 * FUNCTION
 * 	StateComm::ConfigurationChanged
 * 
 * DESCRIPTION
 * 	Checks to see if any of the Comm parameters have changed.
 * 
 * PARAMETERS
 *	None
 * 
 * RETURN
 * 	true if the Comm configuration has changed.
 */

bool
StateComm::
ConfigurationChanged()
{
	bool	commInterfaceChanged = currentCommInterface != settingsData->comm.commInterface;;
	bool	commProtocolChanged = currentCommProtocol != settingsData->comm.protocol;
	bool	serverIpChanged = currentServerIp != settingsData->comm.serverIp;
	bool	serverSocketChanged = currentServerSocket != settingsData->comm.serverSocket;
	bool	clientSocketChanged = currentClientSocket != settingsData->comm.clientSocket;

	// These comparisons are saved for use by ConfigurationThread.
	analyzerIpChanged = strcmp (currentAnalyzerIp, settingsData->comm.analyzerIp) != 0;
	gatewayChanged = strcmp (currentGateway, settingsData->comm.gateway) != 0;
	ipMaskChanged = strcmp (currentIpMask, settingsData->comm.ipMask) != 0;
    serverIpChanged = (strcmp( currentServerIp, settingsData->comm.serverIp ) != 0);

	return commInterfaceChanged || commProtocolChanged || serverIpChanged || serverSocketChanged ||
		   analyzerIpChanged || gatewayChanged || ipMaskChanged || clientSocketChanged;
}

/*
 * FUNCTION
 * 	StateComm::SetEthernet
 * 
 * DESCRIPTION
 * 	Updates the Ethernet port settings.
 * 
 * PARAMETERS
 *	forceUpdate - Flag to indicate that the ethernet settings should be reset regardless of
 * 				  whether they seem to have changed.
 */

void
StateComm::
SetEthernet( bool forceUpdate )
{
	HKEY hKey = NULL;
	LONG hRes;
	wchar_t		ipWSTR[256];
	BSTR		adapter_name;

	// If either the analyzer IP, the IP mask or the Default Gateway changed,
	// we need to update registry values and rebind the adapter
	if (forceUpdate || analyzerIpChanged || ipMaskChanged || gatewayChanged) {
        SetStaticIp (systemData->cpuBoard, settingsData);
		
		// Update the saved values for Analyzer IP and IP Mask.
		strcpy (currentAnalyzerIp, settingsData->comm.analyzerIp);
		strcpy (currentIpMask, settingsData->comm.ipMask );
		strcpy (currentGateway, settingsData->comm.gateway );
	}

	// Rewrite SetIpAddress regardless of whether the parameters have changed or not.
	// That way we can at least be sure the instrument is in synch once it's reset.
	WriteSetIpAddress();
}


/*
 * FUNCTION
 * 	StateComm::ConfigurationThread
 * 
 * DESCRIPTION
 * 	Updates the Ethernet port settings and kills/starts/restarts ASTM and SerialComm.
 * 
 * PARAMETERS
 *	arg - Standard thread parameter - ignored.
 * 
 * RETURN
 * 	0 - Standard thread return parameter.
 */

void *
StateComm::
ConfigurationThread( void * arg )
{
	bool	doStartComm = *(bool *)arg;
	PROCESS_INFORMATION		proc_info;
	int						result;
	wchar_t					processWSTR[256];
    SerialCommMsg_t         serialCommMsg;

	// Set the Ethernet port TCP/IP parameters.
	SetEthernet ();
	
	// Start ASTM or SerialComm if the flag is true (if this wasn't called from CheckASTM).
	if ( doStartComm )
	{
		// There shouldn't be a Comm process already running, but if there is we'll kill it first.
		if ( systemData->commPid != -1 )
		{
			LogError( "attempt to start SerialComm/ASTM when a process was already running" );
			//kill (systemData->commPid, SIGKILL);
			DWORD exitCode;
			HANDLE process = OpenProcess(0,FALSE, systemData->commPid);
			if (process != NULL) {
                 if ((settingsData->comm.commInterface != COMM_ASTM) && (settingsData->comm.commInterface != COMM_HL7)) {
                    //close serial port before kill process
                    serialCommMsg.msgId = MSG_SERIAL_COMM_CLOSE_PORT;
                    SendSerialCommMessage (&serialCommMsg, sizeof(SendReportData_t));
                    Sleep (1);
                }
				GetExitCodeProcess(process, &exitCode);
				TerminateProcess(process, exitCode);
				CloseHandle(process);
			}
			systemData->commPid = -1;
		}

		// Start/restart SerialComm if the interface isn't ASTM.
		if ((settingsData->comm.commInterface != COMM_ASTM) && (settingsData->comm.commInterface != COMM_HL7))
		{
            LogDebug ("Start COMM process");
			wsprintf(processWSTR, L"%S", SERIALCOMM_PROGRAM);
			result = CreateProcess(processWSTR, L"", NULL, NULL, FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proc_info);
			systemData->commPid = proc_info.dwProcessId;
			if ( result == 0 )
			{
				LogError( "failed to start SerialComm" );
				systemData->commPid = -1;
			}
		}
	
		// Start/restart ASTM.
		else if (settingsData->comm.commInterface == COMM_ASTM) 
		{
            LogDebug ("Start ASTM process");
			wsprintf(processWSTR, L"%S", ASTM_PROGRAM);
			result = CreateProcess(processWSTR, L"", NULL, NULL, FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proc_info);
			systemData->commPid = proc_info.dwProcessId;
			if ( result == 0 )
			{
				LogError( "failed to start ASTM" );
				systemData->commPid = -1;
			}

			// If we were in Research mode, we need to exit it.
			systemData->inResearchMode = false;
             // wait a while to set this flag to be false
            // to prevent networkDetect thread miss triggered
            Sleep (5000);
		} 
        // Start Hl7 process
        else if (settingsData->comm.commInterface == COMM_HL7)
        {
            LogDebug ("Start HL7 process");
			wsprintf(processWSTR, L"%S", HL7_PROGRAM);
			result = CreateProcess(processWSTR, L"", NULL, NULL, FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proc_info);
			systemData->commPid = proc_info.dwProcessId;
			if ( result == 0 )
			{
				LogError( "failed to start HL7" );
				systemData->commPid = -1;
			}

			// If we were in Research mode, we need to exit it.
			systemData->inResearchMode = false;
        } 
	}
	
	// Update the saved Comm Interface and Comm Protocol.
	currentCommInterface = settingsData->comm.commInterface;
	currentCommProtocol = settingsData->comm.protocol;
	
	return 0;
}

void
StateComm::
WriteSetIpAddress()
{
	FILE *	fp;

    WCHAR   wFileName [128];
    int     error;

   // convert to wchar file
    wsprintf (wFileName, L"%S", IP_ADDRESS_FILE_NAME);

    //change file attribution
    if (SetFileAttributes (wFileName, FILE_ATTRIBUTE_NORMAL) == 0) {
      error = GetLastError ();
      LogError ("Fail to change file %s write permit, error =%u ", wFileName, error);
      return ;
    }

	fp = fopen(IP_ADDRESS_FILE_NAME, "wb" );
	if ( fp != 0 )
	{
        if (settingsData->comm.analyzerIp [0] == '\0') {
		    fprintf( fp, "%s\n", "0.0.0.0");
        } else {
		    fprintf( fp, "%s\n", settingsData->comm.analyzerIp );
        }
		if ( settingsData->comm.ipMask[0] == '\0' )
		{
            // save mask as zero */
            fprintf(fp,  "%s\n","0.0.0.0");
		}
		else
		{
			fprintf( fp, "%s\n",settingsData->comm.ipMask );
		}
		if ( settingsData->comm.gateway[0] == '\0' )
		{
            fprintf(fp,  "%s\n","0.0.0.0");
		} else {
			fprintf( fp, "%s\n", settingsData->comm.gateway );
        }
		fclose( fp );
	}
}

/*
 * FUNCTION
 * 	StateComm::ValidateIp
 * 
 * DESCRIPTION
 * 	Checks an IP address string to verify that it represents a valid address.
 *  Assumes that the only characters in the string are decimal digits
 *  and periods.
 * 
 * PARAMETERS
 *	ip - IP address string
 * 
 * RETURN
 * 	true if a valid IP address, else false
 */

bool
StateComm::
ValidateIp( char * ip )
{
	char *	p;
	char *	ipCopy;
	int		i;
	int		value;
	
	// First check for two or more '.'s in a row, since strtok
	// will treat them the same as a single '.'.
	if ( strstr( ip, ".." ) != 0 )
	{
		return false;
	}

	// Make a copy of the ip string, since strtok will modify it.
	ipCopy = _strdup( ip );
	if ( ipCopy == 0 )
	{
		return false;
	}

	// Now check the value of each of the '.'-delimited fields.
	p = strtok( ipCopy, "." );
	i = 0;
	while ( p != 0 )
	{
		value = atoi( p );
		if ( (value < 0) || (value > 255) )
		{
			return false;
		}
		p = strtok( 0, "." );
		i++;
	}

	// Finally, verify that we got exactly four fields - no more
	// and no fewer.
	if ( i != 4 )
	{
		return false;
	}

	// All checks passed, so return true.
	return true;
}

/*
 * FUNCTION
 * 	StateComm::ValidateSocket
 * 
 * DESCRIPTION
 * 	Checks a socket value string to verify that it represents a legal socket.
 *  Assumes that the only characters in the string are decimal digits.
 * 
 * PARAMETERS
 *	socket - socket value string.
 * 
 * RETURN
 * 	true if a valid socket value, else false
 */

bool
StateComm::
ValidateSocket( char * socket )
{
	int value;

	// Check string length so atoi doesn't overflow and also to make sure
	// it's not blank.
	if ( (strlen( socket) == 0) || (strlen( socket ) > 5) )
	{
		return false;
	}
	
	// Now make sure we're in range.
	value = atoi( socket );
	if ( (value < 0) || (value > 0xFFFF) )
	{
		return false;
	}
		
	// Legal value.
	return true;
}

void
StateComm::
LoadFuseSettings()
{
    // For the NGA, we can just set the fixed values. For the JEN2, it's necessary to parse the
    ParseFuseSettings (settingsData);
}

/*
 * StateCommAstmCheck
 */

void
StateCommNetworkCheck::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_CHECK_TCPIP_SETTINGS ] );

	wcscpy( msg.data.middleViewResults.text, strings[ MSTR_TESTING_CONNECTION ] );

	msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
	SendGuiMessage( &msg, sizeof( GuiMiddleViewResultsData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );

	// Send a trigger message so we move on to ProcessMethod().
	SendSettingsMessage( MSG_SETTINGS_TRIGGER );
}

state_t
StateCommNetworkCheck::
ProcessMessage( int msgId, char * data )
{
	state_t 			retVal = STATE_COMM_NETWORK_CHECK;
	int					idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t		msg;
	int					clientSocket = -1;
	int					status = 0;
	struct sockaddr_in	serverSocket = { 0 };
	bool				socketError;
	
	switch ( msgId )
	{
		case MSG_SETTINGS_TRIGGER:
			socketError = false;

			//clientSocket = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
            clientSocket = socket( AF_INET, SOCK_STREAM, 0 );

			if ( clientSocket == INVALID_SOCKET )
			{
				LogError( "socket() failed %d", WSAGetLastError());
				socketError = true;
			}
			else
			{			
				//status = inet_aton( settingsData->comm.serverIp, &serverSocket.sin_addr );
				serverSocket.sin_addr.S_un.S_addr = inet_addr(settingsData->comm.serverIp);
				if ( serverSocket.sin_addr.S_un.S_addr == 0 || serverSocket.sin_addr.S_un.S_addr == INADDR_NONE)
				{
					LogError( "inet_addr() failed" );
					socketError = true;
				}
				
				else
				{
					serverSocket.sin_family = AF_INET;
					serverSocket.sin_port = htons( settingsData->comm.serverSocket );
					status = connect( clientSocket, (struct sockaddr *)&serverSocket, sizeof( serverSocket ) );
					if ( status == -1 )
					{
						LogError( "connect() failed" );
						socketError = true;
					}
					closesocket( clientSocket );
				}
			}
			
			if ( socketError )
			{
				wcscpy( msg.data.middleViewResults.text, strings[ MSTR_CONNECTION_FAILED ] );
			}
			else
			{
				wcscpy( msg.data.middleViewResults.text, strings[ MSTR_CONNECTION_SUCCESSFUL ] );
			}
			msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
			SendGuiMessage( &msg, sizeof( GuiMiddleViewResultsData_t ) );
			break;
		
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = SaveConfiguration( stateStack.Home() );
					break;
				default: break;
			}
			break;
	}

	return retVal;
}
 
/*
 * StateCommAstmEnterAnalyzerIp
 */

void
StateCommNetworkEnterAnalyzerIp::
Enter()
{
	GuiReceiveMsg_t msg;

	SetTopLabel( strings[ TSTR_ENTER_ANALYZER_IP_ADDRESS ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_IP;
	msg.data.middleEnterIp.text[0] = '\0';
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIpData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
	
	memset( value, 0, sizeof( value ) );
}

state_t
StateCommNetworkEnterAnalyzerIp::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_COMM_NETWORK_ENTER_ANALYZER_IP;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t msg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			if ( idata == -1 )		// 'C'
			{
				memset( value, 0, sizeof( value ) );
			}
			else if ( wcslen( value ) < (sizeof( value ) - 1) )
			{
				if ( (idata >= 0) && (idata <= 9) )
				{
					value[ wcslen( value ) ] = idata + '0';
				}
				else if ( idata == -2 )		// '.'
				{
					value[ wcslen( value ) ] = L'.';
				}
			}

			msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
			wcscpy( msg.data.middleEnterIp.text, value );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterIpData_t ) );

			break;
		
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
				case 2:		// Save
					if ( ValidateIp( WstrToCstr (value) ) == true )
					{
						strcpy( settingsData->comm.analyzerIp, WstrToCstr (value) );
						retVal = stateStack.Pop();
					}
					else
					{
						errString = ESTR_ILLEGAL_IP_ADDRESS;
						errInfoString = EISTR_ILLEGAL_IP_ADDRESS;
						retVal = STATE_COMM_ERROR;
						stateStack.Push( STATE_COMM_NETWORK_ENTER_ANALYZER_IP );
					}
					break;
				default: break;
			}
			break;
			
		default:
			break;
	}

	return retVal;
}

/*
 * StateCommNetworkEnterGateway
 */

void
StateCommNetworkEnterGateway::
Enter()
{
	GuiReceiveMsg_t msg;

	SetTopLabel( strings[ TSTR_ENTER_GATEWAY_IP_ADDRESS ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_IP;
	msg.data.middleEnterIp.text[0] = '\0';
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIpData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
	
	memset( value, 0, sizeof( value ) );
}

state_t
StateCommNetworkEnterGateway::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_COMM_NETWORK_ENTER_GATEWAY;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t msg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			if ( idata == -1 )		// 'C'
			{
				memset( value, 0, sizeof( value ) );
			}
			else if ( wcslen( value ) < (sizeof( value ) - 1) )
			{
				if ( (idata >= 0) && (idata <= 9) )
				{
					value[ wcslen( value ) ] = idata + '0';
				}
				else if ( idata == -2 )		// '.'
				{
					value[ wcslen( value ) ] = L'.';
				}
			}

			msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
			wcscpy( msg.data.middleEnterIp.text, value );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterIpData_t ) );

			break;
		
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
				case 2:		// Save
					if ( (wcslen( value) == 0) || (ValidateIp( WstrToCstr (value )) == true) )
					{
						strcpy( settingsData->comm.gateway, WstrToCstr (value) );
						retVal = stateStack.Pop();
					}
					else
					{
						errString = ESTR_ILLEGAL_IP_ADDRESS;
						errInfoString = EISTR_ILLEGAL_IP_ADDRESS;
						retVal = STATE_COMM_ERROR;
						stateStack.Push( STATE_COMM_NETWORK_ENTER_GATEWAY );
					}
					break;
				default: break;
			}
			break;
			
		default:
			break;
	}

	return retVal;
}

/*
 * StateCommNetworkEnterIpMask
 */

void
StateCommNetworkEnterIpMask::
Enter()
{
	GuiReceiveMsg_t msg;

	SetTopLabel( strings[ TSTR_ENTER_IP_MASK ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_IP;
	msg.data.middleEnterIp.text[0] = '\0';
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIpData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
	
	memset( value, 0, sizeof( value ) );
}

state_t
StateCommNetworkEnterIpMask::
ProcessMessage( int msgId, char * data )
{
	state_t 				retVal = STATE_COMM_NETWORK_ENTER_IP_MASK;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t msg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			if ( idata == -1 )		// 'C'
			{
				memset( value, 0, sizeof( value ) );
			}
			else if ( wcslen( value ) < (sizeof( value ) - 1) )
			{
				if ( (idata >= 0) && (idata <= 9) )
				{
					value[ wcslen( value ) ] = idata + '0';
				}
				else if ( idata == -2 )		// '.'
				{
					value[ wcslen( value ) ] = L'.';
				}
			}

			msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
			wcscpy( msg.data.middleEnterIp.text, value );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterIpData_t ) );

			break;
		
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
				case 2:		// Save
					if ( ValidateIp( WstrToCstr (value) ) == true )
					{
						strcpy( settingsData->comm.ipMask, WstrToCstr (value) );
						retVal = stateStack.Pop();
					}
					else
					{
						errString = ESTR_ILLEGAL_IP_MASK;
						errInfoString = EISTR_ILLEGAL_IP_MASK;
						retVal = STATE_COMM_ERROR;
						stateStack.Push( STATE_COMM_NETWORK_ENTER_IP_MASK );
					}
					break;
				default: break;
			}
			break;
			
		default:
			break;
	}

	return retVal;
}

/*
 * StateCommNetworkEnterServerIp
 */

void
StateCommNetworkEnterServerIp::
Enter()
{
	GuiReceiveMsg_t msg;

	SetTopLabel( strings[ TSTR_ENTER_SERVER_IP_ADDRESS ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_IP;
	msg.data.middleEnterIp.text[0] = '\0';
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIpData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
	
	memset( value, 0, sizeof( value ) );

}

state_t
StateCommNetworkEnterServerIp::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_COMM_NETWORK_ENTER_SERVER_IP;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t msg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			if ( idata == -1 )		// 'C'
			{
				memset( value, 0, sizeof( value ) );
			}
			else if ( wcslen( value ) < (sizeof( value ) - 1) )
			{
				if ( (idata >= 0) && (idata <= 9) )
				{
					value[ wcslen( value ) ] = idata + '0';
				}
				else if ( idata == -2 )		// '.'
				{
					value[ wcslen( value ) ] = L'.';
				}
			}

			msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
			wcscpy( msg.data.middleEnterIp.text, value );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterIpData_t ) );

			break;
		
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
				case 2:		// Save
					if ( ValidateIp( WstrToCstr (value) ) == true )
					{
						strcpy( settingsData->comm.serverIp, WstrToCstr (value) );
						retVal = stateStack.Pop();
					}
					else
					{
						errString = ESTR_ILLEGAL_IP_ADDRESS;
						errInfoString = EISTR_ILLEGAL_IP_ADDRESS;
						retVal = STATE_COMM_ERROR;
						stateStack.Push( STATE_COMM_NETWORK_ENTER_SERVER_IP );
					}
				default: break;
			}
			break;
			
		default:
			break;
	}

	return retVal;
}

/*
 * StateCommNetworkEnterSocket
 */

void
StateCommNetworkEnterSocket::
Enter()
{
	GuiReceiveMsg_t msg;

    if (socketSetType == SERVER_SOCKET) {
	    SetTopLabel( strings[ TSTR_ENTER_SERVER_SOCKET ] );
    } else {
	    SetTopLabel( strings[ TSTR_ENTER_CLIENT_SOCKET ] );
    }
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_IP;
	msg.data.middleEnterIp.text[0] = L'\0';
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIpData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
	
	memset( value, 0, sizeof( value ) );

}

state_t
StateCommNetworkEnterSocket::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_COMM_NETWORK_ENTER_SOCKET;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t msg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			if ( idata == -1 )		// 'C'
			{
				memset( value, 0, sizeof( value ) );
			}
			else if ( wcslen( value ) < (sizeof( value ) - 1) )
			{
				if ( (idata >= 0) && (idata <= 9) )
				{
					value[ wcslen( value ) ] = idata + '0';
				}
			}

			msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
			wcscpy( msg.data.middleEnterIp.text, value );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterIpData_t ) );

			break;
		
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
				case 2:		// Save
					if ( ValidateSocket( WstrToCstr (value) ) == true )
					{
                        if (socketSetType == SERVER_SOCKET) {
						    settingsData->comm.serverSocket = atoi( WstrToCstr (value));
                        } else {
						    settingsData->comm.clientSocket = atoi( WstrToCstr (value) );
                        }
						retVal = stateStack.Pop();
					}
					else
					{
						errString = ESTR_ILLEGAL_SOCKET;
						errInfoString = EISTR_ILLEGAL_SOCKET;
						retVal = STATE_COMM_ERROR;
						stateStack.Push( STATE_COMM_NETWORK_ENTER_SOCKET );
					}
					break;
				default: break;
			}
			break;
			
		default:
			break;
	}

	return retVal;
}

/*
 * StateCommAstmLayer
 */

void
StateCommAstmLayer::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleOptionInfoData_t *	data = &msg.data.middleOptionInfo;

	SetTopLabel( strings[ TSTR_ASTM_PHYSICAL_LAYER ] );

	msg.msgId = MSG_GUI_MIDDLE_OPTION_INFO;
	memset( data, 0, sizeof( GuiMiddleOptionInfoData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_USB ] );
	wcscpy( data->buttonText[1], strings[ MSTR_TCPIP ] );

	switch ( settingsData->comm.protocol )
	{
		case COMM_USB:		data->selectedButton = 0;	break;
		case COMM_TCPIP:	data->selectedButton = 1;	break;
		default:			data->selectedButton = 0;	break;
	}

	SendGuiMessage( &msg, sizeof( GuiMiddleOptionInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateCommAstmLayer::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_COMM_ASTM_LAYER;
	int			idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_TOGGLE:
			// idata has the button index, with the second byte set
			// to 0xF if the button is selected, or to 0 if it's deselected.
			switch ( idata )
			{
				case 0xF0:	// USB selected
					settingsData->comm.protocol = COMM_USB;
					break;
				case 0xF1:	// TCP/IP selected
				case 0x01:	// TCP/IP pressed when already selected
					settingsData->comm.protocol = COMM_TCPIP;
					retVal = STATE_COMM_TCPIP_SELECT;
					stateStack.Push( STATE_COMM_ASTM_LAYER );
					break;
				default:	break;
			}
			break;
		
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_ASTM_PHYSICAL_LAYER ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_COMM_ASTM_LAYER );
			break;
			
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = SaveConfiguration( stateStack.Home() );
					break;
			}
			break;

		default:
			break;
	}

	return retVal;
}


/*
 * StateCommHl7Layer
 */

#if 0
void
StateCommHl7Layer::
Enter()
{
	GuiReceiveMsg_t			    msg;
    GuiMiddleButtonsData_t *    data = &msg.data.middleButtons;

	SetTopLabel( strings[ TSTR_HL7_PHYSICAL_LAYER ] );

    msg.msgId = MSG_GUI_MIDDLE_BUTTONS;

    memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
    strcpy( data->buttonText[0], strings[ MSTR_HL7_CONFIG ] );
    strcpy( data->buttonText[1], strings[ MSTR_TCPIP ] );

    SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}
#else
void
StateCommHl7Layer::
Enter()
{
    GuiReceiveMsg_t                 msg;
    GuiMiddleButtonsInfoData_t *    data = &msg.data.middleButtonsInfo;

    SetTopLabel (strings [TSTR_HL7_PHYSICAL_LAYER]);

    msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

    memset (data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
    wcscpy( data->buttonText[0], strings[ MSTR_HL7_CONFIG ] );
    wcscpy( data->buttonText[1], strings[ MSTR_TCPIP ] );

    SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

    SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

#endif

state_t
StateCommHl7Layer::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_COMM_HL7_LAYER;
	int			idata = *(int *)data;

	switch ( msgId )
	{
        case MSG_GUI_OUT_BUTTON:
            switch ( idata )
            {
                case 0:     // HL7 protocol selected
                    stateStack.Push( STATE_COMM_HL7_LAYER );
                    retVal =    STATE_COMM_HL7_CONFIG_SELECT;
                break;

                case 1:     // TCPIP selected
                    stateStack.Push( STATE_COMM_HL7_LAYER );
                    retVal = STATE_COMM_TCPIP_SELECT;
                break;

                default:
                break;
            }
        break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = SaveConfiguration (stateStack.Home());
					break;
			}
			break;

         case MSG_GUI_OUT_INFO:
            infoText = strings[ ISTR_HL7_SETTINGS ];
            retVal = STATE_INFO;
            stateStack.Push( STATE_COMM_HL7_LAYER );
        break;


		default:
			break;
	}

	return retVal;
}


void
StateCommHl7ConfigSelect::
Enter ()
{
	GuiReceiveMsg_t				    msg;
    GuiMiddleListMultiData_t *      data = &msg.data.middleListMulti;
    int                             i, showItems, selectPage; 

    topItem = 0;

	SetTopLabel (strings [TSTR_HL7_CONFIG_SELECT]);


    msg.msgId = MSG_GUI_MIDDLE_LIST_MULTI;
    memset (data, 0, sizeof( GuiMiddleListMultiData_t));

    numItems = hl7FileList->tIndex;

    if (numItems > 5) {
        showItems = 5;
    } else {
        showItems = numItems;
    }
    // copy display text
    for (i = 0; i < showItems; i++) {
        wcscpy (data->items[i], CstrToWstr (hl7FileList->hl7FileName [i].displayText));
    }
    // set start up page
    page = 0;

    // check select item page
    selectPage = settingsData->hl7ConfigIdx / 5;
    // only show seletction on selected page
    if (selectPage == page) {
        data->isSelected[settingsData->hl7ConfigIdx % 5] = 1;
    } 
    data->hasUpDown = 1;

    SendGuiMessage (&msg, sizeof(GuiMiddleListData_t));

	SetBottomButtons (strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true);
}

state_t
StateCommHl7ConfigSelect::
ProcessMessage (int msgId, char * data)
{
	state_t 	    retVal = STATE_COMM_HL7_CONFIG_SELECT;
	int			    idata = *(int *)data;
    bool            doUpdate = false;
    unsigned char   bitSet = 0;
    int             showItems, i, maxShow, selectPage;

	switch (msgId) {
        
         case MSG_GUI_OUT_UP:
            if (topItem > 0) {
                topItem -= 5;
                if (page > 0) {
                    page--;
                }
                if (topItem < 0) {
                    topItem = 0;
                    page = 0;
                }
                doUpdate = true;
            }
            break;

        case MSG_GUI_OUT_DOWN:
            if (topItem + 5 < numItems) {
                topItem += 5;
                doUpdate = true;
                page++;
            }
            break;

        case MSG_GUI_OUT_LIST:
            // mask previous setting bits;
            if (idata != 0) {
                bitSet = (1 << (settingsData->hl7ConfigIdx % 5));
                // two pages select same item, should not mask the bit
                if (bitSet != idata) {
                    idata &= (~bitSet);
                }
            }
            // set select index
            switch  (idata) {

                case 0x01: 
                    hl7SetFlag = 0;
                    settingsData->hl7ConfigIdx = 0 + (page * 5);
                break;

                case 0x02:   
                    hl7SetFlag = 1;
                    settingsData->hl7ConfigIdx = 1  + (page * 5);
                break;

                case 0x04: 
                    hl7SetFlag = 2;
                    settingsData->hl7ConfigIdx = 2  + (page * 5);
                break;

                case 0x08: 
                    hl7SetFlag = 3;
                    settingsData->hl7ConfigIdx = 3  + (page * 5);
                break;

                case 0x10:
                    hl7SetFlag = 4;
                    settingsData->hl7ConfigIdx = 4  + (page * 5);
                break;

                default:
                break;
            }
            break;
			
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = SaveConfiguration (stateStack.Home());
                    ParseHL7ConfigurationByIndex (hl7Configuration, settingsData->hl7ConfigIdx);
					break;
			}
			break;

		default:
			break;
	}

    if (1) {
        GuiReceiveMsg_t             msg;
        GuiMiddleListMultiData_t *  data = &msg.data.middleListMulti;

        msg.msgId = MSG_GUI_MIDDLE_LIST_MULTI;
        memset (data, 0, sizeof( GuiMiddleListMultiData_t));

        showItems = (page * 5);
        if (showItems > numItems) {
            maxShow = numItems;
        } else {
            maxShow = showItems + 5;
        }

        for (i = 0; showItems < maxShow; showItems++, i++) {
             wcscpy (data->items[i], CstrToWstr (hl7FileList->hl7FileName [showItems].displayText));
        }
        
        //find out selected page
        selectPage = settingsData->hl7ConfigIdx / 5;
        // in the case the current page is the selected page, display selected item
        if (selectPage == page) {
            data->isSelected[settingsData->hl7ConfigIdx % 5] = 1;
        } 
        data->hasUpDown = 1;

        SendGuiMessage (&msg, sizeof(GuiMiddleListData_t));
    }

	return retVal;
}

/*
 * StateCommTcpIpSelect
 */

void
StateCommTcpIpSelect::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_TCPIP ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_CHANGE_SETTINGS ] );
	wcscpy( data->buttonText[1], strings[ MSTR_CHECK_SETTINGS ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StateCommTcpIpSelect::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_COMM_TCPIP_SELECT;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Change Settings
					//retVal = STATE_COMM_STATIC_TCPIP_SETTINGS;
					//retVal = STATE_COMM_IP_ADDRESS_MODE_SELECT;
                      retVal = STATE_COMM_TCPIP_SETTINGS;
					stateStack.Push( STATE_COMM_TCPIP_SELECT );
					break;
				case 1:		// Check Settings
					retVal = SaveConfiguration( STATE_COMM_NETWORK_CHECK, false );
					stateStack.Push( STATE_COMM_TCPIP_SELECT );
					break;
				default:
					break;
			}
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = SaveConfiguration( stateStack.Home() );
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_TCPIP_SETTINGS ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_COMM_TCPIP_SELECT );
			break;
		default:
			break;
	}
	
	return retVal;
}

// IP address selection
void
StateCommIpAddressModeSelect::
Enter (void)
{
    GuiReceiveMsg_t             msg;
    GuiMiddleOptionInfoData_t   *data = &msg.data.middleOptionInfo;
    configChanged = 0;

	SetTopLabel (strings [TSTR_TCPIP_ADDRESS_TYPE]);

    msg.msgId = MSG_GUI_MIDDLE_OPTION_INFO;

    memset (data, 0, sizeof (GuiMiddleOptionInfoData_t));
    wcscpy (data->buttonText[0], strings [MSTR_STATIC_IP]);
    wcscpy (data->buttonText[1], strings [MSTR_DHCP]);
    if (settingsData->comm.dhcpEnabled) {
        data->selectedButton = 1;
    } else {
        data->selectedButton = 0;
    }
    SendGuiMessage (&msg, sizeof( GuiMiddleOptionInfoData_t));

    SetBottomButtons (strings[BSTR_BACK], false, strings[BSTR_HOME], true);

}

state_t
StateCommIpAddressModeSelect::
ProcessMessage (int msgId, char *data)
{
    int retVal = STATE_COMM_IP_ADDRESS_MODE_SELECT;
    int idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_TOGGLE:
            switch (idata) {
                case 0xF1:      //enable dhcp
                case 0:
                    if (settingsData->comm.dhcpEnabled == false) {
                        retVal = STATE_COMM_NETWORK_SETTING;
                        wcscpy (displayText, strings [MSTR_CHANGE_NET_TYPE]);
                        configChanged = DHCP_ENABLE;
                    } 
                break;

                case 0xF0:      //static ip address
                case 1:
                    if (settingsData->comm.dhcpEnabled == true) {
                       retVal = STATE_COMM_NETWORK_SETTING;
                       wcscpy (displayText, strings [MSTR_CHANGE_NET_TYPE]);
                       configChanged = DHCP_DISABLE;
                    }
                break;

                default:
                break;
            }
        break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:     // Back
                    retVal = stateStack.Pop();
                break;

                case 1:     // Home
                    retVal = SaveConfiguration (stateStack.Home());
                break;

                default:
                break;
            }
        break;

         case MSG_GUI_OUT_INFO:
            infoText = strings[ ISTR_NETWORK_TYPE ];
            retVal = STATE_INFO;
            stateStack.Push (STATE_COMM_IP_ADDRESS_MODE_SELECT); 
         break;

        default:
        break;
    }
    return retVal;
}

// configure network
void
StateCommNetworkSetting::
Enter (void)
{
    GuiReceiveMsg_t     msg;
    SettingsMsg_t       sMsg;

    SetTopLabel (0);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, displayText);
    SendGuiMessage (&msg, sizeof (GuiMiddleMessageData_t ));
    SetBottomButtons ();

    // configurate network driver
    sMsg.msgId = MSG_SETTINGS_IP_ADDRESS_SET_COMPLETE;
    // feed back configuratin status
    sMsg.data = ConfigureNetwork ();
    // wait 1 second to boot up network
    Sleep (1000);

    SendSettingsMessage (&sMsg, sizeof (SettingsMsg_t));
}

// ip setting configuration done
state_t
StateCommNetworkSetting::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_COMM_NETWORK_SETTING;

    switch (msgId) {
        case MSG_SETTINGS_IP_ADDRESS_SET_COMPLETE:
            if (*data == 0) {
                retVal = stateStack.Pop();
            } else {
                errNum = SERR_ETHERNET_PORT;
                retVal = STATE_ERROR;
            }
            // reset flag
            configChanged = 0;
        break;

        default:
        break;
    }

    return retVal;
}

// configurate address mode
int
StateCommNetworkSetting::
ConfigureNetwork (void)
{
    int ret = 0;
    int len;

    if (configChanged & DHCP_DISABLE) {
        // disable dhcp
        if (SetDhcp (systemData->cpuBoard, false, settingsData->comm.hostName) == 0) {
            settingsData->comm.dhcpEnabled = false;
            GetDnsServerAddress (systemData->cpuBoard, settingsData->comm.dnsServer, settingsData->comm.dhcpEnabled);
            WriteSettings (settingsData);
            //enable static ip address
            if (SetStaticIp (systemData->cpuBoard, settingsData) != 0) {
                ret = 1;
            }
        } else {
            ret = 1;
        }
    }
    if (configChanged & DHCP_ENABLE) {
        // change default host name to be analyzer serial number
        if (strcmp (settingsData->comm.hostName, "Analyzer123") == 0) {
            memset (settingsData->comm.hostName, 0, sizeof (settingsData->comm.hostName));
            len = strlen (factoryData->instrumentSerialNumber);
            strncpy (settingsData->comm.hostName,  factoryData->instrumentSerialNumber + (len - 6), 6);
        }
         // enable dhcp
        if (SetDhcp (systemData->cpuBoard, true, settingsData->comm.hostName) == 0) {
            settingsData->comm.dhcpEnabled = true;
            GetDnsServerAddress (systemData->cpuBoard, settingsData->comm.dnsServer, settingsData->comm.dhcpEnabled);
            //update settings
            WriteSettings (settingsData);
        } else {
            ret =1;
        }
    }
    if (configChanged & IP_CHANGED) {
        if (SetStaticIp (systemData->cpuBoard, settingsData) == 0) {
            WriteSettings (settingsData);
        } else {
            ret =1;
        }
    }
    if (configChanged & HOST_NAME_CHANGED) {
        if (SetHostName (settingsData->comm.hostName) == 0) {
            WriteSettings (settingsData);
        } else {
            ret =1;
        }
    }
    return ret;
}

// Set TCPIP Parameters
void
StateCommTcpIpSettings::
Enter (void)
{
    GuiReceiveMsg_t             msg;
    GuiMiddleListData_t *       data = &msg.data.middleList;
    int                         i;

    // get ethernetport info from adapter directly for DHCP 
    GetEthernetPortInfo (&ethernetPortInfo);

    SetTopLabel (strings [TSTR_TCPIP_SETTINGS]);


    wcscpy (label[0], strings [MSTR_ADDRESS_TYPE]);
    wcscpy (label[1], strings [MSTR_ANALYZER_IP]);
    wcscpy (label[2], strings [MSTR_IP_MASK]);
    wcscpy (label[3], strings [MSTR_GATEWAY_IP]);

    if (settingsData->comm.dhcpEnabled) {
        wcscpy (value[0], strings [MSTR_DHCP]);
        wcscpy (value[1], CstrToWstr (ethernetPortInfo.analyzerIp));
        wcscpy (value[2], CstrToWstr (ethernetPortInfo.ipMask));
        wcscpy (value[3], CstrToWstr (ethernetPortInfo.gateway));
    } else {
        wcscpy (value[0], strings [MSTR_STATIC_IP]);
        wcscpy (value[1], CstrToWstr (settingsData->comm.analyzerIp));
        wcscpy (value[2], CstrToWstr (settingsData->comm.ipMask));
        wcscpy (value[3], CstrToWstr (settingsData->comm.gateway));
    }
    wcscpy (label[4], strings [MSTR_SERVER_IP]);
    wcscpy (value[4], CstrToWstr (settingsData->comm.serverIp));
    wcscpy (label[5], strings[MSTR_SERVER_SOCKET]);
    wsprintf (value[5], L"%d", settingsData->comm.serverSocket);
    wcscpy (label[6], strings [MSTR_CLIENT_SOCKET]);
    if (settingsData->comm.commInterface == COMM_HL7) {
        wsprintf (value[6], L"%d", settingsData->comm.clientSocket);
    } else {
        // ASTM, server and client socket should be same value
        wsprintf (value[6], L"%d", settingsData->comm.serverSocket);
    }
    wcscpy (label[7], strings [MSTR_ANALYZER_NAME]);
    wcscpy (value[7], CstrToWstr (ethernetPortInfo.hostName));
    wcscpy (label[8], strings [MSTR_MAC_ADDRESS]);
    wcscpy (value[8], CstrToWstr (ethernetPortInfo.macAddress));

    topItem = 0;
    numItems = 9;

    msg.msgId = MSG_GUI_MIDDLE_LIST_SETTINGS;

    memset (data, 0, sizeof( GuiMiddleListData_t));
    for (i = 0; i < 5 && i < numItems; i++) {
        wcscpy( data->items[i][0], label[i] );
        wcscpy( data->items[i][1], value[i] );
    }
    data->isSelectable = 1;
    data->hasUpDown = 1;

    SendGuiMessage (&msg, sizeof( GuiMiddleListData_t));

    SetBottomButtons (strings [BSTR_BACK], false, strings [BSTR_HOME], true);
}

// Set TCPIP Parameters
state_t
StateCommTcpIpSettings::
ProcessMessage (int msgId, char *data)
{

    state_t retVal = STATE_COMM_TCPIP_SETTINGS;
    int     idata = *(int *)data;
    bool    doUpdate = false;

    switch (msgId) {

        case MSG_GUI_OUT_UP:
            if (topItem > 0) {
                topItem -= 5;
                if (topItem < 0) {
                    topItem = 0;
                }
                doUpdate = true;
            }
            break;

        case MSG_GUI_OUT_DOWN:
            if (topItem + 5 <numItems) {
                topItem += 5;
                doUpdate = true;
            }
            break;

        case MSG_GUI_OUT_LIST:
            switch (topItem + idata) {

                case 0: // IP address type selection
                    stateStack.Push (STATE_COMM_TCPIP_SETTINGS);
                    retVal = STATE_COMM_IP_ADDRESS_MODE_SELECT;
                break;

                case 1: // Analyzer IP
                    if (settingsData->comm.dhcpEnabled == 0) {
                        stateStack.Push (STATE_COMM_TCPIP_SETTINGS);
                        retVal = STATE_COMM_NETWORK_ENTER_ANALYZER_IP;
                    }
                break;

                case 2: // IP Mask
                    if (settingsData->comm.dhcpEnabled == 0) {
                        stateStack.Push (STATE_COMM_TCPIP_SETTINGS);
                        retVal = STATE_COMM_NETWORK_ENTER_IP_MASK;
                    }
                break;

                case 3: // Gateway IP
                    if (settingsData->comm.dhcpEnabled == 0) {
                        stateStack.Push (STATE_COMM_TCPIP_SETTINGS);
                        retVal = STATE_COMM_NETWORK_ENTER_GATEWAY;
                    }
                break;

                case 4: // Server IP
                    stateStack.Push (STATE_COMM_TCPIP_SETTINGS);
                    retVal = STATE_COMM_NETWORK_ENTER_SERVER_IP;
                break;

                case 5: // Server Socket
                    socketSetType = SERVER_SOCKET;
                    stateStack.Push (STATE_COMM_TCPIP_SETTINGS);
                    retVal = STATE_COMM_NETWORK_ENTER_SOCKET;
                break;

                case 6: // Client Socket
                    // only HL7 allowed to configuration client socket
                    if (settingsData->comm.commInterface == COMM_HL7) {
                        socketSetType = CLIENT_SOCKET;
                        stateStack.Push (STATE_COMM_TCPIP_SETTINGS);
                        retVal = STATE_COMM_NETWORK_ENTER_SOCKET;
                    }
                break;

                case 7: // Analyzer host name 
                    stateStack.Push (STATE_COMM_TCPIP_SETTINGS);
                    retVal = STATE_COMM_NETWORK_ENTER_HOST_NAME;
                break;

                default:
                break;
            }
            break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:

            switch (idata) {
                case 0:     // Back
                    retVal = stateStack.Pop();
                break;

                case 1:     // Home
                    retVal = SaveConfiguration (stateStack.Home());
                break;

                default:
                break;
            }

            break;

        default:
        break;
    }

    if (doUpdate) {
        GuiReceiveMsg_t         msg;
        GuiMiddleListData_t *   data = &msg.data.middleList;
        int                     i, j;

        msg.msgId = MSG_GUI_UPDATE_LIST_SETTINGS;
        memset( data, 0, sizeof( GuiMiddleListData_t ) );

        for (i = 0, j = topItem; i < 5 && j < numItems; i++, j++) {
            wcscpy (data->items[i][0], label[j]);
            wcscpy (data->items[i][1], value[j]);
        }

        data->isSelectable = 1;
        data->hasUpDown = 1;

        SendGuiMessage (&msg, sizeof(GuiMiddleListData_t));
    }
    return retVal;
}


// enter analzyer host name
void
StateCommEnterHostname::
Enter()
{
    GuiReceiveMsg_t msg;

    SetTopLabel (strings [TSTR_ENTER_ANALYZER_NET_NAME]);

    msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
    msg.data.middleEnterId.isPassword = 0;
    wcscpy( msg.data.middleEnterId.initialValue, L"" );
    SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );

    SetBottomButtons (strings [BSTR_BACK], false, strings [BSTR_HOME], false, strings [BSTR_SAVE], true);

    memset (value, 0, sizeof(value));
}

state_t
StateCommEnterHostname::
ProcessMessage (int msgId, char * data)
{
    state_t         retVal = STATE_COMM_NETWORK_ENTER_HOST_NAME;
    int             idata = *(int *)data;
    char *          sdata = (char *)data;

    GuiReceiveMsg_t msg;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:     // Back
                    retVal = stateStack.Pop();
                break;

                case 1:     // Home
                    retVal = SaveConfiguration (stateStack.Home());
                break;

                case 2:     // Save, get ID text
                    msg.msgId = MSG_GUI_GET_ID_TEXT;
                    SendGuiMessage( &msg, 0 );
                break;

                default: 
                break;
            }
            break;

        case MSG_GUI_OUT_ACTIVATE:  // Same as Done.
            msg.msgId = MSG_GUI_GET_ID_TEXT;
            SendGuiMessage (&msg, 0);
        break;

        case MSG_GUI_OUT_ID_TEXT:
            //update host name
            strcpy (settingsData->comm.hostName, sdata);
            //update driver host name
            LogDebug ("set new hostname:%s", settingsData->comm.hostName);
            retVal = STATE_COMM_NETWORK_SETTING;
            wcscpy (displayText, strings [MSTR_CHANGE_NET_NAME]);
            configChanged = HOST_NAME_CHANGED;
            break;

        default:
            break;
    }
    return retVal;
}

/*
 * Class StateCommError
 */

void
StateCommError::
Enter()
{
	GuiReceiveMsg_t		guiMsg;
	
	guiMsg.msgId = MSG_GUI_WIN_ERROR;

	wcscpy( guiMsg.data.winError.topLabel, strings[ TSTR_ERROR ] );
	wcscpy( guiMsg.data.winError.message, strings[ errString ] );
	wcscpy( guiMsg.data.winError.buttonText, strings[ BSTR_CONTINUE ] );
	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
}

int
StateCommError::
ProcessMessage( int msgId, char * data )
{
	int		retVal = STATE_COMM_ERROR;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Continue
			retVal = stateStack.Pop();
			break;
	
		case MSG_GUI_OUT_INFO:
			infoText = strings[ errInfoString ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_COMM_ERROR );
			break;
				
		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateCommSetProtocol
 */

void
StateCommSetProtocol::
Enter()
{
    GuiReceiveMsg_t             msg;
    GuiMiddleButtonsData_t *    data = &msg.data.middleButtons;
    HANDLE                       hSearch;
    SerialCommMsg_t             serialCommMsg;
    wchar_t                     wFilename [128];
    WIN32_FIND_DATA             fileData;

    // find out thumb driver fuse xml file
    wsprintf (wFilename, L"%S", FUSE_SETTINGS_FILE_NAME);
    hSearch =  FindFirstFile (wFilename, &fileData);

    // If this is a VS2, and there's a thumb drive, and it's got the Fuse settings file on it,
    // display the options.
    if ( (factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE) &&
         MountThumbDrive() && (hSearch != INVALID_HANDLE_VALUE) )
    {
        // We need this so we know, when we go Back from the next state, whether we're coming back to
        // this state or not. If not, we need to restart the Comm program.
        displayFuseOption = true;

        SetTopLabel( strings[ TSTR_SET_PROTOCOL ] );

        msg.msgId = MSG_GUI_MIDDLE_BUTTONS;
        memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
        wcscpy ( data->buttonText[0], strings[ MSTR_LOAD_FUSE_SETTINGS ] );
        wcscpy ( data->buttonText[1], strings[ MSTR_MANUAL_SETTINGS ] );
   
        SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );
   
        SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
    }

    // No Fuse settings file, so set a trigger to go on to Select Protocol.
    else
    {
        SendSettingsMessage( MSG_SETTINGS_TRIGGER );
        displayFuseOption = false;
    }

    // Save the current configuration for use when it's time to start/restart ASTM, etc.
    // Kill the current Comm program. We'll restart the appropriate one when we leave.
    //kill( systemData->commPid, SIGKILL );
    DWORD exitCode;
    HANDLE process = OpenProcess (0,FALSE, systemData->commPid);
    if (process != NULL) {
        // set networksetting flag to be true
        systemData->networkSetting = true;
        if ((settingsData->comm.commInterface != COMM_ASTM) &&(settingsData->comm.commInterface != COMM_HL7)) {
            // close serial port before kill process
            serialCommMsg.msgId = MSG_SERIAL_COMM_CLOSE_PORT;
            SendSerialCommMessage (&serialCommMsg, sizeof(SendReportData_t));
            Sleep (1);
        }
        //LogDebug ("Kill process commPid =%d" , systemData->commPid);
        GetExitCodeProcess(process, &exitCode);
        TerminateProcess(process, exitCode);
        CloseHandle(process);
    }
    systemData->commPid = -1;


}

state_t
StateCommSetProtocol::
ProcessMessage( int msgId, char * data )
{
    state_t retVal = STATE_COMM_SET_PROTOCOL;
    int     idata = *(int *)data;
    char            dhcpEnable;

    switch ( msgId )
    {
        case MSG_SETTINGS_TRIGGER:
            retVal = STATE_COMM_SELECT_PROTOCOL;
            break;

        case MSG_GUI_OUT_BUTTON:
            // idata has the button index, with the second byte set
            // to 0xF if the button is selected, or to 0 if it's deselected.
            switch ( idata )
            {
                case 0:     // Load Fuse Settings
                    // remeber current dhcp status
                    dhcpEnable = settingsData->comm.dhcpEnabled;
                    LoadFuseSettings();
                     // enable dhcp
                    if (settingsData->comm.dhcpEnabled !=  dhcpEnable) {
                        if (settingsData->comm.dhcpEnabled) {
                            //enable dhcp
                            if (SetDhcp (systemData->cpuBoard, true, settingsData->comm.hostName) == 0) {
                                settingsData->comm.dhcpEnabled = true;
                                GetDnsServerAddress (systemData->cpuBoard, settingsData->comm.dnsServer, settingsData->comm.dhcpEnabled);
                                LogDebug ("enable DHCP ...");
                                //update settings
                                //WriteSettings (settingsData);
                            } else {
                                LogDebug ("Fail to enable DHCP");
                            }
                        } else {
                            // disable dhcp
                            if (SetDhcp (systemData->cpuBoard, false, settingsData->comm.hostName) == 0) {
                                LogDebug ("Disable DHCP ...");
                                GetDnsServerAddress (systemData->cpuBoard, settingsData->comm.dnsServer, settingsData->comm.dhcpEnabled);
                                //WriteSettings (settingsData);
                                //enable static ip address
                                if (SetStaticIp (systemData->cpuBoard, settingsData) != 0) {
                                    LogDebug ("Fail to set static IP address");
                                }
                            } else {
                                LogDebug ("Fail to disable DHCP");
                            }

                        }
                    }
                    // in the case of socket number and server address changed, just saved to new values 
                    // the new port number and server shall be implemented when ASTM or HL7 process started
                    retVal = SaveConfiguration (stateStack.Home());
                    break;
                case 1:     // Manual Settings
                    retVal = STATE_COMM_SELECT_PROTOCOL;
                    stateStack.Push( STATE_COMM_SET_PROTOCOL );
                    break;
                default:
                    break;
            }
            break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch ( idata )
            {
                case 0:     // Back
                    retVal = SaveConfiguration( stateStack.Pop() );
                    break;
                case 1:     // Home
                    retVal = SaveConfiguration( stateStack.Home() );
                    break;
            }
            break;

        default:
            break;
    }

    return retVal;
}



/*
 * class StateCommSelectProtocol
 */
 
void
StateCommSelectProtocol::
Enter()
{
	GuiReceiveMsg_t				msg;
    SerialCommMsg_t             serialCommMsg;
	GuiMiddleOptionInfoData_t *	data = &msg.data.middleOptionInfo;

	SetTopLabel( strings[ TSTR_SELECT_PROTOCOL ] );

	msg.msgId = MSG_GUI_MIDDLE_OPTION_INFO;
	memset( data, 0, sizeof( GuiMiddleOptionInfoData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_NONE ] );
	wcscpy( data->buttonText[1], strings[ MSTR_ASCII_TEXT ] );
	wcscpy( data->buttonText[2], strings[ MSTR_ASTM_1394_7 ] );
	wcscpy( data->buttonText[3], strings[ MSTR_XML ] );
	wcscpy( data->buttonText[4], strings[ MSTR_HL7 ] );

	switch ( settingsData->comm.commInterface )
	{
		case COMM_NONE:		
            data->selectedButton = 0;	
        break;
		case COMM_ASCII:	
            data->selectedButton = 1;	
        break;
		case COMM_ASTM:		
            data->selectedButton = 2;	
        break;
		case COMM_XML:		
            data->selectedButton = 3;	
        break;
		case COMM_HL7:		
            data->selectedButton = 4;	
        break;
		default:			
            data->selectedButton = 0;	
        break;
	}

	SendGuiMessage( &msg, sizeof( GuiMiddleOptionInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
	
	// Save the current configuration for use when it's time to start/restart ASTM, etc.
	currentCommInterface = settingsData->comm.commInterface;
	currentCommProtocol = settingsData->comm.protocol;
	strcpy( currentAnalyzerIp, settingsData->comm.analyzerIp );
	strcpy( currentGateway, settingsData->comm.gateway );
	strcpy( currentIpMask, settingsData->comm.ipMask );
	strcpy( currentServerIp, settingsData->comm.serverIp );
	currentServerSocket = settingsData->comm.serverSocket;
	currentClientSocket = settingsData->comm.clientSocket;
}

state_t
StateCommSelectProtocol::
ProcessMessage( int msgId, char * data )
{
	state_t 				retVal = STATE_COMM_SELECT_PROTOCOL;
	//state_t 				retVal = STATE_COMM_SELECT;
	int						idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_TOGGLE:
			// idata has the button index, with the second byte set
			// to 0xF if the button is selected, or to 0 if it's deselected.
			switch ( idata )
			{
				case 0xF0:		
                    settingsData->comm.commInterface = COMM_NONE;	
                    break;
				case 0xF1:		
                    settingsData->comm.commInterface = COMM_ASCII;	
                    break;
				case 0xF2:	// ASTM selected
				case 0x2:	// ASTM pressed when already selected
					settingsData->comm.commInterface = COMM_ASTM;
					retVal = STATE_COMM_ASTM_LAYER;
					stateStack.Push( STATE_COMM_SELECT_PROTOCOL );
					break;
				case 0xF3:		
                    settingsData->comm.commInterface = COMM_XML;	
                    break;
                case 0xF4:      
                case 0x4:
                    settingsData->comm.commInterface = COMM_HL7;    
                    retVal = STATE_COMM_HL7_LAYER;
                    settingsData->comm.protocol = COMM_TCPIP;
					stateStack.Push (STATE_COMM_SELECT_PROTOCOL);
                    break;
				default:	
                    break;
			}
			break;

		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_COMM_PROTOCOL ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_COMM_SELECT_PROTOCOL );
			break;
			
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
                    if (displayFuseOption == true) {
                        LogDebug ("back displayFuseOption = true");
                        // Fuse setting is loaded and saved, the process has been started
                        retVal = stateStack.Pop();
                    } else {
                        LogDebug ("back displayFuseOption = false");
                        retVal = SaveConfiguration (stateStack.Pop());
                    }
					break;
				case 1:		// Home
					retVal = SaveConfiguration( stateStack.Home() );
					break;
			}
			break;

		default:
			break;
	}
	
	return retVal;
}
