#ifndef __GPIO_H_INCLUDED
#define __GPIO_H_INCLUDED

#include <stdint.h>

#define GPIO_BASE_ADDR			0xd000
#define GPIO_PORT_DIR_REG		(GPIO_BASE_ADDR + 0) 
#define GPIO_PORT_REG			(GPIO_BASE_ADDR + 6)

#define GPIO_PORT_CONFIG		0xa2
//#define GPIO_PORT_CONFIG		0x82

#define GPIO_ENG_RST			0x01	// ENG_RST = active high out
#define GPIO_PUSH_BTN			0x02	// PUSH_BTN = active low in
#define GPIO_ENG_ONCE			0x04	// BUS_CTRL
#define GPIO_HOBBS_SCL			0x08	// HOBBS_SCL
#define GPIO_LED				0x10	// LED_GPIO - 1=on, 2=blink and dim out
#define GPIO_HOBBS_SDA			0x20	// HOBBS_SDA
#define GPIO_POWER_OFF			0x40	// POWER_OFF - active low out
#define GPIO_BIT7				0x80

typedef uint8_t	GpioImage_t;

uint8_t	PciRead( int handle, uint32_t device, uint32_t regIndex );
void	PciWrite( int handle, uint32_t device, uint32_t regIndex, uint8_t val );
bool	GpioInit( GpioImage_t * gpioImage );

#endif	// __GPIO_H_INCLUDED
