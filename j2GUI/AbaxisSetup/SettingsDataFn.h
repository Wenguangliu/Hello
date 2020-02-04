#ifndef __SETTINGSDATAFN_H_INCLUDED
#define __SETTINGSDATAFN_H_INCLUDED

#ifndef __c_plus_plus
extern "C" {
#endif

void SetLanguage(Language_t language);
void GetLanguage(Language_t *pLanguage);

void SetKeyboard(Language_t language);
void GetKeyboard(Language_t *pLanguage);

void SetTimeFmt(bool timeFormat);
void GetTimeFmt(bool *ptimeFormat);

void SetScreenSaverTimeout(unsigned short Timeout);				// Time in minutes
void GetScreenSaverTimeout(unsigned short* Timeout);	
//unsigned short GetScreenSaverTimeout();

void SetPowerSaveTimeout(unsigned short Timeout);				// Time in minutes
void GetPowerSaveTimeout(unsigned short* Timeout);			
//unsigned short GetPowerSaveTimeout();

void SetBacklightLevel(unsigned char Level);					// 0 - 100%
void GetBacklightLevel(unsigned char* Level);				
//unsigned char GetBacklightLevel();

void SetTouchClickVolume(unsigned char Volume);				// 0 - 100%
void GetTouchClickVolume(unsigned char* pVolume);	

void SetAlertVolume(unsigned char Volume);					// 0 - 100%
void GetAlertVolume(unsigned char* pVolume);

void SetStatusVolume(unsigned char Volume);					// 0 - 100%
void GetStatusVolume(unsigned char* pVolume);

void SetComm(CommSettings_t CommSetting);
void GetComm(CommSettings_t *CommSetting);

void SetOperatorIdEnabled(bool Enabled);
void GetOperatorIdEnabled(bool* pEnabled);

void SetAnalysisLockoutEnabled(bool Enabled);
void GetAnalysisLockoutEnabled(bool* pEnabled);

void GetRecallLockoutEnabled(bool *pEnabled);
void SetRecallLockoutEnabled(bool Enabled);

void GetSettingsLockoutEnabled(bool *pEnabled);
void SetSettingsLockoutEnabled(bool Enabled);

void GetSampleTypeAdvanced(bool *pAdvanced);
void SetSampleTypeAdvanced(bool Advanced);

void GetAlternateIdEnabled(bool *pEnabled);
void SetAlternateIdEnabled(bool Enabled);

void GetDoctorIdEnabled(bool *pEnabled);
void SetDoctorIdEnabled(bool Enabled);

void GetLocationEnabled(bool *pEnabled);
void SetLocationEnabled(bool Enabled);

void GetPhoneNumberEnabled(bool *pEnabled);
void SetPhoneNumberEnabled(bool Enabled);

void GetAdmissionOwnerIdEnabled(bool *pEnabled);
void SetAdmissionOwnerIdEnabled(bool Enabled);

void GetDobEnabled(bool *pEnabled);
void SetDobEnabled(bool Enabled);

void GetDovEnabled(bool *pEnabled);
void SetDovEnabled(bool Enabled);

void GetSampleIdEnabled(bool *pEnabled);
void SetSampleIdEnabled(bool Enabled);

void GetGenderEnabled(bool *pEnabled);
void SetGenderEnabled(bool Enabled);

void GetAgeEnabled(bool *pEnabled);
void SetAgeEnabled(bool Enabled);

void GetRaceEnabled(bool *pEnabled);
void SetRaceEnabled(bool Enabled);

void GetEgfrEnabled(bool *pEnabled);
void SetEgfrEnabled(bool Enabled);




void GetPrintNumResults(int *pNumResults);
void SetPrintNumResults(int NumResults);

void GetPrintNumIqc(int *pNumIqc);
void SetPrintNumIqc(int NumIqc);

void GetPrintNumError(int *pNumError);
void SetPrintNumError(int NumError);

void GetErrorPrintAuto(bool *pAuto);
void SetErrorPrintAuto(bool Auto);

void GetDefaultPrinter(int *pPrinter);
void SetDefaultPrinter(int Printer);

#ifndef __c_plus_plus
}
#endif

#endif	//	__SETTINGSDATAFN_H_INCLUDED