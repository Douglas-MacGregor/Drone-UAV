#include "hal_spi.h"
#include "hal_gpio.h"
#include "sx1278_utils.h"
#include <stdlib.h>
#include <stdio.h>

extern HAL_GPIO hal_gpio;
extern HAL_SPI hal_spi;

int read_sx1278(int spi_handle, uint8_t reg_address, uint8_t *value, int len)
{
    uint8_t *transmit_buffer = malloc(len + 1);
    memset(transmit_buffer, 0, len + 1);
    transmit_buffer[0] = reg_address & ~(0b10000000);
    uint8_t *receive_buffer = malloc(len + 1);
    memset(receive_buffer, 0, len + 1);
    int n = hal_spi.read_spi(spi_handle, transmit_buffer, receive_buffer, len + 1);
    memcpy(value, &receive_buffer[1], len);
    free(receive_buffer);
    free(transmit_buffer);
    if (n != len + 1)
    {
        fprintf(stdout, "SPI read error\n");
        return -1;
    }
    else
    {
        return n;
    }
}

int write_sx1278(int spi_handle, uint8_t reg_address, const uint8_t *value, int len)
{
    uint8_t *transmit_buffer = malloc(len + 1);
    memset(transmit_buffer, 0, len + 1);
    transmit_buffer[0] = reg_address | 0b10000000;
    memcpy(&transmit_buffer[1], value, len);
    int n = hal_spi.write_spi(spi_handle, transmit_buffer, len + 1);
    free(transmit_buffer);
    if (n != len + 1)
    {
        fprintf(stdout, "SPI write error\n");
        return -1;
    }
    else
    {
        return n;
    }
}

int set_bits_sx1278(int spi_handle, uint8_t reg_address, uint8_t mask)
{
    uint8_t current_value;
    if (read_sx1278(spi_handle, reg_address, &current_value, 1) < 0)
    {
        fprintf(stdout, "Failed to read register 0x%02X\n", reg_address);
        return -1;
    }
    current_value |= mask;
    if (write_sx1278(spi_handle, reg_address, &current_value, 1) < 0)
    {
        fprintf(stdout, "Failed to write register 0x%02X\n", reg_address);
        return -1;
    }
    return 0;
}

int clear_bits_sx1278(int spi_handle, uint8_t reg_address, uint8_t mask)
{
    uint8_t current_value;
    if (read_sx1278(spi_handle, reg_address, &current_value, 1) < 0)
    {
        fprintf(stdout, "Failed to read register 0x%02X\n", reg_address);
        return -1;
    }
    current_value &= ~mask;
    if (write_sx1278(spi_handle, reg_address, &current_value, 1) < 0)
    {
        fprintf(stdout, "Failed to write register 0x%02X\n", reg_address);
        return -1;
    }
    return 0;
}

int poll_register_sx1278(int spi_handle, uint8_t reg_address, uint8_t mask, uint8_t expected_value, int max_attempts, int delay_us)
{
    int i = 0;
    uint8_t reg_value;
    while (i < max_attempts)
    {
        if (read_sx1278(spi_handle, reg_address, &reg_value, 1) < 0)
        {
            fprintf(stdout, "Failed to read register 0x%02X\n", reg_address);
            return -1;
        }
        if ((reg_value & mask) == expected_value)
        {
            return 0; // Condition met
        }
        usleep(delay_us);
        i++;
    }
    return -1; // Timeout
}

int reset_sx1278(int gpio_reset_pin)
{
    // Set reset pin low
    if (hal_gpio.set_pin_mode(gpio_reset_pin, HAL_GPIO_OUTPUT) < 0)
    {
        fprintf(stderr, "Failed to set reset pin mode\n");
        return -1;
    }
    if (hal_gpio.write_pin(gpio_reset_pin, 0) < 0)
    {
        fprintf(stderr, "Failed to set reset pin low\n");
        return -1;
    }
    usleep(10000); // Hold low for 10ms
    // Set reset pin high
    if (hal_gpio.write_pin(gpio_reset_pin, 1) < 0)
    {
        fprintf(stderr, "Failed to set reset pin high\n");
        return -1;
    }
    if (hal_gpio.set_pin_mode(gpio_reset_pin, HAL_GPIO_INPUT) < 0)
    {
        fprintf(stderr, "Failed to set reset pin mode to input\n");
        return -1;
    }
    usleep(10000); // Wait for 10ms after reset
    return 0;
}