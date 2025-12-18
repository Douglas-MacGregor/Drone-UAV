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

void test_mpu6050_utils_read(void)
{
    iic_handle = 1; // Dummy I2C handle for testing
    mpu6050_Data data;
    data.address = REG_WHO_AM_I;
    data.length = 1;
    uint8_t received_data = 0;
    data.data_receive = &received_data;

    // Mock i2cReadI2CBlockData to return expected value
    int n = read_mpu6050(iic_handle, &data);
    TEST_ASSERT_EQUAL_INT(-1, n); // Since we are using a dummy handle, expect error
    init_gpio();
    iic_handle = init_i2c(1, 0x68); // Assuming
    n = read_mpu6050(iic_handle, &data);
    TEST_ASSERT_EQUAL_INT(1, n);                  // Expect to read 1 byte
    TEST_ASSERT_EQUAL_UINT8(0x68, received_data); // WHO_AM_I should return 0x68 for MPU6050
    close_i2c(iic_handle);
    close_gpio();
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
    RUN_TEST(test_mpu6050_utils_read);
    return UNITY_END();
}