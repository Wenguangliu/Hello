/* Link header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

extern ApContext_t AbContext;

ApWindowLink_t winCriticalStop = {
	"winCriticalStop.wgtw",
	&AbContext,
	AbLinks_winCriticalStop, 0, 1
	};

ApWindowLink_t winMessage = {
	"winMessage.wgtw",
	&AbContext,
	NULL, 6, 0
	};

ApWindowLink_t winBlack = {
	"winBlack.wgtw",
	&AbContext,
	NULL, 9, 0
	};

ApWindowLink_t winLcd = {
	"winLcd.wgtw",
	&AbContext,
	NULL, 10, 0
	};

ApWindowLink_t winErrorInfo = {
	"winErrorInfo.wgtw",
	&AbContext,
	AbLinks_winErrorInfo, 15, 1
	};

ApWindowLink_t winViewResults = {
	"winViewResults.wgtw",
	&AbContext,
	NULL, 22, 0
	};


#if defined(__cplusplus)
}
#endif

