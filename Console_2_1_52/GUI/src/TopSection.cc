/*
 * FILENAME
 * 	TopSection.cc
 * 
 * MODULE DESCRIPTION
 * 	Class method definitions for the top subscreen classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include "ablibs.h"
#include "abimport.h"
#include "Global.h"
#include "TopSection.h"

const int TOP_SECTION_X = 0;
const int TOP_SECTION_Y = 0;

/*
 * FUNCTION
 * 	TopTitle::TopTitle
 * 
 * DESCRIPTION
 * 	Creates the top title window.
 */

TopTitle::
TopTitle()
{
	window = ApCreateModule( ABM_winTopTitle, 0, 0 );
}

/*
 * FUNCTION
 * 	TopTitle::Show
 * 
 * DESCRIPTION
 * 	Makes the associated window visible by moving it from the "hiding place" off
 *  screen to the proper location on the visible screen.
 */

void
TopTitle::
Show()
{
	PhPoint_t pos = {240, 0};
	PtSetResource( window, Pt_ARG_POS, &pos, 0 );
	PtSetResource( window, Pt_ARG_CURSOR_TYPE, cursorType_g, 0 );
}

/*
 * FUNCTION
 * 	TopTitle::Hide
 * 
 * DESCRIPTION
 * 	Makes the associated window invisible by moving it from the visible screen
 * 	to a "hiding place" off screen.
 */

void
TopTitle::
Hide()
{
	PhPoint_t pos = {-400, 0};
	PtSetResource( window, Pt_ARG_POS, &pos, 0 );
}

/*
 * FUNCTION
 * 	TopTitle::SetData
 * 
 * DESCRIPTION
 * 	Sets the label in the top title.
 */

void
TopTitle::
SetData( GuiReceiveMsgData_t * data )
{
	SetString( ABW_ptLabelTopLabel, data->topLabel.label );
}
