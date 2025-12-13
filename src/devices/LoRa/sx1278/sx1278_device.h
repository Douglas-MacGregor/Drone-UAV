#ifndef SX1278_DEVICE_H
#define SX1278_DEVICE_H
#include <stdint.h>
#include "../lora_interface.h"
#include "sx1278_utils.h"

typedef struct
{
    LoRaInterface *vtable; // pointer to interface
    int spi_handle;        // SPI handle for communication
} sx1278_Device;

extern LoRaInterface sx1278_lora_interface;

// functions to implement LoRaInterface
int sx1278_set_frequency(void *self, uint32_t freq);
int sx1278_set_power(void *self, int level);
int sx1278_set_spreading_factor(void *self, int sf);
int sx1278_set_syncword(void *self, uint8_t syncword);
int sx1278_send(void *self, const uint8_t *data, int len);
int sx1278_receive(void *self, uint8_t *buffer, int max_len);
int sx1278_sleep(void *self);
int sx1278_standby(void *self);
int sx1278_init(void *self);
int sx1278_close(void *self);
int sx1278_reset(void *self);

sx1278_Device create_sx1278_device(int spi_handle);
#endif // SX1278_DEVICE_H