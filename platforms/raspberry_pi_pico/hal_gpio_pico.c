#include "hal_gpio.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

/*
    This file provides the Raspberry Pi Pico implementation
    of the generic HAL_GPIO interface.
*/

static int pico_gpio_set_mode(int pin, int mode)
{
    gpio_init(pin); // Initialize the GPIO pin
    if (mode == HAL_GPIO_INPUT)
        gpio_set_dir(pin, GPIO_IN);
    else
        gpio_set_dir(pin, GPIO_OUT);
    return 0;
}

static int pico_gpio_write(int pin, int value)
{
    gpio_put(pin, value ? 1 : 0);
    return 0;
}

static int pico_gpio_read(int pin)
{
    return gpio_get(pin);
}

static int pico_gpio_init()
{
    stdio_init_all();
    return 0;
}

static int pico_gpio_close()
{
    // Cleanup code for Raspberry Pi Pico GPIO can be added here if needed
    return 0;
}

HAL_GPIO hal_gpio = {
    .init_gpio = pico_gpio_init,   // Initialization not required for Pico GPIO
    .close_gpio = pico_gpio_close, // Cleanup not required for Pico GPIO
    .set_pin_mode = pico_gpio_set_mode,
    .write_pin = pico_gpio_write,
    .read_pin = pico_gpio_read};
