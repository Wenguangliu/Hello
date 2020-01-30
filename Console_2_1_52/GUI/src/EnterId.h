/*
 * FILENAME
 * 	EnterId.h
 * 
 * MODULE DESCRIPTION
 * 	Class definition for the EnterId class, which manages the Enter ID subscreen.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __ENTERID_H_INCLUDED
#define __ENTERID_H_INCLUDED

class EnterId
{
public:
	static int		KeyPress( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int		InputVerify( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int		Activate( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static void		Init( char isPassword, char * initialValue );
	static char *	GetIdText();

private:
	static char isPassword;
};

#endif
