#ifndef __DOWNLOADENGINE_H_INCLUDED
#define __DOWNLOADENGINE_H_INCLUDED

#include "Gpio.h"

#define ENGINE_FIRMWARE_FILE	"/Abaxis/Engine/Engine.bin"

bool DownloadEngine( GpioImage_t * gpioImage );
bool Set196OnceMode( GpioImage_t * gpioImage, int onceMode );

#endif	// __DOWNLOADENGINE_H_INCLUDED
