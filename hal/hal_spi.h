#include <stdint.h>

typedef struct
{
    int (*init_spi)(void *self, uint8_t SPI_BUS, uint8_t SPI_MODE, uint32_t SPI_SPEED);
    int (*close_spi)(void *self, int spi_handle);
    int (*write_spi)(void *self, int spi_handle, uint8_t *tx_buffer, int length);
    int (*read_spi)(void *self, int spi_handle, uint8_t *rx_buffer, int length, uint8_t register_address);
} HAL_SPI;