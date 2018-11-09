cmake_minimum_required(VERSION 3.8)
project(ImageProcessingHW_1CMD_Noise)


set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Image.cpp Image.h Histogram.cpp Histogram.h SingleChannel.h SingleChannel.cpp)
add_executable(ImageProcessingHW_1CMD_Noise ${SOURCE_FILES} SingleChannel.cpp)
target_link_libraries(ImageProcessingHW_1CMD_Noise )

