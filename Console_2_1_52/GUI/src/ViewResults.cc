/*
 * FILENAME
 * 	ViewResults.cc
 * 
 * MODULE DESCRIPTION
 * 	Class method definitions for the ViewResults class, which controls the
 * 	scrolling of analysis results display.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "Global.h"
#include "abimport.h"
#include "Sound.h"

/*
 * FUNCTION
 * 	ViewResults::ScrollUp
 * 
 * DESCRIPTION
 * 	Callback for the scroll up button.  Scrolls the results window up one
 * 	window-full if it's not at the top.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon return value.
 */

int
ViewResults::ScrollUp( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t * mt = ABW_ptMultiTextViewResults;
	long *topLine;
	
	PtGetResource( mt, Pt_ARG_MULTITEXT_TOP_LINE, &topLine, 0 );
	
	if ( *topLine > 1 )
	{
		PtSetResource( mt, Pt_ARG_MULTITEXT_TOP_LINE, *topLine == 2 ? 1 : *topLine - 2, 0 );
	}
	
	Click();
	
	return Pt_CONTINUE;
}


/*
 * FUNCTION
 * 	ViewResults::ScrollDown
 * 
 * DESCRIPTION
 * 	Callback for the scroll down button.  Scrolls the results window down one
 * 	window-full if it's not at the bottom.
 * 
 * PARAMETERS
 * 	Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon return value.
 */

int
ViewResults::ScrollDown( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	PtWidget_t * mt = ABW_ptMultiTextViewResults;
	long *topLine;
	long *numLines;
	short *visibleLines;
	
	PtGetResource( mt, Pt_ARG_MULTITEXT_TOP_LINE, &topLine, 0 );
	PtGetResource( mt, Pt_ARG_MULTITEXT_NUM_LINES, &numLines, 0 );
	PtGetResource( mt, Pt_ARG_MULTITEXT_NUM_LINES_VISIBLE, &visibleLines, 0 );
	
	/*
	 * Since we can have a "ragged" last line, we allow scrolling if the
	 * control thinks we're displaying the last line.
	 */
	if ( *topLine + *visibleLines <= *numLines )
	{
		PtSetResource( mt, Pt_ARG_MULTITEXT_TOP_LINE, *topLine + 2, 0 );
	}

	Click();
	
	return Pt_CONTINUE;
}

