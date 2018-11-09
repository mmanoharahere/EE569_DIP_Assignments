cmake_minimum_required(VERSION 3.8)
project(hw3_prob3c)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp )
add_executable(hw3_prob3c ${SOURCE_FILES})
target_link_libraries(hw3_prob3c ${OpenCV_LIBS})