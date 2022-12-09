cmake_minimum_required(VERSION 3.18.4)
project(face-recognition-demo C)

set(CMAKE_C_STANDARD 11)

add_executable(demo demo.c ../recognize.c)
target_link_libraries(demo pthread rt)
