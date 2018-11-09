cmake_minimum_required(VERSION 3.8)
project(hw2_prob1b)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Image.cpp
        Image.h
        main.cpp
        SingleChannel.cpp
        SingleChannel.h)

add_executable(hw2_prob1b ${SOURCE_FILES})