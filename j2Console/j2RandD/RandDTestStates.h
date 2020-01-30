#ifndef _RANDD_TEST_STATES_H
#define _RANDD_TEST_STATES_H

// RandD test states
typedef enum
{
    STATE_BACKGROUND,
    STATE_INFO,
    STATE_MAIN_SELECTION,                    // main selections
    STATE_TROUBLE_SHOOTING,
    STATE_TROUBLE_SHOOTING_2,
    STATE_MANUFACTURING_SELECTION,  
    STATE_MANUFACTURING,
    STATE_MANUFACTURING_2,
    STATE_AMS_TEST_REPORT,
    STATE_AMS_TEST_REPORT_2,
    STATE_AMS_DISPLAY_REPORT,
    STATE_BARCODE_READ_TEST,                // barcode test
    STATE_BARCODE_ALIGNMENT,
    STATE_MOTOR_TEST,                       // motor test
    STATE_DRAWER_TEST,
    STATE_DT_DRAWER_IS_OPENING,             //drawer test
    STATE_DT_DRAWER_IS_OPENED,
    STATE_DT_WAIT_FOR_NEXT_CYCLE,
    STATE_DT_DRAWER_OPENED,
    STATE_DT_LOAD_DISK,
    STATE_DT_DOWNLOAD_RIS_FILE,
    STATE_DT_READ_BARCODE,
    STATE_DT_DRAWER_IS_CLOSING,
    STATE_DT_DRAWER_IS_CLOSED,
    STATE_DT_ANALYSIS_IN_PROCESS,
    STATE_DT_CONFIRM_CANCEL,
    STATE_DT_CANCELING,
    STATE_DT_TEST_COMPLETED,
    STATE_DT_TEST_COUNTER_DISPLAY,
    STATE_DT_REMOVE_DISK,
    STATE_MANUFACTURING_INFO,
    STAET_VIEW_LOG_FILE,                    // display current log
    STATE_HEATER_TEST,                      // heater test
    STATE_WAIT_ANALY_WARM_UP,
    STATE_LAMP_TEST,                        // lamp test
    STATE_TRIM_DAC_CALI_IN_PROGRESS,
    STATE_TRIM_DAC_CALI,
    STATE_TOUCH_SCREEN_CALI,                // touch screen calibration and test
    STATE_CUVETTE_TEST,
    STATE_NETWORK_TEST,                     //network test
    STATE_NETWORK_CONFIG,
    STATE_NETWORK_CONFIGURING,
    STATE_TELNET_CONFIG,
    STATE_ENTER_ANALYZER_IP,
    STATE_ENTER_ANALYZER_MASK,
    STATE_ENTER_GATEWAY,
    STATE_ENTER_SERVER_IP,
    STATE_ENTER_SERVER_SOCKET,
    STATE_ENTER_HOST_NAME,
    STATE_NETWORK_CONFIG_ERROR,
    STATE_ENGINE_SERIAL_DEBUG_PORT_SET,     // enable/disable engine debug port
    STATE_AMS_MOTOR_TEST,                   // ams motor test
    STATE_AMS_MOTOR_TEST_IN_PROGRESS,
    STATE_AMS_MOTOR_TEST_CANCEL,
    STATE_AMS_MOTOR_TEST_RESULTS_CAL,
    STATE_AMS_MOTOR_TEST_SHOW_RESULTS,
    STATE_AMS_MOTOR_TEST_SHOW_REPORTS,
    STATE_AMS_NDXT_TEST,                    // ams ndxt test
    STATE_AMS_NDXT_TEST_IN_PROGRESS,
    STATE_AMS_NDXT_TEST_CANCEL,
    STATE_AMS_NDXT_TEST_RESULTS_CAL,
    STATE_AMS_NDXT_TEST_SHOW_RESULTS,
    STATE_AMS_NDXT_TEST_SHOW_REPORTS,
    STATE_AMS_TEMP_TEST,                    // ams temperatuer test
    STATE_AMS_WAIT_ANALY_WARM_UP,
    STATE_AMS_TEMP_TEST_INPUT_ROTOR_ID,
    STATE_AMS_TEMP_TEST_WRONG_ROTOR_ID,
    STATE_AMS_TEMP_TEST_IN_PROGRESS,
    STATE_AMS_TEMP_TEST_CANCEL,
    STATE_AMS_INPUT_ROTOR_OHMS,
    STATE_AMS_TEMP_TEST_RESULTS_CAL,
    STATE_AMS_TEMP_TEST_SHOW_RESULTS,
    STATE_AMS_TEMP_TEST_SHOW_REPORTS,
    STATE_AMS_DELAY_TEST,                   // ams global delay test
    STATE_AMS_DELAY_TEST_IN_PROGRESS,
    STATE_AMS_DELAY_TEST_CANCEL,
    STATE_AMS_DELAY_TEST_RESULTS_CAL,
    STATE_AMS_DELAY_TEST_SHOW_RESULTS,
    STATE_AMS_DELAY_TEST_SHOW_REPORTS,
    STATE_AMS_BARCODE_TEST,                 // ams barcode test
    STATE_AMS_BARCODE_TEST_IN_PROGRESS,
    STATE_AMS_BARCODE_TEST_CANCEL,
    STATE_AMS_BARCODE_TEST_RESULTS_CAL,
    STATE_AMS_BARCODE_TEST_SHOW_RESULTS,
    STATE_AMS_BARCODE_TEST_SHOW_REPORTS,
    STATE_AMS_ALIGNMENT_TEST,               // ams barcode alignment test
    STATE_AMS_ALIGNMENT_TEST_IN_PROGRESS,
    STATE_AMS_ALIGNMENT_TEST_CANCEL,
    STATE_AMS_ALIGNMENT_TEST_RESULTS_CAL,
    STATE_AMS_ALIGNMENT_TEST_SHOW_RESULTS,
    STATE_AMS_ALIGNMENT_TEST_SHOW_REPORTS,
    STATE_AMS_CUV_TEST,                     // ams cuvette test
    STATE_AMS_CUV_TEST_INPUT_ROTOR_ID,
    STATE_AMS_CUV_TEST_WRONG_ROTOR_ID,
    STATE_AMS_CUV_TEST_IN_PROGRESS,
    STATE_AMS_CUV_TEST_CANCEL,
    STATE_AMS_CUV_TEST_RESULTS_CAL,
    STATE_AMS_CUV_TEST_SHOW_RESULTS,
    STATE_AMS_CUV_TEST_SHOW_REPORTS,
    STATE_MANUFACTURING_ERROR,
    STATE_COUNT                             // total test state counts
};

#endif