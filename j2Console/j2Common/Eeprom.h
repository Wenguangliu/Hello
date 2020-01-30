#ifndef _EEPROM_H
#define _EEPROM_H

#include <windows.h>

#define EEPROM_CMD_REG          0x110

#define EEPROM_CMD_REG_OFFSET   0x0 
#define EEPROM_ADDR_REG_OFFSET  0x4 
#define EEPROM_DATA_REG_OFFSET  0x8
#define EEPROM_READ_REG_OFFSET  0xc

#define WRITE_EEPROM_CMD     0xA0
#define READ_EEPROM_CMD      0xA1
#define BLINK_LED_CMD        0xB0
#define FORCE_LED_ON         0xB1   

extern char 
EepromRead (unsigned char rAddress, char *rValue);

extern unsigned char
EepromWrite (unsigned char wAddress, char wValue);

extern INT8
SetLedBlink (void);

extern INT8
SetLedSteadyOn (void);

#endif
