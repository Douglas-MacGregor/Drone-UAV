#include "mpu6050_device.h"
#include "../imu_interface.h"
#include "mpu6050_device.h"
#include "mpu6050_utils.h"
#include "../../utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

IMUInterface mpu6050_imu_interface = {
    .init = mpu6050_init,
    .close = mpu6050_close,
    .read_data = mpu6050_read_data,
    .self_test = mpu6050_self_test,
    .reset = mpu6050_reset,
    .sleep = mpu6050_sleep,
    .wake = mpu6050_wake};

int mpu6050_init(void *self)
{
    mpu6050_Device *device = (mpu6050_Device *)self;
    device->vtable->reset(self);
    return 0;
}

int mpu6050_close(void *self)
{
    mpu6050_Device *device = (mpu6050_Device *)self;
    // cleanup operations if needed
    return 0;
}

int mpu6050_read_data(void *self, void *data)
{

    mpu6050_Device *device = (mpu6050_Device *)self;
    mpu6050_Data sleep_check;
    sleep_check.address = REG_PWR_MGMT_1;
    uint8_t reg_value;
    sleep_check.length = 1;
    sleep_check.data_receive = &reg_value;
    int n = read_mpu6050(device->iic_handle, &sleep_check);
    if (n != 1)
    {
        fprintf(stderr, "MPU6050 read PWR_MGMT_1 error\n");
        return -1;
    }
    if (reg_value & 0x40 != 0)
    {
        fprintf(stderr, "Warning: MPU6050 is in sleep mode\n");
        return -1;
    }
    // Wake up the device
    IMUData *imu_data = (IMUData *)data;

    int16_t raw_gyroX, raw_gyroY, raw_gyroZ;
    int16_t raw_accelX, raw_accelY, raw_accelZ;
    if (get_gyroX_mpu6050(device->iic_handle, &raw_gyroX) < 0 ||
        get_gyroY_mpu6050(device->iic_handle, &raw_gyroY) < 0 ||
        get_gyroZ_mpu6050(device->iic_handle, &raw_gyroZ) < 0 ||
        get_accelX_mpu6050(device->iic_handle, &raw_accelX) < 0 ||
        get_accelY_mpu6050(device->iic_handle, &raw_accelY) < 0 ||
        get_accelZ_mpu6050(device->iic_handle, &raw_accelZ) < 0)
    {
        fprintf(stderr, "MPU6050 read data error\n");
        return -1; // Error reading data
    }
    float gyroX_dps, gyroY_dps, gyroZ_dps;
    float accelX_g, accelY_g, accelZ_g;
    if (convert_gyro_to_dps(raw_gyroX, device->gyro_fs, &gyroX_dps) < 0 ||
        convert_gyro_to_dps(raw_gyroY, device->gyro_fs, &gyroY_dps) < 0 ||
        convert_gyro_to_dps(raw_gyroZ, device->gyro_fs, &gyroZ_dps) < 0 ||
        convert_accel_to_g(raw_accelX, device->accel_fs, &accelX_g) < 0 ||
        convert_accel_to_g(raw_accelY, device->accel_fs, &accelY_g) < 0 ||
        convert_accel_to_g(raw_accelZ, device->accel_fs, &accelZ_g) < 0)
    {
        fprintf(stderr, "MPU6050 data conversion error\n");
        return -1; // Error converting data
    }
    imu_data->gyroX = gyroX_dps;
    imu_data->gyroY = gyroY_dps;
    imu_data->gyroZ = gyroZ_dps;
    imu_data->accelX = accelX_g;
    imu_data->accelY = accelY_g;
    imu_data->accelZ = accelZ_g;
    imu_data->temperature = -300; // Temperature reading not implemented yet
    return 0;
}

int mpu6050_self_test(void *self)
{
    mpu6050_Device *device = (mpu6050_Device *)self;
    return 0;
}

int mpu6050_reset(void *self)
{
    mpu6050_Device *device = (mpu6050_Device *)self;
    mpu6050_Data data;
    data.address = REG_PWR_MGMT_1;
    data.data = 0x80; // Set the reset bit
    data.length = 1;
    int n = write_mpu6050(device->iic_handle, &data);
    usleep(100000); // wait for 100ms after reset
    if (n < 0)
    {
        fprintf(stderr, "MPU6050 reset error\n");
        return -1;
    }
    return 0;
}

int mpu6050_sleep(void *self)
{
    mpu6050_Device *device = (mpu6050_Device *)self;
    mpu6050_Data data;
    data.address = REG_PWR_MGMT_1;
    data.data = 0x40; // Set the sleep bit
    data.length = 1;
    int n = write_mpu6050(device->iic_handle, &data);
    ulseep(100000); // wait for 100ms after sleep command
    if (n < 0)
    {
        fprintf(stderr, "MPU6050 sleep error\n");
        return -1;
    }
    return 0;
}

int mpu6050_wake(void *self)
{
    mpu6050_Device *device = (mpu6050_Device *)self;
    configure_mpu6050(device->iic_handle, device->gyro_fs, device->accel_fs);
    return 0;
}

mpu6050_Device create_mpu6050_device(int iic_handle, mpu6050_gyro_fs_t gyro_fs, mpu6050_accel_fs_t accel_fs)
{
    mpu6050_Device device;
    device.vtable = &mpu6050_imu_interface;
    device.iic_handle = iic_handle;
    device.gyro_fs = gyro_fs;
    device.accel_fs = accel_fs;
    return device;
}