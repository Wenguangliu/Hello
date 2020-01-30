/*
 * FILENAME
 * 	AnalyzerInfoStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the set analyzer info states of the Settings
 *	state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __ANALYZERINFOSTATES_H_INCLUDED
#define __ANALYZERINFOSTATES_H_INCLUDED

#include "State.h"
#include "SettingsStates.h"
#include "CounterData.h"
#include "VersionData.h"

typedef struct
{
	char label[30];
	char value[30];
} InfoItem_t;

#define NUM_INFO_ITEMS	20

class StateAnalyzerInfo : public StateSettings
{
protected:
	static InfoItem_t		info[ NUM_INFO_ITEMS ];
	static CounterData_t *	counterData;
	static VersionData_t *	versionData;
};

class StateAnalyzerInfoView : public StateAnalyzerInfo
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		topItem;
	int		numItems;
};

class StateAnalyzerInfoPrint : public StateAnalyzerInfo
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};


#endif	// __ANALYZERINFOSTATES_H_INCLUDED
