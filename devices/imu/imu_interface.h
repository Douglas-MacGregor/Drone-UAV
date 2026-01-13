#ifndef IMU_INTERFACE_H
#define IMU_INTERFACE_H

#include <stdint.h>

typedef struct
{
    int (*init)(void *self);
    int (*close)(void *self);
    int (*read_data)(void *self, void *data);
    int (*self_test)(void *self);
    int (*reset)(void *self);
    int (*sleep)(void *self);
    int (*wake)(void *self);
    // add more function pointers as needed
} IMUInterface;

typedef struct
{
    float gyroX;
    float gyroY;
    float gyroZ;
    float accelX;
    float accelY;
    float accelZ;
    float temperature;
} IMUData;

#endif // IMU_INTERFACE_H