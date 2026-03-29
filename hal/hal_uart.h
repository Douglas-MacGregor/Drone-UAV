#ifndef HAL_UART_H
#define HAL_UART_H

typedef struct
{
    int (*init_uart)();
    int (*close_uart)();
    int (*uart_available)();
    int (*uart_read_byte)(char *byte);
    int (*uart_write_byte)(char byte);
    int (*uart_write)(const char *bytes, int length);
} HAL_UART;

#endif