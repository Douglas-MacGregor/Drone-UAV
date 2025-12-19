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

void test_mpu6050_gyro_acc_getters_converter(void)
{
    init_gpio();
    iic_handle = init_i2c(1, 0x68); // Assuming
    configure_mpu6050(iic_handle, GYRO_FS_250, ACCEL_FS_8);
    int16_t raw_gyro_x, raw_gyro_y, raw_gyro_z;
    int n = get_accelX_mpu6050(iic_handle, &raw_gyro_x);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NOT_EQUAL(0, raw_gyro_x); // Expect some non-zero value
    n = get_accelY_mpu6050(iic_handle, &raw_gyro_y);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NOT_EQUAL(0, raw_gyro_y); // Expect some non-zero value
    n = get_accelZ_mpu6050(iic_handle, &raw_gyro_z);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NOT_EQUAL(0, raw_gyro_z); // Expect some non-zero value
    float g_x, g_y, g_z;
    n = convert_accel_to_g(raw_gyro_x, ACCEL_FS_8, &g_x);
    TEST_ASSERT_EQUAL_INT(0, n);
    n = convert_accel_to_g(raw_gyro_y, ACCEL_FS_8, &g_y);
    TEST_ASSERT_EQUAL_INT(0, n);
    n = convert_accel_to_g(raw_gyro_z, ACCEL_FS_8, &g_z);
    TEST_ASSERT_EQUAL_INT(0, n);
    float expected_g_x = (float)raw_gyro_x / 131.0;
    float acctual_g_x;
    convert_gyro_to_dps(raw_gyro_x, GYRO_FS_250, &acctual_g_x);
    TEST_ASSERT_FLOAT_WITHIN(0.1, expected_g_x, acctual_g_x);
    float expected_g_y = (float)raw_gyro_y / 131.0;
    float acctual_g_y;
    convert_gyro_to_dps(raw_gyro_y, GYRO_FS_250, &acctual_g_y);
    TEST_ASSERT_FLOAT_WITHIN(0.1, expected_g_y, acctual_g_y);
    float expected_g_z = (float)raw_gyro_z / 131.0;
    float acctual_g_z;
    convert_gyro_to_dps(raw_gyro_z, GYRO_FS_250, &acctual_g_z);
    TEST_ASSERT_FLOAT_WITHIN(0.1, expected_g_z, acctual_g_z);
    float expected_accel_x = (float)raw_gyro_x / 4096.0;
    float actual_accel_x;
    convert_accel_to_g(raw_gyro_x, ACCEL_FS_8, &actual_accel_x);
    TEST_ASSERT_FLOAT_WITHIN(0.1, expected_accel_x, g_x);
    float expected_accel_y = (float)raw_gyro_y / 4096.0;
    float actual_accel_y;
    convert_accel_to_g(raw_gyro_y, ACCEL_FS_8, &actual_accel_y);
    TEST_ASSERT_FLOAT_WITHIN(0.1, expected_accel_y, g_y);
    float expected_accel_z = (float)raw_gyro_z / 4096.0;
    float actual_accel_z;
    convert_accel_to_g(raw_gyro_z, ACCEL_FS_8, &actual_accel_z);
    TEST_ASSERT_FLOAT_WITHIN(0.1, expected_accel_z, g_z);

    close_i2c(iic_handle);
    close_gpio();
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_mpu6050_device_creation);
    RUN_TEST(test_mpu6050_utils_read);
    RUN_TEST(test_mpu6050_gyro_acc_config);
    return UNITY_END();
}