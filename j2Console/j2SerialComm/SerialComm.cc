/*
 * FILENAME
 * 	SerialComm.cc
 *
 * MODULE DESCRIPTION
 * 	Start-up code for the Serial Comm process that talks to the outside world.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <commctrl.h>
#include <FtdiConfig.h>

#include "SerialComm.h"
#include "Transmit.h"
#include "Receive.h"
#include "Shmem.h"
#include "LogErr.h"
#include "SetSerialPortSpeed.h"


//#define HAVE_MODE_T 1

AStrings        strings;

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
	FT_HANDLE		fdSer;
	HANDLE			transid;
	const char *	port = "/dev/serusb3";
	long			speed = BAUD_RATE_NORMAL;
	int				i=0;

	// Get the command line options.
	while ( i < argc )
	{
		if (argv[i] == "-p")
		{
			port = argv[++i];
			LogDebug("SerialComm| argv[%u]=%s",i,port);
		}
		else if (argv[i] == "-b")
		{
			speed = atoi(argv[++i]);
			LogDebug("SerialComm| argv[%u]=%s",i,port);
		}

		i++;
	}

    if (InitialSerialPort () == -1) {
        return EXIT_FAILURE;
    }

	// Start a thread that gets messages from other Console processes.
	//pthread_create(&transid, NULL, TransmitThread, (void *)&fdSer);
	transid = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TransmitThread, (void *)&fdSer, 0, NULL);

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
	FT_HANDLE fdSer = *(FT_HANDLE *)data;

	Transmit transmit;
	transmit.Run( fdSer );

	return 0;
}

