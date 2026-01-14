
#include "hal_i2c.h"
#include <pigpio.h>
#include <stdlib.h>
#include <stdio.h>

int init_i2c_rasp(void *self, uint8_t I2C_BUS, uint8_t I2C_ADDRESS)
{
    int handle = i2cOpen(I2C_BUS, I2C_ADDRESS, 0);
    if (handle < 0)
    {
        fprintf(stderr, "Failed to open I2C bus %d with address 0x%02X\n", I2C_BUS, I2C_ADDRESS);
        gpioTerminate();
        return -1;
    }
    return handle;
}

int close_i2c_rasp(void *self, int i2c_handle)
{
    if (i2cClose(i2c_handle) < 0)
    {
        fprintf(stderr, "Failed to close I2C handle %d\n", i2c_handle);
        return -1;
    }
    return 0;
}

int write_i2c_rasp(void *self, int i2c_handle, uint8_t *tx_buffer, int length, uint8_t register_address)
{
    int i2c_handle = *((int *)self);
    int bytes_written = i2cWriteDevice(i2c_handle, &register_address, 1);
    if (bytes_written != 1)
    {
        fprintf(stderr, "Failed to write register address 0x%02X\n", register_address);
        return -1;
    }
    bytes_written = i2cWriteDevice(i2c_handle, tx_buffer, length);
    if (bytes_written != length)
    {
        fprintf(stderr, "Failed to write %d bytes to I2C device\n", length);
        return -1;
    }
    return bytes_written;
}

int read_i2c_rasp(void *self, int i2c_handle, uint8_t *rx_buffer, int length, uint8_t register_address)
{
    int i2c_handle = *((int *)self);
    int bytes_written = i2cWriteDevice(i2c_handle, &register_address, 1);
    if (bytes_written != 1)
    {
        fprintf(stderr, "Failed to write register address 0x%02X for reading\n", register_address);
        return -1;
    }
    int bytes_read = i2cReadDevice(i2c_handle, rx_buffer, length);
    if (bytes_read != length)
    {
        fprintf(stderr, "Failed to read %d bytes from I2C device\n", length);
        return -1;
    }
    return bytes_read;
}

HAL_I2C hal_i2c = {
    .init_i2c = init_i2c_rasp,
    .close_i2c = close_i2c_rasp,
    .write_i2c = write_i2c_rasp,
    .read_i2c = read_i2c_rasp};