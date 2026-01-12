#include "../unity.h"
#include "../unity_internals.h"
#include "../../src/devices/Controllers/DualShock/dualshock_device.h"
#include "../../src/devices/Controllers/DualShock/dualshock_utils.h"
#include <stdlib.h>
#include <math.h>

void setUp(void)
{
    // This is run before EACH TEST
}
void tearDown(void)
{
    // This is run after EACH TEST
}

void test_dualshock_create_device(void)
{
    Dualshock_Device device = create_dualshock_device("/dev/hidraw0", 0.08f);
    TEST_ASSERT_NOT_NULL(device.file_handle);
    TEST_ASSERT_EQUAL_FLOAT(0.08f, device.joystick_deadzone_percent);
    fclose(device.file_handle);
}

void test_dualshock_create_device_invalid_deadzone(void)
{
    Dualshock_Device device1 = create_dualshock_device("/dev/hidraw0", 0.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, device1.joystick_deadzone_percent);
    destroy_dualshock_device(&device1);

    Dualshock_Device device2 = create_dualshock_device("/dev/hidraw0", 1.0f);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, device2.joystick_deadzone_percent);
    destroy_dualshock_device(&device2);

    Dualshock_Device device3 = create_dualshock_device("/dev/hidraw0", -0.1f);
    TEST_ASSERT_NULL(device3.file_handle);
    destroy_dualshock_device(&device3);

    Dualshock_Device device4 = create_dualshock_device("/dev/hidraw0", 1.1f);
    TEST_ASSERT_NULL(device4.file_handle);
    destroy_dualshock_device(&device4);
}

void test_dualshock_create_device_invalid_path(void)
{
    Dualshock_Device device = create_dualshock_device("/invalid/path", 0.08f);
    TEST_ASSERT_NULL(device.file_handle);
    destroy_dualshock_device(&device);
}

void test_dualshock_create_device_null_path(void)
{
    Dualshock_Device device = create_dualshock_device(NULL, 0.08f);
    TEST_ASSERT_NULL(device.file_handle);
    destroy_dualshock_device(&device);
}

void test_dualshock_get_button_state(void)
{
    Dualshock_Device device = create_dualshock_device("/dev/hidraw0", 0.08f);
    if (device.file_handle == NULL)
    {
        TEST_IGNORE_MESSAGE("DualShock device file could not be opened. Skipping test.");
        return;
    }

    // Simulate reading a report (this would normally be done via refresh_report)
    device.current_report.buttons_shapes = 0x80; // Example: only the first button pressed
    device.current_report.buttons_misc = 0x01;   // Example: only the first misc button pressed

    uint8_t button_state = device.vtable->get_button_state(&device);
    TEST_ASSERT_EQUAL_UINT8(0x81, button_state); // Expecting 0x80 | 0x01

    device.vtable->refresh_report(&device);
    button_state = device.vtable->get_button_state(&device);
    TEST_ASSERT_EQUAL_UINT8(0x00, button_state); // Assuming no buttons pressed in refreshed report
    destroy_dualshock_device(&device);
}

void test_dualshock_refresh_report(void)
{
    Dualshock_Device device = create_dualshock_device("/dev/hidraw0", 0.08f);
    if (device.file_handle == NULL)
    {
        TEST_IGNORE_MESSAGE("DualShock device file could not be opened. Skipping test.");
        return;
    }

    int result = device.vtable->refresh_report(&device);
    TEST_ASSERT_EQUAL_INT(0, result); // Expecting success
    int threshold = 8;                // Deadzone threshold
    TEST_ASSERT_LESS_OR_EQUAL_INT(threshold, abs(device.current_report.left_stick_x - 128));
    TEST_ASSERT_LESS_OR_EQUAL_INT(threshold, abs(device.current_report.left_stick_y - 128));
    TEST_ASSERT_LESS_OR_EQUAL_INT(threshold, abs(device.current_report.right_stick_x - 128));
    TEST_ASSERT_LESS_OR_EQUAL_INT(threshold, abs(device.current_report.right_stick_y - 128));

    TEST_ASSERT_EQUAL_INT(0, device.current_report.left_trigger);
    TEST_ASSERT_EQUAL_INT(0, device.current_report.right_trigger);

    TEST_ASSERT_EQUAL_INT(0x08, device.current_report.buttons_shapes);
    TEST_ASSERT_EQUAL_INT(0x00, device.current_report.buttons_misc);
    destroy_dualshock_device(&device);
}

void test_dualshock_get_XYZ(void)
{
    Dualshock_Device device = create_dualshock_device("/dev/hidraw0", 0.1f);
    if (device.file_handle == NULL)
    {
        TEST_IGNORE_MESSAGE("DualShock device file could not be opened. Skipping test.");
        return;
    }

    // Simulate reading a report (this would normally be done via refresh_report)
    device.current_report.right_stick_x = 200; // Example value
    device.current_report.right_stick_y = 100; // Example value
    device.current_report.left_trigger = 50;   // Example value
    device.current_report.right_trigger = 150; // Example value

    cordirnate3D_t coords = device.vtable->get_XYZ(&device);
    float expected_x = (200 - 128) / 128.0f;
    float expected_y = (100 - 128) / 128.0f;
    float r = sqrtf(expected_x * expected_x + expected_y * expected_y);
    if (r < device.joystick_deadzone_percent)
    {
        expected_x = 0;
        expected_y = 0;
    }
    else
    {
        float new_r = (r - device.joystick_deadzone_percent) / (1.0f - device.joystick_deadzone_percent);
        float scale = new_r / r;
        expected_x *= scale;
        expected_y *= scale;
    }
    float expected_z = (150.0f - 50.0f) / 255.0f;

    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_x, coords.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_y, coords.y);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_z, coords.z);

    device.vtable->refresh_report(&device);
    coords = device.vtable->get_XYZ(&device);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, coords.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, coords.y);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, coords.z);
    destroy_dualshock_device(&device);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_dualshock_create_device);
    RUN_TEST(test_dualshock_create_device_invalid_deadzone);
    RUN_TEST(test_dualshock_create_device_invalid_path);
    RUN_TEST(test_dualshock_create_device_null_path);
    RUN_TEST(test_dualshock_get_button_state);
    RUN_TEST(test_dualshock_refresh_report);
    RUN_TEST(test_dualshock_get_XYZ);
    return UNITY_END();
}