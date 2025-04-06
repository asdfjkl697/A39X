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
// #define BT_LE_SET 0	   // 0-关闭数据蓝牙 1- 打开数据蓝牙
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
void User_PrintString5_U32(char *Str, u32 nValue, u8 nlen)
{
    if (nlen == 0 || nlen > 10)
        return;

    Str[nlen] = '\0'; // 增加结束符

    while (nlen > 0)
    {
        nlen--;
        Str[nlen] = nValue % 10 + '0';
        nValue /= 10;
    }
}

void User_Copy_Str(char *Str1, char *Str2)
{
    while (*Str2 != '\0')
    {
        *Str1 = *Str2;
        Str1++;
        Str2++;
    }

    *Str1 = '\0';
}

void User_Splicing_Str(char *Str1, char *Str2)
{
    while (*Str1 != '\0')
        Str1++;

    while (*Str2 != '\0')
    {
        *Str1 = *Str2;
        Str1++;
        Str2++;
    }

    *Str1 = '\0';
}

static u8 User_found_strs(char *str, char *_str, char _str1)
{
    u8 i;

    while (*str != '\0' && *str != _str1)
    {
        i = 0;

        while (_str[i] != '\0' && _str[i] != _str1)
        {
            if (_str[i] != str[i])
                break;

            i++;

            if (_str[i] == '\0' || _str[i] == _str1)
                return 1;

            if (str[i] == '\0' || str[i] == _str1)
                return 0;
        }

        str++;
    }

    return 0;
}

static void BlueBoard_Read_Data(void)
{
    EUSART2_Write(0xa3);
    EUSART2_Write(0x00);
    EUSART2_Write(0x00);
    EUSART2_Write(0x5d);
}

static void BlueBoard_Send_Data1(u8 nCom, u8 nData)
{
    tx2_checksum = 0;
    EUSART2_Write(0xA2);
    EUSART2_Write(nCom);
    EUSART2_Write(0x01);
    EUSART2_Write(nData);
    tx2_checksum = (u8)0 - tx2_checksum;
    EUSART2_Write(tx2_checksum);
}

static void BlueBoard_Send_Data2(u8 nCom, u8 nData)
{
    tx2_checksum = 0;
    EUSART2_Write(0xA2);
    EUSART2_Write(nCom);
    EUSART2_Write(0x02);
    EUSART2_Write(0);
    EUSART2_Write(nData);
    tx2_checksum = 0 - tx2_checksum;
    EUSART2_Write(tx2_checksum);
}

void Blue_Value_Re(u8 nData) // 接收数据
{
    static u8 nBuf[32];
    static u8 nRxCnt = 0, nChecksum = 0;
    static u16 nRxLen;

    if (nRxCnt == 0)
    {
        if (nData == 0xa3) // 接收到头码
        {
            nBuf[nRxCnt++] = nData;
            nChecksum = nData;
            nRxLen = 0;
        }
    }
    else if (nRxCnt <= 2)
    {
        nBuf[nRxCnt++] = nData;
        nChecksum += nData;
        nRxLen <<= 8;
        nRxLen |= nData;

        if (nRxLen > 20) // 超出长度
            nRxCnt = 0;
    }
    else
    {
        nBuf[nRxCnt++] = nData;
        nChecksum += nData;

        if (nRxLen)
            nRxLen--;
        else // 接收完成
        {
            nRxCnt = 0;

            if (nChecksum == 0) // 校验正确
            {
                nPlay_State = nBuf[3] & 0x03;
                nPlay_Device = (nBuf[3] >> 2) & 0x03;
                nPlay_Mode = nBuf[5] & 0x07;
                nPlay_ID = nBuf[8];
                nPlay_nVolume = nBuf[4] & 0x0f;
                nBlue_Con = nBuf[15] & 0x30; // 蓝牙状态
                blue_a3_ok = 1;
            }
        }
    }
}

void Blue_Ascii_Re(u8 nData) // 接收字符串
{
    static u8 nBuf[64];
    static u8 nRxCnt = 0;
    nBuf[nRxCnt++] = nData;

    if (nData == '\n')
    {
        nRxCnt = 0;

        if (User_found_strs((char *)nBuf, "ON!", '\n') == 1)
        {
            if (BlueOk)
                Pt_Reset_ID(TASK_ID_BLUE); // 蓝牙任务【1】复位
        }
        else if (User_found_strs((char *)nBuf, "BT_OK", '\n') == 1)
            Blue_At_Re |= 0x01;
        else if (User_found_strs((char *)nBuf, "BREDR_DIS", '\n') == 1)
            Blue_At_Re |= 0x02;
        else if (User_found_strs((char *)nBuf, "NA:", '\n') == 1)
        {
            if (User_found_strs((char *)nBuf, Blue_Name, '\n') == 0)
            {
                Blue_At_Re |= 0x08;
            }
        }
        else if (User_found_strs((char *)nBuf, "BREDR=EN", '\n') == 1)
            Blue_At_Re |= 0x10;
        else if (User_found_strs((char *)nBuf, "LE=EN", '\n') == 1)
            Blue_At_Re |= 0x20;
        else if (User_found_strs((char *)nBuf, "RECON=EN", '\n') == 1)
            Blue_At_Re |= 0x40;
        else if (User_found_strs((char *)nBuf, "TONE=EN", '\n') == 1)
            Blue_At_Re |= 0x80;
        else if (User_found_strs((char *)nBuf, "LE_CONNECTED", '\n') == 1)
            Blue_At_Re |= 0x04;
    }

    if (nData == 0 || nData > 127 || nRxCnt >= 64)
    {
        nRxCnt = 0;
    }
}

#if (USE_CSD_MSUIC)
////删除
#endif // USE_CSD_MSUIC

void os_task_BlueRx(void) // 蓝牙串口处理
{
    u8 nData;

    while (COM2.RX_read != COM2.RX_write)
    {
        nData = EUSART2_Read();
        Blue_Value_Re(nData);
        Blue_Ascii_Re(nData);
#if (USE_CSD_MSUIC)
        App_Value_Re(nData);
#endif

#if (USE_BLUE_APP)
        AppCom(nData);
#endif
    }
}

void os_task_blue(u8 nTask_ID)
{
    // KEIL一般分配临时变量在RAM不在堆栈
    // 因此为了防止任务之间改写凡是作用域跨越os_delay()应作为static
    Pt_Start
        BlueOk = 0;
    Pt_Wait_Sig(VoicePlayID > 0);
    Pt_Pause_ID(TASK_ID_VOICE); // 暂停语音播报任务ID号 1

    // 等待启动，成功去到下一步，失败重启
    //  Blue_Over_Time = 0;
    while ((Blue_At_Re & 0x03) == 0)
    {
        Pt_Wait_Tick(3);

        if (++Blue_Over_Time > 250)
        {
            Blue_Over_Time = 0;
            Blue_At_Re = 0;
            PrintString2("AT+RESET\r\n"); // 重启
            Pt_Reset();
        }
    }

    // 查询蓝牙名称和参数(经典蓝牙、数据蓝牙、自动重连、提示音)
    Blue_At_Re = 0;
    PrintString2("AT+NM\r\n");
    Pt_Wait_Tick(10);
    PrintString2("AT+FUN\r\n");
    Pt_Wait_Tick(250);
    blue_Update_bit = 0;
    // 检验蓝牙名称和参数是否正确，不正确则写入

    if (Blue_At_Re & 0x04)
    {
        //        TM_ACK_Value = TM_ACK_Version_Server  ;
        Timer_1s_Cnt = 0;
        modeflag = 1;
    }

    if (modeflag && Timer_1s_Cnt >= 1)
    {
        //        TM_ACK_Value =  TM_ACK_Model_App ;
        modeflag = 0;
    }

    if (Blue_At_Re & 0x08) // 蓝牙名称
    {
        PrintString2("AT+NM=");
        PrintString2(Blue_Name);
        PrintString2("\r\n");
        Pt_Wait_Tick(50);
        blue_Update_bit = 1;
    }

    if (Blue_At_Re & 0x10) // 经典蓝牙
    {
        if (nBlueState == 0)
        {
            PrintString2("AT+FUN=BREDR=DIS\r\n");
            Pt_Wait_Tick(50);
            blue_Update_bit = 1;
        }
    }
    else
    {
        if (nBlueState)
        {
            PrintString2("AT+FUN=BREDR=EN\r\n");
            Pt_Wait_Tick(50);
            blue_Update_bit = 1;
        }
    }

#if (USE_CSD_MSUIC)

    if (Blue_At_Re & 0x20) // 数据蓝牙
    {
        if (nBlueState == 0)
        {
            PrintString2("AT+FUN=LE=DIS\r\n");
            Pt_Wait_Tick(50);
            blue_Update_bit = 1;
        }
    }
    else
    {
        if (nBlueState)
        {
            PrintString2("AT+FUN=LE=EN\r\n");
            Pt_Wait_Tick(50);
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

    if (Blue_At_Re & 0x20) // 数据蓝牙
    {
        if (nBlueState == 0)
        {
            PrintString2("AT+FUN=LE=DIS\r\n");
            Pt_Wait_Tick(50);
            blue_Update_bit = 1;
        }
    }
    else
    {
        if (nBlueState)
        {
            PrintString2("AT+FUN=LE=EN\r\n");
            Pt_Wait_Tick(50);
            blue_Update_bit = 1;
        }
    }

#endif

    if (Blue_At_Re & 0x40) // 自动连接
    {
    }
    else
    {
        PrintString2("AT+FUN=RECON=EN\r\n");
        Pt_Wait_Tick(50);
        blue_Update_bit = 1;
    }

    if (Blue_At_Re & 0x80) // 提示音
    {
    }
    else
    {
        PrintString2("AT+FUN=TONE=EN\r\n");
        Pt_Wait_Tick(50);
        blue_Update_bit = 1;
    }

    if (nBlueState)
        BlueBoard_Send_Data1(0x13, 0x04); // 蓝牙
    else
        BlueBoard_Send_Data1(0x13, 0x01); // SD

    Pt_Wait_Tick(50);

    if (blue_Update_bit) // 蓝牙设置有更新就重启
    {
        // 重启
        Blue_At_Re = 0;
        PrintString2("AT+RESET\r\n");
        Blue_Over_Time = 0;
        Pt_Reset();
    }

    //    blue_Update_bit = 1;
    BlueBoard_Send_Data1(0x10, nVolume); // 音量
    Pt_Wait_Tick(50);
    Pt_Reset_ID(TASK_ID_VOICE);
    BlueOk = 1;
#if (USE_CSD_MSUIC)

    while (1)
    {
        Pt_Wait_Tick(80);

        if (nCurAutoMode == A_CSD_MODE)
        {
            if (nCurWorkState == WorkState_Work)
            {
                if (CSD_BLUE_STATE != 2)
                    CSD_BLUE_STATE = 2;
            }
            else
            {
                if (CSD_BLUE_STATE != 1)
                    CSD_BLUE_STATE = 1;
            }
        }
        else
        {
            if (CSD_BLUE_STATE != 3)
                CSD_BLUE_STATE = 3;
        }

        CSD_Data_Update();
    }

#else
    Pt_Pause();
#endif // USE_CSD_MSUIC
    Pt_End
}

void os_task_voice(u8 nTask_ID)
{
    // KEIL一般分配临时变量在RAM不在堆栈
    // 因此为了防止任务之间改写凡是作用域跨越os_delay()应作为static
    Pt_Start

        if (VoicePlayID)
    {
        nPlay_time = nPlay_time_Set;
        Voice_DisTime = nPlay_time_Set;
        BlueBoard_Send_Data2(0x23, VoicePlayID); // 插播指令
        BlueBoard_Send_Data1(0x10, nVolume);     // 音量
        Pt_Wait_Tick(50);                        // 250ms
        blue_a3_ok = 0;
        Pt_Wait_Sig(blue_a3_ok == 0 && nPlay_time > 0); // 等待操作完成

        if (Voice_DisTime)
            Voice_DisTime = 4;

        if (nPlay_time != 0)
            nPlay_time = 0;
        else
        {
            Blue_At_Re = 0;
            Blue_Over_Time = 251;
            Pt_Reset_ID(TASK_ID_BLUE); // 重新配置蓝牙
        }

        VoicePlayID = 0;
        BlueBoard_Send_Data1(0x10, 0); // 音量设置为0
        Pt_Wait_Sig(nWake_time > 0);   // 等待唤醒时间到
        Pt_Wait_Tick(20);
    }

    while (1)
    {
        if (nBlueState > 0 && nBlue_Con == 0x30) // 蓝牙开启并且在播放中（蓝牙播放优先）
        {
            if (nPlay_Device != 0x00) // 切换蓝牙
            {
                if (nPlay_State == Play_State_Run) // 记录SD播放状态
                    bPause = 1;

                BlueBoard_Send_Data1(0x13, 0x04);
                Pt_Wait_Tick(10);
            }
        }
        else
        {
            if (nPlay_Device != 0x02) // 切换SD卡
            {
                BlueBoard_Send_Data1(0x13, 0x01);
                Pt_Wait_Tick(100);
            }

            if (BlueState_Play > 0 && nMusicSW > 0) // MP3模式开启
            {
                if (WorkStateGet() == WorkState_Pause) // 暂停状态保持暂停
                {
                    if (nPlay_State == Play_State_Run) // 暂停
                    {
                        bPause = 1; // 暂停
                        BlueBoard_Send_Data1(0x03, 0x00);
                        Pt_Wait_Tick(10);
                    }
                }
                else
                {
                    if (Blue_Play_ID1 != VC_Music1 && Blue_Play_ID1 != VC_Music2 && Blue_Play_ID1 != VC_Music3) // todo 屏蔽其他音乐播放
                    {
                        BlueState_Play = 0;
                        Blue_Play_ID1 = 0;
                        Blue_Play_ID2 = 0;
                    }

                    if (nPlay_Mode != User_Play_Mode ||
                        nPlay_ID < Blue_Play_ID1 ||
                        nPlay_ID > Blue_Play_ID2 ||
                        BlueState_Play == 1) // 满足条件从头播放
                    {
                        BlueState_Play = 2; // 播放中
                        bPause = 0;
                        BlueBoard_Send_Data1(0x12, Play_Mode_One);
                        Pt_Wait_Tick(10);
                        BlueBoard_Send_Data2(0x20, Blue_Play_ID1); // 先单曲播放目标音乐
                        Pt_Wait_Tick(50);

                        if (User_Play_Mode != nPlay_Mode)
                        {
                            BlueBoard_Send_Data1(0x12, User_Play_Mode); // 再指定播放模式
                            Pt_Wait_Tick(20);
                        }
                    }

                    if (nPlay_Mode == Play_Mode_One || nPlay_Mode == Play_Mode_All) // 播放一次的模式
                    {
                        if (bPause) // 有播放到暂停标志的才重新播放
                        {
                            bPause = 0;
                            BlueBoard_Send_Data2(0x02, 0x00);
                            Pt_Wait_Tick(10);
                        }
                    }
                    else // 循环模式直接播放
                    {
                        bPause = 0;

                        if (nPlay_State != Play_State_Run) // 直接播放
                        {
                            BlueBoard_Send_Data2(0x02, 0x00);
                            Pt_Wait_Tick(10);
                        }
                    }
                }
            }
            else
            {
                if (nMusicSW == 0) // todo 复位 250310
                {
                    BlueState_Play = 0;
                    Blue_Play_ID1 = 0;
                    Blue_Play_ID2 = 0;
                }

                if (BlueState_Play == 2)
                    BlueState_Play = 1;

                if (nPlay_State == Play_State_Run) // 暂停
                {
                    BlueBoard_Send_Data1(0x03, 0x00);
                    Pt_Wait_Tick(10);
                }
            }
        }

        if (nPlay_nVolume != nVolume) // 音量核对
        {
            BlueBoard_Send_Data1(0x10, nVolume);
            Pt_Wait_Tick(10);
        }

        Pt_Wait_Tick(100);
        BlueBoard_Read_Data();
        Pt_Wait_Tick(100);
    }

    Pt_End
}

void Blue_Init(void)
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
    UserCode_data = *(uint32_t *)FLASH_SN_ADDR; // 添加编号递增数
    if (UserCode_data == 0xffffffff)        // 如果没有初始则默认
        UserCode_data = 2503255123;

    User_PrintString5_U32(Str, UserCode_data, 10); // 打印递增后的编号到字符串
    // #ifdef A391_ARMKEY
    //     User_Copy_Str(Blue_Name, "A391S-"); // 拷贝蓝牙名称前缀
    // #else
    //     User_Copy_Str(Blue_Name, "A390S-"); // 拷贝蓝牙名称前缀
    // #endif

#ifdef A391S_1
    User_Copy_Str(Blue_Name, "A1"); // 拷贝蓝牙名称前缀
#elif defined A391S_5
    User_Copy_Str(Blue_Name, "A391S-5");
#elif defined A391S_6
    User_Copy_Str(Blue_Name, "A391S-6");
#elif defined A390S_1
    User_Copy_Str(Blue_Name, "IMCM-");
#elif defined A390S_2
    User_Copy_Str(Blue_Name, "A390S-2");
#elif defined A390S_5A
    User_Copy_Str(Blue_Name, "A390S-5A");
#elif defined A390S_5B
    User_Copy_Str(Blue_Name, "A390S-5B");
#else
    User_Copy_Str(Blue_Name, "A390S");
#endif
    User_Splicing_Str(Blue_Name, Str); // 拼接完整蓝牙名称
    // PT初始化
    Pt_Reset_ID(TASK_ID_BLUE);  // 蓝牙
    Pt_Pause_ID(TASK_ID_VOICE); // 播报
Pt_Reset_ID(TASK_ID_APP);   // 蓝牙
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

void VoiceControlPlay(u8 nId)
{
    if (BlueOk == 0 || (nVoicePlayer == 0)) // todo 修改 250222
    {
        return;
    }
    Pt_Reset_ID(TASK_ID_VOICE);
    VoicePlayID = nId;
}

void Voice_Ack(u8 nId)
{
    if (BlueOk == 0) // todo 语音播报 没有关闭修改
    {
        return;
    }
    if (nVoicePlayer == 0 && nId > VC_SitDown) // todo 语音播报 不关闭声控对应词条 250320
    {
        return;
    }
    Pt_Reset_ID(TASK_ID_VOICE);
    VoicePlayID = nId;
}
