#setting min version
cmake_minimum_required(VERSION 3.1.0)

#the name of the executable will be this
project(Tutorial)

find_package(OpenCV REQUIRED)
set(DEPENDENCIES realsense2 ${OpenCV_LIBS})

message(STATUS "OPENCV VERSION ${OpenCV_VERSION}")

add_executable(Tutorial tutorial.cpp cv-helper.hpp)
set_property(TARGET Tutorial PROPERTY CXX_STANDARD 11)
target_link_libraries(Tutorial ${DEPENDENCIES})