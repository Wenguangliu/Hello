/*
 * FILENAME
 * 	SettingsData.h
 * 
 * MODULE DESCRIPTION
 * 	Defines a data structure and default values for the settings configuration
 * 	data stored in the settings configuration file.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __SETTINGSDATA_H_INCLUDED
#define __SETTINGSDATA_H_INCLUDED

#include "CommonDefines.h"

typedef enum
{
	COMM_NONE,
	COMM_ASCII,
	COMM_ASTM,
	COMM_XML,
} CommInterface_t;

typedef enum
{
	COMM_USB,
	COMM_TCPIP
} CommProtocol_t;

typedef struct
{
	CommInterface_t	interface;					// See CommInterface_t
	CommProtocol_t	protocol;					// See CommProtocol_t
	char			analyzerIp[30];
	char			gateway[30];
	char			ipMask[30];
	char			serverIp[30];
	int				serverSocket;
} CommSettings_t;

typedef enum
{
	ASTM_ORDERS_WORK_LIST,
	ASTM_ORDERS_ENTER_ID,
	ASTM_ORDERS_DISABLED
} AstmOrderMethod_t;

#define INTERNAL_PTR	0
#define EXTERNAL_PTR	1

typedef struct
{
	Language_t			language;						// See Language_t
	Language_t			keyboard;
	bool				time24Format;					// true = 24 hour format
	unsigned short		screenSaverTimeout;				// Time in minutes
	unsigned short		powerSaveTimeout;				// Time in minutes
	unsigned char		backlightLevel;					// 0 - 100%
	unsigned char		touchClickVolume;				// 0 - 100%
	unsigned char		alertVolume;					// 0 - 100%
	unsigned char		statusVolume;					// 0 - 100%
	CommSettings_t		comm;
	AstmOrderMethod_t	astmOrderMethod;
	bool				operatorIdEnabled;
	bool				analysisLockoutEnabled;
	bool				recallLockoutEnabled;
	bool				settingsLockoutEnabled;
	bool				sampleTypeAdvanced;
	bool				alternateIdEnabled;
	bool				doctorIdEnabled;
	bool				locationEnabled;
	bool				phoneNumberEnabled;
	bool				admissionOwnerIdEnabled;
	bool				dobEnabled;
	bool				dovEnabled;
	bool				sampleIdEnabled;
	bool				genderEnabled;
	bool				ageEnabled;
	bool				raceEnabled;
	bool				egfrEnabled;
	int					printNumResults;
	int					printNumIqc;
	int					printNumError;
	bool				errorPrintAuto;
	int					defaultPrinter;
} SettingsData_t;

const SettingsData_t defaultSettings = {
	LANG_ENGLISH,	// language
	LANG_ENGLISH,	// keyboard
	false,			// time24Format
	15,				// screenSaverTimeout
	30,				// powerSaveTimeout
	80,				// backlightLevel
	80,				// touchClickVolume
	80,				// alertVolume
	80,				// statusVolume
	{				// comm
		COMM_NONE,		// interface
		COMM_USB,		// protocol
		"192.1.10.30",	// analyzerIp
		"",				// gateway
		"255.255.0.0",	// ipMask
		"192.1.4.67",	// serverIp
		64000			// serverSocket
	},
	ASTM_ORDERS_DISABLED,	// astmOrderMethod
	false,			// operatorIdEnabled
	false,			// analysisLockoutEnabled
	false,			// recallLockoutEnabled;
	false,			// settingsLockoutEnabled;
	false,			// sampleTypeAdvanced
	false,			// alternateIdEnabled
	false,			// doctorIdEnabled
	false,			// locationEnabled
	false,			// phoneNumberEnabled
	false,			// admissionOwnerIdEnabled
	false,			// dobEnabled
	false,			// dovEnabled
	false,			// sampleIdEnabled
	false,			// genderEnabled
	false,			// ageEnabled
	false,			// raceEnabled
	false,			// egfrEnabled
	1,				// printNumResults
	0,				// printNumIqc
	1,				// printNumError
	true,			// errorPrintAuto
	INTERNAL_PTR	// defaultPrinter
};

#endif	// __SETTINGSDATA_H_INCLUDED

