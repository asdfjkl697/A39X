#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "config.h"
#include "WorkState.h"
#include <string.h>

// @ 最长播报时间13s
#ifdef USE_SOUNDWAVE
#define nPlay_time_Set 13 * 10
#else
#define nPlay_time_Set 18 * 10
#endif
#define nWake_time_set 10 * 10 // 唤醒时间

extern u32 UserCode_data;
#ifdef USE_SOUNDWAVE
extern bool SoundWave_M1_Paradox_Flag;
extern u8 nVolume_SoundWave;
extern u8 nVolume_SoundWave_Real;
extern u8 nVolume_Real;
#endif
extern u8 nVolume; // 音量
extern u8 Blue_At_Re;
extern u8 Blue_Over_Time; // 单位100ms

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
// extern void VoiceControl_Handler(void); //* 重复 屏蔽
//	extern void Voice_WakeUp(u8 nID);
extern void Voice_WakeUp(void);
extern void Voice_Swich_Set(u8 x);

extern void Blue_OnOff(u8 temp);

extern void BluePlay_time(void);

extern void BlueState_Play_Deal(void);

#ifdef USE_SOUNDWAVE
extern void Change_Sound(void);
extern void SoundWave_Resume(void);
extern void SoundWave_Reduce(void);
extern void SoundWave_Stop(void);
#endif

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

extern u8 insertSta;

#define Play_State_Pause 2
#define Play_State_Run 1

#define Device_Mode_Blue 0x00
#define Device_Mode_Flash 0x02

#endif
