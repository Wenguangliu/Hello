/*
 * FILENAME
 * 	RocInfo.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code for the class RocInfo, which is used to retrieve and store information
 *  from ROC files. Specifically, it retrieves the rotor product code, plus information for displaying
 *  rotor names and the colors to display those names in from the ROC file for the highest format
 *  version for each rotor number.
 * 
 * 	At startup, when an instance of the class is created, it scans the ROC file directory and gets
 *  the names of all the relevant ROC files. Because the time it takes to parse a ROC file is
 *  substantial, the ROC files aren't all parsed at startup. When the info for a particular rotor
 *  number is needed, if it's not already in memory, the relevant ROC file is parsed and the info
 *  from it is stored in memory for later use as well as the current rotor.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "CommonDefines.h"
#include "RocInfo.h"
#include "ParseRoc.h"
#include "CfgErr.h"
#include "LogErr.h"

char		RocInfo::fileNames[100][10];
RocInfo_t	RocInfo::rocInfo[100];

RocInfo::
RocInfo()
{
	int				rotorNum;
	int				format;
    wchar_t         wFilename[255];
    char            fileName [255];
    WIN32_FIND_DATA fileData;
    HANDLE          hSearch;
    BOOL            bFinished = FALSE;

	char			buff[10];
	char *			p;

    // convert to wChar string
    wsprintf(wFilename, L"%S/*.roc", ROC_FILE_DIRECTORY);
    // get first roc file
    hSearch = FindFirstFile(wFilename, &fileData);
    // search ROC directory 
    if (hSearch != INVALID_HANDLE_VALUE) {
        while (!bFinished) {
            // convert w32 to char file name
            sprintf (fileName, "%S", fileData.cFileName);
            memset (buff, 10, 0);
            strncpy (buff, fileName, 9) ;
            p = strtok( buff, "." );    // gets the rotor number
            if (p != 0) {
                rotorNum = atoi (p);
                if ((rotorNum > 1) && (rotorNum < 100)) {
                    p = strtok (0, ".");       // gets the format
                    if (p != 0) {
                        format = atoi (p);
                        if ((format >= 0) && (format <= 9)) {
                            p = strtok(0, "" );        // gets the extension
                            if ((p != 0) && (strcmp( p, "roc" ) == 0)) {
                                // OK, we've got a valid roc file
                                if ((fileNames [rotorNum ][0] != 0) || (strcmp(fileName, fileNames[ rotorNum ]) > 0)) {
                                    strcpy(fileNames[ rotorNum ], fileName);
                                    
                                }
                            }
                        }
                    }
                }
            }
            // find out next roc file
            if (!FindNextFile (hSearch, &fileData)) {
                if (GetLastError () == ERROR_NO_MORE_FILES) {
            //      LogDebug("No more Roc files");
                    FindClose(hSearch);
                    bFinished = TRUE;
                } 
            }
            
        } // end of while
    } else {
		LogDebug ( "Failed to open /Abaxis/Roc" );
	}
}

char
RocInfo::
RotorProductCode( unsigned char rotorNum )
{
	if ( !rocInfo[ rotorNum ].retrieved )
	{
		GetRocData( rotorNum );
	}
	
	return rocInfo[ rotorNum ].rotorProductCode;
}

const char *
RocInfo::
RotorNameLine1( unsigned char rotorNum )
{
	if ( !rocInfo[ rotorNum ].retrieved )
	{
		GetRocData( rotorNum );
	}
	
	return rocInfo[ rotorNum ].rotorNameLine1;
}

const char *
RocInfo::
RotorNameLine2( unsigned char rotorNum )
{
	if ( !rocInfo[ rotorNum ].retrieved )
	{
		GetRocData( rotorNum );
	}
	
	return rocInfo[ rotorNum ].rotorNameLine2;
}

unsigned
RocInfo::
RotorNameFontColor( unsigned char rotorNum )
{
	if ( !rocInfo[ rotorNum ].retrieved )
	{
		GetRocData( rotorNum );
	}
	
	return rocInfo[ rotorNum ].rotorNameFontColor;
}

unsigned
RocInfo::
RotorNameBackgroundColor( unsigned char rotorNum )
{
	if ( !rocInfo[ rotorNum ].retrieved )
	{
		GetRocData( rotorNum );
	}

	return rocInfo[ rotorNum ].rotorNameBackgroundColor;
}

int
RocInfo::
GetRocData( unsigned char rotorNum )
{
	static RotorConfig_t	rocData;
	char			        fileName[100];
	int				        retVal = 1;
	
	// Signal that we've retrieved (or at least made an attempt) the data for this rotor.
	rocInfo[ rotorNum ].retrieved = true;
	
		// If we don't have a ROC file for this rotor, signal an error.
	if ( fileNames[ rotorNum ][0] == 0 )
	{
		retVal = 0;
	}
	else
	{
		sprintf( fileName, "%s/%s", ROC_FILE_DIRECTORY, fileNames[ rotorNum ] );
	
		if ( ParseRoc( fileName, &rocData ) == CFG_ERR_NONE)
		{
			rocInfo[ rotorNum ].rotorProductCode = rocData.rotorProductCode;
			strcpy( rocInfo[ rotorNum ].rotorNameLine1, rocData.rotorScreenNameLine1 );
			strcpy( rocInfo[ rotorNum ].rotorNameLine2, rocData.rotorScreenNameLine2 );
			rocInfo[ rotorNum ].rotorNameFontColor = rocData.rotorScreenNameFontColor;
			rocInfo[ rotorNum ].rotorNameBackgroundColor = rocData.rotorScreenNameBackgroundColor;
			retVal = 1;
		}
		else
		{
			retVal = 0;
		}
	}

	// Error parsing the ROC file. Display a question mark for the rotor name in black font with
	// white background. This also sets the first character of rotorNameLine1, so subsequent calls
	// to get the info won't repeatedly try to parse the ROC file.
	if ( retVal == 0 )
	{
		rocInfo[ rotorNum ].rotorProductCode = 0;
		strcpy( rocInfo[ rotorNum ].rotorNameLine1, "?" );
		rocInfo[ rotorNum ].rotorNameFontColor = 0;
		rocInfo[ rotorNum ].rotorNameBackgroundColor = 0xFFFFFF;
		LogDebug( "ROC file error for rotor %d", rotorNum );
	}
	
	return retVal;
}
