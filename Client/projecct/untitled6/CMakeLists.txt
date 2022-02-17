cmake_minimum_required(VERSION 3.17)
project(untitled6 C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled6 main.c cJSON.c)
target_link_libraries(untitled6 ws2_32)