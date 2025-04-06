/*!
    \file  main.c
    \brief GPIO running led demo

    \version 2017-02-10, V1.0.0, demo for GD32F30x
    \version 2018-10-10, V1.1.0, demo for GD32F30x
    \version 2018-12-25, V2.0.0, demo for GD32F30x
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32f30x.h"
#include <stdio.h>
#include "systick.h"
#include "config.h"
#include "main.h"

#define TxChMax 4
uint8_t TxPolling = 0;
uint8_t TxPollingPre;
uint8_t TxKeepCnt = 0;
uint8_t PumpState = 0;

uint8_t LegResetStep = 0;
uint8_t MoveDwResetStep = 0;
uint8_t MoveUpResetStep = 0;

uint8_t Core_RestSuccess = 0;
void _485ComRx(void)
{

    u8 data;

    while(COM4.RX_read != COM4.RX_write)
    {

        data = EUART4_Read();

        MoveUpDataTx(data);
        MoveDwDataTx(data);
        LegDataRx(data);
        ArmKeyDataTx(data);
    }
}

void MoveUp_Rest(void)
{
    uint16_t MovoUpCurrent = 0;

    switch(MoveUpResetStep)
    {
        case 0:
            MoveUpKneadStateSet(7, 3, 0);

            if(MoveComplete == GetMoveUpDataTx()->Knead.State)
            {
                MoveUpKneadStateSet(0, 0, 0);
                MoveUpResetStep++;
            }

            break;

        case 1:
            MoveUpTapStateSet(0);
            MoveUpTx_DriveCmd = 1;
            MoveUpResetStep++;
            break;

        case 2:
            MovoUpCurrent = (GetMoveUpDataTx()->Tap.Current_L8) | (GetMoveUpDataTx()->Tap.Current_H8 << 8);

            if(0 == MovoUpCurrent)
            {
                MoveUpResetStep++;
            }

            break;

        case 3:
            MoveUpKneadStateSet(5, 3, 65);
            MoveUpFlexStateSet(2, 3, 0);
            MoveUpTx_DriveCmd = 1;
            MoveUpResetStep++;
            break;

        case 4:
            if(MoveForeward == GetMoveUpDataTx()->Knead.State)
            {
                MoveUpResetStep++;
            }

            break;

        case 5:
            if(((GetMoveUpDataTx()->Knead.State == MoveComplete) || (GetMoveUpDataTx()->Knead.State == MoveStop)) && ((GetMoveUpDataTx()->Flex.Place == MassPlace_Bot)))  // todo 修改 250218
            {
                MoveUpKneadStateSet(0, 0, 0);
                MoveUpFlexStateSet(0, 0, 0);
                MoveUpResetStep++;
            }

            break;

        case 6:
            MoveUpTreadStateSet(1, 5, 0); // todo 复位速度 最快3-5
            MoveUpTx_DriveCmd = 1;
            MoveUpResetStep++;
            break;

        case 7:
            if((MoveForeward == GetMoveUpDataTx()->Tread.State) || (UpTreadPlace & MassPlace_Top))
            {
                MoveUpResetStep++;
            }

            break;

        case 8:
            if((GetMoveUpDataTx()->Tread.State == MoveComplete) || (GetMoveUpDataTx()->Tread.State == MoveStop) || ((UpTreadPlace & MassPlace_Top) == MassPlace_Top))
            {
                MoveUpTreadStateSet(5, 3, 150);
                //              if(MoveBack == GetMoveUpDataTx()->Tread.State)
                MoveUpResetStep++;
            }

            break;

        case 9:
            if(MoveBack == GetMoveUpDataTx()->Tread.State)
            {
                MoveUpResetStep++;
            }

            break;

        case 10:
            if((GetMoveUpDataTx()->Tread.State == MoveComplete) || (GetMoveUpDataTx()->Tread.State == MoveStop))
            {
                MoveUpTreadStateSet(0, 0, 0);
                MoveUpKneadStateSet(5, 3, 0);
                MoveUpTx_DriveCmd = 1;
                MoveUpResetStep++;
            }

            break;

        case 11:
            if(MoveForeward == GetMoveUpDataTx()->Knead.State)
            {
                MoveUpResetStep++;
            }

            break;

        case 12:
            if((GetMoveUpDataTx()->Knead.State == MoveComplete) || (GetMoveUpDataTx()->Knead.State == MoveStop))
            {
                MoveUpKneadStateSet(0, 0, 0);

                MoveUpResetStep = 0;
#if (USE_M2)
                Core_RestSuccess |= 0x01; /* 初始化完成 */
#else
                Core_RestSuccess |= 0x03; /* 初始化完成 */
#endif
            }

            break;

        default:
            break;
    }
}

/*****************************************************************************************************
 * 描述：下机芯复位动作
 * 参数：无
 * 返回：无
 * 注意：上电复位动作，需要监测各类信号并做出相应动作；机芯上电计数位置不确定
 *****************************************************************************************************/
void MoveDw_Rest(void)
{
    //  uint16_t MovoDwCurrent = 0;

    switch(MoveDwResetStep)
    {
        case 0:
            MoveDwTapStateSet(0);
            MoveDwKneadStateSet(7, 3, 0);

            if(MoveComplete == GetMoveDwDataTx()->Knead.State)
            {
                MoveDwKneadStateSet(0, 0, 0);
                MoveDwResetStep++;
            }

            break;

        case 1:
            MoveDwKneadStateSet(5, 3, 0);

            if((MoveForeward == GetMoveDwDataTx()->Knead.State) || (MoveBack == GetMoveDwDataTx()->Knead.State))
            {
                MoveDwResetStep++;
            }

            break;

        case 2:
            if(MoveComplete == GetMoveDwDataTx()->Knead.State)
            {
                MoveDwKneadStateSet(0, 0, 68);
                MoveDwResetStep++;
            }

            break;

        case 3:
            MoveDwTreadStateSet(2, 5, 0); // todo 复位速度 最快3-5

            if((GetMoveDwDataTx()->Tread.Place & MassPlace_Bot) == MassPlace_Bot)
            {
                MoveDwTreadStateSet(0, 0, 0);
                MoveDwResetStep = 0;
                Core_RestSuccess |= 0x02;
            }

            break;
    }
}

void CoreRest(void)
{
    //  if (Core_RestSuccess != 0x03)
    //  {
    //      MoveUp_Rest();
    // #if (USE_M2)
    //      MoveDw_Rest();
    // #endif
    //  }
    if((Core_RestSuccess & 0x01) == 0x00)  // todo 修改 250215
    {
        MoveUp_Rest();
    }

#if (USE_M2)

    if((Core_RestSuccess & 0x02) == 0x00)
    {
        MoveDw_Rest();
    }

#else
    Core_RestSuccess |= 0x02; // todo 单机芯下机芯直接复位成功
#endif
}

void Variable_Init(void)
{
    Flash_Parameter_read();
#ifndef Handle_Old_Version
    User_Memory_Use_DIY(); // todo 增加
#endif
    KeyPush.WiseKeyState = 0; // todo 初始化把这个标志位清零 250304
}

// todo 调试
u8 Debug_Para = 0;
void Debug_Pro(void)
{
    if(Debug_Para == 0)
    {
        return;
    }

    if(Debug_Para == 1)
    {
        Key.State = 2;
        Key.Value = Key_Power;
        Key.Param = 0;
    }
    else if(Debug_Para < 21)
    {
        Key.State = 2;
        Key.Value = Key_AutoMode;
        Key.Param = Debug_Para;
    }
    else if(Debug_Para == 21)
    {
        Key.State = 2;
        Key.Value = Key_BodyCheck_Jump;
    }
    else if(Debug_Para == 22)
    {
        Key.State = 2;
        Key.Value = Key_Wise;
        Key.Param = 0;
    }
    else if(Debug_Para == 23)  // todo 模拟气压按键测试
    {
        Key.State = 2;
        Key.Value = Key_Air;
        Key.Param = 3;
    }

    KeyFunc();

    Debug_Para = 0;
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x4000);
	
    /* configure systick */
    systick_config();
	__enable_irq();
    ota_init();
    UserConfig();
    AirInit();
    HeatInit();
    Motor_Init();
    PowerInit();
    ArmKeyInit();
    LegInit();
    MoveUpInit();
    AT_Init();
    rgbInit();
    Blue_Init();
    Health_Init();
    Variable_Init(); //* 增加 变量初始化
    UserFunc_Set(UserFunc_PowerOff, 0);

    while(1)
    {

        PowerProcess();
        ADC_Process();
        // 蓝牙
        os_task_BlueRx();
        os_task_blue(TASK_ID_BLUE);
        os_task_voice(TASK_ID_VOICE);
        os_task_app(TASK_ID_APP);

#if (USE_Health_Check)
        os_task_Health(TASK_ID_HEALTH);
#endif // USE_Health_Check

        // 声控
        VoiceControl_Handler();
        _485ComRx();

        Light_Update();

        KeyCom_Handler();    /* 手控器接收数据 */
        KeyEncoderHandler(); /* 按键板编码器处理 */

        ErrorCheckProcess(); // todo 故障代码
#ifndef J0201_LC

        if(MChair.CFG.Bit.CoreReset)
        {
            CoreRest();
        }

#endif

        if(Timer_5ms_Cnt > 5)
        {
            OS_RTC_TICK++;
            Timer_5ms_Cnt = 0;
            MotorProcess();
            // if (Flash_Check_Tick > 0) // todo 修改成 关机保存
            // {
            //  if (--Flash_Check_Tick == 0)
            //  {
            //      Flash_Parameter_Write();
            //  }
            // }
        }

        if(Timer_50ms_Cnt > 50)
        {
            Timer_50ms_Cnt = 0;

            /*485轮询通讯*/
            if(TxPolling >= TxChMax)
            {
                TxPolling = 0;
            }

            if(TxPollingPre != TxPolling)
            {
                TxKeepCnt = 0;
                TxPollingPre = TxPolling;
            }
            else
            {
                TxKeepCnt++;    /* 若只有一个通讯设备，该处屏蔽，一般不需要屏蔽 */
            }

            if(TxKeepCnt > 3)
            {
                switch(TxPolling)
                {
                    case 0:
                        ArmKeyTx_DriveCmd = 1;
                        break;

                    case 1:
                        LegTx_DriveCmd = 1;
                        break;

                    case 2:
                        MoveUpTx_DriveCmd = 1;
                        break;

                    case 3:
                        MoveDwTx_DriveCmd = 1;
                        break;
                }

                TxPolling++;
                TxKeepCnt = 0;
            }

            switch(TxPolling)
            {

                case 0:

                    ArmKeyDataRx();
                    break;

                case 1:

                    LegDataTx();
                    break;

                case 2:

                    MoveUpDataRx();
                    break;

                case 3:

                    MoveDwDataRx(); /* 陈工写的Rx为目标板动作而非主板动作，后续看情况修改 */
                    break;
            }

            /*------------------------------------------------------------------------------------*/
        }

        if(Timer_100ms_Cnt >= 100)
        {

            Timer_100ms_Cnt = 0;

            Health_OutTime_Hand();

            if(nPlay_time)
            {
                nPlay_time--;
            }

            // Display_Drive(); // todo 100ms->200ms->100ms 250321
            AT_Process();
            AirProcess();
            WorkStateProcess(); /* 工作状态调度 */
            HeatProcess();
            KeyPowerScan();

            FootRollProcess();
            CalfKneadProcess();
        }

        if(Timer_200ms_Cnt >= 200)
        {
            Timer_200ms_Cnt = 0;

            Timer_1s_Cnt++;
            BackPosUpdate();
            Display_Drive(); // todo 100ms ->200ms 250318
#ifdef LED2_PWM
            Led_Pwm_Update();
#endif
        }

        if(Timer_1s_Cnt >= 5)
        {
            Timer_1s_Cnt = 0;

            MCU_LedProcess();
            Eor_Update();
            Debug_Pro(); // todo 调试程序 250210
#ifdef A391S_LC

            if(AutoModeGet() == 19)  // todo 19号暂时为 老化程序 250305
            {
                nLCState = 1;
            }
            else
            {
                nLCState = 0;
            }

#endif

            if(!nLCState)  // 非老化模式下需要定时
            {
                if(WorkStateGet() == WorkState_Work)
                {
                    if(WorkSecGet() > 0)
                    {
                        WorkSecSet(WorkSecGet() - 1);
                    }
                    else
                    {
                        if(WorkMinGet() > 0)
                        {
                            WorkMinSet(WorkMinGet() - 1);
                            WorkSecSet(59);
                        }
                        else
                        {
                            if(nTimOutResetSw)
                            {
                                UserFunc_Set(UserFunc_PowerOff, 0);
                            }
                            else
                            {
                                UserFunc_Set(UserFunc_PowerOff, 1);
                            }
                        }
                    }
                }
            }
        }
    }
}
