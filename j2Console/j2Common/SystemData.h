#ifndef __SYSTEMDATA_H_INCLUDED
#define __SYSTEMDATA_H_INCLUDED

typedef enum
{
    SYSTEM_STATUS_BUSY = 0,     // Default value is Busy.
    SYSTEM_STATUS_READY,
    SYSTEM_STATUS_NOT_READY,
} SystemStatus_t;


typedef struct
{
	unsigned char	rotorPresent;			// Rotor present at start-up.
	unsigned char	waitForHeater;			// Analysis waits for heater to warm up the system.
	unsigned char	inManufacturingMode;	// Show the manufacturing icon.
	unsigned char	inDemoMode;
	unsigned char	inResearchMode;
	unsigned char	inServiceMode;
	unsigned char	inAdministratorMode;
    unsigned char   inRandDMode;            // R&D debug mode
	unsigned char	keepWindowManager;		// Keep the window manager running.
	unsigned char	enableFtpTelnet;		// Enable ftp and telnet access.
    unsigned char   rdEnableFtpTelnet;      // Enable ftp and telnet access by R&D key 
    unsigned char	engSoftwareUpdate;      // update engine software 
    //unsigned char   systemStatus;           // analysisStatus
	unsigned char	cpuBoard;			    // CUP board 
	int		        burnInDrawerOpen;	    // Time to leave drawer open between cycles in Rotor 1 burn-in.
    int             researchDrawerOpen;     // Time to leave drawer open before automatically closing when in Research mode.
	unsigned long	commPid;				// PID for the comm process (SerialComm or ASTM).
    unsigned long   max4214Errors;          // Maximum # of 4214 (SERR_WAVE_CV_UNREF) errors before stopping when running Rotor 1
    unsigned char   dac10BitsFlag;          // 10 bits or 8 bits DAC Engine board
    unsigned char   engDebugPortEanble;     // Engine board debug port enable
    unsigned char   demoFromKey;            // Demo file read from  USB key
    unsigned char   useFastDemo;            // Use the "fast" demo RIS file 
    unsigned char   networkSetting;         // is setting network
    SystemStatus_t  systemStatus;           // Used by ASTM 'M' queries for system status.
    unsigned long   maxDataBaseSize;     // max allowed database size  


} SystemData_t;


#endif	// __SYSTEMDATA_H_INCLUDED
