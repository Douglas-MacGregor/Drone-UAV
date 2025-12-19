#ifndef mpu6050_UTILS_H
#define mpu6050_UTILS_H

#include <stdint.h>

typedef struct
{
    uint8_t address;
    uint8_t data;
    uint8_t length;
    uint8_t *data_receive;
} mpu6050_Data;

typedef enum
{
    REG_WHO_AM_I = 0x75,
    REG_GYRO_XOUT_H = 0x43,
    REG_GYRO_XOUT_L = 0x44,
    REG_GYRO_YOUT_H = 0x45,
    REG_GYRO_YOUT_L = 0x46,
    REG_GYRO_ZOUT_H = 0x47,
    REG_GYRO_ZOUT_L = 0x48,
    REG_ACCEL_XOUT_H = 0x3B,
    REG_ACCEL_XOUT_L = 0x3C,
    REG_ACCEL_YOUT_H = 0x3D,
    REG_ACCEL_YOUT_L = 0x3E,
    REG_ACCEL_ZOUT_H = 0x3F,
    REG_ACCEL_ZOUT_L = 0x40,
    REG_PWR_MGMT_1 = 0x6B,
    REG_GYRO_CONFIG = 0x1B,
    REG_ACCEL_CONFIG = 0x1C
} mpu6050_registers_t;

typedef enum
{
    GYRO_FS_250 = 0,
    GYRO_FS_500 = 1,
    GYRO_FS_1000 = 2,
    GYRO_FS_2000 = 3
} mpu6050_gyro_fs_t;

typedef enum
{
    ACCEL_FS_2 = 0,
    ACCEL_FS_4 = 1,
    ACCEL_FS_8 = 2,
    ACCEL_FS_16 = 3
} mpu6050_accel_fs_t;

typedef enum
{
    GYRO_250_SENSITIVITY = 131
} mpu6050_gyro_sensitivity_t;

typedef enum
{
    ACCEL_2_SENSITIVITY = 16384,
    ACCEL_4_SENSITIVITY = 8192,
    ACCEL_8_SENSITIVITY = 4096,
    ACCEL_16_SENSITIVITY = 2048
} mpu6050_accel_sensitivity_t;

int read_mpu6050(int i2c_handle, mpu6050_Data *data);
int write_mpu6050(int i2c_handle, mpu6050_Data *data);
int configure_mpu6050(int i2c_handle, mpu6050_gyro_fs_t gyro_fs, mpu6050_accel_fs_t accel_fs);
int get_gyroX_mpu6050(int i2c_handle, int16_t *gyroX);
int get_gyroY_mpu6050(int i2c_handle, int16_t *gyroY);
int get_gyroZ_mpu6050(int i2c_handle, int16_t *gyroZ);
int convert_gyro_to_dps(int16_t raw_gyro, mpu6050_gyro_fs_t fs, float *dps);
int get_accelX_mpu6050(int i2c_handle, int16_t *accelX);
int get_accelY_mpu6050(int i2c_handle, int16_t *accelY);
int get_accelZ_mpu6050(int i2c_handle, int16_t *accelZ);
int convert_accel_to_g(int16_t raw_accel, mpu6050_accel_fs_t fs, float *g);

#endif // mpu6050_UTILS_H