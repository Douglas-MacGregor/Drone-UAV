#include "hal_gpio.h"
#include "hal_time.h"

extern HAL_GPIO hal_gpio;
extern HAL_TIME hal_time;

int main()
{
    hal_gpio.init_gpio();
    int led_pin = 15; // GPIO pin 15 for the LED
    hal_gpio.set_pin_mode(led_pin, HAL_GPIO_OUTPUT);

    while (1)
    {
        hal_gpio.write_pin(led_pin, 1); // Turn LED on
        hal_time.delay_ms(500);         // Delay 500ms
        hal_gpio.write_pin(led_pin, 0); // Turn LED off
        hal_time.delay_ms(500);         // Delay 500ms
    }

    hal_gpio.close_gpio();
    return 0;
}