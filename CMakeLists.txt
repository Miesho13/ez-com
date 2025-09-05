cmake_minimum_required(VERSION 4.0)

project(ez-com)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")

set(ez-com-src
    "main.c"
    "app.c"
    "uart_impl.c"
    "ui_platform_raylib.c"
    "ui.c"
)

add_subdirectory(raylib)

add_executable(ez-com "${ez-com-src}")

target_link_libraries(ez-com PUBLIC raylib)
