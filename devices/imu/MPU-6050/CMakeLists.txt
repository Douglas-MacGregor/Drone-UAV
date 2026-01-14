add_library(mpu6050
    mpu6050_utils.c
    mpu6050_device.c
)

target_include_directories(mpu6050
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/devices/imu
        ${PROJECT_SOURCE_DIR}/hal
)