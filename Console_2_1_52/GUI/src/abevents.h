/* Event header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &winLcd, NULL, NULL, 0, Setup, 1, 5, -400, 0, },
	{ 0 }
	};

ApEventLink_t AbInternalLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &winListColored, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winBot2Button, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winBot3Button, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winBotDateTime, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winButtons, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winEnterDate, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winEnterRange, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winIconMatrix1, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winIconMatrix2, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winList, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winTopTitle, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winViewResults, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winHomeReady, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winHomeWarming, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winProgBarMessage, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winMessage, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winMessageInfo, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winButtonsInfo, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winSelectType, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winEnterId, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winEnterTime, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winEnterValue, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winSelectOption, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winBot1Button, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winCriticalStop, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winWarning1, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winMfgHomeReady, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winMfgHomeWarming, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winOptionInfo, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winWarning2, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winMultiSelect, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winScreenSaver, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winMiddleBackground, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winGraphicInfo1, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winGraphicInfo2, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winGraphicInfo3, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winAnalysisInProcess, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winListMulti, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winEnterIp, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winMultiInfo, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winError, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winLoadOrder, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winCriticalStop2, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winEnterId[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptTextEnterId", 4009, EnterId::InputVerify, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptTextEnterId", 2009, Comm::Activate, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterId1", 2009, EnterId::KeyPress, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterId2", 2009, EnterId::KeyPress, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterId3", 2009, EnterId::KeyPress, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterId4", 2009, EnterId::KeyPress, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterId5", 2009, EnterId::KeyPress, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterId6", 2009, EnterId::KeyPress, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterId7", 2009, EnterId::KeyPress, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIdBack", 2009, EnterId::KeyPress, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterId8", 2009, EnterId::KeyPress, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterId0", 2009, EnterId::KeyPress, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterId9", 2009, EnterId::KeyPress, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIdUnderscore", 2009, EnterId::KeyPress, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winViewResults[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonViewResultsDown", 2009, ViewResults::ScrollDown, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonViewResultsDown", 2012, ViewResults::ScrollDown, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonViewResultsUp", 2009, ViewResults::ScrollUp, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonViewResultsUp", 2012, ViewResults::ScrollUp, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winBot1Button[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonBottomSingle", 2009, Comm::BottomButton, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winBot2Button[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonBottomDoubleLeft", 2009, Comm::BottomButton, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonBottomDoubleRight", 2009, Comm::BottomButton, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winBot3Button[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonBottomTripleLeft", 2009, Comm::BottomButton, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonBottomTripleCenter", 2009, Comm::BottomButton, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonBottomTripleRight", 2009, Comm::BottomButton, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winBotDateTime[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptTimerBottomDateTime", 41002, UpdateDateTime::Update, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winEnterRange[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterRangeLowerUp", 2009, EnterRange::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterRangeLowerUp", 2012, EnterRange::RepeatUp, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterRangeLowerDown", 2009, EnterRange::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterRangeLowerDown", 2012, EnterRange::RepeatDown, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterRange0", 2009, EnterRange::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterRange1", 2009, EnterRange::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterRangeUpperDown", 2009, EnterRange::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterRangeUpperDown", 2012, EnterRange::RepeatDown, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterRangeUpperUp", 2009, EnterRange::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterRangeUpperUp", 2012, EnterRange::RepeatUp, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winEnterDate[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterDateUp0", 2009, Comm::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterDateUp0", 2012, Comm::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterDateDown0", 2009, Comm::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterDateDown0", 2012, Comm::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterDateDown1", 2009, Comm::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterDateDown1", 2012, Comm::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterDateUp1", 2009, Comm::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterDateUp1", 2012, Comm::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterDateUp2", 2009, Comm::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterDateUp2", 2012, Comm::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterDateDown2", 2009, Comm::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterDateDown2", 2012, Comm::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterDate", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winIconMatrix1[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixSounds", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixDateTime", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixNext", 2009, Comm::NextSettings, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixAnalyzerInfo", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixDisplay", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixPrinter", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixLanguage", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winButtons[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonButtons0", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonButtons1", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonButtons3", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonButtons2", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonButtons4", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonButtons5", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winIconMatrix2[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixPrevious", 2009, Comm::PreviousSettings, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixCommunication", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixFactoryDefaults", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixCdUpdate", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixUnits", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixReferenceRanges", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixArchiveReferenceRanges", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixAdvanced", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonIconMatrixSecurity", 2009, Comm::IconMatrix, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winHomeWarming[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonHomeWarmingRecallResults", 2009, Comm::Recall, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonHomeWarmingSettings", 2009, Comm::Settings, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winHomeReady[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonHomeReadyRecallResults", 2009, Comm::Recall, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonHomeReadySettings", 2009, Comm::Settings, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonHomeReadyButton0", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonHomeReadyButton1", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonHomeReadyButton2", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winButtonsInfo[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonButtonsInfo0", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonButtonsInfo1", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonButtonsInfo3", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonButtonsInfo2", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonButtonsInfoInfo", 2009, Comm::Info, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winEnterTime[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterTimeHourUp", 2009, EnterTime::HourUp, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterTimeHourDown", 2009, EnterTime::HourDown, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterTimeMinuteUp", 2009, EnterTime::MinuteUp, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterTimeMinuteDown", 2009, EnterTime::MinuteDown, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterTimeFormat", 2009, EnterTime::Format, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterTimeFormat", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterTimeZeroSec", 2009, EnterTime::ZeroSec, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptTimerEnterTime", 41002, EnterTime::Update, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winEnterValue[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterValue0", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterValue1", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterValueDown", 2009, Comm::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterValueDown", 2012, Comm::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterValueUp", 2009, Comm::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterValueUp", 2012, Comm::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterValue2", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterValue3", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winList[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptListList", 23010, Comm::List, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonListUp", 2009, Comm::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonListUp", 2012, Comm::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonListDown", 2009, Comm::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonListDown", 2012, Comm::Down, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winSelectOption[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonSelectOption0", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonSelectOption1", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonSelectOption2", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonSelectOption3", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonSelectOption4", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonSelectOption5", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winSelectType[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonSelectType0", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonSelectType1", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonSelectType3", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonSelectType2", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonSelectType4", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonSelectType5", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winError[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonErrorInfo", 2009, Comm::Info, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonErrorContinue", 2009, Comm::BottomButton, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winCriticalStop[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonCriticalStopInfo", 2009, Comm::Info, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winWarning1[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonWarning1Info", 2009, Comm::Info, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonWarning1Continue", 2009, Comm::BottomButton, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winMfgHomeReady[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMfgHomeReadyRecallResults", 2009, Comm::Recall, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMfgHomeReadySettings", 2009, Comm::Settings, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMfgHomeReadyButton0", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMfgHomeReadyManufacturing", 2009, Comm::Manufacturing, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMfgHomeReadyButton1", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winMfgHomeWarming[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMfgHomeWarmingRecallResults", 2009, Comm::Recall, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMfgHomeWarmingSettings", 2009, Comm::Settings, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMfgHomeWarmingManufacturing", 2009, Comm::Manufacturing, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winOptionInfo[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonOptionInfo0", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonOptionInfo1", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonOptionInfo2", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonOptionInfo3", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonOptionInfoInfo", 2009, Comm::Info, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winMessageInfo[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMessageInfo", 2009, Comm::Info, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winWarning2[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonWarning2Info", 2009, Comm::Info, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonWarning2Continue", 2009, Comm::BottomButton, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonWarning2Cancel", 2009, Comm::BottomButton, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winMultiSelect[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMultiSelect0", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMultiSelect1", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMultiSelect2", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMultiSelect3", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMultiSelect4", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMultiSelect5", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winScreenSaver[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptLabelScreenSaver", 2009, Touch, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winListMulti[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptListListMulti", 23010, Comm::ListMulti, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonListMultiDown", 2012, Comm::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonListMultiDown", 2009, Comm::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonListMultiUp", 2009, Comm::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonListMultiUp", 2012, Comm::Up, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winEnterIp[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIp1", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIp2", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIp3", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIp4", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIp5", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIp6", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIp7", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIpClear", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIp8", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIp0", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIp9", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonEnterIpPeriod", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winMultiInfo[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMultiInfo0", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMultiInfo1", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMultiInfo2", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMultiInfo3", 2009, Comm::Toggle, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonMultiInfo", 2009, Comm::Info, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winListColored[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "PtButtonListColoredDown", 2009, Comm::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "PtButtonListColoredDown", 2012, Comm::Down, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "PtButtonListColoredUp", 2009, Comm::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "PtButtonListColoredUp", 2012, Comm::Up, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "PtButtonListColored0", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "PtButtonListColored1", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "PtButtonListColored2", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "PtButtonListColored3", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "PtButtonListColored4", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winLoadOrder[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonLoadOrder", 2009, Comm::Button, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winCriticalStop2[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonCriticalStop2Info", 2009, Comm::Info, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonCriticalStop2Restart", 2009, Comm::BottomButton, 0, 0, 0, 0, },
	{ 0 }
	};


#if defined(__cplusplus)
}
#endif

