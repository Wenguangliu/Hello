/*
 * FILENAME
 * 	ArchiveStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine archive
 * 	reference ranges states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <libgen.h>

#include "ArchiveStates.h"
#include "SettingsMessages.h"
#include "LogErr.h"
#include "ResultsPrint.h"
#include "CfgErr.h"
#include "ParseRefRanges.h"

RefRangeArchiveIndex_t *	StateArchive::archiveIndex = ReadArchiveIndex();
RefRangeArchiveList_t		StateArchive::archiveList;
int							StateArchive::selectedArchive = 0;
char						StateArchive::selectedArchiveLabel[30] = "";
int							StateArchive::topItemSelectReport = 0;
SampleType_t				StateArchive::selectedType;
bool						StateArchive::doPrint;

RefRangeArchiveIndex_t *
StateArchive::
ReadArchiveIndex()
{
	FILE *							fp;
	bool							setToDefault = true;
	static RefRangeArchiveIndex_t	index;

	fp = fopen( REF_RANGE_ARCHIVE_INDEX, "r" );
	if ( fp != 0 )
	{
		fread( &index, sizeof( RefRangeArchiveIndex_t ), 1, fp );
		fclose( fp );
		
		if ( index.version == REF_RANGE_ARCHIVE_INDEX_VERSION )
		{
			setToDefault = false;
		}
	}

	// If we failed to open the index file, or it was the wrong version, set the
	// index data to the default.
	if ( setToDefault )
	{
		memset( &index, 0, sizeof( RefRangeArchiveIndex_t ) );
		index.lastEntry = MAX_ARCHIVED_REF_RANGES - 1;
		index.version = REF_RANGE_ARCHIVE_INDEX_VERSION;
		LogInfo( "setting ref ranges archive to default" );
	}
	
	return &index;
}

CfgError_t
StateArchive::
ArchiveCurrentRefRanges()
{
	char		fileName[50];	// Long enough for the full path and file name.
	FILE *		fp;
	CfgError_t	cfgErr;
	
	/*
	 * Write the archive file.
	 */

	// Create the file name using the current time.
	sprintf( fileName, "%s%u.xml", REF_RANGES_DIRECTORY, time( 0 ) );
	
	// Write the file.
	cfgErr = WriteRefRanges( fileName, refRanges );
	if ( cfgErr != CFG_ERR_NONE )
	{
		LogError( "failed to write ref range archive file %s", fileName );
		return cfgErr;
	}

	// Update the entry number.
	if ( ++archiveIndex->lastEntry >= MAX_ARCHIVED_REF_RANGES )
	{
		archiveIndex->lastEntry = 0;
	}
	
	// We're at the limit on archive files, so delete the oldest.
	if ( archiveIndex->fileNames[ archiveIndex->lastEntry ][0] != '\0' )
	{
		if ( unlink( archiveIndex->fileNames[ archiveIndex->lastEntry ] ) != 0 )
		{
			LogError( "failed to delete ref range archive file %s",
					  archiveIndex->fileNames[ archiveIndex->lastEntry ] );
		}
	}
	
	// Put the new filename in the index.
	strcpy( archiveIndex->fileNames[ archiveIndex->lastEntry ], fileName );

	// Write the updated index file.
	fp = fopen( REF_RANGE_ARCHIVE_INDEX, "w" );
	if ( fp != 0 )
	{
		fwrite( archiveIndex, sizeof( RefRangeArchiveIndex_t ), 1, fp );
		fclose( fp );
	}
	else
	{
		return CFG_ERR_FILE_OPEN;
	}
	
	return CFG_ERR_NONE;
}

void
StateArchive::
GetArchiveLabel( char * buff, char * fileName )
{
	time_t			t;
	struct tm *		tm;
	int				hour;
	const char *	suffix;

	t = (time_t)atoi( basename( fileName ) );
	tm = gmtime( &t );
	
	hour = tm->tm_hour;
	if ( settingsData->time24Format == false )
	{
		if ( hour >= 12 )
		{
			suffix = " PM";
			if ( hour > 12 )
			{
				hour -= 12;
			}
		}
		else
		{
			suffix = " AM";
			if ( hour == 0 )
			{
				hour = 12;
			}
		}
	}
	else
	{
		suffix = "";
	}
			
	sprintf( buff, "%d %s %d %d:%02d %s", tm->tm_mday,
										  strings.Month( tm->tm_mon ),
										  tm->tm_year + 1900,
										  hour,
										  tm->tm_min,
										  suffix );
}

void
StateArchive::
GetArchiveList()
{
	int	i;
	
	// Start with an empty list.
	memset( &archiveList, 0, sizeof( RefRangeArchiveList_t ) );

	i = archiveIndex->lastEntry;
	archiveList.len = 0;
	do
	{
		i = (i + 1) % MAX_ARCHIVED_REF_RANGES;
		
		if ( archiveIndex->fileNames[ i ][0] != '\0' )
		{
			GetArchiveLabel( archiveList.names[ archiveList.len ], archiveIndex->fileNames[ i ] );
			strcpy( archiveList.fileNames[ archiveList.len ], archiveIndex->fileNames[ i ] );
			archiveList.len++;
		}
	} while ( i != archiveIndex->lastEntry );
}

CfgError_t
StateArchive::
RetrieveArchive( int index )
{
	char *			fileName = archiveList.fileNames[ index ];
	RefRangeData_t	temp;
	CfgError_t		cfgErr;
	
	// Read and parse the ref ranges file into our temp location.
	memset( &temp, 0, sizeof( RefRangeData_t ) );
	cfgErr = ParseRefRanges( fileName, &temp );
	if ( cfgErr != CFG_ERR_NONE )
	{
		LogError( "failed to read ref range archive file %s", fileName );
		return cfgErr;
	}
	
	// Successful, so copy the temp data to the actual ref range data.
	memcpy( refRanges, &temp, sizeof( RefRangeData_t ) );

	// Update the current ref range file.
	cfgErr = WriteRefRanges( REF_RANGES_CURRENT_FILE, refRanges );
	{
		LogError( "failed to write current ref ranges file %s", REF_RANGES_CURRENT_FILE );
		return cfgErr;
	}

	return CFG_ERR_NONE;
}

/*
 * class StateArchiveSelect
 */
 
void
StateArchiveSelect::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_ARCHIVE_REF_RANGES ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;
	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	strcpy( data->buttonText[0], strings[ MSTR_SEND ] );
	strcpy( data->buttonText[1], strings[ MSTR_RETRIEVE ] );
	data->buttonHighlighted[0] = false;
	data->buttonHighlighted[1] = false;
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateArchiveSelect::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ARCHIVE_SELECT;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Send
					retVal = STATE_ARCHIVE_SEND;
					stateStack.Push( STATE_ARCHIVE_SELECT );
					break;
				case 1:		// Retrieve
					retVal = STATE_ARCHIVE_RETRIEVE;
					stateStack.Push( STATE_ARCHIVE_SELECT );
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
					retVal = stateStack.Home();
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_ARCHIVE_SELECT ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_ARCHIVE_SELECT );
			break;
		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateArchiveSend
 */
 
void
StateArchiveSend::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_SEND_REF_RANGES ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	strcpy( data->buttonText[0], strings[ MSTR_PRINTER ] );
	strcpy( data->buttonText[1], strings[ MSTR_INTERNAL_ARCHIVE ] );
	strcpy( data->buttonText[2], strings[ MSTR_EXTERNAL_ARCHIVE ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateArchiveSend::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_ARCHIVE_SEND;
	int			idata = *(int *)data;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Print
					doPrint = true;
					if ( settingsData->sampleTypeAdvanced )
					{
						topItemSelectReport = 0;
						retVal = STATE_ARCHIVE_SELECT_REPORT_ADVANCED;
					}
					else
					{
						retVal = STATE_ARCHIVE_SELECT_REPORT_EXPRESS;
					}
					stateStack.Push( STATE_ARCHIVE_SEND );
					break;
				case 1:		// Internal
					cfgErr = ArchiveCurrentRefRanges();
					if ( cfgErr != CFG_ERR_NONE )
					{
						errNum = SERR_FILE_WRITE;
						retVal = STATE_ERROR;
					}
					else
					{
						retVal = stateStack.Pop();
					}
					break;
				case 2:		// External
					doPrint = false;
					if ( settingsData->sampleTypeAdvanced )
					{
						retVal = STATE_ARCHIVE_SELECT_REPORT_ADVANCED;
					}
					else
					{
						retVal = STATE_ARCHIVE_SELECT_REPORT_EXPRESS;
					}
					stateStack.Push( STATE_ARCHIVE_SEND );
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
					retVal = stateStack.Home();
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_ARCHIVE_SEND ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_ARCHIVE_SEND );
			break;
		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateArchiveRetrieve
 */
 
void
StateArchiveRetrieve::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i;
	
	GetArchiveList();

	SetTopLabel( strings[ TSTR_INTERNAL_ARCHIVES ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST;
	memset( data, 0, sizeof( GuiMiddleListData_t ) );
	
	for ( i = 0; i < 5 && i < archiveList.len; i++ )
	{
		strcpy( data->items[i][0], archiveList.names[ i ] );
	}	
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false );
	
	topItemArchiveRetrieve = 0;
}

state_t
StateArchiveRetrieve::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ARCHIVE_RETRIEVE;
	int		idata = *(int *)data;
	bool	doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			selectedArchive = topItemArchiveRetrieve + idata;
			strcpy( selectedArchiveLabel, archiveList.names[ selectedArchive ] );
			retVal = STATE_ARCHIVE_CONFIRM_RETRIEVE;
			stateStack.Push( STATE_ARCHIVE_RETRIEVE );
			break;
		case MSG_GUI_OUT_UP:
			if ( topItemArchiveRetrieve > 0 )
			{
				topItemArchiveRetrieve -= 4;
				if ( topItemArchiveRetrieve < 0 )
				{
					topItemArchiveRetrieve = 0;
					
				}
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_DOWN:
			if ( topItemArchiveRetrieve < archiveList.len - 5 )
			{
				topItemArchiveRetrieve += 4;
				doUpdate = true;
			}
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
			}
			break;

		default:
			break;
	}
	
	if ( doUpdate )
	{
		GuiReceiveMsg_t			msg;
		GuiMiddleListData_t *	data = &msg.data.middleList;
		int						i;

		msg.msgId = MSG_GUI_UPDATE_LIST;
		memset( data, 0, sizeof( GuiMiddleListData_t ) );
		
		for ( i = 0; i < 5 && i < archiveList.len - topItemArchiveRetrieve; i++ )
		{
			strcpy( data->items[ i ][0], archiveList.names[ topItemArchiveRetrieve + i ] );
		}	
	
		data->isSelectable = 1;
		data->hasUpDown = 1;
	
		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}
		
	return retVal;
}

/*
 * class StateArchiveConfirmRetrieve
 */
 
void
StateArchiveConfirmRetrieve::
Enter()
{
	GuiReceiveMsg_t		guiMsg;
	char				buff[ 256 ];
	
	sprintf( buff, "%s %s", strings[ ESTR_RESTORING_REF_RANGES ], selectedArchiveLabel );

	guiMsg.msgId = MSG_GUI_WIN_WARNING2;
	strcpy( guiMsg.data.winWarning.topLabel, strings[ TSTR_WARNING ] );
	strcpy( guiMsg.data.winWarning.message, buff );
	strcpy( guiMsg.data.winWarning.buttonText[0], strings[ BSTR_CONTINUE ] );
	strcpy( guiMsg.data.winWarning.buttonText[1], strings[ BSTR_CANCEL ] );
	
	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
}

state_t
StateArchiveConfirmRetrieve::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_ARCHIVE_CONFIRM_RETRIEVE;
	int			idata = *(int *)data;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_ARCHIVE_RESTORE ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_ARCHIVE_CONFIRM_RETRIEVE );
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Continue
					cfgErr = RetrieveArchive( selectedArchive );
					if ( cfgErr != CFG_ERR_NONE )
					{
						errNum = SERR_FILE_READ;
						retVal = STATE_ERROR;
					}
					else
					{
						retVal = stateStack.Pop();
					}
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

/*
 * class StateArchiveSelectReportExpress
 */
 
void
StateArchiveSelectReportExpress::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsData_t *	data = &msg.data.middleButtons;

	SetTopLabel( strings[ TSTR_SELECT_REPORT ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS;

	memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
	strcpy( data->buttonText[0], strings[ MSTR_ALL ] );
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		strcpy( data->buttonText[1], strings[ SSTR_PATIENT ] );
		strcpy( data->buttonText[2], strings[ SSTR_CONTROL ] );
	}
	else
	{
		strcpy( data->buttonText[1], strings[ SSTR_DOG ] );
		strcpy( data->buttonText[2], strings[ SSTR_CAT ] );
		strcpy( data->buttonText[3], strings[ SSTR_HORSE ] );
		strcpy( data->buttonText[4], strings[ SSTR_OTHER ] );
	}

	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateArchiveSelectReportExpress::
ProcessMessage( int msgId, char * data )
{
	state_t 				retVal = STATE_ARCHIVE_SELECT_REPORT_EXPRESS;
	int						idata = *(int *)data;
	const SampleType_t		humanSampleTypes[] = { STYPE_ALL, STYPE_OTHER, STYPE_CONTROL };
	const SampleType_t		vetSampleTypes[] = { STYPE_ALL, STYPE_DOG, STYPE_CAT, STYPE_HORSE, STYPE_OTHER };
	const SampleType_t *	sampleTypes;
	
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		sampleTypes = humanSampleTypes;
	}
	else
	{
		sampleTypes = vetSampleTypes;
	}

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			selectedType = sampleTypes[ idata ];
			retVal = STATE_ARCHIVE_SENDING;
			stateStack.Push( STATE_ARCHIVE_SELECT_REPORT_EXPRESS );
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
			}
			break;
		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateArchiveSelectReportAdvanced
 */
 
void
StateArchiveSelectReportAdvanced::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i;

	activeRanges = GetActiveRefRanges( refRanges );

	SetTopLabel( strings[ TSTR_SELECT_REPORT ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST;

	memset( data, 0, sizeof( GuiMiddleListData_t ) );

	i = 0;
	if ( topItemSelectReport == 0 )
	{
		strcpy( data->items[0][0], strings[ MSTR_ALL ] );
		i = 1;
	}
		
	for ( ; i < 5 && i < activeRanges->len - topItemSelectReport + 1; i++ )
	{
		strcpy( data->items[ i ][0], species[ activeRanges->list[ topItemSelectReport + i - 1 ] ] );
	}
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

int
StateArchiveSelectReportAdvanced::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ARCHIVE_SELECT_REPORT_ADVANCED;
	int		idata = *(int *)data;
	bool	doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			if ( topItemSelectReport + idata == 0 )
			{
				selectedType = STYPE_ALL;
			}
			else
			{
				selectedType = activeRanges->list[ topItemSelectReport + idata - 1 ];
			}
			retVal = STATE_ARCHIVE_SENDING;
			stateStack.Push( STATE_ARCHIVE_SELECT_REPORT_ADVANCED );
			break;

		case MSG_GUI_OUT_UP:
			if ( topItemSelectReport > 0 )
			{
				topItemSelectReport -= 4;
				if ( topItemSelectReport < 0 )
				{
					topItemSelectReport = 0;
				}
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_DOWN:
			if ( activeRanges->len > topItemSelectReport + 5 - 1 )	// -1 correction for All
			{
				topItemSelectReport += 4;
			}
			doUpdate = true;
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
	}

	
	if ( doUpdate )
	{
		GuiReceiveMsg_t			msg;
		GuiMiddleListData_t *	data = &msg.data.middleList;
		int						i;

		memset( &msg, 0, sizeof( msg ) );
		msg.msgId = MSG_GUI_UPDATE_LIST;

		i = 0;
		if ( topItemSelectReport == 0 )
		{
			strcpy( data->items[0][0], strings[ MSTR_ALL ] );
			i = 1;
		}
			
		for ( ; i < 5 && i < activeRanges->len - topItemSelectReport + 1; i++ )
		{
			strcpy( data->items[ i ][0], species[ activeRanges->list[ topItemSelectReport + i - 1 ] ] );
		}
		data->isSelectable = 1;
		data->hasUpDown = 1;

		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}

	return retVal;
}

/*
 * Class StateArchiveSending
 */
 
void 
StateArchiveSending::
Enter()
{
	GuiReceiveMsg_t	guiMsg;
	SerialCommMsg_t	serialCommMsg;;

	SetTopLabel( 0 );
	
	guiMsg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	
	strcpy( guiMsg.data.middleMessage.message, strings[ MSTR_SENDING_REPORT ] );
	
	SendGuiMessage( &guiMsg, sizeof( GuiMiddleMessageData_t ) );
	
	SetBottomButtons();	

	if ( doPrint )
	{
		errNum = ResultsPrint::PrintRefRanges( selectedType );
	}
	else if ( settings->comm.interface != COMM_ASTM )
	{
		serialCommMsg.msgId = MSG_SERIAL_COMM_SEND_REF_RANGES;
		serialCommMsg.sampleType = selectedType;
		SendSerialCommMessage( &serialCommMsg, sizeof( SampleType_t ) );
		errNum = SERR_NONE;
	}

// GLD
// We could do something with a message back from SerialComm to indicate that
// we're done transmitting - if that makes sense according to the size of the
// data we're sending.  Maybe only for all?

	// This generates an event that will call StateArchiveSending::ProcessMessage.
	SendSettingsMessage( MSG_SETTINGS_TRIGGER );
}

state_t 
StateArchiveSending::
ProcessMessage( int msgId, char * data )
{
	if ( msgId == MSG_SETTINGS_TRIGGER )
	{
		if ( errNum != SERR_NONE )
		{
			return STATE_ERROR;
		}
		else
		{
			return stateStack.Pop();
		}
	}
	else
	{
		return STATE_ARCHIVE_SENDING;
	}
}

