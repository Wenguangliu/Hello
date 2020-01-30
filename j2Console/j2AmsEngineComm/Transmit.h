#ifndef __TRANSMIT_H
#define __TRANSMIT_H

class Transmit
{
public:
	void Run( FT_HANDLE fdSer );
	
private:
	char			HexToAscii( unsigned char hex ) { return  "0123456789ABCDEF"[ hex & 0x0F ]; }
	unsigned char	WaitForAck();
	void			SendRs232Message( EngineCommMessageData_t * message );
	void			SendMessage( EngineCommMessageData_t * msgBody );
    void            ProcessEngComSendMsg (EngineCommReceiveMsg_t *msg);

	FT_HANDLE       fdSer;
};

#endif // __TRANSMIT_H
