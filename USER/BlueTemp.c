#include "BlueTemp.h"
#include "Memory/Memory.h"

/********
 * blue voice
 * *********/

u32 UserCode_data = 0;

u8 nVolume;         // 音量
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

u8 BlueOk; // 蓝牙版就绪标志

u8 bPause;         // 从播放到暂停的标志
u8 BlueState_Play; // 播放状态 0-停止 1-播放 2-暂停 3- 运行
u8 Blue_Play_ID1;  // 初始ID
u8 Blue_Play_ID2;
Play_Mode_enum User_Play_Mode; // 循环模式

u8 BT_STATE_READ; // 开始读取蓝牙状态标志

UnionBit BT_STATE;
#define BT_OK BT_STATE.StrackBit.BIT0
#define BT_LE BT_STATE.StrackBit.BIT1
#define BT_BREDR BT_STATE.StrackBit.BIT2
#define BT_RECON BT_STATE.StrackBit.BIT3
#define BT_TONE BT_STATE.StrackBit.BIT4
#define BT_NAME BT_STATE.StrackBit.BIT5

u8 Blue_Ascii_Cnt;

u8 Blue_Connect_State;

#ifdef USE_CSD_MSUIC
u8 CSD_BLUE_STATE;
#endif // USE_CSD_MSUIC

/***************
 * 打印32位无符号整型到字符串
 *  *Str目标字符串
 *  nValue需要打印的数据
 *  nlen打印的长度1~10,
 * ******************/
void User_PrintString2_U32(char *Str, u32 nValue, u8 nlen)
{
    if (nlen == 0 || nlen > 10)
    {
        return;
    }
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
    {
        Str1++;
    }
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
            {
                break;
            }
            i++;
            if (_str[i] == '\0' || _str[i] == _str1)
            {
                return 1;
            }
            if (str[i] == '\0' || str[i] == _str1)
            {
                return 0;
            }
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
        {
            nRxCnt = 0;
        }
    }
    else
    {
        nBuf[nRxCnt++] = nData;
        nChecksum += nData;
        if (nRxLen)
        {
            nRxLen--;
        }
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
            }
        }
    }
}

void Blue_Ascii_Re(u8 nData) // 接收字符串
{
    static u8 nBuf[64];
    nBuf[Blue_Ascii_Cnt++] = nData;
    if (nData == '\n')
    {
        Blue_Ascii_Cnt = 0;
        if (User_found_strs((char *)nBuf, "ON!", '\n') == 1)
        {
            if (BlueOk)
            {
                Pt_Reset_ID(TASK_ID_BLUE); // 蓝牙任务【1】复位
            }
        }
        else if (User_found_strs((char *)nBuf, "BT_OK", '\n') == 1)
        {
            BT_OK = 1;
        }
        else if (User_found_strs((char *)nBuf, "BREDR_DIS", '\n') == 1)
        {
            BT_OK = 1;
        }
        else if (User_found_strs((char *)nBuf, "NA:", '\n') == 1)
        {
            if (User_found_strs((char *)nBuf, Blue_Name, '\n') == 1)
            {
                BT_NAME = 1;
            }
        }
        else if (User_found_strs((char *)nBuf, "BREDR=EN", '\n') == 1)
        {
            BT_BREDR = 1;
        }
        else if (User_found_strs((char *)nBuf, "LE=EN", '\n') == 1)
        {
            BT_LE = 1;
        }
        else if (User_found_strs((char *)nBuf, "RECON=EN", '\n') == 1)
        {
            BT_RECON = 1;
        }
        else if (User_found_strs((char *)nBuf, "TONE=EN", '\n') == 1)
        {
            BT_TONE = 1;
        }
    }
    if (nData == 0 || nData > 127 || Blue_Ascii_Cnt >= 64)
    {
        Blue_Ascii_Cnt = 0;
    }
}

#ifdef USE_CSD_MSUIC
static void CSD_Data_Update(void)
{
    tx2_checksum = 0;
    EUSART2_Write(0x06);
    EUSART2_Write(0x02);
    EUSART2_Write(CSD_BLUE_STATE);
    EUSART2_Write(0);
    EUSART2_Write(tx2_checksum);
}

void App_Value_Re(u8 nData) // 接收APP数据
{
    static u8 nBuf[8];
    static u8 nRxCnt = 0, nChecksum = 0;
    if (nRxCnt == 0)
    {
        if (nData == 0x02 && nBuf[0] == 0x06) // 接收到头码
        {
            nBuf[1] = nData;
            nRxCnt = 0x02;
            nChecksum = 0x08;
        }
        else
        {
            nBuf[0] = nData;
        }
    }
    else
    {
        nBuf[nRxCnt++] = nData;
        if (nRxCnt >= 5) // 接收完成
        {
            nRxCnt = 0;
            if (nChecksum == nData) // 校验正确
            {
                if (nBuf[2] == 0x01) // 开机
                {
                    if (WorkStateGet() == WorkState_Pause)
                    {
                        return;
                    }
                    if (!nPowerOn)
                    {
                        UserFunc_Set(UserFunc_PowerOn, 0);
                        Voice_Ack(VC_PowerOn);
                    }
                    // AutoModeSet(20);       //todo 修改 250218
                    AutoPreModeSet(Update_Enable); //* 修改
                    Buzzer = SOUND_KJ;
                }
                else if (nBuf[2] == 0x03) // 关机
                {
                    if (nPowerOn)
                    {
                        UserFunc_Set(UserFunc_PowerOff, 0);
                        Buzzer = SOUND_GJ;
                        Voice_Ack(VC_PowerOff);
                    }
                }
            }
        }
        else
        {
            nChecksum += nData;
        }
    }
}
#elif defined(USE_BLUE_APP)

TM_ACK_ENUM DeviceInf_Update = TM_ACK_NULL;

void Send_App_Data(u8 *nBuf, u8 nLen)
{
    u8 i;
    if (nLen >= COM_TX2_Lenth - 7 && nLen < 4)
    {
        return;
    }
    // 发送头码
    EUSART2_Write(0xaa);
    EUSART2_Write(0x55);
    // 校验和清0
    tx2_checksum = 0;
    for (i = 0; i < 4; i++)
    {
        EUSART2_Write(*nBuf);
        nBuf++;
    }
    nLen -= 4;
    EUSART2_Write(0);
    EUSART2_Write(nLen);
    for (i = 0; i < nLen; i++)
    {
        EUSART2_Write(*nBuf);
        nBuf++;
    }
    EUSART2_Write(tx2_checksum);
    EUSART2_Write(0x0d);
    EUSART2_Write(0x0a);
}

u8 ota_state = 0;
u16 ota_package = 0;
u32 ota_size = 0;
u32 ota_addr = 0;

u8 App_Update_Cnt = 0, App_Update_Cnt1 = 0;

void App_Update_State(void)
{
    u16 i;
    u8 Display_Data[20];
    if (DeviceInf_Update & TM_ACK_Model_App)
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
        Send_App_Data(Display_Data, 11);
        return;
    }
    if (DeviceInf_Update & TM_ACK_Version_Server)
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
        Send_App_Data(Display_Data, 17);
        return;
    }
    if (DeviceInf_Update & TM_ACK_Valid_Sever)
    {
        DeviceInf_Update &= ~TM_ACK_Valid_Sever;
        Display_Data[0] = TM_Device_Server;
        Display_Data[1] = 0Xe6; // 命令
        Display_Data[2] = 0X01; // 参数
        Display_Data[3] = 0X00; // 状态
        Display_Data[4] = (u8)(OTA_PACKAGE_SIZE >> 8);
        Display_Data[5] = (u8)OTA_PACKAGE_SIZE;
        Display_Data[6] = (u8)(OTA_PACKAGE_DELAY >> 8);
        Display_Data[7] = (u8)OTA_PACKAGE_DELAY;
        Send_App_Data(Display_Data, 8);
        Enter_Pause();
        return;
    }
    if (DeviceInf_Update & TM_ACK_DisValid_Sever)
    {
        DeviceInf_Update &= ~TM_ACK_DisValid_Sever;
        Display_Data[0] = TM_Device_Server;
        Display_Data[1] = 0Xe6; // 命令
        Display_Data[2] = 0X00; // 参数
        Display_Data[3] = 0X00; // 状态
        Display_Data[4] = 0x00; //
        Send_App_Data(Display_Data, 5);
        ota_state = ota_state_stop;
        ota_package = 0;
        ota_size = 0;
        ota_addr = 0;
        Exit_Pause();
        return;
    }
    // APP Update
    switch (App_Update_Cnt)
    {
    case 0:
        Display_Data[0] = 0x02;
        Display_Data[1] = 0xe5;
        Display_Data[2] = 0xF1;
        Display_Data[3] = 0x01;
        // if (Health_State > 0 && nCurWorkState != WorkState_FCT)
        //     Display_Data[4] = (WorkState_HealthPre - 1) + Health_State;
        // else
        Display_Data[4] = nCurWorkState;
        Display_Data[5] = Tick_Work_Min;
        Display_Data[6] = Tick_Work_S;
        Display_Data[7] = ((10 - nShoulderValue) << 4) | (nBackPosDisplay);
        Display_Data[8] = nCurAutoMode;
        if (ManualArea_ID == 1)
        {
            Display_Data[9] = 1;
        }
        else if (ManualArea_ID == 2)
        {
            Display_Data[9] = 5;
        }
        else if (ManualArea_ID == 3)
        {
            Display_Data[9] = 3;
        }
        else if (ManualArea_ID == 4)
        {
            Display_Data[9] = 4;
        }
        else if (ManualArea_ID == 5)
        {
            Display_Data[9] = 2;
        }
        if (nWiseDisplay > 7)
        {
            Display_Data[10] = nWiseDisplay + 1;
        }
        else
        {
            Display_Data[10] = nWiseDisplay;
        }
        Display_Data[11] = (MassSpeed << 4) | MassWidDisplay;
        if (UpperValveData & AL4R4)
        {
            Shoulder_Sac = 1;
        }
        else
        {
            Shoulder_Sac = 0;
        }
        if (UpperValveData & (AL12 | AR12))
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
        if (LowerValveData & VD)
        {
            Leg1_Sac = 1;
            Leg2_Sac = 1;
        }
        else
        {
            Leg1_Sac = 0;
            Leg2_Sac = 0;
        }
        if (LowerValveData & VEGFH)
        {
            Foot_Sac = 1;
        }
        else
        {
            Foot_Sac = 0;
        }
        Display_Data[12] = UnionState1.Strack;
        Send_App_Data(Display_Data, 13);
        break;
    case 1:
        Display_Data[0] = 0x02;
        Display_Data[1] = 0xe5;
        Display_Data[2] = 0xF2;
        Display_Data[3] = 0x01;
        i = nBackIntensity + 1;
        i <<= 4;
        if (nCurAirState & Air_State_Upper)
        {
            i |= AirIntensity;
        }
        Display_Data[4] = i;
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
        i = nCurRollState;
        i = (i + 1) / 2;
        Display_Data[6] = (i << 4); // | nLegKnead;
        i = 0;
        if (nUpperHeat)
        {
            i |= 0x80;
        }
        Display_Data[7] = i;
        i = 0;
        if (nLCState)
        {
            i |= 0x40;
        }
        if (nVoiceState)
        {
            i |= 0x08;
        }
        if (nBlueState)
        {
            i |= 0x02;
        }
        if (nLowerHeat)
        {
            i |= 0x01;
        }
        Display_Data[8] = i;
        Display_Data[9] = 0;
        Display_Data[10] = 0;
        Display_Data[11] = 0;
        Display_Data[12] = 0;
        Send_App_Data(Display_Data, 13);
        break;
    case 2:
        switch (App_Update_Cnt1)
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
            Send_App_Data(Display_Data, 13);
            break;
        case 2:
            Display_Data[0] = 0x02;
            Display_Data[1] = 0xe5;
            Display_Data[2] = 0xF6;
            Display_Data[3] = 0x01;
            Display_Data[4] = User_Intensity;
            Display_Data[5] = 0; // nM2BackPosDisplay;
            Display_Data[6] = 0;
            i = nVolume;
            if (nVoicePlayer)
            {
                i |= 0x40;
            }
            if (nMusicSW)
            {
                i |= 0x80;
            }
            Display_Data[7] = i;
            Display_Data[8] = 0;
            Display_Data[9] = 0;
            Display_Data[10] = 0;
            Display_Data[11] = 0;
            Display_Data[12] = 0;
            Send_App_Data(Display_Data, 13);
            break;
        default:
            break;
        }
        if (++App_Update_Cnt1 > 2)
        {
            App_Update_Cnt1 = 0;
        }
        break;
    default:
        break;
    }
    if (++App_Update_Cnt > 2)
    {
        App_Update_Cnt = 0;
    }
}

static u8 ota_found_str(const char *str, const char *compare_str)
{
    while (*str > 0)
    {
        if (*compare_str != *str)
        {
            return 0;
        }
        str++;
        compare_str++;
        if (*compare_str == 0)
        {
            return 1;
        }
    }
    return 0;
}

error_status flash_check(u32 app_addr)
{
    /* check app starting address whether 0x08xxxxxx */
    if (((*(u32 *)(app_addr + 4)) & 0xFF000000) != 0x08000000)
    {
        return ERROR;
    }
    /* check the address of stack */
    if (((*(u32 *)app_addr) - 0x20000000) <= (SRAM_SIZE * 1024))
    {
        return SUCCESS;
    }
    return ERROR;
}

error_status app_ota_erase(u32 write_addr, u32 num_sector)
{
    u16 i;
    flash_status_type status = FLASH_OPERATE_DONE;
    flash_unlock();
    for (i = 0; i < num_sector; i++)
    {
        status = flash_sector_erase(write_addr);
        if (status != FLASH_OPERATE_DONE)
        {
            return ERROR;
        }
        write_addr += SECTOR_SIZE;
    }
    flash_lock();
    return SUCCESS;
}

error_status app_ota_write(u32 write_addr, u8 *p_buffer, u16 num_write)
{
    u16 index;
    u8 read_data, write_data;
    flash_unlock();
    for (index = 0; index < num_write; index++)
    {
        write_data = p_buffer[index];
        flash_byte_program(write_addr + index, write_data);
        read_data = *(u8 *)(write_addr + index);
        if (read_data != write_data)
        {
            flash_lock();
            return ERROR;
        }
    }
    flash_lock();
    return SUCCESS;
}

void App_Cmd_Handler(u8 *nbuf)
{
    u8 nDevice = nbuf[2];
    if (nDevice == TM_Device_Server) // 服务器
    {
        u8 nCmd = nbuf[3];
        u8 nParam = nbuf[4];
        u16 nLen = ((u16)nbuf[6] << 8) +
                   ((u16)nbuf[7]);
        switch (nCmd)
        {
        case 0xf1: // 信息查询
            DeviceInf_Update |= TM_ACK_Version_Server;
            break;
        case 0xf4:              // 升级
            if (nParam == 0x01) // start
            {
                if (ota_state != ota_state_start)
                {
                    ota_size = ((u32)nbuf[8] << 24) +
                               ((u32)nbuf[9] << 16) +
                               ((u32)nbuf[10] << 8) +
                               ((u32)nbuf[11]);
                    if (ota_size <= APP_SIZE && (app_ota_erase(TEMPLATE_START_ADDR, APP_SIZE / SECTOR_SIZE) == SUCCESS))
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
            else if (nParam == 0x02) // data
            {
                if (ota_state == ota_state_start ||
                    ota_state == ota_state_Data)
                {
                    ota_state = ota_state_Data;
                    ota_package++;
                    if ((ota_package & 0x00ff) == nbuf[5] && ota_size >= nLen)
                    {
                        if (app_ota_write(ota_addr, &nbuf[8], nLen) == SUCCESS)
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
            else if (nParam == 0x03) // end
            {
                if (ota_state != ota_state_Data ||
                    ota_size != 0 ||
                    flash_check(TEMPLATE_START_ADDR) == ERROR)
                {
                    DeviceInf_Update |= TM_ACK_DisValid_Sever;
                    App_Update_State();
                }
                u32 flash_addr = TEMPLATE_START_ADDR;
                u32 flash_check = 0;
                u8 mode_check = 0;
                if (nLen >= 4)
                {
                    while (flash_addr < ota_addr)
                    {
                        if (mode_check == 0) // 型号查询
                        {
                            mode_check = ota_found_str(((char *)flash_addr), OTA_MODE);
                        }
                        flash_check += *((u8 *)flash_addr);
                        flash_addr++;
                    }
                    u32 ota_check = ((u32)nbuf[8] << 24) +
                                    ((u32)nbuf[9] << 16) +
                                    ((u32)nbuf[10] << 8) +
                                    ((u32)nbuf[11]);
                    if (ota_check != flash_check)
                    {
                        DeviceInf_Update = TM_ACK_DisValid_Sever;
                        App_Update_State();
                        return;
                    }
                }
                if (nLen >= 5 && nbuf[12] == 1 && mode_check == 0) // 型号校验
                {
                    DeviceInf_Update = TM_ACK_DisValid_Sever;
                    App_Update_State();
                    return;
                }
                write_upgrade_flag();
                DeviceInf_Update = TM_ACK_Valid_Sever;
                App_Update_State();
                APP_WaitCompleted();
                nvic_system_reset();
            }
            break;
        default:
            break;
        }
    }
    else if (nDevice == TM_Device_App) // 服务器
    {
        Key_State = nbuf[5];
        Key_Param = nbuf[4];
        Key_Value = 0;
        switch (nbuf[3])
        {
        case 0xf1: // inf
            DeviceInf_Update |= TM_ACK_Version_Server;
            break;
        case 0xe4: // 型号读取
            DeviceInf_Update |= TM_ACK_Model_App;
            break;
        case 0x30:
            Key_Value = Key_Power;
            App_Update_Cnt = 0;
            break;
        case 0x31:
            Key_Value = Key_Shoulder;
            App_Update_Cnt = 0;
            break;
        case 0x32:
            if (Key_Param == 0x01)
            {
                Key_Value = Key_ShoulderAdj_Jump;
            }
            else if (Key_Param == 0x02)
            {
                Key_Value = Key_BodyCheck_Jump;
            }
            else if (Key_Param == 0x03)
            {
                Key_Value = key_AutoLyJump;
            }
            else if (Key_Param == 0x04)
            {
                Key_Value = Key_BodyCheck_Jump;
            }
            App_Update_Cnt = 0;
            break;
        case 0x34:
            Key_Value = Key_Pause;
            App_Update_Cnt = 0;
            break;
        case 0x35:
            Key_Value = Key_Timing;
            if (Key_Param == 0)
            {
                Key_Param = 2;
            }
            App_Update_Cnt = 0;
            break;
        case 0x3e: // 背景音乐，语音播报
            if (Key_Param == 0x10)
            {
                Key_Value = Key_Player;
                Key_Param = 0;
            }
            else if (Key_Param == 0x11)
            {
                Key_Value = Key_Player;
                Key_Param = 1;
            }
            else
            {
                Key_Value = Key_Music;
            }
            App_Update_Cnt = 2;
            App_Update_Cnt1 = 2;
            break;
        case 0x3f:
            Key_Value = Key_Volume;
            if (Key_Param > 15)
            {
                Key_Param = 15;
            }
            App_Update_Cnt = 2;
            App_Update_Cnt1 = 2;
            break;
        case 0x40:
            if (Key_Param == 0xc1)
            {
                Key_Value = Key_Memory_Use;
                Key_Param = 0;
            }
            else if (Key_Param == 0xc2)
            {
                Key_Value = Key_Memory_Use;
                Key_Param = 1;
            }
            else if (Key_Param == 0xc3)
            {
                Key_Value = Key_Memory_Use;
                Key_Param = 2;
            }
            else
            {
                Key_Value = Key_AutoMode;
            }
            break;
        case 0x41: // 按摩区域
            if (Key_Param == 3)
            {
                Key_Param = 5;
            }
            else if (Key_Param == 4)
            {
                Key_Param = 3;
            }
            else if (Key_Param == 5)
            {
                Key_Param = 4;
            }
            if (nCurWorkState == WorkState_Work)
            {
                if (MassWise >= JRZ && MassWise <= JRZFB)
                {
                    Buzzer = SOUND_ALARM;
                    return;
                }
                // custom_State = 0;
                if (nCurAutoMode != 0)
                {
                    UserFunc_Set(UserFunc_ManualMode, 0);
                }
                ManualArea_Load((ManualArea_ENUM)Key_Param, CurTreadCircle);
                Buzzer = SOUND_ACK;
            }
            App_Update_Cnt = 0;
            break;
        case 0x42:
            Key_Value = Key_Wise;
            if (Key_Param > 7)
            {
                Key_Param--;
            }
            App_Update_Cnt = 0;
            break;
        case 0x43:
            Key_Value = Key_BackSpeed;
            App_Update_Cnt = 0;
            break;
        case 0x44:
            Key_Value = Key_BackWidth;
            App_Update_Cnt = 0;
            break;
        case 0x45:
            Key_Value = Key_BackStrength;
            App_Update_Cnt = 1;
            break;
        case 0x46: // 全身气压
            Key_Value = Key_AppAir;
            Key_Param = 1;
            App_Update_Cnt = 1;
            break;
        case 0x47: // 气压强度
            if (Key_Param < 0x10)
            {
                Key_Value = Key_AppAir;
                Key_Param = 2;
            }
            else
            {
                Key_Value = Key_UpperAirStrength;
            }
            App_Update_Cnt = 1;
            break;
        case 0x48: // 气压强度
            if (Key_Param < 0x10)
            {
                Key_Value = Key_AppAir;
                Key_Param = 3;
            }
            else
            {
                if (nCurAirState != Air_State_All)
                {
                    Key_Value = Key_UpperAirStrength;
                }
            }
            App_Update_Cnt = 1;
            break;
        case 0x4c: // 背部加热
            Key_Value = Key_BackHeat;
            Key_Param = 0x10;
            App_Update_Cnt = 1;
            break;
        case 0x4e: // 小腿加热
            Key_Value = Key_LegsHeat;
            Key_Param = 0x10;
            App_Update_Cnt = 1;
            break;
        case 0x4f: // ROLL
            if (nCurWorkState == WorkState_Work)
            {
                if (nCurRollState > 3)
                {
                    nCurRollState = 3;
                }
                else if (nCurRollState > 1)
                {
                    nCurRollState = 1;
                }
                else if (nCurRollState > 0)
                {
                    nCurRollState = 0;
                }
                else
                {
                    nCurRollState = 5;
                }
                RollSwitch_Tick = 0;
                Buzzer = SOUND_ACK;
            }
            App_Update_Cnt = 1;
            break;
        case 0x4a:
            Key_Value = Key_LegsKnead;
            break;
        case 0x50:
            Key_Value = Key_Lying;
            break;
        case 0x51:
        case 0x54:
            Key_Value = Key_LegsFlex;
            break;
        case 0x52:
            Key_Value = Key_BackLying;
            break;
        case 0x53:
            Key_Value = Key_LegsUpDown;
            break;
        case 0x55:
            Key_Value = Key_Lying;
            break;
        case 0x56:
            Key_Value = Key_Zero;
            break;
        case 0x57:
            Key_Value = Key_BackTread;
            break;
        case 0x58:
            Key_Value = Key_Voice;
            Key_Param = !nVoiceState;
            App_Update_Cnt = 1;
            break;
        case 0x59:
            Key_Value = Key_Voice;
            Key_Param = 0x10;
            App_Update_Cnt = 1;
            break;
        case 0x5b:
            Key_Value = Key_Blue;
            Key_Param = !nBlueState;
            App_Update_Cnt = 1;
            break;
        case 0x5e:
            Key_Value = Key_Health;
            App_Update_Cnt = 0;
            break;
        case 0xe0:
            Key_Param >>= 4;
            if (Key_Param >= 1 && Key_Param <= 3)
            {
                Key_Value = Key_Memory_Save;
            }
            break;
        default:
            break;
        }
        if (Key_Value)
        {
            Key_Handler();
        }
    }
}

#define app_max_rx_len 2048 + 64

void App_Value_Re(u8 nData) // 接收APP数据
{
    static u8 nBuf[app_max_rx_len];
    static u8 nChecksum = 0;
    static u16 nRxCnt = 0, nRx_Length = 0;
    if (nRxCnt == 0)
    {
        if (nData == 0x55 && nBuf[0] == 0xaa) // 接收到头码
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
        if (nRxCnt == 8) // 获取数据包长度
        {
            nRx_Length = nBuf[6];
            nRx_Length <<= 8;
            nRx_Length |= nData + 11;
            if (nRx_Length >= app_max_rx_len - 1)
            {
                nRxCnt = 0;
                return;
            }
        }
        if (nRxCnt < (nRx_Length - 2)) // 校验和 累加
        {
            nChecksum += nData;
        }
        if (nRxCnt == (nRx_Length - 2))
        {
            if (nChecksum != nData)
            {
                nRxCnt = 0;
                return;
            }
        }
        if (nRxCnt >= nRx_Length) // 接收完成
        {
            if (nBuf[nRxCnt - 2] == 0x0d && nData == 0x0a) // 接收完成
            {
                App_Cmd_Handler(nBuf);
            }
            nRxCnt = 0;
        }
    }
}

#endif // USE_CSD_MSUIC

void os_task_BlueRx(void) // 蓝牙串口处理
{
    u8 nData;
    while (COM2.RX_read != COM2.RX_write)
    {
        nData = EUSART2_Read();
        Blue_Value_Re(nData);
        Blue_Ascii_Re(nData);
#ifdef USE_CSD_MSUIC
        App_Value_Re(nData);
#elif defined(USE_BLUE_APP)
        App_Value_Re(nData);
#endif
        //    EUSART4_Write(nData);
    }
}

void os_task_blue(u8 nTask_ID) //todo 蓝牙 250227
{
    static u8 Blue_Over_Time = 0;
    os_task_BlueRx();
    Pt_Start if (BlueOk)
    {
        if (VoicePlayID)
        {
            return;
        }
        BlueOk = 0;
    }
    Pt_Reset_ID(TASK_ID_VOICE); // 复位语音任务
    // 等待启动
    Blue_Over_Time = 0;
    while (!BT_OK) // 等待启动
    {
        Pt_Wait_Tick(20); // 100ms
        if (++Blue_Over_Time >= 25)
        {
            Blue_Over_Time = 0;
            break;
        }
    }

    BT_STATE_READ = 1;
    // 检验蓝牙名称和参数是否正确，不正确则写入
    while (1)
    {
        if (BT_STATE_READ)
        {
            BT_STATE_READ = 0;
            // 查询蓝牙名称和参数(经典蓝牙、数据蓝牙、自动重连、提示音)
            BT_STATE.Strack = 0;
            Blue_Ascii_Cnt = 0;
            PrintString2("AT+NM\r\n");
            Pt_Wait_Tick(10);
            PrintString2("AT+FUN\r\n");
            Pt_Wait_Tick(100);
        }
        if (BT_NAME == 0) // 蓝牙名称
        {
            PrintString2("AT+NM=");
            PrintString2(Blue_Name);
            PrintString2("\r\n");
            Pt_Wait_Tick(20);
            PrintString2("AT+RESET\r\n");
            BT_STATE.Strack = 0;
            Pt_Reset();
        }
        if (BT_BREDR != nBlueState) // 经典蓝牙
        {
            if (nBlueState)
            {
                PrintString2("AT+FUN=BREDR=EN\r\n");
            }
            else
            {
                PrintString2("AT+FUN=BREDR=DIS\r\n");
            }
            Pt_Wait_Tick(20);
            BT_STATE_READ = 1;
        }
        if (BT_LE != BT_LE_SET) // 数据蓝牙
        {
            if (BT_LE_SET)
            {
                PrintString2("AT+FUN=LE=EN\r\n");
            }
            else
            {
                PrintString2("AT+FUN=LE=DIS\r\n");
            }
            Pt_Wait_Tick(20);
            BT_STATE_READ = 1;
        }
        if (BT_RECON != BT_RECON_SET) // 自动重连
        {
            if (BT_RECON_SET)
            {
                PrintString2("AT+FUN=RECON=EN\r\n");
            }
            else
            {
                PrintString2("AT+FUN=RECON=DIS\r\n");
            }
            Pt_Wait_Tick(20);
            BT_STATE_READ = 1;
        }
        if (BT_TONE != BT_TONE_SET) // 提示音
        {
            if (BT_TONE_SET)
            {
                PrintString2("AT+FUN=TONE=EN\r\n");
            }
            else
            {
                PrintString2("AT+FUN=TONE=DIS\r\n");
            }
            Pt_Wait_Tick(20);
            BT_STATE_READ = 1;
        }
        if (Blue_Connect_State)
        {
            if (!nBlueState)
            {
                nBlueState = 1;
                Flash_Check_Tick = 150;
                PrintString2("AT+FUN=BREDR=EN\r\n");
                Pt_Wait_Tick(700); // 3.5S;
            }
            BlueBoard_Send_Data1(0x0b, 0x00); // 重连
            Pt_Wait_Tick(200);                // 1S
            Blue_Connect_State = 0;
        }
        Pt_Wait_Tick(40);
        BlueOk = 1;
        App_Update_State();
    }
    Pt_End
}

void os_task_voice(u8 nTask_ID)
{
    if (!BlueOk)
    {
        return;
    }
    // KEIL一般分配临时变量在RAM不在堆栈
    // 因此为了防止任务之间改写凡是作用域跨越os_delay()应作为static
    Pt_Start if (VoicePlayID)
    {
        nPlay_time = nPlay_time_Set;
        Voice_DisTime = nPlay_time_Set;
        BlueBoard_Send_Data2(0x23, VoicePlayID);                                                 // 插播指令
        BlueBoard_Send_Data1(0x10, nVolume);                                                     // 音量
        Pt_Wait_Tick(100);                                                                       // 500ms
        Pt_Wait_Sig(nPlay_ID == VoicePlayID && nPlay_State == Play_State_Run && nPlay_time > 0); // 等待操作完成
        if (Voice_DisTime)
        {
            Voice_DisTime = 4;
        }
        if (nPlay_time != 0)
        {
            nPlay_time = 0;
        }
        else
        {
            PrintString2("AT+RESET\r\n"); // 重启蓝牙
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
                {
                    bPause = 1;
                }
                BlueBoard_Send_Data1(0x13, 0x04);
                Pt_Wait_Tick(10);
            }
            if (nPlay_nVolume != 15) // 音量核对
            {
                BlueBoard_Send_Data1(0x10, 15);
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
            if (BlueState_Play > 0 && nMusicSW > 0) // MP3模式开启 //@ nMusicSW --> MChair.Bluetooth.MusicSW
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
                if (BlueState_Play == 2)
                {
                    BlueState_Play = 1;
                }
                if (nPlay_State == Play_State_Run) // 暂停
                {
                    BlueBoard_Send_Data1(0x03, 0x00);
                    Pt_Wait_Tick(10);
                }
            }
            if (nPlay_nVolume != nVolume) // 音量核对
            {
                BlueBoard_Send_Data1(0x10, nVolume);
                Pt_Wait_Tick(10);
            }
        }
        //        if (nPlay_nVolume != nVolume) // 音量核对
        //        {
        //            BlueBoard_Send_Data1(0x10, nVolume);
        //            Pt_Wait_Tick(10);
        //        }
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
    BT_STATE_READ = 0;
    BT_STATE.Strack = 0;
    Blue_Ascii_Cnt = 0;
    Blue_Connect_State = 0;
    UserCode_data = 0;
    VoicePlayID = 0; // 播报ID
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
    User_Play_Mode = Play_Mode_FolderLoop; // 循环模式
#ifdef USE_CSD_MSUIC
    CSD_BLUE_STATE = 0x03;
#endif // USE_CSD_MSUIC
    // 序列号和蓝牙名称获取
    UserCode_data = *(uint32_t *)0x8030000; // 添加编号递增数
    if (UserCode_data == 0xffffffff)        // 如果没有初始则默认
    {
        UserCode_data = 1111111111;
    }
    User_PrintString2_U32(Str, UserCode_data, 4); // 打印递增后的编号到字符串
    User_Copy_Str(Blue_Name, "M1040 PRO ");       // 拷贝蓝牙名称前缀
    User_Splicing_Str(Blue_Name, Str);            // 拼接完整蓝牙名称
    // PT初始化
    Pt_Reset_ID(TASK_ID_BLUE);  // 蓝牙
    Pt_Pause_ID(TASK_ID_VOICE); // 播报
}

//todo 屏蔽测试 
// void VoiceControlPlay(u8 nId)
// {
//     if (BlueOk == 0 || (nVoicePlayer == 0))
//     {
//         return;
//     }
//     Pt_Reset_ID(TASK_ID_VOICE);
//     VoicePlayID = nId;
// }

// void Voice_Ack(u8 nId)
// {
//     if (BlueOk == 0)
//     {
//         return;
//     }
//     Pt_Reset_ID(TASK_ID_VOICE);
//     VoicePlayID = nId;
// }

/****************智能声控***********************/

#define VoiceControl_Receive_Data 7

#define VoiceControl_Real_Read COM3.RX_read
#define VoiceControl_Real_Write COM3.RX_write
#define VoiceControl_GetValue() EUSART3_Read()
#define VoiceControl_SendValue(x) EUSART3_Write(x)

uint8_t VoiceControl_HandBuffer[VoiceControl_Receive_Data];

void Voice_Ack_Cmd(uint8_t value)
{
    u8 CheckSum;
    CheckSum = 0x02 + value;
    VoiceControl_SendValue(0xaa);
    VoiceControl_SendValue(0x55);
    VoiceControl_SendValue(0x02);
    VoiceControl_SendValue(value);
    VoiceControl_SendValue(CheckSum);
    VoiceControl_SendValue(0x0d);
    VoiceControl_SendValue(0x0a);
}

void Voice_Ack_None(void)
{
    VoiceControl_SendValue(0xaa);
    VoiceControl_SendValue(0x55);
    VoiceControl_SendValue(0xc0);
    VoiceControl_SendValue(0x0c);
    VoiceControl_SendValue(0xcc);
    VoiceControl_SendValue(0x0d);
    VoiceControl_SendValue(0x0a);
}

void Voice_WakeUp(u8 nID)
{
    Voice_Ack(nID);
    nWake_time = nWake_time_set;
    VoiceControl_SendValue(0xaa);
    VoiceControl_SendValue(0x55);
    VoiceControl_SendValue(0xe0);
    VoiceControl_SendValue(0x0e);
    VoiceControl_SendValue(0xee);
    VoiceControl_SendValue(0x0d);
    VoiceControl_SendValue(0x0a);
}

void Voice_Swich_Set(u8 x) //todo 屏蔽 250306
{
    // nVoiceState = x;
    // if (nVoiceState)
    // {
    //     Voice_Ack(VC_VcOn);
    // }
    // else
    // {
    //     Voice_Ack(VC_VcOff);
    // }
    // Flash_Check_Tick = Flash_Check_Time;
}

#ifdef VoiceControl_Type_A550

void Voice_Cmd_Handler(uint8_t value)
{
    u8 ack_id;
    if ((!nVoiceState) ||
        nCurWorkState == WorkState_FCT ||
        Voice_DisTime > 0)
    {
        return;
    }
    ack_id = VC_NULL;
    switch (value)
    {
    case 0x0e: //   小艾，小艾
        ack_id = VC_WakeUp;
        break;
    case 0x30: //   结束按摩
        if (nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOff, UserFunc_PowerOff_None);
        }
        ack_id = VC_MassageOff;
        break;
    case 0x31: //   开始按摩
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
            nCurAutoMode = 0;
        }
        ack_id = VC_MassageOn;
        break;
    case 0x32: // 零压舒展
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 1;
        ack_id = VC_Auto1;
        break;
    case 0x33: // 中式推拿
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 2;
        ack_id = VC_Auto2;
        break;
    case 0x34: // 疲劳恢复
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 3;
        ack_id = VC_Auto3;
        break;
    case 0x35: // 海浪轻揉
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 4;
        ack_id = VC_Auto4;
        break;
    case 0x36: // 泰式拉筋
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 5;
        ack_id = VC_Auto5;
        break;
    case 0x37: // 舒筋活络
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 6;
        ack_id = VC_Auto6;
        break;
    case 0x38: // 肩颈护理
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 7;
        ack_id = VC_Auto7;
        break;
    case 0x39: // 腰部护理
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 8;
        ack_id = VC_Auto8;
        break;
    case 0x3a: // 腿足舒缓
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 9;
        ack_id = VC_Auto9;
        break;
    case 0x3b: // 元气唤醒
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 15;
        ack_id = VC_Auto10;
        break;
    case 0x3c: // 安神助眠
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 18;
        ack_id = VC_Auto11;
        break;
    case 0x3d: // 关闭气压
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        Air_Set_Mode(Air_State_Stop);
        ack_id = VC_AirOff;
        break;
    case 0x40: // 打开气压
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (nCurWorkState == WorkState_Work)
        {
            if (nCurAirState == 0)
            {
                Air_Set_Mode(Air_State_All);
            }
            ack_id = VC_AirOn;
        }
        break;
    case 0x41: // 坐姿上升
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        DDG_Set_SitUpCircle(DDG_BCircle_UpDown, DDG_LCircle_UpDown);
        ack_id = VC_SitUp;
        break;
    case 0x42: // 坐姿下降
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        DDG_Set_SitDownCircle(DDG_BCircle_UpDown, DDG_LCircle_UpDown);
        ack_id = VC_SitDown;
        break;
    default:
        break;
    }
    if (ack_id)
    {
        Voice_Ack(ack_id);
    }
    else // 无效
    {
        Voice_Ack(VC_Invalid);
    }
}

#elif defined(VoiceControl_Type_A665_2)

void Voice_Cmd_Handler(uint8_t value)
{
    u8 ack_id, i;
    if ((!nVoiceState) ||
        nCurWorkState == WorkState_FCT ||
        Voice_DisTime > 0)
    {
        return;
    }
    ack_id = VC_NULL;
    switch (value)
    {
    case 0x0e: //   小艾，小艾
        ack_id = VC_WakeUp;
        break;
    case 0x30: //   结束按摩
        if (nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOff, UserFunc_PowerOff_None);
        }
        ack_id = VC_MassageOff;
        break;
    case 0x31: //   开始按摩
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
            nCurAutoMode = Default_AutoMode;
        }
        ack_id = VC_MassageOn;
        break;
    case 0x32: // 零压舒展
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 1;
        ack_id = VC_Auto1;
        break;
    case 0x33: // 元气唤醒
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 15;
        ack_id = VC_Auto2;
        break;
    case 0x34: // 安神助眠
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 18;
        ack_id = VC_Auto3;
        break;
    case 0x35: // 颈肩护理
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 7;
        ack_id = VC_Auto4;
        break;
    case 0x40: // 午间小憩
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 16;
        ack_id = VC_Auto5;
        break;
    case 0x37: // 泰式拉伸
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = 5;
        ack_id = VC_Auto6;
        break;
    case 0x54: // 关闭气压
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        Air_Set_Mode(Air_State_Stop);
        ack_id = VC_AirOff;
        break;
    case 0x53: // 打开气压
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (nCurWorkState == WorkState_Work)
        {
            if (nCurAirState == 0)
            {
                Air_Set_Mode(Air_State_All);
            }
            ack_id = VC_AirOn;
        }
        break;
    case 0x55: // 坐姿上升
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        DDG_Set_SitUpCircle(DDG_BCircle_UpDown, DDG_LCircle_UpDown);
        ack_id = VC_SitUp;
        break;
    case 0x56: // 坐姿下降
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        DDG_Set_SitDownCircle(DDG_BCircle_UpDown, DDG_LCircle_UpDown);
        ack_id = VC_SitDown;
        break;
    case 0x50: // 换种手法
        if (nCurWorkState == WorkState_Work)
        {
            if (nCurAutoMode > 0)
            {
                UserFunc_Set(UserFunc_ManualMode, 0);
                i = 1;
            }
            else
            {
                i = nWiseDisplay + 1;
                if (i > 11)
                {
                    i = 0;
                }
            }
            switch (i)
            {
            default: // 停止
                i = TZ;
                break;
            case 1: // 揉捏
                i = RNZ;
                break;
            case 2: // 拍打1
                i = PDBS;
                break;
            case 3: // 拍打2
                i = PDDS;
                break;
            case 4: // 揉捏拍打
                i = RPZ;
                break;
            case 5: // 指压1
                i = ZYD;
                break;
            case 6: // 指压2
                i = ZYS;
                break;
            case 7: // 肩部提抓
                ManualArea_ID = ManualArea_Fixed;
                ManualArea_Disable();
                i = JRZ;
                break;
            case 8: // 3D1
                i = D3RNZ;
                break;
            case 9: // 3D2
                i = D3RPZ;
                break;
            case 10: // 3D3
                i = D3PDBS;
                break;
            case 11: // 3D4
                i = D3PDDS;
                break;
            }
            M1WiseSet(i);
            Buzzer = SOUND_ACK;
            ack_id = VC_Wise;
        }
        break;
    case 0x57: // 定点按摩
        if (nCurWorkState == WorkState_Work)
        {
            if (nCurAutoMode != 0)
            {
                UserFunc_Set(UserFunc_ManualMode, 0);
            }
            ManualArea_Load(ManualArea_Fixed, CurTreadCircle);
            Buzzer = SOUND_ACK;
            ack_id = VC_Fixed;
        }
        break;
    case 0x51: // 往下一点
        if (nCurWorkState == WorkState_Work &&
            nCurAutoMode == 0 &&
            ManualArea_ID == ManualArea_Fixed &&
            nWiseDisplay != 7)
        {
            i = CurTreadCircle + TreadLocalCircle;
            ManualArea_Load(ManualArea_Fixed, i);
            Buzzer = SOUND_ACK;
            ack_id = VC_Fixed_Down;
        }
        break;
    case 0x52: // 往上一点
        if (nCurWorkState == WorkState_Work &&
            nCurAutoMode == 0 &&
            ManualArea_ID == ManualArea_Fixed &&
            nWiseDisplay != 7)
        {
            if (CurTreadCircle > TreadLocalCircle)
            {
                i = CurTreadCircle - TreadLocalCircle;
            }
            else
            {
                i = 0;
            }
            ManualArea_Load(ManualArea_Fixed, i);
            Buzzer = SOUND_ACK;
            ack_id = VC_Fixed_Up;
        }
        break;
    case 0x58: // 停止调整
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        DDG_Set_Stop();
        ack_id = VC_SitStop;
        break;
    default:
        break;
    }
    if (ack_id)
    {
        Voice_Ack(ack_id);
    }
    else // 无效
    {
        Voice_Ack(VC_Invalid);
    }
}

#elif defined(VoiceControl_Type_A370)

void Voice_Cmd_Handler(uint8_t value)
{
    u8 ack_id, i;
    if ((!nVoiceState) ||
        nCurWorkState == WorkState_FCT ||
        Voice_DisTime > 0)
    {
        return;
    }
    ack_id = VC_NULL;
    switch (value)
    {
    case 0x0e: //   小艾，小艾
        ack_id = VC_WakeUp;
        break;
    case 0x32: //   结束按摩
        if (nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOff, UserFunc_PowerOff_None);
        }
        ack_id = VC_MassageOff;
        break;
    case 0x33: //   结束按摩
        if (nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOff, UserFunc_PowerOff_None);
        }
        ack_id = VC_MassageOff1;
        break;
    case 0x34: //   结束按摩
        if (nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOff, UserFunc_PowerOff_None);
        }
        ack_id = VC_MassageOff2;
        break;
    case 0x30: //   开始按摩
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
            nCurAutoMode = Default_AutoMode;
        }
        ack_id = VC_MassageOn;
        break;
    case 0x31: //   开始按摩
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
            nCurAutoMode = Default_AutoMode;
        }
        ack_id = VC_MassageOn1;
        break;
    case 0x35: // 快速按摩
        //         if (nCurWorkState == WorkState_Pause)
        //             break;
        //         if (!nPowerOn)
        //             UserFunc_Set(UserFunc_PowerOn, 0);
        //         nCurAutoMode = A_A4;
        //         ack_id = VC_Auto1;
        break;
    case 0x36: // 全身按摩
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = A_A4; // 1;
        ack_id = VC_Auto2;
        break;
    case 0x37: // 牵引按摩
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = A_A18; // 18;
        ack_id = VC_Auto3;
        break;
    case 0x38: // 护椎按摩
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = A_A7; // 4;
        ack_id = VC_Auto4;
        break;
    case 0x39: // 肩颈按摩
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = A_A5; // 2;
        ack_id = VC_Auto5;
        break;
    case 0x3A: // 腰背按摩
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = A_A6; // 3;
        ack_id = VC_Auto6;
        break;
    case 0x3B: // 助眠模式
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (!nPowerOn)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
        }
        nCurAutoMode = A_A24; // 11;
        ack_id = VC_Auto7;
        break;
    case 0x3F: // 关闭气压
        if (nCurWorkState == WorkState_Pause || Air_Stre_State > 0)
        {
            break;
        }
        Air_Set_Mode(Air_State_Stop);
        ack_id = VC_AirOff;
        break;
    case 0x3E: // 打开气压
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        if (nCurWorkState == WorkState_Work)
        {
            if (nCurAirState == 0)
            {
                Air_Set_Mode(Air_State_All);
            }
            ack_id = VC_AirOn;
        }
        break;
    case 0x3C: // 换种手法
        if (nCurWorkState == WorkState_Work)
        {
            if (nCurAutoMode > 0)
            {
                UserFunc_Set(UserFunc_ManualMode, 0);
                i = 1;
            }
            else
            {
                i = nWiseDisplay + 1;
                if (i > 11)
                {
                    i = 0;
                }
            }
            switch (i)
            {
            default: // 停止
                i = TZ;
                break;
            case 1: // 揉捏
                i = RNZ;
                break;
            case 2: // 拍打1
                i = PDBS;
                break;
            case 3: // 拍打2
                i = PDDS;
                break;
            case 4: // 揉捏拍打
                i = RPZ;
                break;
            case 5: // 指压1
                i = ZYD;
                break;
            case 6: // 指压2
                i = ZYS;
                break;
            case 7: // 肩部提抓
                ManualArea_ID = ManualArea_Fixed;
                ManualArea_Disable();
                i = JRZ;
                break;
            case 8: // 3D1
                i = D3RNZ;
                break;
            case 9: // 3D2
                i = D3RPZ;
                break;
            case 10: // 3D3
                i = D3PDBS;
                break;
            case 11: // 3D4
                i = D3PDDS;
                break;
            }
            M1WiseSet(i);
            Buzzer = SOUND_ACK;
            ack_id = VC_Wise;
        }
        break;
    case 0x3D: // zero
        if (nCurWorkState == WorkState_Pause)
        {
            break;
        }
        i = nCurZeroState;
        if (++i > 3)
        {
            i = 1;
        }
        DDG_Set_Zero(i);
        Buzzer = SOUND_ACK;
        ack_id = VC_Zero;
        break;
    default:
        break;
    }
    if (ack_id)
    {
        Voice_Ack(ack_id);
    }
    else // 无效
    {
        Voice_Ack(VC_Invalid);
    }
}

#endif // VoiceControl_Type

// todo 有重复暂时屏蔽 250215
//  void VoiceControl_Handler(void)
//  {
//      uint8_t i, CheckSum;
//      while (VoiceControl_Real_Read != VoiceControl_Real_Write)
//      {
//          VoiceControl_HandBuffer[0] = VoiceControl_HandBuffer[1];
//          VoiceControl_HandBuffer[1] = VoiceControl_HandBuffer[2];
//          VoiceControl_HandBuffer[2] = VoiceControl_HandBuffer[3];
//          VoiceControl_HandBuffer[3] = VoiceControl_HandBuffer[4];
//          VoiceControl_HandBuffer[4] = VoiceControl_HandBuffer[5];
//          VoiceControl_HandBuffer[5] = VoiceControl_HandBuffer[6];
//          VoiceControl_HandBuffer[6] = VoiceControl_GetValue();
//          if (VoiceControl_HandBuffer[0] == 0xaa && VoiceControl_HandBuffer[1] == 0x55)
//          {
//              CheckSum = 0;
//              for (i = 2; i < 4; i++)
//              {
//                  CheckSum += VoiceControl_HandBuffer[i];
//              }
//              if (CheckSum == VoiceControl_HandBuffer[4]) // 接收成功
//              {
//                  nWake_time = nWake_time_set;
//                  i = VoiceControl_HandBuffer[3];
//                  if (i != 0x0e) // 唤醒
//                  {
//                      Voice_Ack_Cmd(i);
//                  }
//                  Voice_Cmd_Handler(i);
//              }
//          }
//      }
//  }