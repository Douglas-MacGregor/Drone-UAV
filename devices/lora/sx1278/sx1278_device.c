#include "sx1278_utils.h"
#include "sx1278_device.h"
#include <stdlib.h>
#include "hal_spi.h"
#include "hal_gpio.h"
#include "lora_interface.h"
#include <stdio.h>

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
    sx1278_Device *device = (sx1278_Device *)self;
    device->vtable->reset(self);
    uint8_t opmode;
    if (read_sx1278(device->spi_handle, REG_OPMODE, &opmode, 1) < 0)
    {
        fprintf(stderr, "Failed to read OP_MODE register during init\n");
        return -1;
    }
    if ((opmode & OPMODE_LONG_RANGE) == 0)
    {
        device->vtable->sleep(self);
        if (read_sx1278(device->spi_handle, REG_OPMODE, &opmode, 1) < 0)
        {
            fprintf(stderr, "Failed to read OP_MODE register after sleep\n");
            return -1;
        }
        opmode |= OPMODE_LONG_RANGE;
        if (write_sx1278(device->spi_handle, REG_OPMODE, &opmode, 1) < 0)
        {
            fprintf(stderr, "Failed to set Long Range mode\n");
            return -1;
        }
        if (poll_register_sx1278(device->spi_handle, REG_OPMODE, 0x80, 0x80, 10, 1000) < 0)
        {
            fprintf(stderr, "Timeout waiting for Long Range mode to set\n");
            return -1;
        }
    }
    device->vtable->standby(self);
    return 0;
}

int sx1278_close(void *self)
{
    sx1278_Device *device = (sx1278_Device *)self;
    return 0;
}

int sx1278_send(void *self, const uint8_t *data, int len)
{
    sx1278_Device *device = (sx1278_Device *)self;
    if (len <= 0 || len > 255)
    {
        fprintf(stderr, "Invalid data length for send: %d\n", len);
        return -1;
    }
    if (write_sx1278(device->spi_handle, REG_PAYLOAD_LENGTH, (const uint8_t *)&len, 1) < 0)
    {
        fprintf(stderr, "Failed to set payload length\n");
        return -1;
    }
    if (write_sx1278(device->spi_handle, REG_FIFO_ADDR_PTR, (const uint8_t[]){0x00}, 1) < 0)
    {
        fprintf(stderr, "Failed to set FIFO address pointer\n");
        return -1;
    }
    if (write_sx1278(device->spi_handle, REG_FIFO_TX_BASE_ADDR, (const uint8_t[]){0x00}, 1) < 0)
    {
        fprintf(stderr, "Failed to move tx base ptr\n");
        return -1;
    }
    if (write_sx1278(device->spi_handle, REG_FIFO, data, len) < 0)
    {
        fprintf(stderr, "Failed to write data to FIFO\n");
        return -1;
    }
    uint8_t opmode;
    uint8_t irq_masks;

    if (read_sx1278(device->spi_handle, REG_IRQ_FLAGS_MASKS, &irq_masks, 1) < 0)
    {
        fprintf(stderr, "Failed to read REG_IRQ_FLAGS_MASKS register before transmit\n");
        return -1;
    }
    irq_masks &= ~(IRQ_MASK_TX_DONE); // Enable TxDone interrupt
    if (write_sx1278(device->spi_handle, REG_IRQ_FLAGS_MASKS, &irq_masks, 1) < 0)
    {
        fprintf(stderr, "Failed to write REG_IRQ_FLAGS_MASKS register before transmit\n");
        return -1;
    }
    if (write(device->spi_handle, REG_IRQ_FLAGS, (const uint8_t[]){0xFF}, 1) < 0)
    {
        fprintf(stderr, "Failed to clear REG_IRQ_FLAGS\n");
        return -1;
    }
    if (read_sx1278(device->spi_handle, REG_OPMODE, &opmode, 1) < 0)
    {
        fprintf(stderr, "Failed to read OP_MODE register before transmit\n");
        return -1;
    }
    opmode &= ~0b00000111;
    opmode |= OPMODE_TX;
    if (write_sx1278(device->spi_handle, REG_OPMODE, &opmode, 1) < 0)
    {
        fprintf(stderr, "Failed to set TX mode\n");
        return -1;
    }
    if (poll_register_sx1278(device->spi_handle, REG_IRQ_FLAGS, IRQ_MASK_TX_DONE, IRQ_MASK_TX_DONE, 100, 10000) < 0)
    {
        fprintf(stderr, "Timeout waiting for TX_DONE\n");
        return -2;
    }
    if (write_sx1278(device->spi_handle, REG_IRQ_FLAGS, (const uint8_t[]){IRQ_MASK_TX_DONE}, 1) < 0)
    {
        fprintf(stderr, "Failed to clear TX_DONE flag\n");
        return -1;
    }
    device->vtable->standby(self);
    return len;
}

int sx1278_receive(void *self, uint8_t *buffer, int len)
{
    sx1278_Device *device = (sx1278_Device *)self;
    if (len <= 0 || len > 255)
    {
        fprintf(stderr, "Invalid buffer length for receive: %d\n", len);
        return -1;
    }
    uint8_t opmode;
    uint8_t irq_masks;
    if (read_sx1278(device->spi_handle, REG_IRQ_FLAGS_MASKS, &irq_masks, 1) < 0)
    {
        fprintf(stderr, "Failed to read REG_IRQ_FLAGS_MASKS register before receive\n");
        return -1;
    }
    irq_masks &= ~(IRQ_MASK_RX_DONE | IRQ_MASK_PAYLOAD_CRC_ERROR); //
    if (write_sx1278(device->spi_handle, REG_IRQ_FLAGS_MASKS, &irq_masks, 1) < 0)
    {
        fprintf(stderr, "Failed to write REG_IRQ_FLAGS_MASKS register before receive\n");
        return -1;
    }
    if (write_sx1278(device->spi_handle, REG_IRQ_FLAGS, (const uint8_t[]){0xFF}, 1) < 0)
    {
        fprintf(stderr, "Failed to clear REG_IRQ_FLAGS before receive\n");
        return -1;
    }
    if (write_sx1278(device->spi_handle, REG_FIFO_ADDR_PTR, (const uint8_t[]){0x00}, 1) < 0)
    {
        fprintf(stderr, "Failed to set FIFO address pointer before receive\n");
        return -1;
    }
    if (read_sx1278(device->spi_handle, REG_OPMODE, &opmode, 1) < 0)
    {
        fprintf(stderr, "Failed to read OP_MODE register before receive\n");
        return -1;
    }
    opmode &= ~0b00000111;
    opmode |= OPMODE_RX_CONTINUOUS;
    if (write_sx1278(device->spi_handle, REG_OPMODE, &opmode, 1) < 0)
    {
        fprintf(stderr, "Failed to set RX_CONTINUOUS mode\n");
        return -1;
    }
    if (poll_register_sx1278(device->spi_handle, REG_IRQ_FLAGS, IRQ_MASK_RX_DONE, IRQ_MASK_RX_DONE, 1000, 10000) < 0)
    {
        fprintf(stderr, "Timeout waiting for RX_DONE\n");
        return -2;
    }
    if (write_sx1278(device->spi_handle, REG_IRQ_FLAGS, (const uint8_t[]){IRQ_MASK_RX_DONE | IRQ_MASK_PAYLOAD_CRC_ERROR}, 1) < 0)
    {
        fprintf(stderr, "Failed to clear RX_DONE and PAYLOAD_CRC_ERROR flags\n");
        return -1;
    }
    uint8_t num_bytes;
    if (read_sx1278(device->spi_handle, REG_RX_NB_BYTES, &num_bytes, 1) < 0)
    {
        fprintf(stderr, "Failed to read number of received bytes\n");
        return -1;
    }
    if (num_bytes > len)
    {
        fprintf(stderr, "Received data length %d exceeds buffer size %d\n", num_bytes, len);
        return -3;
    }
    if (write_sx1278(device->spi_handle, REG_FIFO_ADDR_PTR, (const uint8_t[]){0x00}, 1) < 0)
    {
        fprintf(stderr, "Failed to set FIFO address pointer before reading received data\n");
        return -1;
    }
    if (read_sx1278(device->spi_handle, REG_FIFO, buffer, num_bytes) < 0)
    {
        fprintf(stderr, "Failed to read received data from FIFO\n");
        return -1;
    }
    device->vtable->standby(self);
    return num_bytes;
}

int sx1278_set_syncword(void *self, uint8_t syncword)
{
    sx1278_Device *device = (sx1278_Device *)self;
    return write_sx1278(device->spi_handle, REG_SYNC_WORD, &syncword, 1);
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
    uint8_t current_opmode;
    sx1278_Device *device = (sx1278_Device *)self;
    if (read_sx1278(device->spi_handle, REG_OPMODE, &current_opmode, 1) < 0)
    {
        fprintf(stderr, "Failed to read OP_MODE register\n");
        return -1;
    }
    current_opmode &= ~0b00000111;
    current_opmode |= OPMODE_SLEEP;
    if (write_sx1278(device->spi_handle, REG_OPMODE, &current_opmode, 1) < 0)
    {
        fprintf(stderr, "Failed to write OP_MODE register to set sleep mode\n");
        return -1;
    }
    if (poll_register_sx1278(device->spi_handle, REG_OPMODE, 0b00000111, OPMODE_SLEEP, 10, 1000) < 0)
    {
        fprintf(stderr, "Timeout waiting for sleep mode to set\n");
        return -1;
    }
    return 0;
}

int sx1278_standby(void *self)
{
    uint8_t current_opmode;
    sx1278_Device *device = (sx1278_Device *)self;
    if (read_sx1278(device->spi_handle, REG_OPMODE, &current_opmode, 1) < 0)
    {
        fprintf(stderr, "Failed to read OP_MODE register\n");
        return -1;
    }
    current_opmode &= ~0b00000111;
    current_opmode |= OPMODE_STDBY;
    if (write_sx1278(device->spi_handle, REG_OPMODE, &current_opmode, 1) < 0)
    {
        fprintf(stderr, "Failed to write OP_MODE register to set standby mode\n");
        return -1;
    }
    if (poll_register_sx1278(device->spi_handle, REG_OPMODE, 0b00000111, OPMODE_STDBY, 10, 1000) < 0)
    {
        fprintf(stderr, "Timeout waiting for standby mode to set\n");
        return -1;
    }
    return 0;
}

int sx1278_reset(void *self)
{
    return reset_sx1278(((sx1278_Device *)self)->gpio_reset_pin);
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