#ifndef LEG_H
#define LEG_H

#include "config.h"
#include "_485Com.h"
#include "Heat.h"
#include "FootRoll.h"
#include "CalfKnead.h"
#include "FootFlex.h"
#include "Air.h"
#include <string.h>

typedef enum
{
	LegActionStop,
	LegActionDown,
	LegActionUp,
	LegActionSuccess,
} Leg_State_t;

void LegInit(void);
// void LegDataRx(u8 dat);
void LegDataTx(void);
#ifdef Debug_Mode
extern LegDataTx_S LegTx;
extern LegDataRx_S LegRx;
#endif
LegDataRx_S *GetLegDataRx(void);
extern Leg_State_t Leg_State;

void LegRollStateSet(uint8_t sta, uint8_t spd);
void LegKneadStateSet(uint8_t sta, uint8_t spd);
void LegHeatStateSet(uint8_t sta, uint8_t level);
void LegAirStateSet(uint8_t air);
void LegFlexStateSet(uint8_t sta, uint8_t spd, uint16_t circle);

#endif
