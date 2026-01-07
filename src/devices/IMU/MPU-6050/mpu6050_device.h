#ifndef MPU6050_DEVICE_H
#define MPU6050_DEVICE_H

#include "../imu_interface.h"
#include "mpu6050_utils.h"
#include "mpu6050_utils.h"

typedef struct
{
    IMUInterface *vtable; // pointer to interface
    int iic_handle;       // SPI handle for communication
    mpu6050_gyro_fs_t gyro_fs;
    mpu6050_accel_fs_t accel_fs;
    cordirnate3D_t gyro_bias;
    cordirnate3D_t accel_bias;
} mpu6050_Device;

extern IMUInterface mpu6050_imu_interface;

int mpu6050_init(void *self);
int mpu6050_close(void *self);
int mpu6050_read_data(void *self, void *data);
int mpu6050_self_test(void *self);
int mpu6050_reset(void *self);
int mpu6050_sleep(void *self);
int mpu6050_wake(void *self);
int mpu6050_self_test(void *self);
int mpu6050_reset(void *self);
int mpu6050_sleep(void *self);
int mpu6050_wake(void *self);

mpu6050_Device create_mpu6050_device(int iic_handle, mpu6050_gyro_fs_t gyro_fs, mpu6050_accel_fs_t accel_fs);
mpu6050_Device create_mpu6050_device(int iic_handle, mpu6050_gyro_fs_t gyro_fs, mpu6050_accel_fs_t accel_fs);

#endif // MPU6050_DEVICE_H