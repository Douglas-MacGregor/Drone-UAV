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
    cordirnate3D_t accel_bias[3];
    cordirnate3D_t gyro_bias[3];
    int m = init_gpio();
    TEST_ASSERT_EQUAL_INT(0, m);
    iic_handle = init_i2c(1, 0x68); // Assuming
    int n = configure_mpu6050(iic_handle, GYRO_FS_250, ACCEL_FS_8, &gyro_bias, &accel_bias);
    int n = configure_mpu6050(iic_handle, GYRO_FS_250, ACCEL_FS_8, &gyro_bias, &accel_bias);
    TEST_ASSERT_EQUAL_INT(0, n); // Expect to write 2 bytes (one for gyro config, one for accel config)
    mpu6050_Data data;
    data.address = REG_GYRO_CONFIG;
    data.length = 1;
    uint8_t gyro_config = 0;
    data.data_receive = &gyro_config;
    n = read_mpu6050(iic_handle, &data);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_UINT8(0x00, gyro_config); // GYRO_FS
    data.address = REG_ACCEL_CONFIG;
    uint8_t accel_config = 0;
    data.data_receive = &accel_config;
    n = read_mpu6050(iic_handle, &data);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_UINT8(0x10, accel_config); // ACCEL_FS
    n = configure_mpu6050(iic_handle, GYRO_FS_1000, ACCEL_FS_16, &gyro_bias, &accel_bias);
    n = configure_mpu6050(iic_handle, GYRO_FS_1000, ACCEL_FS_16, &gyro_bias, &accel_bias);
    TEST_ASSERT_EQUAL_INT(0, n);
    data.address = REG_GYRO_CONFIG;
    gyro_config = 0;
    data.data_receive = &gyro_config;
    n = read_mpu6050(iic_handle, &data);
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
    mpu6050_Device device = create_mpu6050_device(iic_handle, GYRO_FS_250, ACCEL_FS_8);
    mpu6050_Device device = create_mpu6050_device(iic_handle, GYRO_FS_250, ACCEL_FS_8);
    TEST_ASSERT_EQUAL_INT(iic_handle, device.iic_handle);
    TEST_ASSERT_EQUAL_PTR(&mpu6050_imu_interface, device.vtable);
}

void test_mpu6050_gyro_getters_converter(void) void test_mpu6050_gyro_getters_converter(void)
{
    init_gpio();
    iic_handle = init_i2c(1, 0x68); // Assuming
    cordirnate3D_t gyro_bias = {0.0f, 0.0f, 0.0f};
    cordirnate3D_t accel_bias = {0.0f, 0.0f, 0.0f};
    configure_mpu6050(iic_handle, GYRO_FS_250, ACCEL_FS_8, &gyro_bias, &accel_bias);
    int16_t raw_gyro_x, raw_gyro_y, raw_gyro_z;
    int n = get_gyroX_mpu6050(iic_handle, &raw_gyro_x);
    int n = get_gyroX_mpu6050(iic_handle, &raw_gyro_x);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NOT_EQUAL(0, raw_gyro_x); // Expect some non-zero value
    n = get_gyroY_mpu6050(iic_handle, &raw_gyro_y);
    n = get_gyroY_mpu6050(iic_handle, &raw_gyro_y);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NOT_EQUAL(0, raw_gyro_y); // Expect some non-zero value
    n = get_gyroZ_mpu6050(iic_handle, &raw_gyro_z);
    n = get_gyroZ_mpu6050(iic_handle, &raw_gyro_z);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NOT_EQUAL(0, raw_gyro_z); // Expect some non-zero value
    float g_x, g_y, g_z;
    n = convert_gyro_to_dps(raw_gyro_x, GYRO_FS_250, &g_x, 0.0);
    n = convert_gyro_to_dps(raw_gyro_x, GYRO_FS_250, &g_x, 0.0);
    TEST_ASSERT_EQUAL_INT(0, n);
    n = convert_gyro_to_dps(raw_gyro_y, GYRO_FS_250, &g_y, 0.0);
    n = convert_gyro_to_dps(raw_gyro_y, GYRO_FS_250, &g_y, 0.0);
    TEST_ASSERT_EQUAL_INT(0, n);
    n = convert_gyro_to_dps(raw_gyro_z, GYRO_FS_250, &g_z, 0.0);
    n = convert_gyro_to_dps(raw_gyro_z, GYRO_FS_250, &g_z, 0.0);
    TEST_ASSERT_EQUAL_INT(0, n);
    float expected_g_x = (float)raw_gyro_x / 131.0;
    float acctual_g_x = g_x;
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_g_x, acctual_g_x);
    float acctual_g_x = g_x;
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_g_x, acctual_g_x);
    float expected_g_y = (float)raw_gyro_y / 131.0;
    float acctual_g_y = g_y;
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_g_y, acctual_g_y);
    float acctual_g_y = g_y;
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_g_y, acctual_g_y);
    float expected_g_z = (float)raw_gyro_z / 131.0;
    float acctual_g_z = g_z;
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_g_z, acctual_g_z);
    close_i2c(iic_handle);
    close_gpio();
}

void test_mpu6050_acc_getters_converters(void)
{
    init_gpio();
    iic_handle = init_i2c(1, 0x68); // Assuming
    cordirnate3D_t gyro_bias = {0.0f, 0.0f, 0.0f};
    cordirnate3D_t accel_bias = {0.0f, 0.0f, 0.0f};
    configure_mpu6050(iic_handle, GYRO_FS_250, ACCEL_FS_8, &gyro_bias, &accel_bias);
    int16_t raw_accel_x, raw_accel_y, raw_accel_z;
    int n = get_accelX_mpu6050(iic_handle, &raw_accel_x);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NOT_EQUAL(0, raw_accel_x); // Expect some non-zero value
    n = get_accelY_mpu6050(iic_handle, &raw_accel_y);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NOT_EQUAL(0, raw_accel_y); // Expect some non-zero value
    n = get_accelZ_mpu6050(iic_handle, &raw_accel_z);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NOT_EQUAL(0, raw_accel_z); // Expect some non-zero value
    float a_x, a_y, a_z;
    n = convert_accel_to_g(raw_accel_x, ACCEL_FS_8, &a_x, 0.0);
    TEST_ASSERT_EQUAL_INT(0, n);
    n = convert_accel_to_g(raw_accel_y, ACCEL_FS_8, &a_y, 0.0);
    TEST_ASSERT_EQUAL_INT(0, n);
    n = convert_accel_to_g(raw_accel_z, ACCEL_FS_8, &a_z, 0.0);
    TEST_ASSERT_EQUAL_INT(0, n);
    float expected_a_x = (float)raw_accel_x / 4096.0;
    float acctual_a_x = a_x;
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_a_x, acctual_a_x);
    float expected_a_y = (float)raw_accel_y / 4096.0;
    float acctual_a_y = a_y;
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_a_y, acctual_a_y);
    float expected_a_z = (float)raw_accel_z / 4096.0;
    float acctual_a_z = a_z;
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_a_z, acctual_a_z);
    close_i2c(iic_handle);
    close_gpio();
}

void test_mpu6050_self_test(void)
{
    init_gpio();
    iic_handle = init_i2c(1, 0x68); // Assuming
    mpu6050_Device device = create_mpu6050_device(iic_handle, GYRO_FS_250, ACCEL_FS_8);
    device.vtable->wake(&device);
    int n = device.vtable->self_test(&device);
    TEST_ASSERT_EQUAL_INT(0, n); // Expect self-test to pass
    close_i2c(iic_handle);
    close_gpio();
}

void test_mpu6050_reset_sleep_wake(void)
{
    init_gpio();
    iic_handle = init_i2c(1, 0x68); // Assuming
    mpu6050_Device device = create_mpu6050_device(iic_handle, GYRO_FS_250, ACCEL_FS_8);
    int n = device.vtable->reset(&device);
    TEST_ASSERT_EQUAL_INT(0, n); // Expect reset to succeed
    n = device.vtable->sleep(&device);
    TEST_ASSERT_EQUAL_INT(0, n); // Expect sleep to succeed
    n = device.vtable->wake(&device);
    TEST_ASSERT_EQUAL_INT(0, n); // Expect wake to succeed
    close_i2c(iic_handle);
    close_gpio();
}

void test_mpu6050_device_get_data(void)
{
    init_gpio();
    iic_handle = init_i2c(1, 0x68); // Assuming
    mpu6050_Device device = create_mpu6050_device(iic_handle, GYRO_FS_250, ACCEL_FS_8);
    device.vtable->wake(&device);
    IMUData imu_data;
    int n = device.vtable->read_data(&device, &imu_data);
    TEST_ASSERT_EQUAL_INT(0, n); // Expect read data to succeed
    TEST_ASSERT_NOT_EQUAL(0.0f, imu_data.gyroX);
    TEST_ASSERT_NOT_EQUAL(0.0f, imu_data.gyroY);
    TEST_ASSERT_NOT_EQUAL(0.0f, imu_data.gyroZ);
    TEST_ASSERT_NOT_EQUAL(0.0f, imu_data.accelX);
    TEST_ASSERT_NOT_EQUAL(0.0f, imu_data.accelY);
    TEST_ASSERT_NOT_EQUAL(0.0f, imu_data.accelZ);
    close_i2c(iic_handle);
    close_gpio();
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_mpu6050_device_creation);
    RUN_TEST(test_mpu6050_utils_read);
    RUN_TEST(test_mpu6050_gyro_acc_config);
    RUN_TEST(test_mpu6050_gyro_getters_converter);
    RUN_TEST(test_mpu6050_self_test);
    RUN_TEST(test_mpu6050_reset_sleep_wake);
    RUN_TEST(test_mpu6050_acc_getters_converters);
    RUN_TEST(test_mpu6050_device_get_data);
    RUN_TEST(test_mpu6050_gyro_getters_converter);
    RUN_TEST(test_mpu6050_self_test);
    RUN_TEST(test_mpu6050_reset_sleep_wake);
    RUN_TEST(test_mpu6050_acc_getters_converters);
    RUN_TEST(test_mpu6050_device_get_data);
    return UNITY_END();
}