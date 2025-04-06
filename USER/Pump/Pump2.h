#ifndef PUMP2_H
#define PUMP2_H
#include "config.h"
#include "WorkState.h"

#define PUMP2_DECAY_HIGH gpio_bit_set(GPIOB, GPIO_PIN_13)
#define PUMP2_DECAY_LOW gpio_bit_reset(GPIOB, GPIO_PIN_13)

#define PUMP2_PHASE_HIGH gpio_bit_set(GPIOB, GPIO_PIN_12)
#define PUMP2_PHASE_LOW gpio_bit_reset(GPIOB, GPIO_PIN_12)

#define PUMP2_RESET_HIGH gpio_bit_set(GPIOB, GPIO_PIN_15)
#define PUMP2_RESET_LOW gpio_bit_reset(GPIOB, GPIO_PIN_15)

#define PUMP2_PWM_Enable(x) timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_2, x); // tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_4, x)
#define PUMP2_PWM_Disable() timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_2, 0); // tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_4, 0)

// #define

#endif
