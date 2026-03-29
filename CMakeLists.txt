cmake_minimum_required(VERSION 3.22)

# Only fetch and init Pico SDK if building for Pico
if(DEFINED PLATFORM AND (PLATFORM STREQUAL "raspberry_pi_pico" OR PLATFORM STREQUAL "RaspberryPiPico"))
    # Ensure CMAKE_TOOLCHAIN_FILE is absolute path for Pico SDK (only if it's defined and not empty)
    if(CMAKE_TOOLCHAIN_FILE AND NOT IS_ABSOLUTE "${CMAKE_TOOLCHAIN_FILE}")
        get_filename_component(CMAKE_TOOLCHAIN_FILE "${CMAKE_TOOLCHAIN_FILE}" ABSOLUTE)
    endif()
    
    include(FetchContent)

    FetchContent_Declare(
        pico_sdk
        GIT_REPOSITORY https://github.com/raspberrypi/pico-sdk.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(pico_sdk)

    # Initialize Pico SDK BEFORE project()
    pico_sdk_init()
endif()

# Now define project AFTER Pico SDK init
project(DroneUAV C CXX ASM)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(devices)
add_subdirectory(hal)
add_subdirectory(platforms)
add_subdirectory(apps)
add_subdirectory(tests)
