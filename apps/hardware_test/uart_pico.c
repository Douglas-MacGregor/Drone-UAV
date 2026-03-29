#include "hal_gpio.h"
#include "hal_time.h"
#include "hal_uart.h"
#include "pico/stdlib.h"
#include <string.h>

extern HAL_GPIO hal_gpio;
extern HAL_TIME hal_time;
extern HAL_UART hal_uart;

int main()
{
    int led_pin = 15; // GPIO pin 15 for the LED
    hal_gpio.set_pin_mode(led_pin, HAL_GPIO_OUTPUT);
    hal_gpio.write_pin(led_pin, 0); // Ensure LED is off at start

    hal_gpio.init_gpio();
    hal_uart.init_uart(115200); // Initialize UART with baud rate 115200

    const char *message = "Hello, UART on Raspberry Pi Pico!\n";
    hal_uart.uart_write(0, message, strlen(message)); // Write message to UART0

    while (1)
    {
        char byte;
        if (hal_uart.uart_read_byte(0, &byte)) // Read byte from UART0
        {
            if (byte == 'O')
            {
                hal_gpio.write_pin(led_pin, 1); // Turn on the LED
            }
            else if (byte == 'F')
            {
                hal_gpio.write_pin(led_pin, 0); // Turn off the LED
            }
        }
        hal_time.delay_ms(10); // Small delay to prevent busy-waiting
    }
    hal_uart.close_uart(0); // Close UART0
    hal_gpio.close_gpio();  // Clean up GPIO
    return 0;
}