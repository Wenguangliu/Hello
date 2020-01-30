/* Event header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &winLcd, NULL, NULL, 0, Setup, 1, 5, 240, 0, },
	{ 0 }
	};

ApEventLink_t AbInternalLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &winCriticalStop, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winMessage, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 3, 0, 0L, 0L, 0L, &winLcd, NULL, NULL, 0, NULL, 0, 5, 240, 0, },
	{ 3, 0, 0L, 0L, 0L, &winBlack, NULL, NULL, 0, NULL, 0, 5, -400, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winCriticalStop[] = {
	{ 3, 0, 0L, 0L, 0L, &winErrorInfo, NULL, "ptButtonCriticalStopInfo", 2009, ErrorInfo, 0, 5, 240, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_winErrorInfo[] = {
	{ 9, 0, 0L, 0L, 0L, NULL, NULL, "ptButtonErrorInfo", 2009, NULL, 0, 0, 0, 0, },
	{ 0 }
	};


#if defined(__cplusplus)
}
#endif

