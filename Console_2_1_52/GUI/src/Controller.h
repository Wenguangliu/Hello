/*
 * FILENAME
 * 	Controller.h
 * 
 * MODULE DESCRIPTION
 * 	Class definition for the Controller class, which handles receiving messages
 * 	from other processes and passing them on to the correct objects.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __CONTROLLER_H_INCLUDED
#define __CONTROLLER_H_INCLUDED

#include "ablibs.h"
#include "TopSection.h"
#include "MiddleSection.h"
#include "BottomSection.h"
#include "WinSection.h"
#include "SettingsData.h"

class Controller
{
public:
	Controller();
	void			RestoreCurrentWindow();
	
private:
	static void *	MsgReceiveThread( Controller * controller );
	void 			SetTop( int msgId, GuiReceiveMsgData_t * msgData );
	void			SetMiddle( int msgId, GuiReceiveMsgData_t * msgData );
	void			SetBottom( int msgId, GuiReceiveMsgData_t * msgData );
	void			SetWin( int msgId, GuiReceiveMsgData_t * msgData );
	void			ScreenSaver( int msgId );
	void			Sound( int msgId, GuiReceiveMsgData_t * msgData );
	void			Update( GuiReceiveMsg_t * msg );

	TopSection *				topSection;
	MiddleSection * 			middleSection;
	BottomSection *				bottomSection;
	WinSection *				winSection;
	
	TopTitle *					topTitle;
	
	MiddleAnalysisInProcess *	middleAnalysisInProcess;
	MiddleButtons *				middleButtons;
	MiddleButtonsInfo *			middleButtonsInfo;
	MiddleEnterDate *			middleEnterDate;
	MiddleEnterId *				middleEnterId;
	MiddleEnterIp *				middleEnterIp;
	MiddleEnterRange *			middleEnterRange;
	MiddleEnterTime *			middleEnterTime;
	MiddleEnterValue *			middleEnterValue;
	MiddleGraphicInfo1 *		middleGraphicInfo1;
	MiddleGraphicInfo2 *		middleGraphicInfo2;
	MiddleGraphicInfo3 *		middleGraphicInfo3;
	MiddleHomeReady *			middleHomeReady;
	MiddleHomeReadyMfg *		middleHomeReadyMfg;
	MiddleHomeWarming *			middleHomeWarming;
	MiddleHomeWarmingMfg *		middleHomeWarmingMfg;
	MiddleIconMatrix1 *			middleIconMatrix1;
	MiddleIconMatrix2 *			middleIconMatrix2;
	MiddleList *				middleList;
	MiddleListColored *			middleListColored;
	MiddleListMulti *			middleListMulti;
	MiddleListRefRanges *		middleListRefRanges;
	MiddleListResults *			middleListResults;
	MiddleListSettings *		middleListSettings;
	MiddleLoadOrder *			middleLoadOrder;
	MiddleMessage *				middleMessage;
	MiddleMessageInfo *			middleMessageInfo;
	MiddleMultiInfo *			middleMultiInfo;
	MiddleMultiSelect *			middleMultiSelect;
	MiddleOptionInfo *			middleOptionInfo;
	MiddleProgBarMessage *		middleProgBarMessage;
	MiddleSelectOption *		middleSelectOption;
	MiddleSelectType *			middleSelectType;
	MiddleViewResults *			middleViewResults;
	
	BottomButtons1 *			bottomButtons1;
	BottomButtons2 *			bottomButtons2;
	BottomButtons3 *			bottomButtons3;
	BottomDateTime *			bottomDateTime;
	
	WinWarning1 *				winWarning1;
	WinWarning2 *				winWarning2;
	WinError *					winError;
	WinCriticalStop *			winCriticalStop;
	WinCriticalStop2 *			winCriticalStop2;
	
	static SettingsData_t *		settingsData;
};

#endif
