#include <stdio.h>
#include <sys/mman.h>
#include <hw/inout.h>
#include <sys/neutrino.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

#include "Sound.h"
#include "Global.h"

#define BIT0		0x0001
#define BIT1		0x0002
#define BIT2		0x0004
#define BIT3		0x0008
#define BIT4		0x0010
#define BIT5		0x0020
#define BIT6		0x0040
#define BIT7		0x0080
#define BIT8		0x0100
#define BIT9		0x0200
#define BIT10		0x0400
#define BIT11		0x0800
#define BIT12		0x1000
#define BIT13		0x2000
#define BIT14		0x4000
#define BIT15		0x8000

/* sound chip register definition */
#define ROHM_COMMAND			0x00
#define ROHM_MIDI_VOL			0x01
#define ROHM_ADPCM_VOL			0x02
#define ROHM_INT_STATUS			0x03
#define ROHM_MIDI_FIFO_STATUS	0x04
#define ROHM_ADPCM_FIFO_STATUS	0x05
#define ROHM_TIMER_STATUS		0x06
#define ROHM_SEQU_CONTROL		0x07
#define ROHM_MIDI_FIFO_CONTROL	0x08
#define ROHM_ADPCM_FIFO_CONTROL	0x09
#define ROHM_MIDI_FIFO_WINDOW	0x0A
#define ROHM_ADPCM_FIFO_WINDOW	0x0B
#define ROHM_TIMER_REG_1		0x0C
#define ROHM_TIMER_REG_2		0x0D
#define ROHM_INTERVAL_REG		0x0E
#define ROHM_CALLBACK_STATUS	0x0F
#define ROHM_WAVE_SELECTION		0x10
#define ROHM_TONE				0x11
#define ROHM_MD_SCALE			0x12
#define ROHM_PITCH_HIGH_BYTE	0x13
#define ROHM_PITCH_LOW_BYTE		0x14
#define ROHM_TONE_LEFT_VOL		0x15
#define ROHM_TONE_RIGHT_VOL		0x16
#define ROHM_TONE_VIB			0x17
#define ROHM_TONE_PLAY			0x18
#define ROHM_LED				0x19
#define ROHM_PLL_SETUP			0x1B
#define ROHM_ADPCM_RATE			0x1C

/*
 * sound chip register image
 */
typedef struct {
	uint8_t	 command;
	uint8_t	 midi_vol;
	uint8_t	 adpcm_vol;
	uint8_t	 int_status;
	uint8_t	 midi_fifo_status;
	uint8_t	 adpcm_fifo_status;
	uint8_t	 timer_status;
	uint8_t	 sequ_control;
	uint8_t	 midi_fifo_control;
	uint8_t	 adpcm_fifo_control;
	uint8_t	 midi_fifo_window;
	uint8_t	 adpcm_fifo_windwo;
	uint8_t	 timer1_reg;
	uint8_t	 timer2_reg;
	uint8_t	 interval_reg;
	uint8_t	 callback_status;
	uint8_t	 wave_selection;
	uint8_t	 led;
	uint8_t	 pll_setup;
	uint8_t	 adpcm_rate;
} SoundChipRegImage_t;


typedef enum {
	SOUND_IDLE,
	SOUND_PLAY,
	SOUND_STOP,
} State_t;

typedef enum {
	NO_TYPE,
	ADPCM_TYPE,
	MIDI_TYPE
} PlayType_t;


typedef struct {
	State_t		state;
	PlayType_t	type;
} SoundState_t;

#define SOUND_INT_NUM		12
#define MIDI_DATA_LOAD_LEN	510
#define ADPCM_DATA_LOAD_LEN	120

#define TIMER_INT_BIT		BIT0
#define ADPCM_INT_BIT		BIT1
#define MID_INT_BIT			BIT2

#define FIFO_EMPTY			BIT7
#define FIFO_NE_EMPTY		BIT6
#define FIFO_NE_FULL		BIT5
#define FIFO_LED			BIT4


static void		InitClick();
static void		InitSoundChip();
static void *	InterruptThread( void * data );
static void		SetAdpcmVolume( uint8_t volume );
static void		SetMidiVolume( uint8_t volume );
static void *	SoundControlThread(void *arg);
static int		OpenSoundFile( const char *fileName );
static void		WriteSoundChipReg( uint8_t reg, uint8_t  value );
static uint8_t	ReadSoundChipReg( uint8_t  reg );
static int 		ReadSoundFile( char *buf, unsigned int len );
static void		StartAdpcm();
static void		StopAdpcm();
//static void	StartMidi();
static void		StopMidi();
static void		LoadAdpcmSoundData();
static void		LoadMidiSoundData();
static void		ReloadAdpcmSoundData();
static void		ReloadMidiSoundData();

static SoundChipRegImage_t	soundChipRegImage;
static SoundState_t			soundState;
static sem_t				soundSem;
static uint8_t				clickVolume;
static uint8_t				alertVolume;
static uint8_t				statusVolume;
static FILE *				fp;

void
InitSound()
{
    InitSoundChip();
    InitClick();
}

void
Click()
{
	if ( clickVolume > 0 )
	{
		SetMidiVolume( clickVolume );
		WriteSoundChipReg( ROHM_PITCH_LOW_BYTE, 0x50 );
		WriteSoundChipReg( ROHM_TONE_PLAY, 0x80 );
	}

	// Let the screensaver/power-save monitor know there's been action.
	pthread_sleepon_lock();
	inactionTime_g = 0;
	pthread_sleepon_unlock();
}

void
PlaySound( Sound_t sound )
{
	const char *	fName = "";
	
	if ( soundState.state != SOUND_IDLE )
	{
		if ( soundState.type == ADPCM_TYPE )
		{
			StopAdpcm();
		}
		else
		{
			StopMidi();
		}
	}
		
	switch( sound )
	{
		case SOUND_CRITICAL_STOP:
			if ( alertVolume == 0 )
			{
				return;
			}
			SetAdpcmVolume( alertVolume );
			fName = "/Abaxis/Sound/Critical_Stop_8k.pcm";
			break;
		case SOUND_ERROR:
			if ( alertVolume == 0 )
			{
				return;
			}
			SetAdpcmVolume( alertVolume );
			fName = "/Abaxis/Sound/Error_8k.pcm";
			break;
		case SOUND_WARNING:
			if ( alertVolume == 0 )
			{
				return;
			}
			SetAdpcmVolume( alertVolume );
			fName = "/Abaxis/Sound/Warning_8k.pcm";
			break;
		case SOUND_PRINTING_DONE:
			if ( statusVolume == 0 )
			{
				return;
			}
			SetAdpcmVolume( statusVolume );
			fName = "/Abaxis/Sound/Printing_Done_8k.pcm";
			break;
		
		default:
			return;
	}
			
	if ( OpenSoundFile( fName ) != -1 )
	{
		LoadAdpcmSoundData();
		StartAdpcm();
	}
	
	// Let the screensaver/power-save monitor know there's been action.
	pthread_sleepon_lock();
	inactionTime_g = 0;
	pthread_sleepon_unlock();
}

void
SetClickVolume( unsigned char volume )
{
	// Convert from range 0-100 to range 0-0xFF.
	clickVolume = (unsigned char)(volume * 2.55);
}

void
SetAlertVolume( unsigned char volume )
{
	// Convert from range 0-100 to range 0-0x7F.
	alertVolume = (unsigned char)(volume * 1.27);
}

void
SetStatusVolume( unsigned char volume )
{
	// Convert from range 0-100 to range 0-0x7F.
	statusVolume = (unsigned char)(volume * 1.27);
}

/*
 * write to sound chip register 
 */
void
WriteSoundChipReg(uint8_t reg, uint8_t value)
{
	uintptr_t	handle;

	if ((handle = mmap_device_io(8, 0x0228)) == MAP_DEVICE_FAILED) {
		printf("mmap_device_io PCI failed \n");
		return;
	}
	
	if (ThreadCtl(_NTO_TCTL_IO, 0) == -1) {
		printf("Fail to get memory control privilege\n");
		return;
	}
	out8(handle + 0, reg);
	out8(handle + 1, value);
	return;
} 

/* 
 * read from sound chip register
 */
uint8_t 
ReadSoundChipReg(uint8_t reg)
{
	uint8_t		value = 0;
	uintptr_t	handle;
	
	if ((handle = mmap_device_io(8, 0x0228)) == MAP_DEVICE_FAILED) {
		printf("mmap_device_io PCI failed \n");
	}
	
	if (ThreadCtl(_NTO_TCTL_IO, 0) == -1) {
		printf("Fail to get memory control privilege\n");
	}

	out8(handle + 0, reg);
	value = in8(handle + 1);
	return value;
}




/*
 * init keytone for touch button
 * paramter volume :	Initialize volume which needs to
 *					  be saved in xml file. Everytime system
 *					  bootup, this value need to be read from
 *					  xml file and set by initilization
 *					  tone volme value : 0 - 0x3F
 *
 */

void
InitClick()
{
	WriteSoundChipReg( ROHM_TONE, 0x0D );
	WriteSoundChipReg( ROHM_MD_SCALE, 0x3F );
	WriteSoundChipReg( ROHM_PITCH_HIGH_BYTE, 0x03 );
	WriteSoundChipReg( ROHM_PITCH_LOW_BYTE, 0x50 );

	WriteSoundChipReg( ROHM_TONE_LEFT_VOL, 0x3F );
	WriteSoundChipReg( ROHM_TONE_RIGHT_VOL, 0x3F );
	WriteSoundChipReg( ROHM_TONE_VIB, 0x00 );
}




/*
 *  Initial sound chip. 
 *  this function must be called during system initialization
 *  paramter: volume -- Initialize volume which needs to 
 *					  be saved in xml file. Everytime system
 *					  bootup, this value need to be read from
 *					  xml file and set by initilization
 */

void
InitSoundChip()
{
	uint8_t regValue;

	/* initial semphore */
	sem_init(&soundSem, 0 ,0);

	memset(&soundChipRegImage, 0, sizeof(SoundChipRegImage_t));
	soundState.state = SOUND_IDLE;

	fp = NULL;
	/* make sure , pll stable */
	delay(20);

	/* 
	 * set up pll value 
	 * MCLK = 12 M Hz, n = 0x06, m = 0x1a; 
	 * sys_clk(PLL output) = 13 M Hz
	 * n/2 = 0x03 m/2 = 0x0D 
	 */
	regValue = 0x6D;
	WriteSoundChipReg(ROHM_PLL_SETUP, regValue);
	soundChipRegImage.pll_setup = regValue;

	/* wait for PLL stable > 10ms */
	delay(20);

	/* 
	 * reset and unreset firmware 
	 */ 
	regValue = 0x80;
	WriteSoundChipReg(ROHM_SEQU_CONTROL, regValue);
	regValue = 0x00;
	WriteSoundChipReg(ROHM_SEQU_CONTROL, regValue);
	soundChipRegImage.sequ_control = regValue;

	/* interrupt active high */
	WriteSoundChipReg(ROHM_INT_STATUS, 0x80);


	/*
	 * set command register
	 * SE mode	bit7 - 6 = 00	Use Wave Gen 31 - 0
	 * SFmode	bit4	 = 0	ADPCM sampling rate = 8 K 
	 * PMode	bit3	 = 0	Serial port mode (not used for us)
	 * AN1/2	Bit2	 = 0	Input data formate A 
	 * Mono		Bit1	 = 1	Monophonic
	 * Mute		Bit0	 = 0	mute the sound
	 */

	/* wliu  we may never need to mute the sound chip */
	regValue = BIT1;
	WriteSoundChipReg(ROHM_COMMAND, regValue);
	soundChipRegImage.command = regValue;

	/* 
	 * ADPCM rate and MIDI time set
	 * MIDI sys_clk = 13M
	 * MIDI time 1 = 0x66
	 *	    time 2 = 0x27 
	 * ADPCM rate = 0x1E
	 */
	regValue = 0x66;
	WriteSoundChipReg(ROHM_TIMER_REG_1, regValue);
	soundChipRegImage.timer1_reg = regValue;
	
	regValue = 0x27;
	WriteSoundChipReg(ROHM_TIMER_REG_2, regValue);
	soundChipRegImage.timer2_reg = regValue;

	regValue = 0x1E;
	WriteSoundChipReg(ROHM_ADPCM_RATE, regValue);
	soundChipRegImage.adpcm_rate = regValue;

	/* 
	 * set FIFO control 
	 * set almost full, FIFO 15/16 filled
	 * set almost empty, FIFO 7/8 empty 
	 */

	regValue = 0x82;
	WriteSoundChipReg(ROHM_MIDI_FIFO_CONTROL, regValue);
	WriteSoundChipReg(ROHM_ADPCM_FIFO_CONTROL, regValue);
	soundChipRegImage.midi_fifo_control = regValue;
	soundChipRegImage.adpcm_fifo_control = regValue;

	/*
	 * clear all interrupt bit 
	 * clear FIFO 
	 */
	regValue = 0xFE;
	WriteSoundChipReg(ROHM_MIDI_FIFO_STATUS, regValue);
	WriteSoundChipReg(ROHM_ADPCM_FIFO_STATUS, regValue);
	soundChipRegImage.midi_fifo_status = 0x00;
	soundChipRegImage.adpcm_fifo_status = 0x00;

	pthread_create( NULL, NULL, InterruptThread, NULL );
	pthread_create( NULL, NULL, SoundControlThread, NULL );
}


/*
 * Set ADPCM volume
 * volume 0 - 0x7F
 */
void
SetAdpcmVolume( uint8_t volume )
{
	uint8_t	 regValue;

	/* set ADPCM right volume */
	regValue = (0x80 | volume) ;
	WriteSoundChipReg(ROHM_ADPCM_VOL, regValue);

	/* set ADPCM left volume */
	regValue &= 0x7F;
	WriteSoundChipReg(ROHM_ADPCM_VOL, regValue);
}


/*
 *  Set MIDI volue
 *  Tone sound is MIDI, this volume will
 *  set tone voluem also.
 *  volume 0 - 0xFF
 */
void
SetMidiVolume(uint8_t volume)
{
	WriteSoundChipReg(ROHM_MIDI_VOL, volume);
}

/*
 * Play ADPCM file here
 */
void
StartAdpcm()
{


	soundState.state = SOUND_PLAY;
	soundState.type = ADPCM_TYPE;
	/* wliu may need to cancel mute here */

	WriteSoundChipReg(ROHM_ADPCM_FIFO_STATUS, 0x01);
	soundChipRegImage.adpcm_fifo_status = 0x01;

}

/* 
 * Stop Play ADPCM file
 */
void
StopAdpcm()
{
	uint8_t regValue;

	
	/* wliu may need to enable mute here */

	/* disable ADPCM play */
	regValue = soundChipRegImage.adpcm_fifo_status;
	WriteSoundChipReg(ROHM_ADPCM_FIFO_STATUS, regValue);

	/* reset FIOF and all interrupt bits */
	WriteSoundChipReg(ROHM_ADPCM_FIFO_STATUS, 0xFE);

	regValue = soundChipRegImage.sequ_control | 0x80;
	WriteSoundChipReg(ROHM_SEQU_CONTROL, regValue);
	regValue = soundChipRegImage.sequ_control;
	WriteSoundChipReg(ROHM_SEQU_CONTROL, regValue);

	// GLD
	fclose(fp);
	fp = NULL;

	soundState.state = SOUND_IDLE;
}

/*
 * start to play MIDI file
 */
/*
void
StartMidi()
{


	soundState.state = SOUND_PLAY;
	soundState.type = MIDI_TYPE;
	
	WriteSoundChipReg(ROHM_MIDI_FIFO_STATUS, 0x01);
	soundChipRegImage.midi_fifo_status = 0x01;
}
*/

/*
 * Stop play MIDI file
 */
void
StopMidi()
{
	uint8_t regValue;
	
	/* wliu may need to enable mute here */

	/* disable ADPCM play */
	regValue = soundChipRegImage.midi_fifo_status;
	WriteSoundChipReg(ROHM_MIDI_FIFO_STATUS, regValue);

	/* reset FIOF and all interrupt bits */
	WriteSoundChipReg(ROHM_MIDI_FIFO_STATUS, 0xFE);


	/* reset firmware */
	regValue = soundChipRegImage.sequ_control | 0x80;
	WriteSoundChipReg(ROHM_SEQU_CONTROL, regValue);
	regValue = soundChipRegImage.sequ_control;
	WriteSoundChipReg(ROHM_SEQU_CONTROL, regValue);

	// GLD
	fclose(fp);
	fp = NULL;

	soundState.state = SOUND_IDLE;
}


/*
 * open a sound file
 */
int
OpenSoundFile( const char *fileName )
{
	if ( fp != NULL )
	{
		printf( "Sound file already be open\n" );
		fclose( fp );
	}

	fp = fopen( fileName, "r" );

	if ( fp == NULL )
	{
		printf( "Open file %s fail\n", fileName );
		return -1;
	}
	
	return 0;
}

/*
 * read sound file data into buffer
 */

int
ReadSoundFile(char *buff, unsigned int len)
{
	int		 rLen;

	if (fp == NULL) {
		printf("Sound file is not open\n");
		return -1;
	}

	/* read sound file */
	rLen = fread(buff, 1, len, fp);

	if (ferror(fp))
	{
		fclose( fp );
		printf("Read sound file error\n");
		return -1;
	}
	if (feof(fp)) {
		fclose(fp);
		fp = NULL;
		soundState.state = SOUND_STOP;
	} 

	return rLen;
}

/* 
 * Re-load ADPCM sound data
 * this function must to be called in isr
 */
void
ReloadAdpcmSoundData()
{
	
	/* no more data to be read */
	if (soundState.state != SOUND_PLAY) {
		return;
	}
	LoadAdpcmSoundData();
}

/* 
 *  Re-load MIDI sound data
 *  this function need to be called in isr
 */
void
ReloadMidiSoundData()
{
	
	/* no more data to be read */
	if (soundState.state != SOUND_PLAY) {
		return;
	}
	LoadMidiSoundData();
}

/*
 * load ADPCM data into sound chip FIFO
 */
void
LoadAdpcmSoundData()
{
	int			len;
	char		buff[512];
	char *		ptr;
	uintptr_t	handle;

	if ((handle = mmap_device_io(8, 0x0228)) == MAP_DEVICE_FAILED) {
		printf("mmap_device_io PCI failed \n");
		return;
	 }

	if (ThreadCtl(_NTO_TCTL_IO, 0) == -1) {
		printf("Fail to get memory control privilege\n");
		return;
	}

	ptr = buff; 

	len = ReadSoundFile(buff, ADPCM_DATA_LOAD_LEN);

	out8(handle + 0, ROHM_ADPCM_FIFO_WINDOW);

	if (len != -1) {
		while (len--) {
			out8(handle + 1, *ptr++);
		}
	}

	/* clear almost full interrupt */
	out8(handle + 0, ROHM_ADPCM_FIFO_STATUS);
	out8(handle + 1, (soundChipRegImage.adpcm_fifo_status | 0x2));

}

/*
 * load MIDI data into sound chip FIFO
 */

void
LoadMidiSoundData()
{

	int			len;
	char		buff [512];
	char *		ptr;
	uintptr_t	handle;

	if ((handle = mmap_device_io(8, 0x0228)) == MAP_DEVICE_FAILED) {
		printf("mmap_device_io PCI failed \n");
		return;
	 }

	if (ThreadCtl(_NTO_TCTL_IO, 0) == -1) {
		printf("Fail to get memory control privilege\n");
		return;
	}


	ptr = &buff [0]; 

	len = ReadSoundFile(buff, MIDI_DATA_LOAD_LEN);

	out8(handle +0, ROHM_MIDI_FIFO_WINDOW);
	
	 if (len != -1) {
		while (len--) {
			out8(handle + 1, *ptr++);
		}
	}

	/* clear almost full interrupt */
	out8(handle + 0, ROHM_MIDI_FIFO_STATUS);
	out8(handle + 1, (soundChipRegImage.midi_fifo_status | 0x20));
}

/*
 * create a thread to handle interrupt
 */
void *
InterruptThread(void *data)
{
	struct sigevent event;
	int			 id;
	uint8_t		 regValue, intStatus;

	ThreadCtl(_NTO_TCTL_IO, NULL);

	/* fill event struct */
	memset(&event, 0, sizeof(event));

	event.sigev_notify = SIGEV_INTR;

	/* attach interrtup event */
	id = InterruptAttachEvent(SOUND_INT_NUM, &event, 0);

	/* enable interrupt */
	InterruptUnmask(SOUND_INT_NUM, id);

	/* code to process interrupt */
	while (1) {
		/* wait for interrupt */
		InterruptWait(NULL, NULL);

		/* get interrupt source */
		regValue = ReadSoundChipReg(ROHM_INT_STATUS);
		/* ADPCM interrupt */
		if (regValue & ADPCM_INT_BIT) {
			if (soundState.state != SOUND_IDLE) {
				intStatus = ReadSoundChipReg(ROHM_ADPCM_FIFO_STATUS);
                /* sound finished, send sem to thread to stop play */
                if ((intStatus & FIFO_EMPTY) && (soundState.state == SOUND_STOP)) {
                   sem_post (&soundSem);
                } else if ((intStatus & FIFO_NE_EMPTY) || (intStatus & FIFO_EMPTY)) {
                   /* reload sound file */
                   ReloadAdpcmSoundData ();
                }
            }
            /* clear all interrupt */
            WriteSoundChipReg (ROHM_ADPCM_FIFO_STATUS, 0xF0);
        } else if (regValue & MID_INT_BIT) {
            if (soundState.state != SOUND_IDLE) {
                intStatus = ReadSoundChipReg (ROHM_MIDI_FIFO_STATUS);
                /* sound finished, send sem to thread to stop play */
                if ((intStatus & FIFO_EMPTY) && (soundState.state == SOUND_STOP)) {
                   sem_post (&soundSem);
                } else if ((intStatus & FIFO_NE_EMPTY) || (intStatus & FIFO_EMPTY)) {
                   /* reload sound file */
                   ReloadMidiSoundData ();
                }
            }
			/* clear interrupt */
			WriteSoundChipReg(ROHM_MIDI_FIFO_STATUS, 0xF0);
		} else if (regValue & TIMER_INT_BIT) {
			intStatus = ReadSoundChipReg(ROHM_TIMER_STATUS); 
			
			/* clear interrupt */
			WriteSoundChipReg(ROHM_TIMER_STATUS, intStatus * 0x03); 
		
		}

		/* re-enable interrupt */
		InterruptUnmask(SOUND_INT_NUM, id);
	}
}


/*
 * thread to detect busy bit is 0, then stop play
 */
void *
SoundControlThread(void *arg)
{

	uint8_t fifoStatus;

	while (1) {
		/* waiting for semphore from interrupt thread*/
		sem_wait(&soundSem);
		while (soundState.state != SOUND_IDLE) {
			/* check if not busy, reset firmware, stop play */
			if (soundState.type == ADPCM_TYPE) {
				fifoStatus = ReadSoundChipReg(ROHM_ADPCM_FIFO_STATUS);
				if ((fifoStatus & BIT0) == 0) {
					StopAdpcm();
				}
			} else {
				fifoStatus = ReadSoundChipReg(ROHM_MIDI_FIFO_STATUS);
				if ((fifoStatus & BIT0) == 0) {
					StopMidi();
				}
			}
			/* wait 10ms and test again */
			delay(10);
		}
	}
}

