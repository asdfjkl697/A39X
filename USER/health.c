/*
 * 文件包含 *************************************************************************************************************************************
 */
#include "health.h"
#include "MassageLogic.h"

#if (USE_Health_Check)
/*
 * 类型定义 *************************************************************************************************************************************
 */
Health_State_enum Health_State; // 检测状态

/*
 * 变量定义 *************************************************************************************************************************************
 */
u8 Health_Cnt;      // 检测进度
u16 Health_OutTime; // 检测超时计数
u16 Health_OutCnt;  // 单次检测超时计数

u8 Health_Heart; // 心率
u8 Health_Blood; // 血样
u8 Health_Tired; // 疲劳度
u8 Health_WXH;   // 微循环

u8 Health_Heart_Buf[Health_MaxCheckCnt + 1];
u8 Health_Blood_Buf[Health_MaxCheckCnt + 1];
u8 Health_Tired_Buf[Health_MaxCheckCnt + 1];
u8 Health_WXH_Buf[Health_MaxCheckCnt + 1];

/*
 ************************************************************************************************************************************************
 * 函数定义 *************************************************************************************************************************************
 ************************************************************************************************************************************************
 */

/*****************************************************************************************************
 * 描述:处理健康检测获取的数据
 * 参数:np：要操作的地址
        nmaxvalue：要操作的长度
 * 返回:无
 * 注意:无
*****************************************************************************************************/
u8 Health_Hand_Value(u8 *np, u8 nmaxvalue) // 排序
{
    u8 i, j, temp;
    u32 temp1;

    for (i = nmaxvalue; i > 0; i--)
    {
        for (j = 0; j < i - 1; j++)
        {
            if (np[j] > np[j + 1])
            {
                temp = np[j];
                np[j] = np[j + 1];
                np[j + 1] = temp;
            }
        }
    }

    // 取中间1/3的平均值
    i = nmaxvalue / 3 - 1;
    j = nmaxvalue * 2 / 3;
    temp = 0;
    temp1 = 0;
    for (; i < j; i++)
    {
        temp1 += np[i];
        temp++;
    }

    temp1 /= temp;

    return (u8)temp1;
}

/*****************************************************************************************************
 * 描述:
 * 参数:
 * 返回:
 * 注意:
 *****************************************************************************************************/
void Health_Set_State(Health_State_enum nState)
{
    Health_State = nState;
    Health_OutTime = 0;
    Health_Cnt = 0;
    Health_Heart = 0; // 心率
    Health_Blood = 0; // 血氧
    Health_Tired = 0; // 疲劳等级
    Health_WXH = 0;   // 微循环
    Pt_Reset_ID(TASK_ID_HEALTH);
}

/*****************************************************************************************************
 * 描述:
 * 参数:
 * 返回:
 * 注意:
 *****************************************************************************************************/
void Health_Init(void)
{
    Health_State = Health_State_Stop; // 检测状态
    Health_Cnt = 0;                   // 检测进度
    Health_OutTime = 0;               // 检测超时计数
    Health_OutCnt = 0;                // 单次检测超时计数

    Health_Heart = 0; // 心率
    Health_Blood = 0; // 血样
    Health_Tired = 0; // 疲劳度
    Health_WXH = 0;   // 微循环
}

/*****************************************************************************************************
 * 描述:
 * 参数:
 * 返回:
 * 注意:
 *****************************************************************************************************/
void Health_OutTime_Hand(void) // 100ms
{
    if (Health_State == Health_State_Check)
    {
        if (++Health_OutTime > 10 * 120) // 超时120s
        {
            Health_Set_State(Health_State_Fail);
        }
    }
    else if (Health_State > 0)
    {
        // Buzzer = SOUND_ACK; //* remove 250821
        if (++Health_OutTime > 10 * 20) // 超时退出
        {
            Health_Set_State(Health_State_Stop);
            Exit_Pause();
        }
    }
}

/*****************************************************************************************************
 * 描述:健康检测任务处理
 * 参数:nTask_ID：当前操作的任务，用于模拟的任务调度系统
 * 返回:无
 * 注意:
 *****************************************************************************************************/
void os_task_Health(u8 nTask_ID)
{
    u8 i;
    u8 nHealth_Heart; // 心率
    u8 nHealth_Blood; // 血样
    u8 nHealth_Tired; // 疲劳度
    u8 nHealth_WXH;   // 微循环
    u8 health_tired_data;

    Pt_Start

        if (Health_State == Health_State_Check)
    {

        EUSART1_Write(0x88); // 关闭命令
        Pt_Wait_Tick(10);    // 50ms
        EUSART1_Write(0x8a); // 开启命令
        Pt_Wait_Tick(10);    // 50ms
        EUSART1_Write(0x8a); // 开启命令
        Pt_Wait_Tick(10);    // 50ms
    }
    else
    {
        EUSART1_Write(0x88); // 关闭命令
        Pt_Wait_Tick(10);    // 50ms
        EUSART1_Write(0x88); // 关闭命令
        Pt_Pause();          // 退出
    }

    Health_Cnt = 0;

    while (1) // 循环接收数据
    {
        // 等待串口
#if (Health_Check_Main)
        COM1.RX_write = 0;
        COM1.RX_read = 0;
        COM1.Rx_OverTime = 0;
#endif // Health_Check_Main

#if (Health_Check_Key_OneLine)
        Simple_key_Health = 0;
#endif

        Health_OutCnt = 0;

#if (Health_Check_Main)
        while (1) // 等待接收完成
        {
            Pt_Wait_Tick(1); // 5ms

            if (COM1.RX_read != COM1.RX_write)
            {
                COM1.RX_read = COM1.RX_write;
                COM1.Rx_OverTime = 3;
            }
            else
            {
                if (COM1.Rx_OverTime)
                {
                    if (--COM1.Rx_OverTime == 0)
                        break; // 接收完成退出
                }
            }

            Health_OutCnt++;

            if (Health_OutCnt >= 500 || Health_State != Health_State_Check) // 单次超时2.5S重新开始
            {
                Pt_Reset();
            }
        }

        nHealth_Heart = RX1_Buffer[65];
        nHealth_Blood = RX1_Buffer[66];
        nHealth_Tired = RX1_Buffer[77];
        nHealth_WXH = RX1_Buffer[67];
#endif // Health_Check_Main

#if (Health_Check_Key_OneLine)
        Pt_Wait_Sig(Simple_key_Health == 0);
        Simple_key_Health = 0;

        nHealth_Heart = Simple_key_Buf[1];
        nHealth_Blood = Simple_key_Buf[2];
        nHealth_Tired = Simple_key_Buf[3];

#endif

        // 接收数据并处理
        Health_Heart_Buf[Health_Cnt] = nHealth_Heart;
        Health_Blood_Buf[Health_Cnt] = nHealth_Blood;
        Health_Tired_Buf[Health_Cnt] = nHealth_Tired;
        Health_WXH_Buf[Health_Cnt] = nHealth_WXH;

        if (nHealth_Heart > Health_Heart)
            i = nHealth_Heart - Health_Heart;
        else
            i = Health_Heart - nHealth_Heart;

        if (i < 7 && nHealth_Heart > 20 && Leg_Protect_Flag == 0) // * add Leg_Protect_Flag 250821
        {
            Health_Cnt++;
        }

        if (Health_Cnt >= Health_MaxCheckCnt) // 检测完成取平均值并 退出
        {
            Health_Heart = Health_Hand_Value(Health_Heart_Buf + 10, Health_MaxCheckCnt - 10);
            Health_Blood = Health_Hand_Value(Health_Blood_Buf + 10, Health_MaxCheckCnt - 10);
            health_tired_data = Health_Hand_Value(Health_Tired_Buf + 10, Health_MaxCheckCnt - 10);
            Health_WXH = Health_Hand_Value(Health_WXH_Buf + 10, Health_MaxCheckCnt - 10);

            if (health_tired_data > 0)
            {
                if (health_tired_data > 30) // 健康
                    Health_Tired = 0;
                else if (health_tired_data >= 25) // 轻度疲劳
                    Health_Tired = 1;
                else if (health_tired_data >= 20) // 中度疲劳
                    Health_Tired = 2;
                else // 重度疲劳
                    Health_Tired = 3;
            }

            Health_State = Health_State_Finish;
            Health_OutTime = 0;
            Pt_Pause(); // 退出
        }
        else
        {
            Health_Heart = nHealth_Heart;
            Health_Blood = nHealth_Blood;
            health_tired_data = nHealth_Tired;
            Health_WXH = nHealth_WXH;

            if (health_tired_data > 0)
            {
                if (health_tired_data > 30) // 健康
                    Health_Tired = 0;
                else if (health_tired_data >= 25) // 轻度疲劳
                    Health_Tired = 1;
                else if (health_tired_data >= 20) // 中度疲劳
                    Health_Tired = 2;
                else // 重度疲劳
                    Health_Tired = 3;
            }
        }
    }

    Pt_End
}
#endif
