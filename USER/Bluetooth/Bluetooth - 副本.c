#include "Bluetooth.h"
#include "FMC/fmc.h" //* 增加

u32 UserCode_data = 0;

u8 nVolume; // 音量
u8 Blue_At_Re;
u8 Blue_Over_Time;  // 单位100ms
char Blue_Name[60]; // 蓝牙名称

u8 VoicePlayID; // 播报ID

u8 nWake_time;
u8 nPlay_time;
u8 Voice_DisTime;

u8 nPlay_State;
u8 nPlay_Device;
u8 nPlay_Mode;
u8 nPlay_ID;
u8 nPlay_nVolume;
u8 nBlue_Con;

U8 BlueOk; // 蓝牙版就绪标志
u8 modeflag = 0;
static u8 blue_Update_bit, blue_a3_ok;

u8 bPause;         // 从播放到暂停的标志
u8 BlueState_Play; // 播放状态 0-停止 1-播放 2-暂停 3- 运行
u8 Blue_Play_ID1;  // 初始ID
u8 Blue_Play_ID2;
Play_Mode_enum User_Play_Mode; // 循环模式

// u8 BT_STATE_READ; // 开始读取蓝牙状态标志
// u8 Blue_Ascii_Cnt;
// u8 Blue_Connect_State;
// UnionBit BT_STATE; //todo 测试
// #define BT_OK BT_STATE.StrackBit.BIT0
// #define BT_LE BT_STATE.StrackBit.BIT1
// #define BT_BREDR BT_STATE.StrackBit.BIT2
// #define BT_RECON BT_STATE.StrackBit.BIT3
// #define BT_TONE BT_STATE.StrackBit.BIT4
// #define BT_NAME BT_STATE.StrackBit.BIT5
// #define BT_LE_SET 0     // 0-关闭数据蓝牙 1- 打开数据蓝牙
// #define BT_RECON_SET 0 // 0-关闭自动重连 1- 打开自动重连
// #define BT_TONE_SET 1  // 0-关闭提示音 1- 打开提示音

extern u8 tx2_checksum;
#if (USE_CSD_MSUIC)
u8 CSD_BLUE_STATE;
#endif // USE_CSD_MSUIC

/***************
 * 打印32位无符号整型到字符串
 *  *Str目标字符串
 *  nValue需要打印的数据
 *  nlen打印的长度1~10,
 * ******************/
void User_PrintString5_U32( char *Str, u32 nValue, u8 nlen )
{
    if ( nlen == 0 || nlen > 10 )
        return;
    Str[nlen] = '\0'; // 增加结束符
    while ( nlen > 0 )
    {
        nlen--;
        Str[nlen] = nValue % 10 + '0';
        nValue /= 10;
    }
}

void User_Copy_Str( char *Str1, char *Str2 )
{
    while ( *Str2 != '\0' )
    {
        *Str1 = *Str2;
        Str1++;
        Str2++;
    }
    *Str1 = '\0';
}

void User_Splicing_Str( char *Str1, char *Str2 )
{
    while ( *Str1 != '\0' )
        Str1++;
    while ( *Str2 != '\0' )
    {
        *Str1 = *Str2;
        Str1++;
        Str2++;
    }
    *Str1 = '\0';
}

static u8 User_found_strs( char *str, char *_str, char _str1 )
{
    u8 i;
    while ( *str != '\0' && *str != _str1 )
    {
        i = 0;
        while ( _str[i] != '\0' && _str[i] != _str1 )
        {
            if ( _str[i] != str[i] )
                break;
            i++;
            if ( _str[i] == '\0' || _str[i] == _str1 )
                return 1;
            if ( str[i] == '\0' || str[i] == _str1 )
                return 0;
        }
        str++;
    }
    return 0;
}

static void BlueBoard_Read_Data( void )
{
    EUSART2_Write( 0xa3 );
    EUSART2_Write( 0x00 );
    EUSART2_Write( 0x00 );
    EUSART2_Write( 0x5d );
}

static void BlueBoard_Send_Data1( u8 nCom, u8 nData )
{
    tx2_checksum = 0;
    EUSART2_Write( 0xA2 );
    EUSART2_Write( nCom );
    EUSART2_Write( 0x01 );
    EUSART2_Write( nData );
    tx2_checksum = ( u8 )0 - tx2_checksum;
    EUSART2_Write( tx2_checksum );
}

static void BlueBoard_Send_Data2( u8 nCom, u8 nData )
{
    tx2_checksum = 0;
    EUSART2_Write( 0xA2 );
    EUSART2_Write( nCom );
    EUSART2_Write( 0x02 );
    EUSART2_Write( 0 );
    EUSART2_Write( nData );
    tx2_checksum = 0 - tx2_checksum;
    EUSART2_Write( tx2_checksum );
}

void Blue_Value_Re( u8 nData ) // 接收数据
{
    static u8 nBuf[32];
    static u8 nRxCnt = 0, nChecksum = 0;
    static u16 nRxLen;
    if ( nRxCnt == 0 )
    {
        if ( nData == 0xa3 ) // 接收到头码
        {
            nBuf[nRxCnt++] = nData;
            nChecksum = nData;
            nRxLen = 0;
        }
    }
    else if ( nRxCnt <= 2 )
    {
        nBuf[nRxCnt++] = nData;
        nChecksum += nData;
        nRxLen <<= 8;
        nRxLen |= nData;
        if ( nRxLen > 20 ) // 超出长度
            nRxCnt = 0;
    }
    else
    {
        nBuf[nRxCnt++] = nData;
        nChecksum += nData;
        if ( nRxLen )
            nRxLen--;
        else   // 接收完成
        {
            nRxCnt = 0;
            if ( nChecksum == 0 ) // 校验正确
            {
                nPlay_State = nBuf[3] & 0x03;
                nPlay_Device = ( nBuf[3] >> 2 ) & 0x03;
                nPlay_Mode = nBuf[5] & 0x07;
                nPlay_ID = nBuf[8];
                nPlay_nVolume = nBuf[4] & 0x0f;
                nBlue_Con = nBuf[15] & 0x30; // 蓝牙状态
                blue_a3_ok = 1;
            }
        }
    }
}

void Blue_Ascii_Re( u8 nData ) // 接收字符串
{
    static u8 nBuf[64];
    static u8 nRxCnt = 0;
    nBuf[nRxCnt++] = nData;
    if ( nData == '\n' )
    {
        nRxCnt = 0;
        if ( User_found_strs( ( char * )nBuf, "ON!", '\n' ) == 1 )
        {
            if ( BlueOk )
                Pt_Reset_ID( TASK_ID_BLUE ); // 蓝牙任务【1】复位
        }
        else if ( User_found_strs( ( char * )nBuf, "BT_OK", '\n' ) == 1 )
            Blue_At_Re |= 0x01;
        else if ( User_found_strs( ( char * )nBuf, "BREDR_DIS", '\n' ) == 1 )
            Blue_At_Re |= 0x02;
        else if ( User_found_strs( ( char * )nBuf, "NA:", '\n' ) == 1 )
        {
            if ( User_found_strs( ( char * )nBuf, Blue_Name, '\n' ) == 0 )
            {
                Blue_At_Re |= 0x08;
            }
        }
        else if ( User_found_strs( ( char * )nBuf, "BREDR=EN", '\n' ) == 1 )
            Blue_At_Re |= 0x10;
        else if ( User_found_strs( ( char * )nBuf, "LE=EN", '\n' ) == 1 )
            Blue_At_Re |= 0x20;
        else if ( User_found_strs( ( char * )nBuf, "RECON=EN", '\n' ) == 1 )
            Blue_At_Re |= 0x40;
        else if ( User_found_strs( ( char * )nBuf, "TONE=EN", '\n' ) == 1 )
            Blue_At_Re |= 0x80;
        else if ( User_found_strs( ( char * )nBuf, "LE_CONNECTED", '\n' ) == 1 )
            Blue_At_Re |= 0x04;
    }
    if ( nData == 0 || nData > 127 || nRxCnt >= 64 )
    {
        nRxCnt = 0;
    }
}

#if (USE_CSD_MSUIC)
////删除
#endif // USE_CSD_MSUIC

void os_task_BlueRx( void ) // 蓝牙串口处理
{
    u8 nData;
    while ( COM2.RX_read != COM2.RX_write )
    {
        nData = EUSART2_Read();
        Blue_Value_Re( nData );
        Blue_Ascii_Re( nData );
#if (USE_CSD_MSUIC)
App_Value_Re( nData );
#endif
#if (USE_BLUE_APP)
App_Value_Re(nData);
#endif
}
}

        void os_task_blue( u8 nTask_ID )
        {
            // KEIL一般分配临时变量在RAM不在堆栈
            // 因此为了防止任务之间改写凡是作用域跨越os_delay()应作为static
            Pt_Start
            BlueOk = 0;
            Pt_Wait_Sig( VoicePlayID > 0 );
            Pt_Pause_ID( TASK_ID_VOICE ); // 暂停语音播报任务ID号 1
            // 等待启动，成功去到下一步，失败重启
            //  Blue_Over_Time = 0;
            while ( ( Blue_At_Re & 0x03 ) == 0 )
            {
                Pt_Wait_Tick( 3 );
                if ( ++Blue_Over_Time > 250 )
                {
                    Blue_Over_Time = 0;
                    Blue_At_Re = 0;
                    PrintString2( "AT+RESET\r\n" ); // 重启
                    Pt_Reset();
                }
            }
            // 查询蓝牙名称和参数(经典蓝牙、数据蓝牙、自动重连、提示音)
            Blue_At_Re = 0;
            PrintString2( "AT+NM\r\n" );
            Pt_Wait_Tick( 10 );
            PrintString2( "AT+FUN\r\n" );
            Pt_Wait_Tick( 250 );
            blue_Update_bit = 0;
            // 检验蓝牙名称和参数是否正确，不正确则写入
            if ( Blue_At_Re & 0x04 )
            {
                //        TM_ACK_Value = TM_ACK_Version_Server  ;
                Timer_1s_Cnt = 0;
                modeflag = 1;
            }
            if ( modeflag && Timer_1s_Cnt >= 1 )
            {
                //        TM_ACK_Value =  TM_ACK_Model_App ;
                modeflag = 0;
            }
            if ( Blue_At_Re & 0x08 ) // 蓝牙名称
            {
                PrintString2( "AT+NM=" );
                PrintString2( Blue_Name );
                PrintString2( "\r\n" );
                Pt_Wait_Tick( 50 );
                blue_Update_bit = 1;
            }
            if ( Blue_At_Re & 0x10 ) // 经典蓝牙
            {
                if ( nBlueState == 0 )
                {
                    PrintString2( "AT+FUN=BREDR=DIS\r\n" );
                    Pt_Wait_Tick( 50 );
                    blue_Update_bit = 1;
                }
            }
            else
            {
                if ( nBlueState )
                {
                    PrintString2( "AT+FUN=BREDR=EN\r\n" );
                    Pt_Wait_Tick( 50 );
                    blue_Update_bit = 1;
                }
            }
#if (USE_CSD_MSUIC)
if ( Blue_At_Re & 0x20 ) // 数据蓝牙
{
if ( nBlueState == 0 )
{
PrintString2( "AT+FUN=LE=DIS\r\n" );
Pt_Wait_Tick( 50 );
blue_Update_bit = 1;
}
}
else
{
if ( nBlueState )
{
PrintString2( "AT+FUN=LE=EN\r\n" );
Pt_Wait_Tick( 50 );
blue_Update_bit = 1;
}
}
#else
//    if (Blue_At_Re & 0x20) //数据蓝牙
//    {
//        PrintString5("AT+FUN=LE=DIS\r\n");
//        Pt_Wait_Tick(50);
//        blue_Update_bit = 1;
//    }
if ( Blue_At_Re & 0x20 ) // 数据蓝牙
{
if ( nBlueState == 0 )
{
PrintString2( "AT+FUN=LE=DIS\r\n" );
Pt_Wait_Tick( 50 );
blue_Update_bit = 1;
}
}
else
{
if ( nBlueState )
{
PrintString2( "AT+FUN=LE=EN\r\n" );
Pt_Wait_Tick( 50 );
blue_Update_bit = 1;
}
}
#endif
if ( Blue_At_Re & 0x40 ) // 自动连接
{
}
else
{
PrintString2( "AT+FUN=RECON=EN\r\n" );
Pt_Wait_Tick( 50 );
blue_Update_bit = 1;
}
if ( Blue_At_Re & 0x80 ) // 提示音
{
}
else
{
PrintString2( "AT+FUN=TONE=EN\r\n" );
Pt_Wait_Tick( 50 );
blue_Update_bit = 1;
}
if ( nBlueState )
BlueBoard_Send_Data1( 0x13, 0x04 ); // 蓝牙
else
BlueBoard_Send_Data1( 0x13, 0x01 ); // SD
Pt_Wait_Tick( 50 );
if ( blue_Update_bit ) // 蓝牙设置有更新就重启
{
// 重启
Blue_At_Re = 0;
PrintString2( "AT+RESET\r\n" );
Blue_Over_Time = 0;
Pt_Reset();
}
//    blue_Update_bit = 1;
BlueBoard_Send_Data1( 0x10, nVolume ); // 音量
Pt_Wait_Tick( 50 );
Pt_Reset_ID( TASK_ID_VOICE );
BlueOk = 1;
App_Update_State();
#if (USE_CSD_MSUIC)
while ( 1 )
{
Pt_Wait_Tick( 80 );
if ( AutoModeGet() == A_CSD_MODE )
{
if ( WorkStateGet() == WorkState_Work )
{
if ( CSD_BLUE_STATE != 2 )
CSD_BLUE_STATE = 2;
}
else
{
if ( CSD_BLUE_STATE != 1 )
CSD_BLUE_STATE = 1;
}
}
else
{
if ( CSD_BLUE_STATE != 3 )
CSD_BLUE_STATE = 3;
}
CSD_Data_Update();
}
#else
Pt_Pause();
#endif // USE_CSD_MSUIC
Pt_End
}

            void os_task_voice( u8 nTask_ID )
            {
                // KEIL一般分配临时变量在RAM不在堆栈
                // 因此为了防止任务之间改写凡是作用域跨越os_delay()应作为static
                Pt_Start
                if ( VoicePlayID )
                {
                    nPlay_time = nPlay_time_Set;
                    Voice_DisTime = nPlay_time_Set;
                    BlueBoard_Send_Data2( 0x23, VoicePlayID ); // 插播指令
                    BlueBoard_Send_Data1( 0x10, nVolume );   // 音量
                    Pt_Wait_Tick( 50 );                      // 250ms
                    blue_a3_ok = 0;
                    Pt_Wait_Sig( blue_a3_ok == 0 && nPlay_time > 0 ); // 等待操作完成
                    if ( Voice_DisTime )
                        Voice_DisTime = 4;
                    if ( nPlay_time != 0 )
                        nPlay_time = 0;
                    else
                    {
                        Blue_At_Re = 0;
                        Blue_Over_Time = 251;
                        Pt_Reset_ID( TASK_ID_BLUE ); // 重新配置蓝牙
                    }
                    VoicePlayID = 0;
                    BlueBoard_Send_Data1( 0x10, 0 ); // 音量设置为0
                    Pt_Wait_Sig( nWake_time > 0 ); // 等待唤醒时间到
                    Pt_Wait_Tick( 20 );
                }
                while ( 1 )
                {
                    if ( nBlueState > 0 && nBlue_Con == 0x30 ) // 蓝牙开启并且在播放中（蓝牙播放优先）
                    {
                        if ( nPlay_Device != 0x00 ) // 切换蓝牙
                        {
                            if ( nPlay_State == Play_State_Run ) // 记录SD播放状态
                                bPause = 1;
                            BlueBoard_Send_Data1( 0x13, 0x04 );
                            Pt_Wait_Tick( 10 );
                        }
                    }
                    else
                    {
                        if ( nPlay_Device != 0x02 ) // 切换SD卡
                        {
                            BlueBoard_Send_Data1( 0x13, 0x01 );
                            Pt_Wait_Tick( 100 );
                        }
                        if ( BlueState_Play > 0 && nMusicSW > 0 ) // MP3模式开启
                        {
                            if ( WorkStateGet() == WorkState_Pause ) // 暂停状态保持暂停
                            {
                                if ( nPlay_State == Play_State_Run ) // 暂停
                                {
                                    bPause = 1; // 暂停
                                    BlueBoard_Send_Data1( 0x03, 0x00 );
                                    Pt_Wait_Tick( 10 );
                                }
                            }
                            else
                            {
                                if ( Blue_Play_ID1 != 45 && Blue_Play_ID1 != 46 && Blue_Play_ID1 != 47 ) // todo 屏蔽其他音乐播放
                                {
                                    BlueState_Play = 0;
                                    Blue_Play_ID1 = 0;
                                    Blue_Play_ID2 = 0;
                                }
                                if ( nPlay_Mode != User_Play_Mode ||
                                        nPlay_ID < Blue_Play_ID1 ||
                                        nPlay_ID > Blue_Play_ID2 ||
                                        BlueState_Play == 1 )  // 满足条件从头播放
                                {
                                    BlueState_Play = 2; // 播放中
                                    bPause = 0;
                                    BlueBoard_Send_Data1( 0x12, Play_Mode_One );
                                    Pt_Wait_Tick( 10 );
                                    BlueBoard_Send_Data2( 0x20, Blue_Play_ID1 ); // 先单曲播放目标音乐
                                    Pt_Wait_Tick( 50 );
                                    if ( User_Play_Mode != nPlay_Mode )
                                    {
                                        BlueBoard_Send_Data1( 0x12, User_Play_Mode ); // 再指定播放模式
                                        Pt_Wait_Tick( 20 );
                                    }
                                }
                                if ( nPlay_Mode == Play_Mode_One || nPlay_Mode == Play_Mode_All ) // 播放一次的模式
                                {
                                    if ( bPause ) // 有播放到暂停标志的才重新播放
                                    {
                                        bPause = 0;
                                        BlueBoard_Send_Data2( 0x02, 0x00 );
                                        Pt_Wait_Tick( 10 );
                                    }
                                }
                                else     // 循环模式直接播放
                                {
                                    bPause = 0;
                                    if ( nPlay_State != Play_State_Run ) // 直接播放
                                    {
                                        BlueBoard_Send_Data2( 0x02, 0x00 );
                                        Pt_Wait_Tick( 10 );
                                    }
                                }
                            }
                        }
                        else
                        {
                            if ( nMusicSW == 0 ) // todo 复位 250310
                            {
                                BlueState_Play = 0;
                                Blue_Play_ID1 = 0;
                                Blue_Play_ID2 = 0;
                            }
                            if ( BlueState_Play == 2 )
                                BlueState_Play = 1;
                            if ( nPlay_State == Play_State_Run ) // 暂停
                            {
                                BlueBoard_Send_Data1( 0x03, 0x00 );
                                Pt_Wait_Tick( 10 );
                            }
                        }
                    }
                    if ( nPlay_nVolume != nVolume ) // 音量核对
                    {
                        BlueBoard_Send_Data1( 0x10, nVolume );
                        Pt_Wait_Tick( 10 );
                    }
                    Pt_Wait_Tick( 100 );
                    BlueBoard_Read_Data();
                    Pt_Wait_Tick( 100 );
                }
                Pt_End
            }
            /***
            增加蓝牙app控制接口  LZX 20250319


            **/

#define app_max_rx_len 2048 + 64

            static UnionBit UnionState1;

#define Shoulder_Sac UnionState1.StrackBit.BIT0
#define Back_Sac UnionState1.StrackBit.BIT1
#define Arm1_Sac UnionState1.StrackBit.BIT2
#define Arm2_Sac UnionState1.StrackBit.BIT3
#define Hand_Sac UnionState1.StrackBit.BIT4
#define Hips_Sac UnionState1.StrackBit.BIT5
#define Leg1_Sac UnionState1.StrackBit.BIT6
#define Leg2_Sac UnionState1.StrackBit.BIT7

            /*****变量定义区域 */

            u8 ota_state = 0;
            u16 ota_package = 0;
            u32 ota_size = 0;
            u32 ota_addr = 0;

            u8 App_Update_Cnt = 0, App_Update_Cnt1 = 0;
            TM_ACK_ENUM DeviceInf_Update = TM_ACK_NULL;

            void App_Cmd_Handler( u8 *nbuf );
            
            void Send_App_Data( u8 *nBuf, u8 nLen );
            ErrStatus app_ota_write( u32 write_addr, u8 *p_buffer, u16 num_write );
            u8 ota_found_str( const char *str, const char *compare_str );
            ErrStatus flash_check( u32 app_addr );
            ErrStatus app_ota_erase( u32 write_addr, u32 num_sector );

            void App_Value_Re( u8 nData ) // 接收APP数据
            {
                static u8 nBuf[app_max_rx_len];
                static u8 nChecksum = 0;
                static u16 nRxCnt = 0, nRx_Length = 0;
                if ( nRxCnt == 0 )
                {
                    if ( nData == 0x55 && nBuf[0] == 0xaa ) // 接收到头码
                    {
                        nBuf[1] = nData;
                        nRxCnt = 0x02;
                        nChecksum = 0;
                        nRx_Length = app_max_rx_len - 10;
                    }
                    else
                    {
                        nBuf[0] = nData;
                    }
                }
                else
                {
                    nBuf[nRxCnt++] = nData;
                    if ( nRxCnt == 8 ) // 获取数据包长度
                    {
                        nRx_Length = nBuf[6];
                        nRx_Length <<= 8;
                        nRx_Length |= nData + 11;
                        if ( nRx_Length >= app_max_rx_len - 1 )
                        {
                            nRxCnt = 0;
                            return;
                        }
                    }
                    if ( nRxCnt < ( nRx_Length - 2 ) ) // 校验和 累加
                    {
                        nChecksum += nData;
                    }
                    if ( nRxCnt == ( nRx_Length - 2 ) )
                    {
                        if ( nChecksum != nData )
                        {
                            nRxCnt = 0;
                            return;
                        }
                    }
                    if ( nRxCnt >= nRx_Length ) // 接收完成
                    {
                        if ( nBuf[nRxCnt - 2] == 0x0d && nData == 0x0a ) // 接收完成
                        {
                            App_Cmd_Handler( nBuf );
                        }
                        nRxCnt = 0;
                    }
                }
            }

            void App_Cmd_Handler( u8 *nbuf )
            {
                u8 nDevice = nbuf[2];
                if ( nDevice == TM_Device_Server ) // 服务器
                {
                    u8 nCmd = nbuf[3];
                    u8 nParam = nbuf[4];
                    u16 nLen = ( ( u16 )nbuf[6] << 8 ) +
                               ( ( u16 )nbuf[7] );
                    switch ( nCmd )
                    {
                    case 0xf1: // 信息查询
                        DeviceInf_Update |= TM_ACK_Version_Server;
                        break;
                    case 0xf4:              // 升级
                        if ( nParam == 0x01 ) // start
                        {
                            if ( ota_state != ota_state_start )
                            {
                                ota_size = ( ( u32 )nbuf[8] << 24 ) +
                                           ( ( u32 )nbuf[9] << 16 ) +
                                           ( ( u32 )nbuf[10] << 8 ) +
                                           ( ( u32 )nbuf[11] );
                                if ( ota_size <= APP_SIZE && ( app_ota_erase( TEMPLATE_START_ADDR, APP_SIZE / SECTOR_SIZE ) == SUCCESS ) )
                                {
                                    ota_state = ota_state_start;
                                    ota_addr = TEMPLATE_START_ADDR;
                                    ota_package = 0;
                                    DeviceInf_Update = TM_ACK_Valid_Sever;
                                    App_Update_State();
                                }
                                else
                                {
                                    DeviceInf_Update = TM_ACK_DisValid_Sever;
                                    App_Update_State();
                                }
                            }
                            else
                            {
                                DeviceInf_Update = TM_ACK_Valid_Sever;
                                App_Update_State();
                            }
                        }
                        else if ( nParam == 0x02 )   // data
                        {
                            if ( ota_state == ota_state_start ||
                                    ota_state == ota_state_Data )
                            {
                                ota_state = ota_state_Data;
                                ota_package++;
                                if ( ( ota_package & 0x00ff ) == nbuf[5] && ota_size >= nLen )
                                {
                                    if ( app_ota_write( ota_addr, &nbuf[8], nLen ) == SUCCESS )
                                    {
                                        ota_addr += nLen;
                                        ota_size -= nLen;
                                        DeviceInf_Update = TM_ACK_Valid_Sever;
                                        App_Update_State();
                                        return;
                                    }
                                }
                            }
                            DeviceInf_Update = TM_ACK_DisValid_Sever;
                            App_Update_State();
                        }
                        else if ( nParam == 0x03 )   // end
                        {
                            if ( ota_state != ota_state_Data ||
                                    ota_size != 0 ||
                                    flash_check( TEMPLATE_START_ADDR ) == ERROR )
                            {
                                DeviceInf_Update |= TM_ACK_DisValid_Sever;
                                App_Update_State();
                            }
                            u32 flash_addr = TEMPLATE_START_ADDR;
                            u32 flash_check = 0;
                            u8 mode_check = 0;
                            if ( nLen >= 4 )
                            {
                                while ( flash_addr < ota_addr )
                                {
                                    if ( mode_check == 0 ) // 型号查询
                                    {
                                        mode_check = ota_found_str( ( ( char * )flash_addr ), OTA_MODE );
                                    }
                                    flash_check += *( ( u8 * )flash_addr );
                                    flash_addr++;
                                }
                                u32 ota_check = ( ( u32 )nbuf[8] << 24 ) +
                                                ( ( u32 )nbuf[9] << 16 ) +
                                                ( ( u32 )nbuf[10] << 8 ) +
                                                ( ( u32 )nbuf[11] );
                                if ( ota_check != flash_check )
                                {
                                    DeviceInf_Update = TM_ACK_DisValid_Sever;
                                    App_Update_State();
                                    return;
                                }
                            }
                            if ( nLen >= 5 && nbuf[12] == 1 && mode_check == 0 ) // 型号校验
                            {
                                DeviceInf_Update = TM_ACK_DisValid_Sever;
                                App_Update_State();
                                return;
                            }
                            //write_upgrade_flag();
                            DeviceInf_Update = TM_ACK_Valid_Sever;
                            App_Update_State();
                            // APP_WaitCompleted();
                            NVIC_SystemReset();
                        }
                        break;
                    default:
                        break;
                    }
                }
                else if ( nDevice == TM_Device_App )   // 服务器
                {
                    Key.State = nbuf[5];
                    Key.Param = nbuf[4];
                    Key.Value = 0;
                    switch ( nbuf[3] )
                    {
                    case 0xf1: // inf
                        DeviceInf_Update |= TM_ACK_Version_Server;
                        break;
                    case 0xe4: // 型号读取
                        DeviceInf_Update |= TM_ACK_Model_App;
                        break;
                    case 0x30:
                        Key.Value = Key_Power;
                        App_Update_Cnt = 0;
                        break;
                    case 0x31:
                        Key.Value = Key_Shoulder;
                        App_Update_Cnt = 0;
                        break;
                    case 0x32:
                        if ( Key.Param == 0x01 )
                        {
                            Key.Value = Key_ShoulderAdj_Jump;
                        }
                        else if ( Key.Param == 0x02 )
                        {
                            Key.Value = Key_BodyCheck_Jump;
                        }
                        else if ( Key.Param == 0x03 )
                        {
                            Key.Value = key_AutoLyJump;
                        }
                        else if ( Key.Param == 0x04 )
                        {
                            Key.Value = Key_BodyCheck_Jump;
                        }
                        App_Update_Cnt = 0;
                        break;
                    case 0x34:
                        Key.Value = Key_Pause;
                        App_Update_Cnt = 0;
                        break;
                    case 0x35:
                        Key.Value = Key_Timing;
                        if ( Key.Param == 0 )
                        {
                            Key.Param = 2;
                        }
                        App_Update_Cnt = 0;
                        break;
                    case 0x3e: // 背景音乐，语音播报
                        if ( Key.Param == 0x10 )
                        {
                            Key.Value = Key_Player;
                            Key.Param = 0;
                        }
                        else if ( Key.Param == 0x11 )
                        {
                            Key.Value = Key_Player;
                            Key.Param = 1;
                        }
                        else
                        {
                            Key.Value = Key_Music;
                        }
                        App_Update_Cnt = 2;
                        App_Update_Cnt1 = 2;
                        break;
                    case 0x3f:
                        Key.Value = Key_Volume;
                        if ( Key.Param > 15 )
                        {
                            Key.Param = 15;
                        }
                        App_Update_Cnt = 2;
                        App_Update_Cnt1 = 2;
                        break;
                    case 0x40:
                        if ( Key.Param == 0xc1 )
                        {
                            Key.Value = Key_Memory_Use;
                            Key.Param = 0;
                        }
                        else if ( Key.Param == 0xc2 )
                        {
                            Key.Value = Key_Memory_Use;
                            Key.Param = 1;
                        }
                        else if ( Key.Param == 0xc3 )
                        {
                            Key.Value = Key_Memory_Use;
                            Key.Param = 2;
                        }
                        else
                        {
                            Key.Value = Key_AutoMode;
                        }
                        break;
                    case 0x41: // 按摩区域
                        if ( Key.Param == 3 )
                        {
                            Key.Param = 5;
                        }
                        else if ( Key.Param == 4 )
                        {
                            Key.Param = 3;
                        }
                        else if ( Key.Param == 5 )
                        {
                            Key.Param = 4;
                        }
                        if ( WorkStateGet() == WorkState_Work )
                        {
                            // custom_State = 0;
                            if ( AutoModeGet() != 0 )
                            {
                                UserFunc_Set( UserFunc_ManualMode, 0 );
                            }
                            //ManualArea_Load((ManualArea_ENUM)Key.Param, CurTreadCircle);
                            MoveUpState.ManualArea_ID = Key.Param;
                            MassAreaSet( MoveUp, ( MassageArea_E )MoveUpState.ManualArea_ID );
                            Buzzer = SOUND_ACK;
                        }
                        App_Update_Cnt = 0;
                        break;
                    case 0x42:
                        Key.Value = Key_Wise;
                        if ( Key.Param > 7 )
                        {
                            Key.Param--;
                        }
                        App_Update_Cnt = 0;
                        break;
                    case 0x43:
                        Key.Value = Key_BackSpeed;
                        App_Update_Cnt = 0;
                        break;
                    case 0x44:
                        Key.Value = Key_BackWidth;
                        App_Update_Cnt = 0;
                        break;
                    case 0x45:
                        Key.Value = Key_BackStrength;
                        App_Update_Cnt = 1;
                        break;
                    case 0x46: // 全身气压
                        Key.Value = Key_AppAir;
                        Key.Param = 1;
                        App_Update_Cnt = 1;
                        break;
                    case 0x47: // 气压强度
                        if ( Key.Param < 0x10 )
                        {
                            Key.Value = Key_AppAir;
                            Key.Param = 2;
                        }
                        else
                        {
                            Key.Value = Key_UpperAirStrength;
                        }
                        App_Update_Cnt = 1;
                        break;
                    case 0x48: // 气压强度
                        if ( Key.Param < 0x10 )
                        {
                            Key.Value = Key_AppAir;
                            Key.Param = 3;
                        }
                        else
                        {
                            if ( AirModeGet() != AirModeFull )
                            {
                                Key.Value = Key_UpperAirStrength;
                            }
                        }
                        App_Update_Cnt = 1;
                        break;
                    case 0x4c: // 背部加热
                        Key.Value = Key_BackHeat;
                        Key.Param = 0x10;
                        App_Update_Cnt = 1;
                        break;
                    case 0x4e: // 小腿加热
                        Key.Value = Key_LegsHeat;
                        Key.Param = 0x10;
                        App_Update_Cnt = 1;
                        break;
                    case 0x4f: // ROLL
                        if ( WorkStateGet() == WorkState_Work )
                        {
                            if ( RollSpeedGet() > 3 )
                            {
                                RollSpeedSet( 3 );
                            }
                            else if ( RollSpeedGet() > 1 )
                            {
                                RollSpeedSet( 1 );
                            }
                            else if ( RollSpeedGet() > 0 )
                            {
                                RollSpeedSet( 0 );
                            }
                            else
                            {
                                RollSpeedSet( 5 );
                            }
                            //RollSwitch_Tick = 0;
                            Buzzer = SOUND_ACK;
                        }
                        App_Update_Cnt = 1;
                        break;
                    case 0x4a:
                        Key.Value = Key_LegsKnead;
                        break;
                    case 0x50:
                        Key.Value = Key_Lying;
                        break;
                    case 0x51:
                    case 0x54:
                        Key.Value = Key_LegsFlex;
                        break;
                    case 0x52:
                        Key.Value = Key_BackLying;
                        break;
                    case 0x53:
                        Key.Value = Key_LegsUpDown;
                        break;
                    case 0x55:
                        Key.Value = Key_Lying;
                        break;
                    case 0x56:
                        Key.Value = Key_Zero;
                        break;
                    case 0x57:
                        Key.Value = Key_BackTread;
                        break;
                    case 0x58:
                        Key.Value = Key_Voice;
                        Key.Param = !nVoiceControlSw;
                        App_Update_Cnt = 1;
                        break;
                    case 0x59:
                        Key.Value = Key_Voice;
                        Key.Param = 0x10;
                        App_Update_Cnt = 1;
                        break;
                    case 0x5b:
                        Key.Value = Key_Blue;
                        Key.Param = !nBlueState;
                        App_Update_Cnt = 1;
                        break;
                    case 0x5e:
                        Key.Value = Key_Health;
                        App_Update_Cnt = 0;
                        break;
                    case 0xe0:
                        Key.Param >>= 4;
                        if ( Key.Param >= 1 && Key.Param <= 3 )
                        {
                            Key.Value = Key_Memory_Save;
                        }
                        break;
                    default:
                        break;
                    }
                    if ( Key.Value )
                    {
                        KeyFunc();
                    }
                }
            }


            void App_Update_State( void )
            {
                u16 i;
                u8 Display_Data[20];
                if ( DeviceInf_Update & TM_ACK_Model_App )
                {
                    DeviceInf_Update &= ~TM_ACK_Model_App;
                    Display_Data[0] = TM_Device_App;
                    Display_Data[1] = 0xe4;
                    Display_Data[2] = 0x01;
                    Display_Data[3] = 0x01;
                    Display_Data[4] = 5;
                    Display_Data[5] = 'G';
                    Display_Data[6] = 'T';
                    Display_Data[7] = '9';
                    Display_Data[8] = '9';
                    Display_Data[9] = '8';
                    Display_Data[10] = '6';
                    Send_App_Data( Display_Data, 11 );
                    return;
                }
                if ( DeviceInf_Update & TM_ACK_Version_Server )
                {
                    DeviceInf_Update &= ~TM_ACK_Version_Server;
                    Display_Data[0] = TM_Device_Server;
                    Display_Data[1] = 0xf1;
                    Display_Data[2] = 0x00;
                    Display_Data[3] = 0x01;
                    Display_Data[4] = 0xa1;
                    Display_Data[5] = UserCode_data >> 24;
                    Display_Data[6] = UserCode_data >> 16;
                    Display_Data[7] = UserCode_data >> 8;
                    Display_Data[8] = UserCode_data;
                    Display_Data[9] = 1; // 1.0 固件版本
                    Display_Data[10] = 0;
                    Display_Data[11] = 1; // 1.0 硬件版本
                    Display_Data[12] = 0;
                    Display_Data[13] = 0; // ERR
                    Display_Data[14] = 0;
                    Display_Data[15] = 0;
                    Display_Data[16] = 1;
                    Send_App_Data( Display_Data, 17 );
                    return;
                }
                if ( DeviceInf_Update & TM_ACK_Valid_Sever )
                {
                    DeviceInf_Update &= ~TM_ACK_Valid_Sever;
                    Display_Data[0] = TM_Device_Server;
                    Display_Data[1] = 0Xe6; // 命令
                    Display_Data[2] = 0X01; // 参数
                    Display_Data[3] = 0X00; // 状态
                    Display_Data[4] = ( u8 )( OTA_PACKAGE_SIZE >> 8 );
                    Display_Data[5] = ( u8 )OTA_PACKAGE_SIZE;
                    Display_Data[6] = ( u8 )( OTA_PACKAGE_DELAY >> 8 );
                    Display_Data[7] = ( u8 )OTA_PACKAGE_DELAY;
                    Send_App_Data( Display_Data, 8 );
                    Enter_Pause();
                    return;
                }
                if ( DeviceInf_Update & TM_ACK_DisValid_Sever )
                {
                    DeviceInf_Update &= ~TM_ACK_DisValid_Sever;
                    Display_Data[0] = TM_Device_Server;
                    Display_Data[1] = 0Xe6; // 命令
                    Display_Data[2] = 0X00; // 参数
                    Display_Data[3] = 0X00; // 状态
                    Display_Data[4] = 0x00; //
                    Send_App_Data( Display_Data, 5 );
                    ota_state = ota_state_stop;
                    ota_package = 0;
                    ota_size = 0;
                    ota_addr = 0;
                    Exit_Pause();
                    return;
                }
                // APP Update
                switch ( App_Update_Cnt )
                {
                case 0:
                    Display_Data[0] = 0x02;
                    Display_Data[1] = 0xe5;
                    Display_Data[2] = 0xF1;
                    Display_Data[3] = 0x01;
                    // if (Health_State > 0 &&  WorkStateGet() != WorkState_FCT)
                    //     Display_Data[4] = (WorkState_HealthPre - 1) + Health_State;
                    // else
                    Display_Data[4] =  WorkStateGet();
                    Display_Data[5] = WorkMinGet();
                    Display_Data[6] = WorkSecGet();
                    Display_Data[7] = ( ( 10 - ShoulderValue ) << 4 ) | ( BackPosUpGet() );
                    Display_Data[8] = AutoModeGet();
                    if ( MoveUpState.ManualArea_ID == 1 )
                    {
                        Display_Data[9] = 1;
                    }
                    else if ( MoveUpState.ManualArea_ID == 2 )
                    {
                        Display_Data[9] = 5;
                    }
                    else if ( MoveUpState.ManualArea_ID == 3 )
                    {
                        Display_Data[9] = 3;
                    }
                    else if ( MoveUpState.ManualArea_ID == 4 )
                    {
                        Display_Data[9] = 4;
                    }
                    else if ( MoveUpState.ManualArea_ID == 5 )
                    {
                        Display_Data[9] = 2;
                    }
                    //nWiseDisplay具体什么含义？
                    /*
                    if (nWiseDisplay > 7)
                    {
                      Display_Data[10] = nWiseDisplay + 1;
                    }
                    else
                    {
                      Display_Data[10] = nWiseDisplay;
                    }
                      */
                    Display_Data[11] = ( MassSpeedGet( MoveUp ) << 4 ) | MassWidthGet( MoveUp );
                    //气囊对于的部位需要重新适配
                    if ( AirPartGet().Part2.Bit.Ankle )
                    {
                        Shoulder_Sac = 1;
                    }
                    else
                    {
                        Shoulder_Sac = 0;
                    }
                    if ( AirPartGet().Part2.Bit.Ankle )
                    {
                        Arm1_Sac = 1;
                        Arm2_Sac = 1;
                    }
                    else
                    {
                        Arm1_Sac = 0;
                        Arm2_Sac = 0;
                    }
                    // if ((UpperValveData & TLR) && Pump_State > 0)
                    //  Back_Sac = 1;
                    if ( AirPartGet().Part2.Bit.Ankle )
                    {
                        Leg1_Sac = 1;
                        Leg2_Sac = 1;
                    }
                    else
                    {
                        Leg1_Sac = 0;
                        Leg2_Sac = 0;
                    }
                    Display_Data[12] = UnionState1.Strack;
                    Send_App_Data( Display_Data, 13 );
                    break;
                case 1:
                    Display_Data[0] = 0x02;
                    Display_Data[1] = 0xe5;
                    Display_Data[2] = 0xF2;
                    Display_Data[3] = 0x01;
                    i = nBackIntensity + 1;
                    i <<= 4;
                    //气压模式和气压强度都是只有一个参数
                    if ( AirModeGet() )
                    {
                        i |= AirIntensityGet();
                    }
                    Display_Data[4] = i;
                    //上半身和下半身气压强度暂时不需要
                    /*
                    if (nCurAirState & Air_State_Upper)
                    {
                      i = AirIntensity;
                    }
                    i <<= 4;
                    if (nCurAirState & Air_State_Lower)
                    {
                      i |= AirIntensity;
                    }
                    Display_Data[5] = i;
                    */
                    i = RollSpeedGet();
                    i = ( i + 1 ) / 2;
                    Display_Data[6] = ( i << 4 ); // | nLegKnead;
                    i = 0;
                    if ( HeatStateGet( BackHeat ) )
                    {
                        i |= 0x80;
                    }
                    Display_Data[7] = i;
                    i = 0;
                    if ( nLCState )
                    {
                        i |= 0x40;
                    }
                    if ( nVoiceControlSw )
                    {
                        i |= 0x08;
                    }
                    if ( nBlueState )
                    {
                        i |= 0x02;
                    }
                    if ( HeatStateGet( LegHeat ) )
                    {
                        i |= 0x01;
                    }
                    Display_Data[8] = i;
                    Display_Data[9] = 0;
                    Display_Data[10] = 0;
                    Display_Data[11] = 0;
                    Display_Data[12] = 0;
                    Send_App_Data( Display_Data, 13 );
                    break;
                case 2:
                    switch ( App_Update_Cnt1 )
                    {
                    // case 0:
                    //     Display_Data[0] = 0x02;
                    //     Display_Data[1] = 0xe5;
                    //     Display_Data[2] = 0xF3;
                    //     Display_Data[3] = 0x01;
                    //     Display_Data[4] = Health_Heart;
                    //     Display_Data[5] = Health_Blood;
                    //     Display_Data[6] = 0;
                    //     Display_Data[7] = Health_WXH;
                    //     Display_Data[8] = Health_Tired;
                    //     i = 0;
                    //     if (Health_Tired > 0)
                    //     {
                    //         if (Health_Tired > 30) // 健康
                    //             i = 1;
                    //         else if (Health_Tired >= 25) // 轻度疲劳
                    //             i = 2;
                    //         else if (Health_Tired >= 20) // 中度疲劳
                    //             i = 3;
                    //         else // 重度疲劳
                    //             i = 4;
                    //     }
                    //     Display_Data[9] = i;
                    //     Display_Data[10] = Health_Cnt * 4;
                    //     Display_Data[11] = 0;
                    //     Display_Data[12] = 0;
                    //     Send_App_Data(Display_Data, 13);
                    //     break;
                    case 1:
                        Display_Data[0] = 0x02;
                        Display_Data[1] = 0xe5;
                        Display_Data[2] = 0xF4;
                        Display_Data[3] = 0x01;
                        Display_Data[4] = 0;
                        Display_Data[5] = 0;
                        Display_Data[6] = 0;
                        Display_Data[7] = 0;
                        Display_Data[8] = 0;
                        Display_Data[9] = 0;
                        Display_Data[10] = 0;
                        Display_Data[11] = 0;
                        Display_Data[12] = 0;
                        Send_App_Data( Display_Data, 13 );
                        break;
                    case 2:
                        Display_Data[0] = 0x02;
                        Display_Data[1] = 0xe5;
                        Display_Data[2] = 0xF6;
                        Display_Data[3] = 0x01;
                        Display_Data[4] = UserIntensityGet();
                        Display_Data[5] = 0; // nM2BackPosDisplay;
                        Display_Data[6] = 0;
                        i = nVolume;
                        if ( nVoicePlayer )
                        {
                            i |= 0x40;
                        }
                        if ( nMusicSW )
                        {
                            i |= 0x80;
                        }
                        Display_Data[7] = i;
                        Display_Data[8] = 0;
                        Display_Data[9] = 0;
                        Display_Data[10] = 0;
                        Display_Data[11] = 0;
                        Display_Data[12] = 0;
                        Send_App_Data( Display_Data, 13 );
                        break;
                    default:
                        break;
                    }
                    if ( ++App_Update_Cnt1 > 2 )
                    {
                        App_Update_Cnt1 = 0;
                    }
                    break;
                default:
                    break;
                }
                if ( ++App_Update_Cnt > 2 )
                {
                    App_Update_Cnt = 0;
                }
            }

            u8 ota_found_str( const char *str, const char *compare_str )
            {
                while ( *str > 0 )
                {
                    if ( *compare_str != *str )
                    {
                        return 0;
                    }
                    str++;
                    compare_str++;
                    if ( *compare_str == 0 )
                    {
                        return 1;
                    }
                }
                return 0;
            }

            ErrStatus flash_check( u32 app_addr )
            {
                /* check app starting address whether 0x08xxxxxx */
                if ( ( ( *( u32 * )( app_addr + 4 ) ) & 0xFF000000 ) != 0x08000000 )
                {
                    return ERROR;
                }
                /* check the address of stack */
                if ( ( ( *( u32 * )app_addr ) - 0x20000000 ) <= ( SRAM_SIZE * 1024 ) )
                {
                    return SUCCESS;
                }
                return ERROR;
            }

            ErrStatus app_ota_erase( u32 write_addr, u32 num_sector )
            {
                ErrStatus  rtn = SUCCESS;
                fmc_unlock();
                for ( uint32_t i = 0; i < num_sector; i++ )
                {
                    fmc_page_erase( write_addr + ( FLASH_PAGE_SIZE * i ) );
                }
                fmc_lock();
                return rtn;
            }

            ErrStatus app_ota_write( u32 write_addr, u8 *p_buffer, u16 num_write )
            {
                if ( ( write_addr % 4 ) != 0 ) //|| writeAddr < AP_ADDR_MIN || writeAddr > AP_ADDR_MAX ) // || (writeAddr + numByteToWrite) > AP_ADDR_MAX
                {
                    return ERROR;
                }
                if ( memcmp( ( uint8_t * )write_addr, p_buffer, num_write ) == 0 )       //如果已经是一样,则不再写入
                {
                    return SUCCESS;
                }
                uint16_t leftLen = 0;
                uint16_t u16Loop = 0;
                uint32_t temp = 0;
                fmc_unlock();
                while ( 1 )
                {
                    if ( u16Loop < num_write )
                    {
                        temp = 0;
                        leftLen = num_write - u16Loop;
                        if ( leftLen >= 4 )
                        {
                            memcpy( &temp, p_buffer, 4 );
                            fmc_word_program( write_addr, temp );
                            /* clear all pending flags */
                            fmc_flag_clear( FMC_FLAG_BANK0_END );
                            fmc_flag_clear( FMC_FLAG_BANK0_WPERR );
                            fmc_flag_clear( FMC_FLAG_BANK0_PGERR );
                            write_addr += 4;
                            p_buffer += 4;
                            u16Loop += 4;
                        }
                        else
                        {
                            memcpy( &temp, p_buffer, leftLen );
                            fmc_word_program( write_addr, temp );
                            /* clear all pending flags */
                            fmc_flag_clear( FMC_FLAG_BANK0_END );
                            fmc_flag_clear( FMC_FLAG_BANK0_WPERR );
                            fmc_flag_clear( FMC_FLAG_BANK0_PGERR );
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                fmc_lock();
                return SUCCESS;
            }
//app数据发送函数
            void Send_App_Data( u8 *nBuf, u8 nLen )
            {
                u8 i;
                if ( nLen >= COM_TX2_Lenth - 7 && nLen < 4 )
                {
                    return;
                }
                // 发送头码
                EUSART2_Write( 0xaa );
                EUSART2_Write( 0x55 );
                // 校验和清0
                tx2_checksum = 0;
                for ( i = 0; i < 4; i++ )
                {
                    EUSART2_Write( *nBuf );
                    nBuf++;
                }
                nLen -= 4;
                EUSART2_Write( 0 );
                EUSART2_Write( nLen );
                for ( i = 0; i < nLen; i++ )
                {
                    EUSART2_Write( *nBuf );
                    nBuf++;
                }
                EUSART2_Write( tx2_checksum );
                EUSART2_Write( 0x0d );
                EUSART2_Write( 0x0a );
            }
			
			void APPUpData( void )
{
    if( ota_state == ota_state_end )
    {
        if( bWriteUpGrade == 1 )
        {
            nWriteUpGrade++;
            if( nWriteUpGrade >= 10 )
            {
                bWriteUpGrade = 0;
                nWriteUpGrade = 0;
                ota_state = ota_state_stop;
                NVIC_SystemReset();
            }
        }
    }
    else
    {
        if( bWriteUpGrade == 1 )
        {
            ota_state = ota_state_stop;
        }
        bWriteUpGrade = 0;
        nWriteUpGrade = 0;
    }
}
void App_Update_State_main( void )
{
    //    u16 i;
    u8 Display_Data[20];
    //    if( nAPP_Dis100ms == APP_TICK )
    if( APP_TICK - nAPP_Dis100ms >= 50 )
    {
        APPUpData();
        nAPP_Dis100ms = APP_TICK ;//+ 2;
        if( DeviceInf_Update & 0x10 )
        {
            DeviceInf_Update &= ~0x10;
            Display_Data[0] = TM_Device_Server;
            Display_Data[1] = 0Xe6; // 命令
            Display_Data[2] = 0X01; // 参数
            Display_Data[3] = 0X00; // 状态
            Display_Data[4] = 0x00 ;
            Display_Data[5] = 200;
            Display_Data[6] = ota_page >> 8; //数据包序号高字节
            Display_Data[7] = ota_page;////数据包序号低字节  获取第3包
            Send_App_Data( Display_Data, 8 );
            nAPP_ACK_Tick = RTC_TICK + 20;
            return;
        }
        else
        {
            if( ( ota_state == ota_state_start ) || ( nOnLine_Auto_NO != 0 ) )
            {
                if( nAPP_ACK_Tick == RTC_TICK )
                    //                if( RTC_TICK - nAPP_ACK_Tick >=  20 )
                {
                    DeviceInf_Update = DeviceInf_Update | 0x10;
                }
            }
        }
    }
}
            /******
            蓝牙app控制程序结束
            */




            void Blue_Init( void )
            {
                char Str[11];
                // 变量初始化
                UserCode_data = 0;
                Blue_At_Re = 0;
                Blue_Over_Time = 0; // 单位100ms
                VoicePlayID = 0;    // 播报ID
                nWake_time = 0;
                nPlay_time = 0;
                nPlay_State = 0;
                nPlay_Device = 0;
                nPlay_Mode = 0;
                nPlay_ID = 0;
                nPlay_nVolume = 0;
                nBlue_Con = 0;
                BlueOk = 0;
                nVolume = 6;
                bPause = 0;         // 从播放到暂停的标志
                BlueState_Play = 0; // 播放状态
                Blue_Play_ID1 = 0;  // 初始ID
                Blue_Play_ID2 = 0;
                // User_Play_Mode = Play_Mode_FolderLoop; // 循环模式
                User_Play_Mode = Play_Mode_OneLoop; // todo 单曲循环 250306
                nVoicePlayer = 1; // todo 暂时不确定用途
                nMusicSW = 1;
                nVoiceControlSw = 1; // todo 开启声控
#if (USE_CSD_MSUIC)
CSD_BLUE_STATE = 0x03;
#endif
// 序列号和蓝牙名称获取
// UserCode_data = *(uint32_t *)0x8034000; // 添加编号递增数
// uint16_t pbuf[2];
// GDFLASH_Read(0x8030000, pbuf, 2);
// UserCode_data = pbuf[0];
// UserCode_data <<= 16;
// UserCode_data |= pbuf[1];
UserCode_data = *( uint32_t * )0x8030000; // 添加编号递增数
if ( UserCode_data == 0xffffffff )      // 如果没有初始则默认
UserCode_data = 1111111111;
User_PrintString5_U32( Str, UserCode_data, 6 ); // 打印递增后的编号到字符串
#ifdef A391_ARMKEY
User_Copy_Str( Blue_Name, "A391S-app" ); // 拷贝蓝牙名称前缀
#else
User_Copy_Str( Blue_Name, "A390S-" ); // 拷贝蓝牙名称前缀
#endif
// User_Copy_Str(Blue_Name, "A12308046666");     // 拷贝蓝牙名称前缀
User_Splicing_Str( Blue_Name, Str ); // 拼接完整蓝牙名称
// PT初始化
Pt_Reset_ID( TASK_ID_BLUE ); // 蓝牙
Pt_Pause_ID( TASK_ID_VOICE ); // 播报
}

// todo 删除 250222
//  void VoiceControlPlay(u8 nId)
//  {
//      if (MChair.Bluetooth.VoicePlayer)
//      {
//          Voice_Ack(nId);
//      }
//  }

// void Voice_Ack(u8 nId)
// {
//     if (BlueOk == 0)
//         return;

//     Pt_Reset_ID(TASK_ID_VOICE);
//     VoicePlayID = nId;
// }

                void VoiceControlPlay( u8 nId )
                {
                    if ( BlueOk == 0 || ( nVoicePlayer == 0 ) ) // todo 修改 250222
                    {
                        return;
                    }
                    Pt_Reset_ID( TASK_ID_VOICE );
                    VoicePlayID = nId;
                }

                void Voice_Ack( u8 nId )
                {
                    if ( BlueOk == 0 )
                    {
                        return;
                    }
                    Pt_Reset_ID( TASK_ID_VOICE );
                    VoicePlayID = nId;
                }
