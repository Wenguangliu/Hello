#ifndef __TRANSMIT_H_INCLUDED
#define __TRANSMIT_H_INCLUDED

class Transmit
{
public:
	void Run( int fdSer );
	
private:
	char			HexToAscii( unsigned char hex ) { return  "0123456789ABCDEF"[ hex & 0x0F ]; }
	unsigned char	WaitForAck();
	void			SendRs232Message( EngineCommMessageData_t * message );
	void			SendMessage( EngineCommMessageData_t * msgBody );

	int fdSer;
};

#endif // __TRANSMIT_H_INCLUDED
