
#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

#include "pico/stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

bool get_bootsel_button();

void init_gpio(int pin, bool dir_out, int pull_resistor);

bool get_gpio(int pin);

void set_gpio(int pin, int val);

#ifdef __cplusplus
}
#endif

#endif