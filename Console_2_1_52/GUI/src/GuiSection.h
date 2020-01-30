/*
 * FILENAME
 * 	GuipSection.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the GuiSection class.  This is a superclass for all
 * 	the GUI sections.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __GUISECTION_H_INCLUDED
#define __GUISECTION_H_INCLUDED

#include "ablibs.h"

class GuiSection
{
public:
	static void		SetString( PtWidget_t * widget, const char * str, bool doSpace = false );

protected:
	static void		SetList( PtWidget_t * widget, const char ** list, int numItems, int index = 1 );
};

#endif // __GUISECTION_H_INCLUDED
