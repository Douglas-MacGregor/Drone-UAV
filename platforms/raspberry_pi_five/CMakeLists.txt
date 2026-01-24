# Raspberry Pi HAL implementation
add_library(raspberry_pi_five_hal
    hal_gpio_lgpio.c
    hal_i2c_lgpio.c
    hal_spi_lgpio.c
)

target_include_directories(raspberry_pi_five_hal
    PUBLIC
        ${CMAKE_SOURCE_DIR}/hal
)

# Link with pigpio if available
find_library(PIGPIO_LIBRARY lgpio)
if(PIGPIO_LIBRARY)
    target_link_libraries(raspberry_pi_five_hal PUBLIC ${PIGPIO_LIBRARY})
endif()