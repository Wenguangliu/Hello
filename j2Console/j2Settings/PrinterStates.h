/*
 * FILENAME
 * 	PrinterStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the printer settings states of the Settings
 *	state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __PRINTERSTATES_H_INCLUDED
#define __PRINTERSTATES_H_INCLUDED

#include "State.h"
#include "SettingsStates.h"

class StatePrinterSelect : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StatePrinterConfigure : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StatePrinterTest : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StatePrinterSetDefault : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StatePrinterSelectReports : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StatePrinterSetResultsCopies : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		setting;
};

class StatePrinterSetIqcCopies : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		setting;
};

class StatePrinterSetErrorCopies : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		setting;
	bool	autoPrint;
};

#endif	// __PRINTERSTATES_H_INCLUDED
