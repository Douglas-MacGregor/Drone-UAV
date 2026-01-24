#include "lgpio.h"
#include "hal_i2c.h"

int lgpio_init_i2c(uint8_t i2c_bus, uint32_t i2c_speed)
{
    int i2c_handle = lgI2cOpen(i2c_bus, i2c_speed);
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
    int result = lgI2cWriteRegBlock(i2c_handle, register_address, tx_buffer, length);
    if (result < 0)
    {
        return -1;
    }
    return result;
}

int lgpio_read_i2c(int i2c_handle, uint8_t *rx_buffer, int length, uint8_t register_address)
{
    int result = lgI2cReadRegBlock(i2c_handle, register_address, rx_buffer, length);
    if (result < 0)
    {
        return -1;
    }
    return result;
}

HAL_I2C hal_i2c = {
    .init_i2c = lgpio_init_i2c,
    .close_i2c = lgpio_close_i2c};