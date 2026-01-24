#include "lgpio.h"
#include "hal_i2c.h"

int lgpio_init_i2c(uint8_t i2c_bus, uint8_t i2c_address)
{
    // lgI2cOpen takes: i2cDev (bus), i2cAddr (device address), i2cFlags (0 for default)
    int i2c_handle = lgI2cOpen(i2c_bus, i2c_address, 0);
    if (i2c_handle < 0)
    {
        return -1;
    }
    return i2c_handle;
}

int lgpio_close_i2c(int i2c_handle)
{
    lgI2cClose(i2c_handle);
    return 0;
}

int lgpio_write_i2c(int i2c_handle, uint8_t *tx_buffer, int length, uint8_t register_address)
{
    // For register-based writes, we need to write the register first, then the data
    // lgpio doesn't have a direct WriteRegBlock, so we use WriteDevice with register prepended
    uint8_t write_buffer[length + 1];
    write_buffer[0] = register_address;
    for (int i = 0; i < length; i++)
    {
        write_buffer[i + 1] = tx_buffer[i];
    }

    int result = lgI2cWriteDevice(i2c_handle, (char *)write_buffer, length + 1);
    if (result < 0)
    {
        return -1;
    }
    return result;
}

int lgpio_read_i2c(int i2c_handle, uint8_t *rx_buffer, int length, uint8_t register_address)
{
    // First write the register address
    int result = lgI2cWriteDevice(i2c_handle, (char *)&register_address, 1);
    if (result < 0)
    {
        return -1;
    }

    // Then read the data
    result = lgI2cReadDevice(i2c_handle, (char *)rx_buffer, length);
    if (result < 0)
    {
        return -1;
    }
    return result;
}

HAL_I2C hal_i2c = {
    .init_i2c = lgpio_init_i2c,
    .close_i2c = lgpio_close_i2c,
    .write_i2c = lgpio_write_i2c,
    .read_i2c = lgpio_read_i2c};