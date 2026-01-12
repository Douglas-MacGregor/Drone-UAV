
#include "dualshock_device.h"
#include "dualshock_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../drone_controller_interface.h"
#include "../../utils.h"
#include <math.h>

DroneControllerInterface dualshock_controller_interface = {
    .get_XYZ = dualshock_get_XYZ,
    .get_button_state = dualshock_get_button_state,
    .refresh_report = dualshock_refresh_report};

/*
 * Function: dualshock_get_XYZ
 * Description: Retrieves the XYZ coordinates from the DualShock device
 * Parameters: self - Pointer to the Dualshock device instance
 * Returns: cordirnate3D_t structure containing X, Y, Z normalized between -1.0 and 1.0
 * Side Effects: None
 * Assumptions: current_report field in Dualshock_Device is up-to-date
 * Notes: Applies deadzone filtering to joystick inputs
 */
cordirnate3D_t dualshock_get_XYZ(void *self)
{
    Dualshock_Device *device = (Dualshock_Device *)self;
    cordirnate3D_t coords = {0.0f, 0.0f, 0.0f};
    float x = (device->current_report.right_stick_x - 128) / 128.0f;
    float y = (device->current_report.right_stick_y - 128) / 128.0f;
    float r = sqrtf(x * x + y * y);

    if (r < device->joystick_deadzone_percent)
    {
        x = 0;
        y = 0;
    }
    else
    {
        float new_r = (r - device->joystick_deadzone_percent) / (1.0f - device->joystick_deadzone_percent);
        float scale = new_r / r;
        x *= scale;
        y *= scale;
    }

    coords.x = x;
    coords.y = y;
    coords.z = ((float)device->current_report.right_trigger - (float)device->current_report.left_trigger) / 255.0f;
    return coords;
}

/*
 * Function: dualshock_get_button_state
 * Description: Retrieves the current button state from the DualShock device
 * Parameters: self - Pointer to the Dualshock device instance
 * Returns: 8-bit button state representation
 * Side Effects: None
 * Assumptions: current_report field in Dualshock_Device is up-to-date
 * Notes: Combines button states from buttons_shapes and buttons_misc fields
 *   Output format: Bit 7: Triangle, Bit 6: Circle, Bit 5: Cross, Bit 4: Square, Bit 3: reserved, Bit 2: reserved, Bit 1: R1, Bit 0: L1
 */

uint8_t dualshock_get_button_state(void *self)
{
    Dualshock_Device *device = (Dualshock_Device *)self;
    uint8_t button_state = 0;
    button_state |= (device->current_report.buttons_shapes & 0x80);
    button_state |= (device->current_report.buttons_misc & 0b11);
    return button_state;
}

/*
 * Function: dualshock_refresh_report
 * Description: Refreshes the DualShock report by reading from the device file
 * Parameters: self - Pointer to the Dualshock device instance
 * Returns: 0 on success, -1 on error
 * Assumptions: Valid file handle in device structure
 * Side Effects: Updates the current_report field in the Dualshock_Device structure
 * Notes: Uses get_dualshock_report utility function
 */
int dualshock_refresh_report(void *self)
{
    Dualshock_Device *device = (Dualshock_Device *)self;
    DualShockReport report_buffer;
    if (get_dualshock_report(device->file_handle, &report_buffer) < 0)
    {
        fprintf(stderr, "Failed to refresh DualShock report\n");
        return -1;
    }
    return 0;
}

/*
 * Function: create_dualshock_device
 * Description: Creates and initializes a DualShock device instance
 * Parameters: file_name - Path to the DualShock device file
 *             name_length - Length of the file name
 *             joystick_deadzone - Deadzone percentage for joystick inputs (0.0 to 1.0)
 * Returns: Initialized Dualshock_Device structure
 * Side Effects: Opens the device file for reading
 * Assumptions: Valid file path and deadzone value provided
 * Notes: Makes the file handle NULL if opening fails
 */

Dualshock_Device create_dualshock_device(const char *file_name, float joystick_deadzone)
{
    Dualshock_Device device = {0}; // Initialize to zero

    if (joystick_deadzone < 0.0f || joystick_deadzone > 1.0f)
    {
        fprintf(stderr, "Invalid joystick deadzone value: %f. Must be between 0.0 and 1.0\n", joystick_deadzone);
        device.file_handle = NULL; // Return invalid device
        return device;
    }

    device.vtable = &dualshock_controller_interface;
    device.joystick_deadzone_percent = joystick_deadzone;
    device.file_handle = fopen(file_name, "rb");
    if (device.file_handle == NULL)
    {
        fprintf(stderr, "Failed to open DualShock device file: %s\n", file_name);
    }
    return device;
}

int destroy_dualshock_device(Dualshock_Device *device)
{
    if (device->file_handle)
    {
        fclose(device->file_handle);
        device->file_handle = NULL;
    }
    return 0;
}