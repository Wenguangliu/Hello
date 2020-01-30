#ifndef _PING_TEST_H
#define _PING_TEST_H

#define MAX_IP_STRING_LENGTH       32
#define PING_TIMES                 5
typedef enum
{
    PING_SUCCESS            = 0,
    PAIG_FAIL_TO_OPEN_FILE  = 1,
    PING_INVALID_IP_ADDRESS = 2,
    PING_FAIL_OPEN_HANDLE   = 3,
    PING_TIME_OUT           = 4,
    PING_MSG_ERROR          = 5,
    PING_SEND_ECHO_FAIL     = 6,
} pingStatus_t;

typedef struct
{
    char            analyzerIp[30];
    char            gateway[30];
    char            ipMask[30];
} netSettings_t;

#pragma pack (push,1)
typedef struct
{
    unsigned char   pingPass;
    unsigned short  pingTime;
    unsigned short  pingStatus;

}pingReport_t;

typedef struct {
    unsigned long   ipAddress;
    pingReport_t    pingReport [PING_TIMES];
} netPortTestMsg_t;
#pragma pack (pop)


extern int
Ping (unsigned char pingTimes, netPortTestMsg_t *netPortTestMsgPrt);

#endif
