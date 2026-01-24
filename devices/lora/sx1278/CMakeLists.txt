add_library(sx1278
    sx1278_utils.c
    sx1278_device.c
)

target_include_directories(sx1278
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/devices/lora
        ${PROJECT_SOURCE_DIR}/hal
)