/*
 * FILENAME
 * 	ViewResults.h
 * 
 * MODULE DESCRIPTION
 * 	Class definition for the ViewResults class, which handles scrolling of the
 * 	Middle View Results subscreen.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __VIEWRESULTS_H_INCLUDED
#define __VIEWRESULTS_H_INCLUDED

class ViewResults
{
public:
	static int ScrollUp( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int ScrollDown( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
};

#endif
