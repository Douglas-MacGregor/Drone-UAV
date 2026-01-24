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

# Link with lgpio library
find_library(LGPIO_LIBRARY lgpio)
if(LGPIO_LIBRARY)
    target_link_libraries(raspberry_pi_five_hal PUBLIC ${LGPIO_LIBRARY})
    message(STATUS "Found lgpio library: ${LGPIO_LIBRARY}")
else()
    message(WARNING "lgpio library not found - Pi 5 HAL may not work")
endif()