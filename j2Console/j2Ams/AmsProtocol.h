#ifndef _AMSPROTOCOL_H
#define _AMSPROTOCOL_H

/* ASCII command characters */
#define SOH     0x01    /* start of heading */
#define STX     0x02    /* Start of checksummed data */
#define ETX     0x03    /* End of checksummed data */
#define EOT     0x04    /* End of transmission */
#define ENQ     0x05    /* Enquire */
#define ACK     0x06    /* Positive acknowledgement */
#define LF      0x0a    /* Line feed */
#define CR      0x0d    /* Carriage return */
#define NAK     0x15    /* Negative acknowledgement */
#define ESC     0x1b    /* Escape */
#define EOFILE  0x1a    /* end of file */
#define LF      0x0a
#define CR      0x0d

/* AMS  communication protocol */
#define NO_MSGS                 0
#define START_ANALYSIS          'A'
#define WRITE_MEM_REC           'a'
#define ABORT_COMPLETE          'B'
#define BARCODE_ALIGN_RESPONSE  'b'
#define GET_CALS                'C'
#define SENDING_CALS            'c'
#define REQUEST_DATE_TIME       'D'
#define SEND_DATE_TIME          'd'
#define ERRORS                  'E'
#define NETPORT_TEST_RESPONSE   'e'
#define FINAL_RESULTS           'F'
#define CUVETTE_TEST_RESPONSE   'f'
#define SET_TEMPERATURE         'G'
#define BARCODE_GAIN_RESPONSE   'g'
#define CHANGE_CALS             'H'
#define SET_DATE_TIME           'h'
#define RAM_CONTENTS            'I'
#define READ_INSTRUCTION_RAM    'i'
#define CANCEL                  'J'
#define OK                      'K'
#define FAIL                    'k'
#define UPLOAD_LOG_FILE         'L'
#define START_TX_LOG_FILE       'l'
#define RUN_MIX_PROFILE         'M'
#define SET_MOTOR_SPEED         'm'
#define TRANSFER_MIX_PROFILE    'N'
#define TRANSFER_SAMPLE_PROFILE 'n'
#define DATA_RAM_CONTENTS       'O'
#define MOTOR_PARAMS            'o'
#define PROGRAM_BLOCK           'P'
#define END_PROGRAM             'p'
#define INITIALIZE_RTC          'Q'
#define READ_DATA_RAM           'q'
#define UPLOAD_RAD_FILE         'R'
#define START_UPLOAD_RAD_FILE   'r'
#define PREPARE_TO_RESET        'S'
#define RECALL_RESULTS          's'
#define SELF_TEST               'T'
#define TRANSMIT_COMPLETE       't'
#define STATUS                  'U'
#define REQUEST_STATUS          'u'
#define SW_REV                  'V'
#define REQUEST_SW_REV          'v'
#define RAW_RESULTS             'W'
#define WRITE_DATA_RAM          'w'
#define ERROR_LOG               'X'
#define DUMP_ERRORS             'x'
#define GET_TEMPERATURE         'Y'
#define TEMP_RESPONSE           'y'
#define START_PROGRAM_MODE      'Z'
#define WAKE_UP                 'z'

#define CMD_OFFSET              1
#define STX_OFFSET              2
#define LEN_OFFSET              3
#define CRC_LENGTH              4
#define MSG_HEADER              7   /* SOH + COMMAND + STX + LENGTH */
#define MSG_TAIL_LENGTH         5   /* CRC + EXT */
#define ETX_OFFSET              3   /* ACK / NAK ETX Offset */
#define INDEX_OFFSET            11  /* SOH + COMMAND + STX + LENGTH + TYPE */   
#define PAYLOAD_OFFSET          15  /* SOH + COMMAND + STX + LENGTH + TYP + INDEX */ 


/* write ram variable max index */
#define  MAX_VARIABLE_ID_INDEX  36
#define  INIT_NON_VOL_MEM       0xfe
#define  UPDATE_NON_VOL_MEM     0xff

/* id dump line length */
#define ID_LINE_LENGTH      29


#define AIR_TEMP            0           /* thermister ID */
#define AMBIENT_TEMP        1
#define TOP_TEMP            2
#define BOTTOM_TEMP         3


/* Calibration type */ 
typedef enum {
    GLOBAL_CUV_DELAY             = 0,
    CUVETTE_DELAY                = 1,
    BFD_CALIBRATION_FACTOR       = 2,
    INSTRUMENT_SERIAL_NUMBER     = 3,
    EEPROM_VERSION               = 4,
    CAL_BFD_FACTOR               = 5,
    RESTORE_DEFAULT              = 6,
    HEATER_DAC_SETTING           = 7,
    HEATER_TIME_SETTING          = 8,
    FORCE_RAW_DUMP               = 9,
    AIR_TEMPERATURE_OFFSET       = 10,
    NEW_CALIBRATION              = 11,
    CUVETTE_TRANSIMT_LIMIT       = 12,
    FAN_TEMPERATURE_SETTING      = 14,
    UPDATE_FAN_TEMPERATURE       = 15,
    W850_FILTER_CALIBRATION      = 16,
    W630_FILTER_CALIBRATION      = 17,
    W600_FILTER_CALIBRATION      = 18,
    W550_FILTER_CALIBRATION      = 19,
    W515_FILTER_CALIBRATION      = 20,
    W500_FILTER_CALIBRATION      = 21,
    W467_FILTER_CALIBRATION      = 22,
    W405_FILTER_CALIBRATION      = 23,
    W340_FILTER_CALIBRATION      = 24
} CalibrationType_t;

typedef enum {
    ENG_ANALYSIS                = 0x30,     /* '0' */
    WBH_ANALYSIS                = 0x31,     /* '1' */      
    ROTOR_DELAY                 = 0x41,     /* 'A' */
    ROBS_TEST                   = 0x42,     /* 'B' */  
    CUV_DELAY                   = 0x43,     /* 'C' */
    BARCODE                     = 0x44,     /* 'D' */
    ABOARD_TEST                 = 0x45,     /* 'E' */
    CALIBRATION_TEST            = 0x46,     /* 'F' */
    MOTOR_TEST                  = 0x47,     /* 'G' */
    SPEED_CUV                   = 0x48,     /* 'H' */
    SPEED_ENC                   = 0x49,     /* 'I' */
    DOWNLOAD                    = 0x5a,     /* 'Z' */
    BARCODE_ALIGN               = 0x62,     /* 'b' */
    NETPORT_TEST                = 0x65,     /* 'e' */
    CUVETTE_TEST                = 0x66,     /* 'f' */
    BARCODE_GAIN                = 0x67,     /* 'g' */
    AUTORUN                     = 0x80,
    ROTOR_07                    = 0x87,
    ROTOR_11                    = 0x8b,
    ROTOR_12                    = 0x8c,
    ROTOR_15                    = 0x8f,
    ROTOR_17                    = 0x91,
    ROTOR_18                    = 0x92,
    ROTOR_21                    = 0x95,
    ROTOR_23                    = 0x97,
    ROTOR_26                    = 0x9a,
    ROTOR_29                    = 0x9d,
    ROTOR_30                    = 0x9e,
    ROTOR_31                    = 0x9f,
    ROTOR_32                    = 0xa0,
    ROTOR_36                    = 0xa4,
    ROTOR_37                    = 0xa5,
    ROTOR_38                    = 0xa6,
    ROTOR_40                    = 0xa8,
    ROTOR_41                    = 0xa9,
    ROTOR_42                    = 0xaa,
    ROTOR_43                    = 0xab,
    ROTOR_44                    = 0xac,
    ROTOR_45                    = 0xad,
    ROTOR_46                    = 0xae,
    ROTOR_47                    = 0xaf,
    ROTOR_48                    = 0xb0,
} AnalysisType_t;

typedef enum {
        CUVDLY_IDX,
        ROBSLP_IDX,
        CALTST_IDX,
        RTRDLY_IDX,
        BARCODE_IDX,
        ENGINE_IDX,
} IdIndex_t;

#define  WAVE_FILTER_OFFSET    W340_FILTER_CALIBRATION 


#endif
