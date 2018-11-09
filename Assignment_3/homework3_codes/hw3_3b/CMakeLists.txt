cmake_minimum_required(VERSION 3.8)
project(hw3_3b)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp )
add_executable(hw3_3b ${SOURCE_FILES})
target_link_libraries(hw3_3b ${OpenCV_LIBS})