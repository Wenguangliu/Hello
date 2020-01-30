/*
 * FILENAME
 * 	BottomSection.cc
 * 
 * MODULE DESCRIPTION
 * 	Class method definitions for the bottom subscreen classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include "Global.h"
#include "BottomSection.h"
#include "Colors.h"

/*
 * BottomSection
 */
 
/*
 * FUNCTION
 * 	BottomSection::Show
 * 
 * DESCRIPTION
 * 	Makes the associated window visible by moving it from its offscreen "hiding
 * 	place" to proper position on the visible screen.
 */

void
BottomSection::
Show()
{
	PhPoint_t pos = {240, 280};
	PtSetResource( window, Pt_ARG_POS, &pos, 0 );
	PtSetResource( window, Pt_ARG_CURSOR_TYPE, cursorType_g, 0 );
}

/*
 * FUNCTION
 * 	BottomSection::Hide
 * 
 * DESCRIPTION
 * 	Makes the associated window invisible by moving it from the visible screen
 * 	to a "hiding place" off the screen.
 */

void
BottomSection::
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
	}
}

/*
 * BottomButtons1
 */
 
/*
 * FUNCTION
 * 	BottomButtons1::BottomButtons1
 * 
 * DESCRIPTION
 * 	Creates a link to the window created in PhAB and any child widgets that
 * 	may need to be manipulated.
 * 
 * NOTE
 * 	This description applies to all constructors for the subclasses of
 * 	BottomSection.
 */

BottomButtons1::
BottomButtons1()
{
	window = ApCreateModule( ABM_winBot1Button, 0, 0 );
	
	button = ABW_ptButtonBottomSingle;
}

/*
 * FUNCTION
 * 	BottomButtons1::SetData
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
 * 	BottomSection.
 */

void
BottomButtons1::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( button, data->bottomButtons.buttonText[0], true );
		
	if ( data->bottomButtons.buttonHighlighted[0] )
	{
		PtSetResource( button, Pt_ARG_FILL_COLOR, BLUE_HILITE, 0 );
	}
	else
	{
		PtSetResource( button, Pt_ARG_FILL_COLOR, BLUE_NORMAL, 0 );
	}
}

/*
 * BottomButtons2
 */
 
BottomButtons2::
BottomButtons2()
{
	window = ApCreateModule( ABM_winBot2Button, 0, 0 );
	
	buttons[0] = ABW_ptButtonBottomDoubleLeft;
	buttons[1] = ABW_ptButtonBottomDoubleRight;
}

void
BottomButtons2::
SetData( GuiReceiveMsgData_t * data )
{
	for ( int i = 0; i < 2; i++ )
	{
		SetString( buttons[i], data->bottomButtons.buttonText[i], true );
			
		if ( data->bottomButtons.buttonHighlighted[i] )
		{
			PtSetResource( buttons[i], Pt_ARG_FILL_COLOR, BLUE_HILITE, 0 );
		}
		else
		{
			PtSetResource( buttons[i], Pt_ARG_FILL_COLOR, BLUE_NORMAL, 0 );
		}
	}
}

/*
 * BottomButtons3
 */
 
BottomButtons3::
BottomButtons3()
{
	window = ApCreateModule( ABM_winBot3Button, 0, 0 );
	
	buttons[0] = ABW_ptButtonBottomTripleLeft;
	buttons[1] = ABW_ptButtonBottomTripleCenter;
	buttons[2] = ABW_ptButtonBottomTripleRight;
}

void
BottomButtons3::
SetData( GuiReceiveMsgData_t * data )
{
	for ( int i = 0; i < 3; i++ )
	{
		SetString( buttons[i], data->bottomButtons.buttonText[i], true );
			
		if ( data->bottomButtons.buttonHighlighted[i] )
		{
			PtSetResource( buttons[i], Pt_ARG_FILL_COLOR, BLUE_HILITE, 0 );
		}
		else
		{
			PtSetResource( buttons[i], Pt_ARG_FILL_COLOR, BLUE_NORMAL, 0 );
		}
	}
}

/*
 * BottomDateTime
 */
 
BottomDateTime::
BottomDateTime()
{
	window = ApCreateModule( ABM_winBotDateTime, 0, 0 );
}

void
BottomDateTime::
Show()
{
	UpdateDateTime::Start();
	BottomSection::Show();
}

void
BottomDateTime::
Hide()
{
	BottomSection::Hide();
	UpdateDateTime::Stop();
}

void
BottomDateTime::
SetData( GuiReceiveMsgData_t * data )
{
// GLDGLDGLD
//	UpdateDateTime::Init( &data->bottomDateTime );
}
