#include <stdio.h>
//#include <stdint.h>
//#include <sys/mman.h>
//#include <hw/inout.h>
//#include <sys/neutrino.h>
//#include <unistd.h>

#include "Gpio.h"


/* output 8 bits */
unsigned char
Out8 (unsigned short port, unsigned char value)
{
  __asm {
    mov al, value
    mov dx, port
    out dx, al
  }
  return (value);
}

/* read 8 bits */
unsigned char
In8 (unsigned short port)
{
  unsigned char value;

  __asm {
    mov dx, port
    in al, dx
    mov value, al
  }
  return (value);
}

/* write 32 bits */
unsigned long
Out32 (unsigned short port, unsigned long value)
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

/* read 32 bit */
unsigned long
In32 (unsigned short port)
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


/*
 i rehad pci io register
 * handle       --- QNX memory map address
 * busNum       --- PCI bus number
 * deviceNUM    --- PCI device number
 * functionNUM  --- PCI function number
 * registerNum  --- PCI reigster number
 * for PCI input and output, it should be at address 0xcfc,therefore, the input address =  handle + (0xcfc - 0xcf8) + register number)
 */
UINT32
PciRead (UINT32 busNum, UINT32 deviceNum, UINT32 functionNum, UINT32 registerNum)
{
    DWORD cValue;
    WORD  port;
    DWORD data;
    
    cValue = (PCI_CONF_ACCESS_ENA | (busNum << 16) | (deviceNum << 11) | (functionNum << 8) | (registerNum & 0xfc));
    
    port = PCI_IO_ADDRESS;

    /* write control register to access pci io*/
     __asm {
                mov eax,cValue 
                mov dx, port
                out dx, eax 
           }
    /* read data from PCI register */
    port = PCI_DATA_ADDRESS  | (registerNum & 0xfc);
    __asm {
                mov dx,  port 
                in  eax, dx 
                mov data,eax
           }
    return data;
}



/*
 * write pci io register
 *
 */

int
PciWrite (UINT32 busNum, UINT32 deviceNum, UINT32 functionNum,UINT32 registerNum, UINT32 wValue)
{
    DWORD cValue;
    WORD  port;
    DWORD data;
    
    data = wValue;
    
    cValue = (PCI_CONF_ACCESS_ENA | (busNum << 16) | (deviceNum << 11) | (functionNum << 8) | (registerNum & 0xfc));

    port = PCI_IO_ADDRESS; 

    /* write control register to access pci io*/
     __asm {
                mov eax, cValue 
                mov dx, port
                out dx, eax 
           }
    /* write value to pci register */
    port = PCI_DATA_ADDRESS | ( registerNum & 0xfc); 

     __asm {
                mov eax, data
                mov dx, port
                out dx, eax 
           }
    return 0;
}

/*
 * write one byte to Smbus register
 */
void
SmbusOneByteWrite (unsigned char device, unsigned char command, unsigned char *pBuffer, unsigned char length)
{
    unsigned short  smBusioBase;

    // get smBus IO base address
    smBusioBase =  PciRead (0, 0x1f, 3, 0x20) & 0xFFF0;

    // now, implement the I2Bus protocol
    Out8 (smBusioBase, 0xFF);               // clear status register
    //In8  (smBusioBase + 2);

    Out8 (smBusioBase + 2, 0x14);           // byte data command
    Out8 (smBusioBase + 4, device);         // slave device address
    Out8 (smBusioBase + 3, command);        // command
    Out8 (smBusioBase + 5, length);         // account
    Out8 (smBusioBase + 6, 0);
    Out8 (smBusioBase + 7, pBuffer[0]);     // data to be written
    Out8 (smBusioBase + 9, 0);

    Out8 (smBusioBase + 2, 0x54);           // block command
    Out8 (smBusioBase    , 0x80);
    Out8 (smBusioBase    , 0x42);
    Out8 (smBusioBase    , 0xFF);
}
