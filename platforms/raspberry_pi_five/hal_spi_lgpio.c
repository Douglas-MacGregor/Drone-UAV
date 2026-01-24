#include "lgpio.h"
#include "hal_spi.h"

int lgpio_init_spi(uint8_t spi_bus, uint8_t spi_mode, uint32_t spi_speed)
{
    int spi_handle = lgSpiOpen(spi_bus, spi_speed, spi_mode);
    if (spi_handle < 0)
    {
        return -1;
    }
    return spi_handle;
}

int lgpio_close_spi(int spi_handle)
{
    lgSpiClose(spi_handle);
    return 0;
}

int lgpio_write_spi(int spi_handle, uint8_t *tx_buffer, int length)
{
    int result = lgSpiWrite(spi_handle, tx_buffer, length);
    if (result < 0)
    {
        return -1;
    }
    return result;
}

int lgpio_read_spi(int spi_handle, uint8_t *tx_buffer, uint8_t *rx_buffer, int length)
{
    int result = lgSpiXfer(spi_handle, tx_buffer, rx_buffer, length);
    if (result < 0)
    {
        return -1;
    }
    return result;
}

HAL_SPI hal_spi = {
    .init_spi = lgpio_init_spi,
    .close_spi = lgpio_close_spi,
    .write_spi = lgpio_write_spi,
    .read_spi = lgpio_read_spi};
