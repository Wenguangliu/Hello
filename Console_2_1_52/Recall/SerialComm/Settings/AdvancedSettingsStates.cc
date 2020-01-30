/*
 * FILENAME
 * 	AdvancedSettingsStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine advanced
 * 	settings states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdio.h>
#include <string.h>

#include "AdvancedSettingsStates.h"
#include "ParseSettings.h"

/*
 * class StateAdvancedSelect
 */
 
void
StateAdvancedSelect::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsData_t *	data = &msg.data.middleButtons;

	SetTopLabel( strings[ TSTR_ADVANCED_SETTINGS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS;
	memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
	strcpy( data->buttonText[0], strings[ MSTR_DATA_ENTRY_OPTIONS ] );
	strcpy( data->buttonText[1], strings[ MSTR_ASTM_ORDERS ] );
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		strcpy( data->buttonText[2], strings[ MSTR_ANALYTE_OPTIONS ] );
	}
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateAdvancedSelect::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ADVANCED_SELECT;
	int		idata = *(int *)data;
	

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Data Entry Options
					retVal = STATE_ADVANCED_DATA_ENTRY_OPTIONS;
					stateStack.Push( STATE_ADVANCED_SELECT );
					break;
				case 1:		// ASTM Orders
					retVal = STATE_ADVANCED_ASTM_ORDER_METHOD;
					stateStack.Push( STATE_ADVANCED_SELECT );
					break;
				case 2:		// Analyte Options
					retVal = STATE_ADVANCED_ANALYTE_OPTIONS;
					stateStack.Push( STATE_ADVANCED_SELECT );
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
 * class StateAdvancedAnalyteOptions
 */
 
void
StateAdvancedAnalyteOptions::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleMultiSelectData_t *	data = &msg.data.middleMultiSelect;

	SetTopLabel( strings[ TSTR_ANALYTE_OPTIONS ] );
	
	msg.msgId = MSG_GUI_MIDDLE_MULTI_SELECT;
	
	memset( data, 0, sizeof( GuiMiddleMultiSelectData_t ) );
	strcpy( data->buttonText[0], strings[ MSTR_EGFR ] );
	data->buttonChecked[0] = settingsData->egfrEnabled ? 1 : 0;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateAdvancedAnalyteOptions::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ADVANCED_ANALYTE_OPTIONS;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_TOGGLE:
			switch ( idata & 0x0F )
			{
				case 0:
					settingsData->egfrEnabled = idata & 0xF0 ? 1 : 0;
					break;
			}
			WriteSettings( settingsData );
			break;
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
				case 1:		// Home
					retVal = stateStack.Home();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

	return retVal;
}

/*
 * class StateAdvancedDataEntryOptions
 */
 
void
StateAdvancedDataEntryOptions::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleMultiSelectData_t *	data = &msg.data.middleMultiSelect;
	bool							altIdEnabled;

	SetTopLabel( strings[ TSTR_DATA_ENTRY_OPTIONS ] );
	
	msg.msgId = MSG_GUI_MIDDLE_MULTI_SELECT;
	
	memset( data, 0, sizeof( GuiMiddleMultiSelectData_t ) );
	strcpy( data->buttonText[0], strings[ MSTR_OPERATOR_ID ] );
	strcpy( data->buttonText[1], strings[ MSTR_ALTERNATE_ID ] );
	strcpy( data->buttonText[2], strings[ MSTR_GENDER ] );
	strcpy( data->buttonText[3], strings[ MSTR_AGE ] );
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		strcpy( data->buttonText[4], strings[ MSTR_RACE ] );
	}

	altIdEnabled = settingsData->alternateIdEnabled			||
				   settingsData->doctorIdEnabled			||
				   settingsData->locationEnabled			||
				   settingsData->phoneNumberEnabled			||
				   settingsData->admissionOwnerIdEnabled	||
				   settingsData->dobEnabled					||
				   settingsData->dovEnabled					||
				   settingsData->sampleIdEnabled;

	data->buttonChecked[0] = settingsData->operatorIdEnabled	? 1 : 0;
	data->buttonChecked[1] = altIdEnabled						? 1 : 0;
	data->buttonChecked[2] = settingsData->genderEnabled		? 1 : 0;
	data->buttonChecked[3] = settingsData->ageEnabled			? 1 : 0;
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		data->buttonChecked[4] = settingsData->raceEnabled			? 1 : 0;
	}
	
	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateAdvancedDataEntryOptions::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ADVANCED_DATA_ENTRY_OPTIONS;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_TOGGLE:
			switch ( idata & 0x0F )
			{
				case 0:
					settingsData->operatorIdEnabled = idata & 0xF0 ? 1 : 0;
					if ( !settingsData->operatorIdEnabled && (expirys.NumOps() > 0) )
					{
						settingsData->operatorIdEnabled = true;
						errNum = SERR_CANNOT_REMOVE_OP_ID_SCREEN;
						retVal = STATE_ERROR;
						stateStack.Push( STATE_ADVANCED_DATA_ENTRY_OPTIONS );
					}
					break;
				case 1:
					retVal = STATE_ADVANCED_ALTERNATE_ID;
					stateStack.Push( STATE_ADVANCED_DATA_ENTRY_OPTIONS );
					break;
				case 2:
					settingsData->genderEnabled = idata & 0xF0 ? 1 : 0;
					break;
				case 3:
					settingsData->ageEnabled = idata & 0xF0 ? 1 : 0;
					break;
				case 4:
					settingsData->raceEnabled = idata & 0xF0 ? 1 : 0;
					break;
				case 5:
					settingsData->egfrEnabled = idata & 0xF0 ? 1 : 0;
					break;
			}
			WriteSettings( settingsData );
			break;
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
				case 1:		// Home
					retVal = stateStack.Home();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

	return retVal;
}

/*
 * class StateAdvancedAstmOrderMethod
 */
 
void
StateAdvancedAstmOrderMethod::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleOptionInfoData_t *	data = &msg.data.middleOptionInfo;

	SetTopLabel( strings[ TSTR_ASTM_ORDER_METHOD ] );
	
	msg.msgId = MSG_GUI_MIDDLE_OPTION_INFO;
	
	memset( data, 0, sizeof( GuiMiddleMultiSelectData_t ) );
	strcpy( data->buttonText[0], strings[ MSTR_WORK_LIST ] );
	strcpy( data->buttonText[1], strings[ MSTR_ENTER_PATIENT_ID ] );
	strcpy( data->buttonText[2], strings[ MSTR_DISABLED ] );

	switch ( settingsData->astmOrderMethod )
	{
		case ASTM_ORDERS_WORK_LIST:	data->selectedButton = 0;	break;
		case ASTM_ORDERS_ENTER_ID:	data->selectedButton = 1;	break;
		case ASTM_ORDERS_DISABLED:	data->selectedButton = 2;	break;
		default: break;
	}
		
	SendGuiMessage( &msg, sizeof( GuiMiddleSelectOptionData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateAdvancedAstmOrderMethod::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ADVANCED_ASTM_ORDER_METHOD;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_TOGGLE:
			switch ( idata )
			{
				case 0xF0: settingsData->astmOrderMethod = ASTM_ORDERS_WORK_LIST;	break;
				case 0xF1: settingsData->astmOrderMethod = ASTM_ORDERS_ENTER_ID;	break;
				case 0xF2: settingsData->astmOrderMethod = ASTM_ORDERS_DISABLED;	break;
				default: break;
			}
			WriteSettings( settingsData );
			break;

		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_ASTM_ORDER_METHOD ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_ADVANCED_ASTM_ORDER_METHOD );
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
				default:
					break;
			}
			break;
		default:
			break;
	}

	return retVal;
}

/*
 * class StateAdvancedAlternateId
 */
 
void
StateAdvancedAlternateId::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleListMultiData_t *	data = &msg.data.middleListMulti;

	SetTopLabel( strings[ TSTR_ALTERNATE_ID ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST_MULTI;
	memset( data, 0, sizeof( GuiMiddleListMultiData_t ) );
	
	strcpy( data->items[0], strings[ MSTR_ALTERNATE_ID ] );
	strcpy( data->items[1], strings[ MSTR_SAMPLE_ID ] );
	strcpy( data->items[2], strings[ MSTR_DOCTOR_ID ] );
	strcpy( data->items[3], strings[ MSTR_LOCATION ] );
	strcpy( data->items[4], strings[ MSTR_PHONE_NUMBER ] );
	
	data->hasUpDown = 1;

	if ( settingsData->alternateIdEnabled )
	{
		data->isSelected[0] = 1;
	}
	if ( settingsData->sampleIdEnabled )
	{
		data->isSelected[1] = 1;
	}
	if ( settingsData->doctorIdEnabled )
	{
		data->isSelected[2] = 1;
	}
	if ( settingsData->locationEnabled )
	{
		data->isSelected[3] = 1;
	}
	if ( settingsData->phoneNumberEnabled )
	{
		data->isSelected[4] = 1;
	}

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
	
	page = 0;
}

state_t
StateAdvancedAlternateId::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ADVANCED_ALTERNATE_ID;
	int		idata = *(int *)data;
	bool	doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			if ( page == 0 )
			{
				settingsData->alternateIdEnabled = idata & 0x01;
				settingsData->sampleIdEnabled = idata & 0x02;
				settingsData->doctorIdEnabled = idata & 0x04;
				settingsData->locationEnabled = idata & 0x08;
				settingsData->phoneNumberEnabled = idata & 0x10;
			}
			else
			{
				settingsData->phoneNumberEnabled = idata & 0x01;
				settingsData->admissionOwnerIdEnabled = idata & 0x02;
				settingsData->dobEnabled = idata & 0x04;
				settingsData->dovEnabled = idata & 0x08;
			}	

			WriteSettings( settingsData );
//			retVal = stateStack.Pop();
			break;
		case MSG_GUI_OUT_UP:
			if ( page == 1 )
			{
				page = 0;
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_DOWN:
			if ( page == 0 )
			{
				page = 1;
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
		GuiReceiveMsg_t				msg;
		GuiMiddleListMultiData_t *	data = &msg.data.middleListMulti;

		msg.msgId = MSG_GUI_UPDATE_LIST_MULTI;
		memset( data, 0, sizeof( GuiMiddleListData_t ) );
		
		if ( page == 0 )
		{
			strcpy( data->items[0], strings[ MSTR_ALTERNATE_ID ] );
			strcpy( data->items[1], strings[ MSTR_SAMPLE_ID ] );
			strcpy( data->items[2], strings[ MSTR_DOCTOR_ID ] );
			strcpy( data->items[3], strings[ MSTR_LOCATION ] );
			strcpy( data->items[4], strings[ MSTR_PHONE_NUMBER ] );

			if ( settingsData->alternateIdEnabled )
			{
				data->isSelected[0] = 1;
			}
			if ( settingsData->sampleIdEnabled )
			{
				data->isSelected[1] = 1;
			}
			if ( settingsData->doctorIdEnabled )
			{
				data->isSelected[2] = 1;
			}
			if ( settingsData->locationEnabled )
			{
				data->isSelected[3] = 1;
			}
			if ( settingsData->phoneNumberEnabled )
			{
				data->isSelected[4] = 1;
			}
		}
		else
		{
			if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
			{
				strcpy( data->items[0], strings[ MSTR_PHONE_NUMBER ] );
				strcpy( data->items[1], strings[ MSTR_ADMISSION_ID ] );
				strcpy( data->items[2], strings[ MSTR_DATE_OF_BIRTH ] );
			}
			else
			{
				strcpy( data->items[0], strings[ MSTR_PHONE_NUMBER ] );
				strcpy( data->items[1], strings[ MSTR_OWNER_ID ] );
				strcpy( data->items[2], strings[ MSTR_DATE_OF_BIRTH ] );
				strcpy( data->items[3], strings[ MSTR_DATE_OF_VACCINATION ] );
			}

			if ( settingsData->phoneNumberEnabled )
			{
				data->isSelected[0] = 1;
			}
			if ( settingsData->admissionOwnerIdEnabled )
			{
				data->isSelected[1] = 1;
			}
			if ( settingsData->dobEnabled )
			{
				data->isSelected[2] = 1;
			}
			if ( settingsData->dovEnabled && (factoryData->abaxisProductCode != PICCOLO_PRODUCT_CODE) )
			{
				data->isSelected[3] = 1;
			}
		}			
		data->hasUpDown = 1;
	
		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}
		
	return retVal;
}

