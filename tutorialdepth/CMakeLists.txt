#setting min version
cmake_minimum_required(VERSION 3.1.0)

#the name of the executable will be this
project(TutorialDepth)

find_package(OpenCV REQUIRED)
set(DEPENDENCIES realsense2 ${OpenCV_LIBS})

message(STATUS "OPENCV VERSION ${OpenCV_VERSION}")

add_executable(TutorialDepth tutorialdepth.cpp cv-helper.hpp)
set_property(TARGET TutorialDepth PROPERTY CXX_STANDARD 11)
target_link_libraries(TutorialDepth ${DEPENDENCIES})