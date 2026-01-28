# Raspberry Pi Pico HAL implementation
add_library(raspberry_pi_pico_hal
    hal_gpio_pico.c
    hal_i2c_pico.c
    hal_spi_pico.c
)

target_include_directories(raspberry_pi_pico_hal
    PUBLIC
        ${CMAKE_SOURCE_DIR}/hal
)

# Link with Pico SDK libraries
# pico_stdlib is required for stdio, SDK init, etc.
# hardware_gpio, hardware_i2c, hardware_spi are the HAL drivers
target_link_libraries(raspberry_pi_pico_hal
    PUBLIC
        pico_stdlib
)
