#ifndef _VIEWLOG_H
#define _VIEWLOG_H
#include "GuiMessages.h"
#include "StateAnalysis.h"
 
#define VIEW_LOG_FILE_NAME       "/Hard Disk/Abaxis/log.current"

#define PAGE_SIZE           2048

class StateViewLog : public StateAnalysis 
{
public:
    void    Enter();
    state_t ProcessMessage(int msgId, char * data);
private: 
    static void             SendLogMessage (void);
    static int              pageNumber;
    static int              totalPages;
    static HANDLE           logSemHandle;
    static char             logMsg [PAGE_SIZE];
    static int              totallReadLen;
    static int              fileSize;
    static int              lastPgLen;
};
#endif
