# Simulation HAL implementation (placeholder)
add_library(sim_hal INTERFACE)

target_include_directories(sim_hal
    INTERFACE
        ${CMAKE_SOURCE_DIR}/hal
)