#ifndef ROTORINFO_H_
#define ROTORINFO_H_

#include "CommonDefines.h"

typedef struct
{
	char		rotorProductCode;
	char		rotorNameLine1[26];
	char		rotorNameLine2[26];
	unsigned	rotorNameFontColor;
	unsigned	rotorNameBackgroundColor;
} RotorInfo_t;

class RotorInfo
{
public:
					RotorInfo();
	
	char			RotorProductCode( unsigned char rotorNum )			{ return rotorInfo[ rotorNum ].rotorProductCode; };
	const char *	RotorNameLine1( unsigned char rotorNum )			{ return rotorInfo[ rotorNum ].rotorNameLine1; };
	const char *	RotorNameLine2( unsigned char rotorNum )			{ return rotorInfo[ rotorNum ].rotorNameLine2; };
	unsigned		RotorNameFontColor( unsigned char rotorNum )		{ return rotorInfo[ rotorNum ].rotorNameFontColor; };
	unsigned		RotorNameBackgroundColor( unsigned char rotorNum )	{ return rotorInfo[ rotorNum ].rotorNameBackgroundColor; };

private:
	static RotorInfo_t	rotorInfo[ MAX_ROTOR_NUM + 1 ];
};

#endif /*ROTORINFO_H_*/
