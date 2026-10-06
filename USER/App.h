#ifndef APP_H
#define APP_H

//#include "DEFINE.h"
#include "config.h"
#include "WorkState.h"

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



	
#define Shoulder_Sac UnionState1.StrackBit.BIT0
#define Back_Sac UnionState1.StrackBit.BIT1
#define Arm1_Sac UnionState1.StrackBit.BIT2
#define Arm2_Sac UnionState1.StrackBit.BIT3
#define Hand_Sac UnionState1.StrackBit.BIT4
#define Hips_Sac UnionState1.StrackBit.BIT5
#define Leg1_Sac UnionState1.StrackBit.BIT6
#define Leg2_Sac UnionState1.StrackBit.BIT7

ErrStatus app_ota_erase(u32 write_addr, u32 num_sector); // @ add 260303
ErrStatus app_ota_write(u32 write_addr, u8 *p_buffer, u16 num_write); 

extern void write_upgrade_flag(void);

void AppCom(u8 dat);
void ota_init(void);
extern void App_init(void);
extern void os_task_app(OS_TASK_ID nTask_ID);
extern void App_Cmd_Handler(u8 *nbuf, u16 nLen);
extern u16 TM_ACK_Value;


#define FLASH_SIZE (1024 * (*((u32 *)0x1FFFF7E0))) /* read from at32 flash capacity register(unit:byte) */
#define SRAM_SIZE 32                               /* sram size, unit:kbyte */

/* app starting address */
#define OTA_SIZE 0x4000
#define APP_START_ADDR 0x08000000 + OTA_SIZE
//app size
#define APP_SIZE (u32)1024 * 216
//app backup address
#define TEMPLATE_START_ADDR 0x08042000//APP_START_ADDR + APP_SIZE

/* the previous sector of app starting address is ota upgrade flag */
#define OTA_UPGRADE_FLAG_ADDR 0x0803a000 //(APP_START_ADDR - 0x800)

/* when app received cmd 0x5aa5 from pc-tool, will set up the flag,
indicates that an app upgrade will follow, see ota application note for more details */
#define OTA_UPGRADE_FLAG 0x41544B38


/*data address*/
//The address where the system parameters are stored
#define SYSTEM_PARA_ADDR 0x0803a800    //2K

//The address where the user programs are stored
#define USER_PROGRAM_ADDR 0x0803b000   //2k



#define ONLINE_AUTO_ADDR1                 0x0803b800//(RUNTIME_ADDR-0x800)   //2K 
#define ONLINE_AUTO_ADDR2                 0x0803c000//(ONLINE_AUTO_ADDR1-0x800)   //2K 
#define ONLINE_AUTO_ADDR3                 0x0803c800//(ONLINE_AUTO_ADDR2-0x800)   //2K  
#define ONLINE_AUTO_ADDR4                 0x0803d000//(ONLINE_AUTO_ADDR3-0x800)   //2K  
#define ONLINE_AUTO_ADDR5                 0x0803d800//(ONLINE_AUTO_ADDR4-0x800)   //2K    
#define ONLINE_FLAG_ADDR                  0x0803e000//  (ONLINE_AUTO_ADDR5-0x800)   //2K  

#define AutoLoad_START_ADDR ONLINE_AUTO_ADDR1//0x08035000 
#define AutoLoad_SIZE (u32)1024 * 2
#define AutoLoad_ADDR_Offset 0x800
#define APP_WaitCompleted() while (COM3.B_TX_busy)

#define FLASH_SN_ADDR     0x0803f000


#define OTA_PACKAGE_SIZE 200 // byte
#define OTA_PACKAGE_DELAY 0  // ms 数据包接收间隔，写0表示使用应答的方式

#define OTA_MODE "0TA_MODE_A391S-1"


//u8 TxF1Enable();
//u8 TxF2Enable();
#endif /* APP_H */
