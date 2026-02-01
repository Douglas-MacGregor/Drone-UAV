
#ifndef HAL_TIME_H
#define HAL_TIME_H

typedef struct
{
    int (*init_time)();
    int (*close_time)();
    void (*delay_s)(unsigned int s);
    void (*delay_ms)(unsigned int ms);
    void (*delay_us)(unsigned int us);
    unsigned long (*get_time_ms)();
} HAL_TIME;

#endif