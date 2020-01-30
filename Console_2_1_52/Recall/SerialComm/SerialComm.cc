/*
 * FILENAME
 * 	SerialComm.cc
 * 
 * MODULE DESCRIPTION
 * 	Start-up code for the Serial Comm process that talks to the outside world.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <sys/neutrino.h>

#include "SerialComm.h"
#include "Transmit.h"
#include "Receive.h"
#include "Shmem.h"
#include "LogErr.h"

int				transmitReceiveState = NEITHER_ACTIVE;
unsigned char	receivedChar;
Receive *		receive;


void *	TransmitThread( void * data );

/*
 * FUNCTION
 * 	main
 * 
 * DESCRIPTION
 * 	Gets the command line parameters, which may contain an alternate serial port
 * 	and/or baud rate, opens the serial port, and inits it to the desired baud
 * 	rate.  It then starts the Transmit thread and then passes control of the
 * 	main thread to the Receive thread.
 * 
 * PARAMETERS
 * 	Possible command line parameters are:
 * 	 -p<port>
 * 	 -b<baud rate>
 * 
 * RETURNS
 * 	Exit status.
 */

int
main(int argc, char *argv[])
{
	int				fdSer;
	int				c;
	struct termios	term;
	const char *	port = "/dev/ser2";
	long			speed = BAUD_RATE_NORMAL;
	int				temp;
	
	// Get the command line options.
	while ( ( c = getopt( argc, argv, "p:b:" ) ) != -1 )
	{
		switch ( c )
		{
			case 'p':		// Port name.
				port = optarg;
				break;

			case 'b':		// Baud rate.
				if ( sscanf( optarg, "%d", &temp ) == 1 )
				{
					speed = temp;
				}
				break;
		}
	}
	
	// Open the serial port.
	fdSer = open( port, O_RDWR, 0 );
	if ( fdSer == -1 )
	{
		LogError( "Failed to open serial port %s\n", port );
		return EXIT_FAILURE;
	}

	// Set the serial port's baud rate and handshaking.
	tcgetattr( fdSer, &term );
	term.c_ispeed = speed;
	term.c_ospeed = speed;
	term.c_iflag &= ~(IXOFF | IXON);	// Disable Xon/Xoff.
	term.c_cflag &= ~(IHFLOW | OHFLOW);	// Disable RTS/CTS.
	tcsetattr( fdSer, TCSANOW, &term );

	// Start a thread that gets messages from other Console processes.
	ThreadCreate( 0, TransmitThread, &fdSer, NULL );
	
	// Receive the data.
	receive = new Receive();
	receive->Run( fdSer );

	return EXIT_SUCCESS;
}

/*
 * FUNCTION
 * 	TransmitThread
 * 
 * DESCRIPTION
 * 	Creates a Transmit object to control the Transmit thread.
 * 
 * PARAMETERS
 * 	data - Pointer to the serial port handle.
 * 
 * RETURNS
 * 	The thread creation routine expects a function which returns a void *.  In
 * 	actual operation, this Transmit Run method will not return unless there is
 * 	a fatal error in start-up.
 */

void *
TransmitThread( void * data )
{
	int fdSer = *(int *)data;

	Transmit transmit;
	transmit.Run( fdSer );

	return 0;
}

