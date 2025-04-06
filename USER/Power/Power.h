#ifndef POWER_H
#define POWER_H

#include "config.h"
#include "WorkState.h"
#include<string.h>

#define _5VA_ON() gpio_bit_set(GPIOE,GPIO_PIN_5)
#define _5VA_OFF() gpio_bit_reset(GPIOE,GPIO_PIN_5)

#define _5VB_ON() gpio_bit_set(GPIOE,GPIO_PIN_4)
#define _5VB_OFF() gpio_bit_reset(GPIOE,GPIO_PIN_4)


#define _5VC_ON() gpio_bit_set(GPIOE,GPIO_PIN_3)
#define _5VC_OFF() gpio_bit_reset(GPIOE,GPIO_PIN_3)

#ifdef A390PCB
#define _24V_ON() gpio_bit_reset(GPIOD,GPIO_PIN_1)
#define _24V_OFF() gpio_bit_set(GPIOD,GPIO_PIN_1)
#else
#define _24V_ON() gpio_bit_set(GPIOD,GPIO_PIN_1)
#define _24V_OFF() gpio_bit_reset(GPIOD,GPIO_PIN_1)
#endif 


#define PowerStartTime 3000 // 开关电源24V启动时间 ms

typedef enum
{
		PowerOff = 0,
		PowerStart,
		PowerReady,
	
} PowerState_E;
		

typedef struct
{

		PowerState_E State;
		u32 Tick;

		
} PowerInfo_S;

u8 PowerStateGet(void);

void PowerInit(void);
void PowerProcess(void);
#endif 

