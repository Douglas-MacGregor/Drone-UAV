#include <stdint.h>
#include "../utils.h"
#include "sx1278/mySX1278.h"
#include <stdlib.h>

typedef struct
{
    int (*close)(void *self);
    int (*send)(void *self, const uint8_t *data, int len);
    int (*receive)(void *self, uint8_t *buffer, int max_len);
    int (*set_syncword)(void *self, uint8_t syncword);
    int (*set_frequency)(void *self, uint32_t freq);
    int (*set_power)(void *self, int level);
    int (*set_spreading_factor)(void *self, int sf);
} LoRaInterface;

typedef struct
{
    LoRaInterface *vtable; // pointer to interface
    int spi_handle;        // SPI handle for communication
} LoRaDevice;
