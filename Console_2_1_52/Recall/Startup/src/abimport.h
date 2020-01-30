/* Import (extern) header for application - AppBuilder 2.03  */

#if defined(__cplusplus)
extern "C" {
#endif

#include "abdefine.h"

extern ApEventLink_t AbInternalLinks[];

extern ApWindowLink_t winCriticalStop;
extern ApWindowLink_t winMessage;
extern ApWindowLink_t winBlack;
extern ApWindowLink_t winLcd;
extern ApWindowLink_t winErrorInfo;
extern ApWindowLink_t winViewResults;
extern ApWidget_t AbWidgets[ 27 ];


#if defined(__cplusplus)
}
#endif


#ifdef __cplusplus
int Init( int argc, char **argv );
int Setup( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int ErrorInfo( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#endif
