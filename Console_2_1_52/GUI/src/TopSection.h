/*
 * FILENAME
 * 	TopSection.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the top subscreens.  Includes the parent class for
 * 	all the top subscreens, TopSection, and all the actual top subscreen classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __TOPSECTION_H_INCLUDED
#define __TOPSECTION_H_INCLUDED

#include "ablibs.h"
#include "GuiMessages.h"
#include "GuiSection.h"

class TopSection : public GuiSection
{
public:
	virtual void Show() {};
	virtual void Hide() {};
	virtual void SetData( GuiReceiveMsgData_t * data ) {};

protected:
	PtWidget_t * window;
};

class TopTitle : public TopSection
{
public:
	TopTitle();
	void Show();
	void Hide();
	void SetData( GuiReceiveMsgData_t * data );
};

#endif
