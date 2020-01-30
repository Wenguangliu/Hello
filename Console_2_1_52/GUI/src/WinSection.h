/*
 * FILENAME
 * 	WinSection.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the full window subscreens.  Includes the parent class
 *  for all the full window subscreens, WinSection, and all the actual full
 * 	window subscreen classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __WINSECTION_H_INCLUDED
#define __WINSECTION_H_INCLUDED

#include "ablibs.h"
#include "abimport.h"
#include "GuiMessages.h"
#include "GuiSection.h"


class WinSection : public GuiSection
{
public:
	virtual void 	Show();
	virtual void 	Hide();
	virtual void 	SetData( GuiReceiveMsgData_t * data ) {};
	PtWidget_t *	Window() { return window; };

protected:
	PtWidget_t * window;
};

class WinWarning1 : public WinSection
{
public:
	WinWarning1();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * topLabel;
	PtWidget_t * message;
	PtWidget_t * bottomButton;
};

class WinWarning2 : public WinSection
{
public:
	WinWarning2();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * topLabel;
	PtWidget_t * message;
	PtWidget_t * bottomButton[2];
};

class WinError : public WinSection
{
public:
	WinError();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * topLabel;
	PtWidget_t * message;
	PtWidget_t * bottomButton;
};

class WinCriticalStop : public WinSection
{
public:
	WinCriticalStop();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * topLabel;
	PtWidget_t * message;
};

class WinCriticalStop2 : public WinSection
{
public:
	WinCriticalStop2();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * topLabel;
	PtWidget_t * message;
	PtWidget_t * bottomButton;
};

#endif
