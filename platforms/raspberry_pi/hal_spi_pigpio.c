#include "hal_spi.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int init_spi_pigpio(uint8_t spi_bus, uint8_t spi_mode, uint32_t spi_speed)
{
    int spi_handle = spiOpen(SPI_CHANNEL, SPI_CHANNEL, SPI_MODE);
    if (spi_handle < 0)
    {
        fprintf(stderr, "spi init fail\n");
        return -1;
    }
    return spi_handle;
}

int close_spi_pigpio(int spi_handle)
{
    spiClose(spi_handle);
    return 0;
}

int write_spi_pigpio(int spi_handle, uint8_t *tx_buffer, int length)
{
    return spiXfer(spi_handle, (char *)tx_buffer, NULL, length);
}

int read_spi_pigpio(int spi_handle, uint8_t *tx_buffer, uint8_t *rx_buffer, int length)
{
    return spiXfer(spi_handle, (char *)tx_buffer, (char *)rx_buffer, length);
}

HAL_SPI hal_spi_pigpio = {
    .init_spi = init_spi_pigpio,
    .close_spi = close_spi_pigpio,
    .write_spi = write_spi_pigpio,
    .read_spi = read_spi_pigpio};