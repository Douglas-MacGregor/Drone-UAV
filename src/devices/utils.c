#include <pigpio.h>
#include "utils.h"
#include <stdio.h>

int init_spi()
{
    int spi_handle = spiOpen(SPI_CHANNEL, SPI_SPEED, SPI_MODE);
    if (spi_handle < 0)
    {
        fprintf(stdout, "spi init fail\n");
        return -1;
    }
    return spi_handle;
}

void close_spi(int spi_handle)
{
    spiClose(spi_handle);
    return;
}

int init_gpio()
{
    if (gpioInitialise() < 0)
    {
        fprintf(stdout, "gpio init fail\n");
        return -1;
    }
    return 0;
}

void close_gpio()
{
    gpioTerminate();
    return;
}

int init_i2c(uint8_t I2C_BUS, uint8_t I2C_ADDRESS)
{
    int i2c_handle = i2cOpen(I2C_BUS, I2C_ADDRESS, 0);
    if (i2c_handle < 0)
    {
        fprintf(stdout, "i2c init fail\n");
        return -1;
    }
    return i2c_handle;
}

void close_i2c(int i2c_handle)
{
    i2cClose(i2c_handle);
    return;
}