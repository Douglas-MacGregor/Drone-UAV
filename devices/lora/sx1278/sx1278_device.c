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

/*
 * Function: sx1278_init
 * Description: Initializes the SX1278 device by resetting it and configuring Long Range mode
 * Parameters: self - Pointer to the SX1278 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Leaves device in standby mode
 * Assumptions: Device is properly connected via SPI and reset pin is functional
 * Notes: None
 */

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

/*
 * Function: sx1278_close
 * Description: Clean up operations for the SX1278 device if necessary
 * Parameters: self - Pointer to the SX1278 device instance
 * Returns: 0 on success
 * Side Effects: None currently implemented
 * Assumptions: Device has been previously initialized
 */
int sx1278_close(void *self)
{
    sx1278_Device *device = (sx1278_Device *)self;
    return 0;
}

/*
 * Function: sx1278_send
 * Description: Sends data using the SX1278 device
 * Parameters: self - Pointer to the SX1278 device instance
 *            data - Pointer to the data buffer to send
 *            len - Length of the data to send
 * Returns: Number of bytes sent on success, -1 on error, -2 on timeout
 * Side Effects: Transmits data over LoRa
 * Assumptions: Device is properly initialized and in standby mode
 */

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
    if (write_sx1278(device->spi_handle, REG_IRQ_FLAGS, (const uint8_t[]){0xFF}, 1) < 0)
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
    if (poll_register_sx1278(device->spi_handle, REG_IRQ_FLAGS, IRQ_FLAG_TX_DONE, IRQ_FLAG_TX_DONE, 100, 10000) < 0)
    {
        fprintf(stderr, "Timeout waiting for TX_DONE\n");
        return -2;
    }
    if (write_sx1278(device->spi_handle, REG_IRQ_FLAGS, (const uint8_t[]){IRQ_FLAG_TX_DONE}, 1) < 0)
    {
        fprintf(stderr, "Failed to clear TX_DONE flag\n");
        return -1;
    }
    device->vtable->standby(self);
    return len;
}

/*
 * Function: sx1278_receive
 * Description: Receives data using the SX1278 device
 * Parameters: self - Pointer to the SX1278 device instance
 *            buffer - Pointer to the buffer to store received data
 *            len - Length of the buffer
 * Returns: Number of bytes received on success, -1 on error, -2 on timeout, -3 if buffer too small
 * Side Effects: Receives data over LoRa
 * Assumptions: Device is properly initialized and in standby mode
 */

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
        device->vtable->standby(self); // Return to standby even on timeout
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

/*
 * Function: sx1278_set_syncword
 * Description: Sets the sync word for the SX1278 device
 * Parameters: self - Pointer to the SX1278 device instance
 *            syncword - Sync word value to set
 * Returns: 0 on success, -1 on error
 * Side Effects: Updates the sync word register
 * Assumptions: Device is properly initialized
 * Notes: None
 */

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

/*
 * Function: sx1278_sleep
 * Description: Puts the SX1278 device into sleep mode
 * Parameters: self - Pointer to the SX1278 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Device enters sleep mode
 * Assumptions: Device is properly initialized
 * Notes: None
 */

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

/*
 * Function: sx1278_standby
 * Description: Puts the SX1278 device into standby mode
 * Parameters: self - Pointer to the SX1278 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Device enters standby mode
 * Assumptions: Device is properly initialized
 * Notes: None
 */

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

/*
 * Function: sx1278_reset
 * Description: Resets the SX1278 device
 * Parameters: self - Pointer to the SX1278 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Device is reset and requires reconfiguration
 * Assumptions: Device is properly connected via SPI and reset pin is functional
 * Notes: Leaves device in unspecified state
 */

int sx1278_reset(void *self)
{
    return reset_sx1278(((sx1278_Device *)self)->gpio_reset_pin);
}

/*
 * Function: create_sx1278_device
 * Description: Creates and initializes an SX1278 device instance
 * Parameters: spi_handle - SPI handle for communication
 *            gpio_reset_pin - GPIO pin number for reset control
 * Returns: Initialized sx1278_Device instance
 * Side Effects: None
 * Assumptions: SPI and GPIO subsystems are properly initialized
 * Notes: None
 */

sx1278_Device create_sx1278_device(int spi_handle, int gpio_reset_pin)
{
    sx1278_Device device;
    device.vtable = &sx1278_lora_interface;
    device.spi_handle = spi_handle;
    device.gpio_reset_pin = gpio_reset_pin;
    return device;
}

/*
 * Function: destroy_sx1278_device
 * Description: Cleans up the SX1278 device instance
 * Parameters: device - Pointer to the sx1278_Device instance
 * Returns: 0 on success
 * Side Effects: None currently implemented
 * Assumptions: Device has been previously created
 * Notes: None
 */

int destroy_sx1278_device(sx1278_Device *device)
{
    // No dynamic memory to free in this implementation
    return 0;
}