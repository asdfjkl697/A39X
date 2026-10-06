#ifndef HEALTH_H
#define HEALTH_H
/*
 * 文件包含 *************************************************************************************************************************************
 */
#include "config.h"
#include "main.h"
#include <string.h>

typedef enum
{
	Health_State_Stop = 0, // 停止
	Health_State_Fail,	   // 失败
	Health_State_Check,	   // 检测中
	Health_State_Finish,   // 检测完成
} Health_State_enum;

#define Health_MaxCheckCnt 25 // 最大检测次数 必须大于10 否则出错

#define Health_ProgressBar ((100 / Health_MaxCheckCnt) * Health_Cnt) // 检测进度百分比

extern Health_State_enum Health_State; // 检测状态
extern u8 Health_Cnt;				   // 检测进度
extern u16 Health_OutTime;			   // 检测超时计数
extern u16 Health_OutCnt;			   // 单次检测超时计数

extern u8 Health_Heart; // 心率
extern u8 Health_Blood; // 血样
extern u8 Health_Tired; // 疲劳度
extern u8 Health_WXH;	// 微循环

extern void Health_Init(void);
extern void Health_Set_State(Health_State_enum nState);
extern void Health_OutTime_Hand(void);
extern void os_task_Health(u8 nTask_ID);

#endif
