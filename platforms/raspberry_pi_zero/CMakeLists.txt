# Raspberry Pi HAL implementation
add_library(raspberry_pi_zero_hal
    hal_gpio_pigpio.c
    hal_i2c_pigpio.c
    hal_spi_pigpio.c
    hal_time_zero.c
    hal_uart_pigpio.c
)

target_include_directories(raspberry_pi_zero_hal
    PUBLIC
        ${CMAKE_SOURCE_DIR}/hal
)

# Link with pigpio if available
find_library(PIGPIO_LIBRARY pigpio)
if(PIGPIO_LIBRARY)
    target_link_libraries(raspberry_pi_zero_hal PUBLIC ${PIGPIO_LIBRARY})
endif()