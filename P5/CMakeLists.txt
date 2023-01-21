cmake_minimum_required(VERSION 3.23.2)
project(P5_Multitasking)

set(CMAKE_CXX_STANDARD 20)

add_executable(P5_A1_Multitasking aufgabe1.cpp)
add_executable(P5_A2_Multitasking aufgabe2.cpp)
add_executable(P5_A3_Multitasking
            aufgabe3.cpp
            Semaphore.h
            Semaphore.cpp)
