#ifndef HAL_UART_H
#define HAL_UART_H

typedef struct
{
    int (*init_uart)(int baud_rate);
    int (*close_uart)(int handle);
    int (*uart_available)(int handle);
    int (*uart_read_byte)(int handle, char *byte);
    int (*uart_write_byte)(int handle, char byte);
    int (*uart_write)(int handle, const char *bytes, int length);
} HAL_UART;

#endif