#ifndef MOVEDW_H
#define MOVEDW_H

#include "config.h"
#include "_485Com.h"
#include "MassageInit.h"
#include <string.h>

MoveDwDataTx_S *GetMoveDwDataTx(void);

void MoveDwTreadStateSet(u8 sta, u8 spd, u16 cir);
void MoveDwDataTx(u8 dat);
extern u8 MoveDwTx_DriveCmd;
void MoveDwDataRx(void);

void MoveDwKneadStateSet(u8 sta, u8 spd, u16 cir);
void MoveDwTapStateSet(uint8_t spd);
#endif
