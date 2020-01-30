/*
 * FILENAME
 * 	MiddleSection.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the middle subscreens.  Includes the parent class for
 * 	all the middle subscreens, MiddleSection, the parent class for the list
 * 	subscreens, MiddleList, and all the actual middle subscreen classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __MIDDLESECTION_H_INCLUDED
#define __MIDDLESECTION_H_INCLUDED

#include "ablibs.h"
#include "GuiMessages.h"
#include "GuiSection.h"
#include "FactoryData.h"

/*
 * MiddleSection
 */
 
class MiddleSection : public GuiSection
{
public:
	virtual void	Show();
	virtual void	Hide();
	virtual void	SetData( GuiReceiveMsgData_t * data ) {};
	virtual void	Update( GuiReceiveMsg_t * msg ) {};
	static void		GetBackground();

protected:
	static PhImage_t *	bkgdImage;
	PtWidget_t * 		window;
};

/*
 * MiddleListParent
 */
 
class MiddleListParent : public MiddleSection
{
public:
	MiddleListParent();
	virtual void Show();
	virtual void Hide();

protected:
	virtual void SetColumns( short column0, short column1, short column2 );
	
	static PtWidget_t * listWindow;
	static PtWidget_t * list;
};

/*
 * MiddleAnalysisInProcess
 */
 
class MiddleAnalysisInProcess : public MiddleSection
{
public:
	MiddleAnalysisInProcess();
	void SetData( GuiReceiveMsgData_t * data );
	void Update( GuiReceiveMsg_t * msg );
	
private:
	PtWidget_t * label;
	PtWidget_t * text;
	PtWidget_t * progBar;
};

/*
 * MiddleButtons
 */
 
class MiddleButtons : public MiddleSection
{
public:
	MiddleButtons();
	void SetData( GuiReceiveMsgData_t * data );
	
private:
	PtWidget_t * buttons[6];
};

/*
 * MiddleButtonsInfo
 */
 
class MiddleButtonsInfo : public MiddleSection
{
public:
	MiddleButtonsInfo();
	void SetData( GuiReceiveMsgData_t * data );
	
private:
	PtWidget_t * buttons[4];
};

/*
 * MiddleEnterDate
 * */
 
class MiddleEnterDate : public MiddleSection
{
public:
	MiddleEnterDate();
	void Show();
	void SetData( GuiReceiveMsgData_t * data );
	void Update( GuiReceiveMsg_t * msg );

private:
	PtWidget_t * labelLeft;
	PtWidget_t * labelMiddle;
	PtWidget_t * labelRight;
	PtWidget_t * textLeft;
	PtWidget_t * textMiddle;
	PtWidget_t * textRight;
	PtWidget_t * button;
};

/*
 * MiddleEnterId
 */
 
class MiddleEnterId : public MiddleSection
{
public:
	MiddleEnterId();
	void Show();
	void SetData( GuiReceiveMsgData_t * data );
	void SendIdText();
	void Update( GuiReceiveMsg_t * msg );
};

/*
 * MiddleEnterIp
 */
 
class MiddleEnterIp : public MiddleSection
{
public:
	MiddleEnterIp();
	void Show();
	void SetData( GuiReceiveMsgData_t * data );
	void Update( GuiReceiveMsg_t * msg );
};

/*
 * MiddleEnterRange
 */
 
class MiddleEnterRange : public MiddleSection
{
public:
	MiddleEnterRange();
	void Show();
	void SetData( GuiReceiveMsgData_t * data );
	void SendRange();
};

/*
 * MiddleEnterTime
 */
 
class MiddleEnterTime : public MiddleSection
{
public:
	MiddleEnterTime();
	void Show();
	void Hide();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * hoursLabel;
	PtWidget_t * minutesLabel;
	PtWidget_t * formatButton;
	PtWidget_t * zeroSecButton;
};

/*
 * MiddleEnterValue
 */
 
class MiddleEnterValue : public MiddleSection
{
public:
	MiddleEnterValue();
	void Show();
	void SetData( GuiReceiveMsgData_t * data );
	void Update( GuiReceiveMsg_t * msg );

private:
	PtWidget_t * currentLabel;
	PtWidget_t * newLabel;
	PtWidget_t * currentValue;
	PtWidget_t * newValue;
	PtWidget_t * button0;
	PtWidget_t * button1;
	PtWidget_t * checkButtons[2];
};

/*
 * MiddleGraphicInfo1
 */
 
class MiddleGraphicInfo1 : public MiddleSection
{
public:
	MiddleGraphicInfo1();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * text;
};

/*
 * MiddleGraphicInfo2
 */
 
class MiddleGraphicInfo2 : public MiddleSection
{
public:
	MiddleGraphicInfo2();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * text;
};

/*
 * MiddleGraphicInfo3
 */
 
class MiddleGraphicInfo3 : public MiddleSection
{
public:
	MiddleGraphicInfo3();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * text;
};

/*
 * MiddleHomeReady
 */

class MiddleHomeReady : public MiddleSection
{
public:
	MiddleHomeReady();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * buttons[2];
};

/*
 * MiddleHomeReadyMfg
 */

class MiddleHomeReadyMfg : public MiddleSection
{
public:
	MiddleHomeReadyMfg();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * buttons[2];
};

/*
 * MiddleHomeWarming
 */
 
class MiddleHomeWarming : public MiddleSection
{
public:
	MiddleHomeWarming();
	void SetData( GuiReceiveMsgData_t * data );
	void Update( GuiReceiveMsg_t * msg );

private:
	PtWidget_t * label;
	PtWidget_t * progBar;
};

/*
 * MiddleHomeWarmingMfg
 */
 
class MiddleHomeWarmingMfg : public MiddleSection
{
public:
	MiddleHomeWarmingMfg();
	void SetData( GuiReceiveMsgData_t * data );
	void Update( GuiReceiveMsg_t * msg );

private:
	PtWidget_t * label;
	PtWidget_t * progBar;
};

/*
 * MiddleIconMatrix1
 */
 
class MiddleIconMatrix1 : public MiddleSection
{
public:
	MiddleIconMatrix1();
	void SetData( GuiReceiveMsgData_t * data ) {};
};

/*
 * MiddleIconMatrix2
 */
 
class MiddleIconMatrix2 : public MiddleSection
{
public:
	MiddleIconMatrix2();
	void SetData( GuiReceiveMsgData_t * data ) {};

private:
	static FactoryData_t *	factoryData;
};

/*
 * MiddleList
 */
class MiddleList : public MiddleListParent
{
public:
	void Show();
	void SetData( GuiReceiveMsgData_t * data );
	void Update( GuiReceiveMsg_t * msg );
};

/*
 * MiddleListColored
 */
 
class MiddleListColored : public MiddleSection
{
public:
	MiddleListColored();
	void SetData( GuiReceiveMsgData_t * data );
	
private:
	PtWidget_t * buttons[5];
};

/*
 * MiddleListMulti
 */

class MiddleListMulti : public MiddleSection
{
public:
	MiddleListMulti();
	void SetData( GuiReceiveMsgData_t * data );
	void Update( GuiReceiveMsg_t * msg );

private:
	static PtWidget_t * list;
};

/*
 * MiddleListRefRanges
 */
 
class MiddleListRefRanges : public MiddleListParent
{
public:
	void Show();
	void SetData( GuiReceiveMsgData_t * data );
	void Update( GuiReceiveMsg_t * msg );
};

/*
 * MiddleListResults
 */
 
class MiddleListResults : public MiddleListParent
{
public:
	void Show();
	void SetData( GuiReceiveMsgData_t * data );
	void Update( GuiReceiveMsg_t * msg );
};

/*
 * MiddleListSetttings
 */
 
class MiddleListSettings : public MiddleListParent
{
public:
	void Show();
	void SetData( GuiReceiveMsgData_t * data );
	void Update( GuiReceiveMsg_t * msg );
};

/*
 * MiddleLoadOrder
 */

class MiddleLoadOrder : public MiddleSection
{
public:
	MiddleLoadOrder();
	void SetData( GuiReceiveMsgData_t * data );
	
private:
	PtWidget_t * orderLabel;
	PtWidget_t * rotorName;
	PtWidget_t * button;
};

/*
 * MiddleMessage
 */
 
class MiddleMessage : public MiddleSection
{
public:
	MiddleMessage();
	void SetData( GuiReceiveMsgData_t * data );
	
private:
	PtWidget_t * text;
};	

/*
 * MiddleMessageInfo
 */
 
class MiddleMessageInfo : public MiddleSection
{
public:
	MiddleMessageInfo();
	void SetData( GuiReceiveMsgData_t * data );
	
private:
	PtWidget_t * text;
};

/*
 * MiddleMultiInfo
 */

class MiddleMultiInfo : public MiddleSection
{
public:
	MiddleMultiInfo();
	void SetData( GuiReceiveMsgData_t * data );
	
private:
	PtWidget_t * buttons[4];
};
	
/*
 * MiddleMultiSelect
 */
 
class MiddleMultiSelect : public MiddleSection
{
public:
	MiddleMultiSelect();
	void SetData( GuiReceiveMsgData_t * data );
	
private:
	PtWidget_t * buttons[6];
};

/*
 * MiddleOptionInfo
 */

class MiddleOptionInfo : public MiddleSection
{
public:
	MiddleOptionInfo();
	void SetData( GuiReceiveMsgData_t * data );
	
private:
	PtWidget_t * buttons[4];
};

/*
 * MiddleProgBarMessage
 */
 
class MiddleProgBarMessage : public MiddleSection
{
public:
	MiddleProgBarMessage();
	void SetData( GuiReceiveMsgData_t * data );
	void Update( GuiReceiveMsg_t * msg );
	
private:
	PtWidget_t * label;
	PtWidget_t * text;
	PtWidget_t * progBar;
};

/*
 * MiddleSelectOption
 */
class MiddleSelectOption : public MiddleSection
{
public:
	MiddleSelectOption();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * buttons[6];
};

/*
 * MiddleSelectType
 */
 
class MiddleSelectType : public MiddleSection
{
public:
	MiddleSelectType();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * buttons[6];
};

/*
 * MiddleViewResults
 */
 
class MiddleViewResults : public MiddleSection
{
public:
	MiddleViewResults();
	void SetData( GuiReceiveMsgData_t * data );

private:
	PtWidget_t * text;
};

#endif
