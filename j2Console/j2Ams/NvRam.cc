#include <windows.h>
#include <stdio.h>

#include "Eeprom.h"
#include "LogErr.h"
#include "Global.h"

#define EEPROM_CMD_REG          0x110

#define EEPROM_CMD_REG_OFFSET   0x0
#define EEPROM_ADDR_REG_OFFSET  0x4
#define EEPROM_DATA_REG_OFFSET  0x8
#define EEPROM_READ_REG_OFFSET  0xc

#define WRITE_EEPROM_CMD     0xA0
#define READ_EEPROM_CMD      0xA1
#define BLINK_LED_CMD        0xB0
#define FORCE_LED_ON         0xB1


/*
 * FUNCTION
 *  ReadNvRamStr  
 *
 * DESCRIPTION
 *  Read String from NVRAM. The String should have 
 *  the '\0' terminal.
 *
 * PARAMETERS
 *   *str       buffer pointer to save the string
 *
 * RETURNS
 *     0         ---- Read successfule
 *    -1         ---- Read Fail    
 */

unsigned char
ReadNvRamStr (char *str)
{

    unsigned char rAddress = 0 ; 

    do {
       EepromRead (rAddress, str);
       rAddress++;
    } while (*str++ != '\0');

    *str = '\0';

    return 0;
}

/*
 * FUNCTION
 *  WriteNvRamStr
 *
 * DESCRIPTION
 *  Write String to NVRAM. At the end of  string, 
 *  the char '\0' terminal should be added
 *
 * PARAMETERS
 *   startAddress   first nvRam address to be written (0 - 254) 
 *   *str           buffer pointer to save the string
 *   len            length of string (max  = 255)
 *
 * RETURNS
 *     0         ---- Write successfule
 *    -1         ---- Write Fail
 */

unsigned char
WriteNvRamStr (unsigned char startAddress, char *str, unsigned char len)
{

    unsigned char  wAddress;

    wAddress  = startAddress;

    while (len-- > 0) {
        EepromWrite (wAddress, (*str));
        wAddress++;
        str++;
    }
    EepromWrite (wAddress, '\0');

    return 0;
}

/*
 * FUNCTION
 *  InitNvRamValue 
 *
 * DESCRIPTION
 *  Write NVARM to be 0xff from start address to 128 
 *
 * PARAMETERS
 *   startAddress   first nvRam address to be written (0 - 127)
 *
 * RETURNS
 *     0         ---- Write successfule
 *    -1         ---- Write Fail
 */

unsigned char
InitNvRamValue (unsigned char startAddress)
{
    
    unsigned char rValue = 0xff;
    while (startAddress < 128) {
        EepromWrite (startAddress, rValue);
        startAddress++; 
        
    }
    return 0;
}
