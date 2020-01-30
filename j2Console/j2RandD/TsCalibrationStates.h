#ifndef _TSCALIBRATIONSTATES_H
#define _TSCALIBRATIONSTATES_H
#include "GuiMessages.h"
#include "ManufacturingStates.h"


class StateTsCalibration : public StateRandD
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
};
#endif

