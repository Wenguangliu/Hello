/*
 * FILENAME
 * 	GuiSection.cc
 * 
 * MODULE DESCRIPTION
 * 	Class method definitions for the global GUI section classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */

#include "ablibs.h"
#include "abimport.h"
#include "Global.h"
#include "GuiSection.h"
#include "TranslateUtf.h"

/*
 * FUNCTION
 * 	GuiSection::SetString
 * 
 * DESCRIPTION
 * 	Sets a text string in a widget.  Handles adding padding spaces to work
 * 	around a problem with the STPC-90 video driver that clips the last column of
 *  pixels from the last character in a string and puts it at the beginning of
 * 	the first character in the string when anti-aliased characters are used.
 * 	It also translates from ISO-8859-1 (the single-byte extended Latin character
 *  set) to UTF-8, which is what Photon uses.
 * 
 * PARAMETERS
 * 	widget - The widget whose Pt_ARG_TEXT_STRING is to be set.
 * 	str - The string to manipulate and put in the widget.
 * 	doSpace - A flag to indicate whether padding spaces are needed.
 */

void
GuiSection::
SetString( PtWidget_t * widget, const char * str, bool doSpace )
{
	char			buff[ 1000 ];
	const char *	p;
	char *			utf;

// GLD
// This doesn't test for buffer overrun!!!

	if ( doSpace )
	{
		strcpy( buff, " " );
		
		while ( (p = strchr( str, '\n' )) != 0 )
		{
			strncat( buff, str, p - str );
			strcat( buff, " \n " );
			str = ++p;
		}
		strcat( buff, str );
		strcat( buff, " " );
		p = buff;
	}
	else
	{
		p = str;
	}

	// Do the ISO-8859-1 to UTF-8 translation.  If it succeeds, we use the
	// return value.  Else, we use the untranslated string.
	utf = TranslateToUtf( p );
	if ( utf != 0 )
	{
		p = utf;
	}

	// Set the string in the widget.
	PtSetResource( widget, Pt_ARG_TEXT_STRING, p, 0 );

	if ( utf != 0 )
	{
		free( utf );
	}
}


/*
 * FUNCTION
 * 	GuiSection::SetList
 * 
 * DESCRIPTION
 * 	Sets the text strings in a list.  Handles ISO-8859-1 to UTF-8 translation
 * 	like SetString.
 * 
 * PARAMETERS
 * 	widget - The widget whose Pt_ARG_TEXT_STRING is to be set.
 * 	list - The item strings.
 * 	numItems - Number of items in the list.
 * 	index - Entry number in the list widget where the new list should be
 * 			inserted.
 */

void
GuiSection::
SetList( PtWidget_t * widget, const char ** list, int numItems, int index )
{
	char *	p[ numItems ];
	char *	freeList[ numItems ];
	int		i;
	
	for ( i = 0; i < numItems; i++ )
	{
		freeList[ i ] = p[ i ] = TranslateToUtf( list[ i ] );
		if ( p[ i ] == 0 )
		{
			p[ i ] = (char *)list[ i ];
		}
	}

	// Put the translated list items into the list widget.
	PtListAddItems( widget, (const char **)p, numItems, index );
	
	// Free all the malloc'd objects.
	for ( i = 0; i < numItems; i++ )
	{
		if ( freeList[ i ] != 0 )
		{
			free( freeList[ i ] );
		}
	}
}
