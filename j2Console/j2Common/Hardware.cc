//#include <stdint.h>
//#include <sys/mman.h>
//#include <sys/neutrino.h>
//#include <hw/inout.h>
#include <stdio.h>

#include "LogErr.h"
#include "Gpio.h"
#include "Hardware.h"

// set Intel 945 backlight
void
SetBacklight (int setting)
{
    PciWrite (0, 2, 1, 0xf4, (UINT32)setting);
}

//set BT2 backlight
void
SetBtBackLight (int setting)
{
    unsigned char setValue;
    // turn off backlight, min value is 1, instead of 0
    if (setting == 0) {
        setValue = 1;
    } else {
        setValue = (unsigned char)setting;
    }
    // somehow it requires set two times for stable settings
    SmbusOneByteWrite (0x50, 0x80, (unsigned char*)&setValue, 0x01);
    SmbusOneByteWrite (0x50, 0x80, (unsigned char*)&setValue, 0x01);
}

void
LcdBackLightSet (char boardId, int setting) 
{
    if (boardId == 'B') {
        SetBtBackLight (setting);
    } else {
        SetBacklight (setting);
    }
}
