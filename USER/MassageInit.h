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
#define _3D_INTENSITY_MAX 5

#ifdef USE_ONLINE_MODE
#define Max_AutoMode 25
#elif defined AUTO_NUM_24
#define Max_AutoMode 24
#else
#define Max_AutoMode 20
#endif

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
	u8 MassIntensity; // * 发给机芯的 3D强度 250308

} ManualModeInfo_S;

typedef enum
{
	// 时间组，运动时间由主板控制
	TZ = 0x00,		 // 手法停止   (拍打停止，揉宽、伸缩可控)
	RNZ = TZ + 1,	 // 揉捏正    (拍打停止，揉速度可以设置、伸缩可控)
	RNF = TZ + 2,	 // 揉捏反    (拍打停止，揉速度可以设置、伸缩可控)
	RNZB = TZ + 3,	 // 揉捏正变 (内圈快 外圈慢) (拍打停止，揉速度可以设置、伸缩可控)
	RNFB = TZ + 4,	 // 揉捏反变 (内圈快 外圈慢) (拍打停止，揉速度可以设置、伸缩可控)
	PD = TZ + 5,	 // 拍打  (拍打速度可控，揉宽可以设置、伸缩可控)
	PDB = TZ + 6,	 // 拍打变化 (快慢结合，拍打速度可控，揉宽可以设置、伸缩可控)
	RPZ = TZ + 7,	 // 揉拍正 (拍打速度可控，揉速度可以设置、伸缩可控)
	RPF = TZ + 8,	 // 揉拍反 (拍打速度可控，揉速度可以设置、伸缩可控)
	RPZB = TZ + 9,	 // 揉拍正变 (内圈快 外圈慢)  (拍打速度可控，揉速度可以设置，伸缩可控)
	RPFB = TZ + 10,	 // 揉拍反变 (内圈快 外圈慢)  (拍打速度可控，揉速度可以设置，伸缩可控)
	ZY = TZ + 11,	 // 指压 (拍打停止，揉宽可以设置、伸缩可以控)
	ZYS = TZ + 12,	 // 指压保持 (伸出 -- 零位) (拍打停止，揉宽可以设置、伸缩可控)
	ZYD = TZ + 13,	 // 2段指压 伸出一半 -- 完全伸出 -- 零位）(拍打停止，揉宽可以设置、伸缩可控)
	D3RNZ = TZ + 14, // 3D揉捏正 (拍打停止，正揉，自动伸缩，伸缩量可控)  +
	D3RNF = TZ + 15, // 3D揉捏反 (拍打停止，正揉，自动伸缩，伸缩量可控)  +

	D3PD = 0x10,	   // 3D拍打  (拍打速度可控，揉宽可以设置，自动伸缩，伸缩量可控)
	D3RPZ = D3PD + 1,  // 3D揉拍正 (拍打速度可控，正揉，自动伸缩，伸缩量可控)
	D3RPF = D3PD + 2,  // 3D揉拍反 (拍打速度可控，正揉，自动伸缩，伸缩量可控)
	RNZFD = D3PD + 3,  // 宽中位置正反揉 (拍打停止，正反揉，伸缩可控)     +
	RNZFS = D3PD + 4,  // 窄中位置正反揉 (拍打停止，正反揉，伸缩可控)     +
	RNKZQH = D3PD + 5, // 宽窄切换,中间停1秒
	JRZ = D3PD + 6,	   // 肩部提抓 (拍打停止，正揉、伸缩可控)
	JRC = D3PD + 7,	   // 颈部揉搓，小揉搓
	BURC = D3PD + 8,   // 背部上揉搓, 70 - 126
	BDRC = D3PD + 9,   // 背部下揉搓, 66 - 10
	QUA = D3PD + 10,   // 四分之一, 下窄中
	QUB = D3PD + 11,   // 四分之一, 上窄中
	QUC = D3PD + 12,   // 四分之一, 上宽中
	QUD = D3PD + 13,   // 四分之一, 下宽中
	QUA4D = D3PD + 14, // 四分之一A,伸缩跟随
	QUB4D = D3PD + 15, // 四分之一B,伸缩跟随

	QUC4D = 0x20,				  // 四分之一C,伸缩跟随
	QUD4D = QUC4D + 1,			  // 四分之一D,伸缩跟随
	RNZFS_TR_FLW = QUC4D + 2,	  // 窄中位置正反揉, 行走跟随
	RNZFS_TR_FLE_FLW = QUC4D + 3, // 窄中位置正反揉, 行走、伸缩跟随

	// 次数组
	RNZ_C = 0x40,
	RNF_C = RNZ_C + 1,
	RNZB_C = RNZ_C + 2,
	RNFB_C = RNZ_C + 3,
	RPZ_C = RNZ_C + 4,
	RPF_C = RNZ_C + 5,
	RPZB_C = RNZ_C + 6,
	RPFB_C = RNZ_C + 7,
	ZYS_C = RNZ_C + 8,
	ZYD_C = RNZ_C + 9,
	D3RNZ_C = RNZ_C + 10,
	D3RNF_C = RNZ_C + 11,
	D3RPZ_C = RNZ_C + 12,
	D3RPF_C = RNZ_C + 13,
	RNZFD_C = RNZ_C + 14,
	RNZFS_C = RNZ_C + 15,

	RNKZQH_C = 0x50,
	JRZ_C = RNKZQH_C + 1,
	JRC_C = RNKZQH_C + 2,
	BURC_C = RNKZQH_C + 3,
	BDRC_C = RNKZQH_C + 4,
	QUA_C = RNKZQH_C + 5,	 // 四分之一, 下窄中
	QUB_C = RNKZQH_C + 6,	 // 四分之一, 上窄中
	QUC_C = RNKZQH_C + 7,	 // 四分之一, 上宽中
	QUD_C = RNKZQH_C + 8,	 // 四分之一, 下宽中
	QUA4D_C = RNKZQH_C + 9,	 // 四分之一，下窄中， 4D
	QUB4D_C = RNKZQH_C + 10, // 四分之一，上窄中， 4D
	QUC4D_C = RNKZQH_C + 11, // 四分之一，上窄中， 4D
	QUD4D_C = RNKZQH_C + 12, // 四分之一，上窄中， 4D

	MAX_CNT,

	// 行走由机芯自己控制,0x80起
	SHOULDERCHECK = 0x80, // 肩部检测 // JBJC = 0x80,
	TN,					  // 推拿
	TNUP,				  // 推拿上 TNS,
	TNDW,				  // 推拿下 TNX,
	TJA,				  // 提肩A
	TJB,				  // 提肩B
	NOTREAD_MAX,		  // * 结束

	MaxWise,

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
#ifdef USE_M2_4D
void MassIntensitySet_M2(u8 inten);
#endif
MassageInfo_S *MassModeGet(u8 param);
void MassageUpdate(void);

#endif
