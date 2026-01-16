add_subdirectory(raspberry_pi)
add_subdirectory(stm32)
add_subdirectory(sim)

# Create platform_hal alias based on PLATFORM variable
if(NOT DEFINED PLATFORM)
    set(PLATFORM "raspberry_pi")  # Default for now
endif()

if(PLATFORM STREQUAL "raspberry_pi" OR PLATFORM STREQUAL "RaspberryPi")
    add_library(platform_hal ALIAS raspberry_pi_hal)
elseif(PLATFORM STREQUAL "STM32" OR PLATFORM STREQUAL "stm32")
    add_library(platform_hal ALIAS stm32_hal)
elseif(PLATFORM STREQUAL "sim")
    add_library(platform_hal ALIAS sim_hal)
else()
    add_library(platform_hal ALIAS raspberry_pi_hal)  # Default fallback
endif()