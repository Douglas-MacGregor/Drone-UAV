add_library(sx1278
    sx1278_utils.c
    sx1278_device.c
    ${PROJECT_SOURCE_DIR}/older-code/utils.c
)

target_include_directories(sx1278
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/devices/lora
        ${PROJECT_SOURCE_DIR}/hal
        ${PROJECT_SOURCE_DIR}/older-code
)