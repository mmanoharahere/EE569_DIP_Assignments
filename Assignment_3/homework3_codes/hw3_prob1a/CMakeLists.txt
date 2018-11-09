cmake_minimum_required(VERSION 3.8)
project(hw3_prob1a)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Image.cpp Image.h Texture.cpp Texture.h)
add_executable(hw3_prob1a ${SOURCE_FILES} Image.cpp Image.h Texture.cpp Texture.h)
