/*
 * FILENAME
 * 	RefRangesStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the set reference ranges states of the Settings
 *	state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __REFRANGESSTATES_H_INCLUDED
#define __REFRANGESSTATES_H_INCLUDED

#include "State.h"
#include "SettingsStates.h"
#include "Species.h"
#include "Analytes.h"
#include "Units.h"

class StateRefRanges : public StateSettings
{
protected:
	void	FormatRefRange( char * buff, RefRange_t range, UnitData_t unitData );
	void	FormatRefRangeValue( char * buff, float value, UnitData_t unitData );

	static Gender_t			gender;
	static SpeciesGroup_t	selectedGroup;
	static SampleType_t		selectedSampleType;
	static AnalyteId_t		selectedAnalyte;
	static Species			species;
	static Analytes			analytes;
	static Units			units;
	static int				topItemListAnalytes;
	static int				topItemListSpecies;
	static int				topItemRemoveSpecies;
	static int				topItemListAnalyteRanges;
	static int				topItemPickName;
	static int				topItemListSpeciesRanges;
	static int				topItemSelectSpeciesLabel;
};

class StateRefRangesSelect : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateRefRangesOrganize : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateRefRangesListAnalytes : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	const AnalyteList_t *	analyteList;
};

class StateRefRangesListSpecies : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	ActiveRangesList_t *	activeRanges;
};

class StateRefRangesAddSpecies : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateRefRangesPickName : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	AvailableGroupsList_t *	groupList;
};

class StateRefRangesSetAgeRange : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateRefRangesListAnalyteRanges : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	const AnalyteList_t *	analyteList;
};

class StateRefRangesListSpeciesRanges : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	ActiveRangesList_t *	activeRanges;
};

class StateRefRangesModifyRange : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
	
private:
	RefRange_t	range;
};

class StateRefRangesSelectSpeciesLabel : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	void	GetSpeciesList();
	
	SampleType_t	list[ STYPE_LAST_NAMED_SPECIES - STYPE_FIRST_NAMED_SPECIES + 1 ];
	int				listLen;
};

class StateRefRangesRemoveSpecies : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	ActiveRangesList_t *	activeRanges;
};

class StateRefRangesConfirmRemoval : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateRefRangesDefaultSpecies : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateRefRangesSelectAnalyteGender : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateRefRangesSelectDemographicGender : public StateRefRanges
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

#endif	// __REFRANGESSTATES_H_INCLUDED
