#ifndef MASSAGELOGIC_H
#define MASSAGELOGIC_H

#include "config.h"
#include "MoveBoth.h"
#include <string.h>

extern u8 Leg_Protect_Flag;
extern bool Leg_Touch_Flag;
extern bool Leg_Touch_Air_Flag;
extern bool STETCHGAS_Touch_Flag;
extern bool MoveUpDw_Loss_Flag;

extern void MassageLogic_Process(void);
extern void Debug_Pro(void);
#endif
