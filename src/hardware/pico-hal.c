#include <stdio>
#include "pico/stdlib.h" 
#include "hardware/i2c.h" 

#define I2C_PORT i2c0

static int addr = 0x28;


void pico_hal_init_i2c() {
    init_i2c(I2C_PORT, 400000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);
}

bool check_i2c_connection(int device_i2c_addr, int8_t read_reg, int8_t expected_reg_value) {
    sleep_ms(1000);
    int8_t reg = read_reg;
    int8t_t received_reg_value[1];
    i2c_write_blocking(I2C_PORT, device_i2c_addr, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, device_i2c_addr, received_reg_value, 1, false);
    return (received_reg_value[0] == expected_reg_value);
}

