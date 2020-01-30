/*
 * FILENAME
 * 	Comm.cc
 * 
 * MODULE DESCRIPTION
 * 	Class method definitions for the Comm class, which handles messages sent to
 * 	other processes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// GLD
#include <errno.h>

/* Local headers */
#include "ablibs.h"
#include "Global.h"
#include "abimport.h"
#include "GuiMessages.h"
#include "Comm.h"
#include "LogErr.h"
#include "Sound.h"

mqd_t Comm::mqueue = 0;

/*
 * FUNCTION
 * 	Comm:Info
 * 
 * DESCRIPTION
 * 	Event handler for all Info buttons.  Informs the controlling process that
 * 	this button was pressed.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::Info( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	Click();

	SendMessage( MSG_GUI_OUT_INFO, 0 );
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::Button
 * 
 * DESCRIPTION
 * 	Event handler for all generic buttons.  Informs the controlling process that
 * 	this button was pressed.  Includes an index value for the button.  The index
 * 	value is screen-dependent.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::Button( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	Click();

	SendMessage( MSG_GUI_OUT_BUTTON, widget->user_data[0] != 0 ? atoi( widget->user_data ) : -1 );
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::Up
 * 
 * DESCRIPTION
 * 	Event handler for all Up buttons.  Informs the controlling process that
 * 	this button was pressed.  Includes an index value for the button.  The index
 * 	value is screen-dependent.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::Up( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	Click();

	SendMessage( MSG_GUI_OUT_UP, widget->user_data[0] != 0 ? atoi( widget->user_data ) : -1 );

	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::Down
 * 
 * DESCRIPTION
 * 	Event handler for all Down buttons.  Informs the controlling process that
 * 	this button was pressed.  Includes an index value for the button.  The index
 * 	value is screen-dependent.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::Down( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	Click();

	SendMessage( MSG_GUI_OUT_DOWN, widget->user_data[0] != 0 ? atoi( widget->user_data ) : -1 );

	return Pt_CONTINUE;
}

/*
 * FUNCTION
 * 	Comm::List
 * 
 * DESCRIPTION
 * 	Event handler for all single-selection list selections.  Informs the
 *	controlling process that a list item was selected.  Includes the index value
 *  for the item.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::List( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtGenListItem_t ** selectedItems = PtGenListSelectedItems( ABW_ptListList, NULL );
	
	Click();

	// The selection mode for the list should be either single or none.  If it's
	// none, this will still be called if the user presses a list item.  However,
	// there will be no selected item.
	if ( selectedItems[0] != 0 )
	{
		// Subtract one from the index of the selected item, so we end up with 
		// a zero-based index.
		int index = PtGenListItemIndex( ABW_ptListList, selectedItems[0] ) - 1;
		SendMessage( MSG_GUI_OUT_LIST, index );
	}
	
	free( selectedItems );

	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::ListMulti
 * 
 * DESCRIPTION
 * 	Event handler for multiple-selection list selections.  Informs the
 *  controlling process which items are currently selected.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::ListMulti( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	int					i;
	int					selected = 0;
	PtGenListItem_t **	selectedItems;
	
	if ( cbinfo->event->type == Ph_EV_BUT_PRESS )
	{
		Click();
	
		selectedItems = PtGenListSelectedItems( ABW_ptListListMulti, NULL );
		for ( i = 0; selectedItems[ i ] != 0; i++ )
		{
			// Subtract one from the index of the selected item, so we end up with 
			// a zero-based index.
			int index = PtGenListItemIndex( ABW_ptListListMulti, selectedItems[ i ] ) - 1;
			
			selected |= 1 << index;
		}
	
		SendMessage( MSG_GUI_OUT_LIST, selected );
		
		free( selectedItems );
	}

	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::BottomButton
 * 
 * DESCRIPTION
 * 	Event handler for all Bottom section buttons.  Informs the controlling
 *  process that this button was pressed.  Includes an index value for the
 *	button.  The index value is the offset from the left side of the screen for
 * 	the button.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::BottomButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	Click();

	SendMessage( MSG_GUI_OUT_BOTTOM_BUTTON, widget->user_data[0] != 0 ? atoi( widget->user_data ) : -1 );
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::Recall
 * 
 * DESCRIPTION
 * 	Event handler for the Recall buttons on the Home Warming and Home Ready
 * 	screens.  Informs the controlling process that this button was pressed.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::Recall( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	Click();

	SendMessage( MSG_GUI_OUT_RECALL, 0 );
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::Recall
 * 
 * DESCRIPTION
 * 	Event handler for the Manufacturing buttons on the Home Warming and Home
 *	Ready screens that are specific to manufacturing software.  Informs the
 *	controlling process that this button was pressed.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::Manufacturing( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	Click();

	SendMessage( MSG_GUI_OUT_MANUFACTURING, 0 );
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::Settings
 * 
 * DESCRIPTION
 * 	Event handler for the Settings buttons on the Home Warming and Home Ready
 * 	screens.  Informs the controlling process that this button was pressed.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::Settings( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	Click();

	SendMessage( MSG_GUI_OUT_SETTINGS, 0 );
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::IconMatrix
 * 
 * DESCRIPTION
 * 	Event handler for the icon buttons in the Icon Matrix screens.  Informs the
 *	controlling process that this button was pressed.  Includes a constant value
 * 	to indicate which icon button was pressed.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::IconMatrix( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	int data = -1;
	
	const struct { const char * s; int data; } translate[] = {
		{ "Who",				ICON_WHO				},
		{ "DateTime",			ICON_DATE_TIME			},
		{ "Language",			ICON_LANGUAGE			},
		{ "Printer",			ICON_PRINTER			},
		{ "Screen",				ICON_SCREEN				},
		{ "Sound",				ICON_SOUND				},
		{ "Units",				ICON_UNITS				},
		{ "RefRanges",			ICON_REF_RANGES			},
		{ "ArchiveRefRanges",	ICON_ARCHIVE_REF_RANGES	},
		{ "Comm",				ICON_COMM				},
		{ "Security",			ICON_SECURITY			},
		{ "Advanced",			ICON_ADVANCED			},
		{ "Update",				ICON_UPDATE				},
		{ "Factory",			ICON_FACTORY			},
		{ 0, 0 } };
		
	Click();

	for ( int i = 0; translate[i].s != 0; i++ )
	{
		if ( strcmp( widget->user_data, translate[i].s ) == 0 )
		{
			data = translate[i].data;
			break;
		}
	}

	SendMessage( MSG_GUI_OUT_ICON_MATRIX, data );
		
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::NextSettings
 * 
 * DESCRIPTION
 * 	Event handler for the Next button on the Icon Matrix screens.  Informs the
 *	controlling process that this button was pressed.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::NextSettings( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	Click();

	SendMessage( MSG_GUI_OUT_NEXT_SETTINGS, 0 );
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::PreviousSettings
 * 
 * DESCRIPTION
 * 	Event handler for the Previous button on the Icon Matrix screens.  Informs the
 *	controlling process that this button was pressed.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::PreviousSettings( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	Click();

	SendMessage( MSG_GUI_OUT_PREVIOUS_SETTINGS, 0 );
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::Toggle
 * 
 * DESCRIPTION
 * 	Event handler for all toggle buttons.  Informs the controlling process that
 * 	this button was pressed.  Includes an index value for the button.  The index
 * 	value is screen-dependent and includes a flag to indicate whether the the
 * 	button was selected or deselected.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::Toggle( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	// Button index in the byte 0.  Byte 1 set to 0xF if the button is set.
	int data = widget->user_data[0] != 0 ? atoi( widget->user_data ) : -1;
	if ( ((PtBasicCallback_t *)cbinfo->cbdata)->value != 0 )
	{
		data |= 0xF0;
	}

	Click();

	SendMessage( MSG_GUI_OUT_TOGGLE, data );

	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::Activate
 * 
 * DESCRIPTION
 * 	Event handler for Activate events.  Activate events are triggered by text
 * 	widgets when the Enter key is typed on the keyboard while they're active.
 * 	Informs the controlling process that an activation has occurred.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - standard Photon return value.
 */

int
Comm::Activate( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	Click();

	SendMessage( MSG_GUI_OUT_ACTIVATE, 0 );
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Comm::SetCommReceiver
 * 
 * DESCRIPTION
 * 	Sets the message queue to which the Comm methods should send messages.
 * 
 * PARAMETERS
 * 	queueName - Name of the queue to receive Comm messages.
 */

void
Comm::SetCommReceiver( const char * queueName )
{
	if ( mqueue != 0 )
	{
		mq_close( mqueue );
		mqueue = 0;
	}
	
	if ( queueName != NULL )
	{
		mqueue = mq_open( queueName, O_WRONLY );
		if ( mqueue == -1 )
		{
			LogError( "failed to open message receiver message queue (%s), errno: %d",
				queueName, errno );
			mqueue = 0;
			return;
		}
	}
}

/*
 * FUNCTION
 * 	Comm::SendMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the controlling process.
 * 
 * PARAMETERS
 * 	msgId - Message ID for the message.
 * 	data - Message data.
 */

void
Comm::SendMessage( int msgId, const int data )
{
	GuiSendMsg_t msg;
	
	if ( mqueue != 0 )
	{
		msg.msgId = msgId;
		msg.data.idata = data;
		mq_send( mqueue, (const char *)&msg, sizeof( int ) + sizeof( int ), MQ_PRIO_MAX-1 );
	}
}

/*
 * FUNCTION
 * 	Comm::SendMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the controlling process.
 * 
 * PARAMETERS
 * 	msgId - Message ID for the message.
 * 	data - Message data.
 */

void
Comm::SendMessage( int msgId, const char * data )
{
	GuiSendMsg_t msg;
	
	if ( mqueue != 0 )
	{
		msg.msgId = msgId;
		strcpy( msg.data.sdata, data );
		mq_send( mqueue, (const char *)&msg, sizeof( GuiSendMsg_t ), MQ_PRIO_MAX-1 );
	}		
}

void
Comm::SendMessage( int msgId, GuiSendRangeData_t range )
{
	GuiSendMsg_t msg;
	
	if ( mqueue != 0 )
	{
		msg.msgId = msgId;
		msg.data.range = range;
		mq_send( mqueue, (const char *)&msg, sizeof( GuiSendMsg_t ), MQ_PRIO_MAX-1 );
	}		
}
