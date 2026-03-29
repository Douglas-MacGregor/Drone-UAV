#include "hal_uart.h"
#include <stdio.h>
#include "hal_gpio.h"
#include "hal_time.h"

extern HAL_UART hal_uart;
extern HAL_GPIO hal_gpio;
extern HAL_TIME hal_time;

int main()
{
    hal_gpio.init_gpio();
    int uart_handle = hal_uart.init_uart(115200); // Initialize UART with baud rate 115200

    char buffer[128];
    char byte = 0;
    while (byte != '\n' && byte != '\r' && (sizeof(buffer) - 1) > 0)
    {
        if (hal_uart.uart_read_byte(uart_handle, &byte))
        {
            buffer[strlen(buffer)] = byte; // Append byte to buffer
        }
    }
    buffer[strlen(buffer)] = '\0';    // Null-terminate the string
    printf("Received: %s\n", buffer); // Print the received message
    while (1)
    {
        hal_uart.write_byte(uart_handle, 'O'); // Send 'O' over UART
        hal_time.delay_ms(1000);               // Delay 1000ms
        hal_uart.write_byte(uart_handle, 'F'); // Send 'F' over UART
        hal_time.delay_ms(1000);               // Delay 1000ms
    }
    hal_uart.close_uart(uart_handle); // Close UART
    hal.gpio.close_gpio();            // Clean up GPIO
    return 0;
}