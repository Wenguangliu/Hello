/*
 * FILENAME
 * 	CalcResultsMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the CalcResults process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __CALCRESULTSMESSAGES_H_INCLUDED
#define __CALCRESULTSMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"

#define CALC_RESULTS_QUEUE_NAME	"/CalcResults"

/*
 * Message IDs for messages received by the CalcResults process.
 */
typedef enum
{
	MSG_CALC_RESULTS_INIT_CALCULATION = MSG_FIRST_CALC_RESULTS_ID,
	MSG_CALC_RESULTS_DAC_TRIMS_COMPLETE,
	MSG_CALC_RESULTS_BLACK_OFFSETS_COMPLETE,
	MSG_CALC_RESULTS_BEAD_CHECK_1_COMPLETE,
	MSG_CALC_RESULTS_BEAD_CHECK_2_COMPLETE,
	MSG_CALC_RESULTS_DISTRIBUTION_CHECK_COMPLETE,
	MSG_CALC_RESULTS_ANALYSIS_DONE,
	
	MSG_CALC_RESULTS_OUT_ERROR,
} CalcResultsMsgId_t;

typedef struct
{
	int			patientType;
	int			age;
	Race_t		race;
	Gender_t	gender;
} CalcResultsMsgData_t;

typedef struct
{
	CalcResultsMsgId_t		msgId;
	CalcResultsMsgData_t	data;
} CalcResultsMsg_t;

#endif	/* __CALCRESULTSMESSAGES_H_INCLUDED */

