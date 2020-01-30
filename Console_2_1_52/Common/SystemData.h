#ifndef __SYSTEMDATA_H_INCLUDED
#define __SYSTEMDATA_H_INCLUDED

typedef enum
{
	SYSTEM_STATUS_BUSY = 0,		// Default value is Busy.
	SYSTEM_STATUS_READY,
	SYSTEM_STATUS_NOT_READY,
} SystemStatus_t;

typedef struct
{
	bool			rotorPresent;			// Rotor present at start-up.
	bool			waitForHeater;			// Analysis waits for heater to warm up the system.
	bool			inManufacturingMode;	// Show the manufacturing icon.
	bool			inDemoMode;
	bool			inResearchMode;
	bool			inServiceMode;
	bool			inAdministratorMode;
	int				burnInDrawerOpen;		// Time to leave drawer open between cycles in Rotor 1 burn-in.
	int				researchDrawerOpen;		// Time to leave drawer open before automatically closing when in Research mode.
	bool			keepWindowManager;		// Keep the window manager running.
	bool			demoFromKey;			// Get the demo file from the key drive.
	bool			useFastDemo;			// Use the "fast" demo RIS file.
	bool			enableFtpTelnet;		// Enable ftp and telnet access.
	int				commPid;				// PID for the comm process (SerialComm or ASTM).
	unsigned long	max4214Errors;			// Maximum # of 4214 (SERR_WAVE_CV_UNREF) errors before stopping when running Rotor 1
	SystemStatus_t	systemStatus;			// Used by ASTM 'M' queries for system status.
} SystemData_t;

#endif	// __SYSTEMDATA_H_INCLUDED
