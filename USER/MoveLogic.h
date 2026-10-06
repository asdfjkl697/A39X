#ifndef MOVELOGIC_H
#define MOVELOGIC_H

#include "config.h"
#include "MoveBoth.h"
#include <string.h>

//! 全部计数减半 250823
#define DEFAULT_SHOULDER (u16)100			 // 默认肩位置(肩位置范围中间值) //* 120->150->200 待恢复 250808
#define SHOULDER_MIN (u16)15				 // 肩位置范围的最小值 //* 修改 30->20 最高点
#define SHOULDER_MAX (u16) SHOULDER_MIN * 11 // 肩位置范围的最大值

#define MoveArea_Space 35 // * 局部动作区域 25->50->35 250925

#ifdef USE_M2
#define Follow_Space 260 //* 跟随距离 251007
#ifdef USE_M2_4D
#define MoveUp_Avoid_Position 310 // todo md 260126
#define MoveMaxCircle 649		  // * 最大圈数
#define MoveDwMaxCircle 649		  // todo md 260126
#define MoveDw_UpSpace 238		  // todo md 260126
#define MoveUp_Limit 400		  // todo 430->400 260126
#else
#define MoveUp_Avoid_Position 340 // * 上机芯回避位置 250923
#define MoveMaxCircle 649		  // * 最大圈数
#define MoveDwMaxCircle 780		  // * md 下机芯与上机芯比例 575->649->780
#define MoveDw_UpSpace 217		  // * 碰撞阈值 md 250923 220-217 250928
#define MoveUp_Limit 430		  /* 实际测试 250923*/
#endif
#define MoveDw_Waist 200 /*设置下机芯腰部 BodyAreaDw[2] 250928*/
#else
#define MoveMaxCircle 649 /* MoveMaxCircle 需要根据实际情况调试 250514 */
#define MoveAREA9_11 159  /* 修改腰部位置再降一点 250619 */
#endif

#define UpTreadPlace GetMoveDataTx(MoveUp)->Tread.Signal
#ifdef USE_M2
#define DwTreadPlace GetMoveDataTx(MoveDw)->Tread.Signal
#endif

// * 位置 注意下机芯位置 250201
typedef enum
{
	LB_0 = 0,
	LB_1,
	LB_2,
	LB_3,
	LB_4,
	LB_5,
	LB_6,
	LB_7,
	LB_8,
	LB_9,
	LB_10,
	LB_11,
	LB_H0, // * 增加 250623
	LB_H1, //* 增加 250504
	LB_H2,
	LB_H3,
	LB_H5,
	LB_H6,
	LB_H7,
	LB_H8,
	LB_H9,	// * 增加 250623
	LB_C0,	// * 增加 250717
	LUP,	//* 上限位
	LDW,	//* 下限位
	LTZ,	//* 原位置
	LQS_UP, //* 上一位置
	LQS_DW, //* 下一位置
} Aera_ENUM;

// #define BodyPara 18 // * 11->12->13 增加腰部 增加H1--250504
// #define BodyPara 23			// * 11->12->13 增加腰部 增加H1--250504
#define BodyPara LQS_DW + 1 // * md 251011

extern u8 nBackIntensity;
#ifdef USE_M2_4D
extern u8 nBackIntensity_M2; // todo add 260108
#endif

typedef struct
{
	// uint8_t nBackIntensity; //* 屏蔽  250308
	uint8_t ManualArea_ID; /* 1：上半身 2：下半身 3：全身 4：定点 5：局部 */
} MoveUpState_t;

typedef enum
{
	TreadState_Stop = 0,
	TreadState_Up,
	TreadState_Dw,
	TreadState_UpCircle,
	TreadState_DwCircle,
	TreadState_Fixed, // 定位

} TreadState_E;

typedef enum
{
	KneadState_Stop = 0,
	KneadState_Right,
	KneadState_Left,
	KneadState_RightCircle,
	KneadState_LeftCircle,
	KneadState_RightFixed,
	KneadState_LeftFixed,

} KneadState_E;

typedef enum
{
	FlexState_Stop = 0,
	FlexState_Up,
	FlexState_Dw,
	FlexState_Fixed, // 定位
	FlexState_UpCircle,
	FlexState_DownCircle,

} FlexState_E;

typedef enum
{
	MassPlace_Null = 0,
	MassPlace_Top,
	MassPlace_Bot,
	MassPlace_Eor = 3,
	MassPlace_Hit = 4,
	MassPlace_Top_Hit = 5, //@ top hit 同时
	MassPlace_Bot_Hit = 6, //@ bot hit 同时
	MassPlace_Shouder_Success = 0x8,
	MassPlace_Shouder_Fail = 0x10,
} MassPlace_E;

extern MoveUpState_t MoveUpState;

extern uint8_t PreventFlag;
extern u16 BodyArea[BodyPara];
extern u16 BodyAreaDw[BodyPara]; //* 下机芯 250928
extern u8 Prevent_Cnt;
extern uint16_t MoveSpace;
extern u16 MoveUp_Circle;

extern u8 AutoPro_Flex_Para;

uint8_t BackPosUpGet(void);
void BackPosUpdate(void);
void MoveWhichSet(u8 whi);
u8 MoveWhichGet(void);
void BodyPlaceCal(u16 circle);
extern void M1_M2_void(void);

// extern void AutoPro_Flex_Adjust(void);
extern u8 AutoPro_Flex_Adjust(u8 flexpara);
#ifdef USE_M2_4D
extern uint16_t MoveDw_Circle;
extern u8 AutoPro_Flex_Adjust_M2(u8 flexpara);
#endif
#endif
