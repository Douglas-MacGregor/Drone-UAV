#include "lgpio.h"
#include "hal_gpio.h"

static int gpio_handle = -1;

int lgpio_init_gpio()
{
    gpio_handle = lgGpiochipOpen(0); // Open GPIO chip 0 (main GPIO chip on Pi)
    if (gpio_handle < 0)
    {
        return -1;
    }
    return 0;
}

int lgpio_close_gpio()
{
    if (gpio_handle >= 0)
    {
        lgGpiochipClose(gpio_handle);
        gpio_handle = -1;
    }
    return 0;
}

int lgpio_set_pin_mode(int pin, int mode)
{
    if (gpio_handle < 0)
        return -1;

    if (lgGpioClaimOutput(gpio_handle, 0, pin, 0) != 0)
    {
        // If claim as output fails, try as input
        if (lgGpioClaimInput(gpio_handle, 0, pin) != 0)
        {
            return -1;
        }
    }
    return 0;
}

int lgpio_write_pin(int pin, int value)
{
    if (gpio_handle < 0)
        return -1;

    if (lgGpioWrite(gpio_handle, pin, value) != 0)
    {
        return -1;
    }
    return 0;
}

int lgpio_read_pin(int pin)
{
    if (gpio_handle < 0)
        return -1;

    int value = lgGpioRead(gpio_handle, pin);
    if (value < 0)
    {
        return -1;
    }
    return value;
}

HAL_GPIO hal_gpio = {
    .init_gpio = lgpio_init_gpio,
    .close_gpio = lgpio_close_gpio,
    .set_pin_mode = lgpio_set_pin_mode,
    .write_pin = lgpio_write_pin,
    .read_pin = lgpio_read_pin};