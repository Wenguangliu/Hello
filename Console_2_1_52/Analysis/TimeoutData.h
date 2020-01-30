#ifndef __TIMEOUTDATA_H_INCLUDED
#define __TIMEOUTDATA_H_INCLUDED

typedef struct
{
	int	opticalDacs;
	int	beadCheck1;
	int	beadCheck2;
	int	distributionCheck;
	int	mixDone;
	int	blackOffsets;
	int	pass;
	int	analysisDone;
	int	resultsCalcComplete;
} TimeoutData_t;

#endif	// __TIMEOUTDATA_H_INCLUDED
