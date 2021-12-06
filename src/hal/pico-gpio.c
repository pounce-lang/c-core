#include "pico/stdlib.h"

void init_gpio(int pin, bool dir_out, int pull_resistor) {
    gpio_init(pin);
    gpio_set_dir(pin, dir_out);
};

void get_gpio(int pin) {

};

void set_gpio(int pin, int val) {
    gpio_put(pin, val);
};