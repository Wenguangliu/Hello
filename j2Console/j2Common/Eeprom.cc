#include <stdio.h>

#include "Eeprom.h"
#include "LogErr.h"
#include "Gpio.h"


#define null       0


static uintptr_t   handle = null;


#if 0
/* output 8 bits to eeprom */
unsigned char
out8 (unsigned short port, unsigned char value)
{
  __asm {
    mov al, value
    mov dx, port
    out dx, al
  }
  return (value);
}

/* read 8 bits from eeprom */
unsigned char
in8 (unsigned short port)
{
  unsigned char value;

  __asm {
    mov dx, port
    in al, dx
    mov value, al
  }
  return (value);
}

/* write 32 bits to eeprom */
unsigned long
out32 (unsigned short port, unsigned long value)
{
  __asm {
    xor edx, edx
    xor eax, eax
    mov eax, value
    mov dx, port
    out dx, eax
  }
  return (value);
}

/* read 32 bit from eeprom */
unsigned long
in32 (unsigned short port)
{
  unsigned long value;

  __asm {
    xor edx, edx
    xor eax, eax
    mov dx, port
    in eax, dx
    mov value, eax
  }
  return (value);
}
#endif


/*
 * read from eeprom
 * address :  0 - 127 totally 128 bytes could be read
 * value   :  0 - 255  8 bits values
 */
char 
EepromRead (unsigned char rAddress, char *rValue)
{

    unsigned char status, timeout = 20;
    unsigned short base = EEPROM_CMD_REG;

    /* set read eeprom address */
    Out8(base + EEPROM_ADDR_REG_OFFSET, rAddress);

    /* set read command */
    Out8(base + EEPROM_CMD_REG_OFFSET, READ_EEPROM_CMD);

    /* checking read status */
    do {
      Sleep(100);
      status = In8(base + EEPROM_CMD_REG_OFFSET);
    }   while ((status != 0) && (--timeout != 0));

    if (timeout == 0) {
        LogError ("Read Eeprom timeout");
        return -1;
    }

    *rValue = In8(base + EEPROM_READ_REG_OFFSET);
    return 0;
}

/*
 * write to eeprom
 * address :  0 - 127 totally 128 bytes  could be saved
 * value   :  0 - 255  8 bits values
 */
unsigned char
EepromWrite (unsigned char wAddress, char wValue)
{
    unsigned short base = EEPROM_CMD_REG;

    /* set write value to reg */
    Out8 (base + EEPROM_DATA_REG_OFFSET, wValue);
     /* set read eeprom address */
    Out8 (base + EEPROM_ADDR_REG_OFFSET, wAddress);
    /* set write command */
    Out8 (base + EEPROM_CMD_REG_OFFSET, WRITE_EEPROM_CMD);
    Sleep (10);

    return 0;
}

/*
 * turn on power button led blink
 */

INT8
SetLedBlink (void)
{
    unsigned short base = EEPROM_CMD_REG;

    Out8 (base + EEPROM_CMD_REG_OFFSET, BLINK_LED_CMD);

    return 0;
}

/*
 * turn on power button led stable
 */
INT8
SetLedSteadyOn (void)
{
   unsigned short base = EEPROM_CMD_REG;

    Out8 (base + EEPROM_CMD_REG_OFFSET, FORCE_LED_ON);

    return 0;
}
