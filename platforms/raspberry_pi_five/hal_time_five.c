#include <unistd.h>
#include <time.h>
#include "hal_time.h"

int five_init_time()
{
    return 0; // No special initialization needed
}
int five_close_time()
{
    return 0; // No cleanup needed
}

void five_delay_s(unsigned int s)
{
    sleep(s);
}

void five_delay_ms(unsigned int ms)
{
    usleep(ms * 1000);
}

void five_delay_us(unsigned int us)
{
    usleep(us);
}

unsigned long five_get_time_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

HAL_TIME hal_time = {
    .init_time = five_init_time,
    .close_time = five_close_time,
    .delay_ms = five_delay_ms,
    .delay_us = five_delay_us,
    .delay_s = five_delay_s,
    .get_time_ms = five_get_time_ms};