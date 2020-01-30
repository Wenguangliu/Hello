/* Import (extern) header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

#include "abdefine.h"

extern ApEventLink_t AbInternalLinks[];

extern ApWindowLink_t winLcd;
extern ApWindowLink_t winEnterId;
extern ApWindowLink_t winTopTitle;
extern ApWindowLink_t winViewResults;
extern ApWindowLink_t winBot1Button;
extern ApWindowLink_t winBot2Button;
extern ApWindowLink_t winBot3Button;
extern ApWindowLink_t winBotDateTime;
extern ApWindowLink_t winEnterRange;
extern ApWindowLink_t winEnterDate;
extern ApWindowLink_t winIconMatrix1;
extern ApWindowLink_t winButtons;
extern ApWindowLink_t winIconMatrix2;
extern ApWindowLink_t winHomeWarming;
extern ApWindowLink_t winHomeReady;
extern ApWindowLink_t winProgBarMessage;
extern ApWindowLink_t winButtonsInfo;
extern ApWindowLink_t winEnterTime;
extern ApWindowLink_t winEnterValue;
extern ApWindowLink_t winList;
extern ApWindowLink_t winSelectOption;
extern ApWindowLink_t winSelectType;
extern ApWindowLink_t winError;
extern ApWindowLink_t winCriticalStop;
extern ApWindowLink_t winWarning1;
extern ApWindowLink_t winMfgHomeReady;
extern ApWindowLink_t winMfgHomeWarming;
extern ApWindowLink_t winOptionInfo;
extern ApWindowLink_t winMessage;
extern ApWindowLink_t winMessageInfo;
extern ApWindowLink_t winWarning2;
extern ApWindowLink_t winMultiSelect;
extern ApWindowLink_t winScreenSaver;
extern ApWindowLink_t winMiddleBackground;
extern ApWindowLink_t winGraphicInfo1;
extern ApWindowLink_t winGraphicInfo2;
extern ApWindowLink_t winGraphicInfo3;
extern ApWindowLink_t winAnalysisInProcess;
extern ApWindowLink_t winListMulti;
extern ApWindowLink_t winEnterIp;
extern ApWindowLink_t winMultiInfo;
extern ApWindowLink_t winListColored;
extern ApWindowLink_t winLoadOrder;
extern ApWindowLink_t winCriticalStop2;
extern ApWidget_t AbWidgets[ 296 ];


#if defined(__cplusplus)
}
#endif


#ifdef __cplusplus
int Init( int argc, char **argv );
int Setup( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int Touch( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#endif
