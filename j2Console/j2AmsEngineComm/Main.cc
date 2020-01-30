/*
 * FILENAME
 * 	Main.cc
 *
 * MODULE DESCRIPTION
 * 	Start-up code for the Engine Comm process.
 *
 * AUTHOR
 * 	Greg De Hoogh
 */

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <FtdiConfig.h>

#include "Global.h"
#include "Transmit.h"
#include "Receive.h"
#include "Shmem.h"
#include "LogErr.h"


DWORD WINAPI TransmitThread( void * data );

#define ENGINE_II

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
	FT_HANDLE	    fdSer;
//	int				c;
    const int       port = 3;

    LogDebug ("Startup AmsEnginComm");
	//Sleep (10000);

#if 0
    // Initial Engine board at default configuration
    if (InitialFtdiPort () == -1) {
        LogError ("Fail to initial Ftdi port");
        return EXIT_FAILURE;
    }
#endif


    fdSer = OpenSerialPort(port);

    if (fdSer == INVALID_HANDLE_VALUE) {
        LogError( "failed to open %u", port );
        return EXIT_FAILURE;
    }


     InitGlobal();

	// Start a thread that gets messages from other Console processes and sends
	// them to the Engine.
    CreateThread(NULL, 0, TransmitThread, (void *)&fdSer, 0, NULL);


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

DWORD WINAPI
TransmitThread( void * data )
{
	FT_HANDLE fdSer = *(FT_HANDLE *)data;

	Transmit transmit;
	transmit.Run( fdSer );

	return 0;
}

