#ifndef FUJIAPP_H
#define FUJIAPP_H

// #include "define.h"
#include "config.h"

#define MaxSendLen 64 // todo add 260121

typedef struct
{
	uint8_t DataLen;
	uint8_t DataID;

	struct
	{
		uint8_t SysMode : 4;		/*系统开关机状态,0:关机状态，1：开机运行，2~15：工程模式x*/
		uint8_t Pause : 1;			/*暂停状态*/
		uint8_t MassageTimeOut : 1; /*按摩定时到状态*/
		uint8_t ChairReseting : 1;	/*按摩椅角度机芯复位中*/
	} Sys;

	uint8_t MassageTimeSec; /*按摩剩余时间 秒*/
	uint8_t MassageTimeMin; /*按摩剩余时间 分*/

	/**推杆信息--------------------------------------------------------------*/
	uint8_t SpikeAuto; /*推杆自动程序运行中 如零重力1，零重力2，直立，平躺 */ //* 12

	struct
	{
		uint8_t BackSpikeUpRun : 1;	   /*靠背推杆升背 运行中*/
		uint8_t BackSpikeDownRun : 1;  /*靠背推杆倒背 运行中*/
		uint8_t FeetSpikeUpRun : 1;	   /*腿脚推杆升起 运行中*/
		uint8_t FeetSpikeDownRun : 1;  /*腿脚推杆降下 运行中*/
		uint8_t ZeroSpikeUpRun : 1;	   /*坐部推杆升起 运行中*/
		uint8_t ZeroSpikeDownRun : 1;  /*坐部推杆降下 运行中*/
		uint8_t LegFlexExtendRun : 1;  /*伸缩推杆伸出 运行中*/
		uint8_t LegFlexShortenRun : 1; /*伸缩推杆缩回 运行中*/
	} SpikeRun;

	struct
	{
		uint8_t BackSpikePosition : 4; /*靠背推杆位置 0~15*/
		uint8_t FeetSpikePosition : 4; /*腿脚推杆位置 0~15*/
	} SpikePositionBF;				   //* 14

	struct
	{
		uint8_t ZeroSpikePosition : 4; /*坐部推杆位置 0~15*/
		uint8_t FlexSpikePosition : 4; /*伸缩推杆位置 0~15*/
	} SpikePositionZF;

	struct
	{
		uint8_t BackDownAlarm : 1;	/*靠背降防夹报警*/
		uint8_t FeetDownAlarm : 1;	/*小腿降防夹报警*/
		uint8_t FlexShortAlarm : 1; /*小腿缩防夹报警*/
	} SpikeAntiPinch;				/*推杆防夹 */

	/**机芯信息----------------------------------------------------------------*/
	struct
	{
		uint8_t ScanRun : 1;	/*人体检测中*/
		uint8_t ScanFinish : 1; /*人体检测完成*/
		uint8_t ScanFail : 1;	/*⼈体检测失败*/
		uint8_t AdjustEn : 1;	/*允许肩部位置手动微调使能*/
	} BackScan;					//*17

	uint8_t ShoulderPositionAdjustValue; /*肩部位置⼿动微调档位值(0-10)*/ //* add 260108

	uint8_t CoreAutoMassage; /*机芯自动程序运行中 0:无自动程序运行，1~N:自动程序运行中*/
	// uint8_t CoreManualMassageXYZAction;	 /*机芯手动程序运行中 XYZ手动程序*/
	uint8_t CoreManualMassageMix; /*⼿动程序混合模式， 0：关闭 1：开启*/	 //* add 260108
	uint8_t CoreManualMassageMixAction1; /*机芯⼿动程序运⾏中 XYZ⼿动程序1*/ //* add 260108
	uint8_t CoreManualMassageMixAction2; /*机芯⼿动程序运⾏中 XYZ⼿动程序2*/ //* add 260108
	uint8_t CoreManualMassageMixAction3; /*机芯⼿动程序运⾏中 XYZ⼿动程序3*/ //* add 260108
	uint8_t CoreManualMassageMixAction4; /*机芯⼿动程序运⾏中 XYZ⼿动程序4*/ //* add 260108
	uint8_t CoreManualMassageXYZAction;										 // @ add 手动手法 260127
	uint8_t CoreManualMassageTCSAction;										 /*机芯手动程序运行中 TCS手动程序*/

	struct
	{
		uint8_t CoreStrength : 4; /*机芯强度、3D档位*/
		uint8_t GasStrength : 4;  /*充气强度*/
	} MassageStrength;

	struct
	{
		uint8_t XPosition : 4; /*机芯X轴坐标(0-5) */
		uint8_t ZPosition : 4; /*机芯Z轴坐标(0-10)*/
	} XYZPositon;

	uint8_t YPosition; /*机芯Y轴坐标(0-32)*/

	struct
	{
		uint8_t HeightUpRun : 1;   /*机芯 上行*/
		uint8_t HeightDownRun : 1; /*机芯 下行*/
		uint8_t WidthDecRun : 1;   /*机芯 调窄*/
		uint8_t WidthAddRun : 1;   /*机芯 调宽*/
		uint8_t ManualRange : 2;   /*手动程序按摩范围，0:定点，1:区间，2:全程*/
	} CoreAdjust;

	uint8_t ManualWidth; /*机芯宽度1:窄，2:标准（中）3:宽*/ // @ add 260117
	struct
	{
		uint8_t XYZSpeed : 4; /*XYZ⼿法速度*/
		uint8_t TCSSpeed : 4; /*TCS⼿法速度*/
	} MechaSpeed;

	/**气压按摩----------------------------------------------------------------*/
	struct
	{
		uint8_t NeckWork : 1;	  /*颈部充⽓ 开关*/
		uint8_t ShoulderWork : 1; /*肩部充⽓ 开关*/
		uint8_t ArmLeftWork : 1;  /*左⼿充⽓ 开关*/
		uint8_t ArmRightWork : 1; /*右⼿充⽓ 开关*/
		uint8_t LumbarWork : 1;	  /*腰部充⽓ 开关*/
		uint8_t SeatWork : 1;	  /*座部充⽓ 开关*/
		uint8_t CalvesWork : 1;	  /*腿部充⽓ 开关*/
		uint8_t FeetWork : 1;	  /*脚部充⽓ 开关*/
	} GasWork;

	struct
	{
		uint8_t NeckRun : 1;	 /*颈部充⽓ 运⾏中*/
		uint8_t ShoulderRun : 1; /*肩部充⽓ 运⾏中*/
		uint8_t ArmLeftRun : 1;	 /*左⼿充⽓ 运⾏中*/
		uint8_t ArmRightRun : 1; /*右⼿充⽓ 运⾏中*/
		uint8_t LumbarRun : 1;	 /*腰部充⽓ 运⾏中*/
		uint8_t SeatRun : 1;	 /*座部充⽓ 运⾏中*/
		uint8_t CalvesRun : 1;	 /*腿部充⽓ 运⾏中*/
		uint8_t FeetRun : 1;	 /*脚部充⽓ 运⾏中*/
	} GasRun;

	/**加热按摩、滚轮按摩-------------------------------------------------------*/
	struct
	{
		uint8_t HeatNeckRun : 1;	 /*背部 加热 运行中*/
		uint8_t HeatShawlRun : 1;	 /*披肩 加热 运行中*/
		uint8_t HeatArmRun : 1;		 /*手臂 加热 运行中*/
		uint8_t HeatFeetRun : 1;	 /*脚部 加热 运行中*/
		uint8_t RollerCalvesRun : 1; /*揉腿 滚轮 运行中*/
		uint8_t RollerFeetRun : 2;	 /*脚滚 滚轮 运行中*/
	} HeatRoller;

	/**LED    ----------------------------------------------------------------*/
	struct
	{
		uint8_t LogoRun : 1;	/*Logo灯IO状态*/
		uint8_t WelcomeRun : 1; /*迎宾灯IO状态*/
	} Led;

	uint8_t MusicPlaySn; /*正在播放的⾳乐的序号*/ //* add 260108
	uint8_t MusicVolume; /*⾳乐的⾳量*/			  //* add 260108

	uint8_t ReminderSound; /*提示音*/
	uint8_t FaultCode;	   /*故障代码*/

	/**健康检测----------------------------------------------------------------*/
	//   struct
	//   {
	//     /**待定 */
	//   }Byte;

} CDP_DataFormat0x21_t;

typedef struct
{
	uint8_t DataLen;
	uint8_t DataID;
	uint16_t SysRunTimeHour;			/*系统运⾏时间*/
	uint16_t XMotorRunTimeHour;			/*揉捏⻢达运⾏时间*/
	uint16_t YMotorRunTimeHour;			/*⾏⾛⻢达运⾏时间*/
	uint16_t ZMotorRunTimeHour;			/*强弱⻢达运⾏时间*/
	uint16_t TCSMotorRunTimeHour;		/*敲击⻢达运⾏时间*/
	uint16_t BackSpikeMotorRunTimeHour; /*靠背推杆⻢达运⾏时间*/
	uint16_t FeetSpikeMotorRunTimeHour; /*⼩腿推杆⻢达运⾏时间*/
	uint16_t ZeroSpikeMotorRunTimeHour; /*零重⼒推杆⻢达运⾏时间*/
	uint16_t LegFlexMotorRunTimeHour;	/*⼩腿伸缩⻢达运⾏时间*/
	uint16_t PumpRunTimeHour;			/*⽓泵运⾏时间*/
} CDP_DataFormat0x3F_t;

// extern bool Uart3_Seng_Flag;
extern void EUSART2_Send_Data(u8 *send_data, u8 send_len);
extern void FUJIApp_Recieve(void);
// extern void App_Value_Rx(u8 data);
extern void FUJIApp_Deal_10ms(void);

extern u8 zy_z(u8 *Array, u16 Len);
extern u16 zy_f(u8 *Array, u16 Len);
extern u8 add_checksum(const u8 *data, size_t length);

#endif /* APP_H */
