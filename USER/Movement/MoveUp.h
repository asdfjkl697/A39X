#ifndef MOVEUP_H
#define MOVEUP_H

#include "config.h"
#include "_485Com.h"
#include "MassageInit.h"
#include <string.h>

MoveUpDataTx_S *GetMoveUpDataTx(void);
u16 MoveUpCurTreadCircleGet(void);
void MoveUpDataTx(u8 dat);
void MoveUpFlexStateSet(u8 sta, u8 spd, u16 cir);
void MoveUpTreadStateSet(u8 sta, u8 spd, u16 cir);
void MoveUpKneadStateSet(u8 sta, u8 spd, u16 cir);
void MoveUpTapStateSet(u8 spd);

void MoveUpDataRx(void);

extern bool MoveUpTx_DriveCmd;

void MoveUpInit(void);

#endif
