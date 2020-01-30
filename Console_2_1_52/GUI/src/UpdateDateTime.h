/*
 * FILENAME
 * 	UpdateDateTime.h
 * 
 * MODULE DESCRIPTION
 * 	Class definition for the UpdateDateTime class, which handles updates to the
 * 	Bottom Date/Time screen when it's visible.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __UPDATEDATETIME_H_INCLUDED
#define __UPDATEDATETIME_H_INCLUDED

#include "GuiSection.h"

class UpdateDateTime
{
public:
	static void	Init( GuiBottomDateTimeData_t *msgData );
	static void	Start();
	static void	Stop();
	static int	Update( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
};

#endif
