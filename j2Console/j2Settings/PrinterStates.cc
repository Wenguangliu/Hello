/*
 * FILENAME
 * 	PrinterStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine set printer
 * 	settings states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "PrinterStates.h"
#include "SettingsMessages.h"
#include "SettingsData.h"
#include "ParseSettings.h"
#include "CreateDisplay.h"
#include "CfgErr.h"

#define MAX_COPIES	99
	
/*
 * class StatePrinterSelect
 */
 
void
StatePrinterSelect::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_PRINTER_SETUP ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_CONFIGURE ] );
	wcscpy( data->buttonText[1], strings[ MSTR_TEST ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StatePrinterSelect::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_PRINTER_SELECT;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Configure
					retVal = STATE_PRINTER_CONFIGURE;
					stateStack.Push( STATE_PRINTER_SELECT );
					break;
				case 1:		// Test
					retVal = STATE_PRINTER_TEST;
					stateStack.Push( STATE_PRINTER_SELECT );
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
			infoText = strings[ ISTR_PRINTER_SETUP ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_PRINTER_SELECT );
			break;
		default:
			break;
	}
	
	return retVal;
}

/*
 * class StatePrinterConfigure
 */
 
void
StatePrinterConfigure::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_CONFIGURE_PRINTER ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_SET_DEFAULT ] );
	wcscpy( data->buttonText[1], strings[ MSTR_SELECT_REPORTS ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StatePrinterConfigure::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_PRINTER_CONFIGURE;
	int		idata = *(int *)data;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Set Default
					retVal = STATE_PRINTER_SET_DEFAULT;
					stateStack.Push( STATE_PRINTER_CONFIGURE );
					break;
				case 1:		// Select Reports
					retVal = STATE_PRINTER_SELECT_REPORTS;
					stateStack.Push( STATE_PRINTER_CONFIGURE );
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
			infoText = strings[ ISTR_PRINTER_CONFIGURE ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_PRINTER_CONFIGURE );
			break;
		default:
			break;
	}

	return retVal;
}

/*
 * class StatePrinterTest
 */
 
void
StatePrinterTest::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_SELECT_PRINTER ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS;

	memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_ANALYZER_PRINTER ] );
	wcscpy( data->buttonText[1], strings[ MSTR_EXTERNAL_PRINTER ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StatePrinterTest::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_PRINTER_TEST;
	int		idata = *(int *)data;
    PrinterMsg_t    printerMsg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Internal Printer
                    printerMsg.msgId= MSG_PRINT_INTERNAL_PRINTER_TEST;
                    errNum = SendPrinterMessage (&printerMsg);
					break;
				case 1:		// External Printer
                    printerMsg.msgId= MSG_PRINT_EXTERNAL_PRINTER_TEST;
                    errNum = SendPrinterMessage (&printerMsg);
					break;
			}
			if ( errNum != SERR_NONE )
			{
				retVal = STATE_ERROR;
				stateStack.Push( STATE_PRINTER_TEST );
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
 * class StatePrinterSetDefault
 */
 
void
StatePrinterSetDefault::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleSelectOptionData_t *	data = &msg.data.middleSelectOption;

	SetTopLabel( strings[ TSTR_SET_DEFAULT ] );

	msg.msgId = MSG_GUI_MIDDLE_SELECT_OPTION;

	memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_ANALYZER_PRINTER ] );
	wcscpy( data->buttonText[1], strings[ MSTR_EXTERNAL_PRINTER ] );
	
	data->selectedButton = settingsData->defaultPrinter == INTERNAL_PTR ? 0 : 1;
	
	SendGuiMessage( &msg, sizeof( GuiMiddleSelectOptionData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StatePrinterSetDefault::
ProcessMessage( int msgId, char * data )
{
	state_t 	    retVal = STATE_PRINTER_SET_DEFAULT;
	int			    idata = *(int *)data;
	CfgError_t	    cfgErr;
    PrinterMsg_t    printerMsg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_TOGGLE:
			switch ( idata )
			{
				case 0xF0:		// Internal Printer
					settingsData->defaultPrinter = INTERNAL_PTR;
                    printerMsg.msgId= MSG_PRINT_INTERNAL_PRINTER_DEFAULT;
                    errNum = SendPrinterMessage (&printerMsg);
					break;
				case 0xF1:		// External Printer
                    printerMsg.msgId= MSG_PRINT_EXTERNAL_PRINTER_DEFAULT;
                    errNum = SendPrinterMessage (&printerMsg);
					settingsData->defaultPrinter = EXTERNAL_PTR;
					break;
			}
			cfgErr = WriteSettings( settingsData );
			if ( cfgErr != CFG_ERR_NONE )
			{
				errNum = SERR_FILE_WRITE;
				retVal = STATE_ERROR;
				stateStack.Push( STATE_PRINTER_SET_DEFAULT );
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
 * class StatePrinterSelectReports
 */
 
void
StatePrinterSelectReports::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_SELECT_REPORTS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	
	wsprintf( data->buttonText[0], L"%s: %d", strings[ MSTR_RESULTS ], settingsData->printNumResults );
	wsprintf( data->buttonText[1], L"%s: %d", strings[ MSTR_IQC ], settingsData->printNumIqc );
	wsprintf( data->buttonText[2], L"%s: %d", strings[ MSTR_ERRORS ], settingsData->printNumError );
	if ( settingsData->errorPrintAuto )
	{
		wcscat( data->buttonText[2], L" " );
		wcscat( data->buttonText[2], strings[ MSTR_AUTO ] );
	}		

	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StatePrinterSelectReports::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_PRINTER_SELECT_REPORTS;
	int		idata = *(int *)data;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Results
					retVal = STATE_PRINTER_SET_RESULTS_COPIES;
					stateStack.Push( STATE_PRINTER_SELECT_REPORTS );
					break;
				case 1:		// iQC
					retVal = STATE_PRINTER_SET_IQC_COPIES;
					stateStack.Push( STATE_PRINTER_SELECT_REPORTS );
					break;
				case 2:		// Errors
					retVal = STATE_PRINTER_SET_ERROR_COPIES;
					stateStack.Push( STATE_PRINTER_SELECT_REPORTS );
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
			infoText = strings[ ISTR_PRINTER_SELECT_REPORTS ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_PRINTER_SELECT_REPORTS );
			break;
		default:
			break;
	}

	return retVal;
}

/*
 * class StatePrinterSetResultsCopies
 */
 
void
StatePrinterSetResultsCopies::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterValueData_t *	data = &msg.data.middleEnterValue;

	setting = settingsData->printNumResults;
	
	SetTopLabel( strings[ TSTR_NUM_RESULTS_COPIES ] );

	msg.msgId = MSG_GUI_MIDDLE_ENTER_VALUE;

	memset( data, 0, sizeof( GuiMiddleEnterValueData_t ) );
	wcscpy( data->currentLabel, strings[ MSTR_CURRENT ] );
	wcscpy( data->newLabel, strings[ MSTR_NEW ] );
	wsprintf( data->currentValue, L"%d", setting );
	wsprintf( data->newValue, L"%d", setting );
	wcscpy( data->button0Text, strings[ MSTR_CLEAR ] );
	wcscpy( data->button1Text, strings[ MSTR_DEFAULT ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
}

state_t
StatePrinterSetResultsCopies::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_PRINTER_SET_RESULTS_COPIES;
	int			idata = *(int *)data;
	bool		doUpdate = false;
	CfgError_t	cfgErr;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Clear
					setting = 0;
					doUpdate = true;
					break;
				case 1:		// Default
					setting = defaultSettings.printNumResults;
					doUpdate = true;
					break;
			}
			break;
		case MSG_GUI_OUT_UP:
			if ( setting < MAX_COPIES )
			{
				setting++;
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_DOWN:
			if ( setting > 0 )
			{
				setting--;
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
				case 2:		// Save
					settingsData->printNumResults = setting;
					cfgErr = WriteSettings( settingsData );
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

	if ( doUpdate == true )
	{
		GuiReceiveMsg_t	msg;
		
		msg.msgId = MSG_GUI_UPDATE_ENTER_VALUE_NEW;
		wsprintf( msg.data.middleEnterValue.newValue, L"%d", setting );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );
	}

	return retVal;
}

/*
 * class StatePrinterSetIqcCopies
 */
 
void
StatePrinterSetIqcCopies::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterValueData_t *	data = &msg.data.middleEnterValue;

	setting = settingsData->printNumIqc;
	
	SetTopLabel( strings[ TSTR_NUM_IQC_COPIES ] );

	msg.msgId = MSG_GUI_MIDDLE_ENTER_VALUE;

	memset( data, 0, sizeof( GuiMiddleEnterValueData_t ) );
	wcscpy( data->currentLabel, strings[ MSTR_CURRENT ] );
	wcscpy( data->newLabel, strings[ MSTR_NEW ] );
	wsprintf( data->currentValue, L"%d", setting );
	wsprintf( data->newValue, L"%d", setting );
	wcscpy( data->button0Text, strings[ MSTR_CLEAR ] );
	wcscpy( data->button1Text, strings[ MSTR_DEFAULT ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
}

state_t
StatePrinterSetIqcCopies::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_PRINTER_SET_IQC_COPIES;
	int			idata = *(int *)data;
	bool		doUpdate = false;
	CfgError_t	cfgErr;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Clear
					setting = 0;
					doUpdate = true;
					break;
				case 1:		// Default
					setting = defaultSettings.printNumIqc;
					doUpdate = true;
					break;
			}
			break;
		case MSG_GUI_OUT_UP:
			if ( setting < MAX_COPIES )
			{
				setting++;
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_DOWN:
			if ( setting > 0 )
			{
				setting--;
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
				case 2:		// Save
					settingsData->printNumIqc = setting;
					cfgErr = WriteSettings( settingsData );
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

	if ( doUpdate == true )
	{
		GuiReceiveMsg_t	msg;
		
		msg.msgId = MSG_GUI_UPDATE_ENTER_VALUE_NEW;
		wsprintf( msg.data.middleEnterValue.newValue, L"%d", setting );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );
	}

	return retVal;
}

/*
 * class StatePrinterSetErrorCopies
 */
 
void
StatePrinterSetErrorCopies::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterValueData_t *	data = &msg.data.middleEnterValue;

	setting = settingsData->printNumError;
	autoPrint = settingsData->errorPrintAuto;
	
	SetTopLabel( strings[ TSTR_NUM_ERROR_COPIES ] );

	msg.msgId = MSG_GUI_MIDDLE_ENTER_VALUE;

	memset( data, 0, sizeof( GuiMiddleEnterValueData_t ) );
	wcscpy( data->currentLabel, strings[ MSTR_CURRENT ] );
	wcscpy( data->newLabel, strings[ MSTR_NEW ] );
	wsprintf( data->currentValue, L"%d", setting );
	wsprintf( data->newValue, L"%d", setting );
	wcscpy( data->button0Text, strings[ MSTR_CLEAR ] );
	wcscpy( data->button1Text, strings[ MSTR_DEFAULT ] );
	wcscpy( data->checkButtonText[0], strings[ MSTR_AUTO ] );
	wcscpy( data->checkButtonText[1], strings[ MSTR_ALWAYS ] );

	data->buttonChecked[ autoPrint ? 0 : 1 ] = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
}

state_t
StatePrinterSetErrorCopies::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_PRINTER_SET_ERROR_COPIES;
	int			idata = *(int *)data;
	bool		doUpdate = false;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Clear
					setting = 0;
					doUpdate = true;
					break;
				case 1:		// Default
					setting = defaultSettings.printNumError;
					doUpdate = true;
					break;
			}
			break;
		case MSG_GUI_OUT_TOGGLE:
			switch ( idata )
			{
				case 0xF2:		// Auto
					autoPrint = true;
					break;
				case 0xF3:		// Always
					autoPrint = false;
					break;
			}
			break;
		case MSG_GUI_OUT_UP:
			if ( setting < MAX_COPIES )
			{
				setting++;
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_DOWN:
			if ( setting > 0 )
			{
				setting--;
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
				case 2:		// Save
					settingsData->printNumError = setting;
					settingsData->errorPrintAuto = autoPrint;
					cfgErr = WriteSettings( settingsData );
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

	if ( doUpdate == true )
	{
		GuiReceiveMsg_t	msg;
		
		msg.msgId = MSG_GUI_UPDATE_ENTER_VALUE_NEW;
		
		memset( &msg.data.middleEnterValue, 0, sizeof( GuiMiddleEnterValueData_t ) );
		wsprintf( msg.data.middleEnterValue.newValue, L"%d", setting );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );
	}

	return retVal;
}
