#include "stdafx.h"

//j2Common.lib include directories
#include "SettingsData.h"
#include "FactoryData.h"
#include "Hardware.h"
#include "Eeprom.h"
#include "Gpio.h"

#include "SettingsDataFn.h"
#include "FactoryDataFn.h"
#include "SystemData.h"
#include "AbaxisSetup.h"

extern SettingsData_t*	g_pSettings;
extern FactoryData_t*   g_pFactory;
extern SystemData_t*    g_pSystemData;

//	Language_t		language;
void SetLanguage(Language_t language)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->language = language;
	}
}
void GetLanguage(Language_t *pLanguage)
{
	if(pLanguage != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pLanguage = g_pSettings->language;
		}
		else
		{
			*pLanguage = NUM_LANGUAGES;	//	We will have to treat this as error on the other side
		}
	}
}

//	Language_t		keyboard;
void SetKeyboard(Language_t language)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->keyboard = language;
	}
}
void GetKeyboard(Language_t *pLanguage)
{
	if(pLanguage != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pLanguage = g_pSettings->keyboard;
		}
		else
		{
			*pLanguage = NUM_LANGUAGES;	//	We will have to treat this as error on the other side
		}
	}
}
	
//	bool			time24Format;					// true = 24 hour format
void SetTimeFmt(bool timeFormat)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->time24Format = timeFormat;
	}
}
void GetTimeFmt(bool *ptimeFormat)
{
	if(ptimeFormat != NULL)
	{
		if(g_pSettings != NULL)
		{
			*ptimeFormat = g_pSettings->time24Format;
		}
		else
		{
			*ptimeFormat = false;	//	We will have to treat this as error on the other side
		}
	}
}

//	unsigned short	screenSaverTimeout;				// Time in minutes
void SetScreenSaverTimeout(unsigned short Timeout)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->screenSaverTimeout = Timeout;
	}
}
void GetScreenSaverTimeout(unsigned short* pTimeout)
{
	if(pTimeout != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pTimeout = g_pSettings->screenSaverTimeout;
		}
		else
		{
			*pTimeout = 0;
		}
	}
}

void SetPowerSaveTimeout(unsigned short Timeout)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->powerSaveTimeout = Timeout;		
	}
}
void GetPowerSaveTimeout(unsigned short* pTimeout)
{
	if(pTimeout != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pTimeout = g_pSettings->powerSaveTimeout;			
		}
		else
		{
			*pTimeout = 0;
		}
	}
}

//	unsigned char	backlightLevel;					// 0 - 100%
void SetBacklightLevel(unsigned char Level)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->backlightLevel = Level;
	}
}
void GetBacklightLevel(unsigned char* pLevel)
{
	if(pLevel != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pLevel = g_pSettings->backlightLevel;
		}
		else
		{
			*pLevel = 0;
		}
	}
}

//	unsigned char	touchClickVolume;				// 0 - 100%
void SetTouchClickVolume(unsigned char Volume)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->touchClickVolume = Volume;
	}
}
void GetTouchClickVolume(unsigned char* pVolume)
{
	if(pVolume != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pVolume = g_pSettings->touchClickVolume;
		}
		else
		{
			*pVolume = 0;			
		}
	}
}

//	unsigned char	alertVolume;					// 0 - 100%
void SetAlertVolume(unsigned char Volume)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->alertVolume = Volume;
	}
}
void GetAlertVolume(unsigned char* pVolume)
{
	if(pVolume != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pVolume = g_pSettings->alertVolume;
		}
		else
		{
			*pVolume = 0;
		}
	}
}

//	unsigned char	statusVolume;					// 0 - 100%
void SetStatusVolume(unsigned char Volume)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->statusVolume = Volume;
	}
}
void GetStatusVolume(unsigned char* pVolume)
{
	if(pVolume != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pVolume = g_pSettings->statusVolume;
		}
		else
		{
			*pVolume = 0;
		}
	}
}

//	CommSettings_t	comm;
void SetComm(CommSettings_t CommSetting)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->comm = CommSetting;
	}
}
void GetComm(CommSettings_t *pCommSetting)
{
	if(pCommSetting != NULL)
	{
		if(g_pSettings != NULL)
		{
			memcpy((PVOID)pCommSetting, (PVOID)&g_pSettings->comm, sizeof(CommSettings_t));
		}
		else
		{
			memset((PVOID)pCommSetting, 0, sizeof(CommSettings_t));
		}
	}
}

//	bool			operatorIdEnabled;
void SetOperatorIdEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->operatorIdEnabled = Enabled;
	}
}
void GetOperatorIdEnabled(bool* pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->operatorIdEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}

//	bool			analysisLockoutEnabled;
void SetAnalysisLockoutEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->analysisLockoutEnabled = Enabled;
	}
}
void GetAnalysisLockoutEnabled(bool* pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->analysisLockoutEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}


/****************************************************/
//	bool			recallLockoutEnabled;
void GetRecallLockoutEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->recallLockoutEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetRecallLockoutEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->recallLockoutEnabled = Enabled;
	}
}

//	bool			settingsLockoutEnabled;
void GetSettingsLockoutEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->settingsLockoutEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetSettingsLockoutEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->settingsLockoutEnabled = Enabled;
	}
}

//	bool			sampleTypeAdvanced;
void GetSampleTypeAdvanced(bool *pAdvanced)
{
	if(pAdvanced != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pAdvanced = g_pSettings->sampleTypeAdvanced;
		}
		else
		{
			*pAdvanced = false;
		}
	}
}
void SetSampleTypeAdvanced(bool Advanced)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->sampleTypeAdvanced = Advanced;
	}
}

//	bool			alternateIdEnabled;
void GetAlternateIdEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->alternateIdEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetAlternateIdEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->alternateIdEnabled = Enabled;
	}
}

//	bool			doctorIdEnabled;
void GetDoctorIdEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->doctorIdEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetDoctorIdEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->doctorIdEnabled = Enabled;
	}
}

//	bool			locationEnabled;
void GetLocationEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->locationEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetLocationEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->locationEnabled = Enabled;
	}
}

//	bool			phoneNumberEnabled;
void GetPhoneNumberEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->phoneNumberEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetPhoneNumberEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->phoneNumberEnabled = Enabled;
	}
}

//	bool			admissionOwnerIdEnabled;
void GetAdmissionOwnerIdEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->admissionOwnerIdEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetAdmissionOwnerIdEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->admissionOwnerIdEnabled = Enabled;
	}
}

//	bool			dobEnabled;
void GetDobEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->dobEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetDobEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->dobEnabled = Enabled;
	}
}

//	bool			dovEnabled;
void GetDovEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->dovEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetDovEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->dovEnabled = Enabled;
	}
}

//	bool			sampleIdEnabled;
void GetSampleIdEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->sampleIdEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetSampleIdEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->sampleIdEnabled = Enabled;
	}
}

//	bool			genderEnabled;
void GetGenderEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->genderEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetGenderEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->genderEnabled = Enabled;
	}
}

//	bool			ageEnabled;
void GetAgeEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->ageEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetAgeEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->ageEnabled = Enabled;
	}
}

//	bool			raceEnabled;
void GetRaceEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->raceEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetRaceEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->raceEnabled = Enabled;
	}
}

//	bool			egfrEnabled;
void GetEgfrEnabled(bool *pEnabled)
{
	if(pEnabled != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pEnabled = g_pSettings->egfrEnabled;
		}
		else
		{
			*pEnabled = false;
		}
	}
}
void SetEgfrEnabled(bool Enabled)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->egfrEnabled = Enabled;
	}
}

//	int				printNumResults;
void GetPrintNumResults(int *pNumResults)
{
	if(pNumResults != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pNumResults = g_pSettings->printNumResults;
		}
		else
		{
			*pNumResults = -1;
		}
	}
}
void SetPrintNumResults(int NumResults)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->printNumResults = NumResults;
	}
}

//	int				printNumIqc;
void GetPrintNumIqc(int *pNumIqc)
{
	if(pNumIqc != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pNumIqc = g_pSettings->printNumIqc;
		}
		else
		{
			*pNumIqc = -1;
		}
	}
}
void SetPrintNumIqc(int NumIqc)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->printNumIqc = NumIqc;
	}
}

//	int				printNumError;
void GetPrintNumError(int *pNumError)
{
	if(pNumError != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pNumError = g_pSettings->printNumError;
		}
		else
		{
			*pNumError = -1;
		}
	}
}
void SetPrintNumError(int NumError)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->printNumError = NumError;
	}
}

//	bool			errorPrintAuto;
void GetErrorPrintAuto(bool *pAuto)
{
	if(pAuto != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pAuto = g_pSettings->errorPrintAuto;
		}
		else
		{
			*pAuto = false;
		}
	}
}
void SetErrorPrintAuto(bool Auto)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->errorPrintAuto = Auto;
	}
}

//	int				defaultPrinter;
void GetDefaultPrinter(int *pPrinter)
{
	if(pPrinter != NULL)
	{
		if(g_pSettings != NULL)
		{
			*pPrinter = g_pSettings->defaultPrinter;
		}
		else
		{
			*pPrinter = -1;
		}
	}
}
void SetDefaultPrinter(int Printer)
{
	if(g_pSettings != NULL)
	{
		g_pSettings->defaultPrinter = Printer;
	}
}

void GetAbaxisProductCode(char *pProductCode)
{
	if(pProductCode != NULL)
	{
		if(g_pFactory != NULL)
		{
			*pProductCode = g_pFactory->abaxisProductCode;
		}
		else
		{
			*pProductCode = -1;
		}
	}
}

//Setting Backlight and LED for j2Gui Screen Saver functionality
void SetDeviceBacklight(int setting)
{
    LcdBackLightSet (g_pSystemData->cpuBoard, setting);
}

INT8 SetDeviceLedBlink ()
{
	return(SetLedBlink ());
}

INT8 SetDeviceLedSteadyOn ()
{	
	return (SetLedSteadyOn ());
}

// Sets the sound volume
// The sound volume is 8 bit register for both left and right channel
// notice: Volume = 0 does not affect anything, the min soulld be 1
void
SetSoundVolume (unsigned char volume)
{
    unsigned int fullVolume, newVolume;

    //somehow the most of high four bits must be F (Don't know the reason)
    //set max value to be A to prevent speaker noise
    newVolume = (unsigned int)(volume /100.0 * 0x0A) | 0xF000;
    //printf ("voluem =%d newVolume = %x\n\r", volume, newVolume);
    //Setting the same volume to both the high-order word and low-order word.
    fullVolume = (((unsigned int)newVolume & 0x0000ffff) | ((unsigned int)newVolume << 16));
    waveOutSetVolume (NULL, fullVolume);
}
