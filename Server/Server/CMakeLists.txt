cmake_minimum_required(VERSION 3.17)
project(Server C)

set(CMAKE_C_STANDARD 99)

add_executable(Server main.c cJSON.c)
target_link_libraries(Server ws2_32)