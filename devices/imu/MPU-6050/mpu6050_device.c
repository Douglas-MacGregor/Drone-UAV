#include "mpu6050_device.h"
#include "imu_interface.h"
#include "mpu6050_utils.h"
#include "coordinate.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "hal_time.h"

IMUInterface mpu6050_imu_interface = {
    .init = mpu6050_init,
    .close = mpu6050_close,
    .read_data = mpu6050_read_data,
    .self_test = mpu6050_self_test,
    .reset = mpu6050_reset,
    .sleep = mpu6050_sleep,
    .wake = mpu6050_wake};

extern HAL_TIME hal_time;

/*
 * Function: mpu6050_init
 * Description: Initializes the MPU6050 device by resetting and waking it
 * Parameters: self - Pointer to the MPU6050 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Leaves device in 'awake' state
 * Assumptions: Device is properly connected via I2C
 * Notes: None
 */
int mpu6050_init(void *self)
{
    mpu6050_Device *device = (mpu6050_Device *)self;
    device->vtable->reset(self);
    device->vtable->wake(self);
    return 0;
}

/*
 * Function: mpu6050_close
 * Description: Clean up operations for the MPU6050 device if necessary
 * Parameters: self - Pointer to the MPU6050 device instance
 * Returns: 0 on success
 * Side Effects: None currently implemented
 * Assumptions: Device has been previously initialized
 * Notes: None
 */
int mpu6050_close(void *self)
{
    mpu6050_Device *device = (mpu6050_Device *)self;
    return 0;
}

/*
 * Function: mpu6050_read_data
 * Description: Reads data from the device registers and performs necessary conversions
 * Parameters: self - Pointer to the MPU6050 device instance
 *            data - Pointer to IMUData structure to store the results
 * Returns: 0 on success, -1 on error
 * Side Effects: Returns gyro data in dps and acceleration in g's
 * Assumptions: The device is powered on and in an 'awake' state
 * Notes: Currently doesn't read temperature data
 */
int mpu6050_read_data(void *self, void *data)
{
    mpu6050_Device *device = (mpu6050_Device *)self;
    IMUData *imu_data = (IMUData *)data;

    int16_t raw_gyroX, raw_gyroY, raw_gyroZ;
    int16_t raw_accelX, raw_accelY, raw_accelZ;
    int8_t raw_data[14];
    mpu6050_Data mpu_data;
    mpu_data.address = REG_ACCEL_XOUT_H; // Starting register for accel and gyro
    mpu_data.length = 14;
    mpu_data.data_receive = (uint8_t *)raw_data;
    int n = read_mpu6050(device->iic_handle, &mpu_data);
    if (n != 14)
    {
        fprintf(stderr, "MPU6050 read data error\n");
        return -1; // Error reading data
    }
    raw_accelX = (int16_t)((raw_data[0] << 8) | raw_data[1]);
    raw_accelY = (int16_t)((raw_data[2] << 8) | raw_data[3]);
    raw_accelZ = (int16_t)((raw_data[4] << 8) | raw_data[5]);
    // Skipping temperature bytes [6] and [7]
    raw_gyroX = (int16_t)((raw_data[8] << 8) | raw_data[9]);
    raw_gyroY = (int16_t)((raw_data[10] << 8) | raw_data[11]);
    raw_gyroZ = (int16_t)((raw_data[12] << 8) | raw_data[13]);
    float gyroX_dps, gyroY_dps, gyroZ_dps;
    float accelX_g, accelY_g, accelZ_g;
    if (convert_gyro_to_dps(raw_gyroX, device->gyro_fs, &(imu_data->gyroX), device->gyro_bias.x) < 0 ||
        convert_gyro_to_dps(raw_gyroY, device->gyro_fs, &(imu_data->gyroY), device->gyro_bias.y) < 0 ||
        convert_gyro_to_dps(raw_gyroZ, device->gyro_fs, &(imu_data->gyroZ), device->gyro_bias.z) < 0 ||
        convert_accel_to_g(raw_accelX, device->accel_fs, &(imu_data->accelX), device->accel_bias.x) < 0 ||
        convert_accel_to_g(raw_accelY, device->accel_fs, &(imu_data->accelY), device->accel_bias.y) < 0 ||
        convert_accel_to_g(raw_accelZ, device->accel_fs, &(imu_data->accelZ), device->accel_bias.z) < 0)
    {
        fprintf(stderr, "MPU6050 data conversion error\n");
        return -1; // Error converting data
    }
    imu_data->temperature = -300; // Temperature reading not implemented yet
    return 0;
}

/*
 * Function: mpu6050_self_test
 * Description: Performs self-test on the MPU6050 device
 * Parameters: self - Pointer to the MPU6050 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Resets device configuration during test and leaves it 'awake'
 * Assumptions: Device is powered on and connected via I2C
 * Notes: None
 */
int mpu6050_self_test(void *self)
{
    mpu6050_Device *device = (mpu6050_Device *)self;
    device->vtable->reset(self);

    mpu6050_Data power;
    power.address = REG_PWR_MGMT_1;
    power.data = 0x00; // Clear sleep bit to wake up the device
    power.length = 1;
    int n = write_mpu6050(device->iic_handle, &power);
    if (n < 0)
    {
        fprintf(stderr, "MPU6050 wake write error\n");
        return -1;
    }
    mpu6050_Data config;
    config.address = REG_GYRO_CONFIG;
    config.data = 0x00; // Set gyro full scale to ±250 dps
    config.length = 1;
    n = write_mpu6050(device->iic_handle, &config);
    if (n < 0)
    {
        fprintf(stderr, "MPU6050 gyro config write error\n");
        return -1;
    }
    config.address = REG_ACCEL_CONFIG;
    config.data = 0x10; // Set accel full scale to ±2g
    config.length = 1;
    n = write_mpu6050(device->iic_handle, &config);
    if (n < 0)
    {
        fprintf(stderr, "MPU6050 accel config write error\n");
        return -1;
    }

    coordinate3D_t gyro_bias_initial;
    coordinate3D_t accel_bias_initial;
    get_gyro_mean_window_mpu6050(device->iic_handle, &gyro_bias_initial, 200.0);
    get_accel_mean_window_mpu6050(device->iic_handle, &accel_bias_initial, 200.0);
    mpu6050_Data data;
    data.address = REG_GYRO_CONFIG;
    data.data = (0xE0); // Enable self-test for all axes
    data.length = 1;
    n = write_mpu6050(device->iic_handle, &data);
    if (n < 0)
    {
        fprintf(stderr, "MPU6050 self-test write error\n");
        return -1;
    }
    data.address = REG_ACCEL_CONFIG;
    data.data = (0xE0 | 0x10); // Enable self-test for all axes
    data.length = 1;
    n = write_mpu6050(device->iic_handle, &data);
    if (n < 0)
    {
        fprintf(stderr, "MPU6050 self-test write error\n");
        return -1;
    }
    hal_time.delay_ms(200); // wait for 200ms for self-test to complete
    coordinate3D_t gyro_bias_self_test;
    coordinate3D_t accel_bias_self_test;
    get_gyro_mean_window_mpu6050(device->iic_handle, &gyro_bias_self_test, 200.0);
    get_accel_mean_window_mpu6050(device->iic_handle, &accel_bias_self_test, 200.0);
    // Compare biases to determine if self-test passed
    // str values //
    float gyro_diff_x = gyro_bias_self_test.x - gyro_bias_initial.x;
    float gyro_diff_y = gyro_bias_self_test.y - gyro_bias_initial.y;
    float gyro_diff_z = gyro_bias_self_test.z - gyro_bias_initial.z;
    float accel_diff_x = accel_bias_self_test.x - accel_bias_initial.x;
    float accel_diff_y = accel_bias_self_test.y - accel_bias_initial.y;
    float accel_diff_z = accel_bias_self_test.z - accel_bias_initial.z;

    // Factory trim values
    uint8_t self_test_gx, self_test_gy, self_test_gz;
    uint8_t self_test_ax, self_test_ay, self_test_az;
    data.address = REG_SELF_TEST_X;
    data.length = 1;
    data.data_receive = &self_test_gx;
    read_mpu6050(device->iic_handle, &data);
    self_test_ax = (self_test_gx & 0xE0) >> 3;
    self_test_gx &= 0x1F;

    data.address = REG_SELF_TEST_Y;
    data.length = 1;
    data.data_receive = &self_test_gy;
    read_mpu6050(device->iic_handle, &data);
    self_test_ay = (self_test_gy & 0xE0) >> 3;
    self_test_gy &= 0x1F;

    data.address = REG_SELF_TEST_Z;
    data.length = 1;
    data.data_receive = &self_test_gz;
    read_mpu6050(device->iic_handle, &data);
    self_test_az = (self_test_gz & 0xE0) >> 3;
    self_test_gz &= 0x1F;
    uint8_t self_test_a;
    data.address = REG_SELF_TEST_A;
    data.length = 1;
    data.data_receive = &self_test_a;
    read_mpu6050(device->iic_handle, &data);
    self_test_ax |= (self_test_a & 0x30) >> 4;
    self_test_ay |= (self_test_a & 0x0C) >> 2;
    self_test_az |= (self_test_a & 0x03);

    float factory_trim_gyro_x = 25.0f * 131.0f * powf(1.046f, (float)self_test_gx - 1.0f);
    float factory_trim_gyro_y = -25.0f * 131.0f * powf(1.046f, (float)self_test_gy - 1.0f);
    float factory_trim_gyro_z = 25.0f * 131.0f * powf(1.046f, (float)self_test_gz - 1.0f);
    float factory_trim_accel_x = 4096.0f * 0.34f * powf(0.92f / 0.34f, ((self_test_ax - 1.0f) / 30));
    float factory_trim_accel_y = 4096.0f * 0.34f * powf(0.92f / 0.34f, ((self_test_ay - 1.0f) / 30));
    float factory_trim_accel_z = 4096.0f * 0.34f * powf(0.92f / 0.34f, ((self_test_az - 1.0f) / 30));

    float gyro_result_x = ((gyro_diff_x - factory_trim_gyro_x) / factory_trim_gyro_x) * 100.0f;
    float gyro_result_y = ((gyro_diff_y - factory_trim_gyro_y) / factory_trim_gyro_y) * 100.0f;
    float gyro_result_z = ((gyro_diff_z - factory_trim_gyro_z) / factory_trim_gyro_z) * 100.0f;
    float accel_result_x = ((accel_diff_x - factory_trim_accel_x) / factory_trim_accel_x) * 100.0f;
    float accel_result_y = ((accel_diff_y - factory_trim_accel_y) / factory_trim_accel_y) * 100.0f;
    float accel_result_z = ((accel_diff_z - factory_trim_accel_z) / factory_trim_accel_z) * 100.0f;
    if (gyro_result_x < -14.0f || gyro_result_x > 14.0f ||
        gyro_result_y < -14.0f || gyro_result_y > 14.0f ||
        gyro_result_z < -14.0f || gyro_result_z > 14.0f ||
        accel_result_x < -14.0f || accel_result_x > 14.0f ||
        accel_result_y < -14.0f || accel_result_y > 14.0f ||
        accel_result_z < -14.0f || accel_result_z > 14.0f)
    {
        fprintf(stderr, "MPU6050 Self-Test Failed\n");
        return -1;
    }
    device->vtable->reset(self);
    hal_time.delay_ms(100); // wait for 100ms after reset
    device->vtable->wake(self);
    hal_time.delay_ms(100); // wait for 100ms after wake
    return 0;
}

/*
 * Function: mpu6050_reset
 * Description: Resets the MPU6050 device
 * Parameters: self - Pointer to the MPU6050 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Device is reset and requires reconfiguration
 * Assumptions: Device is properly connected via I2C
 * Notes: Leaves device in unspecified state
 */
int mpu6050_reset(void *self)
{
    mpu6050_Device *device = (mpu6050_Device *)self;
    mpu6050_Data data;
    data.address = REG_PWR_MGMT_1;
    data.data = 0x80; // Set the reset bit
    data.length = 1;
    int n = write_mpu6050(device->iic_handle, &data);
    hal_time.delay_ms(100); // wait for 100ms after reset
    if (n < 0)
    {
        fprintf(stderr, "MPU6050 reset error\n");
        return -1;
    }
    return 0;
}

/*
 * Function: mpu6050_sleep
 * Description: Puts the MPU6050 device into sleep mode
 * Parameters: self - Pointer to the MPU6050 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Device enters sleep mode
 * Assumptions: Device is properly connected via I2C
 * Notes: None
 */
int mpu6050_sleep(void *self)
{
    mpu6050_Device *device = (mpu6050_Device *)self;
    mpu6050_Data data;
    data.address = REG_PWR_MGMT_1;
    data.data = 0x40; // Set the sleep bit
    data.length = 1;
    int n = write_mpu6050(device->iic_handle, &data);
    hal_time.delay_ms(100); // wait for 100ms after sleep command
    if (n < 0)
    {
        fprintf(stderr, "MPU6050 sleep error\n");
        return -1;
    }
    return 0;
}

/*
 * Function: mpu6050_wake
 * Description: Wakes the MPU6050 device from sleep mode and configures it
 * Parameters: self - Pointer to the MPU6050 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Device is configured and ready for operation
 * Assumptions: Device is properly connected via I2C
 * Notes: Leaves device in 'awake' state and sets device bias values
 */
int mpu6050_wake(void *self)
{
    mpu6050_Device *device = (mpu6050_Device *)self;
    configure_mpu6050(device->iic_handle, device->gyro_fs, device->accel_fs, &(device->gyro_bias), &(device->accel_bias));
    return 0;
}

/*
 * Function: create_mpu6050_device
 * Description: Creates and initializes an MPU6050 device instance
 * Parameters: iic_handle - I2C handle for communication
 *            gyro_fs - Gyroscope full-scale range
 *            accel_fs - Accelerometer full-scale range
 * Returns: Initialized mpu6050_Device structure
 * Side Effects: None
 * Assumptions: None
 * Notes: None
 */
mpu6050_Device create_mpu6050_device(int iic_handle, mpu6050_gyro_fs_t gyro_fs, mpu6050_accel_fs_t accel_fs)
{
    mpu6050_Device device;
    device.vtable = &mpu6050_imu_interface;
    device.iic_handle = iic_handle;
    device.gyro_fs = gyro_fs;
    device.accel_fs = accel_fs;
    return device;
}