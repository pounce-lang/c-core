cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
project(pounce_repl_project)
pico_sdk_init()

add_executable(pounce_repl
    src/pico-repl.c
)

target_link_libraries(pounce_repl pico_stdlib)
pico_add_extra_outputs(pounce_repl)
