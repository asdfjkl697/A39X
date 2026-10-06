#ifndef FOOTROLL_H
#define FOOTROLL_H

#include "config.h"
#include "Leg.h"
#include "FootRoll.h"
#include <string.h>

typedef struct
{
	uint8_t Dir;  // 方向
	uint8_t Time; // 时间

} RollAutoTable_S;

typedef struct
{
	uint8_t Step;	 // 步进
	uint32_t Tick;	 // 计时
	uint8_t Count;	 // 计数
	uint8_t CurMode; // 当前模式
	uint8_t PreMode; // 记录模式
	uint8_t State;	 // 方向
	uint8_t Speed;	 // 档位

} FootRollInfo_S;

void FootRollProcess(void);

void RollModeSet(u8 mode);
void RollStateSet(u8 sta);
void RollSpeedSet(u8 spd);
u8 RollStateGet(void);
u8 RollSpeedGet(void);

uint8_t RollModeGet(void);
#endif
