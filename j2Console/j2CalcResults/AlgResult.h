#ifndef __ALGRESULT_H
#define __ALGRESULT_H

#if defined(__cplusplus)
extern "C" {
#endif


typedef struct
{
	unsigned char		analytePrintOrder;
	float				analyteResult;
	long				analyteFlags;
} AlgorithmResult_t;


#if defined(__cplusplus)
}
#endif

#endif	// __ALGRESULT_H