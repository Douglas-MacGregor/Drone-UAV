#include "sx1278_utils.h"
#include "sx1278_device.h"
#include <stdlib.h>
#include "hal_spi.h"
#include "hal_gpio.h"
#include "lora_interface.h"

LoRaInterface sx1278_lora_interface = {
    .init = sx1278_init,
    .close = sx1278_close,
    .send = sx1278_send,
    .receive = sx1278_receive,
    .set_syncword = sx1278_set_syncword,
    .set_frequency = sx1278_set_frequency,
    .set_power = sx1278_set_power,
    .set_spreading_factor = sx1278_set_spreading_factor,
    .sleep = sx1278_sleep,
    .standby = sx1278_standby,
    .reset = sx1278_reset};

int sx1278_init(void *self)
{
    return 0;
}

int sx1278_close(void *self)
{
    return 0;
}

int sx1278_send(void *self, const uint8_t *data, int len)
{
    return 0;
}

int sx1278_receive(void *self, uint8_t *buffer, int len)
{
    return 0;
}

int sx1278_set_syncword(void *self, uint8_t syncword)
{
    return 0;
}

int sx1278_set_frequency(void *self, uint32_t freq)
{
    return 0;
}

int sx1278_set_power(void *self, int level)
{
    return 0;
}

int sx1278_set_spreading_factor(void *self, int sf)
{
    return 0;
}

int sx1278_sleep(void *self)
{
    return 0;
}

int sx1278_standby(void *self)
{
    return 0;
}

int sx1278_reset(void *self)
{
    return 0;
}

sx1278_Device create_sx1278_device(int spi_handle, int gpio_reset_pin)
{
    sx1278_Device device;
    device.vtable = &sx1278_lora_interface;
    device.spi_handle = spi_handle;
    return device;
}

int destroy_sx1278_device(sx1278_Device *device)
{
    // No dynamic memory to free in this implementation
    return 0;
}