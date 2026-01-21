
#include "hal_gpio.h"
#include <pigpio.h>
#include <stdio.h>

/*
    This file provides the Raspberry Pi implementation
    of the generic HAL_GPIO interface.
*/

/* ---------- Internal functions ---------- */

static int pi_gpio_set_mode(int pin, int mode)
{
    if (mode == HAL_GPIO_INPUT)
        return gpioSetMode(pin, PI_INPUT);
    else
        return gpioSetMode(pin, PI_OUTPUT);

    return 0;
}

static int pi_gpio_write(int pin, int value)
{
    return gpioWrite(pin, value ? 1 : 0);
}

static int pi_gpio_read(int pin)
{
    return gpioRead(pin);
}

static int pi_gpio_init()
{
    if (gpioInitialise() < 0)
    {
        return -1; // Initialization failed
    }
    return 0; // Success
}

static int pi_gpio_close()
{
    gpioTerminate();
    return 0; // Success
}

/* ---------- Exported HAL object ---------- */

HAL_GPIO hal_gpio = {
    .init_gpio = pi_gpio_init,
    .close_gpio = pi_gpio_close,
    .set_pin_mode = pi_gpio_set_mode,
    .write_pin = pi_gpio_write,
    .read_pin = pi_gpio_read};