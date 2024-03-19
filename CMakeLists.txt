cmake_minimum_required(VERSION 3.27)
project(test C)

set(CMAKE_C_STANDARD 23)

add_executable(test main.c glad.c)

target_link_libraries(test glfw m)
