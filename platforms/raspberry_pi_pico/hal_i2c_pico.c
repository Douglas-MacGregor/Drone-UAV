#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "hal_i2c.h"

int pico_i2c_init(uint8_t I2C_BUS, uint8_t I2C_ADDRESS)
{
    if (I2C_BUS == 0)
    {
        i2c_init(i2c0, 100 * 1000);
        gpio_set_function(0, GPIO_FUNC_I2C);
        gpio_set_function(1, GPIO_FUNC_I2C);
        // i2c_slave_init(i2c0, I2C_ADDRESS, 0);
    }
    else if (I2C_BUS == 1)
    {
        i2c_init(i2c1, 100 * 1000);
        gpio_set_function(26, GPIO_FUNC_I2C);
        gpio_set_function(27, GPIO_FUNC_I2C);
        // i2c_slave_init(i2c1, I2C_ADDRESS, 0);
    }
    else
    {
        return -1; // Invalid I2C bus
    }
    return 0;
}

int pico_i2c_close(int i2c_handle)
{
    // Cleanup code for Raspberry Pi Pico I2C can be added here if needed
    if (i2c_handle == 0)
    {
        i2c_deinit(i2c0);
    }
    else if (i2c_handle == 1)
    {
        i2c_deinit(i2c1);
    }
    return 0;
}

int pico_i2c_write(int i2c_handle, uint8_t *tx_buffer, int length, uint8_t register_address)
{
    if (i2c_handle == 0)
    {
        return i2c_write_blocking(i2c0, register_address, tx_buffer, length, false);
    }
    else if (i2c_handle == 1)
    {
        return i2c_write_blocking(i2c1, register_address, tx_buffer, length, false);
    }
    return -1; // Invalid I2C handle
}

int pico_i2c_read(int i2c_handle, uint8_t *rx_buffer, int length, uint8_t register_address)
{
    if (i2c_handle == 0)
    {
        return i2c_read_blocking(i2c0, register_address, rx_buffer, length, false);
    }
    else if (i2c_handle == 1)
    {
        return i2c_read_blocking(i2c1, register_address, rx_buffer, length, false);
    }
    return -1; // Invalid I2C handle
}

HAL_I2C hal_i2c = {
    .init_i2c = pico_i2c_init,
    .close_i2c = pico_i2c_close,
    .write_i2c = pico_i2c_write,
    .read_i2c = pico_i2c_read};