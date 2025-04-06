#ifndef MASSAGEINIT_H
#define MASSAGEINIT_H

#include "config.h"
#include "Motor.h"
#include "Air.h"
#include "Heat.h"
#include "Bluetooth.h"
#include "NOI.h"
#include <string.h>

#define Default_AutoMode 1
#define Max_AutoMode 20
#define _3D_INTENSITY_MAX 5
extern u8 ShoulderValue;

typedef enum
{
	MoveStop = 0, /* 停止 */
	MoveForeward, /* 正转 */
	MoveBack,	  /* 反转 */
	MoveComplete, /* 动作完成 */
} MoveState_t;

// typedef enum
// {
// 	SingalMid = 0, /* 运行在中间位置 */
// 	SignalTop,	   /* 运行在上限位 */
// 	SignalBot,	   /* 运行在下限位 */

// 	SignalDouble, /* 上下限位都触发 */
// } MoveSignal_t;

typedef enum
{
	MoveUp = 0,
	MoveDw,

} MassageMove_E;

typedef enum /* 1：上半身 2：下半身 3：全身 4：定点 5：局部 */
{
	Area_Null = 0,
	Area_Upper,
	Area_Lower,
	Area_Full,
	Area_Fixed,
	Area_Part,

} MassageArea_E;

typedef enum
{
	MassInten_0 = 0,
	MassInten_1,
	MassInten_2,
	MassInten_3,

} MassageIntensity_E;

typedef struct
{

	u8 Zero;
	u8 AirMode;
	u8 AirIntensity[3];
	u8 _3DIntensity[3];
	u8 Roll;
	u8 LegKnead;
	u8 Heat;
	u8 Music;
	u8 NOI;

} MassageInfo_S;

typedef struct
{

	u8 CurMode;
	u8 PreMode;
	u8 Intensity;

} AutoModeInfo_S;

typedef struct
{

	u8 MassArea;
	u16 MassParam;
	u8 MassWise;
	u8 MassWidth;
	u8 MassSpeed;
	u8 MassIntensity; // todo 发给机芯的 3D强度 250308

} ManualModeInfo_S;

typedef enum
{
	TZ = 0, // 手法停止
	RNZ,	// 揉捏正
	RNF,	// 揉捏反
	RNZB,	// 揉捏正变（内圈快 外圈慢）
	RNFB,	// 揉捏反变 （内圈快 外圈慢）

	PD,	  //*5 拍打
	PDB,  // 拍打变化 （快慢结合）
	RPZ,  // 揉拍正
	RPZB, // 揉p正变 （内圈快 外圈慢）

	ZY,	 // 指压   做推拿用
	ZYS, //* 10 指压保持（伸出到位置停止3秒）
	ZYD, // 深层指压--2段式（伸出一半，停1S 继续伸出））
	JRZ, // 肩部提抓 //@ 暂无

	D3RNZ, // 3D揉捏正
	D3RNF, // 3D揉捏反
	D3PD,  //*15 3D拍打
	D3RPZ, // 3D揉拍

	RNTNZ, // 肩胛骨推拿正  //@ 暂无
	RNTNF, // 肩胛骨推拿反  //@ 暂无
	RNZFD, // 宽中位置正反揉
	RNZFS, //*20 窄中位置正反揉

	RNZC,  //* RNZ 次数

	SHOULDERCHECK,

	TN,	  //*22 推拿
	TNUP, //*23 推拿上
	TNDW, //*24 推拿下
	TJB,  //*25 提肩
	TJA,  //TODO 26 提肩

	MaxWise, // 22

} ManualWise_E;

extern ManualModeInfo_S Manual[2];

void MassageInit(void);

u8 AutoModeGet(void);
void AutoModeSet(u8 para);
void AutoPreModeSet(u8 para);
u8 UserIntensityGet(void);
void UserIntensitySet(u8 inten);
void MassAreaSet(u8 whi, MassageArea_E area);
uint8_t MassAreaGet(u8 whi);
void MassArea_PartParamSet(u8 whi, u16 para);
u16 MassArea_PartParamGet(u8 whi);
void MassWiseSet(u8 whi, ManualWise_E wise);
u8 MassWiseGet(u8 whi);
void MassSpeedSet(u8 whi, u8 spd);
u8 MassSpeedGet(u8 whi);
u8 MassWidthGet(u8 whi);
void MassWidthSet(u8 whi, u8 width);
u8 MassIntensityGet(void);
void MassIntensitySet(u8 inten);
MassageInfo_S *MassModeGet(u8 param);
void MassageUpdate(void);

#endif
