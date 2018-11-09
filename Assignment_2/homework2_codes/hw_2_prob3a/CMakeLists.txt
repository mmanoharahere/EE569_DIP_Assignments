cmake_minimum_required(VERSION 3.8)
project(hw_2_prob3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Image.cpp
        Image.h
        main.cpp
        Morph.cpp
        Morph.h)

add_executable(hw_2_prob3 ${SOURCE_FILES})