#include "lgpio.h"
#include "hal_gpio.h"

int lgpio_init_gpio()
{
    if (lgGpioInitialise() < 0)
    {
        return -1;
    }
    return 0;
}
int lgpio_close_gpio()
{
    lgGpioTerminate();
    return 0;
}

int lgpio_set_pin_mode(int pin, int mode)
{
    if (lgGpioSetMode(pin, mode) != 0)
    {
        return -1;
    }
    return 0;
}

int lgpio_write_pin(int pin, int value)
{
    if (lgGpioWrite(pin, value) != 0)
    {
        return -1;
    }
    return 0;
}

int lgpio_read_pin(int pin)
{
    int value = lgGpioRead(pin);
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