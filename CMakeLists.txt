cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(pounce_repl_project)
pico_sdk_init()

## PICO REPL 
add_executable(pounce_repl
    src/repl/pico-repl.c
    src/hal/pico-gpio.c
)
target_link_libraries(pounce_repl pico_stdlib hardware_i2c) # tinyusb_dev (hardware_flash hardware_sync)
pico_enable_stdio_usb(pounce_repl 1)
pico_enable_stdio_uart(pounce_repl 0)
pico_add_extra_outputs(pounce_repl)
# add a url via pico_set_program_url
# example_auto_set_url(pounce_cme)
pico_set_program_name(pounce_repl "pico_pounce_repl")
pico_set_program_description(pounce_repl "pico pounce repl")
# pico_set_program_version_string(pounce_repl "0.3.0")
pico_set_program_url(pounce_repl "https://googlechromelabs.github.io/serial-terminal/")
target_compile_definitions(pounce_repl PRIVATE
    POUNCE_PICO_RP2040=0  SF_PRO_MICRO_RP2040=0)


## PICO Pounce Cooperative Multitasking Environment
add_executable(pounce_cme
    src/cmp/pico-controller.c
    src/hal/pico-gpio.c
)
target_link_libraries(pounce_cme pico_stdlib hardware_i2c) # tinyusb_dev (hardware_flash hardware_sync)
pico_enable_stdio_usb(pounce_cme 1)
pico_enable_stdio_uart(pounce_cme 0)
pico_add_extra_outputs(pounce_cme)
# add a url via pico_set_program_url
# example_auto_set_url(pounce_cme)
pico_set_program_name(pounce_cme "pico_pounce_cme")
pico_set_program_description(pounce_cme "pico pounce cme")
# pico_set_program_version_string(pounce_cme "0.3.0")
pico_set_program_url(pounce_cme "https://googlechromelabs.github.io/serial-terminal/")
target_compile_definitions(pounce_cme PRIVATE
    POUNCE_PICO_RP2040=1  SF_PRO_MICRO_RP2040=0)


## SF-PRO-MICRO-RP2040 REPL 
add_executable(sf-pro-micro-rp2040_pounce_repl
    src/repl/pico-repl.c
    src/hal/pico-gpio.c
    src/hal/led/WS2812.c
)
pico_generate_pio_header(sf-pro-micro-rp2040_pounce_repl ${CMAKE_CURRENT_LIST_DIR}/src/hal/led/WS2812.pio)
target_link_libraries(sf-pro-micro-rp2040_pounce_repl 
    pico_stdlib hardware_i2c hardware_pio) # tinyusb_dev (hardware_flash hardware_sync)
pico_enable_stdio_usb(sf-pro-micro-rp2040_pounce_repl 1)
pico_enable_stdio_uart(sf-pro-micro-rp2040_pounce_repl 0)
pico_add_extra_outputs(sf-pro-micro-rp2040_pounce_repl)
# add a url via pico_set_program_url
# example_auto_set_url(pounce_cme)
pico_set_program_name(sf-pro-micro-rp2040_pounce_repl "sf-pro-micro-rp2040_pounce_repl")
pico_set_program_description(sf-pro-micro-rp2040_pounce_repl "sf-pro-micro-rp2040_pounce repl")
# pico_set_program_version_string(sf-pro-micro-rp2040_pounce_repl "0.3.0")
pico_set_program_url(sf-pro-micro-rp2040_pounce_repl "https://googlechromelabs.github.io/serial-terminal/")
target_compile_definitions(sf-pro-micro-rp2040_pounce_repl PRIVATE
    POUNCE_PICO_RP2040=0  SF_PRO_MICRO_RP2040=1)
