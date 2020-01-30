#ifndef __SOUND_H
#define __SOUND_H

typedef enum
{
	SOUND_CRITICAL_STOP,
	SOUND_ERROR,
	SOUND_WARNING,
	SOUND_PRINTING_DONE,
	SOUND_NONE,
} Sound_t;

void	InitSound();
void	Click();
void	SetClickVolume( unsigned char volume );			// 0 - 100
void	SetAlertVolume( unsigned char volume );			// 0 - 100
void	SetStatusVolume( unsigned char volume );		// 0 - 100
void	PlaySound( Sound_t sound );


void	SoundChipTest();


#endif	// __SOUND_H
