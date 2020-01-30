/*
 * FILENAME
 * 	BottomSection.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the bottom subscreens.  Includes the parent class for
 * 	all the bottom subscreens, BottomSection, and all the actual bottom subscreen
 *  classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __BOTTOMSECTION_H_INCLUDED
#define __BOTTOMSECTION_H_INCLUDED

#include "ablibs.h"
#include "abimport.h"
#include "GuiMessages.h"
#include "GuiSection.h"


class BottomSection : public GuiSection
{
public:
	virtual void Show();
	virtual void Hide();
	virtual void SetData( GuiReceiveMsgData_t * data ) {};
	
protected:
	PtWidget_t *window;
};

class BottomButtons1 : public BottomSection
{
public:
	BottomButtons1();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * button;
};

class BottomButtons2 : public BottomSection
{
public:
	BottomButtons2();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * buttons[2];
};

class BottomButtons3 : public BottomSection
{
public:
	BottomButtons3();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * buttons[3];
};

class BottomDateTime : public BottomSection
{
public:
	BottomDateTime();
	void Show();
	void SetData( GuiReceiveMsgData_t * data );
	void Hide();
};

#endif
