/*
 * FILENAME
 * 	ArchiveStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the archive reference ranges states of the Settings
 *	state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __ARCHIVESTATES_H_INCLUDED
#define __ARCHIVESTATES_H_INCLUDED

#include "State.h"
#include "SettingsStates.h"
#include "Species.h"
#include "CfgErr.h"

#define MAX_ARCHIVED_REF_RANGES			10
#define REF_RANGE_ARCHIVE_INDEX_VERSION	1
#define REF_RANGE_ARCHIVE_INDEX			REF_RANGES_DIRECTORY"RefRanges.ndx"

typedef struct
{
	int		version;
	int		lastEntry;
	char	fileNames[ MAX_ARCHIVED_REF_RANGES ][ 100 ];	// Long enough to hold full path.
} RefRangeArchiveIndex_t;

typedef struct
{
	int		len;
	char	names[ MAX_ARCHIVED_REF_RANGES ][ 100 ];
	char	fileNames[ MAX_ARCHIVED_REF_RANGES ][ 100 ];
} RefRangeArchiveList_t;

class StateArchive : public StateSettings
{
protected:
	CfgError_t						ArchiveCurrentRefRanges();
	void							GetArchiveLabel( char * buff, char * fileName );
	void							GetArchiveList();
	static RefRangeArchiveIndex_t *	ReadArchiveIndex();
	CfgError_t						RetrieveArchive( int index );
	
	
	static RefRangeArchiveIndex_t *	archiveIndex;
	static RefRangeArchiveList_t	archiveList;
	static int						selectedArchive;
	static char						selectedArchiveLabel[30];
	static int						topItemSelectReport;
	static SampleType_t				selectedType;
	static bool						doPrint;
};

class StateArchiveSelect : public StateArchive
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateArchiveSend : public StateArchive
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateArchiveRetrieve : public StateArchive
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int						topItemArchiveRetrieve;
};

class StateArchiveConfirmRetrieve : public StateArchive
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateArchiveSelectReportExpress : public StateArchive
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateArchiveSelectReportAdvanced : public StateArchive
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	ActiveRangesList_t *	activeRanges;
	Species					species;
};

class StateArchiveSending : public StateArchive
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};
	
#endif	// __ARCHIVESTATES_H_INCLUDED
