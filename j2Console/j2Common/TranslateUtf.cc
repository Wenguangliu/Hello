#include <stdlib.h>
#include <string.h>

#include "TranslateUtf.h"

/*
 * FUNCTION
 * 	TranslateToUtf
 * 
 * DESCRIPTION
 * 	Translates a string from ISO-8859-1 to UTF-8.
 * 
 * PARAMETERS
 * 	str - The string to translate.
 * 
 * RETURNS
 * 	A translated version of the input string, or 0 if there is an error.
 * 
 * NOTE: The return value, if there is one, has been malloc'd and must be
 * 		 freed by the caller.
 */

char *
TranslateToUtf( const char * str )
{
	char *			utf = 0;

	// Since we're using the ISO-8859-1 charset, the very largest our result could
	// be is twice the source string.
	utf = (char *)malloc( strlen( str ) * 2 + 1 );
	if ( utf == 0 )
	{
		return 0;
	}

	return TranslateToUtf( str, utf );
}

/*
 * FUNCTION
 * 	TranslateToUtf
 * 
 * DESCRIPTION
 * 	Translates a string from ISO-8859-1 to UTF-8.
 * 
 * PARAMETERS
 * 	str - The string to translate.
 *  buff - Location in which to store the translated string.
 * 
 * RETURNS
 * 	A translated version of the input string, or 0 if there is an error.
 */

char *
TranslateToUtf( const char * str, char * buff )
{
	unsigned char *	src;
	char *			dest;

	for ( src = (unsigned char *)str, dest = buff; *src != '\0'; src++, dest++ )
	{
		if ( *src < 0x80 )
		{
			*dest = *src;
		}
		else
		{
			*dest = 0xC0 | (*src >> 6);
			*++dest = 0x80 | (*src & 0x3F);
		}
	}
	*dest = '\0';
	
	return buff;
}

/*
 * FUNCTION
 * 	TranslateFromUtf
 * 
 * DESCRIPTION
 * 	Translates a string from UTF-8 to ISO-8859-1.
 * 
 * PARAMETERS
 * 	str - The string to translate.
 * 
 * RETURNS
 * 	A translated version of the input string, or 0 if there is an error.
 * 
 * NOTE: The return value, if there is one, has been malloc'd and must be
 * 		 freed by the caller.
 */

char *
TranslateFromUtf( const char * str )
{
	char *			iso = 0;

	// The longest our result string will be is the same length as the source.
	iso = (char *)malloc( strlen( str ) + 1 );
	if ( iso == 0 )
	{
		return 0;
	}

	return TranslateFromUtf( str, iso );
}

char *
TranslateFromUtf( const char * str, char * buff )
{
	unsigned char *	src;
	char *			dest;

	for ( src = (unsigned char *)str, dest = buff; *src != '\0'; src++, dest++ )
	{
		// ASCII character
		if ( (*src & 0x80) == 0 )
		{
			*dest = *src;
		}
		// Latin-1 extended.
		else if ( (*src & 0xC0) == 0xC0 )
		{
//			*dest = ((*src << 6) & 0xC0) | (*++src & 0x3F);
			*dest = ((*src << 6) & 0xC0) | (*(src + 1) & 0x3F);
			src++;
		}
		// Unrecognized.
		else
		{
			*dest = '?';
		}
	}
	*dest = '\0';

	return buff;
}
