#ifndef _PRINTER_COM_H_
#define _PRINTER_COM_H_
#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif



#define  PRN_CURRENT_ERR                0x10
#define  PRN_SPEED_ERR                  0x20
#define  PRN_INTERSITY_ERR              0x30
#define  PRN_FONT_ERR                   0x40
#define  PRN_HEAT_ERR                   0x50
#define  PRN_LINE_SPACE_ERR             0x60
#define  PRN_LETTER_SPACE_ERR           0x70
#define  PRN_UNKNOW_ERR                 0x80


int
ApsPrinterPortClose (HANDLE hPrinter);

HANDLE WINAPI
PrinterPortOpen (PWSTR portName);

HANDLE WINAPI
ApsPrinterPortOpen (void);

int
GetApsPrinterId (HANDLE hPrinter, char *idBuf, int size);

int
GetApsPrinterStatus (HANDLE hPrinter);


DWORD
InitialApsPrinter (HANDLE hPrinter);

DWORD
SendStringToApsPrinter (HANDLE hPrinter, unsigned char *txBuf, int txSize);

DWORD
PrintString (HANDLE hPrinter, char *txStr);

int
ApsPrinteTestString (HANDLE hPrinter, char *testTitle);

char *
CenterText (const char * str, char * buff);

unsigned char
PrinterStatus (HANDLE hPrinter);

int
ResetPrinter (HANDLE h);

BOOL 
ReadStringFromAspPrinter (HANDLE hPrinter, char *rxBuf, DWORD *rdBytes);
#ifdef __cplusplus
}

int
GetPrinterFirmWareVersion (PBYTE rBuf);
#endif
#endif
