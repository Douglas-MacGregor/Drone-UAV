#include "hal_gpio.h"
#include "hal_time.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

extern HAL_GPIO hal_gpio;
extern HAL_TIME hal_time;

int main()
{

    hal_gpio.init_gpio();
    uint pwm_pin = 2; // GPIO pin 18 for PWM output

    int led_pin = 15; // GPIO pin 15 for the LED
    hal_gpio.set_pin_mode(led_pin, HAL_GPIO_OUTPUT);

    gpio_set_function(pwm_pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pwm_pin);
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv(&cfg, 125.0f); // Set clock divider to 4
    pwm_config_set_wrap(&cfg, 20000);    // Set wrap value for 1 kHz frequency (assuming 125 MHz clock)
    pwm_init(slice_num, &cfg, true);     // Initialize PWM slice with enabled state
    pwm_set_gpio_level(pwm_pin, 1000);
    hal_time.delay_ms(5000);
    // 🔹 Now safe to increase throttle slightly
    pwm_set_gpio_level(pwm_pin, 1200);

    while (1)
    {
        hal_time.delay_ms(2);
        hal_gpio.write_pin(led_pin, 1); // Turn LED on
        hal_time.delay_ms(500);         // Delay 500ms
        hal_gpio.write_pin(led_pin, 0); // Turn LED off
        hal_time.delay_ms(500);         // Delay 500ms
    }
    return 0;
}