#ifndef AMSCOMM_H_
#define AMSCOMM_H_

typedef enum 
{
    AMS_TX_IDLE,
    AMS_TX_TEMP,
    AMS_DUMP_TEMP_RESILT,
    AMS_UPLOAD_LOG_FILE,
    AMS_UPLOAD_RAD_FILE,
} AmsTxCommad_t; 

typedef struct 
{
    int             msgId;
    AmsTxCommad_t   txCommand;
    unsigned char   data [512];
} AmsTxMsg_t;

typedef struct
{
    unsigned short  year;
    unsigned char   month;
    unsigned char   day;
    unsigned char   hour;
    unsigned char   minute;
} timeFlag_t;

extern char sysName [11];
extern char rxChar;

#endif

