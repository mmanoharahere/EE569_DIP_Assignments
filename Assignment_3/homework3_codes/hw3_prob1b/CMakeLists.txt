cmake_minimum_required(VERSION 3.8)
project(hw3_prob1b)


set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Image.h Image.cpp Texture.h Texture.cpp)
add_executable(hw3_prob1b ${SOURCE_FILES} Image.h Image.cpp Texture.h Texture.cpp)
