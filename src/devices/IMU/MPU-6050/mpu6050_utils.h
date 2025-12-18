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
} mpu6050_registers_t;

#endif // mpu6050_UTILS_H