cmake_minimum_required(VERSION 3.8)
project(ColorToGray_HW_1_1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Image.cpp Image.h)
add_executable(ColorToGray_HW_1_1 ${SOURCE_FILES})
target_link_libraries(ColorToGray_HW_1_1 )

