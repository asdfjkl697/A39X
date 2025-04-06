#ifndef LED_H
#define LED_H

#include "config.h"
#include <string.h>

#define LED1_ON() gpio_bit_reset(GPIOE, GPIO_PIN_12)
#define LED1_OFF() gpio_bit_set(GPIOE, GPIO_PIN_12)

#ifdef LED2_PWM
extern u16 LED2_PWM_Data;
extern bool LED2_PWM_Flag;
extern void Led_Pwm_Update(void);
#define LDE2_Pwm_On(dyty) timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1, dyty)
#define LDE2_Pwm_Off() timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1, 0)
#else
#define LED2_ON() gpio_bit_reset(GPIOE, GPIO_PIN_10)
#define LED2_OFF() gpio_bit_set(GPIOE, GPIO_PIN_10)
#endif

#define ONE_PULSE (108)							  // 1 码
#define ZERO_PULSE (36)							  // 0 码
#define RESET_PULSE (70)						  // 80 ，复位信号
#define LED_NUMS (48)							  // led 数量
#define LED_DATA_LEN (24)						  // led 长度，单个需要24bits
#define LED_GROUPS 12							  // led组数，将所有led分组
#define WS2812_DATA_LEN (LED_NUMS * LED_DATA_LEN) // ws2812灯条需要的数组长度

typedef struct
{
	uint32_t iTick; // 定时器
	uint8_t mode;	// 模式
	uint8_t curmode;
	uint8_t paraR;	// R参数
	uint8_t paraG;	// G参数
	uint8_t paraB;	// B参数
	uint8_t times;	// 变化间隔时间
	uint8_t updata; // 更新标志

	uint8_t switchSta;	  // 切换状态
	uint8_t switchCnt;	  // 中间计数
	uint8_t switchSetCnt; // 切换参数
	uint8_t switchTimes;  // 间隔时间累加

	/* 流水灯用到的参数 */
	uint8_t groupleds; // 将所有LED分组后，每组内的LED数量
	uint8_t ledhead;   // LED灯组的头
	uint8_t ledtail;   // LED灯组的尾
} rgbLogic_t;

void MCU_LedProcess(void);
void rgbInit(void);
void rgbInitHw(void);
void Light_Params_Set(uint8_t mode, uint8_t paraR, uint8_t paraG, uint8_t paraB, uint8_t times);
void Light_Update(void);
void Light_init(uint8_t led_nums);
void Light_set_RGB(uint8_t R, uint8_t G, uint8_t B, uint16_t num);
#endif
