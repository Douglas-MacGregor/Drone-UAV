#ifndef CONTROLLER_INTERFACE_H
#define CONTROLLER_INTERFACE_H
#include <stdint.h>

typedef struct
{
    float (*get_yaw)(void *self);
    float (*get_pitch)(void *self);
    float (*get_roll)(void *self);
    float (*get_throttle)(void *self);
    uint8_t (*get_button_state)(void *self);
} ControllerInterface;

#endif // CONTROLLER_INTERFACE_H