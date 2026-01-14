add_library(dualshock
    dualshock_utils.c
    dualshock_device.c
)

target_include_directories(dualshock
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/devices/controller
        ${PROJECT_SOURCE_DIR}/hal
)