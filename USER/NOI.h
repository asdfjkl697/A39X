#ifndef NOI_H
#define NOI_H

#include "config.h"
#include <string.h>

#define NOI_ON() gpio_bit_reset(GPIOC, GPIO_PIN_13)
#define NOI_OFF() gpio_bit_set(GPIOC, GPIO_PIN_13)

#define FS_ON() gpio_bit_set(GPIOB, GPIO_PIN_4)
#define FS_OFF() gpio_bit_reset(GPIOB, GPIO_PIN_4)

void NOI_Process(void);

#endif
