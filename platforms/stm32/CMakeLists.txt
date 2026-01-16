# STM32 HAL implementation (placeholder)
add_library(stm32_hal INTERFACE)

target_include_directories(stm32_hal
    INTERFACE
        ${CMAKE_SOURCE_DIR}/hal
)