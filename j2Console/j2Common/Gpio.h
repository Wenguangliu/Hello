#ifndef GPIO_H_
#define GPIO_H_

#include <windows.h>

#define  GPIO_USE_SEL_OFFSET        0x00
#define  GPIO_SEL_OFFSET            0x04
#define  GPIO_LVL_OFFSET            0x0c
#define  GPIO_BLNK_OFFSET           0x18
#define  GPIO_INV_OFFSET            0x2C
#define  GPIO_USE_SEL2_OFFSET       0x30
#define  GPIO_SE2_OFFSET            0x34
#define  GPIO_LVL2_OFFSET           0x38
#define  GPIO_ADDRESS(gpio,offset) (gpio + offset)

#define PCI_IO_ADDRESS      0x0cf8
#define PCI_DATA_ADDRESS    0x0cfc
#define PCI_CONF_ACCESS_ENA 0x80000000

#define false               0
#define true                1

extern unsigned char
Out8 (unsigned short port, unsigned char value);

extern unsigned char
In8 (unsigned short port);

unsigned long
Out32 (unsigned short port, unsigned long value);

unsigned long
In32 (unsigned short port);


extern UINT32
PciRead (UINT32 busNum, UINT32 deviceNum, UINT32 functionNum, UINT32 registerNum);

extern int
PciWrite (UINT32 busNum, UINT32 deviceNum, UINT32 functionNum,UINT32 registerNum, UINT32 wValue);

extern void
SmbusOneByteWrite (unsigned char device, unsigned char command, unsigned char *pBuffer, unsigned char length);
#endif
