/*
 * FILENAME
 * 	CommStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine set communication
 * 	parameters states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "CommStates.h"
#include "SettingsMessages.h"
#include "ParseSettings.h"
#include "CfgErr.h"
#include "LogErr.h"
#include "GetThumbKeyMode.h"

CommInterface_t	StateComm::currentCommInterface;
CommProtocol_t	StateComm::currentCommProtocol;
char			StateComm::currentAnalyzerIp[30];
char			StateComm::currentGateway[30];
char			StateComm::currentIpMask[30];
char			StateComm::currentServerIp[30];
int				StateComm::currentServerSocket;
StringId_t		StateComm::errString = STR_BLANK;
StringId_t		StateComm::errInfoString = STR_BLANK;
bool			StateComm::analyzerIpChanged = false;
bool			StateComm::gatewayChanged = false;
bool			StateComm::ipMaskChanged = false;

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
SaveConfiguration( state_t returnState, bool _doStartComm )
{
	CfgError_t	cfgErr;
	static bool	doStartComm;

	if ( ConfigurationChanged() )
	{
		cfgErr = WriteSettings( settingsData );
		if ( cfgErr != CFG_ERR_NONE )
		{
			errNum = SERR_FILE_WRITE;
			returnState = STATE_ERROR;
		}
	
		doStartComm = _doStartComm;
		ThreadCreate( 0, ConfigurationThread, (void *)&doStartComm, 0 );
		
		// Update the saved values for Server IP and Server Socket.
		strcpy( currentServerIp, settingsData->comm.serverIp );
		currentServerSocket = settingsData->comm.serverSocket;
		
		// If we're not in ASTM now, we may have come from there, so recheck the thumb key in case
		// we need to go back to Research mode.
		if ( _doStartComm && (settingsData->comm.interface != COMM_ASTM) )
		{
			GetThumbKeyMode( systemData );
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
	bool	commInterfaceChanged = currentCommInterface != settingsData->comm.interface;;
	bool	commProtocolChanged = currentCommProtocol != settingsData->comm.protocol;
	bool	serverIpChanged = currentServerIp != settingsData->comm.serverIp;
	bool	serverSocketChanged = currentServerSocket != settingsData->comm.serverSocket;

	// These comparisons are saved for use by ConfigurationThread.
	analyzerIpChanged = strcmp( currentAnalyzerIp, settingsData->comm.analyzerIp ) != 0;
	gatewayChanged = strcmp( currentGateway, settingsData->comm.gateway ) != 0;
	ipMaskChanged = strcmp( currentAnalyzerIp, settingsData->comm.analyzerIp ) != 0;

	return commInterfaceChanged || commProtocolChanged || serverIpChanged || serverSocketChanged ||
		   analyzerIpChanged || gatewayChanged || ipMaskChanged;
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
	int		pid;

	// If either the analyzer IP or the IP mask changed, we need to run ifconfig to set the new parameters
	// and also restart inetd and qconn.
	if ( forceUpdate || analyzerIpChanged || ipMaskChanged )
	{
		// Run ifconfig
		if ( settingsData->comm.ipMask[0] == '\0' )
		{
			pid = spawnlp( P_WAIT, "ifconfig", "ifconfig", "en0", settingsData->comm.analyzerIp, "up", 0 );
		}
		else
		{
			pid = spawnlp( P_WAIT, "ifconfig", "ifconfig", "en0", settingsData->comm.analyzerIp,
						   "netmask", settingsData->comm.ipMask, "up", 0 );
		}
		if ( pid == -1 )
		{
			LogError( "failed to run ifconfig" );
		}
		else
		{
			// Restart inetd.
			pid = spawnlp( P_WAIT, "slay", "slay", "-f", "-Q", "inetd", 0 );
			if ( pid == -1 )
			{
				LogError( "failed to run slay inetd" );
			}
			else
			{
				pid = spawnlp( P_WAIT, "inetd", "inetd", 0 );
				if ( pid == -1 )
				{
					LogError( "failed to run inetd" );
				}
			}

			// Restart qconn.
			pid = spawnlp( P_WAIT, "slay", "slay", "-f", "-Q", "qconn", 0 );
			if ( pid == -1 )
			{
				LogError( "failed to run slay qconn" );
			}
			else
			{
				pid = spawnlp( P_WAIT, "qconn", "qconn", 0 );
				if ( pid == -1 )
				{
					LogError( "failed to run qconn" );
				}
			}
		}
		
		// Update the saved values for Analyzer IP and IP Mask.
		strcpy( currentAnalyzerIp, settingsData->comm.analyzerIp );
		strcpy( currentIpMask, settingsData->comm.ipMask );
	}
	
	// If the gateway address changed, delete the old gateway and install the new one.
	if ( forceUpdate || gatewayChanged )
	{
		pid = spawnlp( P_WAIT, "route", "route", "delete", "default", 0 );
		if ( pid == -1 )
		{
			LogError( "failed to run route" );
		}
		else if ( settingsData->comm.gateway[0] != '\0' )
		{
			pid = spawnlp( P_WAIT, "route", "route", "add", "default", settingsData->comm.gateway, 0 );
			if ( pid == -1 )
			{
				LogError( "failed to run route" );
			}
		}

		// Update the saved value for Gateway IP.		
		strcpy( currentGateway, settingsData->comm.gateway );
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

	// Set the Ethernet port TCP/IP parameters.
	SetEthernet();
	
	// Start ASTM or SerialComm if the flag is true (if this wasn't called from CheckASTM).
	if ( doStartComm )
	{
		// There shouldn't be a Comm process already running, but if there is we'll kill it first.
		if ( systemData->commPid != -1 )
		{
			LogError( "attempt to start SerialComm/ASTM when a process was already running" );
			kill( systemData->commPid, SIGKILL );
		}

		// Start/restart SerialComm if the interface isn't ASTM.
		if ( settingsData->comm.interface != COMM_ASTM )
		{
			systemData->commPid = spawnlp( P_NOWAIT, SERIALCOMM_PROGRAM, SERIALCOMM_PROGRAM, 0 );
			if ( systemData->commPid == -1 )
			{
				LogError( "failed to start SerialComm" );
			}
		}
	
		// Start/restart ASTM.
		else
		{
			systemData->commPid = spawnlp( P_NOWAIT, ASTM_PROGRAM, ASTM_PROGRAM, 0 );
			if ( systemData->commPid == -1 )
			{
				LogError( "failed to start ASTM" );
			}

			// If we were in Research mode, we need to exit it.
			systemData->inResearchMode = false;
		}
	}
	
	// Update the saved Comm Interface and Comm Protocol.
	currentCommInterface = settingsData->comm.interface;
	currentCommProtocol = settingsData->comm.protocol;
	
	return 0;
}

void
StateComm::
WriteSetIpAddress()
{
	FILE *	fp;

	fp = fopen( "/SetIpAddress", "w" );
	if ( fp != 0 )
	{
		fprintf( fp, "#!/proc/boot/ksh\n" ); 
		if ( settingsData->comm.ipMask[0] == '\0' )
		{
			fprintf( fp, "ifconfig en0 %s up\n", settingsData->comm.analyzerIp );
		}
		else
		{
			fprintf( fp, "ifconfig en0 %s netmask %s up\n", settingsData->comm.analyzerIp, settingsData->comm.ipMask );
		}
		if ( settingsData->comm.gateway[0] != '\0' )
		{
			fprintf( fp, "route add default %s\n", settingsData->comm.gateway );
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
	ipCopy = strdup( ip );
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


/*
 * StateCommAstmCheck
 */

void
StateCommAstmCheck::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_CHECK_TCPIP_SETTINGS ] );

	strcpy( msg.data.middleViewResults.text, strings[ MSTR_TESTING_CONNECTION ] );

	msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
	SendGuiMessage( &msg, sizeof( GuiMiddleViewResultsData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );

	// Send a trigger message so we move on to ProcessMethod().
	SendSettingsMessage( MSG_SETTINGS_TRIGGER );
}

state_t
StateCommAstmCheck::
ProcessMessage( int msgId, char * data )
{
	state_t 			retVal = STATE_COMM_ASTM_CHECK;
	int					idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t		msg;
	int					clientSocket;
	int					status = 0;
	struct sockaddr_in	serverSocket = { 0 };
	bool				socketError;
	
	switch ( msgId )
	{
		case MSG_SETTINGS_TRIGGER:
			socketError = false;

			clientSocket = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
			if ( clientSocket == -1 )
			{
				LogError( "socket() failed" );
				socketError = true;
			}
			else
			{			
				status = inet_aton( settingsData->comm.serverIp, &serverSocket.sin_addr );
				if ( status == 0 )
				{
					LogError( "inet_aton() failed" );
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
					close( clientSocket );
				}
			}
			
			if ( socketError )
			{
				strcpy( msg.data.middleViewResults.text, strings[ MSTR_CONNECTION_FAILED ] );
			}
			else
			{
				strcpy( msg.data.middleViewResults.text, strings[ MSTR_CONNECTION_SUCCESSFUL ] );
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
StateCommAstmEnterAnalyzerIp::
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
StateCommAstmEnterAnalyzerIp::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_COMM_ASTM_ENTER_ANALYZER_IP;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t msg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			if ( idata == -1 )		// 'C'
			{
				memset( value, 0, sizeof( value ) );
			}
			else if ( strlen( value ) < (sizeof( value ) - 1) )
			{
				if ( (idata >= 0) && (idata <= 9) )
				{
					value[ strlen( value ) ] = idata + '0';
				}
				else if ( idata == -2 )		// '.'
				{
					value[ strlen( value ) ] = '.';
				}
			}

			msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
			strcpy( msg.data.middleEnterIp.text, value );
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
					if ( ValidateIp( value ) == true )
					{
						strcpy( settingsData->comm.analyzerIp, value );
						retVal = stateStack.Pop();
					}
					else
					{
						errString = ESTR_ILLEGAL_IP_ADDRESS;
						errInfoString = EISTR_ILLEGAL_IP_ADDRESS;
						retVal = STATE_COMM_ERROR;
						stateStack.Push( STATE_COMM_ASTM_ENTER_ANALYZER_IP );
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
 * StateCommAstmEnterGateway
 */

void
StateCommAstmEnterGateway::
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
StateCommAstmEnterGateway::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_COMM_ASTM_ENTER_GATEWAY;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t msg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			if ( idata == -1 )		// 'C'
			{
				memset( value, 0, sizeof( value ) );
			}
			else if ( strlen( value ) < (sizeof( value ) - 1) )
			{
				if ( (idata >= 0) && (idata <= 9) )
				{
					value[ strlen( value ) ] = idata + '0';
				}
				else if ( idata == -2 )		// '.'
				{
					value[ strlen( value ) ] = '.';
				}
			}

			msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
			strcpy( msg.data.middleEnterIp.text, value );
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
					if ( (strlen( value) == 0) || (ValidateIp( value ) == true) )
					{
						strcpy( settingsData->comm.gateway, value );
						retVal = stateStack.Pop();
					}
					else
					{
						errString = ESTR_ILLEGAL_IP_ADDRESS;
						errInfoString = EISTR_ILLEGAL_IP_ADDRESS;
						retVal = STATE_COMM_ERROR;
						stateStack.Push( STATE_COMM_ASTM_ENTER_GATEWAY );
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
 * StateCommAstmEnterIpMask
 */

void
StateCommAstmEnterIpMask::
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
StateCommAstmEnterIpMask::
ProcessMessage( int msgId, char * data )
{
	state_t 				retVal = STATE_COMM_ASTM_ENTER_IP_MASK;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t msg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			if ( idata == -1 )		// 'C'
			{
				memset( value, 0, sizeof( value ) );
			}
			else if ( strlen( value ) < (sizeof( value ) - 1) )
			{
				if ( (idata >= 0) && (idata <= 9) )
				{
					value[ strlen( value ) ] = idata + '0';
				}
				else if ( idata == -2 )		// '.'
				{
					value[ strlen( value ) ] = '.';
				}
			}

			msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
			strcpy( msg.data.middleEnterIp.text, value );
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
					if ( ValidateIp( value ) == true )
					{
						strcpy( settingsData->comm.ipMask, value );
						retVal = stateStack.Pop();
					}
					else
					{
						errString = ESTR_ILLEGAL_IP_MASK;
						errInfoString = EISTR_ILLEGAL_IP_MASK;
						retVal = STATE_COMM_ERROR;
						stateStack.Push( STATE_COMM_ASTM_ENTER_IP_MASK );
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
 * StateCommAstmEnterServerIp
 */

void
StateCommAstmEnterServerIp::
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
StateCommAstmEnterServerIp::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_COMM_ASTM_ENTER_SERVER_IP;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t msg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			if ( idata == -1 )		// 'C'
			{
				memset( value, 0, sizeof( value ) );
			}
			else if ( strlen( value ) < (sizeof( value ) - 1) )
			{
				if ( (idata >= 0) && (idata <= 9) )
				{
					value[ strlen( value ) ] = idata + '0';
				}
				else if ( idata == -2 )		// '.'
				{
					value[ strlen( value ) ] = '.';
				}
			}

			msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
			strcpy( msg.data.middleEnterIp.text, value );
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
					if ( ValidateIp( value ) == true )
					{
						strcpy( settingsData->comm.serverIp, value );
						retVal = stateStack.Pop();
					}
					else
					{
						errString = ESTR_ILLEGAL_IP_ADDRESS;
						errInfoString = EISTR_ILLEGAL_IP_ADDRESS;
						retVal = STATE_COMM_ERROR;
						stateStack.Push( STATE_COMM_ASTM_ENTER_SERVER_IP );
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
 * StateCommAstmEnterServerSocket
 */

void
StateCommAstmEnterServerSocket::
Enter()
{
	GuiReceiveMsg_t msg;

	SetTopLabel( strings[ TSTR_ENTER_SERVER_SOCKET ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_IP;
	msg.data.middleEnterIp.text[0] = '\0';
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIpData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
	
	memset( value, 0, sizeof( value ) );

}

state_t
StateCommAstmEnterServerSocket::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_COMM_ASTM_ENTER_SERVER_SOCKET;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t msg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			if ( idata == -1 )		// 'C'
			{
				memset( value, 0, sizeof( value ) );
			}
			else if ( strlen( value ) < (sizeof( value ) - 1) )
			{
				if ( (idata >= 0) && (idata <= 9) )
				{
					value[ strlen( value ) ] = idata + '0';
				}
			}

			msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
			strcpy( msg.data.middleEnterIp.text, value );
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
					if ( ValidateSocket( value ) == true )
					{
						settingsData->comm.serverSocket = atoi( value );
						retVal = stateStack.Pop();
					}
					else
					{
						errString = ESTR_ILLEGAL_SOCKET;
						errInfoString = EISTR_ILLEGAL_SOCKET;
						retVal = STATE_COMM_ERROR;
						stateStack.Push( STATE_COMM_ASTM_ENTER_SERVER_SOCKET );
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
	strcpy( data->buttonText[0], strings[ MSTR_USB ] );
	strcpy( data->buttonText[1], strings[ MSTR_TCPIP ] );

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
					retVal = STATE_COMM_ASTM_SELECT;
					stateStack.Push( STATE_COMM_ASTM_LAYER );
					break;
				default:	break;
			}
			break;
		
		case MSG_GUI_OUT_INFO:
// GLD
// Need new ISTR_
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
 * StateCommAstmSelect
 */

void
StateCommAstmSelect::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_TCPIP_SETTINGS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	strcpy( data->buttonText[0], strings[ MSTR_CHANGE_SETTINGS ] );
	strcpy( data->buttonText[1], strings[ MSTR_CHECK_SETTINGS ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StateCommAstmSelect::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_COMM_ASTM_SELECT;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Change Settings
					retVal = STATE_COMM_ASTM_SETTINGS;
					stateStack.Push( STATE_COMM_ASTM_SELECT );
					break;
				case 1:		// Check Settings
					retVal = SaveConfiguration( STATE_COMM_ASTM_CHECK, false );
					stateStack.Push( STATE_COMM_ASTM_SELECT );
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
// GLD
// Need new ISTR_
			infoText = strings[ ISTR_TCPIP_SETTINGS ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_COMM_ASTM_SELECT );
			break;
		default:
			break;
	}
	
	return retVal;
}

/*
 * StateCommAstmSettings
 */

void
StateCommAstmSettings::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i;

	SetTopLabel( strings[ TSTR_ASTM_TCPIP_SETTINGS ] );

	topItem = 0;
	
	strcpy( label[0], strings[ MSTR_ANALYZER_IP ] );
	strcpy( value[0], settingsData->comm.analyzerIp );
	strcpy( label[1], strings[ MSTR_IP_MASK ] );
	strcpy( value[1], settingsData->comm.ipMask );
	strcpy( label[2], strings[ MSTR_GATEWAY_IP ] );
	strcpy( value[2], settingsData->comm.gateway );
	strcpy( label[3], strings[ MSTR_SERVER_IP ] );
	strcpy( value[3], settingsData->comm.serverIp );
	strcpy( label[4], strings[ MSTR_SERVER_SOCKET ] );
	sprintf( value[4], "%d", settingsData->comm.serverSocket );
	numItems = 5;

	msg.msgId = MSG_GUI_MIDDLE_LIST_SETTINGS;

	memset( data, 0, sizeof( GuiMiddleListData_t ) );
	for ( i = 0; i < 6 && i < numItems; i++ )
	{
		strcpy( data->items[i][0], label[i] );
		strcpy( data->items[i][1], value[i] );
	}
	data->isSelectable = 1;
	data->hasUpDown = 0;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );

}

state_t
StateCommAstmSettings::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_COMM_ASTM_SETTINGS;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			stateStack.Push( STATE_COMM_ASTM_SETTINGS );
			switch ( topItem + idata )
			{
				case 0:	// Analyzer ID
					retVal = STATE_COMM_ASTM_ENTER_ANALYZER_IP;
					break;
				case 1:	// IP Mask
					retVal = STATE_COMM_ASTM_ENTER_IP_MASK;
					break;
				case 2:	// Gateway IP
					retVal = STATE_COMM_ASTM_ENTER_GATEWAY;
					break;
				case 3:	// Server IP
					retVal = STATE_COMM_ASTM_ENTER_SERVER_IP;
					break;
				case 4:	// Server Socket
					retVal = STATE_COMM_ASTM_ENTER_SERVER_SOCKET;
					break;
				default:
					stateStack.Pop();
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
				default:
					break;
			}
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

	strcpy( guiMsg.data.winError.topLabel, strings[ TSTR_ERROR ] );
	strcpy( guiMsg.data.winError.message, strings[ errString ] );
	strcpy( guiMsg.data.winError.buttonText, strings[ BSTR_CONTINUE ] );
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
	GuiReceiveMsg_t				msg;
	GuiMiddleOptionInfoData_t *	data = &msg.data.middleOptionInfo;

	SetTopLabel( strings[ TSTR_SET_PROTOCOL ] );

	msg.msgId = MSG_GUI_MIDDLE_OPTION_INFO;
	memset( data, 0, sizeof( GuiMiddleOptionInfoData_t ) );
	strcpy( data->buttonText[0], strings[ MSTR_NONE ] );
	strcpy( data->buttonText[1], strings[ MSTR_ASCII_TEXT ] );
	strcpy( data->buttonText[2], strings[ MSTR_ASTM_1394_7 ] );
	strcpy( data->buttonText[3], strings[ MSTR_XML ] );

	switch ( settingsData->comm.interface )
	{
		case COMM_NONE:		data->selectedButton = 0;	break;
		case COMM_ASCII:	data->selectedButton = 1;	break;
		case COMM_ASTM:		data->selectedButton = 2;	break;
		case COMM_XML:		data->selectedButton = 3;	break;
		default:			data->selectedButton = 0;	break;
	}

	SendGuiMessage( &msg, sizeof( GuiMiddleOptionInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
	
	// Save the current configuration for use when it's time to start/restart ASTM, etc.
	currentCommInterface = settingsData->comm.interface;
	currentCommProtocol = settingsData->comm.protocol;
	strcpy( currentAnalyzerIp, settingsData->comm.analyzerIp );
	strcpy( currentGateway, settingsData->comm.gateway );
	strcpy( currentIpMask, settingsData->comm.ipMask );
	strcpy( currentServerIp, settingsData->comm.serverIp );
	currentServerSocket = settingsData->comm.serverSocket;
	
	// Kill the current Comm program. We'll restart the appropriate one when we leave.
	kill( systemData->commPid, SIGKILL );
	systemData->commPid = -1;
}

state_t
StateCommSetProtocol::
ProcessMessage( int msgId, char * data )
{
	state_t 				retVal = STATE_COMM_SET_PROTOCOL;
	int						idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_TOGGLE:
			// idata has the button index, with the second byte set
			// to 0xF if the button is selected, or to 0 if it's deselected.
			switch ( idata )
			{
				case 0xF0:		settingsData->comm.interface = COMM_NONE;	break;
				case 0xF1:		settingsData->comm.interface = COMM_ASCII;	break;
				case 0xF2:	// ASTM selected
				case 0x2:	// ASTM pressed when already selected
					settingsData->comm.interface = COMM_ASTM;
					retVal = STATE_COMM_ASTM_LAYER;
					stateStack.Push( STATE_COMM_SET_PROTOCOL );
					break;
				case 0xF3:		settingsData->comm.interface = COMM_XML;	break;
				default:	break;
			}
			break;

		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_COMM_PROTOCOL ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_COMM_SET_PROTOCOL );
			break;
			
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = SaveConfiguration( stateStack.Pop() );
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

