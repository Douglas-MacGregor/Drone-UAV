#include "mpu6050_utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "imu_interface.h"
#include "hal_i2c.h"

extern HAL_I2C hal_i2c;

int read_mpu6050(int i2c_handle, mpu6050_Data *data)
{
    int n = hal_i2c.read_i2c(i2c_handle, data->data_receive, data->length, data->address);
    if (n < 0)
    {
        fprintf(stderr, "MPU6050 read error\n");
        return -1;
    }
    else
    {
        return n;
    }
}

int write_mpu6050(int i2c_handle, mpu6050_Data *data)
{
    int n = hal_i2c.write_i2c(i2c_handle, &data->data, data->length, data->address);
    if (n < 0)
    {
        fprintf(stderr, "MPU6050 write error\n");
        return -1;
    }
    else
    {
        return n;
    }
}

int get_gyroX_mpu6050(int i2c_handle, int16_t *gyroX)
{
    mpu6050_Data data;
    data.address = REG_GYRO_XOUT_H; // Gyro X high byte register
    data.length = 2;
    uint8_t buffer[2];
    data.data_receive = buffer;
    int n = read_mpu6050(i2c_handle, &data);
    if (n != 2)
    {
        return -1;
    }
    *gyroX = (int16_t)((buffer[0] << 8) | buffer[1]);
    return 0;
}

int get_gyroY_mpu6050(int i2c_handle, int16_t *gyroY)
{
    mpu6050_Data data;
    data.address = REG_GYRO_YOUT_H; // Gyro Y high byte register
    data.length = 2;
    uint8_t buffer[2];
    data.data_receive = buffer;
    int n = read_mpu6050(i2c_handle, &data);
    if (n != 2)
    {
        return -1;
    }
    *gyroY = (int16_t)((buffer[0] << 8) | buffer[1]);
    return 0;
}

int get_gyroZ_mpu6050(int i2c_handle, int16_t *gyroZ)
{
    mpu6050_Data data;
    data.address = REG_GYRO_ZOUT_H; // Gyro Z high byte register
    data.length = 2;
    uint8_t buffer[2];
    data.data_receive = buffer;
    int n = read_mpu6050(i2c_handle, &data);
    if (n != 2)
    {
        return -1;
    }
    *gyroZ = (int16_t)((buffer[0] << 8) | buffer[1]);
    return 0;
}

int convert_gyro_to_dps(int16_t raw_gyro, mpu6050_gyro_fs_t fs, float *dps, float bias)
{
    float sensitivity;
    switch (fs)
    {
    case GYRO_FS_250:
        sensitivity = GYRO_250_SENSITIVITY;
        break;
    case GYRO_FS_500:
        sensitivity = GYRO_250_SENSITIVITY / 2.0f;
        break;
    case GYRO_FS_1000:
        sensitivity = GYRO_250_SENSITIVITY / 4.0f;
        break;
    case GYRO_FS_2000:
        sensitivity = GYRO_250_SENSITIVITY / 8.0f;
        break;
    default:
        return -1; // Invalid full-scale range
    }
    *dps = ((float)raw_gyro - bias) / sensitivity;
    return 0;
}

int get_accelX_mpu6050(int i2c_handle, int16_t *accelX)
{
    mpu6050_Data data;
    data.address = REG_ACCEL_XOUT_H; // Accel X high byte register
    data.length = 2;
    uint8_t buffer[2];
    data.data_receive = buffer;
    int n = read_mpu6050(i2c_handle, &data);
    if (n != 2)
    {
        return -1;
    }
    *accelX = (int16_t)((buffer[0] << 8) | buffer[1]);
    return 0;
}

int get_accelY_mpu6050(int i2c_handle, int16_t *accelY)
{
    mpu6050_Data data;
    data.address = REG_ACCEL_YOUT_H; // Accel Y high byte register
    data.length = 2;
    uint8_t buffer[2];
    data.data_receive = buffer;
    int n = read_mpu6050(i2c_handle, &data);
    if (n != 2)
    {
        return -1;
    }
    *accelY = (int16_t)((buffer[0] << 8) | buffer[1]);
    return 0;
}

int get_accelZ_mpu6050(int i2c_handle, int16_t *accelZ)
{
    mpu6050_Data data;
    data.address = REG_ACCEL_ZOUT_H; // Accel Z high byte register
    data.length = 2;
    uint8_t buffer[2];
    data.data_receive = buffer;
    int n = read_mpu6050(i2c_handle, &data);
    if (n != 2)
    {
        return -1;
    }
    *accelZ = (int16_t)((buffer[0] << 8) | buffer[1]);
    return 0;
}

int convert_accel_to_g(int16_t raw_accel, mpu6050_accel_fs_t fs, float *g, float bias)
{
    float sensitivity;
    switch (fs)
    {
    case ACCEL_FS_2:
        sensitivity = ACCEL_2_SENSITIVITY;
        break;
    case ACCEL_FS_4:
        sensitivity = ACCEL_4_SENSITIVITY;
        break;
    case ACCEL_FS_8:
        sensitivity = ACCEL_8_SENSITIVITY;
        break;
    case ACCEL_FS_16:
        sensitivity = ACCEL_16_SENSITIVITY;
        break;
    default:
        return -1; // Invalid full-scale range
    }
    *g = ((float)raw_accel - bias) / sensitivity;
    return 0;
}

int configure_mpu6050(int i2c_handle, mpu6050_gyro_fs_t gyro_fs, mpu6050_accel_fs_t accel_fs, coordinate3D_t *gyro_bias, coordinate3D_t *accel_bias)
{
    mpu6050_Data data;

    // Wake up the MPU-6050 by writing 0 to the PWR_MGMT_1 register
    data.address = REG_PWR_MGMT_1;
    data.data = 0x00;
    data.length = 1;
    int n = write_mpu6050(i2c_handle, &data);
    if (n < 0)
    {
        fprintf(stderr, "Failed to wake up MPU-6050\n");
        return -1;
    }
    usleep(100000); // 100 ms

    // Configure gyro full-scale range
    data.address = REG_GYRO_CONFIG;
    data.data = (gyro_fs << 3); // Set FS_SEL bits
    data.length = 1;
    n = write_mpu6050(i2c_handle, &data);
    if (n < 0)
    {
        fprintf(stderr, "Failed to configure gyro full-scale range\n");
        return -1;
    }

    // Configure accelerometer full-scale range
    data.address = REG_ACCEL_CONFIG;
    data.data = (accel_fs << 3); // Set AFS_SEL bits
    data.length = 1;
    n = write_mpu6050(i2c_handle, &data);
    if (n < 0)
    {
        fprintf(stderr, "Failed to configure accelerometer full-scale range\n");
        return -1;
    }

    n = get_accel_mean_window_mpu6050(i2c_handle, accel_bias, 1000.0);
    if (n < 0)
    {
        fprintf(stderr, "Failed to get accelerometer bias\n");
        return -1;
    }
    n = get_accel_mean_window_mpu6050(i2c_handle, gyro_bias, 1000.0);
    if (n < 0)
    {
        fprintf(stderr, "Failed to get gyroscope bias\n");
        return -1;
    }
    return 0;
}

int get_gyro_mean_window_mpu6050(int i2c_handle, coordinate3D_t *gyro_bias, float samples)
{
    // Placeholder implementation
    int16_t raw_gyroX, raw_gyroY, raw_gyroZ;
    float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;
    for (int i = 0; i < samples; i++)
    {
        get_gyroX_mpu6050(i2c_handle, &raw_gyroX);
        get_gyroY_mpu6050(i2c_handle, &raw_gyroY);
        get_gyroZ_mpu6050(i2c_handle, &raw_gyroZ);
        sumX += (float)raw_gyroX;
        sumY += (float)raw_gyroY;
        sumZ += (float)raw_gyroZ;
        usleep(1000); // 1ms delay between samples
    }
    gyro_bias->x = sumX / samples;
    gyro_bias->y = sumY / samples;
    gyro_bias->z = sumZ / samples;
    return 0;
}

int get_accel_mean_window_mpu6050(int i2c_handle, coordinate3D_t *accel_bias, float samples)
{
    int16_t raw_accelX, raw_accelY, raw_accelZ;
    float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;
    for (int i = 0; i < samples; i++)
    {
        get_accelX_mpu6050(i2c_handle, &raw_accelX);
        get_accelY_mpu6050(i2c_handle, &raw_accelY);
        get_accelZ_mpu6050(i2c_handle, &raw_accelZ);
        sumX += (float)raw_accelX;
        sumY += (float)raw_accelY;
        sumZ += (float)raw_accelZ;
        usleep(1000); // 1ms delay between samples
    }
    accel_bias->x = sumX / samples;
    accel_bias->y = sumY / samples;
    accel_bias->z = sumZ / samples;
    return 0;
}