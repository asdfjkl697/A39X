#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "config.h"
#include "WorkState.h"
#include <string.h>

#define nPlay_time_Set 18 * 10 // 最长播报时间13s
#define nWake_time_set 10 * 10 // 唤醒时间

extern u32 UserCode_data;

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
// extern void VoiceControl_Handler(void); //todo 重复 屏蔽
//	extern void Voice_WakeUp(u8 nID);
extern void Voice_WakeUp(void);
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

#define Play_State_Pause 2
#define Play_State_Run 1

#define Device_Mode_Blue 0x00
#define Device_Mode_Flash 0x02






/***********
增加app控制相关的定义  LZX  20250319
*/

#define OTA_MODE "0TA_MODE_A661S_31"

#define nPlay_time_Set 18 * 10 // 最长播报时间13s
#define nWake_time_set 10 * 10 // 唤醒时间

#define Play_State_Pause  2
#define Play_State_Run    1

#define Device_Mode_Blue  0x00
#define Device_Mode_Flash 0x02

#define BT_LE_SET 1    // 0-关闭数据蓝牙 1- 打开数据蓝牙
#define BT_RECON_SET 1 // 0-关闭自动重连 1- 打开自动重连
#define BT_TONE_SET 1  // 0-关闭提示音 1- 打开提示音


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

#define OTA_PACKAGE_SIZE 300  //byte  一般蓝牙300  手控器串口2048
#define OTA_PACKAGE_DELAY 0  //ms 数据包接收间隔，写0表示使用应答的方式


#define ota_state_stop 0
#define ota_state_start 1
#define ota_state_Data 2
#define ota_state_error 3//2
#define ota_state_end 4//3

#define APPMODEL    0x04 //APP MODEL 1:A100S  2:A666  3:A710
#define CHAIRMODE1  0x41    //A
#define CHAIRMODE2  0x33  //3
#define CHAIRMODE3  0x36    //6
#define CHAIRMODE4  0x35    //5
#define CHAIRMODE5  0x53    //S
#define CHAIRMODE6  0x2D    //-
#define CHAIRMODE7  0x31    //1


#define SECTOR_SIZE 2048 /* this parameter depends on the specific model of the chip */

#define FLASH_SIZE (1024 * (*((u32 *)0x1FFFF7E0))) /* read from at32 flash capacity register(unit:byte) */
#define SRAM_SIZE 32                               /* sram size, unit:kbyte */

/* app starting address */
#define OTA_SIZE 0x2000
#define APP_START_ADDR 0x08000000 + OTA_SIZE
#define APP_SIZE (u32)1024 * 92
#define TEMPLATE_START_ADDR APP_START_ADDR + APP_SIZE

/* the previous sector of app starting address is ota upgrade flag */
#define OTA_UPGRADE_FLAG_ADDR (APP_START_ADDR - 0x800)

/* when app received cmd 0x5aa5 from pc-tool, will set up the flag,
indicates that an app upgrade will follow, see ota application note for more details */
#define OTA_UPGRADE_FLAG 0x41544B38



#define AutoLoad_START_ADDR 0x08035000 
#define AutoLoad_SIZE (u32)1024 * 2
#define AutoLoad_ADDR_Offset 0x800


void App_Update_State_main( void );
void App_Value_Re( u8 nData ); // 接收APP数据
void App_Update_State( void );
/***********
增加app控制相关的定义 结束  LZX  20250319
*/



#endif
