#ifndef ROCINFO_H_
#define ROCINFO_H_

typedef struct
{
	bool		retrieved;
	char		rotorProductCode;
	char		rotorNameLine1[26];
	char		rotorNameLine2[26];
	unsigned	rotorNameFontColor;
	unsigned	rotorNameBackgroundColor;
} RocInfo_t;

class RocInfo
{
public:
					RocInfo();
	
	char			RotorProductCode( unsigned char rotorNum );
	const char *	RotorNameLine1( unsigned char rotorNum );
	const char *	RotorNameLine2( unsigned char rotorNum );
	unsigned		RotorNameFontColor( unsigned char rotorNum );
	unsigned		RotorNameBackgroundColor( unsigned char rotorNum );

private:
	int				GetRocData( unsigned char rotorNum );

	static char			fileNames[100][10];
	static RocInfo_t	rocInfo[100];
};

#endif /*ROCINFO_H_*/
