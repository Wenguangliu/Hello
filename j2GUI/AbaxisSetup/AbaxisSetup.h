#ifndef __ABAXISSETUP_H_INCLUDED
#define __ABAXISSETUP_H_INCLUDED

#ifndef __c_plus_plus
extern "C" {
#endif

void SetDeviceBacklight(int setting);
INT8 SetDeviceLedBlink();
INT8 SetDeviceLedSteadyOn();
void SetSoundVolume(unsigned char volume);

#ifndef __c_plus_plus
}
#endif

#endif	//	__ABAXISSETUP_H_INCLUDED