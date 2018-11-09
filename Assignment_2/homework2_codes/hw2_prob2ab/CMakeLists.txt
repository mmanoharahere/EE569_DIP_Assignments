cmake_minimum_required(VERSION 3.8)
project(hw2_prob2a)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Dithering.cpp
        Dithering.h
        Image.cpp
        Image.h
        main.cpp)

add_executable(hw2_prob2a ${SOURCE_FILES})