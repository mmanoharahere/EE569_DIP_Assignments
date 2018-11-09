cmake_minimum_required(VERSION 3.8)
project(hw3_prob2a)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Image.h Image.cpp EdgeDetector.cpp EdgeDetector.h Histogram.h Histogram.cpp)
add_executable(hw3_prob2a ${SOURCE_FILES} Image.h Image.cpp EdgeDetector.cpp EdgeDetector.h Histogram.h Histogram.cpp)
