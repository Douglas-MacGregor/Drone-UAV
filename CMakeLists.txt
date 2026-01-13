cmake_minimum_required(VERSION 3.22)
project(DroneUAV C)

add_subdirectory(devices)
add_subdirectory(hal)
add_subdirectory(platforms)
add_subdirectory(apps)
add_subdirectory(tests)