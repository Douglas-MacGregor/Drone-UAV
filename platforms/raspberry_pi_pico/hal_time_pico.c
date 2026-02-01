#include "pico/stdlib.h"
#include "hal_time.h"

int pico_init_time()
{
    return 0; // No special initialization needed
}

int pico_close_time()
{
    return 0; // No cleanup needed
}

void pico_delay_s(unsigned int s)
{
    sleep_ms(s * 1000);
}

void pico_delay_ms(unsigned int ms)
{
    sleep_ms(ms);
}

void pico_delay_us(unsigned int us)
{
    sleep_us(us);
}

unsigned long pico_get_time_ms()
{
    return to_ms_since_boot(get_absolute_time());
}

HAL_TIME hal_time = {
    .init_time = pico_init_time,
    .close_time = pico_close_time,
    .delay_ms = pico_delay_ms,
    .delay_us = pico_delay_us,
    .delay_s = pico_delay_s,
    .get_time_ms = pico_get_time_ms};