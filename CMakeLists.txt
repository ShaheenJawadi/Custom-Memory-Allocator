cmake_minimum_required(VERSION 3.27)
project(Custom_Memory_Allocator_ C)

set(CMAKE_C_STANDARD 99)

add_executable(Custom_Memory_Allocator_ main.c memory_allocator.c)
