#include "hal_uart.h"
#include <pigpio.h>

int init_uart(int baud_rate)
{
    int handle = serOpen("/dev/serial0", baud_rate, 0);
    if (handle < 0)
    {
        return -1; // Initialization failed
    }
    return handle; // Success
}

int close_uart(int handle)
{
    return serClose(handle);
}

int uart_available(int handle)
{
    return serDataAvailable(handle);
}

int uart_read_byte(int handle, char *byte)
{
    if (serDataAvailable(handle) > 0)
    {
        *byte = serReadByte(handle);
        return 1; // Byte read successfully
    }
    return 0; // No data available
}

int uart_write_byte(int handle, char byte)
{
    return serWriteByte(handle, byte);
}

int uart_write(int handle, const char *bytes, int length)
{
    return serWrite(handle, bytes, length);
}

HAL_UART hal_uart = {
    .init_uart = init_uart,
    .close_uart = close_uart,
    .uart_available = uart_available,
    .uart_read_byte = uart_read_byte,
    .uart_write_byte = uart_write_byte,
    .uart_write = uart_write};