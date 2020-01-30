/*
 * FILENAME
 * 	Colors.h
 * 
 * MODULE DESCRIPTION
 * 	Color constants for GUI buttons, to allow programmatically choosing
 * 	highlighted or non-highlighted.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __COLORS_H_INCLUDED
#define __COLORS_H_INCLUDED

#include <Pt.h>

// GLD
// We ought to get these from Photon -- maybe a special window with the colors.
// Or else, from some specific buttons.
const PgColor_t	BLUE_NORMAL = 0x00004A;
const PgColor_t	BLUE_HILITE = 0x2A5394;
const PgColor_t	BLUE_OUTLINE = 0x000033;

const PgColor_t ORANGE_HILITE = 0xFFCE00;
const PgColor_t ORANGE_NORMAL = 0xFF9600;
const PgColor_t ORANGE_OUTLINE = 0xD9AF00;

#endif
