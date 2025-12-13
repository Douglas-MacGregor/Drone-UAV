#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "sx1278_utils.h"
#include "../../utils.h"
#include <stdlib.h>
#include <pigpio.h>

// lower level functions for SX1278 operation

int init_sx1278()
{
    int spi_handle = init_spi();
    if (spi_handle < 0)
    {
        fprintf(stdout, "spi init fail\n");
        return -1;
    }
    // Wait for SX1278 to stabilize
    deactivate_lora(spi_handle);
    set_stdby_mode(spi_handle);
    usleep(1000);
    reset_sx1278(spi_handle);
    usleep(1000);
    return spi_handle;
}

void reset_sx1278(int spi_handle)
{
    gpioSetMode(PIN_RESET, PI_OUTPUT);
    gpioWrite(PIN_RESET, 1);
    usleep(1000);

    gpioWrite(PIN_RESET, 0);
    usleep(1000);

    gpioSetMode(PIN_RESET, PI_INPUT);
    usleep(10000);
    return;
}

void close_sx1278(int spi_handle)
{
    close_spi(spi_handle);
}

int read_sx1278(int spi_handle, SX1278Data *data)
{
    uint8_t *transmit_buffer = malloc(data->receive_length + 1);
    memset(transmit_buffer, 0, data->receive_length + 1);
    transmit_buffer[0] = data->address & ~(0b10000000);
    uint8_t *receive_buffer = malloc(data->receive_length + 1);
    memset(receive_buffer, 0, data->receive_length + 1);
    int n = spiXfer(spi_handle, (char *)transmit_buffer, (char *)receive_buffer, data->receive_length + 1);
    memccpy(data->data_receive, &receive_buffer[1], data->receive_length, data->receive_length);
    free(receive_buffer);
    free(transmit_buffer);
    if (n != data->receive_length + 1)
    {
        fprintf(stdout, "SPI read error\n");
        return -1;
    }
    else
    {
        return n;
    }
}

int write_sx1278(int spi_handle, SX1278Data *data)
{
    uint8_t *transmit_buffer = malloc(data->transmit_length + 1);
    memset(transmit_buffer, 0, data->transmit_length + 1);
    transmit_buffer[0] = data->address | 0b10000000;
    memcpy(&transmit_buffer[1], data->data_transmit, data->transmit_length);
    int n = spiXfer(spi_handle, (char *)transmit_buffer, NULL, data->transmit_length + 1);
    free(transmit_buffer);
    if (n != data->transmit_length + 1)
    {
        fprintf(stdout, "SPI write error\n");
        return -1;
    }
    else
    {
        return n;
    }
}

int activate_lora(int spi_handle)
{
    int n = set_sleep_mode(spi_handle);
    if (n < 0)
    {
        fprintf(stdout, "Failed to set sleep mode\n");
        return n;
    }
    SX1278Data data;
    data.address = REG_OPMODE;
    data.write = 0;
    uint8_t mode; // Set LoRa bit
    data.data_receive = &mode;
    data.receive_length = 1;
    n = read_sx1278(spi_handle, &data);
    if (n < 0)
    {
        return n;
    }
    if ((mode & (0b111)) != 0)
    {
        fprintf(stdout, "Warning: SX1278 not in sleep mode before activating LoRa\n");
    }
    mode |= 0b10000000; // Set LoRa bit
    data.write = 1;
    data.data_transmit = &mode;
    data.transmit_length = 1;
    n = write_sx1278(spi_handle, &data);
    if (n < 0)
    {
        return n;
    }
    n = poll_reg(spi_handle, REG_OPMODE, (uint8_t)0x80, mode, 1000, 1000);
    return n;
}

int deactivate_lora(int spi_handle)
{
    int n = set_sleep_mode(spi_handle);
    if (n < 0)
    {
        fprintf(stdout, "Failed to set sleep mode\n");
        return n;
    }
    SX1278Data data;
    data.address = REG_OPMODE;
    data.write = 0;
    uint8_t mode;
    data.data_receive = &mode;
    data.receive_length = 1;
    n = read_sx1278(spi_handle, &data);
    if (n < 0)
    {
        return n;
    }
    if ((mode & (0b111)) != 0)
    {
        fprintf(stdout, "Warning: SX1278 not in sleep mode before activating LoRa\n");
    }
    mode &= ~(0b10000000); // Clear LoRa bit
    data.write = 1;
    data.data_transmit = &mode;
    data.transmit_length = 1;
    n = write_sx1278(spi_handle, &data);
    if (n < 0)
    {
        return n;
    }
    n = poll_reg(spi_handle, REG_OPMODE, (uint8_t)0x80, mode, 1000, 1000);
    return n;
}

int set_tx_mode(int spi_handle)
{
    set_stdby_mode(spi_handle);
    SX1278Data data;
    data.address = REG_OPMODE;
    data.write = 1;
    uint8_t mode = OPMODE_DEFAULT & ~(0b111);
    mode |= OPMODE_TX; // TX mode
    data.data_transmit = &mode;
    data.transmit_length = 1;
    int n = write_sx1278(spi_handle, &data);
    return n;
}

int set_rx_continuous_mode(int spi_handle)
{
    set_stdby_mode(spi_handle);
    SX1278Data data;
    data.address = REG_OPMODE;
    data.write = 1;
    uint8_t mode = OPMODE_DEFAULT & ~(0b111);
    mode |= OPMODE_RX_CONTINUOUS; // RX continuous mode
    data.data_transmit = &mode;
    data.transmit_length = 1;
    int n = write_sx1278(spi_handle, &data);
    return n;
}

int set_rx_single_mode(int spi_handle)
{
    set_stdby_mode(spi_handle);
    SX1278Data data;
    data.address = REG_OPMODE;
    data.write = 1;
    uint8_t mode = OPMODE_DEFAULT & ~(0b111);
    mode |= OPMODE_RX_SINGLE; // RX single
    data.data_transmit = &mode;
    data.transmit_length = 1;
    int n = write_sx1278(spi_handle, &data);
    return n;
}

int set_sleep_mode(int spi_handle)
{
    SX1278Data data;
    data.address = REG_OPMODE;
    data.write = 0;
    uint8_t current_mode;
    data.data_receive = &current_mode;
    data.receive_length = 1;
    int n = read_sx1278(spi_handle, &data);
    if (n < 0)
    {
        fprintf(stdout, "Failed to read current mode\n");
        return n;
    }
    uint8_t mode = current_mode & ~(0b111);
    mode |= OPMODE_SLEEP; // Sleep mode
    data.data_transmit = &mode;
    data.transmit_length = 1;
    data.write = 1;
    n = write_sx1278(spi_handle, &data);
    if (n < 0)
    {
        fprintf(stdout, "Failed to write sleep mode\n");
        return n;
    }
    data.write = 0;
    n = read_sx1278(spi_handle, &data);
    if (n < 0)
    {
        fprintf(stdout, "Failed to read back mode\n");
        return n;
    }
    n = poll_reg(spi_handle, REG_OPMODE, (uint8_t)0x07, mode, 100, 1000);
    return n;
}

int set_stdby_mode(int spi_handle)
{
    SX1278Data data;
    data.address = REG_OPMODE;
    data.write = 0;
    uint8_t current_mode;
    data.data_receive = &current_mode;
    data.receive_length = 1;
    int n = read_sx1278(spi_handle, &data);
    if (n < 0)
    {
        return n;
    }
    uint8_t mode = (current_mode & ~(0b111));
    mode |= OPMODE_STDBY; // Standby mode
    data.address = REG_OPMODE;
    data.data_transmit = &mode;
    data.transmit_length = 1;
    data.write = 1;
    n = write_sx1278(spi_handle, &data);
    if (n < 0)
    {
        return n;
    }
    fprintf(stdout, "Mode %.2X\n", mode);
    n = poll_reg(spi_handle, REG_OPMODE, (uint8_t)0x07, mode, 1000, 1000);
    data.write = 0;
    int y = read_sx1278(spi_handle, &data);
    fprintf(stdout, "After poll, mode %.2X\n", current_mode);
    return n;
}

int rx_set_base_address(int spi_handle, uint8_t address)
{
    if (address > 0xFF)
    {
        fprintf(stdout, "address pointer out of range\n");
        return -1;
    }
    SX1278Data data;
    data.address = REG_FIFO_RX_BASE_ADDR;
    data.write = 1;
    data.data_transmit = &address;
    data.transmit_length = 1;
    int n = write_sx1278(spi_handle, &data);
    return n;
}

int tx_set_base_address(int spi_handle, uint8_t address)
{
    if (address > 0xFF)
    {
        fprintf(stdout, "address pointer out of range\n");
        return -1;
    }
    SX1278Data data;
    data.address = REG_FIFO_TX_BASE_ADDR;
    data.write = 1;
    data.data_transmit = &address;
    data.transmit_length = 1;
    int n = write_sx1278(spi_handle, &data);
    return n;
}

int spi_set_fifo_addr_ptr(int spi_handle, uint8_t address)
{
    if (address > 0xFF)
    {
        fprintf(stdout, "FIFO address pointer out of range\n");
        return -1;
    }
    SX1278Data data;
    data.address = REG_FIFO_ADDR_PTR; // REG_FIFO_ADDR_PTR
    data.write = 1;
    data.data_transmit = &address;
    data.transmit_length = 1;
    int n = write_sx1278(spi_handle, &data);
    return n;
}

int poll_reg(int spi_handle, uint8_t reg_address, uint8_t mask, uint8_t expected_value, int max_attempts, int delay_us)
{
    SX1278Data data;
    data.address = reg_address;
    data.write = 0;
    uint8_t reg_value;
    data.data_receive = &reg_value;
    data.receive_length = 1;
    int attempt = 0;
    if (max_attempts < 0)
    {
        max_attempts = 100; // Default max attempts
    }
    while (attempt < max_attempts)
    {

        int n = read_sx1278(spi_handle, &data);
        if (n < 0)
        {
            return -1; // Read error
        }

        if ((reg_value & mask) == (expected_value & mask))
        {
            return 0; // Success
        }
        usleep(delay_us);
        attempt++;
    }
    return -2; // Timeout
}

void print_reg_values(int spi_handle)
{
    SX1278Data data;
    data.write = 0;
    data.receive_length = 1;
    uint8_t reg_value;
    data.data_receive = &reg_value;
    for (uint8_t addr = 0x00; addr <= 0x04; addr++)
    {
        data.address = addr;
        int n = read_sx1278(spi_handle, &data);
        if (n < 0)
        {
            fprintf(stdout, "Failed to read register 0x%02X\n", addr);
        }
        else
        {
            fprintf(stdout, "Reg 0x%02X: 0x%02X\n", addr, reg_value);
        }
    }
}