#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>

#include "Util.h"
#include "Global.h"

/*
 * FUNCTION
 *  AsciiToHex
 *
 * DESCRIPTION
 *  Converts an ASCII hexadecimal string number to a binary value.
 *
 * PARAMETERS
 *  ascii - ASCII hexadecimal string.
 *
 * RETURNS
 *  Binary value.
 */

unsigned char
AsciiToHex (char ascii)
{
    unsigned char hex = 0xFF;

    if (isxdigit (ascii)) {
        hex = ascii & 0x0f;
        if (ascii > '9') {
            hex += 9;
        }
    }

    return hex;
}
/*
 * FUNCTION
 *  AsciiToByte
 *
 * DESCRIPTION
 *  Converts ASCII string to 8 bits hexadecimal/decimal value .
 *
 * PARAMETERS
 *  *amcii  --- ascii string pointer
 *  type    --- convert to hex or decimal
 *
 * RETURNS
 *   byte vaule
 */

unsigned char
AsciiToDec (char ascii)
{
    return (ascii - '0');
}

unsigned char
AsciiToByte (char *ascii, ConvertType_t type)
{
    unsigned byte;

    if (type == HEX_TYPE) {
        byte    =  (AsciiToHex (ascii[0]) << 4) & 0xf0;
        byte    += (AsciiToHex (ascii[1]) << 0) & 0x0f;
    } else {

        byte = (AsciiToDec (ascii[0])) * 10 + (AsciiToDec (ascii[1]));

    }

    return byte;
}

/*
 * FUNCTION
 *  AsciiToWord
 *
 * DESCRIPTION
 *  Converts ASCII string to 16 bits hexadecimal value.
 *
 * PARAMETERS
 *  *amcii  -- ascii string pointer
 *
 * RETURNS
 *  unsigned int value
 */

unsigned short
AsciiToWord (char *ascii, char swapByte)
{
    unsigned short word;

    if (swapByte) {
        word  =  (AsciiToHex (ascii[0]) << 4) ;
        word  += (AsciiToHex (ascii[1]) << 0) ;
        word  += (AsciiToHex (ascii[2]) << 12);
        word  += (AsciiToHex (ascii[3]) << 8) ;
    } else {
        word  =  (AsciiToHex (ascii[0]) << 12);
        word  += (AsciiToHex (ascii[1]) <<  8);
        word  += (AsciiToHex (ascii[2]) <<  4);
        word  += (AsciiToHex (ascii[3]) <<  0);
    }
    return  word;
}

/*
 * FUNCTION
 *  AsciiToLong
 *
 * DESCRIPTION
 *  Converts ASCII string to 32 bits hexadecimal value.
 *
 * PARAMETERS
 *  *amcii  -- ascii string pointer
 *
 * RETURNS
 *  unsigned int value
 */

unsigned int
AsciiToLong (char *ascii,  char swapByte)
{
    unsigned int  genInt;

    if (swapByte) {
        genInt  =  (AsciiToHex (ascii[0]) <<  4);
        genInt  += (AsciiToHex (ascii[1]) <<  0);
        genInt  += (AsciiToHex (ascii[2]) << 12);
        genInt  += (AsciiToHex (ascii[3]) <<  8);
        genInt  += (AsciiToHex (ascii[4]) << 20);
        genInt  += (AsciiToHex (ascii[5]) << 16);
        genInt  += (AsciiToHex (ascii[6]) << 28);
        genInt  += (AsciiToHex (ascii[7]) << 24);
    } else {
        genInt  =  (AsciiToHex (ascii[0]) << 28);
        genInt  += (AsciiToHex (ascii[1]) << 24);
        genInt  += (AsciiToHex (ascii[2]) << 20);
        genInt  += (AsciiToHex (ascii[3]) << 16);
        genInt  += (AsciiToHex (ascii[4]) << 12);
        genInt  += (AsciiToHex (ascii[5]) <<  8);
        genInt  += (AsciiToHex (ascii[6]) <<  4);
        genInt  += (AsciiToHex (ascii[7]) <<  0);
    }

    return  genInt;
}
/*
 * FUNCTION
 *  AsciiToFloat
 *
 * DESCRIPTION
 *  Converts ASCII string to 32 bits float hexadecimal value.
 *
 * PARAMETERS
 *  *ascii  -- ascii string pointer
 *
 * RETURNS
 *  float value
 */

float
AsciiToFloat (char *asciiPtr, char swapByte)
{

    float genFloat;
    char  *floatPtr;

    floatPtr = (char *)&genFloat;


    if (swapByte) {

        *floatPtr   = (AsciiToHex (*asciiPtr++) << 4);
        *floatPtr  +=  AsciiToHex (*asciiPtr++);
        floatPtr++;

        *floatPtr   = (AsciiToHex (*asciiPtr++) << 4);
        *floatPtr += AsciiToHex (*asciiPtr++);
        floatPtr++;

        *floatPtr   = (AsciiToHex (*asciiPtr++) << 4);
        *floatPtr += AsciiToHex (*asciiPtr++);
        floatPtr++;

        *floatPtr   = (AsciiToHex (*asciiPtr++) << 4);
        *floatPtr  +=  AsciiToHex (*asciiPtr);

    } else {

        floatPtr += 3;

        *floatPtr   = (AsciiToHex (*asciiPtr++) << 4);
        *floatPtr  +=  AsciiToHex (*asciiPtr++);
        floatPtr--;

        *floatPtr   = (AsciiToHex (*asciiPtr++) << 4);
        *floatPtr  +=  AsciiToHex (*asciiPtr++);
        floatPtr--;

        *floatPtr   = (AsciiToHex (*asciiPtr++) << 4);
        *floatPtr  +=  AsciiToHex (*asciiPtr++);
        floatPtr--;

        *floatPtr   = (AsciiToHex (*asciiPtr++) << 4);
        *floatPtr  +=  AsciiToHex (*asciiPtr);

    }
    return genFloat;
}

/*
 * FUNCTION
 *  HexToAscii
 *
 * DESCRIPTION
 *  Converts a binary value to an ASCII hexadecimal string.
 *
 * PARAMETERS
 *  hex - Binary value to convert.
 *
 * RETURNS
 *  ASCII hex string.
 */

char
HexToAscii(unsigned char hex)
{
    return  "0123456789ABCDEF"[hex & 0x0f];
}



/*
 * FUNCTION
 *  ByteToAscii
 *
 * DESCRIPTION
 *  Converts a 8 bit hex to an ASCII hexadecimal string.
 *
 * PARAMETERS
 *  byte --- 8 bit hexadecimal value
 *  asciiPtr --- ascii buf pointer
 *
 * RETURNS
 *   void
 */

char *
ByteToAscii (char byte, char *asciiPtr)
{
    *asciiPtr++ = HexToAscii ((char )(byte >>4));
    *asciiPtr++  = HexToAscii ((char )(byte));

    return asciiPtr;

}
/*
 * FUNCTION
 *  WordToAscii
 *
 * DESCRIPTION
 *  Converts a 16 bit hex to an ASCII hexadecimal string.
 *
 * PARAMETERS
 *  word --- 16 bit hexadecimal value
 *  asciiPtr --- ascii buf pointer
 *
 * RETURNS
 *   void
 */

char *
WordToAscii (unsigned short word, char *asciiPtr, char swapByte)
{
    if (swapByte) {
        *asciiPtr++ = HexToAscii ((char )(word >> 4));
        *asciiPtr++ = HexToAscii ((char )(word >> 0));
        *asciiPtr++ = HexToAscii ((char )(word >> 12));
        *asciiPtr++ = HexToAscii ((char )(word >> 8));
    } else {
        *asciiPtr++ = HexToAscii ((char )(word >>12));
        *asciiPtr++ = HexToAscii ((char )(word >>8));
        *asciiPtr++ = HexToAscii ((char )(word >>4));
        *asciiPtr++ = HexToAscii ((char )word);
    }
    return (asciiPtr);
}

/*
 * FUNCTION
 *  LongToAscii
 *
 * DESCRIPTION
 *  Converts a 32 bit hex to an ASCII  string.
 *
 * PARAMETERS
 *  genInt --- 32 bit hexadecimal value
 *  asciiPtr --- ascii buf pointer
 *
 * RETURNS
 *   void
 */

char *
LongToAscii (unsigned int genInt, char *asciiPtr, char byteSwap)
{
    if (byteSwap) {
        *asciiPtr++ = HexToAscii ((char )(genInt >>4));
        *asciiPtr++ = HexToAscii ((char )(genInt >>0));
        *asciiPtr++ = HexToAscii ((char )(genInt >>12));
        *asciiPtr++ = HexToAscii ((char )(genInt >>8));
        *asciiPtr++ = HexToAscii ((char )(genInt >>20));
        *asciiPtr++ = HexToAscii ((char )(genInt >>16));
        *asciiPtr++ = HexToAscii ((char )(genInt >>28));
        *asciiPtr++ = HexToAscii ((char )(genInt >>24));
    } else {
        *asciiPtr++ = HexToAscii ((char )(genInt >>28));
        *asciiPtr++ = HexToAscii ((char )(genInt >>24));
        *asciiPtr++ = HexToAscii ((char )(genInt >>20));
        *asciiPtr++ = HexToAscii ((char )(genInt >>16));
        *asciiPtr++ = HexToAscii ((char )(genInt >>12));
        *asciiPtr++ = HexToAscii ((char )(genInt >>8));
        *asciiPtr++ = HexToAscii ((char )(genInt >>4));
        *asciiPtr++ = HexToAscii ((char )genInt);
    }
    return asciiPtr;
}

/*
 * FUNCTION
 *  FloatToAscii
 *
 * DESCRIPTION
 *  Converts a 32 bit float  to an ASCII string.
 *
 * PARAMETERS
 *  genFloat --- 32 bit hexadecimal value
 *  asciiPtr --- ascii buf pointer
 *
 * RETURNS
 *   void
 */

char *
FloatToAscii (float genFloat, char *asciiPtr, char byteSwap)
{

    char *floatPtr;

    floatPtr = (char *)&genFloat;

    if (byteSwap) {
        *asciiPtr++ = HexToAscii (*floatPtr >>4);
        *asciiPtr++ = HexToAscii (*floatPtr++);
        *asciiPtr++ = HexToAscii (*floatPtr >>4);
        *asciiPtr++ = HexToAscii (*floatPtr++);
        *asciiPtr++ = HexToAscii (*floatPtr >>4);
        *asciiPtr++ = HexToAscii (*floatPtr++);
        *asciiPtr++ = HexToAscii (*floatPtr >>4);
        *asciiPtr++ = HexToAscii (*floatPtr);
    } else {
        floatPtr += 3;  /* move to MSB */
        *asciiPtr++ = HexToAscii (*floatPtr >> 4);
        *asciiPtr++ = HexToAscii (*floatPtr--);
        *asciiPtr++ = HexToAscii (*floatPtr >> 4);
        *asciiPtr++ = HexToAscii (*floatPtr--);
        *asciiPtr++ = HexToAscii (*floatPtr >> 4);
        *asciiPtr++ = HexToAscii (*floatPtr--);
        *asciiPtr++ = HexToAscii (*floatPtr >> 4);
        *asciiPtr++ = HexToAscii (*floatPtr);
    }

    return asciiPtr;
}


