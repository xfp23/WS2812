cmake_minimum_required(VERSION 3.13)

# 项目名（可改）
project(WS2812 LANGUAGES C)

# 自动收集头文件与源文件
file(GLOB WS2812_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

file(GLOB WS2812_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# 生成静态库
add_library(ws2812 STATIC
    ${WS2812_HEADERS}
    ${WS2812_SOURCES}
)

# 公开 include 路径给上层工程
target_include_directories(ws2812
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# C 标准（如需要可改 cmake -DCMAKE_C_STANDARD=99）
set_target_properties(ws2812 PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
)
