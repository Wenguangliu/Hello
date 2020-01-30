/*
 * FILENAME
 * 	UnitsStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the select units states of the Settings
 *	state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __UNITSSTATES_H_INCLUDED
#define __UNITSSTATES_H_INCLUDED

#include "State.h"
#include "SettingsStates.h"
#include "Units.h"
#include "Analytes.h"

class StateUnits : public StateSettings
{
protected:
	static Units			units;
	static AnalyteId_t		selectedAnalyte;
	static int				topItem;
	static Analytes			analytes;
	static AnalyteList_t	analyteList;
};

class StateUnitsSelect : public StateUnits
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateUnitsGroupSelect : public StateUnits
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateUnitsGlobal : public StateUnits
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateUnitsElectrolytes : public StateUnits
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateUnitsEnzymes : public StateUnits
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateUnitsLipids : public StateUnits
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateUnitsMinerals : public StateUnits
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateUnitsProteins : public StateUnits
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateUnitsList : public StateUnits
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	void	GetAnalyteList();
};

class StateUnitsSet : public StateUnits
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	UnitOptions_t *	unitOptions;
};


#endif	// __UNITSSTATES_H_INCLUDED
