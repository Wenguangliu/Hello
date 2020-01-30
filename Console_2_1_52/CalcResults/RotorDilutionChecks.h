#ifndef __ROTORDILUTIONCHECKS_H
#define __ROTORDILUTIONCHECKS_H

#if defined(__cplusplus)
extern "C" {
#endif


void UpdateRotorDilutionCheckProbs(unsigned char, unsigned long, unsigned long, float);

void ProcessRotorDilutionChecksErrors(unsigned char*);


#if defined(__cplusplus)
}
#endif

#endif	// __ROTORDILUTIONCHECKS_H
