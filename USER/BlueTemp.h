#ifndef VOICECONTROL_H
#define VOICECONTROL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "config.h"

	/***************Blue*****************/

#define nPlay_time_Set 18 * 10 // 最长播报时间13s
#define nWake_time_set 10 * 10 // 唤醒时间

	extern u32 UserCode_data;

	extern u8 nVolume; // 音量

	extern u8 VoicePlayID; // 播报ID

	extern u8 nWake_time;
	extern u8 nPlay_time, Voice_DisTime;

	extern u8 nPlay_State;
	extern u8 nPlay_Device;
	extern u8 nPlay_Mode;
	extern u8 nPlay_ID;
	extern u8 nPlay_nVolume;
	extern u8 nBlue_Con;

	extern u8 BlueOk;

	extern void os_task_BlueRx(void);
	extern void os_task_blue(u8 nTask_ID);
	extern void os_task_voice(u8 nTask_ID);
	extern void Blue_Init(void);
	extern void VoiceControlPlay(u8 nId);
	extern void Voice_Ack(u8 nId);
	// extern void VoiceControl_Handler(void); //todo 重复 屏蔽
	extern void Voice_WakeUp(u8 nID);
	extern void Voice_Swich_Set(u8 x);

#define VoicePlay VoiceControlPlay

	typedef enum
	{
		Play_Mode_AllLoop = 0x01,	 // 全部循环
		Play_Mode_OneLoop = 0x02,	 // 单曲循环
		Play_Mode_FolderLoop = 0x03, // 文件夹循环
		Play_Mode_One = 0x05,		 // 单曲一次
		Play_Mode_All = 0x06,		 // 全部一次
	} Play_Mode_enum;

	extern u8 BlueState_Play; // 播放状态 0-停止 1-播放 2-暂停 3- 运行
	extern u8 Blue_Play_ID1;  // 初始ID
	extern u8 Blue_Play_ID2;
	extern Play_Mode_enum User_Play_Mode; // 循环模式

	extern u8 Blue_Connect_State;

#define Play_State_Pause 2
#define Play_State_Run 1

#define Device_Mode_Blue 0x00
#define Device_Mode_Flash 0x02

#define BT_LE_SET 0	   // 0-关闭数据蓝牙 1- 打开数据蓝牙
#define BT_RECON_SET 0 // 0-关闭自动重连 1- 打开自动重连
#define BT_TONE_SET 1  // 0-关闭提示音 1- 打开提示音

#ifdef VoiceControl_Type_A550

	typedef enum
	{
		VC_NULL = 0,   //
		VC_WakeUp,	   //	小艾，小艾
		VC_MassageOff, //	结束按摩
		VC_MassageOn,  //	开始按摩
		VC_Auto1,
		VC_Auto2,
		VC_Auto3,
		VC_Auto4,
		VC_Auto5,
		VC_Auto6,
		VC_Auto7,
		VC_Auto8,
		VC_Auto9,
		VC_Auto10,
		VC_Auto11,
		VC_AirOff,	//	关闭气压
		VC_AirOn,	//	打开气压
		VC_SitUp,	//	坐姿上升
		VC_SitDown, //	坐姿下降
		VC_PowerOff,
		VC_PowerOn,
		VC_MassPause,
		VC_MassContinue,
		VC_AutoSit,
		VC_BodyCheck,
		VC_ShoulderAdj,
		VC_Reset,
		VC_VcOff,
		VC_VcOn,
		VC_Invalid,
		VC_BbOff,
		VC_BbOn,
		VC_BlueOff,
		VC_BlueOn,
		VC_AutoWait,
		VC_ZeroOff,
		VC_Zero1,
		VC_Zero2,
		VC_Zero3,
		VC_MassStart,
		VC_MassMode,
	} Voice_BB_enum;

#elif defined(VoiceControl_Type_A665_2)

typedef enum
{
	VC_NULL = 0,   //
	VC_WakeUp,	   //	小艾，小艾
	VC_MassageOff, //	结束按摩
	VC_MassageOn,  //	开始按摩
	VC_Auto1,
	VC_Auto2,
	VC_Auto3,
	VC_Auto4,
	VC_Auto5,
	VC_Auto6,

	VC_Wise,
	VC_Fixed,
	VC_Fixed_Up,
	VC_Fixed_Down,
	VC_SitStop,

	// VC_Auto7,
	// VC_Auto8,
	// VC_Auto9,
	// VC_Auto10,
	// VC_Auto11,
	VC_AirOff,	//	关闭气压
	VC_AirOn,	//	打开气压
	VC_SitUp,	//	坐姿上升
	VC_SitDown, //	坐姿下降
	VC_PowerOff,
	VC_PowerOn,
	VC_MassPause,
	VC_MassContinue,
	VC_AutoSit,
	VC_BodyCheck,
	VC_ShoulderAdj,
	VC_Reset,
	VC_VcOff,
	VC_VcOn,
	VC_Invalid,
	VC_BbOff,
	VC_BbOn,
	VC_BlueOff,
	VC_BlueOn,
	VC_AutoWait,
	VC_ZeroOff,
	VC_Zero1,
	VC_Zero2,
	VC_Zero3,
	VC_MassStart,
	VC_MassMode,
} Voice_BB_enum;

#elif defined(VoiceControl_Type_A370)
typedef enum
{
	VC_NULL = 0,	//
	VC_WakeUp,		//	小艾，小艾
	VC_MassageOn,	//	开始按摩
	VC_MassageOn1,	//	开始按摩
	VC_MassageOff,	//	结束按摩
	VC_MassageOff1, //	结束按摩
	VC_MassageOff2, //	结束按摩
	VC_Auto1,
	VC_Auto2,
	VC_Auto3,
	VC_Auto4,
	VC_Auto5,
	VC_Auto6,
	VC_Auto7,
	VC_Wise,
	VC_Zero,
	VC_AirOn,  //	打开气压
	VC_AirOff, //	关闭气压
	VC_PowerOff,
	VC_PowerOn,
	VC_MassPause,
	VC_MassContinue,
	VC_AutoSit,
	VC_BodyCheck,
	VC_ShoulderAdj,
	VC_Reset,
	VC_VcOff,
	VC_VcOn,
	VC_Invalid,
	VC_BbOff,
	VC_BbOn,
	VC_BlueOff,
	VC_BlueOn,
	VC_AutoWait,
	VC_ZeroOff,
	VC_Zero1,
	VC_Zero2,
	VC_Zero3,
	VC_MassStart,
	VC_MassMode,
	VC_VolumeUp,
	VC_VolumeDown,
	VC_VolumeMax,
	VC_VolumeMin,
	VC_NUSIC1,
	VC_NUSIC2,
	VC_NUSIC3,
	VC_ManualMode, // 手动模式
				   // 下面还有26个自动手法语音播报，排序到72？

	VC_Stop = 74,
	VC_Knead,
	VC_Knock1,
	VC_Knock2,
	VC_KneadKnock,
	VC_Press1,
	VC_Press2,
	VC_Should_L_G, // 肩部提抓
	VC_3D1,
	VC_3D2,
	VC_3D3,
	VC_3D4,
	VC_Point,
	VC_Part,
	VC_UpperBody,
	VC_DownBody,
	VC_FullBody,
	VC_Add_Collect,	   // 加入收藏
	VC_Enable_Collect, // 启动收藏
	VC_None_Collect,   // 未选择收藏

} Voice_BB_enum;
#endif // VoiceControl_Type

	typedef enum
	{
		TM_ACK_NULL = 0,
		TM_ACK_Model_App = 0x01,
		TM_ACK_Version_Server = 0x02,
		TM_ACK_Model_Server = 0x04,
		// TM_ACK_WifiConfig = 0x08,
		// TM_ACK_BlueSet =0x10,
		// TM_ACK_VoiceSet = 0x20,
		TM_ACK_Reset = 0x40,
		TM_ACK_Valid_Sever = 0x80,
		TM_ACK_DisValid_Sever = 0x100,
		// TM_ACK_Valid_Voice = 0x100,
		// TM_ACK_WakeUp = 0x200,
	} TM_ACK_ENUM;

	typedef enum
	{
		TM_Device_Voice = 0x01,
		TM_Device_App,
		TM_Device_Server,
	} TM_Device;

#define APP_WaitCompleted() while (COM5.B_TX_busy)

#define OTA_PACKAGE_SIZE 300 // byte  一般蓝牙300  手控器串口2048
#define OTA_PACKAGE_DELAY 0	 // ms 数据包接收间隔，写0表示使用应答的方式

#define OTA_MODE "OTA_MODEL_A357B"

#define ota_state_stop 0
#define ota_state_start 1
#define ota_state_Data 2

#ifdef __cplusplus
}
#endif

#endif /* DEFINE_H */