#include "hal_gpio.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

extern HAL_GPIO hal_gpio;

int main()
{

    hal_gpio.init_gpio();
    uint pwm_pin = 2; // GPIO pin 18 for PWM output
    gpio_set_function(pwm_pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pwm_pin);
    pwm_config cfg = pwm_get_default_config();
    pwm_set_clkdiv(&cfg, 4.0f);      // Set clock divider to 4
    pwm_set_wrap(&cfg, 1000);        // Set wrap value for 1 kHz frequency (assuming 125 MHz clock)
    pwm_init(slice_num, &cfg, true); // Initialize PWM slice with enabled state
    while (true)
    {
        for (int duty = 0; duty <= 1000; duty++)
        {
            pwm_set_gpio_level(pwm_pin, duty);
            sleep_ms(2);
        }

        for (int duty = 1000; duty >= 0; duty--)
        {
            pwm_set_gpio_level(pwm_pin, duty);
            sleep_ms(2);
        }
    }
    return 0;
}