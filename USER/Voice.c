#include "Voice.h"
#include "MassageInit.h"

#define VoiceControl_Head 0XAA
#define VoiceControl_Tial 0X55

#define VoiceControl_Receive_Data 7

#define VoiceControl_Real_Read COM3.RX_read
#define VoiceControl_Real_Write COM3.RX_write
#define VoiceControl_GetValue() EUART3_Read()
#define VoiceControl_SendValue(x) EUART3_Write(x)

VoiceKey_t VoiceKey;
uint8_t VoiceControl_HandBuffer[VoiceControl_Receive_Data], VoiceControl_HandCnt = 0, VoiceControl_RX_Status = 0;

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

void Voice_Swich_Set(u8 x)
{
    nVoiceControlSw = x;

    if (nVoiceControlSw)
        Voice_Ack(VC_VcOn);
    else
        Voice_Ack(VC_VcOff);

    //    Flash_Check_Tick = Flash_Check_Time;
}

void Voice_WakeUp(void)
{
    VoiceControl_SendValue(0xaa);
    VoiceControl_SendValue(0x55);
    VoiceControl_SendValue(0xe0);
    VoiceControl_SendValue(0x0e);
    VoiceControl_SendValue(0xee);
    VoiceControl_SendValue(0x0d);
    VoiceControl_SendValue(0x0a);
}

void Voice_Cof(u8 x) // 声控使能设置
{
    VoiceControl_SendValue(0xaa);
    VoiceControl_SendValue(0x55);

    if (x == 1)
    {
        VoiceControl_SendValue(0xa1);
        VoiceControl_SendValue(0x1a);
        VoiceControl_SendValue(0xbb);
    }
    else
    {
        VoiceControl_SendValue(0xa2);
        VoiceControl_SendValue(0x2a);
        VoiceControl_SendValue(0xcc);
    }

    VoiceControl_SendValue(0x0d);
    VoiceControl_SendValue(0x0a);
}

u8 Voice_Old_Date = 0;  // * 增加 250426
u32 Voice_tick = 0;     // * 新增
u32 Go_little_tick = 0; // * add for go little down/up
bool Go_little_Flag = 0;
void Voice_Cmd_Handler(uint8_t value)
{
    u8 i, temp;

    if (insertSta)
    {
        return;
    }

    if (TICK_COUNTER(Voice_tick, 4000)) // @ 8S -> 4S 260114
    {
        Voice_Old_Date = 0;
    }

    if (Voice_Old_Date == value)
    {
        return;
    }

    // if (WorkStateGet() == WorkState_Pause) // * 语音识别开关 250306
    if (WorkStateGet() == WorkState_Pause || WorkStateGet() == WorkState_TurnOn) // @ md 260115
    {
        // Voice_Ack_None();
        Voice_Ack(VC_Invalid);
        return;
    }
    if (nLCState) //* 老化退出
        return;
    if (WorkStateGet() == WorkState_FCT)
        return;
    switch (value)
    {
    case 0x30: // 按摩关闭 语音识别有问题
        if ((WorkStateGet() != WorkState_Wait) /* && (WorkStateGet() != WorkState_Reset)*/)
        {
            UserFunc_Set(UserFunc_PowerOff, 0);
#ifdef VIETNAM_VOICE
            Voice_Ack(VC_OK);
#else
            Voice_Ack(VC_MassageOff);
#endif
            Voice_Old_Date = value;
            TICK_RESET(Voice_tick);
        }
        else
        {
            Buzzer = SOUND_ALARM;
        }
        break;

    case 0x31: // 按摩开启
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            UserFunc_Set(UserFunc_PowerOn, 0);
#ifdef VIETNAM_VOICE
            Voice_Ack(VC_OK);
#else
            Voice_Ack(VC_MassageOn);
#endif
            Voice_Old_Date = value;
            TICK_RESET(Voice_tick);
        }
        else
        {
            Buzzer = SOUND_ALARM;
        }
        break;

#ifdef ENGLISH_VOICE
    case 0x32: // total relax
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 1)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(1);
                Voice_Ack(VC_Auto1);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 1)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(1);
                Voice_Ack(VC_Auto1);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x33: // energy boost
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 16)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(16);
                Voice_Ack(VC_Auto2);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 16)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(16);
                Voice_Ack(VC_Auto2);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x34: // Sweet
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 20)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(20);
                Voice_Ack(VC_Auto3);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 20)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(20);
                Voice_Ack(VC_Auto3);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }

        break;

    case 0x35: // 4 Neck Shoulder
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 7)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(7);
                Voice_Ack(VC_Auto4);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 7)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(7);
                Voice_Ack(VC_Auto4);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }

        break;

    case 0x40: //* Lunch
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 17)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(17);
                Voice_Ack(VC_Auto5);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 17)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(17);
                Voice_Ack(VC_Auto5);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x37: //* Thai
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 5)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(5);
                Voice_Ack(VC_Auto6);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 5)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(5);
                Voice_Ack(VC_Auto6);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x53: //* open air
        if (WorkStateGet() == WorkState_Work)
        {
            if (AirModeGet() > 0)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                Voice_Ack(VC_AirOn);
                AirModeSet(AirModeFull);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else
        {
            Buzzer = SOUND_ALARM;
        }

        break;

    case 0x54: //* close air
        if (WorkStateGet() == WorkState_Work)
        {
            if (AirModeClose == AirModeGet())
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                Voice_Ack(VC_AirOff);
                AirModeSet(AirModeClose);
                memset(&AirT_Work, 0, sizeof(AirT_Work_S)); // * 初始化气压参数 251209
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else
        {
            Buzzer = SOUND_ALARM;
        }
        break;

    case 0x55: // up the seat
        Voice_Old_Date = value;
        TICK_RESET(Voice_tick);
        Voice_Ack(VC_SitUp);
        DDG_Set_SitUpCircle(DDG_BCircle_UpDown, DDG_LCircle_UpDown);
        break;

    case 0x56: // down the seat
        Voice_Old_Date = value;
        TICK_RESET(Voice_tick);
        Voice_Ack(VC_SitDown);
        DDG_Set_SitDownCircle(DDG_BCircle_UpDown, DDG_LCircle_UpDown);
        break;

    case 0x50: // change the other mode
        // if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
        if (WorkStateGet() == WorkState_Work) // * modify 250813
        {
            Voice_Old_Date = value;
            TICK_RESET(Voice_tick);
            Voice_Ack(VC_Change);

            if (AutoModeGet() > 0)
            {
                MassWiseSet(MoveDw, TZ);
                UserFunc_Set(UserFunc_ManualMode, 0);
                temp = ZYD;
            }
            else
            {
                temp = MassWiseGet(MoveUp);
            }

            switch (temp)
            {
            case TZ: // 揉捏
                i = RNZ;
                break;
            case RNZ: // 拍打2
                i = PD;
                break;
            case PD: // 揉捏拍打
                i = RPZ;
                break;
            case RPZ: // 敲击
                i = PDB;
                break;
            case PDB: // 指压
                i = ZYS;
                break;
            case ZYS: // 3D1
                i = D3RNZ;
                break;
            case D3RNZ: // 3D2
                i = D3PD;
                break;
            case D3PD: // 3D3
                i = D3RPZ;
                break;
            case D3RPZ: // 3D指压
                i = ZYD;
                break;
            case ZYD: // 3D指压
                i = TZ;
                break;
            default: // 停止
                i = TZ;
                break;
            }

            if (0 == MassSpeedGet(MoveUp))
                MassSpeedSet(MoveUp, 3);

            MassWiseSet(MoveUp, (ManualWise_E)i);
            MassIntensitySet(nBackIntensity << 1); // * 增加让 ZYD ZYS 工作 250319
            KeyPush.WiseKeyState = 1;
            // Buzzer = SOUND_ACK;
        }
        else
        {
            Buzzer = SOUND_ALARM;
            Voice_Ack(VC_Invalid);
        }
        break;

    case 0x57: // Fixed massage
        // if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc) || (AutoModeGet() != 0))
        if (WorkStateGet() == WorkState_Work) // * modify 250813
        {
            Voice_Old_Date = value;
            TICK_RESET(Voice_tick);
            Voice_Ack(VC_Fixed);
            if (AutoModeGet() > 0) // * 增加 250322
            {
                MassWiseSet(MoveUp, TZ);
                MassWiseSet(MoveDw, TZ);
                UserFunc_Set(UserFunc_ManualMode, 0);
            }

            if (MassWiseGet(MoveUp) == JRZ)
            {
                Buzzer = SOUND_ALARM;
                return;
            }
#ifdef USE_M2
            if (MoveWhichGet())
            {
                MoveUpState.ManualArea_ID = Area_Fixed;
                MassAreaSet(MoveDw, (MassageArea_E)MoveUpState.ManualArea_ID);
            }
            else
#endif
            {
                MoveUpState.ManualArea_ID = Area_Fixed;
                MassAreaSet(MoveUp, (MassageArea_E)MoveUpState.ManualArea_ID);
            }
        }
        else
        {
            Buzzer = SOUND_ALARM;
            Voice_Ack(VC_Invalid);
        }
        break;

    case 0x51: // go little down
        if (WorkStateGet() == WorkState_Work && AutoModeGet() == 0)
        {
            if (MassAreaGet(MoveUp) == Area_Fixed || MassAreaGet(MoveUp) == Area_Part)
            {
                Go_little_Flag = 1;
                TICK_RESET(Go_little_tick);
                KeyPush.TreadKeyState = TreadState_Dw;
                Voice_Ack(VC_Littdw);
            }
            else
            {
                Voice_Ack(VC_Invalid);
            }
        }
        else
        {
            Voice_Ack(VC_Invalid);
        }

        // Voice_Ack(VC_Littdw);
        // DDG_Set_SitDownCircle(DDG_BCircle_UpDown >> 1, DDG_LCircle_UpDown >> 1);
        Voice_Old_Date = value;
        TICK_RESET(Voice_tick);
        break;

    case 0x52: // go little up
        if (WorkStateGet() == WorkState_Work && AutoModeGet() == 0)
        {
            if (MassAreaGet(MoveUp) == Area_Fixed || MassAreaGet(MoveUp) == Area_Part)
            {
                Go_little_Flag = 1;
                TICK_RESET(Go_little_tick);
                KeyPush.TreadKeyState = TreadState_Up;
                Voice_Ack(VC_Littup);
            }
            else
            {
                Voice_Ack(VC_Invalid);
            }
        }
        else
        {
            Voice_Ack(VC_Invalid);
        }
        // Voice_Ack(VC_Littup);
        // DDG_Set_SitUpCircle(DDG_BCircle_UpDown >> 1, DDG_LCircle_UpDown >> 1);
        Voice_Old_Date = value;
        TICK_RESET(Voice_tick);
        break;

    case 0x58: // stop adjustment
        Voice_Ack(VC_Stopadj);
        DDG_Set_Stop();
        Voice_Old_Date = value;
        TICK_RESET(Voice_tick);
        break;
#elif defined VIETNAM_VOICE
    case 0x32: //* 舒适按摩
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 1)
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(1);
                Voice_Ack(VC_OK);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 1)
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(1);
                Voice_Ack(VC_OK);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }

        break;

    case 0x33: //* 轻松按摩
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 2)
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(2);
                Voice_Ack(VC_OK);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 2)
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(2);
                Voice_Ack(VC_OK);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }

        break;

    case 0x34: //* 全身按摩
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 6)
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(6);
                Voice_Ack(VC_OK);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 6)
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(6);
                Voice_Ack(VC_OK);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }

        break;

    case 0x35: //* 肩颈按摩
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 5)
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(5);
                Voice_Ack(VC_OK);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 5)
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(5);
                Voice_Ack(VC_OK);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }

        break;

    case 0x36: //* 背腰按摩
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 4)
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(4);
                Voice_Ack(VC_OK);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 4)
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(4);
                Voice_Ack(VC_OK);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x37: //* 牵引按摩
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 3)
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(3);
                Voice_Ack(VC_OK);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 3)
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(3);
                Voice_Ack(VC_OK);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x38: //* 定点按摩
        if (WorkStateGet() == WorkState_Work)
        {
            Voice_Old_Date = value;
            TICK_RESET(Voice_tick);
            Voice_Ack(VC_OK);
            if (AutoModeGet() > 0) // * 增加 250322
            {
                MassWiseSet(MoveUp, TZ);
                MassWiseSet(MoveDw, TZ);
                UserFunc_Set(UserFunc_ManualMode, 0);
            }

            if (MassWiseGet(MoveUp) == JRZ)
            {
                Buzzer = SOUND_ALARM;
                return;
            }
#ifdef USE_M2
            if (MoveWhichGet())
            {
                MoveUpState.ManualArea_ID = Area_Fixed;
                MassAreaSet(MoveDw, (MassageArea_E)MoveUpState.ManualArea_ID);
            }
            else
#endif
            {
                MoveUpState.ManualArea_ID = Area_Fixed;
                MassAreaSet(MoveUp, (MassageArea_E)MoveUpState.ManualArea_ID);
            }
        }
        else
        {
            Buzzer = SOUND_ALARM;
        }
        break;

    case 0x3a: //* go little down
        if (WorkStateGet() == WorkState_Work && AutoModeGet() == 0)
        {
            if (MassAreaGet(MoveUp) == Area_Fixed || MassAreaGet(MoveUp) == Area_Part)
            {
                Go_little_Flag = 1;
                TICK_RESET(Go_little_tick);
                KeyPush.TreadKeyState = TreadState_Dw;
                Voice_Ack(VC_OK);
            }
            else
            {
                // Voice_Ack(VC_Invalid);
            }
        }
        else
        {
            // Voice_Ack(VC_Invalid);
        }
        // Voice_Ack(VC_OK);
        // DDG_Set_SitDownCircle(DDG_BCircle_UpDown >> 1, DDG_LCircle_UpDown >> 1);
        Voice_Old_Date = value;
        TICK_RESET(Voice_tick);
        break;

    case 0x39: //* go little up
        if (WorkStateGet() == WorkState_Work && AutoModeGet() == 0)
        {
            if (MassAreaGet(MoveUp) == Area_Fixed || MassAreaGet(MoveUp) == Area_Part)
            {
                Go_little_Flag = 1;
                TICK_RESET(Go_little_tick);
                KeyPush.TreadKeyState = TreadState_Up;
                Voice_Ack(VC_OK);
            }
            else
            {
                // Voice_Ack(VC_Invalid);
            }
        }
        else
        {
            // Voice_Ack(VC_Invalid);
        }
        // Voice_Ack(VC_OK);
        // DDG_Set_SitUpCircle(DDG_BCircle_UpDown >> 1, DDG_LCircle_UpDown >> 1);
        Voice_Old_Date = value;
        TICK_RESET(Voice_tick);
        break;

    case 0x3b: //* change the other mode
        if (WorkStateGet() == WorkState_Work)
        {
            Voice_Old_Date = value;
            TICK_RESET(Voice_tick);
            Voice_Ack(VC_OK);

            if (AutoModeGet() > 0)
            {
                MassWiseSet(MoveDw, TZ);
                UserFunc_Set(UserFunc_ManualMode, 0);
                temp = ZYD;
            }
            else
            {
                temp = MassWiseGet(MoveUp);
            }

            switch (temp)
            {
            case TZ: // 揉捏
                i = RNZ;
                break;
            case RNZ: // 拍打2
                i = PD;
                break;
            case PD: // 揉捏拍打
                i = RPZ;
                break;
            case RPZ: // 敲击
                i = PDB;
                break;
            case PDB: // 指压
                i = ZYS;
                break;
            case ZYS: // 3D1
                i = D3RNZ;
                break;
            case D3RNZ: // 3D2
                i = D3PD;
                break;
            case D3PD: // 3D3
                i = D3RPZ;
                break;
            case D3RPZ: // 3D指压
                i = ZYD;
                break;
            case ZYD: // 3D指压
                i = TZ;
                break;
            default: // 停止
                i = TZ;
                break;
            }

            if (0 == MassSpeedGet(MoveUp))
                MassSpeedSet(MoveUp, 3);

            MassWiseSet(MoveUp, (ManualWise_E)i);
            MassIntensitySet(nBackIntensity << 1); // * 增加让 ZYD ZYS 工作 250319

            KeyPush.WiseKeyState = 1;
            // Buzzer = SOUND_ACK;
        }
        else
        {
            Buzzer = SOUND_ALARM;
        }
        break;

    case 0x3c: //* open air
        if (WorkStateGet() == WorkState_Work)
        {
            if (AirModeGet() > 0)
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                Voice_Ack(VC_OK);
                AirModeSet(AirModeFull);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else
        {
            Buzzer = SOUND_ALARM;
        }

        break;

    case 0x3d: //* close air
        if (WorkStateGet() == WorkState_Work)
        {
            if (AirModeClose == AirModeGet())
            {
                Buzzer = SOUND_ALARM;
            }
            else
            {
                Voice_Ack(VC_OK);
                AirModeSet(AirModeClose);
                memset(&AirT_Work, 0, sizeof(AirT_Work_S));
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else
        {
            Buzzer = SOUND_ALARM;
        }
        break;

    case 0x41: // up the seat
        Voice_Old_Date = value;
        TICK_RESET(Voice_tick);
        Voice_Ack(VC_OK);
        DDG_Set_SitUpCircle(DDG_BCircle_UpDown, DDG_LCircle_UpDown);
        break;

    case 0x42: // down the seat
        Voice_Old_Date = value;
        TICK_RESET(Voice_tick);
        Voice_Ack(VC_OK);
        DDG_Set_SitDownCircle(DDG_BCircle_UpDown, DDG_LCircle_UpDown);
        break;

    case 0x40: // stop adjustment
        Voice_Ack(VC_OK);
        DDG_Set_Stop();
        Voice_Old_Date = value;
        TICK_RESET(Voice_tick);
        break;
#else
    case 0x32: // 零压舒展
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 1)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(1);
                Voice_Ack(VC_Auto1);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 1)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(1);
                Voice_Ack(VC_Auto1);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }

        break;

    case 0x33: // 中式推拿
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 2)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(2);
                Voice_Ack(VC_Auto2);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 2)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(2);
                Voice_Ack(VC_Auto2);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }

        break;

    case 0x34: // 疲劳恢复
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 3)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(3);
                Voice_Ack(VC_Auto3);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 3)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(3);
                Voice_Ack(VC_Auto3);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x35: // 4 海浪轻柔
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 4)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(4);
                Voice_Ack(VC_Auto4);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 4)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(4);
                Voice_Ack(VC_Auto4);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }

        break;

    case 0x36: //* 泰式拉筋
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 5)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(5);
                Voice_Ack(VC_Auto5);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 5)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(5);
                Voice_Ack(VC_Auto5);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x37: //* 舒筋活络
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 6)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(6);
                Voice_Ack(VC_Auto6);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 6)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(6);
                Voice_Ack(VC_Auto6);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x38: //* 肩颈护理
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 7)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(7);
                Voice_Ack(VC_Auto7);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 7)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(7);
                Voice_Ack(VC_Auto7);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x39: //* 腰部护理
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 9)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(9);
                Voice_Ack(VC_Auto8);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 9)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(9);
                Voice_Ack(VC_Auto8);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x3a: //* 腿足舒缓
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 10)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(10);
                Voice_Ack(VC_Auto9);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 10)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(10);
                Voice_Ack(VC_Auto9);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x3b: //* 元气唤醒
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 16)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(16);
                Voice_Ack(VC_Auto10);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 16)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(16);
                Voice_Ack(VC_Auto10);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x3c: //* 安神助眠
        if (WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset)
        {
            Buzzer = SOUND_ALARM;
        }
        else if (WorkStateGet() == WorkState_AutoLying || WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_ShoulderAdj)
        {
            if (AutoModeGet() == 20)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(20);
                Voice_Ack(VC_Auto11);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_WaitFunc) // * 250222
        {
            if (AutoModeGet() == 20)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                AirModeSet(AirModeAuto);
                AutoModeSet(20);
                Voice_Ack(VC_Auto11);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        break;

    case 0x3d: //* 关闭气压
        if (WorkStateGet() == WorkState_Work)
        {
            if (AirModeClose == AirModeGet())
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                Voice_Ack(VC_AirOff);
                AirModeSet(AirModeClose);
                memset(&AirT_Work, 0, sizeof(AirT_Work_S));
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else
        {
            Buzzer = SOUND_ALARM;
        }

        break;

    case 0x40: //* 打开气压
        if (WorkStateGet() == WorkState_Work)
        {
            if (AirModeGet() > 0)
            {
                Buzzer = SOUND_ALARM;
                Voice_Ack(VC_Invalid);
            }
            else
            {
                Voice_Ack(VC_AirOn);
                AirModeSet(AirModeFull);
                Voice_Old_Date = value;
                TICK_RESET(Voice_tick);
            }
        }
        else
        {
            Buzzer = SOUND_ALARM;
        }
        break;

    case 0x41: // 坐姿上升
        Voice_Ack(VC_SitUp);
        DDG_Set_SitUpCircle(DDG_BCircle_UpDown, DDG_LCircle_UpDown);
        Voice_Old_Date = value;
        TICK_RESET(Voice_tick);
        break;

    case 0x42: // 坐姿下降
        Voice_Ack(VC_SitDown);
        DDG_Set_SitDownCircle(DDG_BCircle_UpDown, DDG_LCircle_UpDown);
        Voice_Old_Date = value;
        TICK_RESET(Voice_tick);
        break;
#endif

    default:
        Voice_Old_Date = 0;
        break;
    }
}

void VoiceControl_Handler(void)
{
    uint8_t CheckSum = 0;
    if (Go_little_Flag) // * add for go little up/down 250812
    {
        if (TICK_COUNTER(Go_little_tick, 2000))
        {
            Go_little_Flag = 0;
            KeyPush.TreadKeyState = TreadState_Stop;
        }
    }

    while (VoiceControl_Real_Read != VoiceControl_Real_Write)
    {
        VoiceControl_HandBuffer[VoiceControl_HandCnt] = VoiceControl_GetValue();

        switch (VoiceControl_RX_Status)
        {
        default:
            if (VoiceControl_HandBuffer[VoiceControl_HandCnt] == VoiceControl_Head)
                VoiceControl_RX_Status = 1, VoiceControl_HandCnt++;

            break;

        case 1:
            if (VoiceControl_HandBuffer[VoiceControl_HandCnt] == VoiceControl_Tial)
                VoiceControl_RX_Status = 2, VoiceControl_HandCnt++;
            else
            {
                if (VoiceControl_HandBuffer[VoiceControl_HandCnt] != VoiceControl_Head)
                    VoiceControl_RX_Status = 0, VoiceControl_HandCnt = 0;
            }

            break;

        case 2:

            if (++VoiceControl_HandCnt >= VoiceControl_Receive_Data)
                VoiceControl_HandCnt = 0, VoiceControl_RX_Status = 3;

            break;
        }

        if (VoiceControl_RX_Status == 3)
        {
            VoiceControl_RX_Status = 0;
            if (VoiceControl_HandBuffer[2] == 0xe0 && VoiceControl_HandBuffer[3] == 0x0e) // * 唤醒词 小艾小艾
            {
                // Voice_Flag = 0;
                // Poweroff_Voice_Flag = 0;
                Voice_Old_Date = 0;
                if (nVoiceControlSw)
                    Voice_Ack(VC_WakeUp);
                // else //* 关闭没有声音 250812
                //     Voice_Ack(VC_Invalid);
                return;
            }
            if (nVoiceControlSw == 0) // * 语音识别关闭 250306
                return;

            CheckSum = VoiceControl_HandBuffer[2] + VoiceControl_HandBuffer[3]; // 计算校验和
            if (CheckSum != VoiceControl_HandBuffer[4])
                return;

            if (VoiceControl_HandBuffer[2] == 0x02)
            {
                Voice_Cmd_Handler(VoiceControl_HandBuffer[3]);
                Voice_WakeUp(); // @ add 260114
            }
        }
    }
}
