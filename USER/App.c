#include "App.h"

u8 App_Update_Cnt = 0;
bool App_Update_Flag = 0;

u8 AT_Auto21[AutoLoad_SIZE] = {0};
u8 AT_Auto22[AutoLoad_SIZE] = {0};
u8 AT_Auto23[AutoLoad_SIZE] = {0};
u8 AT_Auto24[AutoLoad_SIZE] = {0};
u8 AT_Auto25[AutoLoad_SIZE] = {0}; // * 增加

bool Online_Err = 0;         // * 在线程序出错标志
bool Write_Upgrade_Flag = 0; // * 升级结束标志 250418
u32 Write_Upgrade_Tick = 0;
// u8 *AT_Auto16;

u8 flash_read_AutoOnline(uint32_t read_addr, uint8_t *p_buffer, uint16_t num_read)
{
    uint16_t i;
    if (*(uint8_t *)(read_addr) == 0xFF)
    {
        return 0;
    }
    for (i = 0; i < num_read; i++)
    {
        p_buffer[i] = *(uint8_t *)(read_addr);
        read_addr += 1;
    }
    return 1;
}

#define SECTOR_SIZE 2048 /* this parameter depends on the specific model of the chip */

/**
 * @brief  check flash upgrade flag.
 * @param  none
 * @retval none
 */
FlagStatus flash_upgrade_flag_read(void)
{
    if ((*(u32 *)OTA_UPGRADE_FLAG_ADDR) == OTA_UPGRADE_FLAG)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}

/**
 * @brief  write flash upgrade flag.
 * @param  none
 * @retval none
 */
void write_upgrade_flag(void)
{
    fmc_unlock();
    fmc_page_erase(OTA_UPGRADE_FLAG_ADDR);
    fmc_word_program(OTA_UPGRADE_FLAG_ADDR, OTA_UPGRADE_FLAG);
    fmc_lock();
}

/**
 * @brief  ota init.
 * @param  none
 * @retval none
 */
void ota_init(void)
{
    if ((*(uint32_t *)OTA_UPGRADE_FLAG_ADDR) == OTA_UPGRADE_FLAG)
    {
        fmc_unlock();
        fmc_page_erase(OTA_UPGRADE_FLAG_ADDR);
        fmc_lock();
    }
}

/**
 * @}
 */

/**
 * @}
 */

ErrStatus app_ota_erase(u32 write_addr, u32 num_sector)
{
    ErrStatus rtn = SUCCESS;
    fmc_unlock();
    for (uint32_t i = 0; i < num_sector; i++)
    {
        fmc_page_erase(write_addr + (SECTOR_SIZE * i));
    }
    fmc_lock();
    return rtn;
}

ErrStatus app_ota_write(u32 write_addr, u8 *p_buffer, u16 num_write)
{
    if ((write_addr % 4) != 0) //|| writeAddr < AP_ADDR_MIN || writeAddr > AP_ADDR_MAX ) // || (writeAddr + numByteToWrite) > AP_ADDR_MAX
    {
        return ERROR;
    }
    if (memcmp((uint8_t *)write_addr, p_buffer, num_write) == 0) // 如果已经是一样,则不再写入
    {
        return SUCCESS;
    }
    uint16_t leftLen = 0;
    uint16_t u16Loop = 0;
    uint32_t temp = 0;
    fmc_unlock();
    while (1)
    {
        if (u16Loop < num_write)
        {
            temp = 0;
            leftLen = num_write - u16Loop;
            if (leftLen >= 4)
            {
                memcpy(&temp, p_buffer, 4);
                fmc_word_program(write_addr, temp);
                /* clear all pending flags */
                fmc_flag_clear(FMC_FLAG_BANK0_END);
                fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
                fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
                write_addr += 4;
                p_buffer += 4;
                u16Loop += 4;
            }
            else
            {
                memcpy(&temp, p_buffer, leftLen);
                fmc_word_program(write_addr, temp);
                /* clear all pending flags */
                fmc_flag_clear(FMC_FLAG_BANK0_END);
                fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
                fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
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

u16 TM_ACK_Value = 0; // 设备回复
u8 ota_state = 0;
u16 ota_package = 0;
u32 ota_size_num = 0;     // 总数
u32 ota_size_package = 0; // 分包数
u32 ota_addr = 0;
u16 ota_over_time = 0;
u32 ota_size_cnt = 0;

u8 AutoLoad_state = 0;
u16 AutoLoad_package = 0;
u32 AutoLoad_size_num = 0;     // 总数
u32 AutoLoad_size_package = 0; // 分包数
u32 AutoLoad_addr = 0;
u16 AutoLoad_over_time = 0;
u16 AutoLoad_size_cnt[5] = {0}; // * 修改 250413
u8 AutoLoad_TempCnt = 0;
u8 AutoLoad_RealCnt = 0;

#define AutoLoad_state_stop 0
#define AutoLoad_state_start 1
#define AutoLoad_state_Data 2
#define AutoLoad_state_end 3
#define AutoLoad_over_time_default 10

#define ota_state_stop 0
#define ota_state_start 1
#define ota_state_Data 2
#define ota_state_end 3
#define ota_over_time_default 10

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

void Send_App_Data(u8 *nBuf, u8 nLen)
{
    // Arm_App_Update(nBuf, nLen);
}

void App_Update_Cnt_Set(u8 cnt)
{
    App_Update_Cnt = cnt;
}

void App_Update_State(void)
{
    // static u8 App_Update_Cnt, App_Update_Cnt1;
    u32 i;
    u8 temp = 0, temp1 = 0;
    // APP Update
    switch (App_Update_Cnt)
    {
    case 0:
        u8 Display_Data_F1[20] = {0};
        Display_Data_F1[0] = 0xAA;
        Display_Data_F1[1] = 0x55;
        Display_Data_F1[2] = 0x02;
        Display_Data_F1[3] = 0xe5;
        Display_Data_F1[4] = 0xF1;
        Display_Data_F1[5] = 0x01;
        Display_Data_F1[6] = 0x00;
        Display_Data_F1[7] = 0x09;
        if (ota_state == ota_state_start)
        {
            Display_Data_F1[8] = 0x0D;
        }
        else
        {
            Display_Data_F1[8] = WorkStateGet();
        }
        // 按摩时间
        Display_Data_F1[9] = WorkMinGet(); // * 修改 250406 不-1
        Display_Data_F1[10] = WorkSecGet();
        // u8 m1pos_app = BackPosUpGet() - 1;
        // if (m1pos_app > 7) // todo md 260924
        //     m1pos_app = 7;
        uint16_t M1Circle = GetMoveDataTx(MoveUp)->Tread.CircleH << 8 | GetMoveDataTx(MoveUp)->Tread.CircleL;
        u8 m1pos_app = M1Circle / 66;
        Display_Data_F1[11] = ((10 - ShoulderValue) << 4) | m1pos_app; // * 10->11 修改 有问题 250408
        Display_Data_F1[12] = AutoModeGet();
        // if (AutoModeGet() >= 16 && AutoModeGet() <= 19)
        // {
        //     Display_Data_F1[12] = 209 + AutoModeGet();
        // }
        if (AutoModeGet() >= 21 && AutoModeGet() <= 25)
        {
            // Display_Data_F1[12] = 224 + AutoModeGet();
            if (AutoLoad_RealCnt > 5) // * 舌诊、体脂秤 250418
                Display_Data_F1[12] = 209 + AutoModeGet();
            else
                Display_Data_F1[12] = 204 + AutoModeGet();
        }
        // 按摩区域
        if (AutoModeGet() == 0)
        {
            if (MoveUpState.ManualArea_ID == Area_Upper)
            {
                Display_Data_F1[13] = 3;
            }
            else if (MoveUpState.ManualArea_ID == Area_Lower)
            {
                Display_Data_F1[13] = 4;
            }
            else if (MoveUpState.ManualArea_ID == Area_Full)
            {
                Display_Data_F1[13] = 2;
            }
            else if (MoveUpState.ManualArea_ID == Area_Fixed)
            {
                Display_Data_F1[13] = 1;
            }
            else if (MoveUpState.ManualArea_ID == Area_Part)
            {
                Display_Data_F1[13] = 5;
            }
        }
        else
        {
            Display_Data_F1[13] = 0;
        }
        // 上机芯按摩手法
        switch (MassWiseGet(MoveUp))
        {
        default:
        case TZ: // 停止
            Display_Data_F1[14] = 0;
            break;
        case RNZ: // 揉捏
            Display_Data_F1[14] = 1;
            break;
        case PD: // 拍打2
            Display_Data_F1[14] = 2;
            break;
        case RPZ: // 揉捏拍打
            Display_Data_F1[14] = 4;
            break;
        case PDB: // 敲击
            Display_Data_F1[14] = 8;
            break;
        case ZYS: // 指压
            Display_Data_F1[14] = 5;
            break;
        case D3RNZ: // 3D揉捏
            Display_Data_F1[14] = 0x20;
            break;
        case D3PD: // 3D拍打
            Display_Data_F1[14] = 0x21;
            break;
        case D3RPZ: // 3D揉捏拍打
            Display_Data_F1[14] = 0x22;
            break;
        case ZYD: // 3D指压
            Display_Data_F1[14] = 0x24;
            break;
        }

        temp = MassSpeedGet(MoveUp); // * 增加 250419
        if (MassWiseGet(MoveUp) == TZ || MassWiseGet(MoveUp) == ZYS)
            temp = 0;
        temp1 = MassWidthGet(MoveUp) + 1;
        if (MassWiseGet(MoveUp) == RNZ || MassWiseGet(MoveUp) == RPZ || MassWiseGet(MoveUp) == D3RNZ || MassWiseGet(MoveUp) == D3RPZ)
            temp1 = 0;

        // 上机芯按摩速度和宽度
        Display_Data_F1[15] = (temp << 4) | temp1; // * 修改 250406
        /*气囊图标显示根据实际位置调整
         */
        // * 修改 250408
        if (AirPartGet().Part1.Bit.ShoulderL | AirPartGet().Part1.Bit.ShoulderR)
        {
            Shoulder_Sac = 1;
        }
        else
        {
            Shoulder_Sac = 0;
        }
        if (AirPartGet().Part1.Bit.ArmLB | AirPartGet().Part1.Bit.ArmLF | AirPartGet().Part1.Bit.ArmRB | AirPartGet().Part1.Bit.ArmRF)
        {
            Arm1_Sac = 1;
        }
        else
        {
            Arm1_Sac = 0;
        }

        if (AirPartGet().Part2.Bit.CalfBot | AirPartGet().Part2.Bit.CalfSide) // * 修改 250420
        {
            // Leg2_Sac = 1;
            Hips_Sac = 1;
        }
        else
        {
            // Leg2_Sac = 0;
            Hips_Sac = 0;
        }

        if (AirPartGet().Part2.Bit.Ankle | AirPartGet().Part2.Bit.FeetF | AirPartGet().Part2.Bit.FeetSide) // * 修改 250420
        {
            Leg1_Sac = 1;
        }
        else
        {
            Leg1_Sac = 0;
        }

        Display_Data_F1[16] = UnionState1.Strack;
        Display_Data_F1[17] = 0x00;
        Display_Data_F1[18] = 0x0D;
        Display_Data_F1[19] = 0x0A;
        for (u8 i = 0; i < 20; i++)
        {
            EUSART2_Write(Display_Data_F1[i]);
        }
        App_Update_Cnt++;
        break;
    case 1:
        u8 Display_Data_F2[20] = {0};
        Display_Data_F2[0] = 0xAA;
        Display_Data_F2[1] = 0x55;
        Display_Data_F2[2] = 0x02;
        Display_Data_F2[3] = 0xe5;
        Display_Data_F2[4] = 0xF2;
        Display_Data_F2[5] = 0x01;
        Display_Data_F2[6] = 0x00;
        Display_Data_F2[7] = 0x09;
        i = nBackIntensity + 1;
        i <<= 4;
        i |= AirIntensityGet();
        Display_Data_F2[8] = i;
        // 之前的逻辑是每个气压模式强度都是独立的，这里需要调整
        if (AirModeGet() == AirModeUpper)
        {
            Display_Data_F2[9] = AirIntensityGet() << 4;
            Display_Data_F2[9] &= 0xf0;
        }
        else if (AirModeGet() == AirModeLower)
        {
            Display_Data_F2[9] = AirIntensityGet();
            Display_Data_F2[9] &= 0x0f;
        }
        else if (AirModeGet() == AirModeFull || AirModeGet() == AirModeAuto) // * 修改 250406
        {
            Display_Data_F2[9] = (AirIntensityGet() << 4); //+ AirIntensity;
            Display_Data_F2[9] |= AirIntensityGet();
        }
        else
        {
            Display_Data_F2[9] = 0;
        }
        i = RollSpeedGet();

        Display_Data_F2[10] = (i << 4) | (KneadModeGet() > 0); // @ md 260927
        i = 0;

        if (HeatStateGet(BackHeat)) // 背部加热
        {
            i |= 0x80;
        }
        Display_Data_F2[11] = i;
        i = 0;

        if (HeatStateGet(LegHeat)) // 小腿加热
        {
            i |= 0x01;
        }
        if (nLCState) // 老化模式
        {
            i |= 0x40;
        }
        if (nVoiceControlSw) // 声控开关
        {
            i |= 0x08;
        }
        if (nBlueState) // 蓝牙开关
        {
            i |= 0x02;
        }
        Display_Data_F2[12] = i;
        Display_Data_F2[13] = 0;
        Display_Data_F2[14] = 0;
        Display_Data_F2[15] = 0;
        Display_Data_F2[16] = 0;
        Display_Data_F2[17] = 0;
        Display_Data_F2[18] = 0x0D;
        Display_Data_F2[19] = 0x0A;

        for (u8 i = 0; i < 20; i++)
        {
            EUSART2_Write(Display_Data_F2[i]);
        }
        App_Update_Cnt++;
        break;
    case 2:
        u8 Display_Data_F6[20] = {0};
        Display_Data_F6[0] = 0xAA;
        Display_Data_F6[1] = 0x55;
        Display_Data_F6[2] = 0x02;
        Display_Data_F6[3] = 0xe5;
        Display_Data_F6[4] = 0xF6;
        Display_Data_F6[5] = 0x01;
        Display_Data_F6[6] = 0x00;
        Display_Data_F6[7] = 0x09;
        Display_Data_F6[8] = 0; //*B1
        uint16_t M2Circle = GetMoveDataTx(MoveDw)->Tread.CircleH << 8 | GetMoveDataTx(MoveDw)->Tread.CircleL;
        Display_Data_F6[9] = 8 - M2Circle / 72;
        Display_Data_F6[10] = 0;
        Display_Data_F6[11] = 0; //*B4

        Display_Data_F6[12] = (AutoModeGet() == 0); //*B5

        // 下机芯按摩手法
        switch (MassWiseGet(MoveDw)) //*B7
        {
        default:
        case TZ: // 停止
            Display_Data_F6[14] = 0;
            break;
        case RNZ: // 揉捏
            Display_Data_F6[14] = 1;
            break;
        case PD: // 拍打2
            Display_Data_F6[14] = 2;
            break;
        case RPZ: // 揉捏拍打
            Display_Data_F6[14] = 4;
            break;
        case PDB: // 敲击
            Display_Data_F6[14] = 8;
            break;
        }

        temp = MassSpeedGet(MoveDw);
        if (MassWiseGet(MoveDw) == TZ)
            temp = 0;
        temp1 = MassWidthGet(MoveDw) + 1;
        if (MassWiseGet(MoveDw) == RNZ || MassWiseGet(MoveDw) == RPZ)
            temp1 = 0;
        Display_Data_F6[15] = temp << 4 | temp1; //* B8
        Display_Data_F6[16] = 0;                 //* B9
        Display_Data_F6[17] = 0x00;
        Display_Data_F6[18] = 0x0D;
        Display_Data_F6[19] = 0x0A;

        for (u8 i = 0; i < 20; i++)
        {
            EUSART2_Write(Display_Data_F6[i]);
        }
        App_Update_Cnt = 0;
        break;

    case 3: // todo 上传运行时间 260927
        u8 Display_Data_F9[64] = {0};
        Display_Data_F9[0] = 0xAA;
        Display_Data_F9[1] = 0x55;
        Display_Data_F9[2] = 0x02;
        Display_Data_F9[3] = 0xe5;
        Display_Data_F9[4] = 0xF9;
        Display_Data_F9[5] = 0x01;
        Display_Data_F9[6] = 0x00;
        Display_Data_F9[7] = 40;
#ifdef PART_USE_SUM_TIME
        u32 time_get_temp = Part_USE_Get(10);
        Display_Data_F9[8] = time_get_temp & 0xff;
        Display_Data_F9[9] = (time_get_temp >> 8) & 0xff;
        Display_Data_F9[10] = (time_get_temp >> 16) & 0xff;
        Display_Data_F9[11] = (time_get_temp >> 24) & 0xff;
        time_get_temp = Part_USE_Get(0);
        Display_Data_F9[12] = time_get_temp & 0xff; // B5
        Display_Data_F9[13] = (time_get_temp >> 8) & 0xff;
        Display_Data_F9[14] = (time_get_temp >> 16) & 0xff;
        Display_Data_F9[15] = 0;
        Display_Data_F9[16] = 0;
        Display_Data_F9[17] = 0;
        time_get_temp = Part_USE_Get(1);
        Display_Data_F9[18] = time_get_temp & 0xff; // B5
        Display_Data_F9[19] = (time_get_temp >> 8) & 0xff;
        Display_Data_F9[20] = (time_get_temp >> 16) & 0xff;
        time_get_temp = Part_USE_Get(2);
        Display_Data_F9[21] = time_get_temp & 0xff; // B5
        Display_Data_F9[22] = (time_get_temp >> 8) & 0xff;
        Display_Data_F9[23] = (time_get_temp >> 16) & 0xff;
        time_get_temp = Part_USE_Get(3);
        Display_Data_F9[24] = time_get_temp & 0xff; // B5
        Display_Data_F9[25] = (time_get_temp >> 8) & 0xff;
        Display_Data_F9[26] = (time_get_temp >> 16) & 0xff;
        time_get_temp = Part_USE_Get(4);
        Display_Data_F9[27] = time_get_temp & 0xff; // B5
        Display_Data_F9[28] = (time_get_temp >> 8) & 0xff;
        Display_Data_F9[29] = (time_get_temp >> 16) & 0xff;
        time_get_temp = Part_USE_Get(5);
        Display_Data_F9[30] = time_get_temp & 0xff; // B5
        Display_Data_F9[31] = (time_get_temp >> 8) & 0xff;
        Display_Data_F9[32] = (time_get_temp >> 16) & 0xff;
        time_get_temp = Part_USE_Get(6);
        Display_Data_F9[33] = time_get_temp & 0xff; // B5
        Display_Data_F9[34] = (time_get_temp >> 8) & 0xff;
        Display_Data_F9[35] = (time_get_temp >> 16) & 0xff;
        time_get_temp = Part_USE_Get(7);
        Display_Data_F9[36] = time_get_temp & 0xff; // B5
        Display_Data_F9[37] = (time_get_temp >> 8) & 0xff;
        Display_Data_F9[38] = (time_get_temp >> 16) & 0xff;
        time_get_temp = Part_USE_Get(8);
        Display_Data_F9[39] = time_get_temp & 0xff; // B5
        Display_Data_F9[40] = (time_get_temp >> 8) & 0xff;
        Display_Data_F9[41] = (time_get_temp >> 16) & 0xff;
        time_get_temp = Part_USE_Get(9);
        Display_Data_F9[42] = time_get_temp & 0xff; // B5
        Display_Data_F9[43] = (time_get_temp >> 8) & 0xff;
        Display_Data_F9[44] = (time_get_temp >> 16) & 0xff;
        Display_Data_F9[45] = 0;
        Display_Data_F9[46] = 0;
        Display_Data_F9[47] = 0;
#endif
        Display_Data_F9[48] = 0x0D;
        Display_Data_F9[49] = 0x0A;
        for (u8 i = 0; i < 50; i++)
        {
            EUSART2_Write(Display_Data_F9[i]);
        }
        App_Update_Cnt = 0;
        break;

    case 5:
        u8 Display_Data[20] = {0};
        Display_Data[0] = 0xAA;
        Display_Data[1] = 0x55;
        Display_Data[2] = 0x02;
        Display_Data[3] = 0xe5;
        Display_Data[4] = 0xF5;
        Display_Data[5] = 0x01;
        Display_Data[6] = 0x00;
        Display_Data[7] = 0x09;
        Display_Data[8] = (u32)(ota_size_cnt * 100) / ota_size_num;
        Display_Data[9] = 0;
        Display_Data[10] = 0;
        Display_Data[11] = 0;
        Display_Data[12] = 0;
        Display_Data[13] = 0;
        Display_Data[14] = 0;
        Display_Data[15] = 0;
        Display_Data[16] = 0;
        Display_Data[17] = 0;
        Display_Data[18] = 0x0D;
        Display_Data[19] = 0x0A;
        for (u8 i = 0; i < 20; i++)
        {
            EUSART2_Write(Display_Data[i]);
        }
        App_Update_Cnt++;
        break;

    default:
        App_Update_Cnt = 0;
        break;
    }
}

void App_Ack(/*u16 TM_ACK_Value*/)
{
    u8 len = 0;
    u8 Display_Data[25];
    // todo 注册型号 版本号 260616
    if (TM_ACK_Value & TM_ACK_Model_App)
    {
        TM_ACK_Value &= ~TM_ACK_Model_App;
        Display_Data[0] = 0xAA;
        Display_Data[1] = 0x55;
        Display_Data[2] = TM_Device_App;
        Display_Data[3] = 0xe4;
        Display_Data[4] = 0x01;
        Display_Data[5] = 0x01;
        Display_Data[6] = 0x00;
#ifdef A391S_8
        len = 7;
        Display_Data[7] = len;
        Display_Data[8] = 0x00;
        Display_Data[9] = 'L';
        Display_Data[10] = '6';
        Display_Data[11] = 'S';
        Display_Data[12] = 'P';
        Display_Data[13] = 'R';
        Display_Data[14] = 'O';
        Display_Data[15] = 0;
#elif defined A396S_1
        len = 7;
        Display_Data[7] = len;
        Display_Data[8] = 0x00;
        Display_Data[9] = 'L';
        Display_Data[10] = '6';
        Display_Data[11] = 'S';
        Display_Data[12] = 'M';
        Display_Data[13] = 'A';
        Display_Data[14] = 'X';
        Display_Data[15] = 0;
#else
        len = 7;
        Display_Data[7] = len;
        Display_Data[8] = 0x00;
        Display_Data[9] = 'L';
        Display_Data[10] = '6';
        Display_Data[11] = 'S';
        Display_Data[12] = 'P';
        Display_Data[13] = 'R';
        Display_Data[14] = 'O';
        Display_Data[15] = 0;
#endif
        Display_Data[len + 9] = 0x0D;
        Display_Data[len + 10] = 0x0A;
        for (u8 i = 0; i < (len + 11); i++)
        {
            EUSART2_Write(Display_Data[i]);
        }
        // Send_App_Data(Display_Data, 11);
        App_Update_Cnt_Set(3); // @ run time 260927
        return;
    }
    if (TM_ACK_Value & TM_ACK_Model_Server)
    {
        TM_ACK_Value &= ~TM_ACK_Model_Server;
        Display_Data[0] = 0xAA;
        Display_Data[1] = 0x55;
        Display_Data[2] = TM_Device_Server;
        Display_Data[3] = 0xF1;
        Display_Data[4] = 0x01;
        Display_Data[5] = 0x01;
        Display_Data[6] = 0x00;
#ifdef A391S_8
        len = 7;
        Display_Data[7] = len;
        Display_Data[8] = 0x00;
        Display_Data[9] = 'L';
        Display_Data[10] = '6';
        Display_Data[11] = 'S';
        Display_Data[12] = 'P';
        Display_Data[13] = 'R';
        Display_Data[14] = 'O';
        Display_Data[15] = 0;
#elif defined A396S_1
        len = 7;
        Display_Data[7] = len;
        Display_Data[8] = 0x00;
        Display_Data[9] = 'L';
        Display_Data[10] = '6';
        Display_Data[11] = 'S';
        Display_Data[12] = 'M';
        Display_Data[13] = 'A';
        Display_Data[14] = 'X';
        Display_Data[15] = 0;
#else
        len = 7;
        Display_Data[7] = len;
        Display_Data[8] = 0x00;
        Display_Data[9] = 'L';
        Display_Data[10] = '6';
        Display_Data[11] = 'S';
        Display_Data[12] = 'P';
        Display_Data[13] = 'R';
        Display_Data[14] = 'O';
        Display_Data[15] = 0;
#endif
        Display_Data[len + 9] = 0x0D;
        Display_Data[len + 10] = 0x0A;
        for (u8 i = 0; i < (len + 11); i++)
        {
            EUSART2_Write(Display_Data[i]);
        }
        // Send_App_Data(Display_Data, 11);
        return;
    }
    // 按摩椅信息
    if (TM_ACK_Value & TM_ACK_Version_Server)
    {
        TM_ACK_Value &= ~TM_ACK_Version_Server;
        Display_Data[0] = 0xAA;
        Display_Data[1] = 0x55;
        Display_Data[2] = TM_Device_Server;
        Display_Data[3] = 0xf1;
        Display_Data[4] = 0x00;
        Display_Data[5] = 0x01;
        Display_Data[6] = 0x00;
        Display_Data[7] = 0x0D;
        Display_Data[8] = 0;
        Display_Data[9] = UserCode_data >> 24;
        Display_Data[10] = UserCode_data >> 16;
        Display_Data[11] = UserCode_data >> 8;
        Display_Data[12] = UserCode_data;
        // * 版本号修改 升级判断依据 250415
#if (FIRMWARE_VERSION == 0)
        Display_Data[13] = 1; // 1.0 固件版本
        Display_Data[14] = 0;
        Display_Data[15] = 1; // 1.0 硬件版本
        Display_Data[16] = 0;
#elif (FIRMWARE_VERSION == 1)
        Display_Data[13] = 1; // 1.0 固件版本
        Display_Data[14] = 1;
        Display_Data[15] = 1; // 1.0 硬件版本
        Display_Data[16] = 0;
#endif
        Display_Data[17] = 0; // ERR
        Display_Data[18] = 0;
        Display_Data[19] = 0;
        Display_Data[20] = 0;
        Display_Data[21] = 0;
        Display_Data[22] = 0x0D;
        Display_Data[23] = 0x0A;
        for (u8 i = 0; i < 24; i++)
        {
            EUSART2_Write(Display_Data[i]);
        }
        // Send_App_Data(Display_Data, 17);
        return;
    }
    if (TM_ACK_Value & TM_ACK_Valid_Sever)
    {
        TM_ACK_Value &= ~TM_ACK_Valid_Sever;
        Display_Data[0] = 0xAA;
        Display_Data[1] = 0x55;
        Display_Data[2] = TM_Device_Server;
        Display_Data[3] = 0Xe6; // 命令
        Display_Data[4] = 0X01; // 参数
        Display_Data[5] = 0X00; // 状态
        Display_Data[6] = 0X00; // 状态
        Display_Data[7] = 0X00; // 状态
        Display_Data[8] = Display_Data[2] + Display_Data[3] +
                          Display_Data[4] + Display_Data[5] + Display_Data[6] +
                          Display_Data[7]; //
        Display_Data[9] = 0X0D;            // 状态
        Display_Data[10] = 0X0A;           // 状态
        // if (Arm_Version > 0)//新版本扶手板支持200字节升级
        // {
        // Display_Data[4] = (u8)(OTA_PACKAGE_SIZE >> 8);
        // Display_Data[5] = (u8)OTA_PACKAGE_SIZE;
        // }
        // else
        // {
        // Display_Data[4] = 0;
        // Display_Data[5] = 60;
        // }
        // Display_Data[6] = (u8)(OTA_PACKAGE_DELAY >> 8);
        // Display_Data[7] = (u8)OTA_PACKAGE_DELAY;
        // Send_App_Data(Display_Data, 8);
        for (u8 i = 0; i < 11; i++)
        {
            EUSART2_Write(Display_Data[i]);
        }
        // Enter_Pause();
        // ota_over_time = 20; // 20/5= 4S
    }
    if (TM_ACK_Value & TM_ACK_DisValid_Sever)
    {
        // Display_Data[0] = TM_Device_Server;
        // Display_Data[1] = 0Xe6; // 命令
        // Display_Data[2] = 0X00; // 参数
        // Display_Data[3] = 0X00; // 状态
        // Display_Data[4] = 0x00; //
        // Send_App_Data(Display_Data, 5);
        TM_ACK_Value &= ~TM_ACK_Valid_Sever;
        Display_Data[0] = 0xAA;
        Display_Data[1] = 0x55;
        Display_Data[2] = TM_Device_Server;
        Display_Data[3] = 0Xe6; // 命令
        Display_Data[4] = 0X00; // 参数
        Display_Data[5] = 0X00; // 状态
        Display_Data[6] = 0X00; // 状态
        Display_Data[7] = 0X00; // 状态
        Display_Data[8] = Display_Data[2] + Display_Data[3] +
                          Display_Data[4] + Display_Data[5] + Display_Data[6] +
                          Display_Data[7];
        ;                        // 状态
        Display_Data[9] = 0X0D;  // 状态
        Display_Data[10] = 0X0A; // 状态
        for (u8 i = 0; i < 11; i++)
        {
            EUSART2_Write(Display_Data[i]);
        }
        // ota_state = ota_state_stop;
        // ota_package = 0;
        // ota_size = 0;
        // ota_addr = 0;
        // Exit_Pause();
    }
}

ErrStatus flash_check(u32 app_addr)
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

u8 flash_read_check(uint32_t read_addr)
{
    u8 i = 1;
    u16 j = 0;

    for (j = 0; j < SECTOR_SIZE; j += 64)
    {
        if (*(uint8_t *)(read_addr + j) == 0xFF)
        {
            i = 0;
        }
    }
    return i;
}

void App_Cmd_Handler(u8 *nbuf, u16 nLen)
{
    // u8 nDevice = nbuf[0];
    // u8 nCmd = nbuf[1];
    // u8 nParam = nbuf[2];
    u16 i;
    u8 nDevice = nbuf[2];
    u8 nCmd = nbuf[3];
    u8 nParam = nbuf[4];
    if (nDevice == TM_Device_Server) // 服务器
    {
        switch (nCmd)
        {
        case 0xf1: // 信息查询
            App_init();
            TM_ACK_Value |= TM_ACK_Version_Server | TM_ACK_Model_Server;
            break;
        case 0xf4: // 升级
            // Firmware_Update_FC = 255; // * 升级中 250417
            if (nParam == 0x01) // start
            {
                Write_Upgrade_Flag = 0;
                if (nPowerOn)
                {
                    UserFunc_Set(UserFunc_PowerOff, 1); // 关机
                }
                ota_size_num = ((u32)nbuf[8] << 24) +
                               ((u32)nbuf[9] << 16) +
                               ((u32)nbuf[10] << 8) +
                               ((u32)nbuf[11]);
                if (ota_size_num <= APP_SIZE && (app_ota_erase(TEMPLATE_START_ADDR, APP_SIZE / SECTOR_SIZE) == SUCCESS))
                {
                    ota_state = ota_state_start;
                    ota_addr = TEMPLATE_START_ADDR;
                    ota_package = 0;
                    ota_size_cnt = 0;
                    ota_over_time = ota_over_time_default;
                    Pt_Reset_ID(TASK_ID_APP);
                    App_Update_Cnt = 0;
                    App_Update_State();
                }
                else
                {
                    TM_ACK_Value = TM_ACK_DisValid_Sever;
                    App_Ack();
                }
            }
            else if (nParam == 0x02) // data
            {
                if (ota_state == ota_state_start)
                {
                    ota_package++;
                    if ((ota_package & 0x00ff) == nbuf[5] /* && ota_size_num >= nLen*/)
                    {
                        if (app_ota_write(ota_addr, &nbuf[8], nLen) == SUCCESS)
                        {
                            ota_addr += nLen;
                            // ota_size_num -= nLen;
                            for (i = 0; i < nLen; i++)
                            {
                                ota_size_cnt++;
                            }
                            ota_over_time = ota_over_time_default;
                            Pt_Reset_ID(TASK_ID_APP);
                            App_Update_Cnt_Set(5); // @ ota md 2-5 260927
                            App_Update_State();
                        }
                        else
                        {
                            TM_ACK_Value = TM_ACK_DisValid_Sever;
                            App_Ack();
                        }
                    }
                    else
                    {
                        TM_ACK_Value = TM_ACK_DisValid_Sever;
                        App_Ack();
                    }
                }
            }
            else if (nParam == 0x03) // end
            {
                if (ota_state == ota_state_start)
                {
                    ota_state = ota_state_end;
                    ota_over_time = ota_over_time_default;
                    TM_ACK_Value = TM_ACK_Valid_Sever;
                    App_Ack();
                    Write_Upgrade_Flag = 1; // * 修改 250418
                    TICK_RESET(Write_Upgrade_Tick);
                    // write_upgrade_flag(); //* 写更新标志 延时开启 转移 250418
                    // App_Update_Cnt = 0;   // * 直接跳转界面 无效 250417
                    // App_Update_State();
                }
                else
                {
                    ota_state = ota_state_stop;
                }
            }
            break;
        case 0xF5:
            if (nParam == 0x01) // start
            {
                Online_Err = 0;
                AutoLoad_size_num = ((u32)nbuf[6] << 8) + ((u32)nbuf[7]);
                AutoLoad_RealCnt = nbuf[5];
                AutoLoad_TempCnt = (nbuf[5] - 1) % 5; // * 修改 增加中医口舌诊断 250417

                if (AutoModeGet() == (21 + AutoLoad_TempCnt)) // * 正在读取FLASH 需要退出自动程序
                {
                    Online_Err = 0;
                    AT_TableStop(Table_Auto);
                    AutoModeSet(0);
                    AirModeSet(AirModeClose);
                    memset(&AirT_Work, 0, sizeof(AirT_Work_S));
                }

                if (AutoLoad_size_num <= AutoLoad_SIZE &&
                    (app_ota_erase(AutoLoad_START_ADDR + (AutoLoad_ADDR_Offset * AutoLoad_TempCnt), AutoLoad_SIZE / SECTOR_SIZE) == SUCCESS))
                {
                    AutoLoad_state = AutoLoad_state_start;
                    AutoLoad_addr = AutoLoad_START_ADDR + (AutoLoad_ADDR_Offset * AutoLoad_TempCnt);
                    AutoLoad_package = 0;
                    AutoLoad_size_cnt[AutoLoad_TempCnt] = 0;
                    AutoLoad_over_time = AutoLoad_over_time_default;
                    TM_ACK_Value = TM_ACK_Valid_Sever;
                    App_Ack();
                    Pt_Reset_ID(TASK_ID_APP);
                }
                else
                {
                    Online_Err = 1;
                    TM_ACK_Value = TM_ACK_DisValid_Sever;
                    App_Ack();
                }
            }
            else if (nParam == 0x02) // data
            {
                if (AutoLoad_state == AutoLoad_state_start)
                {
                    AutoLoad_package++;
                    if ((AutoLoad_package & 0x00ff) == nbuf[5] /* && ota_size_num >= nLen*/)
                    {
                        if (app_ota_write(AutoLoad_addr, &nbuf[8], nLen) == SUCCESS) // * 调试
                        {
                            AutoLoad_addr += nLen;
                            // ota_size_num -= nLen;
                            for (i = 0; i < nLen; i++)
                            {
                                AutoLoad_size_cnt[AutoLoad_TempCnt]++;
                            }
                            TM_ACK_Value = TM_ACK_Valid_Sever;
                            App_Ack();
                            AutoLoad_over_time = AutoLoad_over_time_default;
                            Pt_Reset_ID(TASK_ID_APP);
                        }
                        else
                        {
                            Online_Err = 1;
                            TM_ACK_Value = TM_ACK_DisValid_Sever;
                            App_Ack();
                        }
                    }
                    else
                    {
                        Online_Err = 1;
                        TM_ACK_Value = TM_ACK_DisValid_Sever;
                        App_Ack();
                    }
                }
            }
            else if (nParam == 0x03) // end
            {
                if (AutoLoad_state == AutoLoad_state_start)
                {
                    AutoLoad_state = AutoLoad_state_end;
                    AutoLoad_over_time = AutoLoad_over_time_default;

                    //  write_upgrade_flag(); // 写更新标志

                    if (flash_read_check(AutoLoad_START_ADDR + (AutoLoad_ADDR_Offset * AutoLoad_TempCnt)) == 0 || Online_Err)
                    {
                        Online_Err = 0;
                        TM_ACK_Value = TM_ACK_DisValid_Sever;
                        App_Ack();
                        break;
                    }

                    TM_ACK_Value = TM_ACK_Valid_Sever;
                    App_Ack();

                    // if (flash_read_AutoOnline(ONLINE_AUTO_ADDR2, AT_Auto22, AutoLoad_size_cnt[1])) //* 这里直接开启 在线自动程序 250414
                    if (AutoLoad_TempCnt < 5) // * 在线程序号 250414
                    {
                        Key.State = 2;
                        Key.Value = Key_AutoMode;
                        Key.Param = 21 + AutoLoad_TempCnt;
                        KeyFunc();
                    }
                    else
                    {
                        Buzzer = SOUND_ALARM;
                    }
                }
                else
                {
                    AutoLoad_state = AutoLoad_state_stop;
                }
            }
            break;
        }
    }
    else if (nDevice == TM_Device_App) // app
    {
        u16 i;
        Key.State = nbuf[5];
        Key.Param = nParam;
        Key.Value = 0;
        switch (nCmd)
        {
        case 0xe4: // 型号读取
            App_init();
            TM_ACK_Value |= TM_ACK_Model_App;
            break;
        case 0x30: // 开关机
            Key.Value = Key_Power;
            break;
        case 0x31: // 肩位置调节
            Key.Value = Key_Shoulder;
            Key.Param = nParam + 1; // * 增加 250406
            break;
        case 0x32: // 状态跳过
            if (Key.Param == 0x01)
            {
                Key.Value = Key_ShoulderAdj_Jump;
            }
            else if (Key.Param == 0x02)
            {
                Key.Value = Key_BodyCheck_Jump;
            }
            else if (Key.Param == 0x03)
            {
                Key.Value = key_AutoLyJump;
            }
            else if (Key.Param == 0x04)
            {
                Key.Value = Key_BodyCheck_Jump;
            }
            break;

        case 0x34: // 暂停
            Key.Value = Key_Pause;
            break;
        case 0x35: // 时间
            // Key.Value = Key_Timing;
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                i = WorkMinGet();
                i *= 60;
                i += WorkSecGet();
                switch (Key.Param)
                {
                // 时间减
                case 0:
                    if (i > 300)
                    {
                        i -= 300;
                    }
                    break;
                // 时间加
                case 1:
                    i += 300;
                    if (i > 2400)
                    {
                        i = 2400;
                    }
                    break;
                default:
                    break;
                }
                WorkMinSet(i / 60);
                WorkSecSet(i % 60);
                Buzzer = SOUND_ACK;
            }
            break;
        case 0x3e: // 背景音乐，语音播报
            if (Key.Param == 0x10)
            {
                Key.Value = Key_Player;
                Key.Param = 0;
            }
            else if (Key.Param == 0x11)
            {
                Key.Value = Key_Player;
                Key.Param = 1;
            }
            break;
        case 0x3f: // 音量
            Key.Value = Key_Volume;
            if (Key.Param > 15)
            {
                Key.Param = 15;
            }
            break;
        case 0x40: // 自动模式
            if (nParam == 0xe1)
            {
                // Key.Value = Key_Memory_Use;
                if (flash_read_AutoOnline(ONLINE_AUTO_ADDR1, AT_Auto21, AutoLoad_size_cnt[0]))
                {
                    Key.Value = Key_AutoMode;
                    Key.Param = 21;
                }
                else
                {
                    Buzzer = SOUND_ALARM;
                }
                // Key.Param = 1;
            }
            else if (nParam == 0xe2)
            {
                // Key.Value = Key_Memory_Use;
                // Key.Param = 2;
                if (flash_read_AutoOnline(ONLINE_AUTO_ADDR2, AT_Auto22, AutoLoad_size_cnt[1]))
                {
                    Key.Value = Key_AutoMode;
                    Key.Param = 22;
                }
                else
                {
                    Buzzer = SOUND_ALARM;
                }
            }
            else if (nParam == 0xe3)
            {
                if (flash_read_AutoOnline(ONLINE_AUTO_ADDR3, AT_Auto23, AutoLoad_size_cnt[2]))
                {
                    Key.Value = Key_AutoMode;
                    Key.Param = 23;
                }
                else
                {
                    Buzzer = SOUND_ALARM;
                }
            }
            else if (nParam == 0xe4)
            {
                if (flash_read_AutoOnline(ONLINE_AUTO_ADDR4, AT_Auto24, AutoLoad_size_cnt[3]))
                {
                    Key.Value = Key_AutoMode;
                    Key.Param = 24;
                }
                else
                {
                    Buzzer = SOUND_ALARM;
                }
            }
            else if (nParam == 0xe5) // * 增加 250413
            {
                if (flash_read_AutoOnline(ONLINE_AUTO_ADDR5, AT_Auto25, AutoLoad_size_cnt[4]))
                {
                    Key.Value = Key_AutoMode;
                    Key.Param = 25;
                }
                else
                {
                    Buzzer = SOUND_ALARM;
                }
            }
            else
            {
                Key.Value = Key_AutoMode;
            }
            break;
#ifdef USE_M2      // todo add 260924
        case 0xc0: // @ 定点
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc) || (AutoModeGet() != 0))
            {
                MoveWhichSet(MoveDw);
                if (AutoModeGet() > 0) // * 增加 250419
                {
                    MassWiseSet(MoveUp, TZ);
                    MassWiseSet(MoveDw, TZ);
                    UserFunc_Set(UserFunc_ManualMode, 0);
                }
                MassAreaSet(MoveDw, Area_Fixed);
                Buzzer = SOUND_ACK;
            }
            break;
        case 0xc1: // @ 手法
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                MoveWhichSet(MoveDw);
                if (AutoModeGet() > 0 || (WorkStateGet() == WorkState_WaitFunc)) // * 修改 250409
                {
                    MassWiseSet(MoveUp, TZ);
                    MassWiseSet(MoveDw, TZ);
                    UserFunc_Set(UserFunc_ManualMode, 0);
                    i = 0;
                }
                else
                {
                    i = MassWiseGet(MoveDw);
                }
                if (/*Manual_Select==0 &&*/ Key.Param == 6 /*i <= JRZFB && i >= JRZ*/)
                {
                    // MoveUpTreadStateSet(TreadState_Stop, 0, 0);
                    Part_Date_Set(MoveUp_Flex, 1, FLEX_FIX_CIRCLE, 0, 0, 2);
                    KeyPush.TreadKeyState = TreadState_Stop;
                }
                switch (Key.Param)
                {
                default: // 停止
                case 0:
                    i = TZ;
                    break;
                case 1: // 揉捏
                    i = RNZ;
                    break;
                case 2: // 拍打2
                    i = PD;
                    break;
                case 4: // 揉捏拍打
                    i = RPZ;
                    break;
                case 8: // 敲击
                    i = PDB;
                    break;
                }

                if (0 == MassSpeedGet(MoveDw))
                {
                    MassSpeedSet(MoveDw, 3);
                }
                MassWiseSet(MoveDw, (ManualWise_E)i);

                KeyPush.WiseKeyState = 1;
                Buzzer = SOUND_ACK;
            }
            break;
        case 0xc2: // @ 速度
            if (WorkStateGet() == WorkState_Work && AutoModeGet() == 0)
            {
                MoveWhichSet(MoveDw);
                if (MassWiseGet(MoveDw) == TZ)
                {
                    Buzzer = SOUND_ALARM;
                    return;
                }
                i = MassSpeedGet(MoveDw);
                if (Key.Param == 0x01) // 循环模式
                {
                    if (i < 5)
                    {
                        MassSpeedSet(MoveDw, i + 1);
                    }
                    else
                    {
                        MassSpeedSet(MoveDw, 1);
                    }
                    Buzzer = SOUND_ACK; //* 已增加
                }
            }
            break;
        case 0xc4: // @ 移动
            if (MassAreaGet(MoveUp) == Area_Full)
            {
                Buzzer = SOUND_ALARM;
                break;
            }
            MoveWhichSet(MoveDw);
            Key.Value = Key_BackTread;
            if (nParam == 1) // 上
            {
                Key.Param = 1;
            }
            if (nParam == 2) // 下
            {
                Key.Param = 2;
            }
            break;
        case 0xc5: // 宽度
            if (((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc)) && AutoModeGet() == 0)
            {
                MoveWhichSet(MoveDw);
                if ((RNZ == MassWiseGet(MoveDw)) ||
                    (RPZ == MassWiseGet(MoveDw)) ||
                    (JRZ == MassWiseGet(MoveDw)))
                {
                    Buzzer = SOUND_ALARM;
                    break;
                }
                i = MassWidthGet(MoveDw);
                if (Key.Param == 0x10) //-
                {
                    if (i > 0)
                    {
                        MassWidthSet(MoveDw, i - 1);
                        Buzzer = SOUND_ACK;
                    }
                    else
                    {
                        Buzzer = SOUND_ALARM;
                    }
                }
                else if (Key.Param == 0x20) //+
                {
                    if (i < 4)
                    {
                        MassWidthSet(MoveDw, i + 1);
                        Buzzer = SOUND_ACK;
                    }
                    else
                    {
                        Buzzer = SOUND_ALARM;
                    }
                }
            }
            break;
#endif
        case 0x41: // 按摩区域
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc) || (AutoModeGet() != 0))
            {
                MoveWhichSet(MoveUp);
                if (AutoModeGet() > 0) // * 增加 250419
                {
                    MassWiseSet(MoveUp, TZ);
#ifdef USE_M2
                    MassWiseSet(MoveDw, TZ);
#endif
                    UserFunc_Set(UserFunc_ManualMode, 0);
                }

                if (MassWiseGet(MoveUp) == JRZ)
                {
                    Buzzer = SOUND_ALARM;
                    return;
                }

                switch (Key.Param)
                {
                default:
                case 1: // 定点
                    MoveUpState.ManualArea_ID = 4;
                    break;
                case 2: // 局部
                    MoveUpState.ManualArea_ID = 5;
                    break;
                case 3: // 全程
                    MoveUpState.ManualArea_ID = 3;
                    break;
                case 4: // 上半身
                    MoveUpState.ManualArea_ID = 1;
                    break;
                case 5: // 下半身
                    MoveUpState.ManualArea_ID = 2;
                    break;
                }
                MassAreaSet(MoveUp, (MassageArea_E)MoveUpState.ManualArea_ID);
                Buzzer = SOUND_ACK;
            }
            break;
        case 0x42: // 按摩手法？
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                MoveWhichSet(MoveUp);
                if (AutoModeGet() > 0 || (WorkStateGet() == WorkState_WaitFunc)) // * 修改 250409
                {
                    MassWiseSet(MoveUp, TZ);
#ifdef USE_M2
                    MassWiseSet(MoveDw, TZ);
#endif
                    UserFunc_Set(UserFunc_ManualMode, 0);
                    i = 0;
                }
                else
                {
                    i = MassWiseGet(MoveUp);
                }
                if (/*Manual_Select==0 &&*/ Key.Param == 6 /*i <= JRZFB && i >= JRZ*/)
                {
                    // MoveUpTreadStateSet(TreadState_Stop, 0, 0);
                    Part_Date_Set(MoveUp_Flex, 1, FLEX_FIX_CIRCLE, 0, 0, 2);
                    KeyPush.TreadKeyState = TreadState_Stop;
                }
                switch (Key.Param)
                {
                default: // 停止
                    i = TZ;
                    break;
                case 0: // 手法1 停止
                    i = TZ;
                    break;
                case 1: // 揉捏
                    i = RNZ;
                    break;
                case 2: // 拍打2
                    i = PD;
                    break;
                case 4: // 揉捏拍打
                    i = RPZ;
                    break;
                case 8: // 敲击
                    i = PDB;
                    break;
                case 5: // 指压
                    i = ZYS;
                    break;
                case 0x20: // 3D揉捏
                    i = D3RNZ;
                    break;
                case 0x21: // 3D拍打
                    i = D3PD;
                    // i = RNZB;
                    break;
                case 0x22: // 3D揉捏拍打
                    i = D3RPZ;
                    // i = RNFB;
                    break;
                case 0x24: // 3D指压
                    i = ZYD;
                    // i = TJA;
                    break;
                }

                if (0 == MassSpeedGet(MoveUp))
                {
                    MassSpeedSet(MoveUp, 3);
                }
                MassWiseSet(MoveUp, (ManualWise_E)i);
                MassIntensitySet(nBackIntensity << 1); // * 增加让 ZYD ZYS 工作 250419

                KeyPush.WiseKeyState = 1;
                Buzzer = SOUND_ACK;
            }
            break;
        case 0x43: // 按摩速度？
            if (WorkStateGet() == WorkState_Work && AutoModeGet() == 0)
            {
                MoveWhichSet(MoveUp);
                if (MassWiseGet(MoveUp) == ZYS || MassWiseGet(MoveUp) == TZ || MassWiseGet(MoveUp) == ZY)
                {
                    Buzzer = SOUND_ALARM;
                    return;
                }
                i = MassSpeedGet(MoveUp);
                if (Key.Param == 0x01) // 循环模式
                {
                    if (i < 5)
                    {
                        MassSpeedSet(MoveUp, i + 1);
                        Buzzer = SOUND_ACK; //* 已增加
                    }
                    else
                    {
                        MassSpeedSet(MoveUp, 1);
                        Buzzer = SOUND_ACK; //* 已增加
                    }
                }
                else if (Key.Param == 0x10) // 循环模式
                {
                    if (i > 1)
                    {
                        MassSpeedSet(MoveUp, i - 1);
                        Buzzer = SOUND_ACK; //* 已增加
                    }

                    else
                        Buzzer = SOUND_ALARM;
                }
                else if (Key.Param == 0x20) // 循环模式
                {
                    if (i < 5)
                    {
                        MassSpeedSet(MoveUp, i + 1);
                        Buzzer = SOUND_ACK; //* 已增加
                    }
                    else
                        Buzzer = SOUND_ALARM;
                }
            }
            break;
        case 0x44: // 按摩宽度？
            if (((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc)) && AutoModeGet() == 0)
            {
                MoveWhichSet(MoveUp);

                if ((RNZ == MassWiseGet(MoveUp)) ||
                    (RPZ == MassWiseGet(MoveUp)) ||
                    (JRZ == MassWiseGet(MoveUp)))
                {
                    Buzzer = SOUND_ALARM;
                    break;
                }
                i = MassWidthGet(MoveUp);
                if (Key.Param == 0x10) // 减
                {
                    if (i > 0) //* 已修改
                    {
                        MassWidthSet(MoveUp, i - 1);
                        Buzzer = SOUND_ACK;
                    }
                    else
                    {
                        Buzzer = SOUND_ALARM;
                    }
                }
                else if (Key.Param == 0x20) // 加
                {
                    if (i < 4) // * 修改 5-4 250213
                    {
                        MassWidthSet(MoveUp, i + 1);
                        Buzzer = SOUND_ACK;
                    }
                    else
                    {
                        Buzzer = SOUND_ALARM;
                    }
                }
            }
            break;
        case 0x45: //@ 背部强度
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                MoveWhichSet(MoveUp);

                if (Key.Param == 0x20)
                {
                    if (nBackIntensity < 4)
                    {
                        nBackIntensity++;
                        Buzzer = SOUND_ACK;
                    }
                    else
                    {
                        Buzzer = SOUND_ALARM;
                    }
                }
                else if (Key.Param == 0x10)
                {
                    if (nBackIntensity > 0)
                    {
                        nBackIntensity--;
                        Buzzer = SOUND_ACK;
                    }
                    else
                    {
                        Buzzer = SOUND_ALARM;
                    }
                }
                else
                {
                    if (nBackIntensity > 3) // * 修改 5->3 250406
                    {
                        nBackIntensity = 0;
                    }
                    else
                    {
                        nBackIntensity++;
                    }
                    Buzzer = SOUND_ACK;
                }

                if (AutoModeGet() == 0) // @ 手动模式直接设置3D强度、自动模式还要参考结合动作表
                {
                    MassIntensitySet(nBackIntensity << 1);
                }
            }
            break;
        case 0x46: // 气压强度，只有一个强度参数

            Key.Value = Key_UpperAirStrength;
            Key.Param = 3; // Key_UpperAirStrength解析处理，参数为3的时候气压强度循环累加

            break;
        case 0x47: // 当作上半身模式
            if (Key.Param == 0x20)
            {
                Key.Param = 1;
                Key.Value = Key_UpperAirStrength;
                Buzzer = SOUND_ALARM;
            }
            else if (Key.Param == 0x10)
            {
                Key.Param = 2;
                Key.Value = Key_UpperAirStrength;
                Buzzer = SOUND_ACK; // * 增加 250419
            }
            else if (Key.Param == 0x30)
            {
                Buzzer = SOUND_ACK; // * 增加 250419
                switch (AirModeGet())
                {
                default:
                case AirModeClose: //
                    AirModeSet(AirModeUpper);
                    break;
                case AirModeUpper: //
                    AirModeSet(AirModeClose);
                    memset(&AirT_Work, 0, sizeof(AirT_Work_S));
                    break;
                case AirModeLower: //
                    AirModeSet(AirModeFull);
                    break;
                case AirModeFull: //
                    AirModeSet(AirModeLower);
                    break;
                case AirModeAuto: // * 增加 250419
                    AirModeSet(AirModeLower);
                    break;
                }
            }
            break;
        case 0x48: // 当作下半身模式
            if (Key.Param == 0x30)
            {
                Buzzer = SOUND_ACK; // * 增加 250419
                switch (AirModeGet())
                {
                default:
                case AirModeClose: //
                    AirModeSet(AirModeLower);
                    break;
                case AirModeUpper: //
                    AirModeSet(AirModeFull);
                    break;
                case AirModeLower: //
                    AirModeSet(AirModeClose);
                    memset(&AirT_Work, 0, sizeof(AirT_Work_S));
                    break;
                case AirModeFull: //
                    AirModeSet(AirModeUpper);
                    break;
                case AirModeAuto: // * 增加 250419
                    AirModeSet(AirModeUpper);
                    break;
                }
            }
            break;
        case 0x4c:
            Key.Value = Key_BackHeat;
            // Key.Param = !nUpperHeat;
            break;
        case 0x4e:
            Key.Value = Key_LegsHeat;
            // Key.Param = !nLowerHeat;
            break;
        case 0x4f: // ROLL
            if ((WorkStateGet() == WorkState_Work) ||
                (WorkStateGet() == WorkState_WaitFunc))
            {
                if (Key.Param == 0x20)
                {
                    i = RollSpeedGet();
                    if (i < 3)
                    {
                        RollSpeedSet(++i);
                    }
                    else
                    {
                        RollSpeedSet(3);
                    }
                    RollModeSet(1);
                    RollStateSet(1);
                }
                else if (Key.Param == 0x10)
                {
                    i = RollSpeedGet();
                    if (i > 0)
                    {
                        RollSpeedSet(--i);
                    }
                    else
                    {
                        RollSpeedSet(0);
                    }

                    if (RollSpeedGet())
                    {
                        RollModeSet(1);
                        RollStateSet(1);
                    }
                    else
                    {
                        RollModeSet(0);
                        RollStateSet(0);
                    }
                }
                else
                {
                    i = RollSpeedGet();
                    if (i < 3)
                    {
                        RollSpeedSet(++i);
                    }
                    else
                    {
                        RollSpeedSet(0);
                    }
                    if (RollSpeedGet())
                    {
                        RollModeSet(1);
                        RollStateSet(1);
                    }
                    else
                    {
                        RollModeSet(0);
                        RollStateSet(0);
                    }
                }
                KeyPush.FootRollKeyState = 1;
                Buzzer = SOUND_ACK; //* 增加
            }
            break;
        case 0x4a:
            Key.Value = Key_LegsKnead;
            break;
        case 0x50: // 停止
            Key.Value = Key_Lying;
            Key.Param = 0;
            break;
        case 0x51:
        case 0x54:
            Key.Value = Key_LegsFlex;
            if (nParam == 1) // 上升
            {
                Key.Param = 2;
            }
            if (nParam == 0) // 下降
            {
                Key.Param = 1;
            }
            break;
        case 0x52:
            Key.Value = Key_BackLying;
            if (nParam == 0) // 上升
            {
                Key.Param = 0;
            }
            if (nParam == 1) // 下降
            {
                Key.Param = 1;
            }
            break;
        case 0x53:
            Key.Value = Key_LegsUpDown;
            if (nParam == 0) // 上升
            {
                Key.Param = 1; // * 修改 250406
            }
            if (nParam == 1) // 下降
            {
                Key.Param = 2; // * 修改 250406
            }
            break;
        case 0x55:
            Key.Value = Key_Lying;
            if (nParam == 0) // 上升
            {
                Key.Param = 1; // * 修改 250406
            }
            if (nParam == 1) // 下降
            {
                Key.Param = 2; // * 修改 250406
            }
            break;
        case 0x56:
            Key.Value = Key_Zero;
            break;
        case 0x57:
            if (MassAreaGet(MoveUp) == Area_Full || MassAreaGet(MoveUp) == Area_Upper || MassAreaGet(MoveUp) == Area_Lower)
            {
                Buzzer = SOUND_ALARM;
                break;
            }
            MoveWhichSet(MoveUp);
            Key.Value = Key_BackTread;
            if (nParam == 0) // 上
            {
                Key.Param = 1;
            }
            if (nParam == 1) // 下
            {
                Key.Param = 2;
            }
            break;
        case 0x58:
            Key.Value = Key_Voice;
            Key.Param = !nVoiceControlSw;
            break;
        case 0x59: // 声控唤醒？
            if (nVoiceControlSw)
            {
                Voice_WakeUp();
                Voice_Ack(VC_WakeUp);
            }
            else
            {
                Buzzer = SOUND_ALARM;
            }
            break;
        case 0x5A: // 负离子
            Key.Value = Key_Ion;
            Key.Param = !nIonState;
            break;
        case 0x5b:
            if (!nBlueState) // * 只开启
                Key.Value = Key_Blue;
            else
                Buzzer = SOUND_ALARM;
            break;
        case 0x5e:
            Key.Value = Key_Health;
            break;
        case 0xe0:
            Key.Param >>= 4;
            if (Key.Param >= 1 && Key.Param <= 3)
            {
                Key.Value = Key_Memory_Save;
            }
            break;
        default:
            break;
        }
        if (Key.Value > 0)
        {
            // * 增加按键音 250407
            if (Key.Value == Key_Lying || Key.Value == Key_BackLying || Key.Value == Key_LegsUpDown || Key.Value == Key_BackUpDown ||
                Key.Value == Key_BackTread || Key.Value == Key_LegsFlex)
            {
                if (Key.State == 1)
                {
                    Buzzer = SOUND_SHOULDER;
                    TICK_RESET(Buzzer_Shoulder_Tick); // * 增加 250509
                }
                else if (Key.State == 2)
                {
                    Buzzer = SOUND_NULL;
                }
            }
            KeyFunc();
        }
    }
}

void os_task_app(OS_TASK_ID nTask_ID) // app任务
{
    // APP循环接收数据
    Pt_Start if (ota_over_time > 0)
    {
        Pt_Wait_Tick(10); // 50mS
        TM_ACK_Value = TM_ACK_Valid_Sever;
        App_Ack();
    }
    while (1)
    {
        Pt_Wait_Tick(50); // 250ms/5ms = 50
        if (ota_over_time > 0)
        {
            ota_over_time--;
            if (ota_over_time == 0)
            {
                if (ota_state == ota_state_end) // 发送完应答后如果更新完成直接重启
                {
                    NVIC_SystemReset(); // 系统复位
                    // UserFunc_Set(UserFunc_PowerOn, 0);
                }
                else
                {
                    TM_ACK_Value = TM_ACK_DisValid_Sever;
                    App_Ack();
                }
                ota_state = ota_state_stop;
            }
        }
        else
        {
            if (TM_ACK_Value)
            {
                App_Ack();
            }
            else
            {
                App_Update_State();
            }
        }
    }
    Pt_End
}

#define AppComHead1 0xAA
#define AppComHead2 0x55
#define AppComTial1 0x0D
#define AppComTial2 0x0A

// static RxPro_S RxVar;
static RxPro_S APP_RxVar;

void AppCom(u8 dat)
{
    if (Write_Upgrade_Flag) // * 增加补丁 250418
    {
        if (TICK_COUNTER(Write_Upgrade_Tick, 1000))
        {
            Write_Upgrade_Flag = 0;
            write_upgrade_flag(); // * 写更新标志 延时开启 转移 250418
        }
    }
    APP_RxVar.RxBuffer[APP_RxVar.RxCount] = dat;
    switch (APP_RxVar.RxState)
    {
    case 0: // 解析帧头1
        if (APP_RxVar.RxBuffer[APP_RxVar.RxCount] == AppComHead1)
        {
            APP_RxVar.RxCount++;
            APP_RxVar.RxState++;
        }
        else
        {
            APP_RxVar.RxCount = 0;
            APP_RxVar.RxState = 0;
        }
        break;
    case 1: // 解析帧头2
        if (APP_RxVar.RxBuffer[APP_RxVar.RxCount] == AppComHead2)
        {
            APP_RxVar.RxCount++;
            APP_RxVar.RxState++;
        }
        else
        {
            APP_RxVar.RxCount = 0;
            APP_RxVar.RxState = 0;
        }
        break;
    case 2: // 解析协议类型
        if (APP_RxVar.RxBuffer[APP_RxVar.RxCount] == 0x02)
        {
            APP_RxVar.RxDataNum = 6;
            APP_RxVar.RxCount++;
            APP_RxVar.RxState++;
        }
        // 服务器
        else if (APP_RxVar.RxBuffer[APP_RxVar.RxCount] == 0x03)
        {
            APP_RxVar.RxCount++;
            APP_RxVar.RxState = 5;
        }
        else
        {
            APP_RxVar.RxCount = 0;
            APP_RxVar.RxState = 0;
        }
        break;
    case 3: // 解析数据
        if (APP_RxVar.RxDataNum > 0)
        {
            APP_RxVar.RxDataNum--;
            APP_RxVar.RxCount++;
        }
        else
        {
            if (APP_RxVar.RxBuffer[APP_RxVar.RxCount] == AppComTial1)
            {
                APP_RxVar.RxCount++;
                APP_RxVar.RxState++;
            }
            else
            {
                APP_RxVar.RxCount = 0;
                APP_RxVar.RxState = 0;
            }
        }
        break;
    case 4: // 判断帧尾
        if (APP_RxVar.RxBuffer[APP_RxVar.RxCount] == AppComTial2)
        {
            App_Cmd_Handler(APP_RxVar.RxBuffer, APP_RxVar.RxDataLen);
        }
        APP_RxVar.RxState = 0;
        APP_RxVar.RxCount = 0;
        break;
    case 5: // 命令
        APP_RxVar.RxDataCmd = APP_RxVar.RxBuffer[APP_RxVar.RxCount];
        if (APP_RxVar.RxDataCmd == 0xF1)
        {
            APP_RxVar.RxDataNum = 5;
            APP_RxVar.RxCount++;
            APP_RxVar.RxState = 3;
        }
        else if (APP_RxVar.RxDataCmd == 0xF4 || APP_RxVar.RxDataCmd == 0xF5)
        {
            APP_RxVar.RxState++;
            APP_RxVar.RxCount++;
        }
        break;
    /*****在线升级数据解析********/
    case 6:
        APP_RxVar.RxDataParm = APP_RxVar.RxBuffer[APP_RxVar.RxCount];
        if (APP_RxVar.RxDataParm == 0x01)
        {
            APP_RxVar.RxDataNum = 8;
            APP_RxVar.RxCount++;
            APP_RxVar.RxState = 3;
        }
        else if (APP_RxVar.RxDataParm == 0x02)
        {
            APP_RxVar.RxState++;
            APP_RxVar.RxCount++;
        }
        else if (APP_RxVar.RxDataParm == 0x03)
        {
            APP_RxVar.RxDataNum = 4;
            APP_RxVar.RxCount++;
            APP_RxVar.RxState = 3;
        }
        break;
    case 7:
        APP_RxVar.RxDataNoPage = APP_RxVar.RxBuffer[APP_RxVar.RxCount];
        APP_RxVar.RxState++;
        APP_RxVar.RxCount++;
        break;
    case 8:
        APP_RxVar.RxDataLenH8 = APP_RxVar.RxBuffer[APP_RxVar.RxCount];
        APP_RxVar.RxState++;
        APP_RxVar.RxCount++;
        break;
    case 9:
        APP_RxVar.RxDataLenL8 = APP_RxVar.RxBuffer[APP_RxVar.RxCount];
        APP_RxVar.RxDataLen = (APP_RxVar.RxDataLenH8 << 8 | APP_RxVar.RxDataLenL8);
        APP_RxVar.RxDataNum = APP_RxVar.RxDataLen + 1;
        APP_RxVar.RxState = 3;
        APP_RxVar.RxCount++;
        break;
    }
}

void App_init(void)
{
    TM_ACK_Value = TM_ACK_NULL; // 初始化查询状态
    ota_state = 0;
    ota_over_time = 0;
    Pt_Reset_ID(TASK_ID_APP);
    // Pt_Pause_ID(TASK_ID_APP);
}
