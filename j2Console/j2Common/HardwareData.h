#ifndef __HARDWAREDATA_H_INCLUDED
#define __HARDWAREDATA_H_INCLUDED

typedef struct
{
	char	data[21];
	char	date[21];
} HardwareItem_t;

typedef struct
{
	struct
	{
		HardwareItem_t	enginePcb;
//		HardwareItem_t	daughterPcb;
//		HardwareItem_t	bezel;
//		HardwareItem_t	cdrom;
//		HardwareItem_t	pc104;
//		HardwareItem_t	optics;
//		HardwareItem_t	fan;
//		HardwareItem_t	loadingMechanism;
//		HardwareItem_t	printer;
//		HardwareItem_t	motor;
	} assemblies;

	struct
	{
//		HardwareItem_t	motor;
//		HardwareItem_t	globalDelay;
//		HardwareItem_t	filterNdxt;
//		HardwareItem_t	barcodeAlignment;
//		HardwareItem_t	barcodeReader;
//		HardwareItem_t	temperature;
//		HardwareItem_t	optics;
//		HardwareItem_t	burnIn;
//		HardwareItem_t	lowSample;
//		HardwareItem_t	highSample;
//		HardwareItem_t	verificationSample;
//		HardwareItem_t	log;
	} tests;
} HardwareData_t;

#endif	// __HARDWAREDATA_H_INCLUDED
