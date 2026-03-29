#include "hal_uart.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

int init_uart(int baud_rate)
{
    int gpio_tx_pin = 0; // GPIO pin for UART TX
    int gpio_rx_pin = 1; // GPIO pin for UART RX
    gpio_set_function(0, UART_FUNCSEL_NUM(uart0, 0));
    gpio_set_function(1, UART_FUNCSEL_NUM(uart0, 1));
    uart_init(uart0, baud_rate);
    return 0;
}

int close_uart(int handle)
{
    if (handle == 0)
    {
        uart_deinit(uart0);
        return 0;
    }
    else if (handle == 1)
    {
        uart_deinit(uart1);
        return 0;
    }

    return -1;
}

int uart_available(int handle)
{
    if (handle == 0)
        return uart_is_readable(uart0);
    else if (handle == 1)
        return uart_is_readable(uart1);

    return -1;
}

int uart_read_byte(int handle, char *byte)
{
    if (handle == 0 && uart_is_readable(uart0))
    {
        *byte = uart_getc(uart0);
        return 1; // Byte read successfully
    }
    else if (handle == 1 && uart_is_readable(uart1))
    {
        *byte = uart_getc(uart1);
        return 1; // Byte read successfully
    }

    return 0; // No data available
}

int uart_write_byte(int handle, char byte)
{
    if (handle == 0)
    {
        uart_putc(uart0, byte);
        return 1;
    }
    else if (handle == 1)
    {
        uart_putc(uart1, byte);
        return 1;
    }

    return -1; // Invalid handle
}

int uart_write(int handle, const char *bytes, int length)
{
    if (handle == 0)
    {
        uart_write_blocking(uart0, bytes, length);
        return length;
    }
    else if (handle == 1)
    {
        uart_write_blocking(uart1, bytes, length);
        return length;
    }

    return -1; // Invalid handle
}

HAL_UART hal_uart = {
    .init_uart = init_uart,
    .close_uart = close_uart,
    .uart_available = uart_available,
    .uart_read_byte = uart_read_byte,
    .uart_write_byte = uart_write_byte,
    .uart_write = uart_write};