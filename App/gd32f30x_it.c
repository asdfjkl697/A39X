/*!
    \file  gd32f30x_it.c
    \brief interrupt service routines

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

#include "gd32f30x_it.h"
#include "systick.h"
#include "config.h"
#include "Motor.h"

uint32_t Timer_1ms = 0, Timer_5ms_Cnt = 0, Timer_50ms_Cnt = 0, Timer_100ms_Cnt = 0, Timer_150ms_Cnt = 0, Timer_200ms_Cnt = 0, Timer_1s_Cnt;
u8 ADC0_Finish = 0;

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    delay_decrement();
    Timer_1ms++;
    Timer_5ms_Cnt++;
    Timer_50ms_Cnt++;
    Timer_100ms_Cnt++;
    Timer_200ms_Cnt++;
}

uint32_t GetTick(void)
{

    return Timer_1ms;
}
u8 Debug_usart0_cnt = 0;
u16 COM0_overflow_count = 0;
void USART0_IRQHandler(void)
{

    if (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE))
    {

//        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
        RX0_Buffer[COM0.RX_write] = usart_data_receive(USART0);
        if (++COM0.RX_write >= COM_RX0_Lenth)
            COM0.RX_write = 0;
    }

    // if (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) //todo deepseek 无效
    // {
    //     const uint8_t next_write = (COM0.RX_write + 1) % COM_RX0_Lenth;
    //     if (next_write != COM0.RX_read)
    //     { // 缓冲区未满时写入
    //         // usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
    //         RX0_Buffer[COM0.RX_write] = usart_data_receive(USART0);
    //         COM0.RX_write = next_write;
    //     }
    //     else
    //     {
    //         // 可选：记录溢出错误或丢弃数据
    //         COM0_overflow_count++;
    //     }
    // }

    // if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_ERR_ORERR)
    //     || usart_interrupt_flag_get(USART0, USART_INT_FLAG_ERR_NERR)
    //     || usart_interrupt_flag_get(USART0, USART_INT_FLAG_ERR_FERR))
    // {
    //     Debug_usart0_cnt++;
    // }

    // usart_interrupt_flag_clear(USART0, USART_INT_FLAG_ERR_ORERR); // todo 调试 250315

    if (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TBE))
    {

        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_TBE);

        if (COM0.TX_read != COM0.TX_write)
        {
            usart_data_transmit(USART0, TX0_Buffer[COM0.TX_read]);
            if (++COM0.TX_read >= COM_TX0_Lenth)
                COM0.TX_read = 0;
        }
        else
        {
            if (COM0.B_TX_busy == 0) // todo 屏蔽 250220
            {
                // usart_data_transmit(USART0, TX0_Buffer[COM0.TX_read]);
                COM0.B_TX_busy = 1;
            }
            else
            {
                COM0.B_TX_busy = 0;
                usart_interrupt_disable(USART0, USART_INT_TBE);
            }
        }
    }
}

void USART1_IRQHandler(void)
{

    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE))
    {

        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RBNE);

        RX1_Buffer[COM1.RX_write] = usart_data_receive(USART1);
        if (++COM1.RX_write >= COM_RX1_Lenth)
            COM1.RX_write = 0;
    }

#ifdef USART1_DMA

#else

    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_TBE))
    {

        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_TBE);

        if (COM1.TX_read != COM1.TX_write)
        {
            usart_data_transmit(USART1, TX1_Buffer[COM1.TX_read]);
            if (++COM1.TX_read >= COM_TX1_Lenth)
                COM1.TX_read = 0;
        }
        else
        {

            //						if(COM1.B_TX_busy ==0){
            //
            //									usart_data_transmit(USART1, TX1_Buffer[COM1.TX_read]);
            //									COM1.B_TX_busy=1;
            //						}
            //						else{
            //
            //								COM1.B_TX_busy=0;
            usart_interrupt_disable(USART1, USART_INT_TBE);
            //						}
        }
    }

#endif
}

void USART2_IRQHandler(void)
{

    if (RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE))
    {

        usart_interrupt_flag_clear(USART2, USART_INT_FLAG_RBNE);

        RX2_Buffer[COM2.RX_write] = usart_data_receive(USART2);
        if (++COM2.RX_write >= COM_RX2_Lenth)
            COM2.RX_write = 0;
    }

#ifdef USART2_DMA

#else

    if (RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_TBE))
    {

        usart_interrupt_flag_clear(USART2, USART_INT_FLAG_TBE);

        if (COM2.TX_read != COM2.TX_write)
        {
            usart_data_transmit(USART2, TX2_Buffer[COM2.TX_read]);
            if (++COM2.TX_read >= COM_TX2_Lenth)
                COM2.TX_read = 0;
        }
        else
        {

            //						if(COM2.B_TX_busy ==0){
            //
            //									usart_data_transmit(USART2, TX2_Buffer[COM2.TX_read]);
            //									COM2.B_TX_busy=1;
            //						}
            //						else{
            //
            //								COM2.B_TX_busy=0;
            usart_interrupt_disable(USART2, USART_INT_TBE);
            //						}
        }
    }

#endif
}

void UART3_IRQHandler(void)
{

    if (RESET != usart_interrupt_flag_get(UART3, USART_INT_FLAG_RBNE))
    {

        usart_interrupt_flag_clear(UART3, USART_INT_FLAG_RBNE);

        RX3_Buffer[COM3.RX_write] = usart_data_receive(UART3);
        if (++COM3.RX_write >= COM_RX3_Lenth)
            COM3.RX_write = 0;
    }

#ifdef UART3_DMA

#else

    if (RESET != usart_interrupt_flag_get(UART3, USART_INT_FLAG_TBE))
    {

        usart_interrupt_flag_clear(UART3, USART_INT_FLAG_TBE);

        if (COM3.TX_read != COM3.TX_write)
        {
            usart_data_transmit(UART3, TX3_Buffer[COM3.TX_read]);
            if (++COM3.TX_read >= COM_TX3_Lenth)
                COM3.TX_read = 0;
        }
        else
        {

            //						if(COM3.B_TX_busy ==0){
            //
            //									usart_data_transmit(UART3, TX3_Buffer[COM3.TX_read]);
            //									COM3.B_TX_busy=1;
            //						}
            //						else{
            //
            //								COM3.B_TX_busy=0;
            usart_interrupt_disable(UART3, USART_INT_TBE);
            //						}
        }
    }

#endif
}

void UART4_IRQHandler(void)
{

    if (RESET != usart_interrupt_flag_get(UART4, USART_INT_FLAG_RBNE))
    {

        usart_interrupt_flag_clear(UART4, USART_INT_FLAG_RBNE);

        RX4_Buffer[COM4.RX_write] = usart_data_receive(UART4);
        if (++COM4.RX_write >= COM_RX4_Lenth)
            COM4.RX_write = 0;
    }
    //		usart_interrupt_flag_clear(USART0,USART_INT_FLAG_ERR_ORERR);

    if (RESET != usart_interrupt_flag_get(UART4, USART_INT_FLAG_TBE))
    {

        usart_interrupt_flag_clear(UART4, USART_INT_FLAG_TBE);

        if (COM4.TX_read != COM4.TX_write)
        {
            usart_data_transmit(UART4, TX4_Buffer[COM4.TX_read]);
            if (++COM4.TX_read >= COM_TX4_Lenth)
                COM4.TX_read = 0;
        }
        else
        {

            if (COM4.B_TX_busy == 1)
            {

                usart_data_transmit(UART4, TX4_Buffer[COM4.TX_read]);
                COM4.B_TX_busy = 0;
            }
            else
            {
                gpio_bit_reset(GPIOD, GPIO_PIN_0);
                usart_interrupt_disable(UART4, USART_INT_TBE);
            }
        }
    }
}

void DMA0_Channel3_IRQHandler(void)
{
    // TX
    if (RESET != dma_interrupt_flag_get(DMA0, DMA_CH3, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH3, DMA_INT_FLAG_FTF);
        dma_channel_disable(DMA0, DMA_CH3);
    }
    // RX
    //    if(RESET != dma_interrupt_flag_get(DMA_CH4, DMA_INT_FLAG_FTF)){
    //        dma_interrupt_flag_clear(DMA_CH4, DMA_INT_FLAG_G);
    //        //g_transfer_complete = SET;
    //    }
}

void DMA0_Channel6_IRQHandler(void)
{
    // TX
    if (RESET != dma_interrupt_flag_get(DMA0, DMA_CH6, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH6, DMA_INT_FLAG_FTF);
        dma_channel_disable(DMA0, DMA_CH6);
    }
}

void DMA0_Channel1_IRQHandler(void)
{
    // TX
    if (RESET != dma_interrupt_flag_get(DMA0, DMA_CH1, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH1, DMA_INT_FLAG_FTF);
        dma_channel_disable(DMA0, DMA_CH1);
    }
}

void DMA1_Channel3_4_IRQHandler(void)
{
    // TX
    if (RESET != dma_interrupt_flag_get(DMA1, DMA_CH4, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA1, DMA_CH4, DMA_INT_FLAG_FTF);
        dma_channel_disable(DMA1, DMA_CH4);
    }
}

void DMA0_Channel0_IRQHandler(void)
{
    // TX
    if (RESET != dma_interrupt_flag_get(DMA0, DMA_CH0, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH0, DMA_INT_FLAG_FTF);
        //		dma_channel_disable(DMA0, DMA_CH0);
        ADC0_Finish = 1;
    }
}

void EXTI5_9_IRQHandler(void)
{
    if (RESET != exti_interrupt_flag_get(EXTI_5))
    {

        exti_interrupt_flag_clear(EXTI_5);

        if (DDGVariable.LDir == DDG_MotorState_Up)
        {
            DDGVariable.LCircleOutTime = 0;
            if (DDGVariable.LnCurCircle)
                DDGVariable.LnCurCircle--;
            if (DDGVariable.LnLocalCircle > 0)
                DDGVariable.LnLocalCircle--;
            nPreLDDGCircle++;
        }
        else if (DDGVariable.LDir == DDG_MotorState_Down)
        {
            DDGVariable.LCircleOutTime = 0;
            if (DDGVariable.LSignalInit)
                DDGVariable.LnCurCircle++;
            if (DDGVariable.LnLocalCircle > 0)
                DDGVariable.LnLocalCircle--;
            nPreLDDGCircle++;
        }
    }

    if (RESET != exti_interrupt_flag_get(EXTI_6))
    {

        exti_interrupt_flag_clear(EXTI_6);
        if (DDGVariable.BDir == DDG_MotorState_Up)
        {
            DDGVariable.BCircleOutTime = 0;
            if (DDGVariable.BnCurCircle > 0)
                DDGVariable.BnCurCircle--;
            if (DDGVariable.BnLocalCircle > 0)
                DDGVariable.BnLocalCircle--;
            nPreBDDGCircle++;
        }
        else if (DDGVariable.BDir == DDG_MotorState_Down)
        {

            DDGVariable.BCircleOutTime = 0;
            if (DDGVariable.BSignalInit)
                DDGVariable.BnCurCircle++;
            if (DDGVariable.BnLocalCircle > 0)
                DDGVariable.BnLocalCircle--;
            nPreBDDGCircle++;
        }
    }

    if (RESET != exti_interrupt_flag_get(EXTI_7))
    {

        exti_interrupt_flag_clear(EXTI_7);
    }
}

#define ADC_MAXNUM 7
uint16_t AD_Get_Value[ADC_MAXNUM];
uint8_t GetValueFlag = 0;

void ADC_Process(void)
{
    uint8_t i, j;
    uint16_t AD_Temp_Value[7] = {0};
    uint16_t MaxValue, MinValue;

    if (ADC0_Finish)
    {
        ADC0_Finish = 0;
        for (i = 0; i < ADC_MAXNUM; i++)
        {

            for (j = i; j <= (i + 63); j = j + ADC_MAXNUM)
            {

                if (GetValueFlag == 0)
                {

                    MinValue = ADC0_Buffer[j];
                    MaxValue = ADC0_Buffer[j];
                    GetValueFlag = 1;
                }
                if (ADC0_Buffer[j] >= MaxValue)
                    MaxValue = ADC0_Buffer[j];
                if (ADC0_Buffer[j] <= MinValue)
                    MinValue = ADC0_Buffer[j];

                AD_Temp_Value[i] = AD_Temp_Value[i] + ADC0_Buffer[j];
            }

            AD_Temp_Value[i] = (AD_Temp_Value[i] - MaxValue - MinValue) / 8;
            GetValueFlag = 0;
        }

        for (i = 0; i < ADC_MAXNUM; i++)
        {

            AD_Get_Value[i] = AD_Temp_Value[i];
        }
    }
    //    TreadCurrent=AD_Get_Value[0]*33*1000/4095;
    //    Roll1Current=AD_Get_Value[1]*33*1000/4095;
    //    HeatCurrent=AD_Get_Value[2]*33*1000/4095;
    //    KneadCurrent=AD_Get_Value[3]*33*1000/4095;
    //    Roll2Current=AD_Get_Value[4]*33*1000/4095;
}
