/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

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

float			EnterRange::lower;
float			EnterRange::upper;
float			EnterRange::lowerDefault;
float			EnterRange::upperDefault;
NumericFormat_t	EnterRange::format;
int				EnterRange::repeatCount = 0;


void
EnterRange::GetRange( float * _lower, float * _upper )
{
	*_lower = lower;
	*_upper = upper;
}

void
EnterRange::Init( GuiMiddleEnterRangeData_t * data )
{
	char buff[20];
	
	lower = data->lowerValue;
	upper = data->upperValue;
	lowerDefault = data->lowerDefault;
	upperDefault = data->upperDefault;
	format = data->format;

	PtSetResource( ABW_ptLabelEnterRangeLower, Pt_ARG_TEXT_STRING, data->lowerLabel, 0 );
	PtSetResource( ABW_ptLabelEnterRangeUpper, Pt_ARG_TEXT_STRING, data->upperLabel, 0 );

	FormatValue( buff, data->lowerValue );
	PtSetResource( ABW_ptTextEnterRangeLower, Pt_ARG_TEXT_STRING, buff, 0 );
	FormatValue( buff, data->upperValue );
	PtSetResource( ABW_ptTextEnterRangeUpper, Pt_ARG_TEXT_STRING, buff, 0 );
	PtSetResource( ABW_ptButtonEnterRange0, Pt_ARG_TEXT_STRING, data->buttonText[0], 0 );
	PtSetResource( ABW_ptButtonEnterRange1, Pt_ARG_TEXT_STRING, data->buttonText[1], 0 );
	
}

int
EnterRange::Up( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	AdjustValue( atoi( widget->user_data ), 1 );

	Click();
	
	repeatCount = 0;

	return Pt_CONTINUE;
}


int
EnterRange::RepeatUp( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	repeatCount++;

	if ( repeatCount < 10 )
	{
		AdjustValue( atoi( widget->user_data ), 1 );
	}
	else if ( repeatCount < 50 )
	{
		AdjustValue( atoi( widget->user_data ), 10 );
	}
	else
	{
		AdjustValue( atoi( widget->user_data ), 100 );
	}
		
	if ( repeatCount == 1 )
	{
		Click();
	}
	
	return Pt_CONTINUE;
}


int
EnterRange::Down( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

{
	AdjustValue( atoi( widget->user_data ), -1 );

	Click();
	
	repeatCount = 0;

	return Pt_CONTINUE;
}

int
EnterRange::RepeatDown( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	repeatCount++;

	if ( repeatCount < 10 )
	{
		AdjustValue( atoi( widget->user_data ), -1 );
	}
	else if ( repeatCount < 50 )
	{
		AdjustValue( atoi( widget->user_data ), -10 );
	}
	else
	{
		AdjustValue( atoi( widget->user_data ), -100 );
	}
	
	if ( repeatCount == 1 )
	{
		Click();
	}
	
	return Pt_CONTINUE;
}

int
EnterRange::Button( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char	buff[20];
	
	switch ( widget->user_data[0] )
	{
		case '0':		// Clear
			lower = 0.0;
			upper = 0.0;
			break;
		case '1':		// Default
			lower = lowerDefault;
			upper = upperDefault;
			break;
		default:
			break;
	}

	FormatValue( buff, lower );
	PtSetResource( ABW_ptTextEnterRangeLower, Pt_ARG_TEXT_STRING, buff, 0 );
	FormatValue( buff, upper );
	PtSetResource( ABW_ptTextEnterRangeUpper, Pt_ARG_TEXT_STRING, buff, 0 );
	
	Click();

	return Pt_CONTINUE;
}

void
EnterRange::FormatValue( char * buff, float value )
{
	switch ( format )
	{
		case Xxxx:
			sprintf( buff, ".%03.0f", value * 1000.0 );
			break;
		case Xxx:	
			sprintf( buff, "%1.2f", value );
			break;
		case Xx:
			sprintf( buff, "%1.1f", value );
			break;
		case X:
			sprintf( buff, "%1.0f", value );
			break;
		case X0:
			sprintf( buff, "%d0", (int)(value + 5.0) / 10 );
			break;
		case X00:
			sprintf( buff, "%d00", (int)(value + 50.0) / 100 );
			break;
	}
}

void
EnterRange::AdjustValue( int index, int adjustment )
{
	char	buff[20];
	
	switch ( index )
	{
		case 0:		// Lower
			lower += adjustment * IncFactor();
			if ( lower < 0.0 )
			{
				lower = 0.0;
			}
			if ( lower > upper )
			{
				upper = lower;
			}
			break;
		case 1:		// Upper
			upper += adjustment * IncFactor();
			
			if ( upper < 0.0 )
			{
				upper = 0.0;
			}
			if ( upper < lower )
			{
				lower = upper;
			}
			break;
		default:
			break;
	}

	FormatValue( buff, lower );
	PtSetResource( ABW_ptTextEnterRangeLower, Pt_ARG_TEXT_STRING, buff, 0 );
	FormatValue( buff, upper );
	PtSetResource( ABW_ptTextEnterRangeUpper, Pt_ARG_TEXT_STRING, buff, 0 );
}

float
EnterRange::IncFactor()
{
	float incFactor = 1.0;
	
	switch ( format )
	{
		case Xxxx:	incFactor = 0.001;	break;
		case Xxx:	incFactor = 0.01;	break;
		case Xx:	incFactor = 0.1;	break;
		case X:		incFactor = 1;		break;
		case X0:	incFactor = 10;		break;
		case X00:	incFactor = 100;	break;
	}
	
	return incFactor;
}

