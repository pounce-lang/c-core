cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
project(pounce_repl_project)
pico_sdk_init()

add_executable(pounce_repl
    src/repl/pico-repl.c
)

target_link_libraries(pounce_repl pico_stdlib) # tinyusb_dev

pico_enable_stdio_usb(pounce_repl 1)
pico_enable_stdio_uart(pounce_repl 0)

pico_add_extra_outputs(pounce_repl)
