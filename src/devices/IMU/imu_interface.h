#ifndef IMU_INTERFACE_H
#define IMU_INTERFACE_H

#include <stdint.h>

typedef struct
{
    int (*init)(void *self);
    int (*close)(void *self);
    int (*read_data)(void *self, void *data);
    // add more function pointers as needed
} IMUInterface;

typedef struct
{
    float acceleration[3]; // x, y, z acceleration
    float gyroscope[3];    // x, y, z angular velocity
    float temperature;     // temperature in degrees Celsius
} IMUData;

#endif // IMU_INTERFACE_H