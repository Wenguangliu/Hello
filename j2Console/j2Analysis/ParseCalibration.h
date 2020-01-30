#ifndef __PARSECALIBRATION_H_INCLUDED
#define __PARSECALIBRATION_H_INCLUDED

#include "CalData.h"
#include "CfgErr.h"

CfgError_t	ParseCalibration( CalibrationData_t * calibrationData );
CfgError_t	WriteCalibration( CalibrationData_t * calibrationData );

#endif	// __PARSECALIBRATION_H_INCLUDED
