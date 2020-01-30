/*
 * FILENAME
 * 	EnterId.cc
 * 
 * MODULE DESCRIPTION
 * 	Class method definitions for the EnterId class, which manages the Enter ID
 *  subscreen.  This class is able to handle regular ID input and masked
 * 	password input.  In the latter case, the actual ID text is put into an
 * 	invisible off-screen text field and the visible text field shows asterisks,
 * 	one for each character in the off-screen field.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <photon/PkKeyDef.h>

/* Local headers */
#include "Global.h"
#include "Sound.h"
#include "TranslateUtf.h"

// Flag to indicate whether this is being used for regular input or masked,
// password input.
char EnterId::isPassword = 0;

/*
 * FUNCTION
 * 	Init
 * 
 * DESCRIPTION
 * 	Initializes ID entry handling.
 * 
 * PARAMETERS
 * 	isPassword - Flag to indicate whether input should be normal or as a masked
 * 			password (only asterisks displayed on the screen).
 * 	initialValue - Initial value to display in the text field.
 */

void
EnterId::Init( char isPassword, char * initialValue )
{
	EnterId::isPassword = isPassword;
	
	if ( isPassword )
	{
		char initialMask[] = "*****************************";
		initialMask[ strlen( initialValue ) ] = 0;
				
		PtSetResource( ABW_ptTextEnterId, Pt_ARG_TEXT_STRING, initialMask, 0 );
		PtSetResource( ABW_ptTextEnterIdHidden, Pt_ARG_TEXT_STRING, initialValue, 0 );
		PtSetResource( ABW_ptTextEnterIdHidden, Pt_ARG_CURSOR_POSITION, strlen( initialValue ), 0 );
	}
	else
	{
		PtSetResource( ABW_ptTextEnterId, Pt_ARG_TEXT_STRING, initialValue, 0 );
	}

	PtSetResource( ABW_ptTextEnterId, Pt_ARG_CURSOR_POSITION, strlen( initialValue ), 0 );
}

/*
 * FUNCTION
 * 	GetIdText
 * 
 * DESCRIPTION
 * 	Retrieves the entered ID text.
 * 
 *  NOTE: The retrieved text uses UTF-8 encoding.  It must be translated to
 * 		  ISO-8859-1 before being returned to an external process.
 *
 * RETURNS
 * 	A pointer to the entered text.
 * 
 */

char *
EnterId::GetIdText()
{
	char *		p;
	
	if ( isPassword )
	{
		PtGetResource( ABW_ptTextEnterIdHidden, Pt_ARG_TEXT_STRING, &p, 0 );
	}
	else
	{
		PtGetResource( ABW_ptTextEnterId, Pt_ARG_TEXT_STRING, &p, 0 );
	}
	
	return p;		
}

/*
 * FUNCTION
 * 	KeyPress
 * 
 * DESCRIPTION
 * 	Callback handler for the buttons on the Enter ID screen.  Emits a keyboard
 * 	entry event that is caught by the text object on the screen.
 * 
 * PARAMETERS
 * 	widget - Widget that triggered this callback.
 * 	apinfo, cbinfo - Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon callback return value.
 */

int
EnterId::KeyPress( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char * 			userData = widget->user_data;
	long			key = '.';
	struct
	{
		PhEvent_t		event;
		PhRect_t		rect;
		PhKeyEvent_t	keyEvent;
	} buttonPressEvent;
	
	if ( userData[0] == '<' )
	{
		key = Pk_BackSpace;
	}
	else
	{
		key = userData[0];
	}
		
	memset( &buttonPressEvent, 0, sizeof( buttonPressEvent ) );
		
	buttonPressEvent.event.type			= Ph_EV_KEY;
	buttonPressEvent.event.emitter.rid	= Ph_DEV_RID;
	buttonPressEvent.event.num_rects	= 1;
	buttonPressEvent.event.data_len		= sizeof( buttonPressEvent.keyEvent );
	buttonPressEvent.event.input_group	= 1;
		
	buttonPressEvent.keyEvent.key_cap	= key;
	buttonPressEvent.keyEvent.key_sym	= key;
		
	buttonPressEvent.keyEvent.key_flags = Pk_KF_Key_Down | Pk_KF_Sym_Valid | Pk_KF_Cap_Valid;
	PtSendEventToWidget( ABW_ptTextEnterId, (PhEvent_t *)&buttonPressEvent );
		
	buttonPressEvent.keyEvent.key_flags &= ~( Pk_KF_Key_Down | Pk_KF_Sym_Valid );
	PtSendEventToWidget( ABW_ptTextEnterId, (PhEvent_t *)&buttonPressEvent );
	
	Click();
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	InputVerify
 * 
 * DESCRIPTION
 * 	Callback handler for the text object on the Enter ID screen.  Verifies that
 *  the entered text is allowed.  Also, takes care of onscreen/offscreen text
 * 	objects for password entry.
 * 
 * PARAMETERS
 * 	cbinfo - Callback info, which contains information on the newly-input text.
 * 	widget, apinfo - Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon callback return value.
 */

int
EnterId::InputVerify( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	char	iso;

	// These are the characters that are allowed.  All others are ignored.
	const char allowedChars[] = " !\"#$%&'()*+,-./0123456789:;?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
								"abcdefghijklmnopqrstuvwxyz{|}~¿ÄÅÆÉÑÖÜßàáâäåæçèéêëìíîïñòóôöùúûüÿ";

	PtTextCallback_t * cb = (PtTextCallback_t *)cbinfo->cbdata;
		
	if ( cb->text != 0 && cb->length != 0 )
	{
		// If the first byte is > 0xDF, this means it's a three-byte or greater
		// UTF-8 value.  All our acceptable characters are single- or two-byte.
		// NOTE: We don't just simply translate the text whatever it is because
		// we don't know what the length is directly.  The value in cb->length
		// will be 1 for one character entered, even if it's a multi-byte
		// character.  We can't use strlen( cb->text ) because there's garbage
		// after the input character sequence and the translation chokes on it.
		if ( (unsigned char)(cb->text[0]) > 0xDF )
		{
			cb->doit = 0;
			return Pt_CONTINUE;
		}

		// If the first byte is < 0x80, then it's normal ASCII and doesn't need
		// to be translated.
		if ( (unsigned char)(cb->text[0]) < 0x80 )
		{
			iso = cb->text[0];
		}
		
		// Default is two-byte UTF-8, which needs to be translated.
		else
		{
			char source[3], dest[2];
			source[0] = cb->text[0];
			source[1] = cb->text[1];
			source[2] = '\0';
			TranslateFromUtf( source, dest );
			iso = dest[0];
		}

		// See if the translated character is in our list of allowed characters.
		if ( strchr( allowedChars, iso ) == 0 )
		{
			cb->doit = 0;
			return Pt_CONTINUE;
		}

		// If it's a password, update the display.
		if ( isPassword )
		{
			PtTextCallback_t * tcb = (PtTextCallback_t *)cbinfo->cbdata;
			static char stars[] = "*****************************";
			
			PtSetResource( ABW_ptTextEnterIdHidden, Pt_ARG_TEXT_SUBSTRING, tcb, 0 );
			
			tcb->text = stars;
		}
	}
	
	return Pt_CONTINUE;
}
