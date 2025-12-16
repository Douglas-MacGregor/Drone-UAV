#ifndef MPU6050_DEVICE_H
#define MPU6050_DEVICE_H

#include "../imu_interface.h"

typedef struct
{
    IMUInterface *vtable; // pointer to interface
    int iic_handle;       // SPI handle for communication
} mpu6050_Device;

extern IMUInterface mpu6050_imu_interface;

int mpu6050_init(void *self);
int mpu6050_close(void *self);
int mpu6050_read_data(void *self, void *data);

mpu6050_Device create_mpu6050_device(int iic_handle);

#endif // MPU6050_DEVICE_H