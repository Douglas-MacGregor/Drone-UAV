add_subdirectory(raspberry_pi_zero)
add_subdirectory(raspberry_pi_five)
add_subdirectory(raspberry_pi_pico)
add_subdirectory(sim)

# Create platform_hal alias based on PLATFORM variable
if(NOT DEFINED PLATFORM)
    set(PLATFORM "raspberry_pi_zero")  # Default for now
endif()

if(PLATFORM STREQUAL "raspberry_pi_zero" OR PLATFORM STREQUAL "RaspberryPiZero")
    add_library(platform_hal ALIAS raspberry_pi_zero_hal)
elseif(PLATFORM STREQUAL "raspberry_pi_five" OR PLATFORM STREQUAL "RaspberryPiFive")
    add_library(platform_hal ALIAS raspberry_pi_five_hal)
elseif(PLATFORM STREQUAL "sim")
    add_library(platform_hal ALIAS sim_hal)
elseif(PLATFORM STREQUAL "raspberry_pi_pico" OR PLATFORM STREQUAL "RaspberryPiPico")
    add_library(platform_hal ALIAS raspberry_pi_pico_hal)
else()
    add_library(platform_hal ALIAS raspberry_pi_zero_hal)  # Default fallback
endif()