
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
    .standby = sx1278_standby,
    .reset = sx1278_reset};

/*
 * Function: sx1278_set_frequency
 * Description: Sets the transmission frequency for the SX1278 device
 * Parameters: self - Pointer to the SX1278 device instance
 *            freq - Frequency in Hz to set
 * Returns: 0 on success, -1 on error
 * Side Effects: Changes device transmission frequency
 * Assumptions: Device is properly initialized
 * Notes: Implementation not yet complete
 */
int sx1278_set_frequency(void *self, uint32_t freq)
{
    // TODO: Implementation here
    return 0;
}

/*
 * Function: sx1278_set_power
 * Description: Sets the transmission power level for the SX1278 device
 * Parameters: self - Pointer to the SX1278 device instance
 *            level - Power level to set
 * Returns: 0 on success, -1 on error
 * Side Effects: Changes device transmission power
 * Assumptions: Device is properly initialized
 * Notes: Implementation not yet complete
 */
int sx1278_set_power(void *self, int level)
{
    // TODO: Implementation here
    return 0;
}

/*
 * Function: sx1278_set_spreading_factor
 * Description: Sets the spreading factor for LoRa modulation
 * Parameters: self - Pointer to the SX1278 device instance
 *            sf - Spreading factor value (typically 6-12)
 * Returns: 0 on success, -1 on error
 * Side Effects: Changes LoRa modulation parameters
 * Assumptions: Device is properly initialized and in LoRa mode
 * Notes: Implementation not yet complete
 */
int sx1278_set_spreading_factor(void *self, int sf)
{
    // TODO: Implementation here
    return 0;
}

/*
 * Function: sx1278_set_syncword
 * Description: Sets the synchronization word for LoRa communication
 * Parameters: self - Pointer to the SX1278 device instance
 *            syncword - Synchronization word value
 * Returns: 0 on success, -1 on error
 * Side Effects: Changes device synchronization word register
 * Assumptions: Device is properly initialized and SPI communication is available
 * Notes: None
 */
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

/*
 * Function: sx1278_send
 * Description: Transmits data using the SX1278 LoRa device
 * Parameters: self - Pointer to the SX1278 device instance
 *            tx_data - Pointer to data buffer to transmit
 *            len - Length of data to transmit (max 128 bytes)
 * Returns: 0 on success, -1 on error
 * Side Effects: Sends data over LoRa radio and waits for transmission complete
 * Assumptions: Device is properly initialized and configured
 * Notes: Maximum payload length is limited to 128 bytes
 */
int sx1278_send(void *self, const uint8_t *tx_data, int len)
{
    sx1278_Device *device = (sx1278_Device *)self;
    int spi_handle = device->spi_handle;
    set_stdby_mode(spi_handle);
    SX1278Data data;
    data.address = REG_FIFO_TX_BASE_ADDR;
    data.write = 1;
    uint8_t address = 0x80;
    data.data_transmit = &address;
    data.transmit_length = 1;
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
        fprintf(stdout, "unable to set FIFO Address Pointer to TX Base Address\n");
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
    // Clear TX_DONE flag
    data.address = REG_IRQ_FLAGS;
    data.write = 1;
    uint8_t clear_tx_done = IRQ_FLAG_TX_DONE;
    data.data_transmit = &clear_tx_done;
    data.transmit_length = 1;
    write_sx1278(spi_handle, &data);
    return 0;
}

/*
 * Function: sx1278_receive
 * Description: Receives data from the SX1278 LoRa device with timeout
 * Parameters: self - Pointer to the SX1278 device instance
 *            buffer - Buffer to store received data
 *            max_len - Maximum length of data to receive
 * Returns: Number of bytes received on success, -1 on error or timeout
 * Side Effects: Enters continuous RX mode and waits for data reception
 * Assumptions: Device is properly initialized and configured
 * Notes: Has 10-second timeout to prevent infinite blocking
 */
int sx1278_receive(void *self, uint8_t *buffer, int max_len)
{
    // REFACTOR!!
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
        return -1;
    }
    data.address = REG_FIFO_RX_BASE_ADDR;
    data.write = 0;
    uint8_t rx_base_addr;
    data.data_receive = &rx_base_addr;
    data.receive_length = 1;
    if (read_sx1278(spi_handle, &data) < 0)
    {
        fprintf(stderr, "Failed to read FIFO RX Base Address\n");
        return -1;
    }
    // Clear IRQ flags first (like in working rx.c)
    data.address = REG_IRQ_FLAGS;
    data.write = 1;
    uint8_t clear = 0xFF;
    data.data_transmit = &clear;
    data.transmit_length = 1;
    write_sx1278(spi_handle, &data);

    // Setup for reading IRQ flags
    data.address = REG_IRQ_FLAGS;
    data.write = 0;
    uint8_t irq_flags = 0;
    data.data_receive = &irq_flags;
    data.receive_length = 1;

    set_rx_continuous_mode(spi_handle);
    read_sx1278(spi_handle, &data);

    // Add timeout to prevent infinite loop (10 seconds max)
    int timeout_count = 0;
    int max_timeout = 10000; // 10 seconds in milliseconds

    while ((irq_flags & IRQ_FLAG_RX_DONE) == 0)
    {
        read_sx1278(spi_handle, &data);
        usleep(1000); // 1ms
        timeout_count++;

        if (timeout_count >= max_timeout)
        {
            fprintf(stderr, "RX timeout - no data received within 10 seconds\n");
            set_stdby_mode(spi_handle);
            return -1;
        }
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
        return -1;
    }
    data.address = REG_FIFO;
    data.write = 0;
    uint8_t *received_data = buffer;
    data.data_receive = received_data;
    data.receive_length = (num_bytes < max_len) ? num_bytes : max_len;
    if (read_sx1278(spi_handle, &data) < 0)
    {
        fprintf(stderr, "Failed to read received data\n");
        return -1;
    }
    return (num_bytes < max_len) ? num_bytes : max_len;
}

/*
 * Function: sx1278_sleep
 * Description: Puts the SX1278 device into sleep mode for power saving
 * Parameters: self - Pointer to the SX1278 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Device enters sleep mode with minimal power consumption
 * Assumptions: Device is properly initialized and SPI communication is available
 * Notes: Device must be woken up before further operations
 */
int sx1278_sleep(void *self)
{
    sx1278_Device *device = (sx1278_Device *)self;
    int spi_handle = device->spi_handle;
    int m = set_stdby_mode(spi_handle);
    if (m < 0)
    {
        fprintf(stderr, "Failed to set standby mode\n");
        return m;
    }
    int n = set_sleep_mode(spi_handle);
    if (n < 0)
    {
        fprintf(stderr, "Failed to set sleep mode\n");
        return n;
    }
    return 0;
}

/*
 * Function: sx1278_standby
 * Description: Puts the SX1278 device into standby mode
 * Parameters: self - Pointer to the SX1278 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Device enters standby mode ready for configuration changes
 * Assumptions: Device is properly initialized and SPI communication is available
 * Notes: Standby mode is required before changing device settings
 */
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

/*
 * Function: sx1278_init
 * Description: Initializes the SX1278 device for LoRa operation
 * Parameters: self - Pointer to the SX1278 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Activates LoRa mode and puts device in standby
 * Assumptions: Device is properly connected via SPI
 * Notes: Must be called before any other device operations
 */
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
    if (n < 0)
    {
        fprintf(stderr, "Failed to set standby mode\n");
        return n;
    }
    return 0;
}

/*
 * Function: sx1278_close
 * Description: Closes the SX1278 device and puts it into sleep mode
 * Parameters: self - Pointer to the SX1278 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Device enters sleep mode for power saving
 * Assumptions: Device is properly initialized
 * Notes: Should be called when device is no longer needed
 */
int sx1278_close(void *self)
{
    sx1278_Device *device = (sx1278_Device *)self;
    int n = device->vtable->sleep(self);
    if (n < 0)
    {
        fprintf(stdout, "Failed to set sleep mode\n");
        return n;
    }
    return 0;
}

/*
 * Function: sx1278_reset
 * Description: Resets the SX1278 device and reinitializes it
 * Parameters: self - Pointer to the SX1278 device instance
 * Returns: 0 on success, -1 on error
 * Side Effects: Device is reset and returned to standby mode
 * Assumptions: Device is properly connected via SPI
 * Notes: Includes necessary delays for proper reset sequence
 */
int sx1278_reset(void *self)
{
    sx1278_Device *device = (sx1278_Device *)self;
    activate_lora(device->spi_handle);
    usleep(10000);
    reset_sx1278(device->spi_handle);
    usleep(10000);
    set_stdby_mode(device->spi_handle);
    return 0;
}

/*
 * Function: create_sx1278_device
 * Description: Creates and initializes an SX1278 device instance
 * Parameters: spi_handle - SPI handle for communication with the device
 * Returns: Initialized sx1278_Device structure
 * Side Effects: Device is initialized and ready for operation
 * Assumptions: SPI interface is properly configured
 * Notes: Automatically calls init function during creation
 */
sx1278_Device create_sx1278_device(int spi_handle)
{
    sx1278_Device device;
    device.vtable = &sx1278_lora_interface;
    device.spi_handle = spi_handle;
    device.vtable->init(&device);
    return device;
}