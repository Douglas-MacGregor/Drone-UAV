#include "../unity.h"
#include "../../src/devices/utils.h"
#include "../../src/devices/IMU/MPU-6050/mpu6050_device.h"
#include "../../src/devices/IMU/MPU-6050/mpu6050_utils.h"
#include <string.h>
#include <unistd.h>
#include <stdint.h>

int iic_handle;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_mpu6050_device_creation(void)
{
    iic_handle = 1; // Dummy I2C handle for testing
    mpu6050_Device device = create_mpu6050_device(iic_handle);
    TEST_ASSERT_EQUAL_INT(iic_handle, device.iic_handle);
    TEST_ASSERT_EQUAL_PTR(&mpu6050_imu_interface, device.vtable);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_mpu6050_device_creation);
    return UNITY_END();
}