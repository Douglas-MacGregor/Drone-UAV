#include <stdint.h>
#define SPI_CHANNEL 0
#define SPI_SPEED 100000
#define SPI_MODE 0

typedef struct
{
    int (*init_spi)(uint8_t spi_bus, uint8_t spi_mode, uint32_t spi_speed);
    int (*close_spi)(int spi_handle);
    int (*write_spi)(int spi_handle, uint8_t *tx_buffer, int length);
    int (*read_spi)(int spi_handle, uint8_t *rx_buffer, int length, uint8_t register_address);
} HAL_SPI;