#include <stdint.h>

typedef struct
{
    int (*init_i2c)(void *self, uint8_t I2C_BUS, uint8_t I2C_ADDRESS);
    int (*close_i2c)(void *self, int i2c_handle);
    int (*write_i2c)(void *self, uint8_t *tx_buffer, int length, uint8_t register_address);
    int (*read_i2c)(void *self, uint8_t *rx_buffer, int length, uint8_t register_address);
} HAL_I2C;