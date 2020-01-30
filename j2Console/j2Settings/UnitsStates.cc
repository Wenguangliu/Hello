/*
 * FILENAME
 * 	UnitsStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine set units
 * 	states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "UnitsStates.h"
#include "SettingsMessages.h"
#include "CfgErr.h"


Units			StateUnits::units;
AnalyteId_t		StateUnits::selectedAnalyte;
int				StateUnits::topItem;
Analytes		StateUnits::analytes;
AnalyteList_t	StateUnits::analyteList;

/*
 * class StateUnitsSelect
 */
 
void
StateUnitsSelect::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsData_t *	data = &msg.data.middleButtons;

	SetTopLabel( strings[ TSTR_SET_UNITS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS;

	memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_ALL_ANALYTES ] );
	wcscpy( data->buttonText[1], strings[ MSTR_ANALYTE_GROUPS ] );
	wcscpy( data->buttonText[2], strings[ MSTR_SINGLE_ANALYTE ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateUnitsSelect::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_UNITS_SELECT;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// All Analytes
					retVal = STATE_UNITS_GLOBAL;
					stateStack.Push( STATE_UNITS_SELECT );
					break;
				case 1:		// Analyte Groups
					retVal = STATE_UNITS_GROUP_SELECT;
					stateStack.Push( STATE_UNITS_SELECT );
					break;
				case 2:		// Single Analyte
					retVal = STATE_UNITS_LIST;
					stateStack.Push( STATE_UNITS_SELECT );

					/*
					 * We set the scroll index to 0 here instead of in the Enter
					 * method for StateUnitsList so that when we go Back to
					 * StateUnitsList from StateUnitsSet we leave it as it was
					 * before so the user is at the same place in the list.
					 */
					topItem = 0;
					break;
				default:
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
		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateUnitsGroupSelect
 */
 
void
StateUnitsGroupSelect::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsData_t *	data = &msg.data.middleButtons;

	SetTopLabel( strings[ TSTR_GROUP_UNITS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS;

	memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_ELECTROLYTES ] );
	wcscpy( data->buttonText[1], strings[ MSTR_ENZYMES ] );
	wcscpy( data->buttonText[2], strings[ MSTR_LIPIDS ] );
	wcscpy( data->buttonText[3], strings[ MSTR_MINERALS ] );
	wcscpy( data->buttonText[4], strings[ MSTR_PROTEINS ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateUnitsGroupSelect::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_UNITS_GROUP_SELECT;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Electrolytes
					retVal = STATE_UNITS_ELECTROLYTES;
					stateStack.Push( STATE_UNITS_GROUP_SELECT );
					break;
				case 1:		// Enzymes
					retVal = STATE_UNITS_ENZYMES;
					stateStack.Push( STATE_UNITS_GROUP_SELECT );
					break;
				case 2:		// Lipids
					retVal = STATE_UNITS_LIPIDS;
					stateStack.Push( STATE_UNITS_GROUP_SELECT );
					break;
				case 3:		// Minerals
					retVal = STATE_UNITS_MINERALS;
					stateStack.Push( STATE_UNITS_GROUP_SELECT );
					break;
				case 4:		// Proteins
					retVal = STATE_UNITS_PROTEINS;
					stateStack.Push( STATE_UNITS_GROUP_SELECT );
					break;
				default:
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
		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateUnitsGlobal
 */
 
void
StateUnitsGlobal::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_ALL_UNITS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;
	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_NON_SI ] );
	wcscpy( data->buttonText[1], strings[ MSTR_SI ] );
	data->buttonHighlighted[0] = false;
	data->buttonHighlighted[1] = false;
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateUnitsGlobal::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_UNITS_GLOBAL;
	int			idata = *(int *)data;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Non SI
					cfgErr = units.SetAllNonSi();
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
				case 1:		// SI
					cfgErr = units.SetAllSi();
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
				default:
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
			infoText = strings[ ISTR_UNITS_GLOBAL ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_UNITS_GLOBAL );
			break;
		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateUnitsElectrolytes
 */
 
void
StateUnitsElectrolytes::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_ELECTROLYTES_UNITS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	wcscpy( data->buttonText[0], L"mmol/L" );
	wcscpy( data->buttonText[1], L"mEq/L" );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateUnitsElectrolytes::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_UNITS_ELECTROLYTES;
	int			idata = *(int *)data;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// mmol/L
					cfgErr = units.SetElectrolytes( UID_MMOLL );
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
				case 1:		// mEq/L
					cfgErr = units.SetElectrolytes( UID_MEQL );
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
				default:
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_SET_ELECTROLYTE_UNITS ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_UNITS_ELECTROLYTES );
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
 * class StateUnitsEnzymes
 */
 
void
StateUnitsEnzymes::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_ENZYMES_UNITS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	wcscpy( data->buttonText[0], L"U/L" );
	wcscpy( data->buttonText[1], L"ukat/L" );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateUnitsEnzymes::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_UNITS_ENZYMES;
	int			idata = *(int *)data;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// U/L
					cfgErr = units.SetEnzymes( UID_UL );
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
				case 1:		// ukat/L
					cfgErr = units.SetEnzymes( UID_UKATL );
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
				default:
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_SET_ENZYME_UNITS ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_UNITS_ENZYMES );
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
 * class StateUnitsLipids
 */
 
void
StateUnitsLipids::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_LIPIDS_UNITS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	wcscpy( data->buttonText[0], L"mg/dL" );
	wcscpy( data->buttonText[1], L"mmol/L" );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateUnitsLipids::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_UNITS_LIPIDS;
	int			idata = *(int *)data;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// mg/dL
					cfgErr = units.SetLipids( UID_MGDL );
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
				case 1:		// mmol/L
					cfgErr = units.SetLipids( UID_MMOLL );
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
				default:
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_SET_LIPID_UNITS ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_UNITS_LIPIDS );
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
 * class StateUnitsMinerals
 */
 
void
StateUnitsMinerals::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_MINERALS_UNITS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	wcscpy( data->buttonText[0], L"mg/dL" );
	wcscpy( data->buttonText[1], L"mmol/L" );
	wcscpy( data->buttonText[2], L"mEq/L" );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateUnitsMinerals::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_UNITS_MINERALS;
	int			idata = *(int *)data;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// mg/dL
					cfgErr = units.SetMinerals( UID_MGDL );
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
				case 1:		// mmol/L
					cfgErr = units.SetMinerals( UID_MMOLL );
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
				case 2:		// mEq/L
					cfgErr = units.SetMinerals( UID_MEQL );
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
				default:
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_SET_MINERAL_UNITS ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_UNITS_MINERALS );
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
 * class StateUnitsProteins
 */
 
void
StateUnitsProteins::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_PROTEINS_UNITS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;
	
	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	wcscpy( data->buttonText[0], L"g/dL" );
	wcscpy( data->buttonText[1], L"g/L" );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateUnitsProteins::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_UNITS_PROTEINS;
	int			idata = *(int *)data;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// g/dL
					cfgErr = units.SetProteins( UID_GDL );
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
				case 1:		// g/L
					cfgErr = units.SetProteins( UID_GL );
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
				default:
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_SET_PROTEIN_UNITS ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_UNITS_PROTEINS );
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
 * class StateUnitsList
 */
 
void
StateUnitsList::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i, j;

	GetAnalyteList();

	SetTopLabel( strings[ TSTR_SINGLE_ANALYTE ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST;
	memset( data, 0, sizeof( GuiMiddleListData_t ) );
	
	for ( i = 0, j = topItem; i < 5 && j < analyteList.len; i++, j++ )
	{
		wcscpy( data->items[i][0], analytes.GetAnalysisString ( analyteList.list[ j ], &strings) );
	}
	
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateUnitsList::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_UNITS_LIST;
	int		idata = *(int *)data;
	bool	doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			selectedAnalyte = analyteList.list[ topItem + idata ];
			retVal = STATE_UNITS_SET;
			stateStack.Push( STATE_UNITS_LIST );
			break;
		case MSG_GUI_OUT_UP:
			if ( topItem >= 4 )
			{
				topItem -= 4;
				if ( topItem < 0 )
				{
					topItem = 0;
				}
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_DOWN:
			if ( topItem < analyteList.len - 5 )
			{
				topItem += 4;
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
		GuiMiddleListData_t *	data = &msg.data.middleList;
		int						i, j;

		msg.msgId = MSG_GUI_UPDATE_LIST;
		memset( data, 0, sizeof( GuiMiddleListData_t ) );
		
		for ( i = 0, j = topItem; i < 5 && j < analyteList.len; i++, j++ )
		{
			wcscpy( data->items[i][0], analytes.GetAnalysisString ( analyteList.list[ j ], &strings ));
		}

		data->isSelectable = 1;
		data->hasUpDown = 1;
	
		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}
		
	return retVal;
}

void
StateUnitsList::
GetAnalyteList()
{
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		analyteList = *analytes.PiccoloAnalytesWithUnits();
	}
	else
	{
		analyteList = *analytes.VetScanAnalytesWithUnits();
	}
}

/*
 * class StateUnitsSet
 */
 
void
StateUnitsSet::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleSelectOptionData_t *	data = &msg.data.middleSelectOption;
	int								i;
	int								buttonIndex;
	wchar_t							buff[50];
	
	unitOptions = units.Options( selectedAnalyte );

	wsprintf( buff, L"%s %s", analytes.GetAnalysisString (selectedAnalyte, &strings), strings[ TSTR_UNITS ] );
	SetTopLabel( buff );

	msg.msgId = MSG_GUI_MIDDLE_SELECT_OPTION;
	memset( data, 0, sizeof( GuiMiddleSelectOptionData_t ) );
	
	// Add the standard units option.
	buttonIndex = 0;
    wcscpy( data->buttonText[ buttonIndex++ ], units.ListString( unitOptions->units[0].unitId, &strings ) );

	// If the SI units option isn't the same as the common units option, add it.
	if ( unitOptions->units[1].unitId != unitOptions->units[0].unitId )
	{
        wcscpy( data->buttonText[ buttonIndex++ ], units.ListString( unitOptions->units[1].unitId, &strings ) );
	}

	// Add the alternate options.
	for ( i = 2; i < unitOptions->count; i++ )
	{
        wcscpy( data->buttonText[ buttonIndex++ ], units.ListString( unitOptions->units[ i ].unitId, &strings ) );
	}

	// Select the current option.  If SI and non-SI are the same and the current
	// option isn't the first one (default non-SI), then we need to adjust the
	// button index.
// GLD
// We could change this by just having one entry in our table for those analytes
// which have the same units for default non-SI and SI.  Then, we'd need an indicator
// to tell us which option to select for default SI when we select SI for all. 
	for ( i = 0, data->selectedButton = 0; i < unitOptions->count; i++ )
	{
		if ( unitOptions->units[ i ].unitId == unitOptions->current )
		{
			data->selectedButton = i;
			break;
		}
	}
	if ( data->selectedButton != 0 && unitOptions->units[1].unitId == unitOptions->units[0].unitId )
	{
		data->selectedButton -= 1;
	}

	SendGuiMessage( &msg, sizeof( GuiMiddleSelectOptionData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateUnitsSet::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_UNITS_SET;
	int			idata = *(int *)data;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_TOGGLE:
			// High byte set to 0xF if button is selected.  High byte clear if
			// button is deselected.
			if ( idata & 0xF0 )
			{
				idata &= 0x0F;
				
				// If it's other than the first button, and both non-SI and SI
				// are the same, increment the index.
				if ( idata > 0 && unitOptions->units[1].unitId == unitOptions->units[0].unitId )
				{
					idata++;
				}

				// Update the units data.
				cfgErr = units.Update( selectedAnalyte, unitOptions->units[ idata ].unitId );
				if ( cfgErr != CFG_ERR_NONE )
				{
					errNum = SERR_FILE_WRITE;
					retVal = STATE_ERROR;
				}
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
					break;
			}
			break;

		default:
			break;
	}
	
	return retVal;
}

