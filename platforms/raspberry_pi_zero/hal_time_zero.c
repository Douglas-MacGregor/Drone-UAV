#include <unistd.h>
#include <time.h>
#include "hal_time.h"

int zero_init_time()
{
    return 0; // No special initialization needed
}
int zero_close_time()
{
    return 0; // No cleanup needed
}

void zero_delay_s(unsigned int s)
{
    sleep(s);
}

void zero_delay_ms(unsigned int ms)
{
    usleep(ms * 1000);
}

void zero_delay_us(unsigned int us)
{
    usleep(us);
}

unsigned long zero_get_time_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

HAL_TIME hal_time = {
    .init_time = zero_init_time,
    .close_time = zero_close_time,
    .delay_ms = zero_delay_ms,
    .delay_us = zero_delay_us,
    .delay_s = zero_delay_s,
    .get_time_ms = zero_get_time_ms};