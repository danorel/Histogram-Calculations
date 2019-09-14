cmake_minimum_required(VERSION 3.9)
project(Histogram-Calculations)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(
        Histogram-Calculations main.cpp
        hands_methods.cpp
)

# linking
target_link_libraries(Histogram-Calculations ${OpenCV_LIBS})