#include <windows.h>
#include <commctrl.h>

#include "FTD2XX.h"
#include "Ftdi.h"
#include "Epcs.h"
#include "FtdiConfig.h"
#include "LogErr.h"


FT_HANDLE       fd [4];


/*
 * close all FTDI device port
 */
void
CloseAllPorts (void)
{
    unsigned short i;

    for (i =0; i < 4; i++) {
        CloseSerialPort (fd [i]);
    }
}

/*
 * initial all ftdi port and  un-reset engine board
 */
int
InitialEngineBoardInterface (void)
{

    int i;

    memset (fd, 0, sizeof fd);

    /* initial Engine board at default configuration */
    if (InitialFtdiPort () == -1) {
        return -1;
    }

    /* open FTDI 4 device port */
    for (i = 0; i < 4; i++)  {
        fd [i] = OpenSerialPort (i);
    }

    /* check MPSSE port is configured OK or not */
    if (CheckMpssePort (fd[0]) == -1) {
        CloseAllPorts ();
        return -1;
    }
    Sleep (10);

    /* reset engine board */
    if (ResetEngineBoard (fd[0], 1) == -1) {
        CloseAllPorts ();
        return -1;
    }
    Sleep (2);
    /* unreset engine board */
    if (ResetEngineBoard (fd[0], 0) == -1) {
        CloseAllPorts ();
        return -1;
    }

    CloseAllPorts ();

    return 0;
}

/*
 * shutdown Engine board
 */
int
EngineBoardReset (void)
{
    fd [0] = OpenSerialPort (0); 

    if (fd [0] == INVALID_HANDLE_VALUE) {
        LogError ("Fail to open ftdi port 0");
        return -1;
    } 

    /* check MPSSE port is configured OK or not */
    if (CheckMpssePort (fd[0]) == -1) {
        LogError ("Ftdi port0 MPSSE config check fail");
        return -2;
    }

    if (ResetEngineBoard (fd[0], 1) == -1) {
        LogError ("Fail to reset Engine board");
        return -3;
    }
    return 0;
}


/*
 * re-boot Engine board
 */
int
ReBootEngineBoard (void)
{
    fd [0] =  OpenSerialPort (0); 
    
    if (fd [0] != INVALID_HANDLE_VALUE) {
        ResetEngineBoard (fd[0], 1); 
        Sleep (5);
        ResetEngineBoard (fd[0], 0);
        CloseSerialPort (fd[0]);
        return 0;
    } 
    return -1;
}

