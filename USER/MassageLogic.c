#include "MassageLogic.h"
#include "Table.h"

// @ 调试程序
#ifdef Debug_Mode
u8 Debug_Para = 0;
void Debug_Pro(void)
{
    if (Debug_Para == 0)
        return;
    if (Debug_Para == 91)
    {
        Key.State = 2;
        Key.Value = Key_Power;
        Key.Param = 0;
    }
    else if (Debug_Para < 26)
    {
        Key.State = 2;
        Key.Value = Key_AutoMode;
        Key.Param = Debug_Para;
    }
    else if (Debug_Para == 30)
    {
        Key.State = 2;
        Key.Value = key_AutoLyJump;
    }
    else if (Debug_Para == 31)
    {
        Key.State = 2;
        Key.Value = Key_BodyCheck_Jump;
    }
    else if (Debug_Para == 32)
    {
        Key.State = 2;
        Key.Value = Key_Wise;
        Key.Param = 0;
    }
    else if (Debug_Para == 33) // * 模拟气压按键测试
    {
        Key.State = 2;
        Key.Value = Key_Air;
        Key.Param = 3;
    }

    KeyFunc();

    Debug_Para = 0;
}
#endif

static bool MoveUpDw_Miss_Resume_Flag = 0; // * 增加250424
static u8 MoveUpDw_Miss_BodyCheck_Cnt = 0;

u8 Leg_Protect_Flag = 0;

bool Leg_Touch_Flag = 0;       // * 触底发生标志位 250513
bool Leg_Touch_Air_Flag = 0;   // * 触地关闭腿脚气压标志 250612
static u32 Leg_Touch_Tick = 0; // * 触底发生超时
bool STETCHGAS_Touch_Flag = 0; // * 增加 250610
bool MoveUpDw_Loss_Flag = 0;

// * 新增 小腿防夹 等 保护动作
static void Protect_Process(void) //* 100ms 时基
{
#ifndef M1M2LOSS_NO_PAUSE
// * 上下机芯无心跳暂停
#ifdef USE_M2
    if (TICK_COUNTER(MoveUp_Loss_tick, 3000) || TICK_COUNTER(MoveDw_Loss_tick, 3000))
#else
    if (TICK_COUNTER(MoveUp_Loss_tick, 3000))
#endif
    {
        if (WorkStateGet() == WorkState_BodyCheck) // * 立即跳转
        {
            if (++MoveUpDw_Miss_BodyCheck_Cnt > 20)
            {
                MoveUpDw_Miss_BodyCheck_Cnt = 0;
                MassWiseSet(MoveUp, TZ);
                BodyPlaceCal(DEFAULT_SHOULDER);
                WorkStateSet(WorkState_Work);
            }
        }
        else if (WorkStateGet() == WorkState_Work)
        {
            // Enter_Pause();
            // MoveUpDw_Miss_Resume_Flag = 1;

            // if (AutoModeGet() > 0 || (WorkStateGet() == WorkState_WaitFunc))
            // {
            //     MassWiseSet(MoveUp, TZ); // * 通讯故障停止 251210
            //     MassWiseSet(MoveDw, TZ);
            //     UserFunc_Set(UserFunc_ManualMode, 0);
            // }
            MoveUpDw_Loss_Flag = 1;
        }
        else if (WorkStateGet() == WorkState_TurnOn && nLowPower) // todo md 260702
        {
            MoveUpDw_Loss_Flag = 1;
            WorkStateSet(WorkState_WaitFunc);
        }
        Buzzer_Ararm = 0x40; //* 通信故障报警声
    }
    else
    {
        Buzzer_Ararm &= 0xbf;
        MoveUpDw_Loss_Flag = 0;
        // if (MoveUpDw_Miss_Resume_Flag && WorkStateGet() != WorkState_Pause)
        // {
        //     MoveUpDw_Miss_Resume_Flag = 0;
        //     PreventFlag = 3;
        //     Prevent_Cnt = 9; // * 模拟复位
        // }
    }
#endif

    // if (AutoModeGet() == 0) // * 250610
    //     STETCHGAS_Touch_Flag = 0;
    if (AutoModeGet() > 0 && AtTabGet(Table_Sub)->CurSubTab >= SUBTB_STR1 && AtTabGet(Table_Sub)->CurSubTab <= SUBTB_STR9) // * md 251113
        STETCHGAS_Touch_Flag = 1;
    else
        STETCHGAS_Touch_Flag = 0;

    /* 小腿伸缩推杆触地动作 */
    if (GetLegDataRx()->Singal.Touch && WorkStateGet() != WorkState_Wait)
    {
        // * 触地不防夹 回缩 250421 关机不动 250507 增加拉伸不动 250612
        if (STETCHGAS_Touch_Flag)
        {
            FootFlexStateSet(FlexStop);
            FootFlexSpeedSet(0);
        }
        else if ((GetLegDataRx()->Singal.Protect) == 0 && (GetLegDataRx()->Singal.Protect2) == 0) // todo md 260629
        {
            Leg_Touch_Flag = 1; // * 转移到这里 250612
            TICK_RESET(Leg_Touch_Tick);
            FootFlexStateSet(FlexUp);
            FootFlexSpeedSet(5);
        }
    }
    // * 增加超时、小腿位置判断 250514
    if ((GetLegDataRx()->Singal.Touch == 0 || WorkStateGet() == WorkState_Wait || WorkStateGet() == WorkState_Reset) &&
        (Leg_Touch_Flag || Leg_Touch_Air_Flag) &&
        (GetLegDataRx()->Flex.Place == 2 || DDG_Get_LnCurCircle > 30 || TICK_COUNTER(Leg_Touch_Tick, 20000)))
    {
        Leg_Touch_Flag = 0;
        if (GetLegDataRx()->Flex.Place == 2 || TICK_COUNTER(Leg_Touch_Tick, 20000))
            Leg_Touch_Air_Flag = 0;
        else
            Leg_Touch_Air_Flag = 1;
        // FootFlexStateSet(FlexStop); //* 添加有问题
        // FootFlexSpeedSet(0);
    }

    /* 小腿防夹动作 */
    if (GetLegDataRx()->Singal.Protect || GetLegDataRx()->Singal.Protect2) // todo md 260629
    {
        if (WorkStateGet() == WorkState_TurnOn)
        {
            // State_OutTime = 0;
            WorkStateSet(WorkState_WaitFunc);
#ifdef LEG_COM
            FootFlexStateSet(FlexStop); /* 调节推杆时，停止当前找脚动作，调节完成后看情况继续 */
            FootFlexSpeedSet(0);
#endif
        }

        if (WorkStateGet() != WorkState_Wait && WorkStateGet() != WorkState_FCT)
        {
            if (WorkStateGet() != WorkState_Pause)
            {
                Enter_Pause(); // @ 王工调试时屏蔽 250402
                Leg_Protect_Flag = 1;
            }
            else if (Health_State != Health_State_Stop) // * add for health test with leg protect 250821
            {
                Leg_Protect_Flag = 1;
            }
            Buzzer_Ararm = 0x10; //* 防夹报警声
        }
    }
    else
    {
        Buzzer_Ararm &= 0xef;
        if (Leg_Protect_Flag)
        {
            Leg_Protect_Flag = 0;
            // Exit_Pause(); //* 防夹采用手动恢复 251119 由于防夹传感器50秒左右会自动恢复
        }
    }
}
//* 健康检测 顶出收回
static void HealthPrepare_Deal(void)
{
    if (Health_State == Health_State_Check && Massage_Flag.HealthPrepare)
    {
        Part_Date_Set(MoveUp_Flex, 1, FLEX_FIX_CIRCLE, 0, 0, 6);
#ifdef USE_M2_4D
        Part_Date_Set(MoveDw_Flex, 1, FLEX_FIX_CIRCLE, 0, 0, 6); // todo add 260127
#endif
        if (MSG4Move[MoveUp].Flex.CircleH == 0 && MSG4Move[MoveUp].Flex.CircleL < 5)
        {
            Massage_Flag.HealthPrepare = 0;
            Enter_Pause();
        }
    }
}
//* 肩部气压与机芯伸缩矛盾
static void AirSHD_MoveUp_Paradox(void)
{
    if (AutoModeGet() == 0 || nLCState) //* 只有正常自动模式有效
        return;
    AirT_Work.Paradox[AirT_SHD] = 0;
    u16 moveupcircle = MoveCurTreadCircleGet(MoveUp);

    if (moveupcircle > (BodyArea[2] + 5) && moveupcircle < (BodyArea[6] - 5) &&
        (MSG2Move[MoveUp].Flex.Para1 > (16 + NECK_FLEX_ADD * 3) || MSG4Move[MoveUp].Knead.CircleL > 55 || MSG4Move[MoveUp].Knead.CircleL < 20))
    {
        AirT_Work.Paradox[AirT_SHD] = 1;
    }
    // if (moveupcircle < (BodyArea[2] + 35) || moveupcircle > (BodyArea[2] + 220)) //* 肩部位置
    // {
    //     return;
    // }
    // if (MSG2Move[MoveUp].Flex.Para1 < 20)
    //     return;
    // AirT_Work.Paradox[AirT_SHD] = 1;
}
//* 下身气压与小腿保护和找脚矛盾
static void AirLower_Leg_Paradox(void)
{
    if (FootFlexStateGet() == FootFind || SIT_NONE_Flag ||
        (STETCHGAS_Touch_Flag == 0 && (Leg_Touch_Flag || Leg_Touch_Air_Flag) && DDG_Get_LnCurCircle > 20))
    {
        AirT_Work.Paradox[AirT_CalfSide] = 1;
        AirT_Work.Paradox[AirT_FeetSide] = 1;
        AirT_Work.Paradox[AirT_Ankle] = 1;
#if (AIR_CLAFBOT)
        AirT_Work.Paradox[AirT_CalfBot] = 1;
#endif
    }
    else
    {
        AirT_Work.Paradox[AirT_CalfSide] = 0;
        AirT_Work.Paradox[AirT_FeetSide] = 0;
        AirT_Work.Paradox[AirT_Ankle] = 0;
#if (AIR_CLAFBOT)
        AirT_Work.Paradox[AirT_CalfBot] = 0;
#endif
    }
}

// @ 音波震动和机芯位置矛盾 260320
#ifdef USE_SOUNDWAVE
#define Hy_Circle 10
static void SoundWave_M1_Paradox(void)
{
    if (AutoModeGet() == 0 || nLCState) //* 只有正常自动模式有效
        return;
    if (!nSoundWaveSW) //* 没开启音波开关跳过
        return;
    u16 moveupcircle = MoveCurTreadCircleGet(MoveUp);
    if (SoundWave_M1_Paradox_Flag && (nVolume_SoundWave != nVolume_SoundWave_Real))
    { // * 等待恢复正常 滞回
        if (moveupcircle > (BodyArea[LB_H2] + Hy_Circle))
        {
            SoundWave_M1_Paradox_Flag = 0;
            SoundWave_Resume();
        }
    }
    else
    {
        if (moveupcircle <= BodyArea[LB_H2])
        {
            SoundWave_M1_Paradox_Flag = 1;
            SoundWave_Reduce();
        }
    }

    // if (SoundWave_M1_Paradox_Flag)
    // { // * 等待恢复正常 滞回
    //     if ((moveupcircle > (BodyArea[LB_H1] + Hy_Circle) && moveupcircle < (BodyArea[LB_H2] - Hy_Circle)) ||
    //         moveupcircle > (BodyArea[LB_3] + Hy_Circle))
    //     {
    //         SoundWave_M1_Paradox_Flag = 0;
    //         SoundWave_Resume();
    //     }
    // }
    // else
    // { // @ 进入矛盾区 颈部以上 与 心脏区域 实际位置需要重新确认 260324
    //     if (moveupcircle <= BodyArea[LB_H1] || (moveupcircle <= BodyArea[LB_3] && moveupcircle >= BodyArea[LB_H2]))
    //     {
    //         SoundWave_M1_Paradox_Flag = 1;
    //         SoundWave_Stop();
    //     }
    // }

    // if (SoundWave_M1_Paradox_Flag)
    // { // * 等待恢复正常 滞回
    //     if (moveupcircle > (BodyArea[LB_H1] + Hy_Circle))
    //     {
    //         SoundWave_M1_Paradox_Flag = 0;
    //         SoundWave_Resume();
    //     }
    // }
    // else
    // {
    //     if (moveupcircle <= BodyArea[LB_H1])
    //     {
    //         SoundWave_M1_Paradox_Flag = 1;
    //         SoundWave_Stop();
    //     }
    // }
}
#endif

static void Customize_Require(void)
{
}

void MassageLogic_Process(void) // * 100ms时基 各部件之间逻辑处理
{
#ifdef USE_SOUNDWAVE
    SoundWave_M1_Paradox(); // @ 音波震动和机芯位置矛盾 260320
#endif
    Customize_Require();     //* 定制运行参数
    AirSHD_MoveUp_Paradox(); //* 肩部气压与机芯伸缩矛盾
    AirLower_Leg_Paradox();  //* 下身气压与小腿保护和找脚矛盾
    M1_M2_void();            //* 上下机芯 碰撞处理
    Protect_Process();       //* 小腿防夹 触地等保护
    HealthPrepare_Deal();    //* 健康检测预处理 3D收回
}
