/*
 * FILENAME
 * 	FactoryDefaultsStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine set factory
 * 	defaults states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "RefRangesStates.h"
#include "SettingsMessages.h"
#include "LogErr.h"
#include "ParseSettings.h"
#include "ParseRefRanges.h"

SpeciesGroup_t	StateRefRanges::selectedGroup = GTYPE_CANINES;
SampleType_t	StateRefRanges::selectedSampleType = STYPE_OTHER;
AnalyteId_t		StateRefRanges::selectedAnalyte = AID_ALB;
Species			StateRefRanges::species;
Analytes		StateRefRanges::analytes;
Units			StateRefRanges::units;

Gender_t		StateRefRanges::gender = GENDER_UNKNOWN;
int				StateRefRanges::topItemListAnalytes = 0;
int				StateRefRanges::topItemListSpecies = 0;
int				StateRefRanges::topItemRemoveSpecies = 0;
int				StateRefRanges::topItemListAnalyteRanges = 0;
int				StateRefRanges::topItemPickName = 0;
int				StateRefRanges::topItemListSpeciesRanges = 0;
int				StateRefRanges::topItemSelectSpeciesLabel = 0;

/*
 * class StateRefRanges
 */

void
StateRefRanges::
FormatRefRange( char * buff, RefRange_t range, UnitData_t unitData )
{
	char	lowBuff[10];
	char	highBuff[10];
	
	FormatRefRangeValue( lowBuff, range.low, unitData );
	FormatRefRangeValue( highBuff, range.high, unitData );
	sprintf( buff, "%s-%s", lowBuff, highBuff );
}

void
StateRefRanges::
FormatRefRangeValue( char * buff, float value, UnitData_t unitData )
{
	value *= unitData.mult;
	
	switch ( unitData.format )
	{
		case Xxxx:
			sprintf( buff, ".%03.0f", value * 1000.0 );
			break;
		case Xxx:	
			sprintf( buff, "%1.2f", value );
			break;
		case Xx:
			sprintf( buff, "%1.1f", value );
			break;
		case X:
			sprintf( buff, "%1.0f", value );
			break;
		case X0:
			sprintf( buff, "%d0", (int)(value + 5.0) / 10 );
			break;
		case X00:
			sprintf( buff, "%d00", (int)(value + 50.0) / 100 );
			break;
	}
}

/*
 * class StateRefRangesSelect
 */
 
void
StateRefRangesSelect::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_INCLUDE_REF_RANGES ] );

	// Start with the last modified as the default analyte and sample type.  We
	// shouldn't ever use these until they've been set somewhere else.
	selectedAnalyte = refRanges->lastModifiedAnalyte;
	selectedSampleType = refRanges->lastModifiedSampleType;

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;
	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );

	wsprintf( data->buttonText[0], L"%s: %s", strings[ MSTR_LAST_MODIFIED ], analytes.GetAnalysisString(selectedAnalyte, &strings));
	wcscpy( data->buttonText[1], strings[ MSTR_ALL ] );
	data->buttonHighlighted[0] = true;
	data->buttonHighlighted[1] = false;
	data->buttonHighlighted[2] = false;
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateRefRangesSelect::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_REFRANGES_SELECT;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Last Modified
					selectedAnalyte = refRanges->lastModifiedAnalyte;
					selectedSampleType = refRanges->lastModifiedSampleType;
					retVal = STATE_REFRANGES_MODIFY_RANGE;
					stateStack.Push( STATE_REFRANGES_SELECT );
					break;
				case 1:		// All
					retVal = STATE_REFRANGES_ORGANIZE;
					stateStack.Push( STATE_REFRANGES_SELECT );
					break;
			}
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_REF_RANGES_INCLUDE ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_REFRANGES_SELECT );
			break;
		default:
			break;
	}
	
	return retVal;
}

/*
 * StateRefRangesOrganize
 */

void
StateRefRangesOrganize::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_MODIFY_REF_RANGES ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;
	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_ANALYTE ] );
	wcscpy( data->buttonText[1], strings[ MSTR_SPECIES ] );
	data->buttonHighlighted[0] = true;
	data->buttonHighlighted[1] = false;
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateRefRangesOrganize::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_REFRANGES_ORGANIZE;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Analyte
					topItemListAnalytes = 0;
					retVal = STATE_REFRANGES_LIST_ANALYTES;
					stateStack.Push( STATE_REFRANGES_ORGANIZE );
					break;
				case 1:		// Species
					topItemListSpecies = 0;
					retVal = STATE_REFRANGES_LIST_SPECIES;
					stateStack.Push( STATE_REFRANGES_ORGANIZE );
					break;
			}
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_REF_RANGES_MODIFY ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_REFRANGES_ORGANIZE );
			break;
		default:
			break;
	}

	return retVal;
}

/*
 * StateRefRangesListAnalytes
 */

void
StateRefRangesListAnalytes::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i;

	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		analyteList = analytes.PiccoloAnalytesWithRefRanges();
	}
	else
	{
		analyteList = analytes.VetScanAnalytesWithRefRanges();
	}
	
	SetTopLabel( strings[ TSTR_ANALYTES ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST;

		for ( i = 0; i < 5 && i < analyteList->len - topItemListAnalytes; i++ )
		{
			wcscpy( data->items[ i ][0], analytes.GetAnalysisString (analyteList->list[ topItemListAnalytes + i ], &strings ));
		}
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateRefRangesListAnalytes::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_REFRANGES_LIST_ANALYTES;
	int		idata = *(int *)data;
	bool	doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			selectedAnalyte = analyteList->list[ topItemListAnalytes + idata ];
			topItemListSpeciesRanges = 0;
			if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
			{
				retVal = STATE_REFRANGES_SELECT_ANALYTE_GENDER;
			}
			else
			{
				retVal = STATE_REFRANGES_LIST_SPECIES_RANGES;
				gender = GENDER_UNKNOWN;
			}
			stateStack.Push( STATE_REFRANGES_LIST_ANALYTES );
			break;

		case MSG_GUI_OUT_UP:
			if ( topItemListAnalytes > 0 )
			{
				topItemListAnalytes -= 4;
				if ( topItemListAnalytes < 0 )
				{
					topItemListAnalytes = 0;
				}
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_DOWN:
			if ( analyteList->len > topItemListAnalytes + 5 )
			{
				topItemListAnalytes += 4;
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
			}
			break;

		default:
			break;
	}

	if ( doUpdate )
	{
		GuiReceiveMsg_t			msg;
		GuiMiddleListData_t *	data = &msg.data.middleList;
		int						i;

		memset( &msg, 0, sizeof( msg ) );
		msg.msgId = MSG_GUI_UPDATE_LIST;

		for ( i = 0; i < 5 && i < analyteList->len - topItemListAnalytes; i++ )
		{
			wcscpy( data->items[ i ][0], analytes.GetAnalysisString ( analyteList->list[ topItemListAnalytes + i ], &strings ));
		}
		data->isSelectable = 1;
		data->hasUpDown = 1;
	
		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}

	return retVal;
}

/*
 * StateRefRangesListSpecies
 */

void
StateRefRangesListSpecies::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i;

	activeRanges = GetActiveRefRanges( refRanges, &strings );

	SetTopLabel( strings[ TSTR_SPECIES ] );

	memset( &msg, 0, sizeof( GuiReceiveMsg_t ) );
	
	msg.msgId = MSG_GUI_MIDDLE_LIST;

	i = 0;
	if ( topItemListSpecies == 0 )
	{
		wcscpy( data->items[0][0], strings[ MSTR_ADD_SPECIES ] );
		wcscpy( data->items[1][0], strings[ MSTR_REMOVE_SPECIES ] );
		wcscpy( data->items[2][0], strings[ MSTR_FACTORY_DEFAULT_SPECIES ] );
		i = 3;
	}
	else if ( topItemListSpecies == 1 )
	{
		wcscpy( data->items[0][0], strings[ MSTR_REMOVE_SPECIES ] );
		wcscpy( data->items[1][0], strings[ MSTR_FACTORY_DEFAULT_SPECIES ] );
		i = 2;
	}
	else if ( topItemListSpecies == 2 )
	{
		wcscpy( data->items[0][0], strings[ MSTR_FACTORY_DEFAULT_SPECIES ] );
		i = 1;
	}
			
	for ( ; i < 5 && i < activeRanges->len - topItemListSpecies + 3; i++ )
	{
		wcscpy( data->items[ i ][0], species.GetSampleTypeString (activeRanges->list[ topItemListSpecies + i - 3 ], &strings ));
	}
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateRefRangesListSpecies::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_REFRANGES_LIST_SPECIES;
	int		idata = *(int *)data;
	bool	doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			if ( topItemListSpecies + idata == 0 )		// Add Species
			{
				retVal = STATE_REFRANGES_ADD_SPECIES;
			}
			else if ( topItemListSpecies + idata == 1 )	// Remove Species
			{
				retVal = STATE_REFRANGES_REMOVE_SPECIES;
				topItemRemoveSpecies = 0;
			}
			else if (  topItemListSpecies + idata == 2)	// Factory Default Species
			{
				retVal = STATE_REFRANGES_DEFAULT_SPECIES;
			}
			else										// Species selected
			{
				selectedSampleType = activeRanges->list[ topItemListSpecies + idata - 3 ];
				topItemListAnalyteRanges = 0;

				// Select gender if Piccolo and not a control
				if ( (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) &&
					 ((selectedSampleType < STYPE_CONTROL) || (selectedSampleType > STYPE_LAST_CONTROL)) )
				{
					retVal = STATE_REFRANGES_SELECT_DEMOGRAPHIC_GENDER;
				}
				else
				{
					retVal = STATE_REFRANGES_LIST_ANALYTE_RANGES;
					gender = GENDER_UNKNOWN;
				}
			}
			stateStack.Push( STATE_REFRANGES_LIST_SPECIES );
			break;

		case MSG_GUI_OUT_UP:
			if ( topItemListSpecies > 0 )
			{
				topItemListSpecies -= 4;
				if ( topItemListSpecies < 0 )
				{
					topItemListSpecies = 0;
				}
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_DOWN:
			if ( activeRanges->len > topItemListSpecies + 5 - 3 )	// -3 correction for Add, Delete and Factory Defaults
			{
				topItemListSpecies += 4;
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
			}
			break;

		default:
			break;
	}

	if ( doUpdate )
	{
		GuiReceiveMsg_t			msg;
		GuiMiddleListData_t *	data = &msg.data.middleList;
		int						i;

		memset( &msg, 0, sizeof( msg ) );
		msg.msgId = MSG_GUI_UPDATE_LIST;

		i = 0;
		if ( topItemListSpecies == 0 )
		{
			wcscpy( data->items[0][0], strings[ MSTR_ADD_SPECIES ] );
			wcscpy( data->items[1][0], strings[ MSTR_REMOVE_SPECIES ] );
			wcscpy( data->items[2][0], strings[ MSTR_FACTORY_DEFAULT_SPECIES ] );
			i = 3;
		}
		else if ( topItemListSpecies == 1 )
		{
			wcscpy( data->items[0][0], strings[ MSTR_REMOVE_SPECIES ] );
			wcscpy( data->items[1][0], strings[ MSTR_FACTORY_DEFAULT_SPECIES ] );
			i = 2;
		}
		else if ( topItemListSpecies == 2 )
		{
			wcscpy( data->items[0][0], strings[ MSTR_FACTORY_DEFAULT_SPECIES ] );
			i = 1;
		}
			
		for ( ; i < 5 && i < activeRanges->len - topItemListSpecies + 3; i++ )
		{
			wcscpy( data->items[ i ][0], species.GetSampleTypeString (activeRanges->list[ topItemListSpecies + i - 3 ], &strings ));
		}
		data->isSelectable = 1;
		data->hasUpDown = 1;
	
		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}

	return retVal;
}

/*
 * StateRefRangesAddSpecies
 */

void
StateRefRangesAddSpecies::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;
	SampleType_t					nextSpecies = GetNextSpeciesNRefRange( refRanges );
	SampleType_t					nextControl = GetNextControlNRefRange( refRanges );
	int								i;
	
	SetTopLabel( strings[ TSTR_ADD_SPECIES ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;
	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );

	i = 0;
	if ( nextSpecies != STYPE_NONE )
	{
		wcscpy( data->buttonText[ i ], species.GetSampleTypeString (nextSpecies , &strings) );
		i++;
	}
	if ( nextControl != STYPE_NONE )
	{
		wcscpy( data->buttonText[ i ], species.GetSampleTypeString (nextControl , &strings) );
		i++;
	}

	if ( factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE )
	{
		wcscpy( data->buttonText[ i ], strings[ MSTR_SELECT ] );
	}
	
	data->buttonHighlighted[0] = true;
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateRefRangesAddSpecies::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_REFRANGES_ADD_SPECIES;
	int			idata = *(int *)data;
	CfgError_t	cfgErr = CFG_ERR_NONE;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Species n
					selectedSampleType = GetNextSpeciesNRefRange( refRanges );
					refRanges->isDefined[ selectedSampleType ] = true;
					cfgErr = WriteRefRanges( REF_RANGES_CURRENT_FILE, refRanges );
					if ( cfgErr == CFG_ERR_NONE )
					{
						settingsData->sampleTypeAdvanced = true;
						cfgErr = WriteSettings( settingsData );
					}
					if ( cfgErr != CFG_ERR_NONE )
					{
						errNum = SERR_FILE_WRITE;
						retVal = STATE_ERROR;
					}
					else
					{
						if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
						{
							retVal = STATE_REFRANGES_SELECT_DEMOGRAPHIC_GENDER;
						}
						else
						{
							retVal = STATE_REFRANGES_LIST_ANALYTE_RANGES;
						}
						stateStack.Push( STATE_REFRANGES_ADD_SPECIES );
					}
					break;
				case 1:		// Control n
					selectedSampleType = GetNextControlNRefRange( refRanges );
					refRanges->isDefined[ selectedSampleType ] = true;
					if ( cfgErr == CFG_ERR_NONE )
					{
						settingsData->sampleTypeAdvanced = true;
						cfgErr = WriteSettings( settingsData );
					}
					if ( cfgErr != CFG_ERR_NONE )
					{
						errNum = SERR_FILE_WRITE;
						retVal = STATE_ERROR;
					}
					else
					{
						retVal = STATE_REFRANGES_LIST_ANALYTE_RANGES;
						stateStack.Push( STATE_REFRANGES_ADD_SPECIES );
					}
					break;
				case 2:		// Select
					topItemPickName = 0;
					retVal = STATE_REFRANGES_PICK_NAME;
					stateStack.Push( STATE_REFRANGES_ADD_SPECIES );
					break;
			}
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_REF_RANGES_ADD ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_REFRANGES_ADD_SPECIES );
			break;
		default:
			break;
	}
	

	return retVal;
}

/*
 * StateRefRangesPickName
 */

void
StateRefRangesPickName::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i;

	groupList = GetGroupsWithAvailableRefRanges( refRanges );
	
	SetTopLabel( strings[ TSTR_SPECIES_AVAILABLE ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST;
	memset( data, 0, sizeof( GuiMiddleListData_t ) );
	
	for ( i = 0; i < 5 && i < groupList->len - topItemPickName; i++ )
	{
		wcscpy( data->items[ i ][0], species.GetGroupString (groupList->list[ i + topItemPickName ], &strings ) );
	}
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false );
}

state_t
StateRefRangesPickName::
ProcessMessage( int msgId, char * data )
{
	state_t 				retVal = STATE_REFRANGES_PICK_NAME;
	int						idata = *(int *)data;
	bool					doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:	
			selectedGroup = groupList->list[ topItemPickName + idata ];
			topItemSelectSpeciesLabel = 0;
			retVal = STATE_REFRANGES_SELECT_SPECIES_LABEL;
			stateStack.Push( STATE_REFRANGES_PICK_NAME );
			break;

		// This logic assumes a maximum of one less than two full pages of groups
		// at a maximum.
		case MSG_GUI_OUT_UP:
			if ( topItemPickName != 0 )
			{
				topItemPickName = 0;
				doUpdate = true;
			}
			break;

		case MSG_GUI_OUT_DOWN:
			if ( topItemPickName == 0 )
			{
				topItemPickName += 4;
				doUpdate = true;
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
			}
			break;

		default:
			break;
	}

	if ( doUpdate )
	{
		GuiReceiveMsg_t			msg;
		GuiMiddleListData_t *	d = &msg.data.middleList;
		int						i;

		msg.msgId = MSG_GUI_UPDATE_LIST;
		memset( d, 0, sizeof( GuiMiddleListData_t ) );
		
		for ( i = 0; i < 5 && i < groupList->len - topItemPickName; i++ )
		{
			wcscpy( d->items[ i ][0], species.GetGroupString ( groupList->list[ i + topItemPickName ], &strings ) );
		}
		d->isSelectable = 1;
		d->hasUpDown = 1;

		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}
	
	return retVal;
}

/*
 * StateRefRangesSetAgeRange
 */
/*
void
StateRefRangesSetAgeRange::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	data = &msg.data.middleEnterDate;

	SetTopLabel( strings[ TSTR_ENTER_AGE_RANGE ] );

	msg.msgId = MSG_GUI_MIDDLE_ENTER_DATE;
	strcpy( data->labelLeft, "" );
	strcpy( data->labelMiddle, "" );
	strcpy( data->labelRight, "" );
	strcpy( data->textLeft, "0" );
	strcpy( data->textMiddle, "2" );
	strcpy( data->textRight, strings[ MSTR_YRS ] );
	data->buttonText[0] = '\0';

	SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

state_t
StateRefRangesSetAgeRange::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_REFRANGES_SET_AGE_RANGE;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_UP:
			break;
		case MSG_GUI_OUT_DOWN:
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Done
					retVal = STATE_REFRANGES_LIST_ANALYTE_RANGES;
					stateStack.Push( STATE_REFRANGES_SET_AGE_RANGE );
					break;
			}
			break;

		default:
			break;
	}

	return retVal;
}
*/

/*
 * StateRefRangesListAnalyteRanges
 */

void
StateRefRangesListAnalyteRanges::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	wchar_t					wBuff [50];
    char                    sBuff [50];
	int						i;

    memset (wBuff, 0, 50);
	// Put the gender in the title if a Piccolo and not a control
	if ( (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) &&
		 ((selectedSampleType < STYPE_CONTROL) || (selectedSampleType > STYPE_LAST_CONTROL)) )
	{
		wsprintf( wBuff, L"%s %s %s", strings.Gender( gender ), species.GetSampleTypeString ( selectedSampleType, &strings ), strings[ TSTR_RANGES ] );
	}
	else
	{
		wsprintf( wBuff, L"%s %s", species.GetSampleTypeString(selectedSampleType, &strings), strings[ TSTR_RANGES ] );
	}

	// Get the appropriate analyte list.
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		analyteList = analytes.PiccoloAnalytesWithRefRanges();
	}
	else
	{
		analyteList = analytes.VetScanAnalytesWithRefRanges();
	}

	SetTopLabel( wBuff );

	msg.msgId = MSG_GUI_MIDDLE_LIST_REF_RANGES;

	for ( i = 0; i < 5 && i < analyteList->len - topItemListAnalyteRanges; i++ )
	{
		AnalyteId_t	analyte = analyteList->list[ topItemListAnalyteRanges + i ];
		wcscpy( data->items[ i ][0], analytes.GetAnalysisString ( analyte, &strings) );
        memset (sBuff, 0, 50);
		FormatRefRange( sBuff,
						refRanges->ranges[ selectedSampleType ][ analyte ][ gender-1 ],
						units[ analyte ] );
        wsprintf (data->items[ i ][1], L"%S", sBuff);
		wcscpy (data->items[ i ][2], units.String( analyte ) );
	}
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false );
}

state_t
StateRefRangesListAnalyteRanges::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_REFRANGES_LIST_ANALYTE_RANGES;
	int		idata = *(int *)data;
	bool	doUpdate = false;
    char    sBuff [50];

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			selectedAnalyte = analyteList->list[ topItemListAnalyteRanges + idata ];
			retVal = STATE_REFRANGES_MODIFY_RANGE;
			stateStack.Push( STATE_REFRANGES_LIST_ANALYTE_RANGES );
			break;

		case MSG_GUI_OUT_UP:
			if ( topItemListAnalyteRanges > 0 )
			{
				topItemListAnalyteRanges -= 4;
				if ( topItemListAnalyteRanges < 0 )
				{
					topItemListAnalyteRanges = 0;
				}
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_DOWN:
			if ( analyteList->len > topItemListAnalyteRanges + 5 )
			{
				topItemListAnalyteRanges += 4;
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
			}
			break;

		default:
			break;
	}

	if ( doUpdate )
	{
		GuiReceiveMsg_t			msg;
		GuiMiddleListData_t *	data = &msg.data.middleList;
		int						i;
		
		memset( &msg, 0, sizeof( msg ) );
		msg.msgId = MSG_GUI_UPDATE_LIST_REF_RANGES;

		for ( i = 0; i < 5 && i < analyteList->len - topItemListAnalyteRanges; i++ )
		{
			AnalyteId_t	analyte = analyteList->list[ topItemListAnalyteRanges + i ];
			wcscpy( data->items[ i ][0], analytes.GetAnalysisString ( analyte, &strings ));
            memset (sBuff, 0, 50);
			FormatRefRange(sBuff, 
                           refRanges->ranges[ selectedSampleType ][ analyte ][ gender-1 ],
						   units[ analyte ] );
            wsprintf (data->items[ i ][1], L"%S", sBuff);
			wcscpy( data->items[ i ][2], units.String( analyte ) );
		}
		data->isSelectable = 1;
		data->hasUpDown = 1;
	
		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}

	return retVal;
}

/*
 * StateRefRangesListSpeciesRanges
 */

void
StateRefRangesListSpeciesRanges::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	wchar_t					wBuff [50];
    char                    sBuff [50];
	int						i;

	activeRanges = GetActiveRefRanges( refRanges, &strings);

    memset (wBuff, 0, 50);
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		wsprintf( wBuff, L"%s %s %s", strings.Gender( gender ), analytes.GetAnalysisString ( selectedAnalyte, &strings), strings[ TSTR_RANGES ] );
	}
	else
	{
		wsprintf( wBuff, L"%s %s", analytes.GetAnalysisString ( selectedAnalyte, &strings) , strings[ TSTR_RANGES ] );
	}
	SetTopLabel( wBuff );

	memset( &msg, 0, sizeof( GuiReceiveMsg_t ) );
	
	msg.msgId = MSG_GUI_MIDDLE_LIST_REF_RANGES;

	for ( i = 0; i < 5 && i < activeRanges->len - topItemListSpeciesRanges; i++ )
	{
		SampleType_t	sampleType = activeRanges->list[ topItemListSpeciesRanges + i ];
		wcscpy( data->items[ i ][0], species.GetSampleTypeString( sampleType, &strings) );
        memset (sBuff, 0, 50);
		FormatRefRange( sBuff,
						refRanges->ranges[ sampleType ][ selectedAnalyte ][ gender-1 ],
						units[ selectedAnalyte ] );
        wsprintf (data->items[ i ][1], L"%S", sBuff);
		wcscpy( data->items[ i ][2], units.String( selectedAnalyte ) );
	}
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false );
}

state_t
StateRefRangesListSpeciesRanges::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_REFRANGES_LIST_SPECIES_RANGES;
	int		idata = *(int *)data;
	bool	doUpdate = false;
    char    sBuff [50];

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			selectedSampleType = activeRanges->list[ topItemListSpeciesRanges + idata ];
			retVal = STATE_REFRANGES_MODIFY_RANGE;
			stateStack.Push( STATE_REFRANGES_LIST_SPECIES_RANGES );
			break;

		case MSG_GUI_OUT_UP:
			if ( topItemListSpeciesRanges > 0 )
			{
				topItemListSpeciesRanges -= 4;
				if ( topItemListSpeciesRanges < 0 )
				{
					topItemListSpeciesRanges = 0;
				}
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_DOWN:
			if ( activeRanges->len > topItemListSpeciesRanges + 5 )
			{
				topItemListSpeciesRanges += 4;
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
			}
			break;

		default:
			break;
	}

	if ( doUpdate )
	{
		GuiReceiveMsg_t			msg;
		GuiMiddleListData_t *	data = &msg.data.middleList;
		int						i;

		memset( &msg, 0, sizeof( msg ) );
		msg.msgId = MSG_GUI_UPDATE_LIST_REF_RANGES;

		for ( i = 0; i < 5 && i < activeRanges->len - topItemListSpeciesRanges; i++ )
		{
			SampleType_t	sampleType = activeRanges->list[ topItemListSpeciesRanges + i ];
			wcscpy( data->items[ i ][0], species.GetSampleTypeString( sampleType, &strings) );
            memset (sBuff, 0, 50);
			FormatRefRange( sBuff,
							refRanges->ranges[ sampleType ][ selectedAnalyte ][ gender-1 ],
							units[ selectedAnalyte ] );
            wsprintf (data->items[ i ][1], L"%S", sBuff);
			wcscpy( data->items[ i ][2], units.String( selectedAnalyte ) );
		}
		data->isSelectable = 1;
		data->hasUpDown = 1;

		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}

	return retVal;
}

/*
 * StateRefRangesModifyRange
 */

void
StateRefRangesModifyRange::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterRangeData_t *	data = &msg.data.middleEnterRange;
	wchar_t						buff[100];

	range = refRanges->ranges[ selectedSampleType ][ selectedAnalyte ][ gender-1 ];

	wsprintf( buff, L"%s %s [%s]", analytes.GetAnalysisString ( selectedAnalyte, &strings),
								 species.GetSampleTypeString (selectedSampleType, &strings),
								 units.String( selectedAnalyte ) );
	SetTopLabel( buff );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_RANGE;
	wcscpy( data->lowerLabel, strings[ MSTR_LOWER ] );
	wcscpy( data->upperLabel, strings[ MSTR_UPPER ] );
	data->lowerValue = range.low * units[ selectedAnalyte ].mult;
	data->upperValue = range.high * units[ selectedAnalyte ].mult;
	data->lowerDefault = GetDefaultRefRange( refRanges, selectedSampleType, selectedAnalyte, (Gender_t)gender ).low * units[ selectedAnalyte ].mult;
	data->upperDefault = GetDefaultRefRange( refRanges, selectedSampleType, selectedAnalyte, (Gender_t)gender ).high * units[ selectedAnalyte ].mult;
	data->format = units[ selectedAnalyte ].format;
	wcscpy( data->buttonText[0], strings[ MSTR_CLEAR ] );
	wcscpy( data->buttonText[1], strings[ MSTR_DEFAULT ] );

	SendGuiMessage( &msg, sizeof( GuiMiddleEnterRangeData_t ) );

	SetBottomButtons( strings[ BSTR_CANCEL ], false, strings[ BSTR_SAVE ], true );
}

state_t
StateRefRangesModifyRange::
ProcessMessage( int msgId, char * data )
{
	state_t				retVal = STATE_REFRANGES_MODIFY_RANGE;
	int					idata = *(int *)data;
	GuiSendRangeData_t	range = *(GuiSendRangeData_t *)data;
	CfgError_t			cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_RANGE:
			range.lower /= units[ selectedAnalyte ].mult;
			range.upper /= units[ selectedAnalyte ].mult;
			refRanges->ranges[ selectedSampleType ][ selectedAnalyte ][ gender-1 ].low = range.lower;
			refRanges->ranges[ selectedSampleType ][ selectedAnalyte ][ gender-1 ].high = range.upper;
			refRanges->lastModifiedAnalyte = selectedAnalyte;
			refRanges->lastModifiedSampleType = selectedSampleType;
			cfgErr = WriteRefRanges( REF_RANGES_CURRENT_FILE, refRanges );
			if ( cfgErr != CFG_ERR_NONE )
			{
				errNum = SERR_FILE_WRITE;
				retVal = STATE_ERROR;
			}
			else
			{
				retVal = stateStack.Pop();
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Cancel
					retVal = stateStack.Pop();
					break;
				case 1:		// Save
					SendGuiMessage( MSG_GUI_GET_RANGE );
					break;
			}
			break;
			
		default:
			break;
	}

	return retVal;
}


void
StateRefRangesSelectSpeciesLabel::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i;

	// Get the list of species we want to list.
	GetSpeciesList();
	
	SetTopLabel( strings[ TSTR_SELECT_SPECIES ] );

	memset( &msg, 0, sizeof( msg ) );
	msg.msgId = MSG_GUI_MIDDLE_LIST;

	for ( i = 0; i < 5 && i < listLen - topItemSelectSpeciesLabel; i++ )
	{
		wcscpy( data->items[ i ][0], species.GetSampleTypeString (list[ topItemSelectSpeciesLabel + i ], &strings) );
	}
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false );
}

state_t
StateRefRangesSelectSpeciesLabel::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_REFRANGES_SELECT_SPECIES_LABEL;
	int			idata = *(int *)data;
	bool		doUpdate = false;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			selectedSampleType = list[ topItemSelectSpeciesLabel + idata ];
			refRanges->isDefined[ selectedSampleType ] = true;
			cfgErr = WriteRefRanges( REF_RANGES_CURRENT_FILE, refRanges );
			if ( cfgErr == CFG_ERR_NONE )
			{
				settingsData->sampleTypeAdvanced = true;
				cfgErr = WriteSettings( settingsData );
			}
			if ( cfgErr != CFG_ERR_NONE )
			{
				errNum = SERR_FILE_WRITE;
				retVal = STATE_ERROR;
			}
			else
			{
				retVal = STATE_REFRANGES_LIST_ANALYTE_RANGES;
				stateStack.Push( STATE_REFRANGES_SELECT_SPECIES_LABEL );
			}
			break;

		case MSG_GUI_OUT_UP:
			if ( topItemSelectSpeciesLabel > 0 )
			{
				topItemSelectSpeciesLabel -= 4;
				if ( topItemSelectSpeciesLabel < 0 )
				{
					topItemSelectSpeciesLabel = 0;
				}
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_DOWN:
			if ( listLen > topItemSelectSpeciesLabel + 5 )
			{
				topItemSelectSpeciesLabel += 4;
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
			}
			break;

		default:
			break;
	}

	if ( doUpdate )
	{
		GuiReceiveMsg_t			msg;
		GuiMiddleListData_t *	data = &msg.data.middleList;
		int						i;

		memset( &msg, 0, sizeof( msg ) );
		msg.msgId = MSG_GUI_UPDATE_LIST;

		for ( i = 0; i < 5 && i < listLen - topItemSelectSpeciesLabel; i++ )
		{
			wcscpy( data->items[ i ][0], species.GetSampleTypeString ( list[ topItemSelectSpeciesLabel + i ], &strings ));
		}
		data->isSelectable = 1;
		data->hasUpDown = 1;

		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}

	return retVal;
}

// GLD
// Maybe this should be part of RefRanges - especially if it becomes a class.
void
StateRefRangesSelectSpeciesLabel::
GetSpeciesList()
{
	int					i, j;
	SpeciesGroupList_t	groupList;
	
	groupList = species.GetGroupList( selectedGroup );

	listLen = groupList.len;
				
	if ( listLen != 0 )
	{
		for ( i = 0, j = 0; i < listLen; i++ )
		{
			if ( !refRanges->isDefined[ groupList.list[ i ] ] )
			{
				list[ j ] = groupList.list[ i ];
				j++;
			}
		}
		listLen = j;
	}
}

/*
 * StateRefRangesRemoveSpecies
 */

void
StateRefRangesRemoveSpecies::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i;

	activeRanges = GetActiveRefRanges( refRanges, &strings);

	SetTopLabel( strings[ TSTR_REMOVE_SPECIES ] );

	memset( &msg, 0, sizeof( GuiReceiveMsg_t ) );
	
	msg.msgId = MSG_GUI_MIDDLE_LIST;

	for ( i = 0; i < 5 && i < activeRanges->len - topItemRemoveSpecies; i++ )
	{
		wcscpy( data->items[ i ][0], species.GetSampleTypeString ( activeRanges->list[ topItemRemoveSpecies + i ] , &strings));
	}
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateRefRangesRemoveSpecies::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_REFRANGES_REMOVE_SPECIES;
	int		idata = *(int *)data;
	bool	doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			selectedSampleType = activeRanges->list[ topItemRemoveSpecies + idata ];
			retVal = STATE_REFRANGES_CONFIRM_REMOVAL;
			stateStack.Push( STATE_REFRANGES_REMOVE_SPECIES );
			break;

		case MSG_GUI_OUT_UP:
			if ( topItemRemoveSpecies > 0 )
			{
				topItemRemoveSpecies -= 4;
				if ( topItemRemoveSpecies < 0 )
				{
					topItemRemoveSpecies = 0;
				}
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_DOWN:
			if ( activeRanges->len > topItemRemoveSpecies + 5 )
			{
				topItemRemoveSpecies += 4;
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
			}
			break;

		default:
			break;
	}

	if ( doUpdate )
	{
		GuiReceiveMsg_t			msg;
		GuiMiddleListData_t *	data = &msg.data.middleList;
		int						i;

		memset( &msg, 0, sizeof( msg ) );
		msg.msgId = MSG_GUI_UPDATE_LIST;

		for ( i = 0; i < 5 && i < activeRanges->len - topItemRemoveSpecies; i++ )
		{
			wcscpy( data->items[ i ][0], species.GetSampleTypeString (activeRanges->list[ topItemRemoveSpecies + i ], &strings ));
		}
		data->isSelectable = 1;
		data->hasUpDown = 1;

		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}

	return retVal;
}

/*
 * StateRefRangesConfirmRemoval
 */

void
StateRefRangesConfirmRemoval::
Enter()
{
	GuiReceiveMsg_t		guiMsg;
	wchar_t				buff[ 256 ];
	
	wsprintf( buff, L"%s %s", species.GetSampleTypeString (selectedSampleType, &strings), strings[ ESTR_REMOVE_FROM_TYPE_LIST ] );

	guiMsg.msgId = MSG_GUI_WIN_WARNING2;
	wcscpy( guiMsg.data.winWarning.topLabel, strings[ TSTR_WARNING ] );
	wcscpy( guiMsg.data.winWarning.message, buff );
	wcscpy( guiMsg.data.winWarning.buttonText[0], strings[ BSTR_CONTINUE ] );
	wcscpy( guiMsg.data.winWarning.buttonText[1], strings[ BSTR_CANCEL ] );
	
	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
}

state_t
StateRefRangesConfirmRemoval::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_REFRANGES_CONFIRM_REMOVAL;
	int			idata = *(int *)data;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_REF_RANGES_REMOVE ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_REFRANGES_CONFIRM_REMOVAL );
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Continue
					refRanges->isDefined[ selectedSampleType ] = false;
					cfgErr = WriteRefRanges( REF_RANGES_CURRENT_FILE, refRanges );
					if ( cfgErr == CFG_ERR_NONE )
					{
						settingsData->sampleTypeAdvanced = true;
						WriteSettings( settingsData );
					}
					if ( cfgErr != CFG_ERR_NONE )
					{
						errNum = SERR_FILE_WRITE;
						retVal = STATE_ERROR;
					}
					else
					{
						retVal = stateStack.Pop();
					}
					break;
				case 1:		// Cancel
					retVal = stateStack.Pop();
					break;
			}
			break;

		default:
			break;
	}

	return retVal;
}

/*
 * StateRefRangesDefaultSpecies
 */

void
StateRefRangesDefaultSpecies::
Enter()
{
	GuiReceiveMsg_t		guiMsg;

	SetTopLabel( strings[ TSTR_FACTORY_DEFAULT_SPECIES ] );

	guiMsg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	wcscpy( guiMsg.data.middleMessage.message, strings[ MSTR_PRESS_YES_REF_RANGES ] );
	
	SendGuiMessage( &guiMsg, sizeof( GuiMiddleMessageData_t ) );


	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false, strings[ BSTR_YES ], true );
}

state_t
StateRefRangesDefaultSpecies::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_REFRANGES_DEFAULT_SPECIES;
	int			idata = *(int *)data;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
				case 2:		// Yes
					SetRefRangesToDefaultSpecies( refRanges, factoryData->abaxisProductCode );
					cfgErr = WriteRefRanges( REF_RANGES_CURRENT_FILE, refRanges );
					if ( cfgErr == CFG_ERR_NONE )
					{
						settingsData->sampleTypeAdvanced = false;
						WriteSettings( settingsData );
					}
					if ( cfgErr != CFG_ERR_NONE )
					{
						errNum = SERR_FILE_WRITE;
						retVal = STATE_ERROR;
					}
					else
					{
						retVal = stateStack.Pop();
					}
					break;
			}
			break;

		default:
			break;
	}

	return retVal;
}

/*
 * StateRefRangesSelectAnalyteGender
 */

void
StateRefRangesSelectAnalyteGender::
Enter()
{
	GuiReceiveMsg_t guiMsg;
	
	SetTopLabel( strings[ TSTR_SELECT_GENDER ] );

	guiMsg.msgId = MSG_GUI_MIDDLE_BUTTONS;
	
	memset( &guiMsg.data.middleButtons, 0, sizeof( GuiMiddleButtonsData_t ) );
	wcscpy( guiMsg.data.middleButtons.buttonText[0], strings[ MSTR_UNKNOWN ] );
	wcscpy( guiMsg.data.middleButtons.buttonText[1], strings[ MSTR_MALE ] );
	wcscpy( guiMsg.data.middleButtons.buttonText[2], strings[ MSTR_FEMALE ] );
	guiMsg.data.middleButtons.buttonHighlighted[0] = 1;
	SendGuiMessage( &guiMsg, sizeof( GuiMiddleButtonsData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

int
StateRefRangesSelectAnalyteGender::
ProcessMessage( int msgId, char * data )
{
	int	retVal = STATE_REFRANGES_SELECT_ANALYTE_GENDER;
	int	idata = (int)*data;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:	gender = GENDER_UNKNOWN;	break;
				case 1:	gender = GENDER_MALE;		break;
				case 2:	gender = GENDER_FEMALE;		break;
			}
			
			retVal = STATE_REFRANGES_LIST_SPECIES_RANGES;
			stateStack.Push( STATE_REFRANGES_SELECT_ANALYTE_GENDER );
			break;
	
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
			}
			break;

		default:
			break;
	}
	
	return retVal;
}


/*
 * StateRefRangesSelectDemographicGender
 */

void
StateRefRangesSelectDemographicGender::
Enter()
{
	GuiReceiveMsg_t guiMsg;
	
	SetTopLabel( strings[ TSTR_SELECT_GENDER ] );

	guiMsg.msgId = MSG_GUI_MIDDLE_BUTTONS;
	
	memset( &guiMsg.data.middleButtons, 0, sizeof( GuiMiddleButtonsData_t ) );
	wcscpy( guiMsg.data.middleButtons.buttonText[0], strings[ MSTR_UNKNOWN ] );
	wcscpy( guiMsg.data.middleButtons.buttonText[1], strings[ MSTR_MALE ] );
	wcscpy( guiMsg.data.middleButtons.buttonText[2], strings[ MSTR_FEMALE ] );
	if ( factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE )
	guiMsg.data.middleButtons.buttonHighlighted[0] = 1;
	SendGuiMessage( &guiMsg, sizeof( GuiMiddleButtonsData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false );
}

int
StateRefRangesSelectDemographicGender::
ProcessMessage( int msgId, char * data )
{
	int	retVal = STATE_REFRANGES_SELECT_DEMOGRAPHIC_GENDER;
	int	idata = (int)*data;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:	gender = GENDER_UNKNOWN;	break;
				case 1:	gender = GENDER_MALE;		break;
				case 2:	gender = GENDER_FEMALE;		break;
			}
			
			retVal = STATE_REFRANGES_LIST_ANALYTE_RANGES;
			stateStack.Push( STATE_REFRANGES_SELECT_DEMOGRAPHIC_GENDER );
			break;
	
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Back
			retVal = stateStack.Pop();
			break;

		default:
			break;
	}
	
	return retVal;
}

