/*
 * FILENAME
 * 	EnterRange.h
 * 
 * MODULE DESCRIPTION
 * 	Class definition for the EnterRange class, which manages the Enter Range subscreen.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __ENTERRANGE_H_INCLUDED
#define __ENTERRANGE_H_INCLUDED

class EnterRange
{
public:
	static void		Init( GuiMiddleEnterRangeData_t * data );
	static int		Button( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int		Up( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int		RepeatUp( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int		Down( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int		RepeatDown( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static void		GetRange( float * lower, float * upper );
//	static char *	GetIdText();

private:
	static void		AdjustValue( int index, int adjustment );
	static void		FormatValue( char * buff, float value );
	static float	IncFactor();
	
	static float			lower, upper;
	static float			lowerDefault, upperDefault;
	static NumericFormat_t	format;
	static int				repeatCount;
};

#endif	// __ENTERRANGE_H_INCLUDED
