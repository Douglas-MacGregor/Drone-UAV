#include "mpu6050_device.h"
#include "mpu6050_utils.h"
#include "../../utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int read_mpu6050(int i2c_handle, mpu6050_Data *data)
{
    int n = i2cReadI2CBlockData(i2c_handle, data->address, data->data_receive, data->length);
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