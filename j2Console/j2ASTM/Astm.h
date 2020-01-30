#ifndef ASTM_H_
#define ASTM_H_

#include "SettingsData.h"
#include "FactoryData.h"
#include "SystemData.h"
#include "AstmInput.h"

extern SettingsData_t *	settingsData;
extern FactoryData_t *	factoryData;
extern SystemData_t *	systemData;

typedef enum
{
	ABAXIS_SEND_DONE_TYPE,										// abaxis send done        
	ABAXIS_SEND_1394_COMMENT_MSG_TYPE,							// abaxis astm 1394 comment type      
	ABAXIS_SEND_ROTOR_RUN_DONE_TYPE,							// abaxis last rotor run done & results database record available
	ABAXIS_SEND_RESULTS_QUERY_TYPE,								// abaxis astm query type       
	ABAXIS_SEND_M1_TYPE,										// abaxis tx queue manufacturers defined 1 type (debug only)      
	ABAXIS_SEND_RESULTS_RANGE_TYPE,								// abaxis astm results range type
	ABAXIS_SEND_ORDER_DATA_INVALID_TYPE,						// abaxis order input data invalid field type
	ABAXIS_SEND_ORDER_CANCELLATION_TYPE,						// abaxis order cancellation type per 9.4.26 (X,I,Y)
    ABAXIS_SEND_M_AS_TYPE,                                      // abaxis manufacturing AS analyzer status type
    ABAXIS_SEND_M_PO_TYPE,                                      // abaxis manufacturing PO pending order type
} AbaxisSendType_t; 

#define ABAXIS_SEND_QUEUE_SIZE			100						// max number of abaxis astm transmit queue entries	      


#if 0
void	QueueAstmTxMsg( AbaxisSendType_t msgSendType,
						unsigned int msgParam1 = 0,
						char msgParam2 = '\0',
						const char *msgParam3 = "",
						const char *msgParam4 = "",
						const AstmInput_t *msgParam5 = 0 );
#endif
void    FillAstmOrdersList();

#endif /*ASTM_H_*/
