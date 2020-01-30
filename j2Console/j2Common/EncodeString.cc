/*
 * FILENAME
 * 	EncodeString.cc
 * 
 * MODULE DESCRIPTION
 * 	Function(s) for encoding strings for XML files. Some characters, such as '<' and
 * 	'>', have special meanings in XML files, and so must be encoded when they're in
 * 	character constants stored in the files.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*
 * FUNCTION
 * 	EncodeString
 * 
 * DESCRIPTION
 * 	Encodes a string so it will store properly in an XML file.
 * 
 * PARAMETERS
 * 	buff - Location to place the encoded string.
 *  str - String to encode.
 */

void
EncodeString( char * buff, const char * str )
{
	unsigned	i;
	
    // Scan the input string, copying alphnumeric characters over directly and
    // encoding all other characters. However, do not encode the extended
    // character set. (Note: isalnum() does not always return true for extended
    // characters.)

	for ( i = 0; i < strlen( str ); i++ )
	{
		if ( isalnum( str[ i ] )  || (unsigned char)str[ i ] > 0x7F )
		{
			*buff = str[ i ];
			buff++;
		}
		else
		{
			sprintf( buff, "&#%u;", str[ i ] );
			buff += strlen( buff );
		}
	}
	
	// Null-terminate the output string.
	*buff = '\0';
}
