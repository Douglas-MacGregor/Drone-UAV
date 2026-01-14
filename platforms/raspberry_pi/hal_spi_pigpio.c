#include "hal_spi.h"
#include <pigpio.h>
#include <stdio.h>

int init_spi_pigpio(void *self, uint8_t SPI_BUS, uint8_t SPI_MODE, uint32_t SPI_SPEED)
{
    int spi_handle = spiOpen(SPI_CHANNEL, SPI_SPEED, SPI_MODE);
    if (spi_handle < 0)
    {
        fprintf(stderr, "spi init fail\n");
        return -1;
    }
    return spi_handle;
}

int close_spi_pigpio(void *self, int spi_handle)
{
    spiClose(spi_handle);
    return 0;
}

int write_spi_pigpio(void *self, int spi_handle, uint8_t *tx_buffer, int length)
{
    int bytes_written = spiWrite(spi_handle, (char *)tx_buffer, length);
    if (bytes_written != length)
    {
        fprintf(stderr, "Failed to write %d bytes to SPI device\n", length);
        return -1;
    }
    return bytes_written;
}

int read_spi_pigpio(void *self, int spi_handle, uint8_t *rx_buffer, int length, uint8_t register_address)
{
    uint8_t *tx_buffer = malloc(length + 1);
    memset(tx_buffer, 0, length + 1);
    tx_buffer[0] = register_address & ~(0b10000000);
    int bytes_transferred = spiXfer(spi_handle, (char *)tx_buffer, (char *)rx_buffer, length + 1);
    free(tx_buffer);
    if (bytes_transferred != length + 1)
    {
        fprintf(stderr, "Failed to read %d bytes from SPI device\n", length);
        return -1;
    }
    // Shift received data to remove the first byte (register address)
    memccpy(rx_buffer, &rx_buffer[1], length, length);
    return length;
}

HAL_SPI hal_spi_pigpio = {
    .init_spi = init_spi_pigpio,
    .close_spi = close_spi_pigpio,
    .write_spi = write_spi_pigpio,
    .read_spi = read_spi_pigpio};