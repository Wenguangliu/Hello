/*
 * FILENAME
 * 	MiddleSection.c
 * 
 * MODULE DESCRIPTION
 * 	Class method definitions for the middle subscreen classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include "ablibs.h"
#include "abimport.h"
#include "MiddleSection.h"
#include "Global.h"
#include "Colors.h"
#include "Shmem.h"
#include "TranslateUtf.h"

PhImage_t *		MiddleSection::bkgdImage = 0;
FactoryData_t *	MiddleIconMatrix2::factoryData = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
PtWidget_t *	MiddleListMulti::list = 0;
PtWidget_t *	MiddleListParent::listWindow = 0;
PtWidget_t *	MiddleListParent::list = 0;

/*
 * MiddleSection
 */
 
/*
 * FUNCTION
 * 	MiddleSection::Show
 * 
 * DESCRIPTION
 * 	Makes the associated window visible by moving it from its offscreen "hiding
 * 	place" to proper position on the visible screen.
 *
 * NOTE
 * 	This description applies to all Show methods for the subclasses of
 *	MiddleSection.
 */

void
MiddleSection::
Show( void )
{
	if ( window != 0 )
	{
		PhPoint_t pos = {240, 20};
		PtSetResource( window, Pt_ARG_POS, &pos, 0 );
		PtSetResource( window, Pt_ARG_CURSOR_TYPE, cursorType_g, 0 );
	}
}

/*
 * FUNCTION
 * 	MiddleSection::Hide
 * 
 * DESCRIPTION
 * 	Makes the associated window invisible by moving it from the visible screen
 * 	to a "hiding place" off the screen.
 *
 * NOTE
 * 	This description applies to all Hide methods for the subclasses of
 *	MiddleSection.
 */

void
MiddleSection::
Hide()
{
	if ( window != 0 )
	{
		PhPoint_t pos = {-400, 0};
		PtSetResource( window, Pt_ARG_POS, &pos, 0 );

		/*
		 * We need to make this call so the background window will repaint
		 * before we draw the new window.  Otherwise, we end up with the new
		 * window painted on top of the old window.
		 */
		PtBkgdHandlerProcess();
	}
}

void
MiddleSection::
GetBackground()
{
	PtWidget_t *	bkgdWindow;

	// Create the module holding our background image.
	bkgdWindow = ApCreateModule( ABM_winMiddleBackground, 0, 0 );
	
	// Get the image.
	PtGetResource( ABW_ptLabelMiddleBkgd, Pt_ARG_LABEL_IMAGE, &bkgdImage, 0 );
	
	// Make sure the image doesn't get deleted by any of the widgets using it.
	bkgdImage->flags = 0;
}


/*
 * MiddleListParent
 */
 
/*
 * FUNCTION
 * 	MiddleListParent::MiddleListParent
 * 
 * DESCRIPTION
 * 	Creates the list window, which is used by all the list classes.
 */

MiddleListParent::
MiddleListParent()
{
	if ( listWindow == 0 )
	{
		listWindow = ApCreateModule( ABM_winList, 0, 0 );
		list = ABW_ptListList;

		PtSetResource( ABW_ptLabelListBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
	}
}

void
MiddleListParent::
Show( )
{
	if ( listWindow != 0 )
	{
		PhPoint_t pos = {240, 20};
		PtSetResource( listWindow, Pt_ARG_POS, &pos, 0 );
		PtSetResource( listWindow, Pt_ARG_CURSOR_TYPE, cursorType_g, 0 );
		PtSetResource( list, Pt_ARG_CURSOR_TYPE, cursorType_g, 0 );
	}
}

void
MiddleListParent::
Hide()
{
	if ( listWindow != 0 )
	{
		PhPoint_t pos = {-400, 0};
		PtSetResource( listWindow, Pt_ARG_POS, &pos, 0 );

		/*
		 * We need to make this call so the background window will repaint
		 * before we draw the new window.  Otherwise, we end up with the new
		 * window painted on top of the old window -- not a pretty sight.
		 */
		PtBkgdHandlerProcess();
	}
}

/*
 * FUNCTION
 * 	MiddleListParent::SetColumns
 * 
 * DESCRIPTION
 * 	Sets the column widths for the three columns in the list widget.
 *
 * PARAMETERS
 * 	column0 - Width of the first column.
 * 	column1 - Width of the second column.
 * 	column2 - Width of the third column.
 */

void
MiddleListParent::
SetColumns( short column0, short column1, short column2 )
{
	PtSetResource( ABW_ptButtonListColumn0, Pt_ARG_WIDTH, column0, 0 );
	PtSetResource( ABW_ptButtonListColumn1, Pt_ARG_WIDTH, column1, 0 );
	PtSetResource( ABW_ptButtonListColumn2, Pt_ARG_WIDTH, column2, 0 );
}

/******************************************************************************/

/*
 * MiddleAnalysisInProcess
 */
 
/*
 * FUNCTION
 * 	MiddleAnalysisInProcess::MiddleAnalysisInProcess
 * 
 * DESCRIPTION
 * 	Creates the subscreen window and inits pointers to some of the contained
 * 	widgets that may be modified (text set, etc.).
 *
 * NOTE
 * 	This description applies to all the constructors of the subclasses of
 * 	MiddleSection.
 */

/*
 * MiddleAnalysisInProcess
 */
 
MiddleAnalysisInProcess::
MiddleAnalysisInProcess()
{
	window = ApCreateModule( ABM_winAnalysisInProcess, 0, 0 );

	label = ABW_ptLabelAnalysisInProcessLabel;
	text = ABW_ptLabelAnalysisInProcess;
	progBar = ABW_ptProgressAnalysisInProcess;

	PtSetResource( ABW_ptLabelAnalysisInProcessBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleAnalysisInProcess::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( label, data->middleProgBarMessage.progBarText, true );
	SetString( text,  data->middleProgBarMessage.message, true );
	PtSetResource( text, Pt_ARG_FILL_COLOR, data->middleProgBarMessage.backgroundColor, 0 );
	PtSetResource( text, Pt_ARG_COLOR, data->middleProgBarMessage.fontColor, 0 );
	PtSetResource( progBar, Pt_ARG_GAUGE_VALUE, data->middleProgBarMessage.progBarValue, 0 );
}

void
MiddleAnalysisInProcess::
Update( GuiReceiveMsg_t * msg )
{
	switch ( msg->msgId )
	{
		case MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS:
			PtSetResource( progBar, Pt_ARG_GAUGE_VALUE, msg->data.middleProgBarMessage.progBarValue, 0 );
			break;
		case MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_TEXT:
			SetString( text, msg->data.middleProgBarMessage.message, true );
			PtSetResource( text, Pt_ARG_FILL_COLOR, msg->data.middleProgBarMessage.backgroundColor, 0 );
			PtSetResource( text, Pt_ARG_COLOR, msg->data.middleProgBarMessage.fontColor, 0 );
			break;
		default:
			break;
	}		
}

MiddleButtons::
MiddleButtons()
{
	window = ApCreateModule( ABM_winButtons, 0, 0 );

	buttons[0] = ABW_ptButtonButtons0;
	buttons[1] = ABW_ptButtonButtons1;
	buttons[2] = ABW_ptButtonButtons2;
	buttons[3] = ABW_ptButtonButtons3;
	buttons[4] = ABW_ptButtonButtons4;
	buttons[5] = ABW_ptButtonButtons5;

	PtSetResource( ABW_ptLabelButtonsBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

/*
 * FUNCTION
 * 	MiddleButtons::SetData
 * 
 * DESCRIPTION
 * 	Sets the data fields (text strings, etc.) in the subsection window to the
 * 	values passed in the message.
 *
 * NOTE
 * 	This description applies to all the SetData methods of the subclasses of
 * 	MiddleSection.
 */

void
MiddleButtons::
SetData( GuiReceiveMsgData_t * data )
{
	for ( int i = 0; i < 6; i++ )
	{
		if ( data->middleButtons.buttonText[i][0] != 0  )
		{
			PtRealizeWidget( buttons[i] );
			SetString( buttons[i], data->middleButtons.buttonText[i], true );
			
			if ( data->middleButtons.buttonHighlighted[i] )
			{
				PtSetResource( buttons[i], Pt_ARG_FILL_COLOR, ORANGE_HILITE, 0 );
			}
			else
			{
				PtSetResource( buttons[i], Pt_ARG_FILL_COLOR, ORANGE_NORMAL, 0 );
			}
		}
		else
		{
			PtUnrealizeWidget( buttons[i] );
		}
	}
}

/*
 * MiddleButtonsInfo
 */
 
MiddleButtonsInfo::
MiddleButtonsInfo()
{
	window = ApCreateModule( ABM_winButtonsInfo, 0, 0 );

	buttons[0] = ABW_ptButtonButtonsInfo0;
	buttons[1] = ABW_ptButtonButtonsInfo1;
	buttons[2] = ABW_ptButtonButtonsInfo2;
	buttons[3] = ABW_ptButtonButtonsInfo3;

	PtSetResource( ABW_ptLabelButtonsInfoBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleButtonsInfo::
SetData( GuiReceiveMsgData_t * data )
{
	for ( int i = 0; i < 4; i++ )
	{
		if ( data->middleButtonsInfo.buttonText[i][0] != 0  )
		{
			PtRealizeWidget( buttons[i] );
			SetString( buttons[i], data->middleButtonsInfo.buttonText[i], true );
			
			if ( data->middleButtonsInfo.buttonHighlighted[i] )
			{
				PtSetResource( buttons[i], Pt_ARG_FILL_COLOR, ORANGE_HILITE, 0 );
			}
			else
			{
				PtSetResource( buttons[i], Pt_ARG_FILL_COLOR, ORANGE_NORMAL, 0 );
			}
		}
		else
		{
			PtUnrealizeWidget( buttons[i] );
		}
	}

	MiddleSection::Show();
}

/*
 * MiddleEnterDate
 */
 
MiddleEnterDate::
MiddleEnterDate()
{
	window = ApCreateModule( ABM_winEnterDate, 0, 0 );

	labelLeft = ABW_ptLabelEnterDateLeft;
	labelMiddle = ABW_ptLabelEnterDateMiddle;
	labelRight = ABW_ptLabelEnterDateRight;
	textLeft = ABW_ptTextEnterDateLeft;
	textMiddle = ABW_ptTextEnterDateMiddle;
	textRight = ABW_ptTextEnterDateRight;
	button = ABW_ptButtonEnterDate;

	PtSetResource( ABW_ptLabelEnterDateBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleEnterDate::
Show()
{
	MiddleSection::Show();
	
	PtWindowFocus( window );
}

void
MiddleEnterDate::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( labelLeft, data->middleEnterDate.labelLeft );
	SetString( labelMiddle, data->middleEnterDate.labelMiddle );
	SetString( labelRight, data->middleEnterDate.labelRight );

	SetString( textLeft, data->middleEnterDate.textLeft );
	SetString( textMiddle, data->middleEnterDate.textMiddle );
	SetString( textRight, data->middleEnterDate.textRight );
	
	if ( data->middleEnterDate.buttonText[0] != '\0' )
	{
		SetString( button, data->middleEnterDate.buttonText );
		PtRealizeWidget( button );
	}
	else
	{		
		PtUnrealizeWidget( button );
	}
}

void
MiddleEnterDate::
Update( GuiReceiveMsg_t * msg )
{
	switch ( msg->msgId )
	{
		case MSG_GUI_UPDATE_ENTER_DATE_LEFT:
			SetString( textLeft, msg->data.middleEnterDate.textLeft );
			break;
			
		case MSG_GUI_UPDATE_ENTER_DATE_MIDDLE:
			SetString( textMiddle, msg->data.middleEnterDate.textMiddle );
			break;
			
		case MSG_GUI_UPDATE_ENTER_DATE_RIGHT:
			SetString( textRight, msg->data.middleEnterDate.textRight );
			break;
			
		default:
			break;
	}
}

/*
 * MiddleEnterId
 */
 
MiddleEnterId::
MiddleEnterId()
{
	window = ApCreateModule( ABM_winEnterId, 0, 0 );

	PtSetResource( ABW_ptLabelEnterIdBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleEnterId::
Show()
{
	MiddleSection::Show();
	
	PtWindowFocus( window );

	PtGiveFocus( ABW_ptTextEnterId, 0 );
}

void
MiddleEnterId::
SetData( GuiReceiveMsgData_t * data )
{
	EnterId::Init( data->middleEnterId.isPassword, data->middleEnterId.initialValue );
}

void
MiddleEnterId::
Update( GuiReceiveMsg_t * msg )
{
	EnterId::Init( msg->data.middleEnterId.isPassword, msg->data.middleEnterId.initialValue );
}

/*
 * FUNCTION
 * 	MiddleEnterId::SendIdText
 * 
 * DESCRIPTION
 * 	Sends the ID text to the controlling process.  Retrieves the text from the
 * 	Enter ID window and translates it from UTF-8 to ISO-8859-1 before sending
 * 	it.
 */

void
MiddleEnterId::
SendIdText()
{
	char *	text;
	char *	p; 
	char *	freeString;
	
	// Get the text from the EnterId window.
	text = EnterId::GetIdText();

	// Translate the text to ISO-8859-1.  If the translate fails, use the
	// untranslated text.
	freeString = p = TranslateFromUtf( text );
	if ( p == 0 )
	{
		p = text;
	}
	
	// Send the text.
	Comm::SendMessage( MSG_GUI_OUT_ID_TEXT, p );
	
	// Free the memory malloc'd by TranslateFromUtf().
	if ( freeString != 0 )
	{
		free( freeString );
	}
}

/*
 * MiddleEnterIp
 */
 
MiddleEnterIp::
MiddleEnterIp()
{
	window = ApCreateModule( ABM_winEnterIp, 0, 0 );

	PtSetResource( ABW_ptLabelEnterIpBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleEnterIp::
Show()
{
	MiddleSection::Show();
	
	PtWindowFocus( window );

	PtGiveFocus( ABW_ptTextEnterIp, 0 );
}

void
MiddleEnterIp::
SetData( GuiReceiveMsgData_t * data )
{
	PtSetResource( ABW_ptTextEnterIp, Pt_ARG_TEXT_STRING, data->middleEnterIp.text, 0 );
}

void
MiddleEnterIp::
Update( GuiReceiveMsg_t * msg )
{
	PtSetResource( ABW_ptTextEnterIp, Pt_ARG_TEXT_STRING, msg->data.middleEnterIp.text, 0 );
}

/*
 * MiddleEnterRange
 */
MiddleEnterRange::
MiddleEnterRange()
{
	window = ApCreateModule( ABM_winEnterRange, 0, 0 );
	
	PtSetResource( ABW_ptLabelEnterRangeBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleEnterRange::
Show()
{
	MiddleSection::Show();
	
	PtWindowFocus( window );
}

void
MiddleEnterRange::
SetData( GuiReceiveMsgData_t * data )
{
	EnterRange::Init( &data->middleEnterRange );
}


/*
 * FUNCTION
 * 	MiddleEnterRange::SendRange
 * 
 * DESCRIPTION
 * 	Sends the range to the controlling process.
 */

void
MiddleEnterRange::
SendRange()
{
	GuiSendRangeData_t range;

	// Get the range from the EnterRange window.
	EnterRange::GetRange( &range.lower, &range.upper );

	// Send the range.
	Comm::SendMessage( MSG_GUI_OUT_RANGE, range );
}

/*
 * MiddleEnterTime
 */
 
MiddleEnterTime::
MiddleEnterTime()
{
	window = ApCreateModule( ABM_winEnterTime, 0, 0 );

	hoursLabel = ABW_ptLabelEnterTimeHour;
	minutesLabel = ABW_ptLabelEnterTimeMinute;
	formatButton = ABW_ptButtonEnterTimeFormat;
	zeroSecButton = ABW_ptButtonEnterTimeZeroSec;

	PtSetResource( ABW_ptLabelEnterTimeBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleEnterTime::
Show()
{
	EnterTime::Start();
	
	MiddleSection::Show();
	
	PtWindowFocus( window );
}

void
MiddleEnterTime::
Hide()
{
	MiddleSection::Hide();
	EnterTime::Stop();
}

void
MiddleEnterTime::
SetData( GuiReceiveMsgData_t * data )
{
	EnterTime::SetFormat( data->middleEnterTime.twentyFourHourFormat );
	
	SetString( hoursLabel, data->middleEnterTime.hoursLabel );
	SetString( minutesLabel, data->middleEnterTime.minutesLabel );
	SetString( formatButton, data->middleEnterTime.formatButtonText, true );
	SetString( zeroSecButton, data->middleEnterTime.zeroSecButtonText, true );
}

/*
 * MiddleEnterValue
 */
 
MiddleEnterValue::
MiddleEnterValue()
{
	window = ApCreateModule( ABM_winEnterValue, 0, 0 );

	currentLabel = ABW_ptLabelEnterValueCurrent;
	newLabel = ABW_ptLabelEnterValueNew;
	currentValue = ABW_ptLabelEnterValueCurrentValue;
	newValue = ABW_ptTextEnterValueNew;
	button0 = ABW_ptButtonEnterValue0;
	button1 = ABW_ptButtonEnterValue1;
	checkButtons[0] = ABW_ptButtonEnterValue2;
	checkButtons[1] = ABW_ptButtonEnterValue3;

	PtSetResource( ABW_ptLabelEnterValueBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleEnterValue::
Show()
{
	MiddleSection::Show();
	
	PtWindowFocus( window );
}

void
MiddleEnterValue::
SetData( GuiReceiveMsgData_t * data )
{
	long *	flags;
	int		i;

	SetString( currentLabel, data->middleEnterValue.currentLabel );
	SetString( newLabel, data->middleEnterValue.newLabel );
	SetString( currentValue, data->middleEnterValue.currentValue );
	SetString( newValue, data->middleEnterValue.newValue );
	SetString( button0, data->middleEnterValue.button0Text, true );
	SetString( button1, data->middleEnterValue.button1Text, true );

	for ( i = 0; i < 2; i++ )
	{
		if ( data->middleEnterValue.checkButtonText[ i ][0] != 0  )
		{
			PtRealizeWidget( checkButtons[ i ] );
			SetString( checkButtons[i], data->middleEnterValue.checkButtonText[ i ], true );
			
			if ( data->middleEnterValue.buttonChecked[ i ] )
			{
				PtGetResource( checkButtons[ i ], Pt_ARG_FLAGS, &flags, 0 );
				*flags |= Pt_SET;
			}
			else
			{
				PtGetResource( checkButtons[ i ], Pt_ARG_FLAGS, &flags, 0 );
				*flags &= ~Pt_SET;
			}
		}
		else
		{
			PtUnrealizeWidget( checkButtons[i] );
		}
	}
}

void
MiddleEnterValue::
Update( GuiReceiveMsg_t * msg )
{
//	long *	flags;
//	int		i;

	SetString( newValue, msg->data.middleEnterValue.newValue );
	
// GLD - 09/26/2011
// The Settings process is sending a message with both buttonChecked values cleared, so if this
// code below actually worked, it'd clear both buttons. However, since they're radio buttons, the
// only way you can actually clear one is to set the other - so this code never did actually did
// anything.
/*
	// This won't have any visible effect if the buttons aren't visible.
	for ( i = 0; i < 2; i++ )
	{
		if ( msg->data.middleEnterValue.buttonChecked[ i ] )
		{
			PtGetResource( checkButtons[ i ], Pt_ARG_FLAGS, &flags, 0 );
			*flags |= Pt_SET;
		}
		else
		{
			PtGetResource( checkButtons[ i ], Pt_ARG_FLAGS, &flags, 0 );
			*flags &= ~Pt_SET;
		}
	}
*/
}

/*
 * MiddleGraphicInfo1
 */
 
MiddleGraphicInfo1::
MiddleGraphicInfo1()
{
	window = ApCreateModule( ABM_winGraphicInfo1, 0, 0 );
	
	text = ABW_ptMultiTextGraphicInfo1;

	PtSetResource( ABW_ptLabelGraphicInfo1Bkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
	
	// We have to disable range selection here because we can't do it in PhAB.
	PtSetResource( text, Pt_ARG_TEXT_FLAGS, Pt_NO_RANGE_SELECTION, Pt_NO_RANGE_SELECTION );
}

void
MiddleGraphicInfo1::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( text, data->middleViewResults.text );
}

/*
 * MiddleGraphicInfo2
 */
 
MiddleGraphicInfo2::
MiddleGraphicInfo2()
{
	window = ApCreateModule( ABM_winGraphicInfo2, 0, 0 );
	
	text = ABW_ptMultiTextGraphicInfo2;

	PtSetResource( ABW_ptLabelGraphicInfo2Bkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
	
	// We have to disable range selection here because we can't do it in PhAB.
	PtSetResource( text, Pt_ARG_TEXT_FLAGS, Pt_NO_RANGE_SELECTION, Pt_NO_RANGE_SELECTION );
}

void
MiddleGraphicInfo2::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( text, data->middleViewResults.text );
}

/*
 * MiddleGraphicInfo3
 */
 
MiddleGraphicInfo3::
MiddleGraphicInfo3()
{
	window = ApCreateModule( ABM_winGraphicInfo3, 0, 0 );
	
	text = ABW_ptMultiTextGraphicInfo3;

	PtSetResource( ABW_ptLabelGraphicInfo3Bkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
	
	// We have to disable range selection here because we can't do it in PhAB.
	PtSetResource( text, Pt_ARG_TEXT_FLAGS, Pt_NO_RANGE_SELECTION, Pt_NO_RANGE_SELECTION );
}

void
MiddleGraphicInfo3::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( text, data->middleViewResults.text );
}

/*
 * MiddleHomeReady
 */
 
MiddleHomeReady::
MiddleHomeReady()
{
	window = ApCreateModule( ABM_winHomeReady, 0, 0 );
	
	buttons[0] = ABW_ptButtonHomeReadyButton0;
	buttons[1] = ABW_ptButtonHomeReadyButton1;
	buttons[2] = ABW_ptButtonHomeReadyButton2;
	
	PtSetResource( ABW_ptLabelHomeReadyBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleHomeReady::
SetData( GuiReceiveMsgData_t * data )
{
	for ( int i = 0; i < 3; i++ )
	{
		if ( data->middleHomeReady.buttonText[i][0] != 0  )
		{
			PtRealizeWidget( buttons[i] );
			SetString( buttons[i], data->middleHomeReady.buttonText[i], true );
			
			if ( data->middleHomeReady.buttonHighlighted[i] )
			{
				PtSetResource( buttons[i], Pt_ARG_FILL_COLOR, ORANGE_HILITE, 0 );
			}
			else
			{
				PtSetResource( buttons[i], Pt_ARG_FILL_COLOR, ORANGE_NORMAL, 0 );
			}
		}
		else
		{
			PtUnrealizeWidget( buttons[i] );
		}
	}
}

/*
 * MiddleHomeReadyMfg
 */
 
MiddleHomeReadyMfg::
MiddleHomeReadyMfg()
{
	window = ApCreateModule( ABM_winMfgHomeReady, 0, 0 );
	
	buttons[0] = ABW_ptButtonMfgHomeReadyButton0;
	buttons[1] = ABW_ptButtonMfgHomeReadyButton1;
	
	PtSetResource( ABW_ptLabelMfgHomeReadyBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleHomeReadyMfg::
SetData( GuiReceiveMsgData_t * data )
{
	for ( int i = 0; i < 2; i++ )
	{
		if ( data->middleHomeReady.buttonText[i][0] != 0  )
		{
			PtRealizeWidget( buttons[i] );
			SetString( buttons[i], data->middleHomeReady.buttonText[i], true );
			
			if ( data->middleHomeReady.buttonHighlighted[i] )
			{
				PtSetResource( buttons[i], Pt_ARG_FILL_COLOR, ORANGE_HILITE, 0 );
			}
			else
			{
				PtSetResource( buttons[i], Pt_ARG_FILL_COLOR, ORANGE_NORMAL, 0 );
			}
		}
		else
		{
			PtUnrealizeWidget( buttons[i] );
		}
	}
}

/*
 * MiddleHomeWarming
 */
 
MiddleHomeWarming::
MiddleHomeWarming()
{
	window = ApCreateModule( ABM_winHomeWarming, 0, 0 );
	
	label = ABW_ptLabelHomeWarming;
	progBar = ABW_ptProgressHomeWarming;
	
	PtSetResource( ABW_ptLabelHomeWarmingBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleHomeWarming::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( label, data->middleHomeWarming.progBarText, true );
	PtSetResource( progBar, Pt_ARG_GAUGE_VALUE_PREFIX, data->middleHomeWarming.progBarText, 0 );
	PtSetResource( progBar, Pt_ARG_GAUGE_VALUE, data->middleHomeWarming.progBarValue, 0 );
}

void
MiddleHomeWarming::
Update( GuiReceiveMsg_t * msg )
{
	PtSetResource( progBar, Pt_ARG_GAUGE_VALUE, msg->data.middleHomeWarming.progBarValue, 0 );
}

/*
 * MiddleHomeWarmingMfg
 */
 
MiddleHomeWarmingMfg::
MiddleHomeWarmingMfg()
{
	window = ApCreateModule( ABM_winMfgHomeWarming, 0, 0 );
	
	label = ABW_ptLabelMfgHomeWarming;
	progBar = ABW_ptProgressMfgHomeWarming;
	
	PtSetResource( ABW_ptLabelMfgHomeWarmingBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleHomeWarmingMfg::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( label, data->middleHomeWarming.progBarText, true );
	PtSetResource( progBar, Pt_ARG_GAUGE_VALUE_PREFIX, data->middleHomeWarming.progBarText, 0 );
	PtSetResource( progBar, Pt_ARG_GAUGE_VALUE, data->middleHomeWarming.progBarValue, 0 );
}

void
MiddleHomeWarmingMfg::
Update( GuiReceiveMsg_t * msg )
{
	PtSetResource( progBar, Pt_ARG_GAUGE_VALUE, msg->data.middleHomeWarming.progBarValue, 0 );
}

/*
 * MiddleIconMatrix1
 */

MiddleIconMatrix1::
MiddleIconMatrix1()
{
	window = ApCreateModule( ABM_winIconMatrix1, 0, 0 );

	PtSetResource( ABW_ptLabelIconMatrix1Bkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

/*
 * MiddleIconMatrix2
 */
 
MiddleIconMatrix2::
MiddleIconMatrix2()
{
	window = ApCreateModule( ABM_winIconMatrix2, 0, 0 );

	PtSetResource( ABW_ptLabelIconMatrix2Bkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
	
	if ( factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE )
	{
		PtUnrealizeWidget( ABW_ptButtonIconMatrixSecurity );
	}
}

/*
 * MiddleList
 */
 
void
MiddleList::
Show()
{
	SetColumns( 216, 0, 0 );
	
	MiddleListParent::Show();
}

void
MiddleList::
SetData( GuiReceiveMsgData_t * data )
{
	int		numItems;
	char *	items[5];

	PtListDeleteAllItems( list );
	for ( numItems = 0; numItems < 5; numItems++ )
	{
		if ( data->middleList.items[numItems][0][0] == 0 )
		{
			break;
		}
		items[numItems] = data->middleList.items[numItems][0];
	}
	SetList( list, (const char **)items, numItems );
	
	// Enable/disable the up/down buttons.
	if ( data->middleList.hasUpDown )
	{
		PtRealizeWidget( ABW_ptButtonListUp );
		PtRealizeWidget( ABW_ptButtonListDown );
	}
	else
	{
		PtUnrealizeWidget( ABW_ptButtonListUp );
		PtUnrealizeWidget( ABW_ptButtonListDown );
	}		
	
	PtSetResource( list, Pt_ARG_SELECTION_MODE,
		data->middleList.isSelectable ? Pt_SINGLE_MODE : Pt_SELECTION_MODE_NONE, 0 );
}

void
MiddleList::
Update( GuiReceiveMsg_t * msg )
{
	SetData( &msg->data );
}

/*
 * MiddleListColored
 */

MiddleListColored::
MiddleListColored()
{
	window = ApCreateModule( ABM_winListColored, 0, 0 );

	buttons[0] = ABW_PtButtonListColored0;
	buttons[1] = ABW_PtButtonListColored1;
	buttons[2] = ABW_PtButtonListColored2;
	buttons[3] = ABW_PtButtonListColored3;
	buttons[4] = ABW_PtButtonListColored4;

	PtSetResource( ABW_ptLabelListColoredBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleListColored::
SetData( GuiReceiveMsgData_t * data )
{
	for ( int i = 0; i < 5; i++ )
	{
		if ( data->middleListColored.items[i][0] != 0  )
		{
			PtRealizeWidget( buttons[i] );
			SetString( buttons[i], data->middleListColored.items[i], true );
			PtSetResource( buttons[i], Pt_ARG_COLOR, data->middleListColored.fontColor[i], 0 );
			PtSetResource( buttons[i], Pt_ARG_FILL_COLOR, data->middleListColored.backgroundColor[i], 0 );
		}
		else
		{
			PtUnrealizeWidget( buttons[i] );
		}
	}
}

/*
 * MiddleListMulti
 */

MiddleListMulti::
MiddleListMulti()
{
	window = ApCreateModule( ABM_winListMulti, 0, 0 );
	
	list = ABW_ptListListMulti;

	PtSetResource( ABW_ptLabelListMultiBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleListMulti::
SetData( GuiReceiveMsgData_t * data )
{
	int		numItems;
	int		i;
	char *	items[5];

	PtListDeleteAllItems( list );
	for ( numItems = 0; numItems < 5; numItems++ )
	{
		if ( data->middleListMulti.items[numItems][0] == 0 )
		{
			break;
		}
		items[ numItems ] = data->middleListMulti.items[ numItems ];
	}
	SetList( list, (const char **)items, numItems );
	
	for ( i = 0; i < numItems; i++ )
	{
		if ( data->middleListMulti.isSelected[ i ] )
		{
			// The list indexes from 1.
			PtListSelectPos( list, i+1 );
		}
	}
	
	// Enable/disable the up/down buttons.
	if ( data->middleListMulti.hasUpDown )
	{
		PtRealizeWidget( ABW_ptButtonListMultiUp );
		PtRealizeWidget( ABW_ptButtonListMultiDown );
	}
	else
	{
		PtUnrealizeWidget( ABW_ptButtonListMultiUp );
		PtUnrealizeWidget( ABW_ptButtonListMultiDown );
	}
}

void
MiddleListMulti::
Update( GuiReceiveMsg_t * msg )
{
	SetData( &msg->data );
}

/*
 * MiddleListRefRanges
 */

void
MiddleListRefRanges::
Show()
{
	SetColumns( 101, 70, 4 );
	
	MiddleListParent::Show();
}

void
MiddleListRefRanges::
SetData( GuiReceiveMsgData_t * data )
{
	char	itemBuff[5][100];
	char *	items[5];
	int		numItems;
	
	PtListDeleteAllItems( list );
	for ( numItems = 0; numItems < 5; numItems++ )
	{
		if ( data->middleList.items[numItems][0][0] == 0 )
		{
			break;
		}
		strcpy( itemBuff[numItems], data->middleList.items[numItems][0] );
		strcat( itemBuff[numItems], "\t" );
		strcat( itemBuff[numItems], data->middleList.items[numItems][1] );
		strcat( itemBuff[numItems], "\t" );
		strcat( itemBuff[numItems], data->middleList.items[numItems][2] );
		items[numItems] = itemBuff[numItems];
	}
	SetList( list, (const char **)items, numItems );
	
	// Enable/disable the up/down buttons.
	if ( data->middleList.hasUpDown )
	{
		PtRealizeWidget( ABW_ptButtonListUp );
		PtRealizeWidget( ABW_ptButtonListDown );
	}
	else
	{
		PtUnrealizeWidget( ABW_ptButtonListUp );
		PtUnrealizeWidget( ABW_ptButtonListDown );
	}		

	PtSetResource( list, Pt_ARG_SELECTION_MODE,
		data->middleList.isSelectable ? Pt_SINGLE_MODE : Pt_SELECTION_MODE_NONE, 0 );
}

void
MiddleListRefRanges::
Update( GuiReceiveMsg_t * msg )
{
	MiddleListRefRanges::SetData( &msg->data );
}

/*
 * MiddleListResults
 */
 
void
MiddleListResults::
Show()
{
	SetColumns( 16, 84, 116 );

	MiddleListParent::Show();
}

void
MiddleListResults::
SetData( GuiReceiveMsgData_t * data )
{
	char	itemBuff[5][100];
	char *	items[5];
	int		numItems;
	
	PtListDeleteAllItems( list );
	for ( numItems = 0; numItems < 5; numItems++ )
	{
		if ( data->middleList.items[numItems][0][0] == 0 )
		{
			break;
		}
		strcpy( itemBuff[numItems], data->middleList.items[numItems][0] );
		strcat( itemBuff[numItems], "\t" );
		strcat( itemBuff[numItems], data->middleList.items[numItems][1] );
		strcat( itemBuff[numItems], "\t" );
		strcat( itemBuff[numItems], data->middleList.items[numItems][2] );
		items[numItems] = itemBuff[numItems];
	}
	SetList( list, (const char **)items, numItems );
	
	// Enable/disable the up/down buttons.
	if ( data->middleList.hasUpDown )
	{
		PtRealizeWidget( ABW_ptButtonListUp );
		PtRealizeWidget( ABW_ptButtonListDown );
	}
	else
	{
		PtUnrealizeWidget( ABW_ptButtonListUp );
		PtUnrealizeWidget( ABW_ptButtonListDown );
	}		
	
	PtSetResource( list, Pt_ARG_SELECTION_MODE,
		data->middleList.isSelectable ? Pt_SINGLE_MODE : Pt_SELECTION_MODE_NONE, 0 );
}

void
MiddleListResults::
Update( GuiReceiveMsg_t * msg )
{
	MiddleListResults::SetData( &msg->data );
}

/*
 * MiddleListSettings
 */
 
void
MiddleListSettings::
Show()
{
	SetColumns( 108, 108, 0 );
	
	MiddleListParent::Show();
}

void
MiddleListSettings::
SetData( GuiReceiveMsgData_t * data )
{
	char	itemBuff[5][100];
	char *	items[5];
	int		numItems;
	
	PtListDeleteAllItems( list );
	for ( numItems = 0; numItems < 5; numItems++ )
	{
		if ( data->middleList.items[numItems][0][0] == 0 )
		{
			break;
		}
		strcpy( itemBuff[numItems], data->middleList.items[numItems][0] );
		strcat( itemBuff[numItems], "\t" );
		strcat( itemBuff[numItems], data->middleList.items[numItems][1] );
		items[numItems] = itemBuff[numItems];
	}
	SetList( list, (const char **)items, numItems );
	
	// Enable/disable the up/down buttons.
	if ( data->middleList.hasUpDown )
	{
		PtRealizeWidget( ABW_ptButtonListUp );
		PtRealizeWidget( ABW_ptButtonListDown );
	}
	else
	{
		PtUnrealizeWidget( ABW_ptButtonListUp );
		PtUnrealizeWidget( ABW_ptButtonListDown );
	}		
	
	PtSetResource( list, Pt_ARG_SELECTION_MODE,
		data->middleList.isSelectable ? Pt_SINGLE_MODE : Pt_SELECTION_MODE_NONE, 0 );
}

void
MiddleListSettings::
Update( GuiReceiveMsg_t * msg )
{
	MiddleListSettings::SetData( &msg->data );
}

/*
 * MiddleLoadOrder
 */

MiddleLoadOrder::
MiddleLoadOrder()
{
	window = ApCreateModule( ABM_winLoadOrder, 0, 0 );

	orderLabel = ABW_ptLabelLoadOrderLabel;
	rotorName = ABW_ptLabelLoadOrderRotorName;
	button    = ABW_ptButtonLoadOrder;

	PtSetResource( ABW_ptLabelLoadOrderBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleLoadOrder::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( orderLabel, data->middleLoadOrder.orderLabel, true );
	SetString( rotorName, data->middleLoadOrder.rotorName, true );
	PtSetResource( rotorName, Pt_ARG_FILL_COLOR, data->middleLoadOrder.backgroundColor, 0 );
	PtSetResource( rotorName, Pt_ARG_COLOR, data->middleLoadOrder.fontColor, 0 );

	if ( data->middleLoadOrder.buttonText[0] != 0  )
	{
		PtRealizeWidget( button );
		SetString( button, data->middleLoadOrder.buttonText, true );
	}
	else
	{
		PtUnrealizeWidget( button );
	}

}

/*
 * MiddleOptionInfo
 */
 
MiddleOptionInfo::
MiddleOptionInfo()
{
	window = ApCreateModule( ABM_winOptionInfo, 0, 0 );

	buttons[0] = ABW_ptButtonOptionInfo0;
	buttons[1] = ABW_ptButtonOptionInfo1;
	buttons[2] = ABW_ptButtonOptionInfo2;
	buttons[3] = ABW_ptButtonOptionInfo3;

	PtSetResource( ABW_ptLabelOptionInfoBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleOptionInfo::
SetData( GuiReceiveMsgData_t * data )
{
	long *	flags;

	for ( int i = 0; i < 4; i++ )
	{
		if ( data->middleOptionInfo.buttonText[i][0] != 0  )
		{
			PtRealizeWidget( buttons[i] );
			SetString( buttons[i], data->middleOptionInfo.buttonText[i], true );
			
			if ( data->middleOptionInfo.selectedButton == i )
			{
				PtGetResource( buttons[i], Pt_ARG_FLAGS, &flags, 0 );
				*flags |= Pt_SET;
			}
			else
			{
				PtGetResource( buttons[i], Pt_ARG_FLAGS, &flags, 0 );
				*flags &= ~Pt_SET;
			}
		}
		else
		{
			PtUnrealizeWidget( buttons[i] );
		}
	}
}

/*
 * MiddleMessage
 */		

MiddleMessage::
MiddleMessage()
{
	window = ApCreateModule( ABM_winMessage, 0, 0 );
	
	text = ABW_ptLabelMessage;

	PtSetResource( ABW_ptLabelMessageBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleMessage::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( text, data->middleMessage.message, true );
}

/*
 * MiddleMessageInfo
 */
 
MiddleMessageInfo::
MiddleMessageInfo()
{
	window = ApCreateModule( ABM_winMessageInfo, 0, 0 );

	text = ABW_ptLabelMessageInfo;

	PtSetResource( ABW_ptLabelMessageInfoBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleMessageInfo::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( text, data->middleMessage.message, true );
}


/*
 * MiddleMultiInfo
 */

MiddleMultiInfo::
MiddleMultiInfo()
{
	window = ApCreateModule( ABM_winMultiInfo, 0, 0 );

	buttons[0] = ABW_ptButtonMultiInfo0;
	buttons[1] = ABW_ptButtonMultiInfo1;
	buttons[2] = ABW_ptButtonMultiInfo2;
	buttons[3] = ABW_ptButtonMultiInfo3;

	PtSetResource( ABW_ptLabelMultiInfoBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleMultiInfo::
SetData( GuiReceiveMsgData_t * data )
{
	long *	flags;

	for ( int i = 0; i < 4; i++ )
	{
		if ( data->middleMultiInfo.buttonText[i][0] != 0  )
		{
			PtRealizeWidget( buttons[i] );
			SetString( buttons[i], data->middleMultiInfo.buttonText[i], true );
			
			if ( data->middleMultiInfo.buttonChecked[i] )
			{
				PtGetResource( buttons[i], Pt_ARG_FLAGS, &flags, 0 );
				*flags |= Pt_SET;
			}
			else
			{
				PtGetResource( buttons[i], Pt_ARG_FLAGS, &flags, 0 );
				*flags &= ~Pt_SET;
			}
		}
		else
		{
			PtUnrealizeWidget( buttons[i] );
		}
	}
}


/*
 * MiddleMultiSelect
 */

MiddleMultiSelect::
MiddleMultiSelect()
{
	window = ApCreateModule( ABM_winMultiSelect, 0, 0 );

	buttons[0] = ABW_ptButtonMultiSelect0;
	buttons[1] = ABW_ptButtonMultiSelect1;
	buttons[2] = ABW_ptButtonMultiSelect2;
	buttons[3] = ABW_ptButtonMultiSelect3;
	buttons[4] = ABW_ptButtonMultiSelect4;
	buttons[5] = ABW_ptButtonMultiSelect5;

	PtSetResource( ABW_ptLabelMultiSelectBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleMultiSelect::
SetData( GuiReceiveMsgData_t * data )
{
	long *	flags;

	for ( int i = 0; i < 6; i++ )
	{
		if ( data->middleMultiSelect.buttonText[i][0] != 0  )
		{
			PtRealizeWidget( buttons[i] );
			SetString( buttons[i], data->middleMultiSelect.buttonText[i], true );
			
			if ( data->middleMultiSelect.buttonChecked[i] )
			{
				PtGetResource( buttons[i], Pt_ARG_FLAGS, &flags, 0 );
				*flags |= Pt_SET;
			}
			else
			{
				PtGetResource( buttons[i], Pt_ARG_FLAGS, &flags, 0 );
				*flags &= ~Pt_SET;
			}
		}
		else
		{
			PtUnrealizeWidget( buttons[i] );
		}
	}
}

/*
 * MiddleProgBarMessage
 */
 
MiddleProgBarMessage::
MiddleProgBarMessage()
{
	window = ApCreateModule( ABM_winProgBarMessage, 0, 0 );

	label = ABW_ptLabelProgBarMessageLabel;
	text = ABW_ptLabelProgBarMessage;
	progBar = ABW_ptProgressProgBarMessage;

	PtSetResource( ABW_ptLabelProgBarMessageBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleProgBarMessage::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( label, data->middleProgBarMessage.progBarText, true );
	SetString( text,  data->middleProgBarMessage.message, true );
//	PtSetResource( text, Pt_ARG_FILL_COLOR, data->middleProgBarMessage.backgroundColor, 0 );
//	PtSetResource( text, Pt_ARG_COLOR, data->middleProgBarMessage.fontColor, 0 );
	PtSetResource( progBar, Pt_ARG_GAUGE_VALUE, data->middleProgBarMessage.progBarValue, 0 );
}

void
MiddleProgBarMessage::
Update( GuiReceiveMsg_t * msg )
{
	switch ( msg->msgId )
	{
		case MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS:
			PtSetResource( progBar, Pt_ARG_GAUGE_VALUE, msg->data.middleProgBarMessage.progBarValue, 0 );
			break;
		case MSG_GUI_UPDATE_PROG_BAR_MESSAGE_TEXT:
			SetString( text, msg->data.middleProgBarMessage.message, true );
//			PtSetResource( text, Pt_ARG_FILL_COLOR, msg->data.middleProgBarMessage.backgroundColor, 0 );
//			PtSetResource( text, Pt_ARG_COLOR, msg->data.middleProgBarMessage.fontColor, 0 );
			break;
		default:
			break;
	}		
}

/*
 * MiddleSelectOption
 */
 
MiddleSelectOption::
MiddleSelectOption()
{
	window = ApCreateModule( ABM_winSelectOption, 0, 0 );

	buttons[0] = ABW_ptButtonSelectOption0;
	buttons[1] = ABW_ptButtonSelectOption1;
	buttons[2] = ABW_ptButtonSelectOption2;
	buttons[3] = ABW_ptButtonSelectOption3;
	buttons[4] = ABW_ptButtonSelectOption4;
	buttons[5] = ABW_ptButtonSelectOption5;

	PtSetResource( ABW_ptLabelSelectOptionBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleSelectOption::
SetData( GuiReceiveMsgData_t * data )
{
	long *	flags;

	for ( int i = 0; i < 6; i++ )
	{
		if ( data->middleSelectOption.buttonText[i][0] != 0  )
		{
			PtRealizeWidget( buttons[i] );
			SetString( buttons[i], data->middleSelectOption.buttonText[i], true );
			
			if ( data->middleSelectOption.selectedButton == i )
			{
				PtGetResource( buttons[i], Pt_ARG_FLAGS, &flags, 0 );
				*flags |= Pt_SET;
			}
			else
			{
				PtGetResource( buttons[i], Pt_ARG_FLAGS, &flags, 0 );
				*flags &= ~Pt_SET;
			}
		}
		else
		{
			PtUnrealizeWidget( buttons[i] );
		}
	}
}

/*
 * MiddleSelectType
 */
 
MiddleSelectType::
MiddleSelectType()
{
	window = ApCreateModule( ABM_winSelectType, 0, 0 );

	buttons[0] = ABW_ptButtonSelectType0;
	buttons[1] = ABW_ptButtonSelectType1;
	buttons[2] = ABW_ptButtonSelectType2;
	buttons[3] = ABW_ptButtonSelectType3;
	buttons[4] = ABW_ptButtonSelectType4;
	buttons[5] = ABW_ptButtonSelectType5;

	PtSetResource( ABW_ptLabelSelectTypeBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
}

void
MiddleSelectType::
SetData( GuiReceiveMsgData_t * data )
{
	long *	flags;
	int		i;

	for ( i = 0; i < 4; i++ )
	{
		if ( data->middleSelectType.buttonText[i][0] != 0  )
		{
			PtRealizeWidget( buttons[i] );
			SetString( buttons[i], data->middleSelectType.buttonText[i], true );
		}
		else
		{
			PtUnrealizeWidget( buttons[i] );
		}
	}
	
	for ( i = 4; i < 6; i++ )
	{
		SetString( buttons[i], data->middleSelectType.buttonText[i], true );
		if ( data->middleSelectType.buttonChecked[i-4] )
		{
			PtGetResource( buttons[i], Pt_ARG_FLAGS, &flags, 0 );
			*flags |= Pt_SET;
		}
		else
		{
			PtGetResource( buttons[i], Pt_ARG_FLAGS, &flags, 0 );
			*flags &= ~Pt_SET;
		}
	}
}

/*
 * MiddleViewResults
 */
 
MiddleViewResults::
MiddleViewResults()
{
	window = ApCreateModule( ABM_winViewResults, 0, 0 );
	
	text = ABW_ptMultiTextViewResults;

	PtSetResource( ABW_ptLabelViewResultsBkgd, Pt_ARG_LABEL_IMAGE, bkgdImage, 0 );
	
	// We have to disable range selection here because we can't do it in PhAB.
	PtSetResource( text, Pt_ARG_TEXT_FLAGS, Pt_NO_RANGE_SELECTION, Pt_NO_RANGE_SELECTION );
}

void
MiddleViewResults::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( text, data->middleViewResults.text );
}

