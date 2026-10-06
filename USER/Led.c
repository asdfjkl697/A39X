#include "Led.h"
#include "Heat.h"
#include "Power.h"

static uint8_t MCU_Step;
static uint8_t MCU_Tick;

#define PROCESS_TICK 50 // 50ms一次
static u8 Light_R[LED_NUMS] = {0};
static u8 Light_G[LED_NUMS] = {0};
static u8 Light_B[LED_NUMS] = {0};

static uint16_t RGB_buffur[RESET_PULSE + WS2812_DATA_LEN] = {0};

rgbLogic_t rgbLogic;

#ifdef LED2_PWM
// #define SYS_CLK 120000000                           // 系统时钟频率 (120MHz)
// #define LED_PWM_FREQ 100000                         // PWM频率 (100kHz)
// #define LED_PWM_PERIOD (SYS_CLK / LED_PWM_FREQ - 1) // ARR 自动重载值
u16 LED2_PWM_Data = 0;
bool LED2_PWM_updw = 0;
bool LED2_PWM_Flag = 0;
void LED_PWM_Config(void)
{
    /* TIMER7 configuration: generate PWM signals with different duty cycles:
       TIMER0CLK = SystemCoreClock / 120 = 1MHz */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER0);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_GPIOE);
    timer_deinit(TIMER0);

    gpio_pin_remap_config(GPIO_TIMER0_FULL_REMAP, (ControlStatus)ENABLE);
    gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    // /* TIMER0 configuration */
    // timer_initpara.prescaler = 0;                       // 119->0
    // timer_initpara.alignedmode = TIMER_COUNTER_EDGE;    // 对齐
    // timer_initpara.counterdirection = TIMER_COUNTER_UP; // 向上计数
    // timer_initpara.period = LED_PWM_PERIOD;             // 99->119
    // timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    // timer_initpara.repetitioncounter = 0;
    // timer_init(TIMER0, &timer_initpara);

    /* TIMER0 configuration */
    timer_initpara.prescaler = 59;                      // 119->0
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;    // 对齐
    timer_initpara.counterdirection = TIMER_COUNTER_UP; // 向上计数
    timer_initpara.period = 199;                        // 99->119
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0, &timer_initpara);

    /* CH2 configuration in PWM mode */
    // timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    // timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE; // * 修改
    // timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    // timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_LOW;
    // timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    // timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    // timer_channel_output_config(TIMER0, TIMER_CH_2, &timer_ocintpara);

    timer_ocintpara.outputstate = TIMER_CCX_DISABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER0, TIMER_CH_1, &timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1, 0);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_1, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

    timer_primary_output_config(TIMER0, (ControlStatus)ENABLE);
    // timer_deadtime_config(TIMER0, 0x4F);  // 0x4F对应约5us死区时间（根据硬件调整） //* 测试

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);
    timer_enable(TIMER0);
}

void Led_Pwm_Update(void)
{
    if (LED2_PWM_Flag == 0)
    {
        if (LED2_PWM_Data > 0)
        {
            LED2_PWM_Data = 0;
            LDE2_Pwm_On(0); // * 调试
        }
        return;
    }

    if (LED2_PWM_updw)
    {
        if (LED2_PWM_Data > 160)
        {
            LED2_PWM_Data = 160;
            LED2_PWM_updw = 0;
        }
        else if (LED2_PWM_Data > 80)
            LED2_PWM_Data += 8;
        else if (LED2_PWM_Data > 40)
            LED2_PWM_Data += 4;
        else if (LED2_PWM_Data > 20)
            LED2_PWM_Data += 2;
        else
            LED2_PWM_Data += 1;
    }
    else
    {
        if (LED2_PWM_Data < 20)
        {
            LED2_PWM_Data = 20;
            LED2_PWM_updw = 1;
        }
        else if (LED2_PWM_Data > 80)
            LED2_PWM_Data -= 8;
        else if (LED2_PWM_Data > 40)
            LED2_PWM_Data -= 4;
        else if (LED2_PWM_Data > 20)
            LED2_PWM_Data -= 2;
        else
            LED2_PWM_Data -= 1;
    }
    LDE2_Pwm_On(LED2_PWM_Data); // * 调试
}

#endif

/*****************************************************************************************************
 * 描述：MCU指示红灯控制
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
void MCU_LedProcess(void)
{

    switch (MCU_Step)
    {
    default:
        break;

    case 0:
        gpio_bit_set(GPIOE, GPIO_PIN_6);
        TICK_RESET(MCU_Tick);
        MCU_Step++;
        break;

    case 1:
        if (TICK_COUNTER(MCU_Tick, 1000))
        {
            if (nLowPower || (Power.State != PowerOff)) // todo 低功耗 260702
                gpio_bit_reset(GPIOE, GPIO_PIN_6);
            TICK_RESET(MCU_Tick);
            MCU_Step++;
        }
        break;

    case 2:
        if (TICK_COUNTER(MCU_Tick, 1000))
        {
            MCU_Step = 0;
        }
        break;
    }
}

/**初始化接口***/
void rgbInit(void)
{
#ifdef LED2_PWM
    LED_PWM_Config();
#endif

    rgbInitHw();
    memset(&rgbLogic, 0, sizeof(rgbLogic));
    TICK_RESET(rgbLogic.iTick);
    Light_init(LED_NUMS);
    Light_Params_Set(2, 0xff, 0x33, 0x99, 1);
    rgbLogic.groupleds = LED_NUMS / LED_GROUPS;
}

/**配置IO口、PWM、DMA*/
void rgbInitHw(void)
{
    dma_parameter_struct dma_init_struct;
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);

    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA0);
    dma_deinit(DMA0, DMA_CH6);
    dma_init_struct.periph_addr = (uint32_t)(&TIMER_CH0CV(TIMER3));
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_addr = (uint32_t)RGB_buffur;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.number = RESET_PULSE + WS2812_DATA_LEN;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH6, &dma_init_struct);
    dma_circulation_disable(DMA0, DMA_CH6);
    dma_memory_to_memory_disable(DMA0, DMA_CH6);

    //    dma_channel_enable(DMA0,DMA_CH6);

    rcu_periph_clock_enable(RCU_TIMER3);
    timer_deinit(TIMER3);

    timer_initpara.prescaler = 0;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;    // 对齐
    timer_initpara.counterdirection = TIMER_COUNTER_UP; // 向上计数
    timer_initpara.period = 144;                        // 833kHz相当于1200ns
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER3, &timer_initpara);

    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER3, TIMER_CH_0, &timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_0, 0);
    timer_channel_output_mode_config(TIMER3, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER3, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* TIMER0 primary output enable */
    timer_primary_output_config(TIMER3, (ControlStatus)ENABLE);
    /* TIMER0 update DMA request enable */
    timer_dma_enable(TIMER3, TIMER_DMA_UPD);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER3);
    /* TIMER0 counter enable */
    //    timer_enable(TIMER3);
    //		nvic_irq_enable(TIMER1_IRQn,0,1); //中断使能
    //  	timer_interrupt_enable(TIMER1,TIMER_INT_UP);
}

/**
mode :0关闭，1常亮，2单色呼吸，3固定彩色呼吸，4动态彩色流动
para： R G B配比

times：常亮：无效；单色呼吸：（PROCESS_TICK单位）；固定彩色呼吸：（PROCESS_TICK单位）；动态彩色流动：（PROCESS_TICK单位）

控制口移到CHEAK4
*/
void Light_Params_Set(uint8_t mode, uint8_t paraR, uint8_t paraG, uint8_t paraB, uint8_t times)
{
    rgbLogic.mode = mode;
    rgbLogic.paraR = paraR;
    rgbLogic.paraG = paraG;
    rgbLogic.paraB = paraB;
    rgbLogic.times = times;
    rgbLogic.switchTimes = 0;
    rgbLogic.updata = 1;
}

/***主函数循环调用**/
void Light_Update(void)
{
    uint8_t i;

    if (TICK_COUNTER(rgbLogic.iTick, PROCESS_TICK))
    {
        TICK_RESET(rgbLogic.iTick);

        if (rgbLogic.curmode != rgbLogic.mode)
        {
            rgbLogic.curmode = rgbLogic.mode;
            rgbLogic.ledhead = 0;
            rgbLogic.ledtail = 0;
        }

        switch (rgbLogic.curmode)
        {
        default:
            break;

        case 0: // 关闭
            memset(Light_R, 0, LED_NUMS);
            memset(Light_G, 0, LED_NUMS);
            memset(Light_B, 0, LED_NUMS);
            break;

        case 1: // 常亮
            memset(Light_R, rgbLogic.paraR, LED_NUMS);
            memset(Light_G, rgbLogic.paraG, LED_NUMS);
            memset(Light_B, rgbLogic.paraB, LED_NUMS);
            break;

        case 2:                                         // 单色呼吸
            if (rgbLogic.switchTimes >= rgbLogic.times) //
            {
                rgbLogic.switchTimes = 0;
                rgbLogic.updata = 1;
                if (rgbLogic.switchCnt > 20 && rgbLogic.switchCnt < 80) // 平滑切换感
                    rgbLogic.switchSetCnt = 4;
                else
                    rgbLogic.switchSetCnt = 1;

                // 模拟波浪式加减
                if (!rgbLogic.switchSta)
                {
                    if (rgbLogic.switchCnt < 100)
                        rgbLogic.switchCnt += rgbLogic.switchSetCnt;
                    else
                        rgbLogic.switchSta = 1;
                }
                else
                {
                    if (rgbLogic.switchCnt > 1)
                        rgbLogic.switchCnt -= rgbLogic.switchSetCnt;
                    else
                        rgbLogic.switchSta = 0;
                }

                memset(Light_R, rgbLogic.paraR * rgbLogic.switchCnt / 100, LED_NUMS);
                memset(Light_G, rgbLogic.paraG * rgbLogic.switchCnt / 100, LED_NUMS);
                memset(Light_B, rgbLogic.paraB * rgbLogic.switchCnt / 100, LED_NUMS);
            }
            else
            {
                rgbLogic.switchTimes++;
            }

            break;

        case 3: // 固定彩色呼吸
            /***

            */
            break;

        case 4: // 动态彩色流动
            if (rgbLogic.switchTimes >= rgbLogic.times)
            {
                rgbLogic.switchTimes = 0;
                rgbLogic.updata = 1;

                if (rgbLogic.ledhead < rgbLogic.groupleds)
                {
                    rgbLogic.ledtail = 0;
                }
                else
                {
                    rgbLogic.ledtail++;
                }

                for (i = 0; i < rgbLogic.ledtail; i++) /* 复位需要点亮的灯段前的LED */
                {
                    Light_R[i] = 0;
                    Light_G[i] = 0;
                    Light_B[i] = 0;
                }

                for (i = rgbLogic.ledtail; i <= rgbLogic.ledhead; i++) /* 置为需要点亮的灯段 */
                {
                    Light_R[i] = 200;
                    Light_G[i] = 200;
                    Light_B[i] = 200;
                }

                for (i = (rgbLogic.ledhead + 1); i < LED_NUMS; i++) /* 复位需要点亮的灯段后的LED */
                {
                    Light_R[i] = 0;
                    Light_G[i] = 0;
                    Light_B[i] = 0;
                }

                if (rgbLogic.ledhead < (LED_NUMS - 1)) /* LED标号为0-47，如果取到48，会赋值第一个LED造成现象错误 */
                {
                    rgbLogic.ledhead++;
                }
                else if (rgbLogic.ledtail == rgbLogic.ledhead)
                {
                    rgbLogic.ledhead = 0;
                    rgbLogic.ledtail = 0;
                }
            }
            else
            {
                rgbLogic.switchTimes++;
            }
            break;
        }

        if (rgbLogic.updata == 1)
        {
            for (i = 0; i < LED_NUMS; i++)
            {
                Light_set_RGB(Light_R[i], Light_G[i], Light_B[i], i);
            }
            //__disable_irq();
            dma_transfer_number_config(DMA0, DMA_CH6, RESET_PULSE + WS2812_DATA_LEN);
            dma_channel_enable(DMA0, DMA_CH6);
            timer_enable(TIMER3);

            //
            while (dma_flag_get(DMA0, DMA_CH6, DMA_INTF_FTFIF) == RESET)
                ;

            timer_disable(TIMER3);
            dma_flag_clear(DMA0, DMA_CH6, DMA_INTC_FTFIFC);
            dma_channel_disable(DMA0, DMA_CH6);

            //__enable_irq();
            rgbLogic.updata = 0;
        }
    }
}

/* 初始化rgb状态*/
void Light_init(uint8_t led_nums)
{
    uint8_t i = 0;

    for (i = 0; i < led_nums; i++)
    {
        Light_set_RGB(0x00, 0x00, 0x00, i);
    }
}

/* 设置单个rgb参数*/
void Light_set_RGB(uint8_t R, uint8_t G, uint8_t B, uint16_t num)
{
    uint16_t i = 0;
    uint16_t *p = RGB_buffur + (num * LED_DATA_LEN);

    if (num > LED_NUMS)
        return;

    //    uint16_t *p = (RGB_buffur + RESET_PULSE) + (num * LED_DATA_LEN);

    for (i = 0; i < 8; i++)
    {
        //
        p[i] = (R << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
        p[i + 8] = (G << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
        p[i + 16] = (B << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
    }
}
