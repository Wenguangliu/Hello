/*
 * FILENAME
 * 	WinSection.cc
 * 
 * MODULE DESCRIPTION
* 	Class method definitions for the full window classes.
  * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include "Global.h"
#include "WinSection.h"

bool	errorScreenActive_g = false;

/*
 * WinSection
 */
 
/*
 * FUNCTION
 * 	WinSection::Show
 * 
 * DESCRIPTION
 * 	Makes the associated window visible by moving it from its offscreen "hiding
 * 	place" to proper position on the visible screen.
 */

void
WinSection::
Show()
{
	if ( window != 0 )
	{
		PhPoint_t pos = {240, 0};
		PtSetResource( window, Pt_ARG_POS, &pos, 0 );
		PtSetResource( window, Pt_ARG_CURSOR_TYPE, cursorType_g, 0 );
		
		pthread_sleepon_lock();
		errorScreenActive_g = true;
		inactionTime_g = 0;
		pthread_sleepon_unlock();
	}
}

/*
 * FUNCTION
 * 	WinSection::Hide
 * 
 * DESCRIPTION
 * 	Makes the associated window invisible by moving it from the visible screen
 * 	to a "hiding place" off the screen.
 */

void
WinSection::
Hide()
{
	if ( window != 0 )
	{
		PhPoint_t pos = {-400, 0};
		PtSetResource( window, Pt_ARG_POS, &pos, 0 );

		/*
		 * We make this call so the background window will repaint
		 * before we draw the new window.  Otherwise, we end up with the new
		 * window painted on top of the old window.  Since the new window
		 * completely covers the old window (unlike the middle section), this
		 * isn't really needed here.  However, it doesn't hurt being here and
		 * it'll make sure we avoid any such issues that could come up in the
		 * future.
		 */
		PtBkgdHandlerProcess();

		pthread_sleepon_lock();
		errorScreenActive_g = false;
		pthread_sleepon_unlock();
	}
}

/*
 * WinWarning
 */
 
/*
 * FUNCTION
 * 	WinWarning1::WinWarning1
 * 
 * DESCRIPTION
 * 	Creates a link to the window created in PhAB and any child widgets that
 * 	may need to be manipulated.
 * 
 * NOTE
 * 	This description applies to all constructors for the subclasses of
 * 	WinSection.
 */

WinWarning1::
WinWarning1()
{
	window = ApCreateModule( ABM_winWarning1, 0, 0 );
	
	topLabel = ABW_ptLabelWarning1TopLabel;
	message = ABW_ptLabelWarning1Message;
	bottomButton = ABW_ptButtonWarning1Continue;
}

/*
 * FUNCTION
 * 	WinWarning1::SetData
 * 
 * DESCRIPTION
 * 	Sets the appropriate widget data for the child widgets in the associated
 * 	window.
 * 
 * PARAMETERS
 * 	data - Widget data, such as label text, received from the controlling
 * 			external process.
 * 
 * NOTE
 * 	This description applies to all SetData methods for the subclasses of
 * 	WinSection.
 */

void
WinWarning1::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( topLabel, data->winWarning.topLabel );
	SetString( message, data->winWarning.message, true );
	SetString( bottomButton, data->winWarning.buttonText[0], true );
}


/*
 * WinWarning2
 */

WinWarning2::
WinWarning2()
{
	window = ApCreateModule( ABM_winWarning2, 0, 0 );
	
	topLabel = ABW_ptLabelWarning2TopLabel;
	message = ABW_ptLabelWarning2Message;
	bottomButton[0] = ABW_ptButtonWarning2Continue;
	bottomButton[1] = ABW_ptButtonWarning2Cancel;
}

void
WinWarning2::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( topLabel, data->winWarning.topLabel );
	SetString( message, data->winWarning.message, true );
	SetString( bottomButton[0], data->winWarning.buttonText[0], true );
	SetString( bottomButton[1], data->winWarning.buttonText[1], true );
}

/*
 * WinError
 */
 
WinError::
WinError()
{
	window = ApCreateModule( ABM_winError, 0, 0 );
	
	topLabel = ABW_ptLabelErrorTopLabel;
	message = ABW_ptLabelErrorMessage;
	bottomButton = ABW_ptButtonErrorContinue;
}

void
WinError::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( topLabel, data->winError.topLabel );
	SetString( message, data->winError.message, true );
	SetString( bottomButton, data->winError.buttonText, true );
}

/*
 * WinCriticalStop
 */
 
WinCriticalStop::
WinCriticalStop()
{
	window = ApCreateModule( ABM_winCriticalStop, 0, 0 );
	
	topLabel = ABW_ptLabelCriticalStopTopLabel;
	message = ABW_ptLabelCriticalStopMessage;
}

void
WinCriticalStop::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( topLabel, data->winError.topLabel );
	SetString( message, data->winError.message, true );
}


/*
 * WinCriticalStop2
 */
 
WinCriticalStop2::
WinCriticalStop2()
{
	window = ApCreateModule( ABM_winCriticalStop2, 0, 0 );
	
	topLabel = ABW_ptLabelCriticalStop2TopLabel;
	message = ABW_ptLabelCriticalStop2Message;
	bottomButton = ABW_ptButtonCriticalStop2Restart;
}

void
WinCriticalStop2::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( topLabel, data->winError.topLabel );
	SetString( message, data->winError.message, true );
	SetString( bottomButton, data->winError.buttonText, true );
}

