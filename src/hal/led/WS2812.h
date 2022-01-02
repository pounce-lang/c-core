#ifndef WS2812_H
#define WS2812_H

#include "pico/types.h"
#include "hardware/pio.h"
#include "../../pounce.h"

#ifdef __cplusplus
extern "C" {
#endif

pdq_node_ptr pf_setWS2812LED(stack_instance_ptr s, pdq_instance_ptr p);
pdq_node_ptr pf_initWS2812LED(stack_instance_ptr s, pdq_instance_ptr p);

#ifdef __cplusplus
}
#endif

#endif