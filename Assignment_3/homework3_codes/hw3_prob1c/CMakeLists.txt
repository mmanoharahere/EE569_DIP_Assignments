cmake_minimum_required(VERSION 3.8)
project(hw3_prob1c)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Texture.h Image.h Image.cpp Texture.cpp)
add_executable(hw3_prob1c ${SOURCE_FILES} Texture.h Image.h Image.cpp Texture.cpp)
target_link_libraries(hw3_prob1c ${OpenCV_LIBS})