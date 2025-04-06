/*
*************************************************************************************************************************************************
* 文件：
* ----------------------------------------------------------------------------------------------------------------------------------------------
* 描述：提供与各模块信息交互的平台，总结构体MChair,
* ----------------------------------------------------------------------------------------------------------------------------------------------
* 注意：
* ----------------------------------------------------------------------------------------------------------------------------------------------
* 历史：
*************************************************************************************************************************************************
*/
#ifndef __SYSTEM_H
#define __SYSTEM_H

/*
 * 文件包含 *************************************************************************************************************************************
 */
#include "config.h"
#include "SysCfg.h"
#include "KeyFunc.h"

/*
 * 类型定义 *************************************************************************************************************************************
 */
/********************************************************************************
 * 键值处理相关
 ********************************************************************************/
typedef struct
{
	uint8_t State;
	uint8_t Value;
	uint8_t Param;

	uint16_t KeyHoldTimeMs; /*按键保持时间*/
} MChairKey_t;				/* 按键键值信息结构体 */

typedef struct
{
#if (DCI_ARMKEY_EN)
	MChairKey_t ArmKey; /* 扶手按键信息 */
#endif
#if (DCI_APP_EN)
	MChairKey_t APP; /* APP按键信息 */
#endif
#if (DCI_VOICE_EN)
	MChairKey_t Voice; /* 语音按键信息 */
#endif
#if (DCI_CTRL_EN)
	MChairKey_t Ctrl; /* 手控器按键信息 */
#endif
} MChairKeyHandle_t;

typedef struct
{
	u8 MassArea;
	u16 MassParam;
	u8 MassWise;
	u8 MassWidth;
	u8 MassSpeed;
	u8 MassIntensity; //* 显示用的3D强度

} MChairCoreState_t;

typedef union
{
	uint8_t All;

	struct
	{
		uint8_t CoreReset : 1;
		uint8_t NoFootFindFlag : 1; /* 在拉伸保持过程中可能不希望找脚，通过该标志位屏蔽找脚动作 */
		// uint8_t TimOutResetSw : 1;	/* 按摩时间到后按摩椅复位开关 */
		uint8_t Res : 5;
	} Bit;

} SystemCfg_t;

typedef struct
{

	uint32_t Tick;
	uint8_t CurState;
	uint8_t CurMin; // 当前分
	uint8_t CurSec; // 当前秒
	uint8_t VoicePlayFlag;

} MChairWorkState_t;

// typedef struct
// {
// 	uint8_t MusicSW;		/* 音乐开关 */
// 	uint8_t VoicePlayer;	/* 语音播报开关 */
// 	uint8_t VoiceControlSw; /* 语音控制开关 */
// } Bluetooth_t;

typedef struct
{
	uint8_t FootFinding; /* 找脚中 */
} Leg_t;

typedef struct
{
	//	MChairKeyHandle_t                      KeyHandle;
	//
	//	MChairWorkState_t                      WorkState;
#if (USE_M2)
	MChairCoreState_t CoreState[2];
#else
	MChairCoreState_t CoreState[1];
#endif

	SystemCfg_t CFG;

	// Bluetooth_t Bluetooth;

#if (LEG_COM)
	Leg_t Leg;
#else
	// Roll_t Roll;
#endif
} MChair_t;

/*
 * 对外声明 *************************************************************************************************************************************
 */
extern MChair_t MChair;

#endif
