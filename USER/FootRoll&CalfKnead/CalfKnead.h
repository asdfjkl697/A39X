#ifndef CALFKNEAD_H
#define CALFKNEAD_H

#include "config.h"
#include "main.h"
#include <string.h>

typedef struct
{
	uint8_t Dir;   // 方向
	uint8_t Speed; // 速度
	uint8_t Time;  // 时间

} KneadAutoTable_S;

typedef struct
{
	uint8_t Step;	 // 步进
	uint32_t Tick;	 // 计时
	uint8_t Count;	 // 计数
	uint8_t CurMode; // 当前模式
	uint8_t PreMode; // 记录模式
	uint8_t State;	 // 方向
	uint8_t Speed;	 // 档位

} CalfKneadInfo_S;

void CalfKneadProcess(void);

void KneadModeSet(u8 mode);
uint8_t KneadModeGet(void);
void KneadStateSet(u8 sta);
void KneadSpeedSet(u8 spd);
u8 KneadStateGet(void);
u8 KneadSpeedGet(void);

#endif
