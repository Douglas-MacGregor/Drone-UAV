#ifndef DUALSHOCK_DEVICE_H
#define DUALSHOCK_DEVICE_H
#include <stdint.h>
#include "../drone_controller_interface.h"
#include "../../utils.h"
#include <stdio.h>
#include "dualshock_utils.h"

typedef struct
{
    DroneControllerInterface *vtable; // pointer to interface
    FILE *file_handle;
    float joystick_deadzone_percent; // File handle for reading DualShock input
    DualShockReport current_report;
} Dualshock_Device;

extern DroneControllerInterface dualshock_controller_interface;

cordirnate3D_t dualshock_get_XYZ(void *self);
uint8_t dualshock_get_button_state(void *self);
int dualshock_refresh_report(void *self);

Dualshock_Device create_dualshock_device(char *file_name, int name_length, float joystick_deadzone);
#endif // DUALSHOCK_DEVICE_H