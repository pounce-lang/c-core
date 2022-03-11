/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "../../pounce.h"
#include "../../stack.h"

#define IS_RGBW false
#define NUM_PIXELS 1

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
// default to pin 2 if the board doesn't have a default WS2812 pin defined
#define WS2812_PIN 25
#endif

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

pdq_node_ptr pf_setWS2812LED(stack_instance_ptr s, pdq_instance_ptr p) {
    pdq_node_ptr a = stack_pop(s);
    if (a == NULL) {
        return NULL; }
    if (a->type == INT_T)
    {
        put_pixel(a->word.i);
            
    }
    pdq_free_node(a);
    return NULL;
}


pdq_node_ptr pf_initWS2812LED(stack_instance_ptr s, pdq_instance_ptr p) {
    //set_sys_clock_48();
    // stdio_init_all();
    // printf("WS2812 Smoke Test, using pin %d", WS2812_PIN);

    // todo get free sm
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    return NULL;
}