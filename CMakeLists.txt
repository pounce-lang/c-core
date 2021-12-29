cmake_minimum_required(VERSION 3.12)
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
example_auto_set_url(pounce_cme)

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
example_auto_set_url(pounce_cme)

#
# pico_set_program_name( "not foo")
# pico_set_program_description(foo "this is a foo")
# pico_set_program_version_string(foo "0.00001a")
# pico_set_program_url(foo "www.plinth.com/foo")
