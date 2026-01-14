add_library(unity
    ${CMAKE_SOURCE_DIR}/tests/unity.c
)

target_include_directories(unity
    PUBLIC
        ${CMAKE_SOURCE_DIR}/tests
)

add_executable(test_mpu6050
    unittest_mpu6050.c
)

target_include_directories(test_mpu6050
    PRIVATE
        ${PROJECT_SOURCE_DIR}/devices/imu/MPU-6050
        ${PROJECT_SOURCE_DIR}/older-code
)

target_link_libraries(test_mpu6050
    PRIVATE
        unity
        imu
        platform_hal
)

add_executable(test_dualshock
    unittest_dualshock.c
)

target_include_directories(test_dualshock
    PRIVATE
        ${PROJECT_SOURCE_DIR}/devices/controller/DualShock
)

target_link_libraries(test_dualshock
    PRIVATE
        unity
        controller
        platform_hal
)

add_executable(test_sx1278
    unittest_sx1278.c
)

target_include_directories(test_sx1278
    PRIVATE
        ${PROJECT_SOURCE_DIR}/devices/lora/sx1278
        ${PROJECT_SOURCE_DIR}/older-code
)

target_link_libraries(test_sx1278
    PRIVATE
        unity
        lora
        platform_hal
)