#ifndef FOOTFLEX_H
#define FOOTFLEX_H

#include "config.h"
#include <string.h>

#define LegFlexMaxCircle 106

typedef enum
{
	FlexStop,
	FlexDown,
	FlexUp,
	// 伸多少圈
	// 缩多少圈
	// 运动到指定位置
	FootFind = 6,

} FootFlexState_E;

typedef struct
{
	FootFlexState_E State; // 状态
	uint8_t Speed;		   // 速度
	uint16_t Circle;	   // 计数

} FootFlexInfo_S;

void FootFlexStateSet(FootFlexState_E sta);
void FootFlexSpeedSet(u8 spd);
FootFlexState_E FootFlexStateGet(void);
u8 FootFlexSpeedGet(void);
void FootFlexCircleSet(u16 cir);
u16 FootFlexCircleGet(void);

#endif
