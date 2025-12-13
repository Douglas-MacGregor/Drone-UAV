
#include "sx1278_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "sx1278_device.h"
#include <unistd.h>

// functions to implement LoRaInterface

LoRaInterface sx1278_lora_interface = {
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

int sx1278_set_frequency(void *self, uint32_t freq)
{
    // TODO: Implementation here
    return 0;
}

int sx1278_set_power(void *self, int level)
{
    // TODO: Implementation here
    return 0;
}

int sx1278_set_spreading_factor(void *self, int sf)
{
    // TODO: Implementation here
    return 0;
}

int sx1278_set_syncword(void *self, uint8_t syncword)
{
    sx1278_Device *device = (sx1278_Device *)self;
    int spi_handle = device->spi_handle;
    SX1278Data data;
    data.address = REG_SYNC_WORD;
    data.write = 1;
    data.data_transmit = &syncword;
    data.transmit_length = 1;
    int n = write_sx1278(spi_handle, &data);
    if (n < 0)
    {
        fprintf(stderr, "Failed to set syncword\n");
        return n;
    }
    return 0;
}

int sx1278_send(void *self, const uint8_t *tx_data, int len)
{
    sx1278_Device *device = (sx1278_Device *)self;
    int spi_handle = device->spi_handle;
    set_stdby_mode(spi_handle);
    SX1278Data data;
    data.address = REG_FIFO_TX_BASE_ADDR;
    data.write = 0;
    uint8_t address = 0x80;
    data.data_transmit = &address;
    data.receive_length = 1;
    if (write_sx1278(spi_handle, &data) < 0)
    {
        fprintf(stdout, "unable to set FIFO TX Base Address: 0x%02X\n", address);
        return -1;
    }
    data.address = REG_FIFO_ADDR_PTR;
    data.write = 1;
    uint8_t fifo_ptr = address;
    data.data_transmit = &fifo_ptr;
    data.transmit_length = 1;
    if (write_sx1278(spi_handle, &data) < 0)
    {
        fprintf(stdout, "unable too set FIFO Address Pointer to TX Base Address\n");
        return -1;
    }
    data.address = REG_FIFO;
    data.write = 1;
    data.data_transmit = (uint8_t *)tx_data;
    data.transmit_length = (len > 128) ? 128 : len;
    if (write_sx1278(spi_handle, &data) < 0)
    {
        fprintf(stderr, "unable to write data to FIFO\n");
        return -1;
    }
    data.address = REG_PAYLOAD_LENGTH;
    data.write = 1;
    uint8_t payload_length = (len > 128) ? 128 : len;
    data.data_transmit = &payload_length;
    data.transmit_length = 1;
    if (write_sx1278(spi_handle, &data) < 0)
    {
        fprintf(stderr, "unable to set payload length to %d\n", payload_length);
        return -1;
    }
    data.address = REG_IRQ_FLAGS;
    data.write = 1;
    uint8_t irq_flags = 0;
    uint8_t clear = 0xFF;
    data.data_transmit = &clear;
    data.transmit_length = 1;
    write_sx1278(spi_handle, &data);
    data.write = 0;
    data.data_receive = &irq_flags;
    data.receive_length = 1;
    set_tx_mode(spi_handle);
    read_sx1278(spi_handle, &data);
    while ((irq_flags & IRQ_FLAG_TX_DONE) == 0)
    {
        read_sx1278(spi_handle, &data);
    }
    set_stdby_mode(spi_handle);
    return 0;
}

int sx1278_receive(void *self, uint8_t *buffer, int max_len)
{
    sx1278_Device *device = (sx1278_Device *)self;
    int spi_handle = device->spi_handle;
    set_stdby_mode(spi_handle);
    SX1278Data data;
    data.address = REG_FIFO_ADDR_PTR;
    data.write = 1;
    uint8_t address = 0x00;
    data.data_transmit = &address;
    data.transmit_length = 1;
    if (write_sx1278(spi_handle, &data) < 0)
    {
        fprintf(stderr, "Failed to set Address Pointer to 0x00\n");
    }
    data.address = REG_FIFO_RX_BASE_ADDR;
    if (read_sx1278(spi_handle, &data) < 0)
    {
        fprintf(stderr, "Failed to set FIFO RX Base Address: 0x%02X\n", address);
    }
    data.address = REG_IRQ_FLAGS;
    data.write = 0;
    uint8_t irq_flags = 0;
    uint8_t clear = 0xFF;
    data.write = 1;
    data.data_transmit = &clear;
    data.transmit_length = 1;
    write_sx1278(spi_handle, &data);
    data.write = 0;
    data.data_receive = &irq_flags;
    data.receive_length = 1;
    set_rx_continuous_mode(spi_handle);
    read_sx1278(spi_handle, &data);
    while ((irq_flags & IRQ_FLAG_RX_DONE) == 0)
    {
        read_sx1278(spi_handle, &data);
        usleep(1000);
    }
    set_stdby_mode(spi_handle);
    fprintf(stdout, "Data received!\n");
    data.address = REG_RX_NB_BYTES;
    data.write = 0;
    uint8_t num_bytes;
    data.data_receive = &num_bytes;
    data.receive_length = 1;
    if (read_sx1278(spi_handle, &data) < 0)
    {
        fprintf(stderr, "Unable to read number of bytes\n");
    }
    data.address = REG_FIFO;
    data.write = 0;
    uint8_t *received_data = buffer;
    data.data_receive = received_data;
    data.receive_length = (num_bytes < max_len) ? num_bytes : max_len;
    if (read_sx1278(spi_handle, &data) < 0)
    {
        fprintf(stderr, "Failed to read received data\n");
    }
    return 0;
}

int sx1278_sleep(void *self)
{
    sx1278_Device *device = (sx1278_Device *)self;
    int spi_handle = device->spi_handle;
    int n = set_sleep_mode(spi_handle);
    if (n < 0)
    {
        fprintf(stderr, "Failed to set sleep mode\n");
        return n;
    }
    return 0;
}

int sx1278_standby(void *self)
{
    sx1278_Device *device = (sx1278_Device *)self;
    int spi_handle = device->spi_handle;
    int n = set_stdby_mode(spi_handle);
    if (n < 0)
    {
        fprintf(stderr, "Failed to set standby mode\n");
        return n;
    }
    return 0;
}

int sx1278_init(void *self)
{
    sx1278_Device *device = (sx1278_Device *)self;
    int spi_handle = device->spi_handle;
    int n = 0;
    n = activate_lora(spi_handle);
    if (n < 0)
    {
        fprintf(stderr, "Failed to activate LoRa mode\n");
        return n;
    }
    n = set_stdby_mode(spi_handle);
    {
        fprintf(stderr, "Failed to set standby mode\n");
        return n;
    }
    fprintf(stderr, "SX1278 initialized successfully\n");
    return 0;
}

int sx1278_close(void *self)
{
    // sx1278_Device *device = (sx1278_Device *)self;
    fprintf(stdout, "Closing SX1278\n");
    return 0;
}

int sx1278_reset(void *self)
{
    sx1278_Device *device = (sx1278_Device *)self;
    activate_lora(device->spi_handle);
    reset_sx1278(device->spi_handle);
    usleep(10000);
    set_stdby_mode(device->spi_handle);
    return 0;
}

sx1278_Device create_sx1278_device(int spi_handle)
{
    sx1278_Device device;
    device.vtable = &sx1278_lora_interface;
    device.spi_handle = spi_handle;
    device.vtable->init(&device);
    return device;
}