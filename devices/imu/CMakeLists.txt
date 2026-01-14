add_library(imu INTERFACE)

target_include_directories(imu
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(imu
    INTERFACE
        mpu6050
)