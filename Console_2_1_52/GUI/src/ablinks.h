/* Link header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

extern ApContext_t AbContext;

ApWindowLink_t winLcd = {
	"winLcd.wgtw",
	&AbContext,
	NULL, 0, 0
	};

ApWindowLink_t winEnterId = {
	"winEnterId.wgtw",
	&AbContext,
	AbLinks_winEnterId, 1, 14
	};

ApWindowLink_t winTopTitle = {
	"winTopTitle.wgtw",
	&AbContext,
	NULL, 17, 0
	};

ApWindowLink_t winViewResults = {
	"winViewResults.wgtw",
	&AbContext,
	AbLinks_winViewResults, 19, 4
	};

ApWindowLink_t winBot1Button = {
	"winBot1Button.wgtw",
	&AbContext,
	AbLinks_winBot1Button, 24, 1
	};

ApWindowLink_t winBot2Button = {
	"winBot2Button.wgtw",
	&AbContext,
	AbLinks_winBot2Button, 26, 2
	};

ApWindowLink_t winBot3Button = {
	"winBot3Button.wgtw",
	&AbContext,
	AbLinks_winBot3Button, 29, 3
	};

ApWindowLink_t winBotDateTime = {
	"winBotDateTime.wgtw",
	&AbContext,
	AbLinks_winBotDateTime, 33, 1
	};

ApWindowLink_t winEnterRange = {
	"winEnterRange.wgtw",
	&AbContext,
	AbLinks_winEnterRange, 37, 10
	};

ApWindowLink_t winEnterDate = {
	"winEnterDate.wgtw",
	&AbContext,
	AbLinks_winEnterDate, 49, 13
	};

ApWindowLink_t winIconMatrix1 = {
	"winIconMatrix1.wgtw",
	&AbContext,
	AbLinks_winIconMatrix1, 64, 7
	};

ApWindowLink_t winButtons = {
	"winButtons.wgtw",
	&AbContext,
	AbLinks_winButtons, 73, 6
	};

ApWindowLink_t winIconMatrix2 = {
	"winIconMatrix2.wgtw",
	&AbContext,
	AbLinks_winIconMatrix2, 81, 9
	};

ApWindowLink_t winHomeWarming = {
	"winHomeWarming.wgtw",
	&AbContext,
	AbLinks_winHomeWarming, 92, 2
	};

ApWindowLink_t winHomeReady = {
	"winHomeReady.wgtw",
	&AbContext,
	AbLinks_winHomeReady, 98, 5
	};

ApWindowLink_t winProgBarMessage = {
	"winProgBarMessage.wgtw",
	&AbContext,
	NULL, 105, 0
	};

ApWindowLink_t winButtonsInfo = {
	"winButtonsInfo.wgtw",
	&AbContext,
	AbLinks_winButtonsInfo, 110, 5
	};

ApWindowLink_t winEnterTime = {
	"winEnterTime.wgtw",
	&AbContext,
	AbLinks_winEnterTime, 117, 8
	};

ApWindowLink_t winEnterValue = {
	"winEnterValue.wgtw",
	&AbContext,
	AbLinks_winEnterValue, 129, 8
	};

ApWindowLink_t winList = {
	"winList.wgtw",
	&AbContext,
	AbLinks_winList, 141, 5
	};

ApWindowLink_t winSelectOption = {
	"winSelectOption.wgtw",
	&AbContext,
	AbLinks_winSelectOption, 150, 6
	};

ApWindowLink_t winSelectType = {
	"winSelectType.wgtw",
	&AbContext,
	AbLinks_winSelectType, 158, 6
	};

ApWindowLink_t winError = {
	"winError.wgtw",
	&AbContext,
	AbLinks_winError, 166, 2
	};

ApWindowLink_t winCriticalStop = {
	"winCriticalStop.wgtw",
	&AbContext,
	AbLinks_winCriticalStop, 172, 1
	};

ApWindowLink_t winWarning1 = {
	"winWarning1.wgtw",
	&AbContext,
	AbLinks_winWarning1, 177, 2
	};

ApWindowLink_t winMfgHomeReady = {
	"winMfgHomeReady.wgtw",
	&AbContext,
	AbLinks_winMfgHomeReady, 183, 5
	};

ApWindowLink_t winMfgHomeWarming = {
	"winMfgHomeWarming.wgtw",
	&AbContext,
	AbLinks_winMfgHomeWarming, 190, 3
	};

ApWindowLink_t winOptionInfo = {
	"winOptionInfo.wgtw",
	&AbContext,
	AbLinks_winOptionInfo, 197, 5
	};

ApWindowLink_t winMessage = {
	"winMessage.wgtw",
	&AbContext,
	NULL, 204, 0
	};

ApWindowLink_t winMessageInfo = {
	"winMessageInfo.wgtw",
	&AbContext,
	AbLinks_winMessageInfo, 207, 1
	};

ApWindowLink_t winWarning2 = {
	"winWarning2.wgtw",
	&AbContext,
	AbLinks_winWarning2, 211, 3
	};

ApWindowLink_t winMultiSelect = {
	"winMultiSelect.wgtw",
	&AbContext,
	AbLinks_winMultiSelect, 218, 6
	};

ApWindowLink_t winScreenSaver = {
	"winScreenSaver.wgtw",
	&AbContext,
	AbLinks_winScreenSaver, 226, 1
	};

ApWindowLink_t winMiddleBackground = {
	"winMiddleBackground.wgtw",
	&AbContext,
	NULL, 228, 0
	};

ApWindowLink_t winGraphicInfo1 = {
	"winGraphicInfo1.wgtw",
	&AbContext,
	NULL, 230, 0
	};

ApWindowLink_t winGraphicInfo2 = {
	"winGraphicInfo2.wgtw",
	&AbContext,
	NULL, 235, 0
	};

ApWindowLink_t winGraphicInfo3 = {
	"winGraphicInfo3.wgtw",
	&AbContext,
	NULL, 239, 0
	};

ApWindowLink_t winAnalysisInProcess = {
	"winAnalysisInProcess.wgtw",
	&AbContext,
	NULL, 243, 0
	};

ApWindowLink_t winListMulti = {
	"winListMulti.wgtw",
	&AbContext,
	AbLinks_winListMulti, 248, 5
	};

ApWindowLink_t winEnterIp = {
	"winEnterIp.wgtw",
	&AbContext,
	AbLinks_winEnterIp, 253, 12
	};

ApWindowLink_t winMultiInfo = {
	"winMultiInfo.wgtw",
	&AbContext,
	AbLinks_winMultiInfo, 269, 5
	};

ApWindowLink_t winListColored = {
	"winListColored.wgtw",
	&AbContext,
	AbLinks_winListColored, 276, 9
	};

ApWindowLink_t winLoadOrder = {
	"winLoadOrder.wgtw",
	&AbContext,
	AbLinks_winLoadOrder, 285, 1
	};

ApWindowLink_t winCriticalStop2 = {
	"winCriticalStop2.wgtw",
	&AbContext,
	AbLinks_winCriticalStop2, 290, 2
	};


#if defined(__cplusplus)
}
#endif

