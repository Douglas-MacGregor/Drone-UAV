#include "../unity.h"
#include "../unity_internals.h"
#include "../../src/devices/Controllers/DualShock/dualshock_device.h"
#include "../../src/devices/Controllers/DualShock/dualshock_utils.h"

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
    Dualshock_Device device = create_dualshock_device("/dev/input/js0", 15, 0.1f);
    TEST_ASSERT_NOT_NULL(device.file_handle);
    TEST_ASSERT_EQUAL_FLOAT(0.1f, device.joystick_deadzone_percent);
    fclose(device.file_handle);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_dualshock_create_device);
    return UNITY_END();
}