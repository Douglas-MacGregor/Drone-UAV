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

void test_mpu6050_gyro_acc_config(void)
{
    int m = init_gpio();
    TEST_ASSERT_EQUAL_INT(0, m);
    iic_handle = init_i2c(1, 0x68); // Assuming
    fprintf(stderr, "Configuring MPU6050 gyro and accel full-scale ranges\n");
    int n = configure_mpu6050(iic_handle, GYRO_FS_250, ACCEL_FS_8);
    fprintf(stderr, "Configuration returned: %d\n", n);
    TEST_ASSERT_EQUAL_INT(0, n); // Expect to write 2 bytes (one for gyro config, one for accel config)
    mpu6050_Data data;
    data.address = REG_GYRO_CONFIG;
    data.length = 1;
    uint8_t gyro_config = 0;
    data.data_receive = &gyro_config;
    fprintf(stderr, "Reading back gyro config\n");
    n = read_mpu6050(iic_handle, &data);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_UINT8(0x00, gyro_config); // GYRO_FS
    data.address = REG_ACCEL_CONFIG;
    uint8_t accel_config = 0;
    data.data_receive = &accel_config;
    n = read_mpu6050(iic_handle, &data);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_UINT8(0x10, accel_config); // ACCEL_FS
    n = configure_mpu6050(iic_handle, GYRO_FS_1000, ACCEL_FS_16);
    TEST_ASSERT_EQUAL_INT(0, n);
    data.address = REG_GYRO_CONFIG;
    gyro_config = 0;
    data.data_receive = &gyro_config;
    n = read_mpu6050(iic_handle, &data);
    fprintf(stderr, "Reading back gyro config\n");
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_UINT8(0x10, gyro_config); // GYRO_FS
    data.address = REG_ACCEL_CONFIG;
    accel_config = 0;
    data.data_receive = &accel_config;
    n = read_mpu6050(iic_handle, &data);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_UINT8(0x18, accel_config); // ACCEL_FS
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
    RUN_TEST(test_mpu6050_gyro_acc_config);

    return UNITY_END();
}