#ifndef CONTROLLER_INTERFACE_H
#define CONTROLLER_INTERFACE_H
#include <stdint.h>
#include "coordinate.h"

typedef struct
{
    coordinate3D_t (*get_XYZ)(void *self);
    uint8_t (*get_button_state)(void *self);
    int (*refresh_report)(void *self);
} DroneControllerInterface;

#endif // CONTROLLER_INTERFACE_H