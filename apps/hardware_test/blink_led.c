#include "hal_gpio.h"
#include "pico/stdlib.h"

extern HAL_GPIO hal_gpio;

int main()
{
    hal_gpio.init_gpio();
    int led_pin = 15; // GPIO pin 15 for the LED
    hal_gpio.set_pin_mode(led_pin, HAL_GPIO_OUTPUT);

    while (1)
    {
        hal_gpio.write_pin(led_pin, 1); // Turn LED on
        sleep_ms(500);                  // Delay 500ms
        hal_gpio.write_pin(led_pin, 0); // Turn LED off
        sleep_ms(500);                  // Delay 500ms
    }

    hal_gpio.close_gpio();
    return 0;
}