#ifndef VOICE_H
#define VOICE_H

#include "config.h"
#include "Bluetooth.h"
#include "WorkState.h"
#include <string.h>

typedef enum
{
#ifndef ENGLISH_VOICE
	VC_NULL = 0,   //
	VC_WakeUp = 1, // 请说指令
	VC_MassageOff, // 结束按摩
	VC_MassageOn,  // 开始按摩
	VC_Auto1,	   // 零压舒展
	VC_Auto2,	   // 中式推拿
	VC_Auto3,	   // 疲劳恢复
	VC_Auto4,	   // 海浪轻柔
	VC_Auto5,	   // 泰式拉筋
	VC_Auto6,
	VC_Auto7,
	VC_Auto8,
	VC_Auto9,
	VC_Auto10,
	VC_Auto11,
	VC_AirOff,		 //	关闭气压
	VC_AirOn,		 //	打开气压
	VC_SitUp,		 //	坐姿上升
	VC_SitDown,		 //	坐姿下降
	VC_PowerOff,	 // 按摩结束
	VC_PowerOn,		 // 开机
	VC_MassPause,	 // 按摩暂停
	VC_MassContinue, // 按摩继续
	VC_AutoSit,		 // 按摩角度调整中
	VC_BodyCheck,	 // 体型检测中
	VC_ShoulderAdj,	 // 体型检测完成
	VC_Reset,		 // 等待按摩椅复位
	VC_VcOff,
	VC_VcOn,
	VC_Invalid,
	VC_BbOff,
	VC_BbOn,
	VC_BlueOff,
	VC_BlueOn,
	VC_AutoWait, // 请选择按摩程序
	VC_ZeroOff,	 // 零重力关闭
	VC_Zero1,
	VC_Zero2,
	VC_Zero3,
	VC_MassStart, // 按摩开始
	VC_MassMode,  // 请选择按摩力度
	VC_VolumeUp,
	VC_VolumeDown,
	VC_VolumeMax,
	VC_VolumeMin,
	VC_Music1,
	VC_Music2,
	VC_Music3,
#else
	VC_NULL = 0,   //
	VC_WakeUp = 1, // im here
	VC_MassageOff, // massage close
	VC_MassageOn,  // massage on
	VC_Auto1,	   // total
	VC_Auto2,	   // energy
	VC_Auto3,	   // sweet
	VC_Auto4,	   // neck
	VC_Auto5,	   // lunch
	VC_Auto6,	   // thai
	VC_Change,	   // change the other mode
	VC_Fixed,	   // Fixed massage
	VC_Littup,
	VC_Littdw,
	VC_Stopadj,
	VC_AirOff,		 //	关闭气压
	VC_AirOn,		 //	打开气压
	VC_SitUp,		 //	坐姿上升
	VC_SitDown,		 //	坐姿下降
	VC_PowerOff,	 // 按摩结束
	VC_PowerOn,		 // 开机
	VC_MassPause,	 // 按摩暂停
	VC_MassContinue, // 按摩继续
	VC_AutoSit,		 // 按摩角度调整中
	VC_BodyCheck,	 // 体型检测中
	VC_ShoulderAdj,	 // 体型检测完成
	VC_Reset,		 // 等待按摩椅复位
	VC_VcOff,
	VC_VcOn,
	VC_Invalid,
	VC_BbOff,
	VC_BbOn,
	VC_BlueOff,
	VC_BlueOn,
	VC_AutoWait, // 请选择按摩程序
	VC_ZeroOff,	 // 零重力关闭
	VC_Zero1,
	VC_Zero2,
	VC_Zero3,
	VC_MassStart, // 按摩开始
	VC_MassMode,  // 请选择按摩力度
	VC_VolumeUp,
	VC_VolumeDown,
	VC_VolumeMax,
	VC_VolumeMin,
	VC_Music1,
	VC_Music2,
	VC_Music3,
#endif

} Voice_BB_enum;

// typedef enum
// {
// 	VC_NULL = 0,  //
// 	VC_WakeUp,	  // 我在
// 	VC_PowerOn,	  // 好的、开机
// 	VC_PowerOff,  // 好的、关机
// 	VC_MassPause, // 好的、暂停
// 	VC_HeatOn,	  // 好的、开启加热
// 	VC_HeatOff,	  // 好的、关闭加热
// 	VC_Strong,	  // 好的、重一点
// 	VC_Light,	  // 好的、轻一点
// 	VC_MassStart, // 好的、开始按摩
// 	VC_Zero2,	  // 好的、开始零重力
// 	VC_AirOn,	  // 打开气压
// 	VC_AirOff,	  // 关闭气压
// 	VC_Invalid,	  // 无效操作
// 	VC_Auto1,
// 	VC_Auto2,
// 	VC_Auto3,
// 	VC_Auto4,
// 	VC_Auto5,
// 	VC_Auto6,
// 	VC_Auto7,
// 	VC_VoiceOn,	 // 声控开启
// 	VC_VoiceOff, // 声控关闭
// } Voice_BB_enum;

typedef struct
{
	uint8_t Keep;
} VoiceKey_t;

extern VoiceKey_t VoiceKey;
extern bool Voice_Flag;

void VoicePlay(u8 ID);
void VoiceAck(u8 ID);

void Voice_Cof(u8 x);
void VoiceControl_Handler(void);
#endif
