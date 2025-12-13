#include <stdint.h>
#include "../lora_interface.h"
#include "sx1278_utils.h"

typedef struct
{
    LoRaInterface *vtable; // pointer to interface
    int spi_handle;        // SPI handle for communication
} sx1278_Device;

LoRaInterface sx1278_vtable = {
    .init = sx1278_init,
    .close = sx1278_close,
    .send = sx1278_send,
    .receive = sx1278_receive,
    .set_frequency = sx1278_set_frequency,
    .set_power = sx1278_set_power,
    .set_spreading_factor = sx1278_set_spreading_factor,
    .set_syncword = sx1278_set_syncword,
    .sleep = sx1278_sleep,
    .standby = sx1278_sleep,
    .reset = sx1278_standby};

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

sx1278_Device create_sx1278_device(int spi_handle);
